#include "samplingthread.h"
#include "signaldata.h"
#include <qwt_math.h>
#include <math.h>
#include "QDebug"

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread::SamplingThread( QObject *parent ):
    QwtSamplingThread( parent ),
    d_frequency( 5.0 ),
    d_amplitude( 20.0 )
{
}

//void SamplingThread::setFrequency( double frequency )
//{
//    d_frequency = frequency;
//}

//double SamplingThread::frequency() const
//{
//    return d_frequency;
//}

void SamplingThread::setAmplitude( double amplitude )
{
    d_amplitude = amplitude;
}

double SamplingThread::amplitude() const
{
    return d_amplitude;
}

//float SamplingThread::getChannelValue(float f)
//{
//    return Channel_value;
//}

void SamplingThread::sample( double elapsed )
{
//    if ( d_frequency > 0.0 )
//    {
//        const QPointF s( elapsed, takeValue( elapsed ) );
//        SignalData::instance().append( s );
        const QPointF s( elapsed, Channel_value );
        SignalData::instance().append( s );
//        qDebug() << "elapsed" << elapsed;
//        i++;
//        qDebug() << "i : " << i;
//    }
}

float SamplingThread::takeValue( float d )
{
    Channel_value = d;

//    qDebug() << "Channel_value" << Channel_value;
    return d;
}
