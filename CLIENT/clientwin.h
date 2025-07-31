#ifndef CLIENTWIN_H
#define CLIENTWIN_H

#include <QMainWindow>
#include <QTcpSocket>
#include"request_json.h"
#include"../structs.h"
#include"../formmaster.h"
#include "../formlogin.h"


QT_BEGIN_NAMESPACE

class AnswerSession;

namespace Ui {
class ClientWin;
}
QT_END_NAMESPACE

class ClientWin : public QMainWindow
{
    Q_OBJECT

public:
    ClientWin(QWidget *parent = nullptr);
    ~ClientWin();

private slots:
    void onReadyRead();
    void on_commandLinkButton_clicked();
    void on_pb_connect_clicked();
    void on_pb_register_clicked();
    void on_pb_login_clicked();

private:
    friend class AnswerSession;
    void SendRequestToGetRooms(){
        static json_obj rooms = req_obj::MakeRequestRoomList();
        auto js =  json::WritetoQByteAnyJson(rooms);
        sock.GuardSendMessageOtherSide(js);
    }

    Ui::ClientWin *ui;
    SocketComplect sock;
};


class AnswerSession {

public:
 AnswerSession(ClientWin* client, const json_obj& obj)
        : _client(client), _obj(obj) {}

 void StartExecute();

private:
    ClientWin* _client;
    const json_obj& _obj;

    void NonBlockingErrorBox(const json_obj& obj){
        QMessageBox* mbox = new QMessageBox(nullptr);
        mbox->setAttribute(Qt::WA_DeleteOnClose);
        mbox->setWindowTitle(obj.value(CONSTANTS::LF_INITIATOR).toString());
        mbox->setText(obj.value(CONSTANTS::LF_REASON).toString());
        mbox->setStandardButtons(QMessageBox::Ok);
        mbox->setModal(false);
        mbox->show();
    }

    std::optional<ACTIONS> GetAction();
    void ExecuteRoomList();


};










#endif // CLIENTWIN_H
