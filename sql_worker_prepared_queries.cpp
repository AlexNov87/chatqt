#include"sql.h"
namespace sql {
QSqlQuery  QueryPreparedToIsertUser(
    const str_type& name,
    const str_type& pass,
    int id){
    QSqlQuery quer;
    str_type admin = R"(
         INSERT INTO users
         (name, password, is_active ,role_id) VALUES
         ( :name , :pass , true, :idr );
        )";
    quer.prepare(admin);
    quer.bindValue(":name", name);
    quer.bindValue(":pass", pass);
    quer.bindValue(":idr", id);
    return quer;
}

QSqlQuery QueryPreparedToDeleteUser(const str_type& name){
    QSqlQuery quer;
    str_type del = "DELETE FROM users WHERE name = :nm";
    quer.prepare(del);
    quer.bindValue(":nm", name);
    return quer;
}

QSqlQuery QueryUpdateUserRole(const str_type& name, int role_id){
    QSqlQuery quer;
    str_type upd = R"(
     UPDATE users SET role_id = :role WHERE name = :name
     )";
    quer.bindValue(":name", name);
    quer.bindValue(":role", role_id);
    return quer;
}
QSqlQuery QueryUpdateUserActiveStatus(const str_type& name, bool is_active){
    QSqlQuery quer;
    str_type upd = R"(
     UPDATE users SET is_active = :act WHERE name = :name
     )";
    quer.bindValue(":name", name);
    quer.bindValue(":act", is_active);
    return quer;
}


}//namespace sql

