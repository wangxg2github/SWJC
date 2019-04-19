#include "net/nettcpthreadpool.h"


ZP_netEngine::ZP_netEngine(QObject *parent)
{
    Q_UNUSED(parent)
}

ZP_netEngine::~ZP_netEngine()
{

}

void ZP_netEngine::AddListeningAddress(QString id, const QHostAddress &address, quint16 nPort, bool bSSLConn)
{
    //判断此IP信息是否已创建监听
    if (m_mapListenThreads.find(id) == m_mapListenThreads.end())
    {
        QThread * pListenThread = new QThread(this);
        ZP_netListenThreadObj *pListenThreadObj = new ZP_netListenThreadObj(id, address, nPort, bSSLConn);

        //connect(pListenThread, &QThread::started, pListenThreadObj, &ZP_netListenThreadObj::on_startListen, Qt::QueuedConnection);
        connect(pListenThread, &QThread::finished, pListenThreadObj, &ZP_netListenThreadObj::deleteLater, Qt::QueuedConnection);
        connect(pListenThread, &QThread::finished, pListenThread, &QThread::deleteLater);

        connect(pListenThreadObj, &ZP_netListenThreadObj::evt_Message, this, &ZP_netEngine::evt_message, Qt::QueuedConnection);
        connect(pListenThreadObj, &ZP_netListenThreadObj::evt_ListenClosed, this, &ZP_netEngine::on_ListenClosed, Qt::QueuedConnection);
        connect(pListenThreadObj, &ZP_netListenThreadObj::evt_NewClientArrived, this, &ZP_netEngine::on_NewClientArrived, Qt::QueuedConnection);

        connect(this, &ZP_netEngine::evt_startListen, pListenThreadObj, &ZP_netListenThreadObj::on_startListen, Qt::QueuedConnection);
        connect(this, &ZP_netEngine::evt_stopListen, pListenThreadObj, &ZP_netListenThreadObj::on_stopListen, Qt::QueuedConnection);


        pListenThreadObj->moveToThread(pListenThread);
        pListenThread->start();

        //开启监听
        emit evt_startListen(id);

        //m_mutex_listen.lock();
        m_mapListenThreads[id] = pListenThread;
        m_mapListenThreadObjs[id] = pListenThreadObj;
        //m_mutex_listen.unlock();
    }
    else
        emit evt_message(this, "Warning>"+QString(tr("This ID has been used.")));
    return;
}

void ZP_netEngine::RemoveListeningAddress(QString id)
{
    if (m_mapListenThreads.find(id) != m_mapListenThreads.end())
    {
        emit evt_stopListen(id);
    }
    return;
}

void ZP_netEngine::RemoveAllAddresses()
{
    foreach (QString id, m_mapListenThreads.keys())
    {
        RemoveListeningAddress(id);
    }
    return;
}

void ZP_netEngine::AddClientTransThreads(int threadNum)
{
    if (threadNum > 0 && threadNum < 256)
    {
        for (int i = 0; i < threadNum; i++)
        {
            QThread * pTransThread = new QThread(this);
            ZP_netTransThreadObj * pTransThreadObj = new ZP_netTransThreadObj();
            pTransThread->start();

            //Connect signals
            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_disconnetFromClient, this, &ZP_netEngine::on__disconnetFromClient,Qt::QueuedConnection);
            //            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_Data_recieved, this, &ZP_netEngine::evt_Data_recieved, Qt::QueuedConnection);
            //            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_Data_transferred, this, &ZP_netEngine::evt_Data_transferred,Qt::QueuedConnection);
            //            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_NewClientConnected, this, &ZP_netEngine::evt_NewClientConnected,Qt::QueuedConnection);
            //            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_ClientEncrypted,this, &ZP_netEngine::evt_ClientEncrypted,Qt::QueuedConnection);
            //            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_SocketError,this, &ZP_netEngine::evt_SocketError,Qt::QueuedConnection);
            //            connect (pTransThreadObj, &ZP_netTransThreadObj::evt_Message, this, &ZP_netEngine::evt_Message,Qt::QueuedConnection);
            connect (this, &ZP_netEngine::evt_NewConnection, pTransThreadObj, &ZP_netTransThreadObj::on_NewConnection, Qt::QueuedConnection);
            //            connect (this, &ZP_netEngine::evt_FireConnection,clientTH,&zp_netTransThread::startConnection,Qt::QueuedConnection);
            //            connect (this, &ZP_netEngine::evt_SendDataToClient,clientTH,&zp_netTransThread::SendDataToClient,Qt::QueuedConnection);
            //            connect (this, &ZP_netEngine::evt_KickAll,clientTH,&zp_netTransThread::KickAllClients,Qt::QueuedConnection);
            //            connect (this, &ZP_netEngine::evt_DeactivteImmediately,clientTH,&zp_netTransThread::DeactivateImmediately,Qt::QueuedConnection);
            //            connect (this, &ZP_netEngine::evt_KickClient,clientTH,&zp_netTransThread::KickClient,Qt::QueuedConnection);

            pTransThreadObj->moveToThread(pTransThread);

            //m_mutex_trans.lock();
            m_vecNetTransThreads.push_back(pTransThread);
            m_vecNetTransThreadObjs.push_back(pTransThreadObj);
            //m_mutex_trans.unlock();
        }
    }
}

