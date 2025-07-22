#include "mainwindow.h"
#include "ui_roomsform.h"

RoomsForm::RoomsForm(std::shared_ptr<GraphicsServer> srv)
    : QWidget(nullptr)
    , ui(new Ui::RoomsForm)
{
    ui->setupUi(this);
    _srv = srv;
}

RoomsForm::~RoomsForm()
{
    delete ui;
}
