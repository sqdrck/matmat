#ifndef SLIDERSPANEL_H
#define SLIDERSPANEL_H

#include <QWidget>
#include <QPainter>

class SlidersPanel : public QWidget
{
    QPixmap* background;
public:
    SlidersPanel(QWidget* parent = 0) : QWidget(parent)
    {
        background = new QPixmap;
        background->load(":/resources/slidersBackground.png");
    }
protected:
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(0, 0, width(), height(), *background);
        QWidget::paintEvent(event);
    }
};

#endif // SLIDERSPANEL_H
