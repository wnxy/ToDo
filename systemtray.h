#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QUrl>
#include <QWidget>
#include <QMenu>

#include "syssetwindow.h"
#include "inforstate.h"

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    SystemTray(QWidget *parent);
    ~SystemTray();

private slots:
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);          // 事件处理函数
    void onSysSetWindow();                                                   // 系统设置函数
    void onInforState();
    void setAppAutoStart();

private:
    void initSysTray();       // 初始化系统托盘
    void addSysTrayMenu();    // 添加托盘菜单

signals:
    void setAppAutoStartSign();

private:
    QWidget *pWidget;         // 父类，用于显示窗体
    QMenu *sysMenu;
    QAction *setAction;
    QAction *infoAction;
//    QAction *aboutAction;
    QAction *quitAction;

    SysSetWindow *sysSetWin;
    InforState *inforWin;
};

#endif // SYSTEMTRAY_H
