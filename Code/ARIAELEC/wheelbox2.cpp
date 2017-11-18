#include "wheelbox2.h"
#include <qwt_wheel.h>
#include <qlcdnumber.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qevent.h>
#include <qapplication.h>

class Wheel2: public QwtWheel
{
public:
    Wheel2( WheelBox2 *parent ):
        QwtWheel( parent )
    {
        setFocusPolicy( Qt::WheelFocus );
        parent->installEventFilter( this );
    }

    virtual bool eventFilter( QObject *object, QEvent *event )
    {
        if ( event->type() == QEvent::Wheel )
        {
            const QWheelEvent *we = static_cast<QWheelEvent *>( event );

            QWheelEvent wheelEvent( QPoint( 5, 5 ), we->delta(),
                we->buttons(), we->modifiers(),
                we->orientation() );

            QApplication::sendEvent( this, &wheelEvent );
            return true;
        }
        return QwtWheel::eventFilter( object, event );
    }
};

WheelBox2::WheelBox2( const QString &title,
        double min, double max, double stepSize, QWidget *parent ):
    QWidget( parent )
{

    d_number = new QLCDNumber( this );
    d_number->setSegmentStyle( QLCDNumber::Filled );
    d_number->setAutoFillBackground( true );
    d_number->setFixedHeight( d_number->sizeHint().height() * 2 );
    d_number->setFocusPolicy( Qt::WheelFocus );

    QPalette pal( Qt::black );
    pal.setColor( QPalette::WindowText, Qt::green );
    d_number->setPalette( pal );

    d_wheel = new Wheel2( this );
    d_wheel->setOrientation( Qt::Vertical );
    d_wheel->setInverted( true );
    d_wheel->setRange( min, max );
    d_wheel->setSingleStep( stepSize );
    d_wheel->setPageStepCount( 5 );
    d_wheel->setFixedHeight( d_number->height() );

    d_number->setFocusProxy( d_wheel );

    QFont font( "Helvetica", 10 );
    font.setBold( true );

    d_label = new QLabel( title, this );
    d_label->setFont( font );

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins( 0, 0, 0, 0 );
    hLayout->setSpacing( 2 );
    hLayout->addWidget( d_number, 10 );
    hLayout->addWidget( d_wheel );

    QVBoxLayout *vLayout = new QVBoxLayout( this );
    vLayout->addLayout( hLayout, 10 );
    vLayout->addWidget( d_label, 0, Qt::AlignTop | Qt::AlignHCenter );

    connect( d_wheel, SIGNAL( valueChanged( double ) ),
        d_number, SLOT( display( double ) ) );
    connect( d_wheel, SIGNAL( valueChanged( double ) ),
        this, SIGNAL( valueChanged( double ) ) );
}

void WheelBox2::setTheme( const QColor &color )
{
    d_wheel->setPalette( color );
}

QColor WheelBox2::theme() const
{
    return d_wheel->palette().color( QPalette::Window );
}

void WheelBox2::setValue( double value )
{
    d_wheel->setValue( value );
    d_number->display( value );
}

double WheelBox2::value() const
{
    return d_wheel->value();
}
