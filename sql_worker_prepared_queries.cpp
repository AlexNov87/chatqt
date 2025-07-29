#include"sql.h"
namespace sql {
QSqlQuery  QueryPreparedToIsertUser(
    const str_type& name,
    const str_type& pass,
    int id){
    QSqlQuery quer;
    str_type admin = R"(
         INSERT INTO users
         (name, password, role_id) VALUES ( :name , :pass , :idr );
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

}//namespace sql

