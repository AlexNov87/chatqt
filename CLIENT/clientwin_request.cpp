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

void ClientWin::SendRequestLogin(){

    //Если не в системе то логиним имя
    if(_my_name.isEmpty() || _my_pass.isEmpty()){
      FormLogin login("Enter your personal data");
      auto res = login.exec();
      if(res != QDialog::Accepted){return;}
      _my_name = login.Name();
      _my_pass = login.Password();
    };

    if(_in_room){
        SendRequestLeaveRoom();
    }
    _my_room = ui->cb_roomlist->currentText();
    json_obj js = req_obj::MakeRequestJoinRoom
        (_my_name, _my_pass, ui->cb_roomlist->currentText());

    QByteArray buf = json::WritetoQByteAnyJson (js);
    sock.GuardSendMessageOtherSide(buf);
}

void ClientWin::SendRequestLeaveRoom() {

    json_obj js = req_obj::MakeRequestDisconnect(_my_token, _my_room);
    QByteArray buf = json::WritetoQByteAnyJson (js);
    sock.GuardSendMessageOtherSide(buf);
}
