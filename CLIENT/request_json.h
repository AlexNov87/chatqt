#ifndef REQUEST_JSON_H
#define REQUEST_JSON_H
#include "../constants.h"
#include "../initializators_help.h"

namespace req_obj {

json_obj MakeRequestAddRooms
    (str_type name, str_type password, str_type roomname);

json_obj MakeRequestDeleteRooms
    (str_type name, str_type password, str_type roomname);

//USERS
json_obj MakeRequestLogin
    (str_type name, str_type password, str_type roomname);

json_obj MakeRequestRegisterUser(str_type name, str_type password);

json_obj MakeRequestDeleteUser
    (str_type name, str_type password, str_type to_delete);

json_obj MakeRequestGetRoomUsers(str_type roomname);

//ROOM
json_obj MakeRequestDisconnect(str_type token);

json_obj MakeRequestPublicMessage(str_type token, str_type message) ;

json_obj MakeRequestPublicMessage
    (str_type token, str_type message, str_type user_to) ;


}//namespace
#endif // REQUEST_JSON_H
