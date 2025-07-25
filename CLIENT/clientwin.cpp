#include "clientwin.h"
#include "./ui_clientwin.h"

ClientWin::ClientWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWin)
{
    ui->setupUi(this);
    connect(&sock, &QTcpSocket::readyRead , this, &ClientWin::onReadyRead);
}

ClientWin::~ClientWin()
{
    delete ui;
}

void ClientWin::on_commandLinkButton_pressed()
{

}


void ClientWin::on_commandLinkButton_clicked()
{

    QHostAddress adr;
    adr.setAddress("127.0.0.1");


     sock.connectToHost(adr, 80);

    auto js = req_obj::MakeRequestLogin("qwe","rty", "uio");
    auto jsq = req_obj::MakeRequestLogin("www","www", "www");
    QByteArray bt = json::WritetoQByteArrayJson(js);
    QByteArray btx = json::WritetoQByteArrayJson(jsq);

    if(sock.waitForConnected(5000)) {
        WriteToSocketWithFlush(&sock,bt);
        WriteToSocketWithFlush(&sock,btx);

    //    sock.write(bt);
     //   sock.write(btx);
    } else {
        QMessageBox::critical(nullptr, "", "NO CONN");
        //   qDebug() << "Ошибка подключения:" << sock.errorString();
    }

}

