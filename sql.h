#ifndef SQL_H
#define SQL_H

#include"initializators_help.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


namespace sql {

class SQLWorker {
public:
    SQLWorker(const json_obj& obj) :
        _host(obj.value(LOAD_CONSTANTS::DB_HOST).toString()),
        _user_name(obj.value(LOAD_CONSTANTS::DB_USER).toString()),
        _password(obj.value(LOAD_CONSTANTS::DB_PASSWORD).toString()),
        _table_name(obj.value(LOAD_CONSTANTS::DB_TABLE_NAME).toString()),
        _port(obj.value(LOAD_CONSTANTS::DB_PORT).toInt())
    {
         BuildBasesAndTables();
    }

private:
    const str_type do_nothing = " ON CONFLICT DO NOTHING";
    QueryExecute(QSqlQuery&quer);

    //Создание объекта
    void FirstConnect();
    bool HasDatabase();
    void CreateDatabase();
    void BuildBasesAndTables();
    void ReconnectToCreatedTable();
    void CreateRoles();
    void CreateUsersBase();



    QSqlDatabase _base;
    str_type
        _host,
        _user_name,
        _password,
        _table_name;
    int _port;


};

}//NAMESPACE

#endif // SQL_H
