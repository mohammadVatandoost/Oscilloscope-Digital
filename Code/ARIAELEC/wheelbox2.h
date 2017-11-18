#ifndef _WHEELBOX2_H_
#define _WHEELBOX2_H_

#include <qwidget.h>

class QwtWheel;
class QLabel;
class QLCDNumber;

class WheelBox2: public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QColor theme READ theme WRITE setTheme )

public:
    WheelBox2( const QString &title,
        double min, double max, double stepSize,
        QWidget *parent = NULL );

    void setTheme( const QColor & );
    QColor theme() const;

    void setUnit( const QString & );
    QString unit() const;

    void setValue( double value );
    double value() const;

Q_SIGNALS:
    double valueChanged( double );

private:
    QLCDNumber *d_number;
    QwtWheel *d_wheel;
    QLabel *d_label;

    QString d_unit;
};

#endif
