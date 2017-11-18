#ifndef _SIGNAL_DATA2_H_
#define _SIGNAL_DATA2_H_ 1

#include <qrect.h>

class SignalData2
{
public:
    static SignalData2 &instance();

    void append( const QPointF &pos );
    void clearStaleValues( double min );

    int size() const;
    QPointF value( int index ) const;
    int k = 0;
    QRectF boundingRect() const;

    void lock();
    void unlock();

private:
    SignalData2();
    SignalData2( const SignalData2 & );
    SignalData2 &operator=( const SignalData2 & );

    virtual ~SignalData2();

    class PrivateData;
    PrivateData *d_data;
};

#endif
