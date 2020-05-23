#ifndef PENDULUMPLOT_H
#define PENDULUMPLOT_H
#include <QWidget>
#include <QVBoxLayout>
#include "qcustomplot.h"
#include "pendulum.h"
#include <QProgressBar>

class PendulumPlot : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* layout;
    QHBoxLayout* statsLayout;

    QCustomPlot* plot1; // xy's
    QCustomPlot* plot2;
    QCustomPlot* plot3; // angle 1
    QCustomPlot* plot4; // angle 2

    QLabel* periodLabel;
    QLabel* timeLabel;

    DoublePendulum* pendulum;
    QPen* xPen;
    QPen* yPen;
    QPen* aPen;
    QPen* axisPen;

    float space = 1.3f; //space above and bellow graph (graph not layout)
    int counter = 0;
    int range = 7;
public:
    PendulumPlot(DoublePendulum* pendulum, QWidget* parent = 0)
        : QWidget(parent), pendulum(pendulum)
    {
        QSizePolicy policy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        policy.setHeightForWidth(true);
        setSizePolicy(policy);

        axisPen = new QPen(Qt::black);
        xPen = new QPen(QColor("#12a4ed"));
        yPen = new QPen(QColor("#43b581"));
        aPen = new QPen(QColor("#7854f6"));

        // Shoulda put these into array
        plot1 = new QCustomPlot;
        plot2 = new QCustomPlot;
        plot3 = new QCustomPlot;
        plot4 = new QCustomPlot;

        plot1->yAxis->setLabel("pendulum 1");
        plot2->yAxis->setLabel("pendulum 2");
        plot3->yAxis->setLabel("α1");
        plot4->yAxis->setLabel("α2");

        plot1->xAxis->setLabel("t");
        plot2->xAxis->setLabel("t");
        plot3->xAxis->setLabel("t");
        plot4->xAxis->setLabel("t");

        plot1->xAxis2->setVisible(true);
        plot2->xAxis2->setVisible(true);
        plot3->xAxis2->setVisible(true);
        plot4->xAxis2->setVisible(true);

        plot1->xAxis2->setTickLabels(false);
        plot2->xAxis2->setTickLabels(false);
        plot3->xAxis2->setTickLabels(false);
        plot4->xAxis2->setTickLabels(false);

        plot1->yAxis2->setVisible(true);
        plot2->yAxis2->setVisible(true);
        plot3->yAxis2->setVisible(true);
        plot4->yAxis2->setVisible(true);

        plot1->yAxis2->setTickLabels(false);
        plot2->yAxis2->setTickLabels(false);
        plot3->yAxis2->setTickLabels(false);
        plot4->yAxis2->setTickLabels(false);


        plot1->addGraph(); //x1
        plot2->addGraph(); //y1
        plot1->addGraph(); //x2
        plot2->addGraph(); //y2

        plot3->addGraph(); //a1
        plot4->addGraph(); //a2

        plot1->graph(0)->setName("x");
        plot2->graph(0)->setName("x");

        plot1->graph(0)->setAdaptiveSampling(true);
        plot2->graph(0)->setAdaptiveSampling(true);
        plot3->graph(0)->setAdaptiveSampling(true);
        plot4->graph(0)->setAdaptiveSampling(true);

        plot1->graph(1)->setName("y");
        plot2->graph(1)->setName("y");
        plot1->graph(1)->setAdaptiveSampling(true);
        plot2->graph(1)->setAdaptiveSampling(true);

        plot1->legend->setVisible(true);
        plot2->legend->setVisible(true);

        plot1->legend->setBorderPen(Qt::NoPen);
        plot2->legend->setBorderPen(Qt::NoPen);
        plot3->legend->setBorderPen(Qt::NoPen);
        plot4->legend->setBorderPen(Qt::NoPen);

        plot1->legend->setBrush(Qt::NoBrush);
        plot2->legend->setBrush(Qt::NoBrush);
        plot3->legend->setBrush(Qt::NoBrush);
        plot4->legend->setBrush(Qt::NoBrush);

        plot1->legend->setIconSize(10, 10);
        plot2->legend->setIconSize(10, 10);

        plot1->setInteraction(QCP::iRangeDrag, true);
        plot2->setInteraction(QCP::iRangeDrag, true);
        plot3->setInteraction(QCP::iRangeDrag, true);
        plot4->setInteraction(QCP::iRangeDrag, true);

        plot1->setInteraction(QCP::iRangeZoom, true);
        plot2->setInteraction(QCP::iRangeZoom, true);
        plot3->setInteraction(QCP::iRangeZoom, true);
        plot4->setInteraction(QCP::iRangeZoom, true);

        connect(plot1, SIGNAL(mousePress(QMouseEvent*)), this, SIGNAL(plotClicked()));
        connect(plot2, SIGNAL(mousePress(QMouseEvent*)), this, SIGNAL(plotClicked()));
        connect(plot3, SIGNAL(mousePress(QMouseEvent*)), this, SIGNAL(plotClicked()));
        connect(plot4, SIGNAL(mousePress(QMouseEvent*)), this, SIGNAL(plotClicked()));


        periodLabel = new QLabel;
        timeLabel = new QLabel;
        periodLabel->setMaximumSize(100, 50);
        timeLabel->setMaximumSize(100, 50);

        statsLayout = new QHBoxLayout;
        statsLayout->setSpacing(10);
        statsLayout->addWidget(periodLabel, 0, Qt::AlignCenter);
        statsLayout->addWidget(timeLabel, 0, Qt::AlignCenter);
        statsLayout->setMargin(30);


        layout = new QVBoxLayout(this);
        layout->addWidget(plot1, 1);
        layout->addWidget(plot2, 1);
        layout->addWidget(plot3, 1);
        layout->addWidget(plot4, 1);
        layout->addLayout(statsLayout, 0);

        layout->setSpacing(0);
        layout->setMargin(0);
    }

