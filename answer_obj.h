#ifndef ANSWER_OBJ_H
#define ANSWER_OBJ_H
#include "constants.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>



namespace  ans_obj {

json_obj MakeAdminErrorObject(str_type reason, ADMIN_ACTIONS act);
json_obj AdminSuccessCreateRoom(str_type name);
json_obj AdminSuccessDeleteRoom(str_type name);
json_obj AdminSuccessDeleteUser(str_type name);

json_obj AdminSuccessBanUser();
json_obj AdminSuccessUnbanUser();
json_obj AdminSuccessUpdateRoleUser();
json_obj AdminUserList(str_type usetlist);
json_obj AdminRoomList(str_type roomlist);



template<typename Foo>
json_obj GuardExceptSetter(Foo foo, ADMIN_ACTIONS this_act){
    try{
        return foo();
    }
    catch(const std::exception& ex){
        return ans_obj::MakeAdminErrorObject
            ("Exception:" + QString(ex.what()), this_act);
    }
    return {};
}

}
namespace  ans_obj {
//ERROR
json_obj MakeErrorObject(str_type reason, ACTIONS act);
json_obj MakeSuccessTemplate(ACTIONS act);

json_obj SuccessCreateUser(str_type name);
json_obj SuccessDisconnect();
json_obj SuccessRoomUsers (str_type roomname, str_type users_serialized);
json_obj SuccessLogin();
json_obj SuccessServerRooms (str_type rooms_serialized);
json_obj SuccessJoinRoom(str_type token, str_type messages_serialized);
json_obj SuccessPrivateMessage();
json_obj SuccessPublicMessage();

json_obj IncomePublicMessage(const str_type author,str_type msg );
json_obj IncomePrivateMessage(const str_type author,str_type msg);

json_obj TestObject();

template<typename Foo>
json_obj GuardExceptSetter(Foo foo, ACTIONS this_act){
    try{
        return foo();
    }
    catch(const std::exception& ex){
        return ans_obj::MakeErrorObject
            ("Exception:" + QString(ex.what()), this_act);
    }
    return {};
}
}//namespace



#endif // ANSWER_OBJ_H
