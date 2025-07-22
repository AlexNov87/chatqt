#include "mainwindow.h"

MainWindow::MainWindow(std::shared_ptr<GraphicsServer> srv)
    : QMainWindow(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _srv = srv;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRooms_triggered()
{
    _srv->_rooms_form->show();
}


void RoomsForm::on_lw_rooms_itemClicked(QListWidgetItem *item)
{

}