protected:
    QSize sizeHint() const override{
        return QSize(350, 600);
    }

    virtual int heightForWidth(int w) const override{
        return w * 60 / 35;
    }
    void resizeEvent(QResizeEvent *event) override{
        QFont font = QFont("SF Pro Diplay", height() * width() * 0.000042f, QFont::Black);
        double penWidth = height() * width() * 0.00001f;
        xPen->setWidthF(penWidth);
        yPen->setWidthF(penWidth);
        aPen->setWidthF(penWidth);
        axisPen->setWidthF(penWidth);

        plot1->legend->setFont(font);
        plot1->yAxis->setLabelFont(font);
        plot1->xAxis->setLabelFont(font);
        plot1->graph(0)->setPen(*xPen);
        plot1->graph(1)->setPen(*yPen);
        plot1->xAxis->setTickLabelFont(font);
        plot1->yAxis->setTickLabelFont(font);
        plot1->setFont(font);

        plot2->legend->setFont(font);
        plot2->xAxis->setLabelFont(font);
        plot2->yAxis->setLabelFont(font);
        plot2->graph(0)->setPen(*xPen);
        plot2->graph(1)->setPen(*yPen);
        plot2->xAxis->setTickLabelFont(font);
        plot2->yAxis->setTickLabelFont(font);
        plot2->setFont(font);

        plot3->legend->setFont(font);
        plot3->xAxis->setLabelFont(font);
        plot3->yAxis->setLabelFont(font);
        plot3->graph(0)->setPen(*aPen);
        plot3->xAxis->setTickLabelFont(font);
        plot3->yAxis->setTickLabelFont(font);
        plot3->setFont(font);

        plot4->legend->setFont(font);
        plot4->xAxis->setLabelFont(font);
        plot4->yAxis->setLabelFont(font);
        plot4->graph(0)->setPen(*aPen);
        plot4->xAxis->setTickLabelFont(font);
        plot4->yAxis->setTickLabelFont(font);
        plot4->setFont(font);

        font.setPixelSize(height() * width() * 0.00008f);

        periodLabel->setFont(font);
        timeLabel->setFont(font);

        QWidget::resizeEvent(event);
    }
