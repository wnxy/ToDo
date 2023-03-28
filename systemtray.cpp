#include "systemtray.h"

SystemTray::SystemTray(QWidget *parent)
    : QSystemTrayIcon(parent)
    , pWidget(parent)
{
    initSysTray();            // 初始化系统托盘
    addSysTrayMenu();         // 添加托盘菜单

    this->show();
}

SystemTray::~SystemTray()
{

}

/**
 * @brief SystemTray::onIconActivated
 * 系统托盘响应
 * @param reason
 */
void SystemTray::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)           // 点击托盘，显示系统主界面
    {
        pWidget->activateWindow();
        pWidget->showNormal();
    }
    else if(reason == QSystemTrayIcon::Context)      // 右击托盘，显示托盘菜单
    {
        sysMenu->exec(QCursor::pos());
    }
}

/**
 * @brief SystemTray::onSysSetWindow
 * 系统设置界面响应函数
 */
void SystemTray::onSysSetWindow()
{
    sysSetWin = new SysSetWindow();

#ifdef QT_DEBUG
    qDebug() << "System setup.";
#endif
}

/**
 * @brief SystemTray::initSysTray
 * 初始化软件托盘
 */
void SystemTray::initSysTray()
{
    setToolTip(tr("我的小本子"));
    setIcon(QIcon(tr(":/Images/Resources/Images/sysIcon.ico")));

    connect(this, &SystemTray::activated, this, &SystemTray::onIconActivated);

#ifdef QT_DEBUG
    qDebug() << "Succeed init system tray.";
#endif
}

/**
 * @brief SystemTray::addSysTrayMenu
 * 设置软件托盘菜单栏
 */
void SystemTray::addSysTrayMenu()
{
    sysMenu = new QMenu(pWidget);
    setAction = new QAction(tr("设置"), sysMenu);
    infoAction = new QAction(tr("帮助与更新"), sysMenu);
    aboutAction = new QAction(tr("关于"), sysMenu);
    quitAction = new QAction(tr("退出"), sysMenu);

    sysMenu->addAction(setAction);
    sysMenu->addAction(infoAction);
    sysMenu->addAction(aboutAction);
    sysMenu->addAction(quitAction);

    // 系统设置
    connect(setAction, &QAction::triggered, this, &SystemTray::onSysSetWindow);
    // 退出
    connect(quitAction, &QAction::triggered, [this](bool){
        this->pWidget->close();
    });

#ifdef QT_DEBUG
    qDebug() << "Succeed add system tray menu.";
#endif
}
