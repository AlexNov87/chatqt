#include "clientwin.h"
#include "./ui_clientwin.h"

void ClientWin::SendRequestToGetRooms(){
    static json_obj rooms = req_obj::MakeRequestRoomList();
    auto js =  json::WritetoQByteAnyJson(rooms);
    sock.GuardSendMessageOtherSide(js);
}


void  ClientWin::SendRequestToGetCurrentRoomUsers(){
    QComboBox* cb_rooms = ui->cb_roomlist;
     json_obj cur_members =
        req_obj::MakeRequestGetRoomUsers(cb_rooms->currentText());
    auto js =  json::WritetoQByteAnyJson(cur_members);
    sock.GuardSendMessageOtherSide(js);
}

void ClientWin::SendRequestJoinRoom(){

    if(!this->_setted_login_parameters){
        json_obj obj = ans_obj::MakeErrorObject
            ("You are not logged in", ACTIONS::SYSTEM);
        NonBlockingErrorBox(obj);
        return;
    }

    if(_in_room){
        auto req = req_obj::MakeRequestDisconnect(_my_token, _my_room);
        QByteArray buf = json::WritetoQByteAnyJson (req);
        sock.GuardSendMessageOtherSide(buf);
    }
    _my_room = ui->cb_roomlist->currentText();

    json_obj js = req_obj::MakeRequestJoinRoom
        (_my_name, _my_pass, _my_room);

    QByteArray buf = json::WritetoQByteAnyJson (js);
    sock.GuardSendMessageOtherSide(buf);
}

void ClientWin::SendRequestLeaveRoom() {
    json_obj js = req_obj::MakeRequestDisconnect(_my_token, _my_room);
    QByteArray buf = json::WritetoQByteAnyJson (js);
    sock.GuardSendMessageOtherSide(buf);
}

void ClientWin::SendRequestMessage() {
    json_obj js ;
    str_type reciever = ui->le_member_name->text().trimmed();
    bool public_msg = reciever.isEmpty();

    if(public_msg){
        js = req_obj::MakeRequestPublicMessage
            (_my_token, ui->te_message->toPlainText(),_my_room);
    }
    else
    {
        js = req_obj::MakeRequestPrivateMessage
            (_my_token, ui->te_message->toPlainText(),reciever,_my_room);
    }
    QByteArray buf = json::WritetoQByteAnyJson(js);
    sock.GuardSendMessageOtherSide(buf);

}

