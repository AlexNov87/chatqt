#include"clientwin.h"

    AdminUserForm::AdminUserForm(ClientWin* client)
    : Formadmin(), _clientwin(client) {
        Init();
    }

    void AdminUserForm::Init(){

        connect(ui->pb_rooms_createroom, &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnCreateRoomClicked);
        connect(ui->pb_rooms_delete_room, &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnDeleteRoomClicked
                );

        connect(ui->pb_users_blockuser , &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnBlockUserClicked
                );
        connect(ui->pb_users_deleteuser , &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnDeleteUserClicked
                );
        connect(ui->pb_users_set_role, &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnModifyUserRoleClicked
                );
        connect(ui->pb_users_updateusers, &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnUpdateUsersClicked
                );
        connect(this, &QWidget::destroyed , [&]{ _clientwin->_admin_opened = false; });
    }


    void AdminUserForm::OnDeleteRoomClicked() {
        FatalErrorMessageBox("1DeleteRoom CLICKED");
    }
    void AdminUserForm::OnCreateRoomClicked()  {
        FatalErrorMessageBox("1CreateRoom CLICKED");
    }

    void AdminUserForm::OnBlockUserClicked() {
        FatalErrorMessageBox("1BlockUser CLICKED");
    }
    void AdminUserForm::OnDeleteUserClicked() {
        FatalErrorMessageBox("1DeleteUser CLICKED");
    }
    void AdminUserForm::OnModifyUserRoleClicked() {
        FatalErrorMessageBox("1ModifyUser CLICKED");
    }
    void AdminUserForm::OnUpdateUsersClicked() {
        FatalErrorMessageBox("1UpdateUsers CLICKED");
    }
