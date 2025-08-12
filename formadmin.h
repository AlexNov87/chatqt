#ifndef FORMADMIN_H
#define FORMADMIN_H

#include <QWidget>
#include<QListWidgetItem>
#include "structs.h"
class ClientWin;
class GraphicsServer;
#include"helpfoo.h"

namespace Ui {
class Formadmin;
}

class Formadmin : public QWidget
{
    Q_OBJECT

public:
    explicit Formadmin(QWidget *parent = nullptr);
    ~Formadmin();

private slots:
    void on_lw_rooms_roomlist_itemDoubleClicked(QListWidgetItem *item);
    void on_table_users_cellClicked(int row, int column);

    void on_tw_adminpanel_tabBarClicked(int index);

    void on_table_rooms_cellClicked(int row, int column);

protected:
    Ui::Formadmin *ui;
    const str_type _EMPTY_ROOM_LBL = "Press on roomlist to select room";
protected:

    virtual void OnDeleteRoomClicked() = 0;
    virtual void OnCreateRoomClicked() = 0;
    virtual void OnUpdateRoomClicked() = 0;
protected:
    virtual void OnBlockUserClicked() = 0;
    virtual void OnUnblockUserClicked() = 0;
    virtual void OnDeleteUserClicked() = 0;
    virtual void OnModifyUserRoleClicked() = 0;
    virtual void OnUpdateUsersClicked() = 0;
    virtual void OnFindUsersClicked() = 0;
    virtual void OnGetAllUsersClicked() = 0;
protected:
    virtual void TabUsersSelected() = 0;
    virtual void TabRoomsSelected() = 0;

};

class AdminUserForm : public Formadmin {
public:
    AdminUserForm(ClientWin* client);

private:
    void Init();
    void OnDeleteRoomClicked() override ;
    void OnCreateRoomClicked() override ;
    void OnUpdateRoomClicked() override ;
private:
    void OnBlockUserClicked() override;
    void OnUnblockUserClicked() override;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override ;
    void OnUpdateUsersClicked() override ;
    void OnFindUsersClicked() override;
    void OnGetAllUsersClicked() override;
private:
    void TabUsersSelected() override{}
    void TabRoomsSelected() override{}
private:
    void HaldleSocket();
    void FillRoomTable(const);
    void FillUserTable();

    ClientWin* _clientwin;
    SocketComplect _sock;
    std::shared_ptr<QTcpSocket> _socket_for_admin;
    QHostAddress _adrs;
};

class AnswerAdminSession
{
public:
    AnswerAdminSession(AdminUserForm* uform, const json_obj& obj);
    void StartExecute();

private:
    AdminUserForm* _user_form;
    const json_obj& _obj;

    std::optional<ADMIN_ACTIONS> GetAction();
    void ExecuteRoomList();
    void ExecuteUserslist();

    void ExecuteAddRoom();
    void ExecuteDeleteRoom();

    void ExecuteDeleteUser();
    void ExecuteBanUser();
    void ExecuteUnbanUser();

    void ExecuteUpdateRole();
    void ExecuteSystem();

};

class AdminServerForm : public Formadmin {
public:
    AdminServerForm(GraphicsServer* srv);
    void Init();

private:
    void OnDeleteRoomClicked() override ;
    void OnCreateRoomClicked() override ;
    void OnUpdateRoomClicked() override ;
private:
    void OnBlockUserClicked() override ;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override;
    void OnUpdateUsersClicked() override ;
    void OnUnblockUserClicked() override ;
    void OnFindUsersClicked() override;
    void OnGetAllUsersClicked() override;
private:
    void TabUsersSelected() override{}
    void TabRoomsSelected() override;
    void UpdateRoomsInternal();
    GraphicsServer* _srv;
};

#endif // FORMADMIN_H
