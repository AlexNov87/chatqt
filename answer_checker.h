#ifndef ANSWER_CHECKER_H
#define ANSWER_CHECKER_H
#include "initializators_help.h"
#include "answer_obj.h"
#include<set>
#include<variant>
#include<QJsonObject>
#include<QJsonArray>
#include<QString>

class ServerAnswerChecker {
public:
    static std::variant<json_obj, QStringList>
    CheckRoomListAnswer(const json_obj& obj);

    static std::variant<json_obj, QStringList>
    CheckMembersRoomListAnswer(const json_obj& obj);

    static std::optional<json_obj>
    CheckJoinRoomAnswer(const json_obj& obj);

};

#endif // ANSWER_CHECKER_H
