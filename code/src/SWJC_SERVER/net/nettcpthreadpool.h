#ifndef NETTCPTHREADPOOL_HPP
#define NETTCPTHREADPOOL_HPP
#include <QObject>
#include <QThread>
#include <QMap>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>

#include "net/nettcpserver.hpp"
#include "net/nettcplistenobj.hpp"
#include "net/nettransthreadobj.h"

class ZP_netEngine : public QObject
{
    Q_OBJECT
public:
    explicit ZP_netEngine(QObject *parent = 0);
    ~ZP_netEngine();

    /**
    * @brief    新增一个监听线程
    * @para     para
    * @date     2019-04-07
    */
    void AddListeningAddress(QString  id, const QHostAddress &address , quint16 nPort, bool bSSLConn = false);

    /**
    * @brief    移除一个监听线程
    * @para     id -> 索引
    * @date     2019-04-07
    */
    void RemoveListeningAddress(QString  id);

    /**
    * @brief    移除所有的监听线程
    * @para     NA
    * @date     2019-04-07
    */
    void RemoveAllAddresses();


    void AddClientTransThreads(int threadNum);
    bool RemoveTransThreadObj(ZP_netTransThreadObj *pThreadObj);

private:
    //保存监听器的线程信息和对象信息
    QMap<QString, QThread *>                m_mapListenThreads;
    QMap<QString, ZP_netListenThreadObj *>  m_mapListenThreadObjs;

    //保存传输线程信息和对象信息
    QVector<QThread *>                      m_vecNetTransThreads;
    QVector<ZP_netTransThreadObj *>         m_vecNetTransThreadObjs;
public slots:
    /**
    * @brief    接收监听线程发出的新客户端连接信号
    * @para     socketDescriptor -> sock描述符
    * @date     2019-04-07
    */
    void on_NewClientArrived(qintptr socketDescriptor);

    /**
    * @brief    接受监听线程发出的关闭一个连接信号
    * @para     para
    * @date     2019-04-07
    */
    void on_ListenClosed(QString id);

    /**
    * @brief    接收传输线程发出的客户端断链请求
    * @para     socketDescriptor -> sock描述符
    * @date     2019-04-12
    */
    void on__disconnetFromClient(qintptr socketDescriptor);

signals:
    void evt_message(QObject *, QString);
    void evt_startListen(QString);
    void evt_stopListen(QString);
    void evt_NewConnection(ZP_netTransThreadObj *, qintptr);
};
#endif // NETTCPTHREADPOOL_HPP
