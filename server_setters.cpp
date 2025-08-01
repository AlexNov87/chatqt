#include "mainwindow.h"
json_obj GraphicsServer::SetIP(str_type ip)  {
    ACTIONS this_act = ACTIONS::SYSTEM;
    auto lam = [&]{
        if (!IsCorrectIP(ip)){ return ans_obj::MakeErrorObject
                ("Not correct IP", this_act);}

        QLineEdit *le_ip = _maiwin->ui->le_setip;
        {
            LG(this->_mtx_net);
            _ip.setAddress(ip);
            le_ip->setText(ip);
        }
        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::SetPort(int port)  {

    ACTIONS this_act = ACTIONS::SYSTEM;
    auto lam = [&]{
        if(!IsCorrectPort(port)){return ans_obj::MakeErrorObject
                ("Not correct Port", ACTIONS::SYSTEM);}
        {
            LG(this->_mtx_net);
            _port = port;
            QSpinBox *sb_port = _maiwin->ui->sb_port;
            sb_port->setValue(port);
        }

        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}


json_obj GraphicsServer::SetMaxUsers(int max) {

    ACTIONS this_act = ACTIONS::SYSTEM;
    auto lam = [&]{
        if(!IsCorrectConnections(max)){return ans_obj::MakeErrorObject
                ("Not correct Max Users", ACTIONS::SYSTEM);}
        {
            LG(this->_mtx_net);
            _max_conn = max;
            QSpinBox *sb_maxconn = _maiwin->ui->sb_maxconn;
            sb_maxconn->setValue(max);
        }

        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

//////////////////////!!!!!!!!!!!!!!!!!!!!!!
json_obj GraphicsServer::AddRoomJs(str_type name,
 str_type password, str_type roomname)  {

    ACTIONS this_act = ACTIONS::CREATE_ROOM;
    auto lam = [&]{

        //Если нет добавляющего в базе
        if(_sql_work->IsAuthorizated(name, password)){
            return  ans_obj::MakeErrorObject
                ("You are not authorizated", this_act);
        }

        if(!HasPermission(name, password, this_act)){
            return  ans_obj::MakeErrorObject
                ("You have no permission to create room", this_act);
        }
        {
            LGR(_mtx_room);
            bool res =  _rooms.insert( {roomname , std::make_shared<ChatRoom>(this, name, roomname)}).second;
            if(!res){
                return ans_obj::MakeErrorObject
                    ("Can not insert room", this_act);
            }
            //добавляем в ф орму комнату
            res = this->_rooms_form->AddRoomTolist(roomname);
            if(!res){
                _rooms.erase(roomname);
                return ans_obj::MakeErrorObject
                    ("Can not insert room to visual form", this_act);
            }
        }
        return ans_obj::SuccessCreateUser(std::move(roomname));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}
//!!!!!!!!!!!!!!!!as2s1212
json_obj GraphicsServer::DeleteRoomJs(str_type name, str_type password, str_type roomname)  {

    ACTIONS this_act = ACTIONS::DELETE_ROOM;
    auto lam = [&]{

        //Если нет удаляющего в базе
        if(!_sql_work->IsAuthorizated(name, password)){
            return  ans_obj::MakeErrorObject
                ("You are not authorizated", this_act);
        }

        LGR(_mtx_room);
        //Если нет комнаты
        if(!_rooms.contains(roomname)){
            return  ans_obj::MakeErrorObject
                ("No room named " + roomname , this_act);
        }

        //Пока не даем комнате разрушиться
        auto rm = _rooms.at(roomname);

        //Если юзер не создавал комнату проверка прав на удаление
        if(rm->GetCreator() != name){
            if(!HasPermission(name, password, this_act)){
                return  ans_obj::MakeErrorObject
                    ("You have no permission to delete room", this_act);
            }
        }

        //удаляем формы комнату
         this->_rooms_form->RemoveRoomFromList(roomname);
        _rooms.erase(roomname);
        return ans_obj::SuccessDeleteRoom(std::move(roomname));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}

json_obj GraphicsServer::JoinRoomUserJs(str_type name, str_type password,
                                        str_type roomname, SocketComplect* complect) {

    ACTIONS this_act = ACTIONS::JOIN_ROOM;
    auto lam = [&]{

        //Если юзера нет в базе
        if(!_sql_work->IsAuthorizated(name,password)){return ans_obj::MakeErrorObject
                ("You are not authorizated" , this_act);}

        LGR(_mtx_room);
        if(!_rooms.contains(roomname)){
            return ans_obj::MakeErrorObject
                ("The is no room:"+ roomname, this_act);
        }

        std::shared_ptr<ChatRoom> room = _rooms.at(roomname);
        str_type token = str_type(this->_token_generator.GenerateHEXToken().data());

        std::shared_ptr<ChatUser> new_user =
            std::make_shared<ChatUser>(std::move(name), std::move(token) ,complect);

        return room->AddUser(new_user);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}



json_obj GraphicsServer::RegisterUserJs(str_type name, str_type password) {

    ACTIONS this_act = ACTIONS::CREATE_USER;
    auto lam = [&]{

        return this->_sql_work->RegisterNewUser
            (std::move(name), std::move(password));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::DeleteUserJs(str_type name, str_type password, str_type to_delete) {

    ACTIONS this_act = ACTIONS::DELETE_USER;
    auto lam = [&]{
        return this->_sql_work->DeleteUser(name, password, to_delete);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

