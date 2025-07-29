#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConfigInit init;
    std::shared_ptr<sql::SQLWorker> sql_work =
        std::make_shared<sql::SQLWorker>(init.Object());



  //  std::shared_ptr<GraphicsServer> server = std::make_shared<GraphicsServer>();
  //  server->InitAndRun();

  FatalErrorMessageBox("OFF");
    return a.exec();
}
