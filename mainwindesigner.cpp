#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindowDesigner::MainWindowDesigner
    (std::shared_ptr<MainWindow> mainwin)
    : _mainwin(mainwin) {
    StatusLabelOff();
}

void MainWindowDesigner::EditStatusLabel(QString text, QColor color){
    _mainwin->ui->lbl_srv_indicator->setText(std::move(text));
    _palette_status.setColor(QPalette::Window, std::move(color));
    _mainwin->ui->lbl_srv_indicator->setPalette(_palette_status);
}

void MainWindowDesigner::StatusLabelOn(){
    EditStatusLabel("Server is running... ", Qt::darkGreen);
}

void MainWindowDesigner::StatusLabelOff(){
    EditStatusLabel("Server is stopped...", Qt::red);
}


