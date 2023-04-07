#include "syssetwindow.h"
#include "ui_syssetwindow.h"

//QSize CustomTabStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
//                                       const QSize &size, const QWidget *widget) const
//{
//    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
//    if(type == QStyle::CT_TabBarTab)
//    {
//        s.transpose();
//        s.rwidth() = 90;
//        s.rheight() = 44;
//    }
//    return s;
//}

//void CustomTabStyle::drawControl(ControlElement element, const QStyleOption *option,
//                                 QPainter *painter, const QWidget *widget) const
//{
//    if(element == CE_TabBarTabLabel)
//    {
//        if(const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
//        {
//            QRect allRect = tab->rect;

//            if(tab->state & QStyle::State_Selected)
//            {
//                painter->save();
//                painter->setPen(0x89cfff);
//                painter->setBrush(QBrush(0x89cfff));
//                painter->drawRect(allRect.adjusted(6, 6, -6, -6));
//            }
//            QTextOption option;
//            option.setAlignment(Qt::AlignCenter);
//            if(tab->state & QStyle::State_Selected)
//            {
//                painter->setPen(0xf8fcff);
//            }
//            else
//            {
//                painter->setPen(0x5d5d5d);
//            }

//            painter->drawText(allRect, tab->text, option);
//            return;
//        }
//    }

//    if(element == CE_TabBarTab)
//    {
//        QProxyStyle::drawControl(element, option, painter, widget);
//    }
//}

SysSetWindow::SysSetWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysSetWindow)
{
    ui->setupUi(this);
//    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->ctlWidget->setVisible(false);
    initSetWindow();

    this->show();
}

SysSetWindow::~SysSetWindow()
{
    delete ui;
}

void SysSetWindow::initSetWindow()
{
    if(autoStart)
    {
        ui->isActcheckBox->setCheckState(Qt::Checked);
    }
    if(!isToDoMove)
    {
        ui->isMovedcheckBox->setCheckState(Qt::Checked);
    }
    this->setFont();
    this->setFontColor();
    this->setMainWinBGColor();
    this->setNoteEditBGColor();
    this->setMainWinBGOpacity();
    this->setNoteEditBGOpacity();
}

void SysSetWindow::setFont()
{
    ui->fontComboBox->setCurrentFont(QFont(fontFamily));
    ui->fontSizeComboBox->setCurrentText(QString::number(fontPointSize));
}

void SysSetWindow::setFontColor()
{
    ui->fontColor->setCurrentColor(noteFontColor);
}

void SysSetWindow::setMainWinBGColor()
{
    ui->mainWinColor->setCurrentColor(mainWinBGColor);
}

void SysSetWindow::setNoteEditBGColor()
{
    ui->noteEditColor->setCurrentColor(noteEditBGColor);
}

void SysSetWindow::setMainWinBGOpacity()
{
    int opacity = alphaToOpacity(mainWinBGAlpha);
    ui->mainWinOpacity->setValue(opacity);
    this->setMainWinBGOpacityView(opacity);
}

void SysSetWindow::setNoteEditBGOpacity()
{
    int opacity = alphaToOpacity(noteEditBGAlpha);
    ui->noteEditOpacity->setValue(opacity);
    this->setNoteEditBGOpacityView(opacity);
}

void SysSetWindow::setMainWinBGOpacityView(int opacity)
{
    QString s = QString::number(opacity) + "%";
    ui->mainWinOpacityLabel->setText(s);
}

void SysSetWindow::setNoteEditBGOpacityView(int opacity)
{
    QString s = QString::number(opacity) + "%";
    ui->noteEditOpacityLabel->setText(s);
}

void SysSetWindow::on_isMovedcheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)     // 选中
    {
        isToDoMove = false;
    }
    else if(arg1 == Qt::Unchecked)
    {
        isToDoMove = true;
    }
}

void SysSetWindow::on_isActcheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)     // 选中
    {
        autoStart = true;
    }
    else if(arg1 == Qt::Unchecked)
    {
        autoStart = false;
    }
    emit isActcheckBoxStateChanged();
}

void SysSetWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    fontFamily = f.family();
}

void SysSetWindow::on_fontSizeComboBox_currentTextChanged(const QString &arg1)
{
    fontPointSize = arg1.toInt();
}

void SysSetWindow::on_fontColor_colorPlateChanged()
{
    noteFontColor = ui->fontColor->onColorSelected();
#ifdef QT_DEBUG
    qDebug() << "Debug: note font color is " << noteFontColor;
#endif
}

void SysSetWindow::on_mainWinColor_colorPlateChanged()
{
    mainWinBGColor = ui->mainWinColor->onColorSelected();
}

void SysSetWindow::on_noteEditColor_colorPlateChanged()
{
    noteEditBGColor = ui->noteEditColor->onColorSelected();
}

void SysSetWindow::on_mainWinOpacity_sliderMoved(int position)
{
    int alpha = opacityToAlpha(position);
    mainWinBGAlpha = alpha;
    int opacity = alphaToOpacity(alpha);
    this->setMainWinBGOpacityView(opacity);
}

void SysSetWindow::on_noteEditOpacity_sliderMoved(int position)
{
    int alpha = opacityToAlpha(position);
    noteEditBGAlpha = alpha;
    int opacity = alphaToOpacity(alpha);
    this->setNoteEditBGOpacityView(opacity);
}
