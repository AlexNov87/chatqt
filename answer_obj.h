#ifndef ANSWER_OBJ_H
#define ANSWER_OBJ_H
#include "alias.h"
#include "constants.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

namespace  ans_obj {
//ERROR
json_obj MakeErrorObject(QString reason, ACTIONS act);
json_obj MakeSuccessTemplate(ACTIONS act);

json_obj SuccessCreateRoom(QString name);

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
