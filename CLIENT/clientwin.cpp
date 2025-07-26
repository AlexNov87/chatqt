#include "clientwin.h"
#include "./ui_clientwin.h"

ClientWin::ClientWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWin)
{
    ui->setupUi(this);
    sock.socket = new QTcpSocket();
    connect(sock.socket, &QTcpSocket::readyRead , this, &ClientWin::onReadyRead);
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
    }



}


void Scene2(SocketComplect& sock){
    auto js = req_obj::MakeRequestRegisterUser("Vasya","123");//1
       QByteArray bt = json::WritetoQByteAnyJson (js);
    WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

        js = req_obj::MakeRequestAddRooms("Vasya ","123","RAAAAAA"); //2
        bt = json::WritetoQByteAnyJson (js);
    WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

        js = req_obj::MakeRequestRoomList(); //3
        bt = json::WritetoQByteAnyJson (js);
        WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

        js = req_obj::MakeRequestLogin("Vasya","123","RAAAAAA"); //4
        bt = json::WritetoQByteAnyJson (js);
        WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);

        js = req_obj::MakeRequestGetRoomUsers("RAAAAAA"); //5
        bt = json::WritetoQByteAnyJson (js);
        WriteToSocketWithFlushAddingSplitSym (sock.socket,bt);


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


void ClientWin::on_commandLinkButton_2_clicked()
{
    QHostAddress adr;
    adr.setAddress("127.0.0.1");
    sock.socket->connectToHost(adr, 80);
    if(sock.socket->waitForConnected(5000)) {} else {
        QMessageBox::critical(nullptr, "", "NO CONN");
    }
}

