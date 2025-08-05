#include "formadmin.h"
#include "ui_formadmin.h"

Formadmin::Formadmin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Formadmin)
{
    ui->setupUi(this);
    ui->lbl_rooms_roomname->setText(_EMPTY_ROOM_LBL);
}

Formadmin::~Formadmin()
{
    delete ui;
}

void Formadmin::on_lw_rooms_roomlist_itemDoubleClicked(QListWidgetItem *item)
{
    ui->lbl_rooms_roomname->setText(item->text());
}



