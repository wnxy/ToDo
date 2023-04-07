#include "../Headers/colorplate.h"

ColorPlate::ColorPlate(QWidget *parent) : QWidget(parent)
{
    colorSelected = new QLabel(this);
    colorSelected->setMaximumSize(100, 25);
    colorSelected->setMinimumSize(100, 25);
//    connect(colorSelected, &QPushButton::clicked, this, &ColorPlate::onColorSelected);
}

ColorPlate::~ColorPlate()
{

}

void ColorPlate::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit colorPlateChanged();
    }
}

void ColorPlate::setCurrentColor(QColor &c)
{
    int r = c.red();
    int g = c.green();
    int b = c.blue();
    this->colorSelected->setStyleSheet(QString("background-color:rgb(%1, %2, %3);").arg(
                QString::number(r),
                QString::number(g),
                QString::number(b)));
}

QColor ColorPlate::onColorSelected()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    this->colorSelected->setStyleSheet(QString("background-color:rgb(%1, %2, %3);").arg(
                QString::number(color.red()),
                QString::number(color.green()),
                QString::number(color.blue())));
    return color;
}
