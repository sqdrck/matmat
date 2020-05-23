#ifndef PENDULUMDISPLAY_H
#define PENDULUMDISPLAY_H

#include <QWidget>
#include <pendulum.h>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>

class PendulumDisplay : public QWidget
{
    Q_OBJECT
public:
    PendulumDisplay(DoublePendulum* pendulum, QWidget* parent = 0) :
        QWidget(parent), pendulum(pendulum)
    {
        updateTimer = new QTimer;
        connect(updateTimer, SIGNAL(timeout()), this, SLOT(update())); 
        grid = new QPixmap;
        grid->load(":/resources/grid.png");
        shadow = new QPixmap;
        shadow->load(":/resources/fade.png");
        bobShadow = new QPixmap;
        bobShadow->load(":/resources/bobShadow.png");
    }
    int scale = 25;
    double scaleMult = 14;
    int traceSize = 300;
    int gridSize = 20;
private:
    DoublePendulum* pendulum;
    int counter = 0;

    QTimer* updateTimer;
    int fps = 60;

    QPixmap* grid;
    QPixmap* shadow;
    QPixmap* bobShadow;

    void fit(){
        scale = scaleMult * width() * height() * 0.00005f;
    }
    double remap(double s, double a1, double a2, double b1, double b2)
    {
        return b1 + (s-a1)*(b2-b1)/(a2-a1);
    }

    void paintPendulum(QPainter& painter){
        PendulumData data = pendulum->dataset[counter];
        int d1 = sqrt(pendulum->mass1 * scale);
        int d2 = sqrt(pendulum->mass2 * scale);

        QPen pen;
        QColor color;
        QPainterPath path;

        double mapped;
        int ceiling = counter > traceSize ? traceSize : counter;

        if(pendulum->mass2 == 0){
            for (int i = counter - ceiling + 1; i < counter; i++) {
                mapped = remap(i, counter, counter - ceiling, 0, 1);
                pen.setWidthF(scale * 0.015f * (1 - mapped * mapped));
                color.setHsl(mapped * 255, 190, 205);
                pen.setColor(color);
                pen.setJoinStyle(Qt::BevelJoin);
                painter.setPen(pen);
                painter.drawLine(pendulum->dataset[i].x1 * scale,
                                 pendulum->dataset[i].y1 * scale,
                                 pendulum->dataset[i-1].x1 * scale,
                        pendulum->dataset[i-1].y1 * scale);
            }
        }
        else{
            for (int i = counter - ceiling + 1; i < counter; i++) {
                mapped = remap(i, counter, counter - ceiling, 0, 1);
                pen.setWidthF(scale * 0.015f * (1 - mapped * mapped));
                color.setHsl(mapped * 255, 190, 205);
                pen.setColor(color);
                pen.setJoinStyle(Qt::BevelJoin);
                painter.setPen(pen);
                painter.drawLine(pendulum->dataset[i].x2 * scale,
                                 pendulum->dataset[i].y2 * scale,
                                 pendulum->dataset[i-1].x2 * scale,
                        pendulum->dataset[i-1].y2 * scale);
            }
        }

        painter.drawPixmap(data.x1 * scale - d1 * 2, data.y1 * scale - d1 * 2, d1 * 4, d1 * 4, *bobShadow);
        painter.drawPixmap(data.x2 * scale - d2 * 2, data.y2 * scale - d2 * 2, d2 * 4, d2 * 4, *bobShadow);

        painter.setPen(QPen(Qt::darkGray, scale * 0.02f, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(0, 0, data.x1 * scale, data.y1 * scale);
        if(d2 != 0)
            painter.drawLine(data.x1 * scale, data.y1 * scale, data.x2 * scale, data.y2 * scale);

//        painter.setBrush(QPen(Qt::darkGray, scale * 0.02f, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(data.x1 * scale - d1/2, data.y1 * scale - d1/2, d1, d1);
        painter.drawEllipse(data.x2 * scale - d2/2, data.y2 * scale - d2/2, d2, d2);
    }

protected:
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setRenderHint(QPainter::QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        painter.translate(width() / 2, height() / 2 - height()/8);
        float pixScale = scale * 0.00247f; // grid has marks with sizes.
        painter.drawPixmap((-2500 - 24) * pixScale,
                           (-2500 - 25) * pixScale,
                           5000 * pixScale,
                           5000 * pixScale,
                           *grid);



        if(pendulum->dataset.isEmpty())
            return;
        painter.drawPixmap(-width()/2, -height() / 2 + height()/8, width(), height(), *shadow);
        paintPendulum(painter);
    }

    virtual void wheelEvent(QWheelEvent *event) override
    {
        int numPixels = event->pixelDelta().y() / 20;
        int numDegrees = event->angleDelta().y() / 50;
        int scroll = numPixels + numDegrees;
        if((scaleMult < 4 && scroll < 0)|| (scaleMult > 85 && scroll > 0))
            return;
        if (numPixels != 0) {
            scaleMult += scroll;
            fit();
        } else if (numDegrees != 0) {
            scaleMult += scroll;
            fit();
        }
        update();
        event->accept();
    }

    QSize sizeHint() const override{
        return QSize(500, 500);
    }
    void resizeEvent(QResizeEvent* event) override{
        fit();
        QWidget::resizeEvent(event);
    }
public slots:
    void step(){
        if(counter < pendulum->dataset.size()-1)
            counter++;
        else
            reset();
    }
    void reset(){
        counter = 0;
    }
    void startUpdate(){
        updateTimer->start(1.0f/fps * 1000);
    }
    void stopUpdate(){
        updateTimer->stop();
    }
    void setFps(int n){
        fps = n;
    }
};


#endif // PENDULUMDISPLAY_H
