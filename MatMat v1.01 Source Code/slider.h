#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>

class Slider : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QRect backgroundRect READ getBackgroundRect WRITE setBackgroundRect)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
private:
    QString labelName;
    double to = 10;

    float popSize = 0.035f; // from 0 to 1

    float spacing = 0.02f; //space in begining
    float labelSize  = 0.2f; // from 0 to 1
    float sliderSize = 0.6f; // from 0 to 1
    float valueSize = 0.2f; // from 0 to 1
    float fontSize10 = 0.3f; // from 0 to 1
    float fontSize100 = 0.25f; // from 0 to 1
    float fontSize1000 = 0.2f; // from 0 to 1

    QSlider* slider;
    int sliderPresicion = 100;

    QLabel* label;
    QLabel* value;
    QFont* font;

    QHBoxLayout* layout;

    // Background
    QRect backgroundRect; //animated
    QRect shrinkedBackgroundRect; //target

    QColor color; //initial color
    QColor backgroundColor; //animated
    QColor hoverColor; //target used to paint even more lighted background color when hovered
    QColor lightedColor; //target. used to paint default background color
    QColor colorText;

    QPropertyAnimation* backgroundRectAnimator;
    QPropertyAnimation* backgroundColorAnimator;

    void resizeBackgroundRect(){
        shrinkedBackgroundRect = QRect(this->rect().x() + this->rect().width() * popSize/2,
                                     this->rect().y() + this->rect().width() *  popSize/2,
                                     this->rect().width() -  this->rect().width() * popSize,
                                     this->rect().height() - this->rect().width() * popSize);
        setBackgroundRect(shrinkedBackgroundRect);
    }
    void initAnimators(){

        backgroundRectAnimator = new QPropertyAnimation(this, "backgroundRect");
        backgroundRectAnimator->setEasingCurve(QEasingCurve::OutCubic);
        backgroundRectAnimator->setDuration(180);

        backgroundColorAnimator = new QPropertyAnimation(this, "backgroundColor");
        backgroundColorAnimator->setEasingCurve(QEasingCurve::OutCubic);
        backgroundColorAnimator->setDuration(180);
    }
    void initColors(){
        lightedColor.setHsl(color.hue(),
                               color.saturation() - 40,
                               color.lightness() + 60);
        backgroundColor = lightedColor;
        hoverColor.setHsl(color.hue(),
                          color.saturation() - 30,
                          color.lightness() + 30);
        colorText.setHsl(color.hue(),
                          color.saturation() - 20,
                          color.lightness() - 100);
    }
    void initSlider(){
        slider = new QSlider(Qt::Horizontal, this);
        slider->setMaximum(to * sliderPresicion);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setValueBySlider(int)));
        slider->setPageStep(1);
        resizeSlider();
    }
    void initFont(){
        font = new QFont("SF Pro Text");
        resizeFont();
    }
    void initLabel(){
        label = new QLabel(this);
        label->setText(labelName);
        label->setStyleSheet(QString("QLabel { color : ") + QString(colorText.name()) + QString("; }"));
        resizeLabel();
    }
    void initValue(){
        value = new QLabel(this);
        value->setStyleSheet(QString("QLabel { color : ") + QString(colorText.name()) + QString("; }"));
        resizeValue();
    }
    void resizeSlider(){
        QRect rect(label->geometry().x() + label->width(),
                   0,
                   this->rect().width() * sliderSize,
                   this->rect().height());
        slider->setGeometry(rect);
        setSliderStyle();

    }
    void resizeLabel(){
        QRect rect(this->rect().width() * spacing * 3,
                   0,
                   this->rect().width() * labelSize,
                   this->rect().height());
        label->setFont(*font);
        label->setGeometry(rect);
    }
    void resizeValue(){
        QRect rect(slider->geometry().x() + slider->geometry().width() + this->rect().width() * spacing,
                   0,
                   this->rect().width() * valueSize,
                   this->rect().height());
        value->setGeometry(rect);
        value->setFont(*font);
    }
    void resizeFont(){
        font->setWeight(QFont::Black);
        font->setPixelSize(this->rect().height() * fontSize10);
        font->setStyleStrategy(QFont::PreferAntialias);
    }
    void setSliderStyle(){
        QString styleSheet;
        styleSheet = QString("QSlider:handle:horizontal { width : ")
                + QString::number(int(rect().height()/2.5))
                + QString("px; ")
                + QString("border-radius : ")
                + QString::number(int(rect().height()/2.5/2) - 2)
                + QString("px; ")
                + QString("margin : ")
                + QString::number(int(-rect().height()/8))
                + QString("px; }")

                + QString("QSlider::groove:horizontal { ")
                + QString("height : ")
                + QString::number(int(rect().height() * 0.15f))
                + QString("px; ")
                + QString("width : ")
                + QString::number(int(rect().width() * sliderSize - int(rect().height()/2.5)/1.6f))
                + QString("px; ")
                + QString("border-radius: ")
                + QString::number(int(rect().height() * 0.15f/2)-1)
                + QString("px; }")

                + QString("QSlider::sub-page { ")
                + QString("background : ")
                + QString(color.name())
                + QString("; ")
                + QString("border-radius: ")
                + QString::number(int(rect().height() * 0.15f/2)-1)
                + QString("px; }")

                + QString("QSlider::add-page { ")
                + QString("border-radius: ")
                + QString::number(int(rect().height() * 0.15f/2)-1)
                + QString("px; }")

                + QString("QSlider::handle:horizontal:hover { ")
                + QString("background: ")
                + QString(color.name())
                + QString("; }");

//        qDebug() << styleSheet;
        slider->setStyleSheet(styleSheet);
    }
