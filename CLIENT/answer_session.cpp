#include "clientwin.h"
#include "./ui_clientwin.h"
#include "answer_checker.h"

std::optional<ACTIONS> AnswerSession::GetAction() {


    if(json::IsErrorJsonObject(_obj)){
        NonBlockingErrorBox(_obj);
        return std::nullopt;
    }

    //Проверка содержания полей
    static std::set<str_type> fields{CONSTANTS::LF_RESULT, CONSTANTS::LF_INITIATOR};
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(_obj, fields);

    if (reason) {
        auto obj =  ans_obj::MakeErrorObject(*reason, ACTIONS::SYSTEM);
        NonBlockingErrorBox(obj);
        return std::nullopt;
    }

    const auto act = _obj.value(CONSTANTS::LF_INITIATOR).toString();
    //Проверка валидности действия
    if(!_NAME_ACTION.contains(act)){
        auto obj =  ans_obj::MakeErrorObject
            ("Action is not recognized", ACTIONS::SYSTEM);
        NonBlockingErrorBox(obj);
        return std::nullopt;
    }
    auto action = _NAME_ACTION.at(act);
    return action;
}



void AnswerSession::StartExecute() {

    auto action = GetAction();
    if(!action) {return;}

    switch (*action) {
    case ACTIONS::GET_ROOMS_LIST:
    {
        ExecuteRoomList();
    }
    break;
    default:
        break;
    }

}

void AnswerSession::ExecuteRoomList(){
    std::variant<json_obj, std::set<str_type>>
        result = ServerAnswerChecker::CheckRoomListAnswer(_obj);

    if(std::holds_alternative<std::set<str_type>>(result)){

        auto roomlist = this->_client->ui->cb_roomlist;
        roomlist ->clear();
        const std::set<str_type>& rms=
            std::get<std::set<str_type>>(result);

        for(str_type rm : rms){
            roomlist->addItem(std::move(rm));
        }
    }
    else {
        NonBlockingErrorBox(std::get<json_obj>(result));
        return;
    }
}
