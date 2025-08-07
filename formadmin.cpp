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

void Formadmin::on_table_users_cellClicked(int row, int column)
{
    FatalErrorMessageBox("Row:"+QString::number(row)+
                         " COL:"+ QString::number(column));
}


void Formadmin::on_tw_adminpanel_tabBarClicked(int index)
{
        switch (index){
          case 0:
           TabRoomsSelected();
           break;
          case 1:
              TabUsersSelected();
           break;
        };
}


void Formadmin::on_table_rooms_cellClicked(int row, int column)
{
    if(column != 0){
        return;
    }
    QTableWidgetItem* item = ui->table_rooms->item(row,0);
    ui->lbl_rooms_roomname->setText(item->text());
}

