#ifndef PENDULUM_H
#define PENDULUM_H
#include <QObject>
#include <QtMath>

struct PendulumData{
    double t;
    double x1;
    double y1;
    double x2;
    double y2;
    double angle1;
    double angle2;
    double vel1;
    double vel2;
    double ac1;
    double ac2;
//    double eK;
};

class DoublePendulum : public QObject
{
    Q_OBJECT
public:
    DoublePendulum(QObject* parent = 0) : QObject(parent){}

    QVector<PendulumData> dataset;
    double len1;
    double len2;
    double mass1;
    double mass2;
    double startAngle1;
    double startAngle2;
    double damping = 0;
    double gravity = 9.8;
    double iters = 6250;
    double precision = 0.016;

    double period = 0;
//    double ekMax = 0;

public slots:
    void calculate(){
        // we asume we calculated first iteration already
        // first iter(calculateFirstIter) needed to display pendulum in initial state while tweaking values
        calculateFirstIter();
//        double inertiaMoment1;
//        double inertiaMoment2;
        double g = gravity * precision * precision;
        double num1;
        double num2;
        double num3;
        double num4;
        double den;
        double damp = (1 - damping *  precision);
        if(mass2 == 0)
            period = 2 * M_PI * sqrt(len1 / gravity);
        PendulumData data;
        for(double i = 1; i < iters; i ++){
            data.t = i * precision;

            num1 = -g * (2 * mass1 + mass2) * sin(dataset[i-1].angle1);
            num2 = -mass2 * g * sin(dataset[i-1].angle1 - 2 * dataset[i-1].angle2);
            num3 = -2 * sin(dataset[i-1].angle1 - dataset[i-1].angle2) * mass2;
            num4 = dataset[i-1].vel2 * dataset[i-1].vel2 * len2 + dataset[i-1].vel1 * dataset[i-1].vel1 * len1 * cos(dataset[i-1].angle1  - dataset[i-1].angle2);
            den = len1 * (2 * mass1 + mass2 - mass2 * cos(2 * dataset[i-1].angle1 - 2 * dataset[i-1].angle2));
            data.ac1 = (num1 + num2 + num3 * num4) / den;

            num1 = 2 * sin(dataset[i-1].angle1 - dataset[i-1].angle2);
            num2 = (dataset[i-1].vel1 * dataset[i-1].vel1 * len1 * (mass1 + mass2));
            num3 = g * (mass1 + mass2) * cos(dataset[i-1].angle1);
            num4 = dataset[i-1].vel2 * dataset[i-1].vel2 * len2 * mass2 * cos(dataset[i-1].angle1 - dataset[i-1].angle2);
            den = len2 * (2 * mass1 + mass2 - mass2 * cos(2 * dataset[i-1].angle1 - 2 * dataset[i-1].angle2));
            data.ac2 = (num1 * (num2 + num3 + num4)) / den;

            data.vel1 = (dataset[i-1].vel1 += data.ac1) * damp;
            data.vel2 = (dataset[i-1].vel2 += data.ac2) * damp;

            data.angle1 = dataset[i-1].angle1 += data.vel1;
            data.angle2 = dataset[i-1].angle2 += data.vel2;

            data.x1 = len1 * sin(data.angle1);
            data.y1 = len1 * cos(data.angle1);

            data.x2 = data.x1 + len2 * sin(data.angle2);
            data.y2 = data.y1 + len2 * cos(data.angle2);

            dataset.push_back(data);
        }
        dataset.shrink_to_fit();
    }

    void calculateFirstIter(){
        dataset.clear();
//        ekMax = 0;
        PendulumData data;
        data.t = precision;

        data.angle1 = startAngle1;
        data.angle2 = startAngle2;

        data.vel1 = 0; data.vel2 = 0;
        data.ac1 = 0; data.ac2 = 0;

        data.x1 = len1 * sin(data.angle1);
        data.y1 = len1 * cos(data.angle1);

        data.x2 = data.x1 + len2 * sin(data.angle2);
        data.y2 = data.y1 + len2 * cos(data.angle2);
        dataset.push_back(data);
    }

    void setLen1(double n)
    {
        len1 = n;
    }
    void setLen2(double n)
    {
        len2 = n;
    }
    void setMass1(double n)
    {
        mass1 = n;
    }
    void setMass2(double n)
    {
        mass2 = n;
    }
    void setGravity(double n)
    {
        gravity = n;
    }
    void setDamping(double n)
    {
        damping = n;
    }
    void setStartAngle1(double n)
    {
        startAngle1 = qDegreesToRadians(n);
    }
    void setStartAngle2(double n)
    {
        startAngle2 = qDegreesToRadians(n);
    }
    void setIters(double n){
        iters = int(n);
    }
    void setIters(int n){
        iters = n;
    }
    void setTime(double n){
        iters = n/precision;
    }
    void setPrecision(double n){
        precision = n;
    }
};

#endif // DOUBLEPENDULUM_H
