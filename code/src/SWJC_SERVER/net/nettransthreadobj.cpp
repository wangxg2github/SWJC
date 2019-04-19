#include "nettransthreadobj.h"


ZP_netTransThreadObj::ZP_netTransThreadObj(QObject *parent)
{
    Q_UNUSED(parent)
    m_isActive = true;
}

ZP_netTransThreadObj::~ZP_netTransThreadObj()
{

}

bool ZP_netTransThreadObj::isActive()
{
    return m_isActive;
}

bool ZP_netTransThreadObj::canExit()
{
    if (m_isActive == true)
        return false;

    if (currentClientCount() > 0)
        return false;
    return true;
}

int ZP_netTransThreadObj::currentClientCount()
{
    int count = 0;
    m_mutexProtect.lock();
    count = m_clientList.size();
    m_mutexProtect.unlock();
    return count;
}

void ZP_netTransThreadObj::on_NewConnection(ZP_netTransThreadObj *pTransObj, qintptr sock)
{
    if (pTransObj != this)
        return;

    QTcpSocket *pClientSock = new QTcpSocket(this);

    if (pClientSock)
    {
        //Initial content
        if (true == pClientSock->setSocketDescriptor(sock))
        {
            connect(pClientSock, &QTcpSocket::readyRead, this, &ZP_netTransThreadObj::on_RecievedData, Qt::QueuedConnection);
            connect(pClientSock, &QTcpSocket::disconnected, this, &ZP_netTransThreadObj::on_ClientClosed, Qt::QueuedConnection);
            connect(pClientSock, &QTcpSocket::bytesWritten, this, &ZP_netTransThreadObj::on_SendedData, Qt::QueuedConnection);
            //connect(pClientSock, &QTcpSocket::error, this, &ZP_netTransThreadObj::on_DisplayError, Qt::QueuedConnection);

            m_mutexProtect.lock();
            m_clientList.insert(pClientSock);
            m_mutexProtect.unlock();

            emit evt_message(pClientSock, QString("New Client Connect. [%1:%2]").arg(pClientSock->peerAddress().toString()
                                                                                     .arg(pClientSock->peerPort())));
        }
        //        else
        //            push_to_rabish_can(sock_client);
    }
    return;
}

void ZP_netTransThreadObj::on_RecievedData()
{
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    if (pSock)
    {
        quint64 extraData = 0;
        if (m_hashExtraData.contains(pSock))
            extraData = m_hashExtraData[pSock];

        QByteArray array = pSock->readAll();

        cout << "Current Thread" << QThread::currentThreadId() << "Client Num:" << m_clientList.size() << "[" <<pSock->peerAddress().toString()
             << ":" << pSock->peerPort() << "]" << array;
    }
}
void ZP_netTransThreadObj::on_ClientClosed()
{
    cout << "Client Close.";
    quint64 sockDescriptor = 0;
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());

    if (pSock && m_clientList.contains(pSock))
    {
        sockDescriptor = pSock->socketDescriptor();
        pSock->close();
        pSock->deleteLater();

        m_mutexProtect.lock();
        m_clientList.remove(pSock);
        m_mutexProtect.unlock();

        emit evt_disconnetFromClient(sockDescriptor);
    }

}

void ZP_netTransThreadObj::on_SendedData(qint64 bytes)
{
    Q_UNUSED(bytes)
}

void ZP_netTransThreadObj::on_DisplayError(QAbstractSocket::SocketError errMsg)
{
    Q_UNUSED(errMsg)
}
