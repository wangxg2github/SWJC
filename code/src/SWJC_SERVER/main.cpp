#include <QApplication>
#include "mainWidget.h"
#include "common/st_logger.h"
#include "common/comm.h"

st_logger g_logger;
void stMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    g_logger.MessageOutput(type, context, msg);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //注册日志接口函数
    qInstallMessageHandler(stMessageOutput);

    mainWidget w;
    w.show();

    return a.exec();
}
