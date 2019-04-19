#ifndef NETTCPSERVER_HPP
#define NETTCPSERVER_HPP

#include <QTcpServer>

class ZP_TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    ZP_TcpServer(QObject *parent)
    {
        Q_UNUSED(parent)
    }
protected:
    /**
    * @brief    新的tcp链接直接泵出，交由监听线程处理
    * @para
    * @date     2019-04-06
    */
    void incomingConnection(qintptr socketDescriptor)
    {
        emit evt_NewClientArrived(socketDescriptor);
    }
signals:
    void evt_NewClientArrived(qintptr socketDescriptor);
};


#endif // NETTCPSERVER_H
