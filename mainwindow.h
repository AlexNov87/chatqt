#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "server_graphics.h"
#include "answer_obj.h"

//UI
#include "./ui_mainwindow.h"
#include "formadmin.h"
#include "ui_formadmin.h"
//

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ServerSession {
public:
    ServerSession(std::shared_ptr<GraphicsServer> srv
                  , SocketComplect* sock);
    void Execute();
private:
    void MakeErrorAnsweToSocket(const json_obj& answer_obj);
    std::optional<json_obj> FirstStepCheckErrors(const json_obj& js_obj);
    bool IsPointersValid();
    json_obj ExecuteExternal(const json_obj& obj);

    std::shared_ptr<GraphicsServer> _srv;
    SocketComplect* _sock;
};

class ServerAdminSession {
public:
    ServerAdminSession(std::shared_ptr<GraphicsServer> srv
     , SocketComplect* sock) : _srv(srv), _sock(sock){}

private:
    std::shared_ptr<GraphicsServer> _srv;
    SocketComplect* _sock;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(std::shared_ptr<GraphicsServer> srv);
    ~MainWindow();
protected:
    friend class MainWindowDesigner;
    friend class GraphicsServer;
    friend class ServerSession;
    Ui::MainWindow *ui;
    std::shared_ptr<MainWindowDesigner> _designer;
    std::shared_ptr<GraphicsServer> _srv;
private slots:
    void on_pb_run_server_clicked();
    void on_pb_stop_server_clicked();
    void on_pb_setoptions_clicked();
    void on_commandLinkButton_clicked();
};

class GraphicsServer : public ServerBase, public GraphicWidgets,
                       public std::enable_shared_from_this<GraphicsServer> {
    Q_OBJECT
public:

    GraphicsServer();
    void InitAndRun();

    json_obj SetIP( str_type ip) override;
    json_obj SetPort(int port) override ;
    json_obj SetMaxUsers(int max) override;

    json_obj AddRoomJs( str_type name,  str_type password,  str_type roomname) override ;
    json_obj DeleteRoomJs( str_type name,  str_type password,  str_type roomname) override;

    json_obj LoginUserJs(str_type name,  str_type password) override;

    json_obj JoinRoomUserJs( str_type name,  str_type password,
                         str_type roomname, SocketComplect* complect) override ;
    json_obj RegisterUserJs( str_type name,  str_type password) override ;
    json_obj DeleteUserJs( str_type name,  str_type password,  str_type to_delete) override ;

    json_obj GetRoomsJs() override ;
    json_obj GetRoomUsersJs( str_type roomname) override;

    json_obj DisconnectJs( str_type token,  str_type room_name) override;

    json_obj PublicMessageJs
        ( str_type token,  str_type message,  str_type room_name) override;

    json_obj PrivateMessageJs
        ( str_type token,  str_type message,
    str_type user_to, str_type room_name) override;

   json_obj UpdateUserRoleJs(str_type name, str_type password,
             str_type to_update, Role role) override;

   json_obj BanUserJs(str_type name, str_type password,
                                      str_type to_ban) override;
   json_obj UnBanUserJs(str_type name, str_type password,
                                       str_type to_ban) override;

private slots:
    void OnNewConnection();
    void OnReadyRead();
    void OnDisconnected();

private:
    friend class MainWindow;
    friend class ServerSession;
    void InitGraphicForms();
    void SetDefaultValues();
};
#endif // MAINWINDOW_H
