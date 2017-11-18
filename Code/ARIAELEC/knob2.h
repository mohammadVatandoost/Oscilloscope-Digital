#ifndef _KNOB2_H_
#define _KNOB2_H_

#include <qwidget.h>

class QwtKnob;
class QLabel;

class Knob2: public QWidget
{
    Q_OBJECT

    Q_PROPERTY( QColor theme READ theme WRITE setTheme )

public:
    Knob2( const QString &title,
        double min, double max, QWidget *parent = NULL );

    virtual QSize sizeHint() const;

    void setValue( double value );
    double value() const;

    void setTheme( const QColor & );
    QColor theme() const;

Q_SIGNALS:
    double valueChanged( double );

protected:
    virtual void resizeEvent( QResizeEvent * );

private:
    QwtKnob *d_knob;
    QLabel *d_label;
};

#endif
