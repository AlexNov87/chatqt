#ifndef REQUEST_JSON_H
#define REQUEST_JSON_H
#include "constants.h"
#include "initializators_help.h"

namespace req_obj {

//USERS
json_obj MakeRequestJoinRoom
    (str_type name, str_type password, str_type roomname);//4

json_obj MakeRequestRegisterUser(str_type name, str_type password);//1


json_obj MakeRequestGetRoomUsers(str_type roomname); //5

//ROOM
json_obj MakeRequestDisconnect(str_type token, str_type room_name);

json_obj MakeRequestPublicMessage(str_type token, str_type message, str_type room_name) ;

json_obj MakeRequestPrivateMessage
    (str_type token, str_type message, str_type user_to, str_type room_name) ;

json_obj MakeRequestRoomList();//3

}//namespace

namespace req_obj {
json_obj AdminMakeRequestDeleteUser
    (str_type name, str_type password, str_type to_delete);

json_obj AdminMakeRequestDeleteRooms
    (str_type name, str_type password, str_type roomname);

json_obj AdminMakeRequestAddRooms
    (str_type name, str_type password, str_type roomname);

json_obj AdminMakeRequestBanUser
    (str_type name, str_type password, str_type to_ban);
json_obj AdminMakeRequestUnbanUser
    (str_type name, str_type password, str_type to_unban);
json_obj AdminMakeRequestFindUsers(str_type find_pattern);
json_obj AdminMakeRequestUpdareRole
    (str_type name, str_type password, str_type to_set_role, Role role);

}//namespace

#endif // REQUEST_JSON_H
