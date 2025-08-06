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

void MainWindow::on_pb_run_server_clicked()
{
    if(_srv->isListening()){
        bool res = ChooseBox("The server is working now, do yo want to stop it, "
                             "and run maybe with new parameters?");
        if(!res){return;}
        this->_srv->close();
    }
    _srv->_maiwindes->StatusLabelOn();
    this->_srv->listen(this->_srv->GetIP(), this->_srv->GetPort());
}

void MainWindow::on_pb_stop_server_clicked()
{
    if(!_srv->isListening()){
        QMessageBox::warning(this,"", "The server is not running");
        return;
    }
    this->_srv->close();
    _srv->_maiwindes->StatusLabelOff();
}


void MainWindow::on_pb_setoptions_clicked()
{
    QHostAddress adress;
    bool res = adress.setAddress(ui->le_setip->text());
    if(!res){
        FatalErrorMessageBox(ui->le_setip->text() + " could not be as IP");
        return;
    }
    _srv->SetIP(ui->le_setip-> text());
    _srv->SetPort(ui->sb_port->value());
    _srv->SetMaxUsers(ui->sb_maxconn->value());
}


