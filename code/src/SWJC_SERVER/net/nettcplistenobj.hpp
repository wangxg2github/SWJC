#ifndef NETTCPLISTENOBJ_HPP
#define NETTCPLISTENOBJ_HPP


#endif // NETTCPLISTENOBJ_H
#include <QObject>
#include <QHostAddress>
#include "nettcpserver.hpp"


class ZP_netListenThreadObj : public QObject
{
    Q_OBJECT
private:
    ZP_TcpServer    *m_tcpServer;
    QString         m_id;   //The listen ID
    QHostAddress    m_addressIP;
    quint16         m_addressPort;

public:
    /**
    * @brief    监听线程构造函数
    * @para     para
    * @date     2019-04-07
    */
    explicit ZP_netListenThreadObj(QString  id, QHostAddress address, quint16 port, bool bSSL = false, QObject *parent = 0)
    {
        Q_UNUSED(bSSL)
        Q_UNUSED(parent)
        m_id = id;
        m_addressIP = address;
        m_addressPort = port;

        m_tcpServer = NULL;
    }

public slots:
    /**
    * @brief    监听线程启动函数入口，完成监听，并将新客户端连接请求泵出
    * @para     id -> 连接标志ID
    * @date     2019-04-07
    */
    void on_startListen(QString  id)
    {
        if (id != m_id)
        {
            evt_Message(this, "id != m_id");
            return;
        }
        if (NULL == m_tcpServer)
        {
            m_tcpServer = new ZP_TcpServer(this);
            connect (m_tcpServer,
                     &ZP_TcpServer::evt_NewClientArrived,
                     this,
                     &ZP_netListenThreadObj::evt_NewClientArrived,
                     Qt::QueuedConnection);

            if (false == m_tcpServer->listen(m_addressIP, m_addressPort))
            {
                disconnect(m_tcpServer,
                           &ZP_TcpServer::evt_NewClientArrived,
                           this,
                           &ZP_netListenThreadObj::evt_NewClientArrived);
                emit evt_Message(this, "Error>" + QString(tr("Can not start listen!")));
                m_tcpServer->deleteLater();
                m_tcpServer = NULL;

                //Close this thread.
                emit evt_ListenClosed(m_id);
            }
        }
        return;
    }

    /**
    * @brief    停止监听线程
    * @para     id -> 连接标志ID
    * @date     2019-04-07
    */
    void on_stopListen(QString  id)
    {
        if (id == m_id)
        {
            if (m_tcpServer)
            {
                disconnect (m_tcpServer,
                            &ZP_TcpServer::evt_NewClientArrived,
                            this,
                            &ZP_netListenThreadObj::evt_NewClientArrived);

                m_tcpServer->close();
                m_tcpServer->deleteLater();
                m_tcpServer = NULL;

                //Close this thread.
                emit evt_Message(this, "Info>" + QString(tr("Listen Closed!")));
                emit evt_ListenClosed(id);
            }
        }
        return;
    }
signals:
    void evt_Message(QObject *, QString);
    void evt_ListenClosed(QString);

    //This message will tell thread pool, a new incoming connection has arrived.
    void evt_NewClientArrived(qintptr socketDescriptor);

};

