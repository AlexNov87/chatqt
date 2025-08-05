#ifndef CLIENTWIN_H
#define CLIENTWIN_H

#include <QMainWindow>
#include <QTcpSocket>
#include<QComboBox>
#include<QCommandLinkButton>
#include"request_json.h"
#include"../structs.h"
#include"../formmaster.h"
#include "../formlogin.h"
#include "../formadmin.h"
#include "../ui_formadmin.h"

QT_BEGIN_NAMESPACE

class AnswerSession;
namespace Ui {
class ClientWin;
}
QT_END_NAMESPACE
class AdminUserForm;

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
    void on_cb_roomlist_currentIndexChanged(int index);

    void on_pb_leave_clicked();
    void on_pb_logout_clicked();
    void on_pb_join_room_clicked();
    void on_pb_set_log_clicked();

    void on_tb_clear_clicked();
    void on_lw_members_itemClicked(QListWidgetItem *item);

    void on_pb_send_message_clicked();

    void on_pb_admin_form_clicked();

private:
    friend class AnswerSession;
    void SendRequestToGetRooms();
    void SendRequestToGetCurrentRoomUsers();
    void SendRequestJoinRoom();
    void SendRequestLeaveRoom();
    void SendRequestMessage();

private:
    void SetLoginOptions();
    void ResetMyData();
    void PrepareClearMenu();

private:
    friend class AdminUserForm;
    Ui::ClientWin *ui;
    SocketComplect sock;
    std::shared_ptr<QMenu> _clear_menu;

    bool _in_room = false;
    bool _setted_login_parameters = false;
    bool _admin_opened = false;

    str_type _my_name;
    str_type _my_pass;
    str_type _my_token;
    str_type _my_room;
};


class AnswerSession
{

public:
 AnswerSession(ClientWin* client, const json_obj& obj)
        : _client(client), _obj(obj) {}

 void StartExecute();

private:
    ClientWin* _client;
    const json_obj& _obj;

    std::optional<ACTIONS> GetAction();
    void ExecuteRoomList();
    void ExecuteRoomMembers();
    void ExecuteJoinRoom();
    void ExecuteDisconnect();
    void ExecutePrivateMsg();
    void ExecutePublicMsg();
};

class AdminUserForm : public Formadmin {
public:
    AdminUserForm(ClientWin* client);

private:
    void Init();
    void OnDeleteRoomClicked() override ;
    void OnCreateRoomClicked() override ;
private:
    void OnBlockUserClicked() override;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override ;
    void OnUpdateUsersClicked() override ;

    ClientWin* _clientwin;
};










#endif // CLIENTWIN_H
