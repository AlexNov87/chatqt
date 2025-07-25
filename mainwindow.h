#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QTcpServer>
#include <QStringListModel>
#include "structs_ch.h"
#include "answer_obj.h"
#include <chrono>

//UI
#include "ui_roomsform.h"
#include "./ui_mainwindow.h"
//
class MainWindow;
class GraphicsServer;
class MainWindowDesigner;
class ServerSession;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace Ui {
class RoomsForm;
}

class RoomsForm : public QWidget
{
    Q_OBJECT

public:
    explicit RoomsForm(std::shared_ptr<GraphicsServer> srv);
    ~RoomsForm();
protected:

    bool AddRoomTolist(const QString& room);
    bool HasRoom(const QString& room);
    bool RemoveRoomFromList(const QString& room);

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
    friend class ServerSession;
    Ui::MainWindow *ui;
    std::shared_ptr<MainWindowDesigner> _designer;
    std::shared_ptr<GraphicsServer> _srv;
private slots:
    void on_actionRooms_triggered();
    void on_pb_run_server_clicked();
    void on_pb_stop_server_clicked();
    void on_pb_setoptions_clicked();
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
    friend class ServerSession;
    std::shared_ptr<MainWindow> _maiwin;
    std::shared_ptr<MainWindowDesigner> _maiwindes;
    std::shared_ptr<RoomsForm>_rooms_form;
    // std::shared_ptr<>();
};

class ServerSession {

public:
    ServerSession(const std::shared_ptr<GraphicsServer> srv, QTcpSocket* sock)
        : _srv(srv), _sock(sock){}

    void Execute(){

        //Проверяем валидность указателей на сокет и сервер.
        if(!IsPointersValid()){return;}

        //Reading to Json-object from socket
        std::optional<json_obj> answer_obj = json::ReadJsonObjectFromSocket(_sock);
        if (!answer_obj){
            return;
        }

        //Проверяем текущий объект на валидность. Если объект будет не вален,
        //то вернется объект, содержащий ошибку.
        std::optional<json_obj> err_obj = FirstStepCheckErrors(*answer_obj);
        if(err_obj){
            MakeErrorAnsweToSocket(*err_obj);
            return;
        }

        WriteToSocketWithFlush(_sock, QString("RESPONCE").toUtf8());
    }

private:
    void MakeErrorAnsweToSocket(const json_obj& answer_obj);

    std::optional<json_obj> FirstStepCheckErrors(const json_obj& js_obj){
        //Проверка, если данный объект является объектом ошибки.
        bool is_err = json::IsErrorJsonObject(js_obj);
        if(is_err){
            return js_obj;
        }

        //Есть ли в объекте поле действия.
        auto reason = json::IsContainsFieldAndNotEmpty(js_obj, CONSTANTS::LF_ACTION);
        if(reason){
            return ans_obj::MakeErrorObject(*reason, ACTIONS::SYSTEM);
        }

        //Если это действие есть в списке
        str_type act_value = (js_obj).value(CONSTANTS::LF_ACTION).toString();
        if(!_ACT_SERVER.contains(act_value)){
            return ans_obj::MakeErrorObject("Can not find action:" + act_value, ACTIONS::SYSTEM);
        };
        return std::nullopt;

    }

    bool IsPointersValid(){
        if(!_srv || !_sock){
            if(!_srv){FatalErrorMessageBox("The server is null");}
            if(!_sock){FatalErrorMessageBox("The socket is null");}
            return false;
        }
        return true;
    }

    std::shared_ptr<GraphicsServer> _srv;
    QTcpSocket* _sock;
    std::shared_ptr<ServerSession> _self;
};


class GraphicsServer : public ServerBase, public GraphicWidgets,
        public std::enable_shared_from_this<GraphicsServer> {
      Q_OBJECT
public:

      GraphicsServer();
      void InitAndRun();

    QJsonObject SetIP(QString ip) override ;
    QJsonObject SetPort(int port) override ;
    QJsonObject SetMaxUsers(int max) override;

    QJsonObject AddRoomJs(QString name, QString password, QString roomname) override ;
    QJsonObject DeleteRoomJs(QString name, QString password, QString roomname) override;

    QJsonObject LoginUserJs(QString name, QString password, QString roomname) override ;
    QJsonObject RegisterUserJs(QString name, QString password) override ;
    QJsonObject DeleteUserJs(QString name, QString password, QString to_delete) override ;

    QJsonObject GetRoomsJs() override ;
    QJsonObject GetRoomUsers(QString roomname) override;

private slots:
    void OnNewConnection();
    void OnReadyRead()
    {
        QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
        if(!clientSocket){
            FatalErrorMessageBox("sock is nullpiiiiiiiiiiiii");
        }
        std::shared_ptr<ServerSession> session =
        std::make_shared<ServerSession>(shared_from_this(), clientSocket);
        session->Execute();
    }

    void OnDisconnected()
    {
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket)
            return;
        clientSocket->deleteLater();
        QMessageBox::critical(nullptr, "", "Client disconnected");
    }

private:
    friend class MainWindow;
    friend class ServerSession;
    void InitGraphicForms();
    void SetDefaultValues();
};

#endif // MAINWINDOW_H
