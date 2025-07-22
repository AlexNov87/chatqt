#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QTcpServer>
#include <memory>
#include <QStringListModel>
#include "structs_ch.h"
#include "answer_obj.h"

//UI
#include "ui_roomsform.h"
#include "./ui_mainwindow.h"
//


class MainWindow;
class GraphicsServer;
class MainWindowDesigner;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace Ui {
class RoomsForm;
}
///////////////////
class RoomsForm : public QWidget
{
    Q_OBJECT

public:
    explicit RoomsForm(std::shared_ptr<GraphicsServer> srv);
    ~RoomsForm();
protected:

    bool AddRoomTolist(const QString& room){
        if(HasRoom(room)) {return false;}
        ui->lw_rooms->addItem(room);
        return true;
    }

    bool HasRoom(const QString& room){
        auto rooms = ui->lw_rooms->findItems(room, Qt::MatchExactly);
        return (!rooms.isEmpty());
    }

    bool RemoveRoomFromList(const QString& room){
       if(!HasRoom(room)) {return false;}
       auto rooms = ui->lw_rooms->findItems(room, Qt::MatchExactly);
       for (auto&& room : rooms){
           delete ui->lw_rooms->takeItem(ui->lw_rooms->row(room));
       }
       return true;
    }

    std::shared_ptr<GraphicsServer> _srv;
    friend class GraphicsServer;
    friend class MainWindow;
    Ui::RoomsForm *ui;
private slots:
    void on_lw_rooms_itemClicked(QListWidgetItem *item);
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
    Ui::MainWindow *ui;
    std::shared_ptr<MainWindowDesigner> _designer;
    std::shared_ptr<GraphicsServer> _srv;
private slots:
    void on_actionRooms_triggered();
};

class MainWindowDesigner
{
public:
    MainWindowDesigner(std::shared_ptr<MainWindow> mainwin);
    void EditStatusLabel(QString text, QColor color);
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
    std::shared_ptr<MainWindow> _maiwin;
    std::shared_ptr<MainWindowDesigner> _maiwindes;
    std::shared_ptr<RoomsForm>_rooms_form;
    // std::shared_ptr<>();
};

class GraphicsServer : public ServerBase, public GraphicWidgets,
        public std::enable_shared_from_this<GraphicsServer> {
public:

    void InitAndRun();

    QJsonObject SetIP(QString ip) override ;
    QJsonObject SetPort(int port) override ;
    QJsonObject SetMaxUsers(int max) override;

    QJsonObject AddRoomJs(QString name, QString password, QString roomname) override ;
    QJsonObject DeleteRoomJs(QString name, QString password, QString roomname) override;

    QJsonObject LoginUserJs(QString name, QString password, QString roomname) override ;
    QJsonObject RegisterUserJs(QString name, QString password) override ;
    QJsonObject DeleteUserJs(QString name, QString password, QString to_delete) override ;

    QJsonObject GetRoomsJs() override {

        ACTIONS this_act = ACTIONS::GET_ROOMS_LIST;
        auto lam = [&]{

            QJsonArray arr;
            {
                LG(_mtx_room);
                for(auto&& room : _rooms){
                    arr.push_back(room.first);
                }
            }


            //!!!!!!!!!!!!!!!!!!!!!!!!!
            return QJsonObject{};
        };
        return ans_obj::GuardExceptSetter(lam, this_act);

    }
private:

    friend class MainWindow;
    void InitGraphicForms();
    void SetDefaultValues();

};



#endif // MAINWINDOW_H
