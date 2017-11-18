#include "curvedata2.h"
#include "signaldata2.h"

const SignalData2 &CurveData2::values() const
{
    return SignalData2::instance();
}

SignalData2 &CurveData2::values()
{
    return SignalData2::instance();
}

QPointF CurveData2::sample( size_t i ) const
{
    return SignalData2::instance().value( i );
}

size_t CurveData2::size() const
{
    return SignalData2::instance().size();
}

QRectF CurveData2::boundingRect() const
{
    return SignalData2::instance().boundingRect();
}
