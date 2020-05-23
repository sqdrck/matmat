#include <QApplication>
#include <QtWidgets>
#include <QElapsedTimer>
#include "slider.h"
#include <pendulumdisplay.h>
#include <pendulum.h>
#include <pendulumplot.h>
#include "sliderspanel.h"
#include "button.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/resources/style1.qss");
    file.open(QFile::ReadOnly);
    QString strCSS = QLatin1String(file.readAll());
    app.setStyleSheet(strCSS);

    QWidget window;
    QHBoxLayout layout(&window);

    SlidersPanel sliders;

    QVBoxLayout slidersLayout(&sliders);

    Slider* m1S = new Slider("m₁(ᵏᵍ)", 10, QColor("#12a4ed"));
    Slider* m2S = new Slider("m₂(ᵏᵍ)", 10, QColor("#12a4ed"));
    Slider* l1S = new Slider("l₁(ᵐ)", 3, QColor("#ff9600"));
    Slider* l2S = new Slider("l₂(ᵐ)", 3, QColor("#ff9600"));
    Slider* angle1S = new Slider("α₁(°)", 3.6f, QColor("#7854f6"));
    Slider* angle2S = new Slider("α₂(°)", 3.6f, QColor("#7854f6"));
    Slider* dampingS = new Slider("b(ᵏᵍ/ˢ⁻¹)", 5, QColor("#fa6441"));
    Slider* gravityS = new Slider("g(ᵐ/ˢ²)", 20, QColor("#43b581"));
    Slider* timeS = new Slider("time(ˢ)", 100, QColor("#43b581"));

    QLabel* description = new QLabel("Set the second mass to zero to simulate a simple pendulum.\n\
                                     You can also move and zoom graphs - it will cause pause.\n\
                                     Scroll mouse wheel when hovering on pendulum to zoom.\n\
                                     Scroll mouse wheel when hovering on slider for more precise edit\n\n\
                                     Created by Pavlichenko Grigory & Pavlenko Daniil. KubSU.\n\
                                     MatMat v1.01. https://github.com/sqdrck/matmat");
    description->setAlignment(Qt::AlignRight);

    Button* startButton = new Button("Unpause ", QColor("#ffca00"));
    Button* calculateButton = new Button("🚀 Calculate  ", QColor("#43b581"));
    Button* pauseButton = new Button("Pause", QColor("#f04747"));
    QHBoxLayout buttonsLayout;


    slidersLayout.addWidget(m1S);
    slidersLayout.addWidget(m2S);
    slidersLayout.addWidget(l1S);
    slidersLayout.addWidget(l2S);
    slidersLayout.addWidget(angle1S);
    slidersLayout.addWidget(angle2S);
    slidersLayout.addWidget(dampingS);
    slidersLayout.addWidget(gravityS);
    slidersLayout.addWidget(timeS);
    slidersLayout.addWidget(startButton);
    slidersLayout.addSpacing(10);
    slidersLayout.addLayout(&buttonsLayout);
    slidersLayout.addSpacing(5);
    slidersLayout.addWidget(description);

    DoublePendulum* pendulum = new DoublePendulum;
    PendulumDisplay* pendulumDisplay = new PendulumDisplay(pendulum);
    PendulumPlot* pendulumPlots = new PendulumPlot(pendulum);
    pendulumDisplay->setFps(60);
    QTimer* timer = new QTimer;
    timer->setInterval(pendulum->precision * 1000); // doesnt change actually. need to fix this

    QObject::connect(calculateButton, SIGNAL(clicked()), pendulum, SLOT(calculate()));
    QObject::connect(calculateButton, SIGNAL(clicked()), pendulumDisplay, SLOT(startUpdate()));
    QObject::connect(calculateButton, SIGNAL(clicked()), pendulumDisplay, SLOT(reset()));
    QObject::connect(calculateButton, SIGNAL(clicked()), pendulumPlots, SLOT(reset()));
    QObject::connect(calculateButton, SIGNAL(clicked()), timer, SLOT(start()));

    QObject::connect(startButton, SIGNAL(clicked()), timer, SLOT(start()));
    QObject::connect(startButton, SIGNAL(clicked()), pendulumDisplay, SLOT(startUpdate()));

    QObject::connect(pauseButton, SIGNAL(clicked()), timer, SLOT(stop()));
    QObject::connect(pauseButton, SIGNAL(clicked()), pendulumDisplay, SLOT(stopUpdate()));

    QObject::connect(timeS, SIGNAL(valueChanged(double)), pendulum, SLOT(setTime(double)));

    QObject::connect(m1S, SIGNAL(valueChanged(double)), pendulum, SLOT(setMass1(double)));
    QObject::connect(m1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(m1S, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(m1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(m1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(m1S, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(m2S, SIGNAL(valueChanged(double)), pendulum, SLOT(setMass2(double)));
    QObject::connect(m2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(m2S, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(m2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(m2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(m2S, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(l1S, SIGNAL(valueChanged(double)), pendulum, SLOT(setLen1(double)));
    QObject::connect(l1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(l1S, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(l1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(l1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(l1S, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(l2S, SIGNAL(valueChanged(double)), pendulum, SLOT(setLen2(double)));
    QObject::connect(l2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(l2S, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(l2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(l2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(l2S, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(angle1S, SIGNAL(valueChanged(double)), pendulum, SLOT(setStartAngle1(double)));
    QObject::connect(angle1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(angle1S, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(angle1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(angle1S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(angle1S, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(angle2S, SIGNAL(valueChanged(double)), pendulum, SLOT(setStartAngle2(double)));
    QObject::connect(angle2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(angle2S, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(angle2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(angle2S, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(angle2S, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(dampingS, SIGNAL(valueChanged(double)), pendulum, SLOT(setDamping(double)));
    QObject::connect(dampingS, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(dampingS, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(dampingS, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(dampingS, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(dampingS, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(gravityS, SIGNAL(valueChanged(double)), pendulum, SLOT(setGravity(double)));
    QObject::connect(gravityS, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(update()));
    QObject::connect(gravityS, SIGNAL(valueChanged(double)), pendulum, SLOT(calculateFirstIter()));
    QObject::connect(gravityS, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(stopUpdate()));
    QObject::connect(gravityS, SIGNAL(valueChanged(double)), pendulumDisplay, SLOT(reset()));
    QObject::connect(gravityS, SIGNAL(valueChanged(double)), pendulumPlots, SLOT(reset()));

    QObject::connect(pendulumPlots, SIGNAL(plotClicked()), timer, SLOT(stop()));

    QObject::connect(timer, SIGNAL(timeout()), pendulumDisplay, SLOT(step()));
    QObject::connect(timer, SIGNAL(timeout()), pendulumPlots, SLOT(step()));

    m1S->setValue(1);
    m1S->setMinimum(0.1);
    m2S->setValue(0);
    l1S->setValue(0.5);
    l2S->setValue(0.5);
    l1S->setMinimum(0.1);
    l2S->setMinimum(0.1);
    angle1S->setSliderPresicion(1);
    angle2S->setSliderPresicion(1);
    angle1S->setValue(90);
    angle2S->setValue(90);

    gravityS->setValue(9.81);
    timeS->setValue(50);
    dampingS->setSliderPresicion(100);
    pendulum->calculateFirstIter();

    layout.addWidget(pendulumPlots, 1);
    layout.addWidget(pendulumDisplay, 2);
    layout.addWidget(&sliders, 1);
    layout.setMargin(0);
    layout.setSpacing(0);

    buttonsLayout.addWidget(startButton, 1);
    buttonsLayout.addWidget(calculateButton, 2);
    buttonsLayout.addWidget(pauseButton, 1);
    buttonsLayout.setSpacing(11);

    slidersLayout.setSpacing(0);

    window.resize(1400, 700);
    window.show();

    return app.exec();
}
