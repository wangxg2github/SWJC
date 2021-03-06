#ifndef COMM_H
#define COMM_H
#include <QDebug>
#include <QCryptographicHash>

#define cout qDebug() << __FILE__ << ":" << __LINE__ << "--->"

#define printLog(logLevel, fmt, args...) \
{\
    MyHelper::applicationLog(__FILE__, __LINE__, logLevel, fmt, ##args); \
}

enum
{
    LOG_ERROR = 0x11,
    LOG_DEBUG = 0x22,
    LOG_WARN  = 0x33,
    LOG_INFO  = 0x44
};



class MyHelper : public QObject
{
public:

    /**
     * @brief getMd5
     * @param data
     * @return
     */
    static QString getMd5(const QString data)
    {
        QString pwd = data + "1qazCDE#";
        QString md5;
        QByteArray bb;
        bb = QCryptographicHash::hash ( pwd.toLatin1(), QCryptographicHash::Md5 );
        return md5.append(bb.toHex());
    }

    static bool checkIP(QString ip)
    {
        QRegExp rx2("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
        if( !rx2.exactMatch(ip) )
        {
            return false;
        }
        return true;
    }


    /**
     * @brief applicationLog
     * @param file
     * @param line
     * @param logLevel
     * @param fmt
     */
    static void applicationLog(const char *file, const unsigned int line, unsigned int logLevel, const char *fmt, ...)
    {
        va_list args;
        char printBuf[512] = {0};
        va_start(args, fmt);
        int bufLen = vsprintf(printBuf, fmt, args);
        va_end(args);

        if(bufLen <= 0 || bufLen >= 512)
        {
            return;
        }

        char printLogBuf[1024] = {0};
        sprintf(printLogBuf, "[%s:%u] %s", file, line, printBuf);

        switch (logLevel)
        {
        case LOG_ERROR:
            qCritical(printLogBuf);
            break;
        case LOG_WARN:
            qWarning(printLogBuf);
            break;
        case LOG_INFO:
            qInfo(printLogBuf);
            break;
        case LOG_DEBUG:
            qDebug(printLogBuf);
            break;
        default:
            break;
        }
        return;
    }
};
#endif // COMM_H
