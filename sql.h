#ifndef SQL_H
#define SQL_H

#include"initializators_help.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QDialog>
#include <condition_variable>
#include <mutex>
#include "formmaster.h"

namespace sql {
class SQLWorker : public std::enable_shared_from_this<sql::SQLWorker> {
public:
    SQLWorker(const json_obj& obj);

    void CreateMaster();


private:
    friend class FormMaster;
    QueryExecute(QSqlQuery&quer);

    //Создание объекта
    void FirstConnect();
    bool HasDatabase();
    void CreateDatabase();
    void BuildBasesAndTables();
    void ReconnectToCreatedTable();
    void CreateRoles();
    void CreateUsersBase();
    void LoadRoles();
    void LoadUsers();



    QSqlDatabase _base;
    std::condition_variable _cv;

    str_type
        _host,
        _user_name,
        _password,
        _table_name;
    int _port;

    std::map<int, str_type> _roles_id;
    std::map<str_type,int> _id_roles;

    std::map<str_type, UserRole> _user_passhash;
    std::mutex _mtx;

    str_type _admin;
    str_type _admin_pass;
    bool _has_master = false;
};

}//NAMESPACE

#endif // SQL_H
