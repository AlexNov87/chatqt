#include "mainwindow.h"
#include "ui_formserverpereferences.h"

FormServerPereferences::FormServerPereferences(std::shared_ptr<GraphicsServer> srv)
    : _srv(srv), ui(new Ui::FormServerPereferences)
{
    ui->setupUi(this);
}

FormServerPereferences::~FormServerPereferences()
{
    delete ui;
}



