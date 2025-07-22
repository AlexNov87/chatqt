#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<GraphicsServer> server = std::make_shared<GraphicsServer>();
    server->InitAndRun();
    return a.exec();
}
