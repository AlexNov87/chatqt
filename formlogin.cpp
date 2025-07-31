#include "formlogin.h"
#include "ui_formlogin.h"

FormLogin::FormLogin(const str_type& label)
    : QDialog(nullptr)
    , ui(new Ui::FormLogin)
{
    ui->setupUi(this);
    QPushButton* okButton = ui->bb_choose->button(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &FormLogin::CheckToAccept);
    ui->lbl_header->setText(label);
}

FormLogin::~FormLogin()
{
    delete ui;
}


void FormLogin::CheckToAccept(){
    str_type nm = ui->le_admin_name->text();
    str_type pw1 = ui->le_password ->text();

    if(nm.isEmpty() || pw1.isEmpty()){
        FatalErrorMessageBox("One ore more fields are empty");
        return;
    }

    if(nm.size() > 49){
        FatalErrorMessageBox("Name is too long (max 49)");
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
