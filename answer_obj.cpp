#include "answer_obj.h"

namespace  ans_obj {
//ERROR
json_obj MakeErrorObject(QString reason, ACTIONS act){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_ERROR);
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_NAME.at(act));
    obj.insert(CONSTANTS::LF_REASON, std::move(reason));
    return obj;
}

json_obj MakeSuccessTemplate(ACTIONS act){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_SUCCESS);
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_NAME.at(act));
    return obj;
}

json_obj SuccessCreateRoom(QString name){
    json_obj obj = MakeSuccessTemplate(ACTIONS::CREATE_ROOM);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(name));
    return obj;
}

}//namespace
