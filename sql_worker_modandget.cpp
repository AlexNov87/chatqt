#include"sql.h"

namespace sql {

json_obj SQLWorker::RegisterNewUser(str_type name, str_type pass){
    LGR(_mtx);
    if(_user_passhash.contains(name)){
        return ans_obj::MakeErrorObject("User already registered"
                                        , ACTIONS::CREATE_USER);
    }

    QSqlQuery quer = QueryPreparedToIsertUser
        (name, pass, _roles_id.at(CONSTANTS::ROLE_USER));
    if(!quer.exec()){
        return ans_obj::MakeErrorObject("Failed Register new user"
                                        , ACTIONS::CREATE_USER);
    }
     _is_users_cached = false;
    _user_passhash.insert({name , UserRole{pass, Role::USER}});
    return ans_obj::SuccessCreateUser(name);
}

json_obj SQLWorker::DeleteUser(str_type name, str_type password, str_type to_delete){
    LGR(_mtx);
    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::DELETE_USER;
    if(auto res = AuthorizatedError(name, password, this_act)){
        return *res;
    }
    if(!_user_passhash.contains(to_delete)){
        return ans_obj::MakeAdminErrorObject("User who must be deleted is not in base"
                                        , this_act);
    }

    if(name != to_delete){
        if(_user_passhash.at(name).role <= _user_passhash.at(to_delete).role){
            return ans_obj::MakeAdminErrorObject("Your rank is not allowed you to delete this user"
                                            , this_act);
        }
    }

    QSqlQuery quer = QueryPreparedToDeleteUser(to_delete);
    if(!quer.exec()){
        return ans_obj::MakeAdminErrorObject("Failed to delete user on SQL"
                                        , this_act);
    }
    _is_users_cached = false;
    _user_passhash.erase(to_delete);
    return ans_obj::AdminSuccessDeleteUser(to_delete);
}

std::optional<json_obj> SQLWorker::AuthorizatedError
    (str_type name, str_type password, ADMIN_ACTIONS act){

    if(!_user_passhash.contains(name)){
      return  ans_obj::MakeAdminErrorObject("User not found", act);
    }

    if(!_user_passhash.at(name).is_active){
      return  ans_obj::MakeAdminErrorObject("You are banned on this_server", act);
    }
    if(_user_passhash.at(name).password != password){
      return ans_obj::MakeAdminErrorObject("Wrong password", act);
    }
    return std::nullopt;
}

std::optional<json_obj> SQLWorker::AuthorizatedError
    (str_type name, str_type password, ACTIONS act){
    LGR(_mtx);
    if(!_user_passhash.contains(name)){
      return  ans_obj::MakeErrorObject("User not found", act);
    }
    if(!_user_passhash.at(name).is_active){
      return   ans_obj::MakeErrorObject("You are banned on this_server", act);
    }
    if(_user_passhash.at(name).password != password){
       return  ans_obj::MakeErrorObject("Wrong password", act);
    }
    return std::nullopt;
}

bool SQLWorker::UpdateMaster(str_type name, str_type pass){
    str_type umast = R"(
        UPDATE users SET name = :name ,
        password = :pass  WHERE role_id = :rd
        )";
    QSqlQuery quer;
    quer.prepare(umast);
    quer.bindValue(":pass", pass);
    quer.bindValue(":name", name);
    quer.bindValue(":rd", _roles_id.at(CONSTANTS::ROLE_MASTER));
    return quer.exec();
}

bool SQLWorker::IsBanned(str_type name){
    LGR(_mtx);
    if(!_user_passhash.contains(name)){
        return false;
    }
    if(!_user_passhash.at(name).is_active){
        return true;
    }
    return false;
}

json_obj SQLWorker::UpdateUserRole(str_type name, Role role, str_type initiator)
{
    LGR(_mtx);
    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::UPDATE_ROLE;
    if(!_user_passhash.contains(name) || !_user_passhash.contains(initiator)){
        return ans_obj::MakeAdminErrorObject
            ("Could not find user or initiator", this_act);
    }

    if(_user_passhash.at(initiator).role <= _user_passhash.at(name).role){
        return ans_obj::MakeAdminErrorObject
            ("You have no permission to modify role of this user", this_act );
    }
    const auto& rolename = _ROLE_NAME.at(role);
    auto query = QueryUpdateUserRole(name, _roles_id.at(rolename));
    if(query.exec()){
        _user_passhash.at(name).role = role;
        return ans_obj::AdminSuccessUpdateRoleUser();
    }
    return ans_obj::MakeAdminErrorObject
        ("Role was not updated", this_act);

}

json_obj SQLWorker::BanUser(str_type name, str_type initiator){
    LGR(_mtx);
    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::BAN_USER;
    if(!_user_passhash.contains(name) || !_user_passhash.contains(initiator)){
        return ans_obj::MakeAdminErrorObject
            ("Could not find user or initiator", this_act);
    }

    if(_user_passhash.at(initiator).role <= _user_passhash.at(name).role){
        return ans_obj::MakeAdminErrorObject
            ("You have no permission to ban this user", this_act);
    }
    auto query = QueryUpdateUserActiveStatus(name,false);
    if(query.exec()){
        return ans_obj::AdminSuccessBanUser();
        this->_user_passhash.at(name).is_active = false;
    }
    return ans_obj::MakeAdminErrorObject
        ("Failed to ban this user", this_act);

}

json_obj SQLWorker::UnbanUser(str_type name, str_type initiator){
    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::UNBAN_USER;
    LGR(_mtx);
    if(!_user_passhash.contains(name) || !_user_passhash.contains(initiator)){
        return ans_obj::MakeAdminErrorObject
            ("Could not find user or initiator", this_act);
    }

    if(_user_passhash.at(initiator).role <= _user_passhash.at(name).role){
        return ans_obj::MakeAdminErrorObject
            ("You have no permission to unban this user", this_act);
    }
    auto query = QueryUpdateUserActiveStatus(name, true);
    if(query.exec()){
        return ans_obj::AdminSuccessUnbanUser();
        this->_user_passhash.at(name).is_active = true;
    }
    return ans_obj::MakeAdminErrorObject
        ("Failed to unban this user", this_act);

}

const str_type& SQLWorker::GetSerializedUsers(){
    json_arr array;
    LGR(_mtx);
    if(_is_users_cached){return _serializated_users;}
    for(auto&& user : _user_passhash)
    {
        const str_type& str = user.first;
        json_obj obj;
        obj.insert(CONSTANTS::LF_NAME, user.first);
        auto role = user.second.role;
        obj.insert(CONSTANTS::LF_ROLE, _ROLE_NAME.at(role));
        array.push_back(std::move(obj));
    }
    _serializated_users = json::WritetoQByteAnyJson(array);
    _is_users_cached = true;
    return _serializated_users;
}

str_type SQLWorker::GetSerializedUsersPredicate(const str_type& predicate){
    json_arr array;
    {
     LGR(_mtx);
    for(auto&& user : _user_passhash)
    {
        const str_type& str = user.first;
        if(!str.contains(predicate)){continue;}
        json_obj obj;
        obj.insert(CONSTANTS::LF_NAME, user.first);
        auto role = user.second.role;
        obj.insert(CONSTANTS::LF_ROLE, _ROLE_NAME.at(role));
        array.push_back(std::move(obj));
    }
    }
    return json::WritetoQByteAnyJson(array);
}

}//namespace sql
