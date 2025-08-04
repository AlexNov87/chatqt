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
    case ACTIONS::GET_ROOM_USERS :
    {
        ExecuteRoomMembers();
    }
    break;
    case ACTIONS::JOIN_ROOM :
    {
        ExecuteJoinRoom();
    }
    break;
    case ACTIONS::DISCONNECT :
    {
        ExecuteDisconnect();
    }
    break;
    case ACTIONS::PRIVATE_MESSAGE :
    {
        FatalErrorMessageBox("Private mess");
    }
    break;
    case ACTIONS::PUBLIC_MESSAGE :
    {
         FatalErrorMessageBox("Public mess");
    }
    break;
    default:
        break;
    }

}

void AnswerSession::ExecuteRoomList(){
    std::variant<json_obj, QStringList>
        result = ServerAnswerChecker::CheckRoomListAnswer(_obj);

    if(std::holds_alternative<QStringList>(result)){
        auto roomlist = this->_client->ui->cb_roomlist;
        roomlist ->clear();
        QStringList& rms= std::get<QStringList>(result);
        roomlist->addItems(rms);
    }
    else {
        NonBlockingErrorBox(std::get<json_obj>(result));
        return;
    }
}

void AnswerSession::ExecuteRoomMembers() {
    std::variant<json_obj, QStringList>
        result = ServerAnswerChecker::CheckMembersRoomListAnswer(_obj);

    if(std::holds_alternative<QStringList>(result)){

       const auto& roomname = this->_client->ui->cb_roomlist->currentText();

        if(roomname != _obj.value(CONSTANTS::LF_ROOMNAME).toString()){
           return;
        }

        QStringList& members = std::get<QStringList>(result);

       auto members_form = this->_client->ui->lw_members;
       members_form->clear();
       members_form->addItems(std::move(members));
    }
    else {
        NonBlockingErrorBox(std::get<json_obj>(result));
    }
}

void AnswerSession::ExecuteJoinRoom(){

    std::optional<json_obj> err =
    ServerAnswerChecker::CheckJoinRoomAnswer(_obj);

    if(err){
        NonBlockingErrorBox(*err);
        return;
    }
    _client->_my_token = _obj.value(CONSTANTS::LF_TOKEN).toString();
    _client->_in_room = true;
}

void  AnswerSession::ExecuteDisconnect(){
    _client->_my_token.clear();
    _client->_in_room = false;
}