public:
    Slider(const QString& label = "lbl", const double& to = 10, const QColor color = Qt::gray, QWidget* parent = 0)
        : QWidget(parent), labelName(label), to(to), color(color){
        setObjectName(label);

        // Size
        QSizePolicy policy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        policy.setHeightForWidth(true);
        setSizePolicy(policy);
        updateGeometry();

        // Init
        initColors();
        resizeBackgroundRect();
        initFont();
        initLabel();
        initSlider();
        initValue();
        initAnimators();

        connect(this, SIGNAL(valueChanged(double)), value, SLOT(setNum(double)));
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(checkValueFont(int)));
        value->setNum(slider->value() * 1.0 / sliderPresicion);
    }

    // Property getters setter
    const QRect& getBackgroundRect(){
        return backgroundRect;
    }

    const QColor& getBackgroundColor(){
        return hoverColor;
    }

    void setBackgroundRect(const QRect& r){
        backgroundRect = r;
        update();
    }

    void setBackgroundColor(const QColor& c){
        backgroundColor = c;
        update();
    }
    // other
    double getValue(){
        return slider->value() * 1.0f / sliderPresicion;
    }

    void setSliderPresicion(int n){
        sliderPresicion = n;
//        slider->setMaximum(slider)
    }
protected:
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath roundedRect;

        roundedRect.addRoundedRect(backgroundRect,
                                   backgroundRect.height()/2.4f,
                                   backgroundRect.height()/2.4);
        painter.fillPath(roundedRect, backgroundColor);

        QWidget::paintEvent(event);
    }

    virtual void enterEvent(QEvent *event) override{
//        qDebug() << "Entered" << this->objectName();

        // Resize background rect
        backgroundRectAnimator->stop();
        backgroundRectAnimator->setStartValue(backgroundRect);
        backgroundRectAnimator->setEndValue(this->rect());
        backgroundRectAnimator->start();


        // Change background color
        backgroundColorAnimator->stop();
        backgroundColorAnimator->setStartValue(backgroundColor);
        backgroundColorAnimator->setEndValue(hoverColor);
        backgroundColorAnimator->start();
        QWidget::enterEvent(event);
    }

    virtual void leaveEvent(QEvent *event) override{
//        qDebug() << "Leaved" << this->objectName();

        // Resize background rect
        backgroundRectAnimator->stop();
        backgroundRectAnimator->setStartValue(backgroundRect);
        backgroundRectAnimator->setEndValue(shrinkedBackgroundRect);
        backgroundRectAnimator->start();


        // Change background color
        backgroundColorAnimator->stop();
        backgroundColorAnimator->setStartValue(backgroundColor);
        backgroundColorAnimator->setEndValue(lightedColor);
        backgroundColorAnimator->start();
        QWidget::leaveEvent(event);
    }

    virtual QSize sizeHint() const override{
        return QSize(300 + 300 * popSize, 25 + 25 * popSize);
    }

    virtual int heightForWidth(int w) const override{
        return w * 25 / 300;
    }

    virtual void resizeEvent(QResizeEvent *event) override{
        resizeBackgroundRect();
        resizeFont();
        resizeLabel();
        resizeSlider();
        resizeValue();
        checkValueFont(slider->value());
        QWidget::resizeEvent(event);
    }

signals:
    void valueChanged(double);
public slots:
    void setValue(double n){
        slider->setValue(n * sliderPresicion);
        emit valueChanged(n);
    }
    void setMinimum(double n){
        slider->setMinimum(n * sliderPresicion);
    }
private slots:
    void setValueBySlider(int n){
        emit valueChanged(n * 1.0f / sliderPresicion);
    }
    void checkValueFont(int n){
        double currValue = n * 1.0f / sliderPresicion;
        if (currValue < 1)
            font->setPixelSize(this->rect().height() * fontSize100);
        else if(currValue <= 10)
            font->setPixelSize(this->rect().height() * fontSize10);
        else if(currValue <= 100)
            font->setPixelSize(this->rect().height() * fontSize100);
        else if(currValue <= 1000)
            font->setPixelSize(this->rect().height() * fontSize1000);
        value->setFont(*font);
    }
};

#endif // SLIDER_H
