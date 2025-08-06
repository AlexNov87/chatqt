#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QTcpServer>
#include <QStringListModel>
#include "structs_ch.h"
#include "answer_obj.h"
#include <chrono>

//UI
#include "ui_roomsform.h"
#include "./ui_mainwindow.h"
#include "formadmin.h"
#include "ui_formadmin.h"
//
class MainWindow;
class GraphicsServer;
class MainWindowDesigner;
class ServerSession;
class AdminServerForm;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace Ui {
class RoomsForm;
}

class RoomsForm : public QWidget
{
    Q_OBJECT

public:
    explicit RoomsForm(std::shared_ptr<GraphicsServer> srv);
    ~RoomsForm();
protected:

    bool AddRoomTolist(const  str_type& room);
    bool HasRoom(const  str_type& room);
    bool RemoveRoomFromList(const  str_type& room);

    std::shared_ptr<GraphicsServer> _srv;
    friend class GraphicsServer;
    friend class MainWindow;
    Ui::RoomsForm *ui;
private slots:
    void on_lw_rooms_itemClicked(QListWidgetItem *item);
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
    void on_actionRooms_triggered();
    void on_pb_run_server_clicked();
    void on_pb_stop_server_clicked();
    void on_pb_setoptions_clicked();
    void on_pb_stop_server_clicked(bool checked);
};

class MainWindowDesigner
{
public:
    MainWindowDesigner(std::shared_ptr<MainWindow> mainwin);
    void EditStatusLabel( str_type text, QColor color);
    void StatusLabelOn();
    void StatusLabelOff();
protected:
    std::shared_ptr<MainWindow> _mainwin;
    QPalette _palette_status;
    std::shared_ptr<QStringListModel> _room_list_model;
};

class GraphicWidgets{
protected:
    friend class GraphicsServer;
    friend class ServerSession;
    std::shared_ptr<MainWindow> _maiwin;
    std::shared_ptr<MainWindowDesigner> _maiwindes;
    std::shared_ptr<RoomsForm>_rooms_form;
    // std::shared_ptr<>();
};

class ServerSession {

public:
    ServerSession(const std::shared_ptr<GraphicsServer> srv
                  , SocketComplect* sock);
    void Execute();
private:
    void MakeErrorAnsweToSocket(const json_obj& answer_obj);
    std::optional<json_obj> FirstStepCheckErrors(const json_obj& js_obj);
    bool IsPointersValid();
    json_obj ExecuteExternal(const json_obj& obj);

    std::shared_ptr<GraphicsServer> _srv;
    SocketComplect* _sock;
    std::shared_ptr<ServerSession> _self;
};

class GraphicsServer : public ServerBase, public GraphicWidgets,
                       public std::enable_shared_from_this<GraphicsServer> {
    Q_OBJECT
public:

    GraphicsServer();
    void InitAndRun();

    json_obj SetIP( str_type ip) override ;
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


class AdminServerForm : public Formadmin {

    AdminServerForm(): Formadmin() {
        Init();
    }

    void Init(){

        connect(ui->pb_rooms_createroom, &QCommandLinkButton::clicked,
                this, &AdminServerForm::OnCreateRoomClicked);
        connect(ui->pb_rooms_delete_room, &QCommandLinkButton::clicked,
                this, &AdminServerForm::OnDeleteRoomClicked
                );

        connect(ui->pb_users_blockuser , &QCommandLinkButton::clicked,
                this, &AdminServerForm::OnBlockUserClicked
                );
        connect(ui->pb_users_deleteuser , &QCommandLinkButton::clicked,
                this, &AdminServerForm::OnDeleteUserClicked
                );
        connect(ui->pb_users_set_role, &QCommandLinkButton::clicked,
                this, &AdminServerForm::OnModifyUserRoleClicked
                );
        connect(ui->pb_users_updateusers, &QCommandLinkButton::clicked,
                this, &AdminServerForm::OnUpdateUsersClicked
                );
    }

private:
    void OnDeleteRoomClicked() override {
        FatalErrorMessageBox("2DeleteRoom CLICKED");
    }
    void OnCreateRoomClicked() override {
        FatalErrorMessageBox("2CreateRoom CLICKED");
    }
private:
    void OnBlockUserClicked() override {
        FatalErrorMessageBox("2BlockUser CLICKED");
    }
    void OnDeleteUserClicked() override {
        FatalErrorMessageBox("2DeleteUser CLICKED");
    }
    void OnModifyUserRoleClicked() override {
        FatalErrorMessageBox("2ModifyUser CLICKED");
    }
    void OnUpdateUsersClicked() override {
        FatalErrorMessageBox("2UpdateUsers CLICKED");
    }
};

#endif // MAINWINDOW_H
