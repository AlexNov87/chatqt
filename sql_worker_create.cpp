#include "sql.h"

namespace sql {

SQLWorker::SQLWorker(const json_obj& obj) :
    _host(obj.value(LOAD_CONSTANTS::DB_HOST).toString()),
    _user_name(obj.value(LOAD_CONSTANTS::DB_USER).toString()),
    _password(obj.value(LOAD_CONSTANTS::DB_PASSWORD).toString()),
    _table_name(obj.value(LOAD_CONSTANTS::DB_TABLE_NAME).toString()),
    _port(obj.value(LOAD_CONSTANTS::DB_PORT).toInt())
{
    BuildBasesAndTables();
    LoadRoles();
    LoadUsers();
    CreateMaster();
}

void SQLWorker::FirstConnect(){
    _base = QSqlDatabase::addDatabase("QPSQL");
    _base.setHostName(_host);
    _base.setUserName(_user_name);
    _base.setPort(_port);
    _base.setPassword(_password);
    if(!_base.open()){
        Fatal(_base.lastError().text());
    }
}

bool SQLWorker::HasDatabase(){

    QSqlQuery query(_base);
    bool query_res = query.prepare
                     ("SELECT * FROM pg_database WHERE datname = :table_name ");
    if (!query_res) {
        Fatal(_base.lastError().text());
    }
    query.bindValue(":table_name", _table_name);
    return static_cast<bool>(QueryExecute(query));
}

int SQLWorker::QueryExecute(QSqlQuery&quer){
    if(!quer.exec()){
        Fatal(_base.lastError().text());
    }
    return static_cast<int>(quer.size());
}

void SQLWorker::CreateDatabase(){
    QSqlQuery query(_base);
    query.prepare("CREATE DATABASE "+ _table_name);
    QueryExecute(query);
}

void SQLWorker::BuildBasesAndTables(){
    FirstConnect();
    if(!HasDatabase()){
        CreateDatabase();
    }
    ReconnectToCreatedTable();
    CreateRoles();
    CreateUsersBase();
}

void SQLWorker::ReconnectToCreatedTable(){
    _base.close();
    _base.setDatabaseName(_table_name);
    if(!_base.open()){
        Fatal(_base.lastError().text());
    }
}

void SQLWorker::CreateRoles(){
    QSqlQuery quer;
    str_type table = R"(
         CREATE TABLE IF NOT EXISTS roles
         (
          id SERIAL PRIMARY KEY,
          role VARCHAR(30) UNIQUE NOT NULL
          )
        )";
    quer.prepare(table);
    QueryExecute(quer);

    str_type insert_role = R"(
        INSERT INTO roles (role)
               VALUES (:role)
          ON CONFLICT (role) DO NOTHING
        )";
    quer.prepare(insert_role);

    for(auto && [rl, _] : _NAME_ROLE){
        quer.bindValue(":role" , rl );
        QueryExecute(quer);
    };
}

void SQLWorker::CreateUsersBase(){
    QSqlQuery quer;
    str_type user_table = R"(
     CREATE TABLE IF NOT EXISTS users (
     id SERIAL PRIMARY KEY,
     name VARCHAR(50) UNIQUE NOT NULL ,
     password VARCHAR(50) NOT NULL,
     is_active BOOLEAN NOT NULL,
     role_id INTEGER NOT NULL,

    CONSTRAINT fk_role
       FOREIGN KEY (role_id)
       REFERENCES roles(id)
       ON DELETE SET NULL
    );
       )";
    quer.prepare(user_table);
    QueryExecute(quer);
}


void SQLWorker::LoadRoles(){

    QSqlQuery quer;
    quer.prepare("SELECT id, role FROM roles");
    QueryExecute(quer);

    while (quer.next()) {
        int id = quer.value("id").toInt();
        str_type role = quer.value("role").toString();
        _roles_id[role] = id;
        _id_roles[id] = std::move(role);
    }
}

void SQLWorker::LoadUsers(){
    QSqlQuery quer;
    str_type load_users = R"(
        SELECT
        users.id, users.name,
        users.password, users.is_active,
        roles.role
        FROM users
        RIGHT JOIN roles ON users.role_id = roles.id
        WHERE users.name IS NOT NULL
        AND users.password IS NOT NULL;
    )";
    quer.prepare(load_users);
    QueryExecute(quer);

    while(quer.next()){
        str_type name =  quer.value("name").toString();
        str_type pass = quer.value("password").toString();
        str_type role_name = quer.value("role").toString();
        bool is_active = quer.value("is_active").toBool();
        if(role_name == CONSTANTS::ROLE_MASTER){
            _has_master = true;
            _master.first = name;
            _master.second = pass;
        }

        _user_passhash.insert({
            std::move(name),
            UserRole{std::move(pass), _NAME_ROLE.at(role_name), is_active}
        });        
    }
}

void SQLWorker::CreateMaster(){

    if(_has_master) {return;}
    Formmaster mst("Creating master user");
    auto result = mst.exec();

    if(result != QDialog::Accepted){
        FatalErrorMessageBox("Failed to create admin user");
    };
    QSqlQuery quer = QueryPreparedToIsertUser(
      mst.Name() , mst.Password() ,
      _roles_id.at(CONSTANTS::ROLE_MASTER)
    );
    QueryExecute(quer);
}

}//NAMESPACE SQL
