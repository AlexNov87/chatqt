#ifndef SERVER_GRAPHICS_H
#define SERVER_GRAPHICS_H
#include"structs_ch.h"
#include<QStringListModel>

//UI
#include "./ui_mainwindow.h"
#include "formadmin.h"
#include "ui_formadmin.h"
//

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow;
class GraphicsServer;
class AdminServerForm;

class MainWindowDesigner
{
public:
    MainWindowDesigner(std::shared_ptr<MainWindow> mainwin);
    void EditStatusLabel( str_type text, QColor color);
    void StatusLabelOn();
    void StatusLabelOff();
protected:
    std::shared_ptr<MainWindow> _mainwin;
    QPalette _palette_status;
    std::shared_ptr<QStringListModel> _room_list_model;
};

class GraphicWidgets{
protected:
    friend class GraphicsServer;
    friend class ServerSession;
    std::shared_ptr<MainWindow> _maiwin;
    std::shared_ptr<MainWindowDesigner> _maiwindes;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(std::shared_ptr<GraphicsServer> srv);
    ~MainWindow();
protected:
    friend class MainWindowDesigner;
    friend class GraphicsServer;
    friend class ServerSession;
    Ui::MainWindow *ui;
    std::shared_ptr<MainWindowDesigner> _designer;
    std::shared_ptr<GraphicsServer> _srv;
private slots:
    void on_pb_run_server_clicked();
    void on_pb_stop_server_clicked();
    void on_pb_setoptions_clicked();
};

#endif // SERVER_GRAPHICS_H