public slots:
    void step(){
        timeLabel->setText(QString("t: ") + QString::number(pendulum->dataset[counter].t, 'f', 2));

        plot1->xAxis->setRange(pendulum->dataset[counter].t, range, Qt::AlignRight);
        plot2->xAxis->setRange(pendulum->dataset[counter].t, range, Qt::AlignRight);

        plot3->xAxis->setRange(pendulum->dataset[counter].t, range, Qt::AlignRight);
        plot4->xAxis->setRange(pendulum->dataset[counter].t, range, Qt::AlignRight);


        if(pendulum->dataset[counter].angle2 > plot4->yAxis->range().upper){
            plot4->yAxis->setRange(plot4->yAxis->range().lower, pendulum->dataset[counter].angle2);
        }
        if(pendulum->dataset[counter].angle2 < plot4->yAxis->range().lower){
            plot4->yAxis->setRange(pendulum->dataset[counter].angle2, plot4->yAxis->range().upper);
        }

        if(pendulum->dataset[counter].angle1 > plot3->yAxis->range().upper){
            plot3->yAxis->setRange(plot3->yAxis->range().lower, pendulum->dataset[counter].angle1);
        }
        if(pendulum->dataset[counter].angle1 < plot3->yAxis->range().lower){
            plot3->yAxis->setRange(pendulum->dataset[counter].angle1, plot3->yAxis->range().upper);
        }

        replotAll();

        if(counter < pendulum->dataset.size()-1)
            counter++;
        else
            reset();

    }
    void setData(){
        timeLabel->setText(QString("t: 0.00"));

        plot3->yAxis->setRange(-pendulum->dataset[0].angle1, pendulum->dataset[0].angle1);
        plot4->yAxis->setRange(-pendulum->dataset[0].angle2, pendulum->dataset[0].angle2);

        for(int i = 0; i < pendulum->dataset.size(); i++){
            plot1->graph(0)->addData(pendulum->dataset[i].t, pendulum->dataset[i].x1);
            plot1->graph(1)->addData(pendulum->dataset[i].t, pendulum->dataset[i].y1);
            plot3->graph(0)->addData(pendulum->dataset[i].t, pendulum->dataset[i].angle1);
            periodLabel->setText(QString("T: ") + QString::number(pendulum->period, 'f', 2));
            if(pendulum->mass2 != 0){
                periodLabel->setText(QString("T: ?"));
                plot2->graph(0)->addData(pendulum->dataset[i].t, pendulum->dataset[i].x2);
                plot2->graph(1)->addData(pendulum->dataset[i].t, pendulum->dataset[i].y2);
                plot4->graph(0)->addData(pendulum->dataset[i].t, pendulum->dataset[i].angle2);
            }
        }

    }
    void reset(){
        counter = 0;
        clearAll();

        plot1->yAxis->setRange(-pendulum->len1 * space, pendulum->len1  * space);
        plot3->yAxis->setRange(-4, 4);
        if(pendulum->mass2 == 0){
            plot2->hide();
            plot4->hide();
        }
        else{
            plot2->show();
            plot4->show();
            plot2->yAxis->setRange((-pendulum->len1 - pendulum->len2)  * space, (pendulum->len1 + pendulum->len2) * space);
            plot4->yAxis->setRange(-4, 4);
        }


        setData();
    }

    void replotAll(){
        replotFirst();
        replotSecond();
    }

    void replotFirst(){
        plot1->replot();
        QApplication::processEvents();
        plot3->replot();
    }

    void replotSecond(){
        plot2->replot();
        QApplication::processEvents();
        plot4->replot();
    }

    void clearAll(){
        plot1->graph(0)->data().data()->clear();
        plot1->graph(1)->data().data()->clear();
        plot2->graph(0)->data().data()->clear();
        plot2->graph(1)->data().data()->clear();
        plot3->graph(0)->data().data()->clear();
        plot4->graph(0)->data().data()->clear();
    }
signals:
    void plotClicked();
};

#endif // DOUBLEPENDULUMPLOT_H
