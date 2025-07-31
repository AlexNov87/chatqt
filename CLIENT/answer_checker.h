#ifndef ANSWER_CHECKER_H
#define ANSWER_CHECKER_H
#include "../initializators_help.h"
#include "../answer_obj.h"
#include<set>
#include<variant>
#include<QJsonObject>
#include<QJsonArray>
#include<QString>

class ServerAnswerChecker {
public:
    static std::variant<json_obj, std::set<str_type>>
    CheckRoomListAnswer(const json_obj& obj);

};

#endif // ANSWER_CHECKER_H
