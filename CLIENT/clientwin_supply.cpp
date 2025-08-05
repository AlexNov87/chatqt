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
    PrepareClearMenu();
    ui->le_member_name->setReadOnly(true);

}

ClientWin::~ClientWin()
{
    delete ui;
    delete sock.socket;
}

void ClientWin::SetLoginOptions(){
    ResetMyData();

    FormLogin login("Enter your personal data");
    auto res = login.exec();
    if(res != QDialog::Accepted){return;}
    _my_name = login.Name();
    _my_pass = login.Password();
    _setted_login_parameters = true;
}

void ClientWin::ResetMyData() {
    if(_in_room){
        SendRequestLeaveRoom();
    }
    if(_setted_login_parameters){
        _my_name.clear();
        _my_pass.clear();
        _my_room.clear();
        _my_token.clear();
        _in_room = false;
        _setted_login_parameters = false;
    }
}

void ClientWin::PrepareClearMenu(){
    _clear_menu = std::make_shared<QMenu>(ui->tb_clear);
    QAction* act_cl_private = _clear_menu->addAction("Clear private field");
    QAction* act_cl_message = _clear_menu->addAction("Clear message field");
    QAction* act_cl_chat = _clear_menu->addAction("Clear chat");

    ui->tb_clear->setMenu(_clear_menu.get());
    ui->tb_clear->setPopupMode(QToolButton::MenuButtonPopup);

    connect(act_cl_private, &QAction::triggered, this, [&]{
    ui->le_member_name->clear(); ;});
    connect(act_cl_message, &QAction::triggered, this, [&]{
        ui->te_message->clear();
    ;});
    connect(act_cl_chat, &QAction::triggered, this , [&]{
         ui->lw_message->clear();
    });





}
