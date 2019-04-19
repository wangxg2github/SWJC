#include "mainWidget.h"
#include "ui_mainWidget.h"

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    m_mainWidgetUI(new Ui::mainWidget)
{
    m_mainWidgetUI->setupUi(this);

    mainWidgetInit();
    settingWidgetInit();
    systemTrayInit();

    m_pNetEngine = new ZP_netEngine(this);
    connect(m_pNetEngine, &ZP_netEngine::evt_message, this, &mainWidget::on_message);

    return;
}

mainWidget::~mainWidget()
{
    delete m_mainWidgetUI;
}


void mainWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::WindowStateChange:
        if (this->isMinimized() == true)
        {
            this->hide();
            //m_systemTrayIcon->show();
        }
        break;
    default:
        break;
    }
    return;
}

void mainWidget::closeEvent(QCloseEvent *event)
{
    if (this->isVisible() == true)
    {
        this->hide();
        m_systemTrayIcon->show();
        this->m_systemTrayIcon->showMessage(tr("系统正在运行"),
                                      tr("如果你想关闭系统，请输入密码."));
        event->ignore();
    }
    else
    {
//        QMessageBox::StandardButton exitBtn = QMessageBox::question(this, "提示",
//                                                                    "是否退出",
//                                                                    QMessageBox::Yes | QMessageBox::No);
//        if (exitBtn == QMessageBox::Yes)
//        {
//            threadStop();
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
        event->accept();
        //threadStop();
//        this->m_systemTrayIcon->showMessage(tr("Server is  going to closed"),
//                                      tr("Waiting for all unfinished progress..."));
    }

}

bool mainWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == (QObject *)m_mainWidgetUI->widgetTitle )
    {
        //双击最大化/恢复窗口
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if (this->property("canMoveAndMax").toBool())
            {
                this->showMaximized();
                this->setProperty("canMoveAndMax", false);

            }
            else
            {
                this->showNormal();
                this->setProperty("canMoveAndMax", true);
            }
            return true;
        }
    }

    QWidget *w = (QWidget *)this->window();
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *mouseevent = static_cast<QMouseEvent *>(event);
    if (mouseevent->type() == QEvent::MouseButtonPress)
    {
        if (mouseevent->button() == Qt::LeftButton)
        {
            mousePressed = true;
            mousePoint = mouseevent->globalPos() - w->pos();
            return true;
        }
    }
    else if (mouseevent->type() == QEvent::MouseButtonRelease)
    {
        mousePressed = false;
        return true;
    }
    else if (mouseevent->type() == QEvent::MouseMove)
    {
        if (mousePressed && (mouseevent->buttons() && Qt::LeftButton))
        {
            w->move(mouseevent->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}


void mainWidget::on_getWidgetTraySignal(QSystemTrayIcon::ActivationReason reason)
{
#if 0
    if (QSystemTrayIcon::Trigger == reason)
    {
        m_systemTrayIcon->showMessage("Message Title",
                                  "欢迎使用此程序",
                                  QSystemTrayIcon::Information,
                                  1000);
    }
#endif
    if (QSystemTrayIcon::DoubleClick == reason)
    {
        m_systemTrayIcon->hide();
        this->showNormal();
    }
    return;
}


void mainWidget::mainWidgetInit()
{
    this->resize(700, 500);
    this->setProperty("canMoveAndMax", true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    m_mainWidgetUI->lab_Name->setText("水文监测系统");
    m_mainWidgetUI->lab_Name->setFont(QFont("Microsoft Yahei", 20));
    this->setWindowTitle(m_mainWidgetUI->lab_Name->text());

    //m_mainWidgetUI->stackedWidget->setStyleSheet("QLabel{font:60pt;}");

    //安装事件过滤器
    m_mainWidgetUI->widgetTitle->installEventFilter(this);

    //设置顶部导航按钮
    QSize icoSize(25, 25);
    int icoWidth = 65;

    QList<QToolButton *> tbtns = m_mainWidgetUI->widgetTitleMenu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, &QToolButton::clicked, this, &mainWidget::on_toolBtnAll_clicked);
    }
    connect(m_mainWidgetUI->pushBtn_Min, &QPushButton::clicked, this,
            [=]()
    {
        showMinimized();
    });
    connect(m_mainWidgetUI->pushBtn_Close, &QPushButton::clicked, this,
            [=]()
    {
        close();
    });

}

void mainWidget::systemTrayInit()
{
    m_actionShowWidget = new QAction("显示界面", this);
    connect(m_actionShowWidget, &QAction::triggered,
            [this]()
    {
        this->showNormal();
    });

    m_actionExit = new QAction("退出程序", this);
    connect(m_actionExit, &QAction::triggered,
            [this]()
    {
        this->close();
    });

    m_menuSystemTray = new QMenu(this);
    m_menuSystemTray->addAction(m_actionShowWidget);
    m_menuSystemTray->addSeparator();
    m_menuSystemTray->addAction(m_actionExit);

    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_systemTrayIcon->setContextMenu(m_menuSystemTray);

    //新建托盘要显示的icon
    QIcon icon = QIcon(":/resource/image/Color_Classic.png");
    //m_systemTrayIcon
    m_systemTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    m_systemTrayIcon->setToolTip("水文监测系统");

    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &mainWidget::on_getWidgetTraySignal);

    m_mainWidgetUI->toolBtn_Main->clicked();
}

