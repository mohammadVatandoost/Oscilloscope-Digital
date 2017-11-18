#include "samplingthread2.h"
#include "signaldata2.h"
#include <qwt_math.h>
#include <math.h>
#include "QDebug"

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread2::SamplingThread2( QObject *parent ):
    QwtSamplingThread( parent ),
    d_frequency( 5.0 ),
    d_amplitude( 20.0 )
{
}

void SamplingThread2::setFrequency( double frequency )
{
    d_frequency = frequency;
}

double SamplingThread2::frequency() const
{
    return d_frequency;
}

void SamplingThread2::setAmplitude( double amplitude )
{
    d_amplitude = amplitude;
}

double SamplingThread2::amplitude() const
{
    return d_amplitude;
}

float SamplingThread2::getChannelValue(float f)
{
    return Channel_value;
}

void SamplingThread2::sample( double elapsed )
{
    if ( d_frequency > 0.0 )
    {
//        const QPointF s( elapsed, takeValue( elapsed ) );
//        SignalData::instance().append( s );
        const QPointF s( elapsed, Channel_value );
        SignalData2::instance().append( s );
//        qDebug() << "elapsed" << elapsed;
    }
}

float SamplingThread2::takeValue( float d )
{
    Channel_value = d;
//    qDebug() << "Channel_value" << Channel_value;
    return d;
}
