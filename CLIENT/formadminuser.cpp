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
        connect(ui->pb_rooms_updaterooms, &QCommandLinkButton::clicked,
                this, &AdminUserForm::OnUpdateRoomClicked);

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

        connect(_sock.socket, &QTcpSocket::readyRead, this,
                &AdminUserForm::HaldleSocket);

        if(!_adrs.setAddress(_clientwin->ui->le_setip->text())){
            FatalErrorMessageBox("Failed set ip");
        }
        _sock.socket->connectToHost(_adrs, _clientwin->ui->sb_port->value());

        if(!_sock.socket->waitForConnected(3000)) {
            FatalErrorMessageBox("Failed to connect");
        }

    }

    void AdminUserForm::HaldleSocket(){
        FatalErrorMessageBox("HAN SOCK ADMIN USER");

        QByteArray data = _sock.socket->readAll();
        _sock.AddToBuffer(data);
        while(auto obj = _sock.GetExecuteObject()){
            json_obj js(json::ReadJsonObjectFromQbyteArray(*obj));
            AnswerAdminSession session(this, std::move(js));
            session.StartExecute();
        }
    }

    void AdminUserForm::OnDeleteRoomClicked() {
        auto obj = req_obj::AdminMakeRequestDeleteRooms(
            _clientwin->_my_name,
            _clientwin ->_my_pass,
            ui->le_rooms_addroom->text()
            );
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }
    void AdminUserForm::OnCreateRoomClicked()  {
        auto obj = req_obj::AdminMakeRequestAddRooms(
            _clientwin->_my_name,
            _clientwin ->_my_pass,
            ui->le_rooms_addroom->text()
            );
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }

    void AdminUserForm::OnBlockUserClicked() {
        auto obj = req_obj::AdminMakeRequestBanUser(
            _clientwin->_my_name,
            _clientwin ->_my_pass,
            ui->lbl_users_usertinfoname->text()
            );
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }
    void AdminUserForm::OnDeleteUserClicked() {
        auto obj = req_obj::AdminMakeRequestDeleteUser(
            _clientwin->_my_name,
            _clientwin ->_my_pass,
            ui->lbl_users_usertinfoname->text()
            );
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }
    void AdminUserForm::OnModifyUserRoleClicked() {
        FatalErrorMessageBox("1ModifyUser CLICKED");
    }
    void AdminUserForm::OnUpdateUsersClicked() {
        auto obj = req_obj::AdminMakeRequestGetUsers() ;
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }

    void AdminUserForm::OnUnblockUserClicked(){
        auto obj = req_obj::AdminMakeRequestUnbanUser(
            _clientwin->_my_name,
            _clientwin ->_my_pass,
            ui->lbl_users_usertinfoname->text()
            );
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }

    void AdminUserForm::OnFindUsersClicked() {
        OnUpdateRoomClicked();
    }

    void AdminUserForm::OnUpdateRoomClicked(){
        auto obj = req_obj::AdminMakeRequestGetRooms();
        QByteArray data = json::WritetoQByteAnyJson(obj);
        _sock.GuardSendMessageOtherSide(data);
    }
