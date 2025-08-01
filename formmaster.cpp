#include "formmaster.h"
#include "ui_formmaster.h"

Formmaster::Formmaster(const str_type& label)
    : QDialog(nullptr)
    , ui(new Ui::Formmaster)
{
    ui->setupUi(this);
    QPushButton* okButton = ui->bb_choose->button(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &Formmaster::CheckToAccept);
    ui->lbl_header->setText(label);
}

Formmaster::~Formmaster()
{
    delete ui;
}

void Formmaster::CheckToAccept(){
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

    if(pw1.size() > 49){
        FatalErrorMessageBox("Password is too long (max 49)");
        return;
    }

    _name = std::move(nm);
    unsigned int hsh =  PassHash(pw1);
    _pass = QString::number(hsh);
    accept();
}

void Formmaster::on_bb_choose_accepted()
{
    CheckToAccept();
}

