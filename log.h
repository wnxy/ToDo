#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QDateTime>

#include "log4qt/logger.h"
#include "log4qt/basicconfigurator.h"
#include "log4qt/propertyconfigurator.h"

#define LOG_DEBUG(msg) Log::debug(msg, __FILE__, __FUNCTION__, __LINE__);
#define LOG_INFO(msg) Log::info(msg, __FILE__, __FUNCTION__, __LINE__);
#define LOG_WARN(msg) Log::warn(msg, __FILE__, __FUNCTION__, __LINE__);
#define LOG_ERROR(msg) Log::error(msg, __FILE__, __FUNCTION__, __LINE__);

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

    QString getLogTime() const;

public slots:
    static Log *getLogger();

public slots:
    void init(const QString configFilePath);

public slots:
    void debug(QString msg, const QString file, const char* func, const int line);
    void info(QString msg, const QString file, const char* func, const int line);
    void warn(QString msg, const QString file, const char* func, const int line);
    void error(QString msg, const QString file, const char* func, const int line);

private:
    static Log *_pInstance;
    static QMutex _qMutex;
    static Log4Qt::Logger *_pLoggerDebug;
    static Log4Qt::Logger *_pLoggerInfo;
    static Log4Qt::Logger *_pLoggerWarn;
    static Log4Qt::Logger *_pLoggerError;
    static QString _configFilePath;
};

#endif // LOG_H
