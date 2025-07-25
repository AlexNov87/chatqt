#include "mainwindow.h"
void GraphicsServer::OnNewConnection(){

    while (hasPendingConnections()) {
        QTcpSocket *sock = nextPendingConnection();
       if(!sock){
            return;
        }
       connect(sock, &QTcpSocket::readyRead,
               this, &GraphicsServer::OnReadyRead);
       connect(sock, &QTcpSocket::disconnected,
               this, &GraphicsServer::OnDisconnected);
    }
}


