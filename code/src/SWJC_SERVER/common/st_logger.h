#ifndef ST_LOGGER_H
#define ST_LOGGER_H

#include <QObject>
#include <QDateTime>
#include <QDir>
#include <QByteArray>
#include <QMutex>
#include <QCoreApplication>

class st_logger : public QObject
{
    Q_OBJECT

    enum
    {
        LOGlEVEL_ERROR,
        LOGLEVEL_WARNING,
        LOGLEVEL_INFO,
        LOGLEVEL_DEBUG,
        LOGlEVEL_BUFF
    };

public:
    explicit st_logger(QObject *parent = 0);

    /**
     * @brief MessageOutput
     * @param type
     * @param context
     * @param msg
     */
    void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    /**
     * @brief ÿ����־�ļ�����С
     * @return
     */
    int maxFileSize() const;

    /**
     * @brief ��ǰ��־�������
     * @return ��ǰ��־����
     */
    int logLevel() const;

public slots:
    /**
     * @brief ������־�ļ��Ĵ�С
     * @param nSize���ļ���Сֵ
     */
    void setMaxFileSize(int nSize);

    /**
     * @brief ������־����
     * @param lv����־����
     */
    void setLogLevel(int lv);


private:
    /**
     * @brief ����һ���µ���־�ļ�
     * @param app
     * @return��true�������ɹ���false������ʧ��
     */
    bool CreateNewLogFile(QCoreApplication * app);

    QFile*      m_pLogFile;
    bool        m_bUseLogFile;
    QString     m_currLogFileName;
    int         m_nLogLevel;
    int         m_nMaxFileSize;
    QMutex      m_mutextLogger;
};

#endif // ST_LOGGER_H