void mainWidget::settingWidgetInit()
{
    /**
     * 添加监听服务器
    */
    /*tabWidget*/
    m_mainWidgetUI->tabWidget_Listen->setColumnCount(3);
    QStringList tabHeader;
    tabHeader << "IP" << "Port" << "Listen Status";
    m_mainWidgetUI->tabWidget_Listen->setHorizontalHeaderLabels(tabHeader);
    //列头隐藏
    m_mainWidgetUI->tabWidget_Listen->verticalHeader()->setVisible(false);
    //表头自动填充
    m_mainWidgetUI->tabWidget_Listen->horizontalHeader()->setStretchLastSection(true);
    //禁止表格编辑
    m_mainWidgetUI->tabWidget_Listen->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //整行选中
    m_mainWidgetUI->tabWidget_Listen->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*LineEdit 校验IP*/
    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    m_mainWidgetUI->lineEdit_IP->setValidator(pReg);

    /*spinBox 校验Port*/
    m_mainWidgetUI->spinBox_Port->setRange(5000, 0x1FFFFFFF);
    m_mainWidgetUI->spinBox_Port->setValue(10802);  
    
    connect(m_mainWidgetUI->pushBtn_AddListen, &QPushButton::clicked, this,
            [=]()
    {
        QString strIp = m_mainWidgetUI->lineEdit_IP->text();
        int port = m_mainWidgetUI->spinBox_Port->value();

        //检查IP的合法性
        if(false == MyHelper::checkIP(strIp))
        {
            QMessageBox::information(NULL, "提醒", "IP不合法，请重新输入...", QMessageBox::Yes);
            m_mainWidgetUI->lineEdit_IP->clear();
            return;
        }

        //检查IP是否重复
        for(int i = 0; i < m_mainWidgetUI->tabWidget_Listen->rowCount(); i++)
        {

            if(m_mainWidgetUI->tabWidget_Listen->item(i, 0) != NULL //一定要先判断非空，否则会报错
                    && m_mainWidgetUI->tabWidget_Listen->item(i, 0)->text() == strIp
                    && m_mainWidgetUI->tabWidget_Listen->item(i, 1)->text().toInt() == port)
            {
                QMessageBox::information(NULL, "提醒", "已存在相同的IP和Port数据，请重新输入...");
                m_mainWidgetUI->lineEdit_IP->clear();
                return;
            }
        }
        m_mainWidgetUI->lineEdit_IP->clear();

        //动态添加一行
        int rowCount = m_mainWidgetUI->tabWidget_Listen->rowCount();
        m_mainWidgetUI->tabWidget_Listen->insertRow(rowCount);
        m_mainWidgetUI->tabWidget_Listen->setItem(rowCount, 0, new QTableWidgetItem(strIp));
        m_mainWidgetUI->tabWidget_Listen->setItem(rowCount, 1, new QTableWidgetItem(QString::number(port)));
        m_mainWidgetUI->tabWidget_Listen->setItem(rowCount, 2, new QTableWidgetItem("No"));
    });
    connect(m_mainWidgetUI->pushBtn_DelListen, &QPushButton::clicked, this,
            [=]()
    {

        int rowNum = m_mainWidgetUI->tabWidget_Listen->currentRow();
        if (rowNum < 0)
            QMessageBox::information(NULL, "提醒", "请选择需要删除的数据！", QMessageBox::Yes);
        else
            m_mainWidgetUI->tabWidget_Listen->removeRow(rowNum);
    });

    /*textEdit 设置日志条数默认500条*/
    m_mainWidgetUI->textEdit_Log->document()->setMaximumBlockCount(500);
    m_mainWidgetUI->textEdit_Log->setReadOnly(true);
    return;
}


void mainWidget::on_toolBtnAll_clicked()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    QList<QToolButton *> tbtns = m_mainWidgetUI->widgetTitleMenu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        if (btn == b)
        {
            btn->setChecked(true);
        }
        else
        {
            btn->setChecked(false);
        }
    }

    if (name == "主界面")
    {
        m_mainWidgetUI->stackedWidget->setCurrentIndex(0);
        //m_mainWidgetUI->stackedWidget->show();
    }
    else if (name == "系统设置")
    {
        m_mainWidgetUI->stackedWidget->setCurrentIndex(1);
    }
    else if (name == "数据查询")
    {
        //m_mainWidgetUI->stackedWidget->setCurrentIndex(2);


        m_pNetEngine->AddListeningAddress("1234", QHostAddress::LocalHost, 10802);
        m_pNetEngine->AddClientTransThreads(2);
        cout << "Start Listen, TransThread Num : 2.";

    }
    else if (name == "用户退出")
    {
        close();
    }

}

void mainWidget::on_message(QObject *obj, QString msg)
{
    QString strDataTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString strMsg = strDataTime + QString("[%1] --->").arg(quint64(obj)) + msg;

    m_mainWidgetUI->textEdit_Log->append(strMsg);
    return;
}
