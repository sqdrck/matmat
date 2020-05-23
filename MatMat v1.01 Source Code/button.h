#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QPainter>

class Button : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QRect backgroundRect READ getBackgroundRect WRITE setBackgroundRect);
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)

private:
    QRect backgroundRect;
    QRect hoverBackgroundRect;
    QColor backgroundColor;
    QColor initialBackgroundColor;
    QColor hoverBackgroundColor;
    QColor pressBackgroundColor;
    QPropertyAnimation* backgroundRectAnimation;
    QPropertyAnimation* backgroundColorAnimation;

    int popSize = 15;

public:
    Button(const QString& text, QColor col, QPushButton* parent = 0) : QPushButton(text, parent){
        backgroundRectAnimation = new QPropertyAnimation(this, "backgroundRect");
        backgroundRectAnimation->setEasingCurve(QEasingCurve::OutCubic);
        backgroundRectAnimation->setDuration(180);
        backgroundColorAnimation = new QPropertyAnimation(this, "backgroundColor");
        backgroundColorAnimation->setEasingCurve(QEasingCurve::OutCubic);
        backgroundColorAnimation->setDuration(180);


        backgroundColor = col;
        initialBackgroundColor = backgroundColor;
        hoverBackgroundColor.setHsl(col.hue(),
                               col.saturation() - 30,
                               col.lightness() + 30);
        pressBackgroundColor.setHsl(col.hue(),
                          col.saturation() - 20,
                          col.lightness() - 50);
    }

    const QRect& getBackgroundRect(){
        return backgroundRect;
    }

    const QColor& getBackgroundColor(){
        return backgroundColor;
    }

    void setBackgroundRect(const QRect& r){
        backgroundRect = r;
        update();
    }

    void setBackgroundColor(const QColor& c){
        backgroundColor = c;
        update();
    }

    void resizeBackgroundRect(){
        hoverBackgroundRect = QRect(this->rect().x() + popSize/2,
                                     this->rect().y() +  popSize/2,
                                     this->rect().width() -  popSize,
                                     this->rect().height() - popSize);
        setBackgroundRect(hoverBackgroundRect);
    }

protected:
    virtual void paintEvent(QPaintEvent* event) override{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath roundedRect;

        roundedRect.addRoundedRect(backgroundRect,
                                   backgroundRect.height()/2.4f,
                                   backgroundRect.height()/2.4);
        painter.fillPath(roundedRect, backgroundColor);
        QPushButton::paintEvent(event);
    }

    virtual void enterEvent(QEvent *event) override{
//        qDebug() << "Entered" << this->objectName();

        // Resize background rect
        backgroundRectAnimation->stop();
        backgroundRectAnimation->setStartValue(backgroundRect);
        backgroundRectAnimation->setEndValue(this->rect());
        backgroundRectAnimation->start();


        // Change background color
        backgroundColorAnimation->stop();
        backgroundColorAnimation->setStartValue(initialBackgroundColor);
        backgroundColorAnimation->setEndValue(hoverBackgroundColor);
        backgroundColorAnimation->start();
        QWidget::enterEvent(event);
    }

    virtual void leaveEvent(QEvent *event) override{
//        qDebug() << "Leaved" << this->objectName();

        // Resize background rect
        backgroundRectAnimation->stop();
        backgroundRectAnimation->setStartValue(backgroundRect);
        backgroundRectAnimation->setEndValue(hoverBackgroundRect);
        backgroundRectAnimation->start();


        // Change background color
        backgroundColorAnimation->stop();
        backgroundColorAnimation->setStartValue(hoverBackgroundColor);
        backgroundColorAnimation->setEndValue(initialBackgroundColor);
        backgroundColorAnimation->start();
        QWidget::leaveEvent(event);
    }

    virtual void mousePressEvent(QMouseEvent *event) override{
        // Resize background rect
        backgroundRectAnimation->stop();
        backgroundRectAnimation->setStartValue(hoverBackgroundRect);
        backgroundRectAnimation->setEndValue(backgroundRect);
        backgroundRectAnimation->start();

        // Change background color
        backgroundColorAnimation->stop();
        backgroundColorAnimation->setStartValue(hoverBackgroundColor);
        backgroundColorAnimation->setEndValue(pressBackgroundColor);
        backgroundColorAnimation->start();
        emit clicked();
        QWidget::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QMouseEvent *event) override{
        // Change background color
        backgroundColorAnimation->stop();
        backgroundColorAnimation->setStartValue(pressBackgroundColor);
        backgroundColorAnimation->setEndValue(hoverBackgroundColor);
        backgroundColorAnimation->start();
        QWidget::mouseReleaseEvent(event);
    }

    virtual void resizeEvent(QResizeEvent *event) override{
        resizeBackgroundRect();
        QWidget::resizeEvent(event);
    }

};


#endif // BUTTON_H
