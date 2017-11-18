#include "signaldata2.h"
#include <qvector.h>
#include <qmutex.h>
#include <qreadwritelock.h>
#include <QDebug>

class SignalData2::PrivateData
{
public:
    PrivateData():
        boundingRect( 1.0, 1.0, -2.0, -2.0 ) // invalid
    {
        values.reserve( 1000 );
    }

    inline void append( const QPointF &sample )
    {
        values.append( sample );

        // adjust the bounding rectangle

        if ( boundingRect.width() < 0 || boundingRect.height() < 0 )
        {
            boundingRect.setRect( sample.x(), sample.y(), 0.0, 0.0 );
        }
        else
        {
            boundingRect.setRight( sample.x() );

            if ( sample.y() > boundingRect.bottom() )
                boundingRect.setBottom( sample.y() );

            if ( sample.y() < boundingRect.top() )
                boundingRect.setTop( sample.y() );
        }
    }

    QReadWriteLock lock;

    QVector<QPointF> values;
    QRectF boundingRect;

    QMutex mutex; // protecting pendingValues
    QVector<QPointF> pendingValues;
};

SignalData2::SignalData2()
{
    d_data = new PrivateData();
}

SignalData2::~SignalData2()
{
    delete d_data;
}

int SignalData2::size() const
{
    return d_data->values.size();
}

QPointF SignalData2::value( int index ) const
{
    return d_data->values[index];
}

QRectF SignalData2::boundingRect() const
{
    return d_data->boundingRect;
}

void SignalData2::lock()
{
    d_data->lock.lockForRead();
}

void SignalData2::unlock()
{
    d_data->lock.unlock();
}

void SignalData2::append( const QPointF &sample )
{
    d_data->mutex.lock();
    d_data->pendingValues += sample;
    k++;
    if(k%25 == 0) {
////        k = 0 ;
//        qDebug() << "K:" << k ;
        const bool isLocked = d_data->lock.tryLockForWrite();
        if ( isLocked )
        {
            const int numValues = d_data->pendingValues.size();
            const QPointF *pendingValues = d_data->pendingValues.data();

            for ( int i = 0; i < numValues; i++ )
             {
                d_data->append( pendingValues[i] );
    //            qDebug() << "pendingValues[i]" << pendingValues[i];
             }
            d_data->pendingValues.clear();

            d_data->lock.unlock();
        }
    }
    d_data->mutex.unlock();
}

void SignalData2::clearStaleValues( double limit )
{
    d_data->lock.lockForWrite();

    d_data->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid

    const QVector<QPointF> values = d_data->values;
    d_data->values.clear();
    d_data->values.reserve( values.size() );

    int index;
    for ( index = values.size() - 1; index >= 0; index-- )
    {
        if ( values[index].x() < limit )
            break;
    }

    if ( index > 0 )
        d_data->append( values[index++] );

    while ( index < values.size() - 1 )
        d_data->append( values[index++] );

    d_data->lock.unlock();
}

SignalData2 &SignalData2::instance()
{
    static SignalData2 valueVector;
    return valueVector;
}
