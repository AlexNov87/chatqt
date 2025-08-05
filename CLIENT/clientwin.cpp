#include "clientwin.h"
#include "client_designer.h"
#include "./ui_clientwin.h"

void ClientWin::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;
    QByteArray data = clientSocket->readAll();
    sock.AddToBuffer(data);
    while (auto obj = sock.GetExecuteObject()) {
        this->ui->lw_message->addItem(*obj);
        json_obj js = json::ReadJsonObjectFromQbyteArray(*obj);
        AnswerSession ans(this, js);
        ans.StartExecute();
    }
}

void ClientWin::on_pb_connect_clicked()
{
    QHostAddress adr;
    if(!adr.setAddress(ui->le_setip->text())){
          FatalErrorMessageBox("IP is incorrect");
          return;
    };

    if(!sock.socket->isOpen()){
        sock.socket->connectToHost(adr, ui->sb_port->value());
        if(sock.socket->waitForConnected(5000)) {
            ClientDesigner::ChangeConnectionButtonOn(ui->pb_connect);
            ui->gb->setEnabled(true);
            SendRequestToGetRooms();
        } else {
           FatalErrorMessageBox("Failed to connect");
        }
    }
    else
    {
        sock.socket->close();
        ClientDesigner::ChangeConnectionButtonOff(ui->pb_connect);
        ui->gb->setEnabled(false);
    }
}

void ClientWin::on_pb_register_clicked()
{
    Formmaster create_user("Creating new user");
    auto res = create_user.exec();
    if(res != QDialog::Accepted){ return; }

    json_obj js = req_obj::MakeRequestRegisterUser
        (create_user.Name(), create_user.Password());

    QByteArray buf = json::WritetoQByteAnyJson (js);
    sock.GuardSendMessageOtherSide(buf);
}

void ClientWin::on_cb_roomlist_currentIndexChanged(int index)
{
    if(ui->cb_roomlist->currentText().isEmpty()){return;}
    SendRequestToGetCurrentRoomUsers();
}

void ClientWin::on_pb_leave_clicked()
{
    if(!_in_room){ return; }
    SendRequestLeaveRoom();
    ui->lw_members->clear();
}

void ClientWin::on_pb_logout_clicked()
{
    ResetMyData();
}

void ClientWin::on_commandLinkButton_clicked()
{
    FatalErrorMessageBox("R:"+_my_room + " T:"+_my_token);
}

void ClientWin::on_pb_join_room_clicked()
{
    SendRequestJoinRoom();
}

void ClientWin::on_pb_set_log_clicked()
{
    SetLoginOptions();
}


void ClientWin::on_tb_clear_clicked()
{
        ui->le_member_name->clear();
        ui->te_message->clear();
        ui->lw_message->clear();
}


void ClientWin::on_lw_members_itemClicked(QListWidgetItem *item)
{
   ui->le_member_name->setText(item->text());
}


void ClientWin::on_pb_send_message_clicked()
{
    SendRequestMessage();
}

