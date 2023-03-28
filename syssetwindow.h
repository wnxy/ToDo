#ifndef SYSSETWINDOW_H
#define SYSSETWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QDebug>

namespace Ui {
class SysSetWindow;
}

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const;
    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget = nullptr) const;
};

class SysSetWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SysSetWindow(QWidget *parent = nullptr);
    ~SysSetWindow();

private:
    Ui::SysSetWindow *ui;
};

#endif // SYSSETWINDOW_H
