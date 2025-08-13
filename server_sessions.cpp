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

                auto res = _srv->AddRoomJs(std::move(name),std::move(password),
                                       std::move(roomname));
                if(json::IsErrorJsonObject(res)){
                    return res;
                }
                return res;
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


             auto res = _srv->DeleteRoomJs(std::move(name),std::move(password),
                                       std::move(roomname));
             if(json::IsErrorJsonObject(res)){
                 return res;
             }

             return res;
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

                 json_obj res = _srv->DeleteUserJs(std::move(name),std::move(password),
                                           std::move(todelete));
                 if(json::IsErrorJsonObject(res)){
                     return res;
                 }

                 return res;
             }

        break;
    case ADMIN_ACTIONS::USER_LIST :
             {
                 str_type user_arr(this->_srv->_sql_work->GetSerializedUsers());
                 json_obj userswithroles(ans_obj::AdminUserList(std::move(user_arr)));
                 return userswithroles;
             }
        break;
    case ADMIN_ACTIONS::ROOM_LIST:
    {
        str_type room_arr = this->_srv->GetRoomlistWithOwners();
                 json_obj roomlistwithowners = ans_obj::AdminRoomList(std::move(room_arr));
        return roomlistwithowners;
    }
    break;
    case ADMIN_ACTIONS::USERLIST_PREDICATE:
    {

        static std::set<str_type> current_complect{
            CONSTANTS::LF_VALUE
        };
        auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);
        if(reason){
            return ans_obj::MakeAdminErrorObject(*reason, act);
        }

        str_type predicate = obj.value(CONSTANTS::LF_VALUE).toString();
        str_type user_arr(this->_srv->_sql_work->GetSerializedUsersPredicate(predicate));
        json_obj userswithroles(ans_obj::AdminUserList(std::move(user_arr)));
        return userswithroles;
    }
    break;
    case ADMIN_ACTIONS::BAN_USER:
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
                 str_type toban = obj.value(CONSTANTS::LF_USER_RECIEVER).toString();

                 auto res = _srv->BanUserJs(std::move(name),std::move(password),
                                        std::move(toban));
                 if(json::IsErrorJsonObject(res)){
                     return res;
                 }
                 return res;

           }
        break;
    case ADMIN_ACTIONS::UNBAN_USER:
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
               str_type tounban = obj.value(CONSTANTS::LF_USER_RECIEVER).toString();

               auto res = _srv->UnBanUserJs(std::move(name),std::move(password),
                                      std::move(tounban));
               if(json::IsErrorJsonObject(res)){
                   return res;
               }
               return res;
           }
        break;

    case ADMIN_ACTIONS::UPDATE_ROLE:
           {
               static std::set<str_type> current_complect{
                   CONSTANTS::LF_NAME , CONSTANTS::LF_PASSWORD ,
                   CONSTANTS::LF_USER_RECIEVER, CONSTANTS::LF_ROLE
               };
               auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj, current_complect);

               if(reason){
                   return ans_obj::MakeAdminErrorObject(*reason, act);
               }

               str_type name = obj.value(CONSTANTS::LF_NAME).toString();
               str_type password = obj.value(CONSTANTS::LF_PASSWORD).toString();
               str_type tounban = obj.value(CONSTANTS::LF_USER_RECIEVER).toString();
               const auto& role = obj.value(CONSTANTS::LF_ROLE).toString();
               if(!_NAME_ROLE.contains(role)){
                   return ans_obj::MakeAdminErrorObject("Role is unrecognized", act);
               };
               auto res = _srv->UpdateUserRoleJs(std::move(name),std::move(password),
                                             std::move(tounban), _NAME_ROLE.at(role));
               if(json::IsErrorJsonObject(res)){
                   return res;
               }
               return res;
            }
        break;
    default:
        break;
    }
    return ans_obj::MakeAdminErrorObject("Unknown type of admin action", act);
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
