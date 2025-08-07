#ifndef FORMADMIN_H
#define FORMADMIN_H

#include <QWidget>
#include<QListWidgetItem>
#include "constants.h"
#include "constants_load.h"
class ClientWin;
class GraphicsServer;


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

protected:
    Ui::Formadmin *ui;
    const str_type _EMPTY_ROOM_LBL = "Press on roomlist to select room";
protected:

    virtual void OnDeleteRoomClicked() = 0;
    virtual void OnCreateRoomClicked() = 0;
protected:
    virtual void OnBlockUserClicked() = 0;
    virtual void OnUnblockUserClicked() = 0;
    virtual void OnDeleteUserClicked() = 0;
    virtual void OnModifyUserRoleClicked() = 0;
    virtual void OnUpdateUsersClicked() = 0;
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
    void OnUnblockUserClicked() override;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override ;
    void OnUpdateUsersClicked() override ;
    ClientWin* _clientwin;
};

class AdminServerForm : public Formadmin {
public:
    AdminServerForm(GraphicsServer* srv);
    void Init();

private:
    void OnDeleteRoomClicked() override ;
    void OnCreateRoomClicked() override ;
private:
    void OnBlockUserClicked() override ;
    void OnDeleteUserClicked() override;
    void OnModifyUserRoleClicked() override;
    void OnUpdateUsersClicked() override ;
    void OnUnblockUserClicked() override ;

    GraphicsServer* _srv;
};

#endif // FORMADMIN_H
