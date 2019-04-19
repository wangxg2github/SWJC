#ifndef NETTRANSTHREADOBJ_H
#define NETTRANSTHREADOBJ_H

#include <QObject>
#include <QMutex>
#include <QSet>
#include <QHash>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include "common/comm.h"

class ZP_netTransThreadObj : public QObject
{
    Q_OBJECT
public:
    ZP_netTransThreadObj(QObject *parent = 0);
    ~ZP_netTransThreadObj();

    bool isActive();
    bool canExit();
    int currentClientCount();

public slots:
    void on_NewConnection(ZP_netTransThreadObj *pTransObj, qintptr sock);

    void on_RecievedData();
    void on_ClientClosed();
    void on_SendedData(qint64 bytes);
    void on_DisplayError(QAbstractSocket::SocketError errMsg);
signals:
    void evt_message(QObject *, QString);
    void evt_disconnetFromClient(quint64 sock);
private:
    bool m_isActive;

    QSet<QTcpSocket *> m_clientList;
    QMutex m_mutexProtect;

    QHash<QObject *, quint64> m_hashExtraData;
};

#endif // NETTRANSTHREADOBJ_H
