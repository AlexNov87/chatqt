#include "clientwin.h"
#include "client_designer.h"
#include "./ui_clientwin.h"

ClientWin::ClientWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWin)
{
    ui->setupUi(this);
    sock.socket = new QTcpSocket();
    connect(sock.socket, &QTcpSocket::readyRead , this, &ClientWin::onReadyRead);
    ClientDesigner::ChangeConnectionButtonOff(ui->pb_connect);

}

ClientWin::~ClientWin()
{
    delete ui;
    delete sock.socket;
}

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


void Scene2(SocketComplect& sock){
    auto js = req_obj::MakeRequestRegisterUser("Vasyaq","123");//1
       QByteArray bt = json::WritetoQByteAnyJson (js);
    WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

    //     js = req_obj::MakeRequestAddRooms("Vasya ","123","RAAAAAA"); //2
    //     bt = json::WritetoQByteAnyJson (js);
    // WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

    //     js = req_obj::MakeRequestRoomList(); //3
    //     bt = json::WritetoQByteAnyJson (js);
    //     WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

    //     js = req_obj::MakeRequestLogin("Vasya","123","RAAAAAA"); //4
    //     bt = json::WritetoQByteAnyJson (js);
    //     WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

    //     js = req_obj::MakeRequestGetRoomUsers("RAAAAAA"); //5
    //     bt = json::WritetoQByteAnyJson (js);
    //     WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);
}



void Scene1(SocketComplect& sock){
    auto js = req_obj::MakeRequestLogin("qwe","rty", "uio");
    auto jsq = req_obj::MakeRequestLogin("www","www", "www");
    QByteArray bt = json::WritetoQByteAnyJson (js);
    QByteArray btx = json::WritetoQByteAnyJson(jsq);
    WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);
    WriteToSocketWithFlushAddingSplitSym(sock.socket,btx);
}



void ClientWin::on_commandLinkButton_clicked()
{
      Scene2(sock);

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


void ClientWin::on_pb_login_clicked()
{
    FormLogin login("Enter your personal data");
    auto res = login.exec();

    // json_obj js = req_obj::MakeRequestLogin()
    //     (login.Name(), login.Password());

    // QByteArray buf = json::WritetoQByteAnyJson (js);
    // sock.GuardSendMessageOtherSide(buf);
}




