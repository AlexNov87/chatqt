#include "sql.h"

namespace sql {

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
     password VARCHAR(50),
     role_id INTEGER,
    CONSTRAINT fk_role
       FOREIGN KEY (role_id)
       REFERENCES roles(id)
       ON DELETE SET NULL
    );
       )";
    quer.prepare(user_table);
    QueryExecute(quer);
}

}
