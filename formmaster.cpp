#include "formmaster.h"
#include "ui_formmaster.h"

Formmaster::Formmaster()
    : QDialog(nullptr)
    , ui(new Ui::Formmaster)
{
    ui->setupUi(this);
    TurnOffOk();
}

Formmaster::~Formmaster()
{
    delete ui;
}

void Formmaster::SetOkState(bool state){
    QPushButton* button = ui->bb_choose->button(QDialogButtonBox::Ok);
    button->setEnabled(state);
}

void Formmaster::TurnOffOk(){
    SetOkState(false);
}
void Formmaster::TurnOnOk(){
    SetOkState(true);
}

void Formmaster::on_pb_create_master_clicked(){
    str_type nm = ui->le_admin_name->text();
    str_type pw1 = ui->le_password ->text();
    str_type pw2 = ui->le_password_repeat->text();

    if(nm.isEmpty() || pw1.isEmpty() || pw2.isEmpty()){
        FatalErrorMessageBox("One ore more fields are empty");
        return;
    }

    if(nm.size() > 49){
        FatalErrorMessageBox("Name is too long (max 49)");
        return;
    }

    if(pw1 != pw2){
       FatalErrorMessageBox("Passwords are not equal");
        return;
    }

    // if(pw1.size() < 1){
    //     FatalErrorMessageBox("Password is too short");
    //     return;
    // }

    if(pw1.size() > 49){
        FatalErrorMessageBox("Password is too long (max 49)");
        return;
    }

    _admin_name = std::move(nm);
     unsigned int hsh =  PassHash(pw1);
    _admin_pass = QString::number(hsh);
    TurnOnOk();
}
