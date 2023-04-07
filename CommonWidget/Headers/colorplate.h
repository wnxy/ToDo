#ifndef COLORPLATE_H
#define COLORPLATE_H

#include <QWidget>
#include <QLabel>
#include <QColorDialog>

#include "common.h"

class ColorPlate : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPlate(QWidget *parent = nullptr);
    ~ColorPlate();

    void mousePressEvent(QMouseEvent *event);

    void setCurrentColor(QColor &);

signals:
    void colorPlateChanged();

public slots:
    QColor onColorSelected();

private:
    QLabel *colorSelected;
};

#endif // COLORPLATE_H
