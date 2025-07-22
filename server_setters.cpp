#include "mainwindow.h"
QJsonObject GraphicsServer::SetIP(QString ip)  {
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


QJsonObject GraphicsServer::SetPort(int port)  {

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


QJsonObject GraphicsServer::SetMaxUsers(int max) {

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

//////////////////////ROOMS
QJsonObject GraphicsServer::AddRoomJs(QString name, QString password, QString roomname)  {

    ACTIONS this_act = ACTIONS::CREATE_ROOM;
    auto lam = [&]{

        if(!HasPermission(name, password, this_act)){
            return  ans_obj::MakeErrorObject
                ("You have no permission to create room", this_act);
        }
        //Crit section
        {
            LG(_mtx_room);
            bool res =  _rooms.insert( {roomname , std::make_shared<ChatRoom>(this, name)}).second;
            if(!res){
                return ans_obj::MakeErrorObject
                    ("Can not insert room", this_act);
            }

            res = this->_rooms_form->AddRoomTolist(roomname);
            if(!res){
                _rooms.erase(roomname);
                return ans_obj::MakeErrorObject
                    ("Can not insert room to visual form", this_act);
            }

        }

        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}
//!!!!!!!!!!!!!!!!as2s1212
QJsonObject GraphicsServer::DeleteRoomJs(QString name, QString password, QString roomname)  {

    ACTIONS this_act = ACTIONS::DELETE_ROOM;
    auto lam = [&]{

        //Если нет удаляющего в базе
        if(!IsUserInBase(name, password)){
            return  ans_obj::MakeErrorObject
                ("You are not in chat-base", this_act);
        }

        LG(_mtx_room);
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

        //Пытаемся удалить комнату из сервера
        bool res = _rooms.erase(roomname);
          if(!res){
              return ans_obj::MakeErrorObject
                  ("Can not delete room", this_act);
          }

          //Пытаемся удалить комнату из формы
          res = this->_rooms_form->RemoveRoomFromList(roomname);
          if(!res){
              //Не получилось удалить - восстанавливаем.
              _rooms.insert({roomname, rm});
              return ans_obj::MakeErrorObject
                  ("Can not delete room to visual form", this_act);
          }


        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}

QJsonObject GraphicsServer::LoginUserJs(QString name, QString password, QString roomname) {

    ACTIONS this_act = ACTIONS::LOGIN;
    auto lam = [&]{

        LG(this->_mtx_mod_users);

        //Если юзера нет в базе
        if(!IsUserInBase(name,password)){return ans_obj::MakeErrorObject
                ("Can not find user", this_act);}

        //Если неверен пароль
        if(password != this->_pass_hash.at(name).password ){
            return ans_obj::MakeErrorObject
                ("Wrong password", this_act);
        }
        QString token = QString(this->_token_generator.GenerateHEXToken().data());

        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}
QJsonObject GraphicsServer::RegisterUserJs(QString name, QString password) {

    ACTIONS this_act = ACTIONS::CREATE_USER ;
    auto lam = [&]{

        LG(this->_mtx_mod_users);

         //Если юзера нет в базе
        bool in_base = IsUserInBase(name, password);
        if(in_base){
            return ans_obj::MakeErrorObject
                ("User with this name is already registered",
                 this_act); }

        //Пытаемся в базу сервера вставить пользователя
        UserRole role = { std::move(password) , Role::USER };
        bool added_to_users = _pass_hash.insert({name, std::move(role)}).second;
        if(added_to_users){ return ans_obj::MakeErrorObject
                ("Failed to add user(server)",
                 this_act); }

        bool sql_added = true;
        //Добавляем в SQL
        /*
          Add TO SQL;

        */
        if(!sql_added){
            _pass_hash.erase(name);
            return ans_obj::MakeErrorObject
                ("Failed to add user(SQL)",
                 ACTIONS::CREATE_USER); }

        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

QJsonObject GraphicsServer::DeleteUserJs(QString name, QString password, QString to_delete) {

    ACTIONS this_act = ACTIONS::DELETE_USER;
    auto lam = [&]{
        LG(this->_mtx_mod_users);

        //Если юзера нет в базе
        if(!IsUserInBase(name,password)){return ans_obj::MakeErrorObject
                ("Can not find initiator user", this_act);}

        if(_pass_hash.contains(to_delete)){
            return ans_obj::MakeErrorObject
                ("Can not find user, who must be deleted", this_act);
        }

        if(name!= to_delete){
            if(!HasPermission(name, password,this_act)){
                return ans_obj::MakeErrorObject
                    ("You have no permission to delete users", this_act);
            }

            if(_pass_hash.at(name).role <= _pass_hash.at(to_delete).role){
                return ans_obj::MakeErrorObject
                    ("Your rank doesn't allow you to delete this user", this_act);
            }
        }

        //Резервируем на случай отката
        auto pair = *_pass_hash.find(to_delete);

        //Пытаемся удалить с сервера
        bool delete_from_server = _pass_hash.erase(to_delete);
        if(!delete_from_server){
            return ans_obj::MakeErrorObject
                ("Failed to delete user(server)",
                 ACTIONS::CREATE_USER);
        }

        //Пытаемся удалить с SQL
        bool sql_deleted = true;
        //Удаляем из SQL
        /*
          Add TO SQL;

        */
        if(!sql_deleted){
            _pass_hash.insert(pair);
            return ans_obj::MakeErrorObject
                ("Failed to delete user(SQL)",
                 ACTIONS::CREATE_USER);
        }

        //!!!!!!!!!!!!!!!!!!!!!!!!!
        return QJsonObject{};
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}
