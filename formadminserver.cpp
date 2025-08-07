#include"mainwindow.h"
AdminServerForm::AdminServerForm(GraphicsServer* srv): Formadmin(), _srv(srv) {
    Init();
}

void AdminServerForm::Init(){

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
    connect(ui->pb_users_unblockuser,&QCommandLinkButton::clicked, this,
            &AdminServerForm::OnUnblockUserClicked
            );
    connect(this, &QWidget::destroyed, [&]{ _srv->_is_admin_showed = false;});

}

void AdminServerForm::OnDeleteRoomClicked() {
    FatalErrorMessageBox("2DeleteRoom CLICKED");
}
void AdminServerForm::OnCreateRoomClicked() {
    FatalErrorMessageBox("2CreateRoom CLICKED");
}

void AdminServerForm::OnBlockUserClicked() {
    FatalErrorMessageBox("2BlockUser CLICKED");
}
void AdminServerForm::OnDeleteUserClicked() {
    FatalErrorMessageBox("2DeleteUser CLICKED");
}
void AdminServerForm::OnModifyUserRoleClicked()  {
    FatalErrorMessageBox("2ModifyUser CLICKED");
}
void AdminServerForm::OnUpdateUsersClicked(){
    FatalErrorMessageBox("2UpdateUsers CLICKED");
}

void AdminServerForm::OnUnblockUserClicked() {
    FatalErrorMessageBox("2UnBlockUser CLICKED");
}

void AdminServerForm::OnFindUsersClicked() {
    FatalErrorMessageBox("2FindUser CLICKED");
}
