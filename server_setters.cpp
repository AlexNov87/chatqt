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

json_obj GraphicsServer::JoinRoomUserJs(str_type name, str_type password,
                                        str_type roomname, SocketComplect* complect) {

    ACTIONS this_act = ACTIONS::JOIN_ROOM;
    auto lam = [&]{

        if(auto res =_sql_work->AuthorizatedError
                       (name, password, this_act)){
            return  *res;
        }

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
