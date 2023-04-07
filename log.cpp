#include "log.h"

Log *Log::_pInstance = nullptr;
QMutex Log::_qMutex;
Log4Qt::Logger *Log::_pLoggerDebug = nullptr;
Log4Qt::Logger *Log::_pLoggerInfo = nullptr;
Log4Qt::Logger *Log::_pLoggerWarn = nullptr;
Log4Qt::Logger *Log::_pLoggerError = nullptr;
QString Log::_configFilePath;

Log::Log(QObject *parent) : QObject(parent)
{
    // 基础配置
    Log4Qt::BasicConfigurator::configure();
}

QString Log::getLogTime() const
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString curDate = dateTime.toString("yyyy-MM-dd");
    QString curTime = dateTime.toString("hh:mm:ss");
    QString curSec = dateTime.toString("zzz");
    return curDate + " " + curTime + " " + curSec;
}

Log *Log::getLogger()
{
    if(!_pInstance)
    {
        QMutexLocker mutexLocker(&_qMutex);
        if(!_pInstance)
        {
            Log *pInstance = new Log();
            _pInstance = pInstance;
        }
    }
    return _pInstance;
}

void Log::init(const QString configFilePath)
{
    if(QFile::exists(configFilePath))
    {
        _configFilePath = configFilePath;
        Log4Qt::PropertyConfigurator::configure(_configFilePath);
    }
    else
    {
//         获取根logger
//        Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
//        Log4Qt::TTCCLayout *layout = new Log4Qt::TTCCLayout();
//        layout->setName("My Layout");
//        layout->activateOptions();

//        // 创建一个 fileAppender（将日志内容输出到文件中）
//        // true 以 Append 方式打开文件，false 以 Truncate 方式打开文件。
//        QString file = QCoreApplication::applicationDirPath() + "/info.log";
//        Log4Qt::FileAppender *fileAppender = new Log4Qt::FileAppender(layout, file, true);
//        fileAppender->setName("My file appender");
//        fileAppender->activateOptions();
//        // 在 root logger 上添加 fileAppender
//        logger->addAppender(fileAppender);

//        // 设置级别为 info
//        logger->setLevel(Log4Qt::Level::INFO_INT);
//        // 允许处理 Qt 消息
//        Log4Qt::LogManager::setHandleQtMessages(true);
//        // 输出信息
//        logger->info("This is a info message.");
//        qInfo() << "This is a info message too.";

    }
    _pLoggerDebug = Log4Qt::Logger::logger("debug");
    _pLoggerInfo = Log4Qt::Logger::logger("info");
    _pLoggerWarn = Log4Qt::Logger::logger("warn");
    _pLoggerError = Log4Qt::Logger::logger("error");
}

void Log::debug(QString msg, const QString file, const char* func, const int line)
{
    QString header = getLogTime() + " [DEBUG] " + file + " Method: " + func + " Line: " + QString::number(line) + ": ";
    _pLoggerDebug->debug(header);
    _pLoggerDebug->debug(msg + '\n');
}

void Log::info(QString msg, const QString file, const char* func, const int line)
{
    QString header = getLogTime() + " [INFO] " + file + " Method: " + func + " Line: " + QString::number(line) + ": ";
    _pLoggerDebug->info(header);
    _pLoggerInfo->info(msg + '\n');
}

void Log::warn(QString msg, const QString file, const char* func, const int line)
{
    QString header = getLogTime() + " [WARN] " + file + " Method: " + func + " Line: " + QString::number(line) + ": ";
    _pLoggerDebug->warn(header);
    _pLoggerWarn->warn(msg + '\n');
}

void Log::error(QString msg, const QString file, const char* func, const int line)
{
    QString header = getLogTime() + " [ERROR] " + file + " Method: " + func + " Line: " + QString::number(line) + ": ";
    _pLoggerDebug->warn(header);
    _pLoggerError->error(msg + '\n');
}

