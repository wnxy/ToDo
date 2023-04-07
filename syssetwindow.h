#ifndef SYSSETWINDOW_H
#define SYSSETWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QDebug>

#include "common.h"

namespace Ui {
class SysSetWindow;
}

//class CustomTabStyle : public QProxyStyle
//{
//public:
//    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
//                           const QSize &size, const QWidget *widget) const;
//    void drawControl(ControlElement element, const QStyleOption *option,
//                     QPainter *painter, const QWidget *widget = nullptr) const;
//};

class SysSetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SysSetWindow(QWidget *parent = nullptr);
    ~SysSetWindow();

private:
    void initSetWindow();
    void setFont();
    void setFontColor();
    void setMainWinBGColor();
    void setNoteEditBGColor();
    void setMainWinBGOpacity();
    void setNoteEditBGOpacity();
    void setMainWinBGOpacityView(int);
    void setNoteEditBGOpacityView(int);

private slots:
    void on_isMovedcheckBox_stateChanged(int arg1);

    void on_isActcheckBox_stateChanged(int arg1);

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_fontSizeComboBox_currentTextChanged(const QString &arg1);

    void on_fontColor_colorPlateChanged();

    void on_mainWinColor_colorPlateChanged();

    void on_noteEditColor_colorPlateChanged();

    void on_mainWinOpacity_sliderMoved(int position);

    void on_noteEditOpacity_sliderMoved(int position);

signals:
    void isActcheckBoxStateChanged();

private:
    Ui::SysSetWindow *ui;
};

#endif // SYSSETWINDOW_H
