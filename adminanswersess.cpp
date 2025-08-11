#include "formadmin.h"
AnswerAdminSession::AnswerAdminSession(AdminUserForm* uform, const json_obj& obj)
    : _user_form(uform), _obj(obj) {}

void AnswerAdminSession::StartExecute(){
    auto action = GetAction();
    if(!action) {return;}
   auto obb = json::WritetoQByteAnyJson(_obj);
   FatalErrorMessageBox(obb);

    switch (*action) {
    case ADMIN_ACTIONS::BAN_USER :
     break;
    case ADMIN_ACTIONS::UNBAN_USER :
        break;
    case ADMIN_ACTIONS::CREATE_ROOM :
        break;
    case ADMIN_ACTIONS::DELETE_ROOM :
        break;
    case ADMIN_ACTIONS::DELETE_USER :
        break;
    case ADMIN_ACTIONS::ROOM_LIST :
        break;
    case ADMIN_ACTIONS::USER_LIST :
        break;
    case ADMIN_ACTIONS::UPDATE_ROLE :
        break;
    case ADMIN_ACTIONS::SYSTEM :
        break;
    case ADMIN_ACTIONS::GET_SERIAL_ROOMS:
        break;
    case ADMIN_ACTIONS::GET_SERIAL_USERS:
        break;
    }
}

std::optional<ADMIN_ACTIONS> AnswerAdminSession::GetAction(){

    if(json::IsErrorJsonObject(_obj)){
        NonBlockingErrorBox(_obj);
        return std::nullopt;
    }

    //Проверка содержания полей
    static std::set<str_type> fields{CONSTANTS::LF_RESULT, CONSTANTS::LF_INITIATOR};
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(_obj, fields);

    if (reason) {
        auto obj =  ans_obj::MakeAdminErrorObject(*reason, ADMIN_ACTIONS::SYSTEM);
        NonBlockingErrorBox(obj);
        return std::nullopt;
    }
    auto action = _NAME_ADMIN_ACTION.at(act);
    return action;
}
void AnswerAdminSession::ExecuteRoomList(){}
void AnswerAdminSession::ExecuteUserslist(){}

void AnswerAdminSession::ExecuteAddRoom(){}
void AnswerAdminSession::ExecuteDeleteRoom(){}

void AnswerAdminSession::ExecuteDeleteUser(){}
void AnswerAdminSession::ExecuteBanUser(){}
void AnswerAdminSession::ExecuteUnbanUser(){}

void AnswerAdminSession::ExecuteUpdateRole(){}
void AnswerAdminSession::ExecuteSystem(){}
