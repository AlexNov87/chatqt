#ifndef CLIENT_DESIGNER_H
#define CLIENT_DESIGNER_H
#include<QCommandLinkButton>
#include"../structs.h"


class ClientDesigner{
public:
    static void ChangeConnectionButtonOn(QCommandLinkButton* btn){
        btn->setStyleSheet(R"(
        QCommandLinkButton {
        color: white;              /* цвет основного текста */
        background-color: green;  /* цвет фона кнопки */
        }
        )");
        btn->setText("Press to disconnect");
    }

    static void ChangeConnectionButtonOff(QCommandLinkButton* btn){
        btn->setStyleSheet(R"(
        QCommandLinkButton {
        color: white;              /* цвет основного текста */
        background-color: red;  /* цвет фона кнопки */
        }
        )");
        btn->setText("Press to connect");
    }

};



#endif // CLIENT_DESIGNER_H
