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
    ACTIONS this_act = ACTIONS::DELETE_USER;
    if(!IsUserLogined(name, password)){
        return ans_obj::MakeErrorObject("You are not logined"
                                        , this_act );
    }
    if(!_user_passhash.contains(to_delete)){
        return ans_obj::MakeErrorObject("User who must be deleted is not in base"
                                        , this_act);
    }

    if(name != to_delete){
        if(_user_passhash.at(name).role <= _user_passhash.at(to_delete).role){
            return ans_obj::MakeErrorObject("Your rank is not allowed you to delete this user"
                                            , this_act);
        }
    }

    QSqlQuery quer = QueryPreparedToDeleteUser(name);
    if(!quer.exec()){
        return ans_obj::MakeErrorObject("Failed to delete user on SQL"
                                        , this_act);
    }

    return ans_obj::SuccessDeleteUser(to_delete);
}

bool SQLWorker::IsUserLogined(str_type name, str_type password){
    LGR(_mtx);
    if(!_user_passhash.contains(name)){
        return false;
    }
    if(_user_passhash.at(name).password != password){
        return false;
    }
    return true;
}

}
