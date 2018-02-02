#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QMessageBox>

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

protected:
    /**
     * @brief 获取系统窗口改变事件，最小化系统
     * @param event ：窗口状态改变
     */
    void changeEvent(QEvent *event);

    /**
     * @brief 重写窗口关闭事件，最小化到托盘
     * @param event ：关闭窗口
     */
    void closeEvent(QCloseEvent *event);

    /**
     * @brief 系统工具栏注册事件过滤器
     * @param obj：窗口工具栏
     * @param event ：双击最大化/恢复，窗口移动
     * @return 若事件被处理，返回true；若事件没有被处理，返回false
     */
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    /**
     * @brief 获取系统托盘信号
     * @param reason
     */
    void on_getWidgetTraySignal(QSystemTrayIcon::ActivationReason reason);

    /**
     * @brief 系统工具栏菜单信号处理
     */
    void on_toolBtnAll_clicked();

    //void on_pushBtn_userConfig_clicked();

private:
    /**
     * @brief 主界面初始化
     */
    void mainWidgetInit();
    /**
     * @brief 系统托盘初始化
     */
    void systemTrayInit();
private:
    Ui::mainWidget *m_mainWidgetUI;

    //托盘右键菜单
    QSystemTrayIcon *m_systemTrayIcon;
    QMenu           *m_menuSystemTray;
    QAction         *m_actionShowWidget;
    QAction         *m_actionExit;
};

#endif // MAINWIDGET_H
