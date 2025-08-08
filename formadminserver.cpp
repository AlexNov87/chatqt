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
    connect(ui->pb_users_unblockuser,&QCommandLinkButton::clicked, this,
            &AdminServerForm::OnUnblockUserClicked
            );
    connect(ui->pb_users_deleteuser , &QCommandLinkButton::clicked,
            this, &AdminServerForm::OnDeleteUserClicked
            );
    connect(ui->pb_users_set_role, &QCommandLinkButton::clicked,
            this, &AdminServerForm::OnModifyUserRoleClicked
            );

    connect(ui->pb_users_finduser, &QCommandLinkButton::clicked,
            this, &AdminServerForm::OnFindUsersClicked);

    connect(ui->pb_users_updateusers, &QCommandLinkButton::clicked,
            this, &AdminServerForm::OnUpdateUsersClicked);

    connect(ui->pb_get_all_users, &QCommandLinkButton::clicked,
            this, &AdminServerForm::OnGetAllUsersClicked);

    connect(this, &QWidget::destroyed, [&]{ _srv->_is_admin_showed = false;});
    UpdateRoomsInternal();

}

void AdminServerForm::OnDeleteRoomClicked() {
    if(ui->lbl_rooms_roomname->text() == this->_EMPTY_ROOM_LBL
        || ui->lbl_rooms_roomname->text().isEmpty()){
        return;
    }
    const auto& master = _srv->_sql_work->GetMaster();
    auto obj = _srv->DeleteRoomJs(
        master.first, master.second,
        ui->lbl_rooms_roomname->text()
        );
    bool err = json::IsErrorJsonObject(obj);
    if(err){
        NonBlockingErrorBox(obj);
        return;
    }
    ui->lbl_rooms_roomname->setText(_EMPTY_ROOM_LBL);
    UpdateRoomsInternal();
}
void AdminServerForm::OnCreateRoomClicked() {
    if(ui->le_rooms_addroom->text().isEmpty()){return;}
    const auto& master = _srv->_sql_work->GetMaster();

    auto obj = _srv->AddRoomJs(
        master.first, master.second,
        ui->le_rooms_addroom->text()
        );
    if(json::IsErrorJsonObject(obj)){
        NonBlockingErrorBox(obj);
    }
    UpdateRoomsInternal();
}

void AdminServerForm::OnBlockUserClicked() {
    if(ui->lbl_users_usertinfoname->text().isEmpty()){
        return;
    }
    const auto& master = _srv->_sql_work->GetMaster();
    auto obj = _srv->BanUserJs(
        master.first, master.second,
        ui->lbl_users_usertinfoname->text()
        );
    if(json::IsErrorJsonObject(obj)){
        NonBlockingErrorBox(obj);
        return;
    }
    QMessageBox::information(this, "BLOCK_USER","BLOCK_USER_SUCCESS");
}

void AdminServerForm::OnUnblockUserClicked() {
    if(ui->lbl_users_usertinfoname->text().isEmpty()){
        return;
    }
    const auto& master = _srv->_sql_work->GetMaster();
    auto obj = _srv->UnBanUserJs(
        master.first, master.second,
        ui->lbl_users_usertinfoname->text()
        );
    if(json::IsErrorJsonObject(obj)){
        NonBlockingErrorBox(obj);
        return;
    }
    QMessageBox::information(this, "UNBLOCK_USER","UNBLOCK_USER_SUCCESS");
}


void AdminServerForm::OnDeleteUserClicked() {
    const auto& master = _srv->_sql_work->GetMaster();
    auto obj = _srv->DeleteUserJs(master.first, master.second,
    ui->lbl_users_usertinfoname->text());
    if(json::IsErrorJsonObject(obj)){
        NonBlockingErrorBox(obj);
        return;
    }
    OnFindUsersClicked();
}
void AdminServerForm::OnModifyUserRoleClicked()  {
    const auto& master = _srv->_sql_work->GetMaster();
    auto text = "ROLE_" + ui->comboBox->currentText();
    if(!_NAME_ROLE.contains(text)){
        FatalErrorMessageBox("NO ROLE:"+ text);
        return;
    }
    Role role = _NAME_ROLE.at(text);
   auto obj = _srv->UpdateUserRoleJs
        (master.first, master.second,
        ui->lbl_users_usertinfoname->text(), role);
    if(json::IsErrorJsonObject(obj)){
        NonBlockingErrorBox(obj);
        return;
    }
    OnFindUsersClicked();
    ui->lbl_users_usertinforole->setText(text);
    QMessageBox::information(this, "CHANGE_ROLE","CHANGE_ROLE_SUCCESS");
}
void AdminServerForm::OnUpdateUsersClicked(){
    OnFindUsersClicked();
}

void AdminServerForm::OnFindUsersClicked() {

    if(ui->le_users_startname->text().isEmpty()){
        OnGetAllUsersClicked();
        return;
    }
    str_type pattern = ui->le_users_startname->text();
    const auto& base_users = _srv->_sql_work->AllUsersBase();
    int row = 0;
    for (auto&& [name, us_role] : base_users){
        if(!name.contains(pattern)){continue;}
        if (row == ui->table_users->rowCount()){
            ui->table_users->insertRow(row);
        }
        ui->table_users->setItem(row, 0,
                                 new QTableWidgetItem(name));
        ui->table_users->setItem(row++, 1,
                                 new QTableWidgetItem(_ROLE_NAME.at(us_role.role)));
    }
    ui->table_users->setRowCount(row);
}

void AdminServerForm::OnGetAllUsersClicked(){
    const auto& base_users = _srv->_sql_work->AllUsersBase();
    ui->table_users->setRowCount(base_users.size());
    int row = 0;
    for (auto&& [name, us_role] : base_users){
        ui->table_users->setItem(row, 0,
        new QTableWidgetItem(name));
        ui->table_users->setItem(row++, 1,
        new QTableWidgetItem(_ROLE_NAME.at(us_role.role)));
    }
}

void AdminServerForm::TabRoomsSelected(){
    if(ui->table_rooms->rowCount() != 0){return;}
    UpdateRoomsInternal();
}

void AdminServerForm::UpdateRoomsInternal(){
    LGR(_srv->_mtx_room);
    int curr_row = 0;
    for (auto &&[name, room] : _srv->_rooms){
        if(curr_row == ui->table_rooms->rowCount()){
        ui->table_rooms->insertRow(curr_row);
        }
        ui->table_rooms->setItem(curr_row, 0, new QTableWidgetItem(name));
        ui->table_rooms->setItem(curr_row++, 1, new QTableWidgetItem(room->GetCreator()));
    }
    ui->table_rooms->setRowCount(curr_row);
}

void AdminServerForm::OnUpdateRoomClicked(){
    UpdateRoomsInternal();
}
