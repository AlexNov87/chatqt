#include "mainwindow.h"
//////////////////////!!!!!!!!!!!!!!!!!!!!!!
json_obj GraphicsServer::AddRoomJs(str_type name,
                                   str_type password, str_type roomname)  {

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::CREATE_ROOM;
    if(auto res = AuthorizatedAndHasPermissionAdmin
        (name, password,this_act)){
        return *res;
    }

    auto lam = [&]{
            LGR(_mtx_room);
            bool res =  _rooms.insert( {roomname , std::make_shared<ChatRoom>(this, name, roomname)}).second;
            if(!res){
                return ans_obj::MakeAdminErrorObject
                    ("Can not insert room", this_act);
            }
            if(!res){
                _rooms.erase(roomname);
                return ans_obj::MakeAdminErrorObject
                    ("Can not insert room to visual form", this_act);
            }
        return ans_obj::AdminSuccessCreateRoom(std::move(roomname));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}
//!!!!!!!!!!!!!!!
json_obj GraphicsServer::DeleteRoomJs(str_type name, str_type password, str_type roomname)  {

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::DELETE_ROOM;
    if(auto res = AuthorizatedAndHasPermissionAdmin(name, password,this_act)){
        return *res;
    }

    auto lam = [&]{
        LGR(_mtx_room);
        //Если нет комнаты
        if(!_rooms.contains(roomname)){
            return  ans_obj::MakeAdminErrorObject
                ("No room named " + roomname , this_act);
        }
        //удаляем формы комнату
        _rooms.erase(roomname);
        return ans_obj::AdminSuccessDeleteRoom(std::move(roomname));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::DeleteUserJs(str_type name, str_type password, str_type to_delete) {

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::DELETE_USER;
    if(auto res = AuthorizatedAndHasPermissionAdmin(name, password,this_act)){
        return *res;
    }
    auto lam = [&]{
        return this->_sql_work->DeleteUser(name, password, to_delete);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::UpdateUserRoleJs(str_type name, str_type password,
                          str_type to_update, Role role){
    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::UPDATE_ROLE;
    if(auto res = AuthorizatedAndHasPermissionAdmin(name, password,this_act)){
        return *res;
    }
    auto lam = [&]{
        return this->_sql_work->UpdateUserRole(to_update, role);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::BanUserJs(str_type name, str_type password,
                   str_type to_ban){

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::BAN_USER;
    if(auto res = AuthorizatedAndHasPermissionAdmin(name, password,this_act)){
        return *res;
    }
    auto lam = [&]{
        return this->_sql_work->BanUser(to_ban);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::UnBanUserJs(str_type name, str_type password,
                     str_type to_ban){
    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::UNBAN_USER;
    if(auto res = AuthorizatedAndHasPermissionAdmin(name, password,this_act)){
        return *res;
    }
    auto lam = [&]{
        return this->_sql_work->UnbanUser(to_ban);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}



