#include "clientwin.h"
#include "./ui_clientwin.h"

ClientWin::ClientWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWin)
{
    ui->setupUi(this);
}

ClientWin::~ClientWin()
{
    delete ui;
}
