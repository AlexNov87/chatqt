#include"sql.h"

namespace sql {

json_obj SQLWorker::RegisterNewUser(str_type name, str_type pass){
    LGR(_mtx);
    if(_user_passhash.contains(name)){
        return ans_obj::MakeErrorObject("User already registered"
                                        , ACTIONS::CREATE_USER);
    }

    QSqlQuery quer = QueryPreparedToIsertUser
        (name, pass, _id_roles.at(CONSTANTS::ROLE_USER));
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
    if(!IsAuthorizated(name, password)){
        return ans_obj::MakeAdminErrorObject("You are not logined"
                                        , this_act );
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

bool SQLWorker::IsAuthorizated(str_type name, str_type password){
    LGR(_mtx);
    if(!_user_passhash.contains(name)){
        return false;
    }
    if(_user_passhash.at(name).password != password){
        return false;
    }
    return true;
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
    quer.bindValue(":rd", _id_roles.at(CONSTANTS::ROLE_MASTER));
    return quer.exec();
}


}//namespace sql