bool ZP_netEngine::RemoveTransThreadObj(ZP_netTransThreadObj *pThreadObj)
{
    if (pThreadObj->canExit() == false)
        return false;
    int nsz = m_vecNetTransThreadObjs.size();
    int idx = -1;

    for (int i = 0; i < nsz && (idx < 0); i++)
    {
        if (m_vecNetTransThreadObjs[i] == pThreadObj)
            idx = i;
    }
    if (idx >= 0 && idx < nsz)
    {
        ZP_netTransThreadObj *pTransThreadObj =  m_vecNetTransThreadObjs[idx];
//        disconnect (clientTH,&zp_netTransThread::evt_ClientDisconnected,this,&zp_net_Engine::evt_ClientDisconnected);
//        disconnect (clientTH,&zp_netTransThread::evt_Data_recieved,this,&zp_net_Engine::evt_Data_recieved);
//        disconnect (clientTH,&zp_netTransThread::evt_Data_transferred,this,&zp_net_Engine::evt_Data_transferred);
//        disconnect (clientTH,&zp_netTransThread::evt_NewClientConnected,this,&zp_net_Engine::evt_NewClientConnected);
//        disconnect (clientTH,&zp_netTransThread::evt_ClientEncrypted,this,&zp_net_Engine::evt_ClientEncrypted);
//        disconnect (clientTH,&zp_netTransThread::evt_SocketError,this,&zp_net_Engine::evt_SocketError);
//        disconnect (clientTH,&zp_netTransThread::evt_Message,this,&zp_net_Engine::evt_Message);
//        disconnect (this,&zp_net_Engine::evt_EstablishConnection,clientTH,&zp_netTransThread::incomingConnection);
//        disconnect (this,&zp_net_Engine::evt_FireConnection,clientTH,&zp_netTransThread::startConnection);
//        disconnect (this,&zp_net_Engine::evt_SendDataToClient,clientTH,&zp_netTransThread::SendDataToClient);
//        disconnect (this,&zp_net_Engine::evt_KickAll,clientTH,&zp_netTransThread::KickAllClients);
//        disconnect (this,&zp_net_Engine::evt_DeactivteImmediately,clientTH,&zp_netTransThread::DeactivateImmediately);
//        disconnect (this,&zp_net_Engine::evt_KickClient,clientTH,&zp_netTransThread::KickClient);

        m_vecNetTransThreads[idx]->quit();
        m_vecNetTransThreads[idx]->wait();
        m_vecNetTransThreads[idx]->deleteLater();

        //m_vecNetTransThreadObjs[idx]->Empty_RabishCan();
        m_vecNetTransThreadObjs[idx]->deleteLater();

        m_vecNetTransThreads.remove(idx);
        m_vecNetTransThreadObjs.remove(idx);
    }
    return true;
}

void ZP_netEngine::on_NewClientArrived(qintptr socketDescriptor)
{
    ZP_netListenThreadObj * pSource = qobject_cast<ZP_netListenThreadObj *>(sender());
    if (!pSource)
    {
        emit evt_message(this, "Warning>" + QString(tr("Non-zp_netListenThread type detected.")));
        return;
    }

    cout << "Incomming client arriverd.";

    int nsz = m_vecNetTransThreads.size();
    int nMinPay = 0x7fffffff;
    int nMinIdx = -1;

    //找出一个最空闲的线程
    for (int i=0; i < nsz && nMinPay != 0; i++)
    {
        if (m_vecNetTransThreadObjs[i]->isActive() == false)
            continue;
        int nPat = m_vecNetTransThreadObjs[i]->currentClientCount();

        if (nPat < nMinPay)
        {
            nMinPay = nPat;
            nMinIdx = i;
        }
    }

    //remove old threads
    for (int i = 0; i < nsz; i++)
        if (m_vecNetTransThreadObjs[i]->isActive() == false )
            RemoveTransThreadObj(m_vecNetTransThreadObjs[i]);

    if (nMinIdx >= 0 && nMinIdx < nsz)
        emit evt_NewConnection(m_vecNetTransThreadObjs[nMinIdx], socketDescriptor);
    else
    {
        emit evt_message(this, "Warning>" + QString(tr("Need Trans Thread Object for clients.")));
        qCritical() << tr("Need Trans Thread Object for clients.");
    }
}

void ZP_netEngine::on_ListenClosed(QString id)
{
    //m_mutex_listen.lock();
    if (m_mapListenThreads.find(id) != m_mapListenThreads.end())
    {
        //Clean objects;
        QThread *pThread = m_mapListenThreads[id];
        ZP_netListenThreadObj *pListenObj = m_mapListenThreadObjs[id];

        m_mapListenThreads.remove(id);
        m_mapListenThreadObjs.remove(id);

        pThread->quit();
        pThread->wait();
        pThread->deleteLater();
        pListenObj->deleteLater();
    }
    //m_mutex_listen.unlock();
    return;
}

void ZP_netEngine::on__disconnetFromClient(qintptr socketDescriptor)
{

}
