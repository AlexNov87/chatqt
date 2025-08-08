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
        connect(ui->pb_users_unblockuser,&QCommandLinkButton::clicked, this,
                &AdminUserForm::OnUnblockUserClicked
                );
        connect(this, &QWidget::destroyed , [&]{ _clientwin->_admin_opened = false; });


        _socket_for_admin = std::make_shared<QTcpSocket>();
        _sock.socket = _socket_for_admin.get();

        if(!_adrs.setAddress(_clientwin->ui->le_setip->text())){
            FatalErrorMessageBox("Failed set ip");
        }
        _sock.socket->connectToHost(_adrs, _clientwin->ui->sb_port->value());

        FatalErrorMessageBox(_adrs.toString()+ "  " +
                             QString::number(_clientwin->ui->sb_port->value()));

        if(!_sock.socket->waitForConnected(3000)) {
            FatalErrorMessageBox("Failed to connect");
        }

        connect(_socket_for_admin.get(),
                &QTcpSocket::readyRead, this, &AdminUserForm::HaldleSocket);

    }

    void AdminUserForm::HaldleSocket(){

    }

    void AdminUserForm::OnDeleteRoomClicked() {


        FatalErrorMessageBox("1DeleteRoom CLICKED");
    }
    void AdminUserForm::OnCreateRoomClicked()  {
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

    void AdminUserForm::OnUnblockUserClicked(){
       FatalErrorMessageBox("1UnBlockUser CLICKED");
    }

    void AdminUserForm::OnFindUsersClicked() {
        FatalErrorMessageBox("1FINDUser CLICKED");
    }

    void AdminUserForm::OnUpdateRoomClicked(){
         FatalErrorMessageBox("1UPDROOM CLICKED");
    }
