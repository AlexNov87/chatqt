#ifndef ANSWER_OBJ_H
#define ANSWER_OBJ_H
#include "constants.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

namespace  ans_obj {
//ERROR
json_obj MakeErrorObject(str_type reason, ACTIONS act);
json_obj MakeSuccessTemplate(ACTIONS act);

json_obj SuccessCreateRoom(str_type name);
json_obj SuccessCreateUser(str_type name);
json_obj SuccessDeleteRoom(str_type name);
json_obj SuccessDeleteUser(str_type name);

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
