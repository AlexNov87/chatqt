#include "mainwindow.h"

bool RoomsForm::AddRoomTolist(const QString& room){
    if(HasRoom(room)) {return false;}
    ui->lw_rooms->addItem(room);
    return true;
}

bool RoomsForm::HasRoom(const QString& room){
    auto rooms = ui->lw_rooms->findItems(room, Qt::MatchExactly);
    return (!rooms.isEmpty());
}

bool RoomsForm::RemoveRoomFromList(const QString& room){
    if(!HasRoom(room)) {return false;}
    auto rooms = ui->lw_rooms->findItems(room, Qt::MatchExactly);
    for (auto&& room : rooms){
        delete ui->lw_rooms->takeItem(ui->lw_rooms->row(room));
    }
    return true;
}

void RoomsForm::on_lw_rooms_itemClicked(QListWidgetItem *item)
{

}

///////////////////////////////////////////////////////////////


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


//МЕНЮ КОМНАТ
void MainWindow::on_actionRooms_triggered()
{
    _srv->_rooms_form->show();
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
