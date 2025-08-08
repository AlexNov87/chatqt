#include "mainwindow.h"
void GraphicsServer::OnNewConnection(){

    while (hasPendingConnections()) {
        QTcpSocket *sock = nextPendingConnection();
        if(!sock){
            return;
        }

        SocketComplect complect;
        complect.socket = sock;
        _socket_db[sock] = std::move(complect);

       connect(sock, &QTcpSocket::readyRead,
               this, &GraphicsServer::OnReadyRead);
       connect(sock, &QTcpSocket::disconnected,
               this, &GraphicsServer::OnDisconnected);
    }
}

void GraphicsServer::OnReadyRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(!clientSocket || !_socket_db.contains(clientSocket)){
        return;
    }
    SocketComplect* complect = &_socket_db.at(clientSocket);
    ExecuteIncoming(shared_from_this(), complect);


}

void GraphicsServer::OnDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;
    clientSocket->deleteLater();
   // QMessageBox::critical(nullptr, "", "Client disconnected");
}


