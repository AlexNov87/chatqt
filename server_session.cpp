#include"mainwindow.h"
void ServerSession::MakeErrorAnsweToSocket(const QJsonObject &answer_obj){
    QByteArray arr;
    auto wg = _srv->_maiwin->ui->lw_events;
    arr = json::WritetoQByteAnyJson(answer_obj);

    wg->addItem(QString(arr));
    WriteToSocketWithFlushAddingSplitSym(_sock->socket, arr);
}

ServerSession::ServerSession
(const std::shared_ptr<GraphicsServer> srv, SocketComplect* sock)
    : _srv(srv), _sock(sock){}

void ServerSession::Execute(){

    //Проверяем валидность указателей на сокет и сервер.
    if(!IsPointersValid()){return;}

    while (auto qbyte = _sock->GetExecuteObject()){
        json_obj json_stuff = json::ReadJsonObjectFromQbyteArray(*qbyte);
        //Проверяем текущий объект на валидность. Если объект будет не вален,
        //то вернется объект, содержащий ошибку.
        std::optional<json_obj> err_obj = FirstStepCheckErrors(json_stuff);
        if(err_obj){
            MakeErrorAnsweToSocket(*err_obj);
            continue;
        }
        json_obj answer = ExecuteExternal(json_stuff);
        QByteArray arr = json::WritetoQByteAnyJson(answer);
        _sock->GuardSendMessageOtherSide(arr);
    }

}

std::optional<json_obj> ServerSession::FirstStepCheckErrors
    (const json_obj& js_obj){
    //Проверка, если данный объект является объектом ошибки.
    bool is_err = json::IsErrorJsonObject(js_obj);
    if(is_err){
        return js_obj;
    }

    static std::set<str_type> obj_complect{CONSTANTS::LF_ACTION};
    //Есть ли в объекте поле действия.
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(js_obj, obj_complect);
    if(reason){
        return ans_obj::MakeErrorObject(*reason, ACTIONS::SYSTEM);
    }

    //Если это действие есть в списке
    str_type act_value = (js_obj).value(CONSTANTS::LF_ACTION).toString();
    if(!_ACT_SERVER.contains(act_value)){
        return ans_obj::MakeErrorObject("Can not find action: " + act_value, ACTIONS::SYSTEM);
    };
    return std::nullopt;
}

bool ServerSession::IsPointersValid(){
    if(!_srv || !_sock){
        if(!_srv){FatalErrorMessageBox("The server is null");}
        if(!_sock){FatalErrorMessageBox("The socket is null");}
        return false;
    }
    return true;
}

json_obj ServerSession::ExecuteExternal(const json_obj& obj){

    QString action_str = obj.value(CONSTANTS::LF_ACTION).toString();
    ACTIONS act =  _NAME_ACTION.at(action_str);
    switch (act) {
    case ACTIONS::CREATE_ROOM :
    {
        static std::set<str_type> current_complect{
            CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_ROOMNAME
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);
        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type name = obj.value(CONSTANTS::LF_NAME).toString();
        str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        return _srv->AddRoomJs(std::move(name),std::move(password),
                               std::move(roomname));
    }
    break;
    //////////////////////////////////////
    case ACTIONS::CREATE_USER :
    {
        static std::set<str_type> current_complect{
            CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type name = obj.value(CONSTANTS::LF_NAME).toString();
        str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
        return _srv->RegisterUserJs(std::move(name),std::move(password));
    }
        break;
    //////////////////////////////////////
    case ACTIONS::DELETE_ROOM :
    {
        static std::set<str_type> current_complect{
            CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_ROOMNAME
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type name = obj.value(CONSTANTS::LF_NAME).toString();
        str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        return _srv->DeleteRoomJs(std::move(name),std::move(password),
                                  std::move(roomname));
    }
        break;
    //////////////////////////////////////
    case ACTIONS::DELETE_USER :
    {

        static std::set<str_type> current_complect{
            CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_USER_TO_DELETE
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type name = obj.value(CONSTANTS::LF_NAME).toString();
        str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
        str_type todelete = obj.value(CONSTANTS::LF_USER_TO_DELETE).toString();
        return _srv->DeleteRoomJs(std::move(name),std::move(password),
                                  std::move(todelete));
    }
        break;
    //////////////////////////////////////
    case ACTIONS::DISCONNECT :
    {

        static std::set<str_type> current_complect{
            CONSTANTS::LF_TOKEN, CONSTANTS::LF_ROOMNAME
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type token = obj.value(CONSTANTS::LF_TOKEN).toString();
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        return _srv->DisconnectJs(std::move(token),std::move(roomname));
    }
        break;
    //////////////////////////////////////
    case ACTIONS::GET_ROOM_USERS :
    {
        static std::set<str_type> current_complect{
           CONSTANTS::LF_ROOMNAME
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        return _srv->GetRoomUsersJs(std::move(roomname));
    }
    break;
    //////////////////////////////////////
    case ACTIONS::JOIN_ROOM :
    {
        static std::set<str_type> current_complect{
            CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_ROOMNAME
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type name = obj.value(CONSTANTS::LF_NAME).toString();
        str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        return _srv->JoinRoomUserJs(std::move(name),std::move(password),
                                  std::move(roomname), _sock);
    }
        break;
    //////////////////////////////////////
    case ACTIONS::GET_ROOMS_LIST :
    {

        return _srv->GetRoomsJs();
    }
        break;
    //////////////////////////////////////
    case ACTIONS::PRIVATE_MESSAGE :
    {
        static std::set<str_type> current_complect{
         CONSTANTS::LF_TOKEN, CONSTANTS::LF_ROOMNAME,
         CONSTANTS::LF_USER_RECIEVER, CONSTANTS::LF_PRIVATE_MESSAGE
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type token = obj.value(CONSTANTS::LF_TOKEN).toString();
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        str_type reciever = obj.value(CONSTANTS::LF_USER_RECIEVER).toString();
        str_type message = obj.value(CONSTANTS::LF_PRIVATE_MESSAGE).toString();

        return _srv->PrivateMessageJs(std::move(token),std::move(message),
        std::move(reciever),std::move(roomname));
    }
        break;
    //////////////////////////////////////
    case ACTIONS::PUBLIC_MESSAGE :
    {
        static std::set<str_type> current_complect {
                CONSTANTS::LF_TOKEN, CONSTANTS::LF_ROOMNAME,
                CONSTANTS::LF_PUBLIC_MESSAGE
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

        if(reason){
            return ans_obj::MakeErrorObject(*reason, act);
        }
        str_type token = obj.value(CONSTANTS::LF_TOKEN).toString();
        str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();
        str_type message = obj.value(CONSTANTS::LF_PUBLIC_MESSAGE).toString();
        return _srv->PublicMessageJs(std::move(token),std::move(message),
                                      std::move(roomname));
    }
        break;
    //////////////////////////////////////
    default:
        return ans_obj::MakeErrorObject("UNKNOWN ACTION", ACTIONS::SYSTEM);

        break;
    }

    return {};

}
