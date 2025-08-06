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

    QSqlQuery quer = QueryPreparedToDeleteUser(name);
    if(!quer.exec()){
        return ans_obj::MakeAdminErrorObject("Failed to delete user on SQL"
                                        , this_act);
    }

    return ans_obj::AdminSuccessDeleteUser(to_delete);
}

std::optional<json_obj> SQLWorker::AuthorizatedError
    (str_type name, str_type password, ADMIN_ACTIONS act){
    LGR(_mtx);
    if(!_user_passhash.contains(name)){
        ans_obj::MakeAdminErrorObject("User not found", act);
    }
    if(!_user_passhash.at(name).is_active){
        ans_obj::MakeAdminErrorObject("You are banned on this_server", act);
    }
    if(_user_passhash.at(name).password != password){
        ans_obj::MakeAdminErrorObject("Wrong password", act);
    }

    return std::nullopt;
}

std::optional<json_obj> SQLWorker::AuthorizatedError
    (str_type name, str_type password, ACTIONS act){
    LGR(_mtx);
    if(!_user_passhash.contains(name)){
        ans_obj::MakeErrorObject("User not found", act);
    }
    if(!_user_passhash.at(name).is_active){
        ans_obj::MakeErrorObject("You are banned on this_server", act);
    }
    if(_user_passhash.at(name).password != password){
        ans_obj::MakeErrorObject("Wrong password", act);
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
json_obj SQLWorker::UpdateUserRole(str_type name, Role role)
{
    LGR(_mtx);
    const auto& rolename = _ROLE_NAME.at(role);
    auto query = QueryUpdateUserRole(name, _roles_id.at(rolename));

  //  return query.exec();
}
json_obj SQLWorker::BanUser(str_type name){
    LGR(_mtx);
    auto query = QueryUpdateUserActiveStatus(name,false);
    //return query.exec();
}
json_obj SQLWorker::UnbanUser(str_type name){
    LGR(_mtx);
    auto query = QueryUpdateUserActiveStatus(name, true);
    //return query.exec();
}



}//namespace sql
