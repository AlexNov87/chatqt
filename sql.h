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
QSqlQuery QueryPreparedToIsertUser( const str_type& name,
                                   const str_type& pass, int id);

QSqlQuery QueryPreparedToDeleteUser(const str_type& name);

QSqlQuery QueryUpdateUserRole(const str_type& name, int role_id);
QSqlQuery QueryUpdateUserActiveStatus(const str_type& name, bool is_active);

class SQLWorker : public std::enable_shared_from_this<sql::SQLWorker> {
public:

    SQLWorker(const json_obj& obj);
    json_obj RegisterNewUser(str_type name, str_type pass);
    json_obj DeleteUser(str_type name, str_type password, str_type to_delete);
    std::optional<json_obj> AuthorizatedError
        (str_type name, str_type password, ACTIONS act);
    std::optional<json_obj> AuthorizatedError
        (str_type name, str_type password, ADMIN_ACTIONS act);
    bool UpdateMaster(str_type name, str_type pass);
    json_obj UpdateUserRole(str_type name, Role role, str_type initiator);
    json_obj BanUser(str_type name , str_type initiator);
    json_obj UnbanUser(str_type name , str_type initiator);
    bool IsBanned(str_type name);
    const std::pair<str_type, str_type>& GetMaster(){
        return _master;
    }
    const std::map<str_type, UserRole>& AllUsersBase(){
        LGR(_mtx);
        return _user_passhash;
    }

    str_type _serializated_users;
    bool _is_users_cached;
    const str_type& GetSerializedUsers();

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
    void CreateMaster();

    QSqlDatabase _base;


    str_type
        _host,
        _user_name,
        _password,
        _table_name;
    int _port;

    std::map<int, str_type> _id_roles;
    std::map<str_type,int> _roles_id;

    std::map<str_type, UserRole> _user_passhash;
    std::set<str_type> _banned;
    std::recursive_mutex _mtx;

    bool _has_master = false;
    std::pair<str_type, str_type> _master;
};

}//NAMESPACE

#endif // SQL_H
