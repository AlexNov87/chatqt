#include"mainwindow.h"

AbstractSession::AbstractSession(std::shared_ptr<GraphicsServer>srv,
                json_obj object, SocketComplect* sock):
    obj(std::move(object)), _srv(srv), _sock(sock)
{
    if(!_srv || !_sock){
        FatalErrorMessageBox("The pointers are bad");
    }
}

ServerSession::ServerSession
(std::shared_ptr<GraphicsServer> srv, json_obj object, SocketComplect* sock)
    :AbstractSession(srv, std::move(object), sock) {}

ServerAdminSession::ServerAdminSession(std::shared_ptr<GraphicsServer> srv,
    json_obj object, SocketComplect* sock) :
AbstractSession(srv, std::move(object), sock){}


json_obj ServerAdminSession::SessionResult() {

    QString action_str = obj.value(CONSTANTS::LF_ACTION).toString();
    ADMIN_ACTIONS act = _NAME_ADMIN_ACTION.at(action_str);
    switch (act) {
    case ADMIN_ACTIONS::CREATE_ROOM:
    {
        static std::set<str_type> current_complect{
                    CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_ROOMNAME
                };
                auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);
                if(reason){
                    return ans_obj::MakeAdminErrorObject(*reason, act);
                }
                str_type name = obj.value(CONSTANTS::LF_NAME).toString();
                str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
                str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();

                 /* ОТОСЛАТЬ НОВЫЙ СПИСОК КОМНАТ*/

                return _srv->AddRoomJs(std::move(name),std::move(password),
                                       std::move(roomname));
    }
        break;
    case ADMIN_ACTIONS::DELETE_ROOM:
         {
             static std::set<str_type> current_complect{
                 CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_ROOMNAME
             };
             auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

             if(reason){
                 return ans_obj::MakeAdminErrorObject(*reason, act);
             }
             str_type name = obj.value(CONSTANTS::LF_NAME).toString();
             str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
             str_type roomname = obj.value(CONSTANTS::LF_ROOMNAME).toString();

               /* ОТОСЛАТЬ НОВЫЙ СПИСОК КОМНАТ*/

             return _srv->DeleteRoomJs(std::move(name),std::move(password),
                                       std::move(roomname));
         }
    break;
    case ADMIN_ACTIONS::DELETE_USER:
             {

                 static std::set<str_type> current_complect{
                     CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,CONSTANTS::LF_USER_RECIEVER
                 };
                 auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

                 if(reason){
                     return ans_obj::MakeAdminErrorObject(*reason, act);
                 }
                 str_type name = obj.value(CONSTANTS::LF_NAME).toString();
                 str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
                 str_type todelete = obj.value(CONSTANTS::LF_USER_RECIEVER).toString();
                 return _srv->DeleteRoomJs(std::move(name),std::move(password),
                                           std::move(todelete));
             }

        break;
    case ADMIN_ACTIONS::FIND_USERS:
        break;
    case ADMIN_ACTIONS::BAN_USER:
        break;
    case ADMIN_ACTIONS::UNBAN_USER:
        break;
    case ADMIN_ACTIONS::UPDATE_ROLE:
        break;
    default:
        break;
    }
// case ADMIN_ACTIONS:
//     break;

}




json_obj ServerSession::SessionResult(){

    QString action_str = obj.value(CONSTANTS::LF_ACTION).toString();
    ACTIONS act =  _NAME_ACTION.at(action_str);
    switch (act) {
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
    case ACTIONS::DISCONNECT:
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
