#ifndef CLIENTWIN_H
#define CLIENTWIN_H

#include <QMainWindow>
#include <QTcpSocket>
#include<QComboBox>
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
    void on_pb_connect_clicked();
    void on_pb_register_clicked();
    void on_pb_login_clicked();
    void on_cb_roomlist_currentIndexChanged(int index);

    void on_pb_leave_clicked();

    void on_pb_logout_clicked();

private:
    friend class AnswerSession;

    void SendRequestToGetRooms();
    void SendRequestToGetCurrentRoomUsers();
    void SendRequestLogin();
    void SendRequestLeaveRoom();

    void ResetMyData() {
        _my_name.clear();
        _my_pass.clear();
        _my_room.clear();
        _my_token.clear();
        _in_room = false;
    }

    Ui::ClientWin *ui;
    SocketComplect sock;

    bool _in_room = false;
    str_type _my_name;
    str_type _my_pass;
    str_type _my_token;
    str_type _my_room;
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
    void ExecuteRoomMembers();
    void ExecuteJoinRoom();
    void ExecuteDisconnect();
};










#endif // CLIENTWIN_H
