#include "mainwindow.h"
//////////////////////!!!!!!!!!!!!!!!!!!!!!!
json_obj GraphicsServer::AddRoomJs(str_type name,
                                   str_type password, str_type roomname)  {

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::CREATE_ROOM;
    auto lam = [&]{

        //Если нет добавляющего в базе
        if(_sql_work->IsAuthorizated(name, password)){
            return  ans_obj::MakeAdminErrorObject
                ("You are not authorizated", this_act);
        }

        if(!HasPermissionAdmin(name, password, this_act)){
            return  ans_obj::MakeAdminErrorObject
                ("You have no permission to create room", this_act);
        }
        {
            LGR(_mtx_room);
            bool res =  _rooms.insert( {roomname , std::make_shared<ChatRoom>(this, name, roomname)}).second;
            if(!res){
                return ans_obj::MakeAdminErrorObject
                    ("Can not insert room", this_act);
            }
            //добавляем в ф орму комнату
            res = this->_rooms_form->AddRoomTolist(roomname);
            if(!res){
                _rooms.erase(roomname);
                return ans_obj::MakeAdminErrorObject
                    ("Can not insert room to visual form", this_act);
            }
        }
        return ans_obj::AdminSuccessCreateRoom(std::move(roomname));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}
//!!!!!!!!!!!!!!!!as2s1212
json_obj GraphicsServer::DeleteRoomJs(str_type name, str_type password, str_type roomname)  {

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::DELETE_ROOM;
    auto lam = [&]{

        //Если нет удаляющего в базе
        if(!_sql_work->IsAuthorizated(name, password)){
            return  ans_obj::MakeAdminErrorObject
                ("You are not authorizated", this_act);
        }

        LGR(_mtx_room);
        //Если нет комнаты
        if(!_rooms.contains(roomname)){
            return  ans_obj::MakeAdminErrorObject
                ("No room named " + roomname , this_act);
        }

        //Пока не даем комнате разрушиться
        auto rm = _rooms.at(roomname);

        //Если юзер не создавал комнату проверка прав на удаление
        if(rm->GetCreator() != name){
            if(!HasPermissionAdmin(name, password, this_act)){
                return  ans_obj::MakeAdminErrorObject
                    ("You have no permission to delete room", this_act);
            }
        }
        //удаляем формы комнату
        this->_rooms_form->RemoveRoomFromList(roomname);
        _rooms.erase(roomname);
        return ans_obj::AdminSuccessDeleteRoom(std::move(roomname));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::DeleteUserJs(str_type name, str_type password, str_type to_delete) {

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::DELETE_USER;
    auto lam = [&]{
        return this->_sql_work->DeleteUser(name, password, to_delete);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}



