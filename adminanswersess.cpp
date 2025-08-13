#include "answer_checker.h"
#include "ui_formadmin.h"
#include "formadmin.h"

AnswerAdminSession::AnswerAdminSession(AdminUserForm* uform, const json_obj& obj)
    : _user_form(uform), _obj(obj) {
}

void AnswerAdminSession::StartExecute(){
    auto action = GetAction();
    if(!action) {return;}
;
    switch (*action) {
    case ADMIN_ACTIONS::BAN_USER :
        QMessageBox::information(nullptr, "", "Ban user success");
        this->_user_form->OnUpdateUsersClicked();
        break;
    case ADMIN_ACTIONS::UNBAN_USER :
        QMessageBox::information(nullptr, "", "Unban user success");
        this->_user_form->OnUpdateUsersClicked();
        break;
    case ADMIN_ACTIONS::CREATE_ROOM :
        QMessageBox::information(nullptr, "", "Create room success");
        this->_user_form->OnUpdateRoomClicked();
        break;
    case ADMIN_ACTIONS::DELETE_ROOM :
        QMessageBox::information(nullptr, "", "Delete room success");
        this->_user_form->OnUpdateRoomClicked();
        break;
    case ADMIN_ACTIONS::DELETE_USER :
        QMessageBox::information(nullptr, "", "Delete user success");
        this->_user_form->OnUpdateUsersClicked();
        break;
    case ADMIN_ACTIONS::ROOM_LIST :
    {

        auto object = ServerAnswerChecker::CheckAdminRooms(_obj);
        if(std::holds_alternative<json_obj>(object)){
            NonBlockingErrorBox(std::get<json_obj>(object));
            return;
        }
        const std::map<str_type,str_type>& room_owner =
            std::get<std::map<str_type,str_type>>(object);
        int row = 0;

        QTableWidget* room_table = this->_user_form->ui->table_rooms;
        room_table->setRowCount(room_owner.size());
        for(auto& [room, owner] : room_owner){
            room_table->setItem(row, 0,
                                new QTableWidgetItem(std::move(room)));
            room_table->setItem(row++, 1,
                                new QTableWidgetItem(std::move(owner)));
        }
    }
        break;
    case ADMIN_ACTIONS::USER_LIST :
    {

        auto object = ServerAnswerChecker::CheckAdminUsers(_obj);
        if(std::holds_alternative<json_obj>(object)){
            NonBlockingErrorBox(std::get<json_obj>(object));
            return;
        }
        const std::map<str_type,str_type>& user_role =
            std::get<std::map<str_type,str_type>>(object);

        int row = 0;
        QTableWidget* user_table = this->_user_form->ui->table_users;
        user_table->setRowCount(user_role.size());
        for(auto& [user, role] : user_role){
            user_table->setItem(row, 0,
                                new QTableWidgetItem(std::move(user)));
            user_table->setItem(row++, 1,
                                new QTableWidgetItem(std::move(role)));
        }
    }
        break;
    case ADMIN_ACTIONS::UPDATE_ROLE :
        QMessageBox::information(nullptr, "", "Update role success");
        this->_user_form->OnUpdateUsersClicked();
        break;
    case ADMIN_ACTIONS::SYSTEM :
        NonBlockingErrorBox(_obj);
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

    const auto act = _obj.value(CONSTANTS::LF_INITIATOR).toString();

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
