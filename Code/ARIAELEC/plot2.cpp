#include "plot2.h"


class Canvas: public QwtPlotCanvas
{
public:
    Canvas( QwtPlot *plot2 = NULL ):
        QwtPlotCanvas( plot2 )
    {
        // The backing store is important, when working with widget
        // overlays ( f.e rubberbands for zooming ).
        // Here we don't have them and the internal
        // backing store of QWidget is good enough.

        setPaintAttribute( QwtPlotCanvas::BackingStore, false );
        setBorderRadius( 10 );

        if ( QwtPainter::isX11GraphicsSystem() )
        {
#if QT_VERSION < 0x050000
            // Even if not liked by the Qt development, Qt::WA_PaintOutsidePaintEvent
            // works on X11. This has a nice effect on the performance.

            setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif

            // Disabling the backing store of Qt improves the performance
            // for the direct painter even more, but the canvas becomes
            // a native window of the window system, receiving paint events
            // for resize and expose operations. Those might be expensive
            // when there are many points and the backing store of
            // the canvas is disabled. So in this application
            // we better don't disable both backing stores.

            if ( testPaintAttribute( QwtPlotCanvas::BackingStore ) )
            {
                setAttribute( Qt::WA_PaintOnScreen, true );
                setAttribute( Qt::WA_NoSystemBackground, true );
            }
        }

        setupPalette();
    }

private:
    void setupPalette()
    {
        QPalette pal = palette();
//        QPalette pal_2 = palette();

#if QT_VERSION >= 0x040400
        QLinearGradient gradient;
        gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
        gradient.setColorAt( 0.0, QColor( 0, 49, 110 ) );
        gradient.setColorAt( 1.0, QColor( 0, 87, 174 ) );

        pal.setBrush( QPalette::Window, QBrush( gradient ) );
//        pal_2.setBrush( QPalette::Window, QBrush( gradient ) );
#else
        pal.setBrush( QPalette::Window, QBrush( color ) );
//        pal_2.setBrush( QPalette::Window, QBrush( color ) );
#endif

        // QPalette::WindowText is used for the curve color
        pal.setColor( QPalette::WindowText, Qt::green );
//        pal_2.setColor( QPalette::WindowText, Qt::yellow );

        setPalette( pal );
       // setPalette( pal_2 );

    }
};

Plot2::Plot2( QWidget *parent ):
    QwtPlot( parent ),
    d_paintedPoints( 0 ),
    d_interval( 0.0, 10.0 ),
    d_timerId( -1 )
{
    d_directPainter = new QwtPlotDirectPainter();

    setAutoReplot( false );
    setCanvas( new Canvas() );

    plotLayout()->setAlignCanvasToScales( true );

    setAxisTitle( QwtPlot::xBottom, "Time [s]" );
    setAxisScale( QwtPlot::xBottom, d_interval.minValue(), d_interval.maxValue() );
    setAxisScale( QwtPlot::yLeft, 0, 5 );

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( false );
    grid->attach( this );

    d_origin = new QwtPlotMarker();
    d_origin->setLineStyle( QwtPlotMarker::Cross );
    d_origin->setValue( d_interval.minValue() + d_interval.width() / 2.0, 0.0 );
    d_origin->setLinePen( Qt::gray, 0.0, Qt::DashLine );
    d_origin->attach( this );

    d_curve = new QwtPlotCurve();
    d_curve->setStyle( QwtPlotCurve::Lines );
    d_curve->setPen( canvas()->palette().color( QPalette::WindowText ) );
    d_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    // change false
    d_curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, true );
    d_curve->setData( new CurveData2() );
    d_curve->attach( this );
}

Plot2::~Plot2()
{
    delete d_directPainter;
//    delete d_directPainter_2;
}

void Plot2::start()
{
    d_clock.start();
    d_timerId = startTimer( 10 );
}

void Plot2::replot()
{
    CurveData2 *data = static_cast<CurveData2 *>( d_curve->data() );
//    CurveData *data_2 = static_cast<CurveData *>( d_curve_2->data() );
    data->values().lock();
//    data_2->values().lock();
    QwtPlot::replot();
    d_paintedPoints = data->size();
//    d_paintedPoints_2 = data_2->size();

    data->values().unlock();
//    data_2->values().unlock();
}

void Plot2::setIntervalLength( double interval )
{
    if ( interval > 0.0 && interval != d_interval.width() )
    {
        d_interval.setMaxValue( d_interval.minValue() + interval );
        setAxisScale( QwtPlot::xBottom,
            d_interval.minValue(), d_interval.maxValue() );

        replot();
    }
}

void Plot2::updateCurve()
{
    CurveData2 *data = static_cast<CurveData2 *>( d_curve->data() );
//    CurveData2 *data_2 = static_cast<CurveData2 *>( d_curve_2->data() );
    data->values().lock();
//    data_2->values().lock();

    const int numPoints = data->size();
//    const int numPoints_2 = data_2->size();
    if ( numPoints > d_paintedPoints )
    {
        const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
        if ( doClip )
        {
            /*
                Depending on the platform setting a clip might be an important
                performance issue. F.e. for Qt Embedded this reduces the
                part of the backing store that has to be copied out - maybe
                to an unaccelerated frame buffer device.
            */

            const QwtScaleMap xMap = canvasMap( d_curve->xAxis() );
            const QwtScaleMap yMap = canvasMap( d_curve->yAxis() );

            QRectF br = qwtBoundingRect( *data,
                d_paintedPoints - 1, numPoints - 1 );

            const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
            d_directPainter->setClipRegion( clipRect );
        }

        d_directPainter->drawSeries( d_curve,
            d_paintedPoints - 1, numPoints - 1 );

        d_paintedPoints = numPoints;
    }

    //************************************
//    if ( numPoints_2 > d_paintedPoints_2 )
//    {
//        const bool doClip_2 = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
//        if ( doClip_2 )
//        {
////            /*
////                Depending on the platform setting a clip might be an important
////                performance issue. F.e. for Qt Embedded this reduces the
////                part of the backing store that has to be copied out - maybe
////                to an unaccelerated frame buffer device.
////            */
//            const QwtScaleMap xMap_2 = canvasMap( d_curve_2->xAxis() );
//            const QwtScaleMap yMap_2 = canvasMap( d_curve_2->yAxis() );

////            QRectF br_2 = qwtBoundingRect( *data_2,
////                d_paintedPoints_2 - 1, numPoints_2 - 1 );

////                        const QRect clipRect_2 = QwtScaleMap::transform( xMap_2, yMap_2, br_2 ).toRect();
////                        d_directPainter_2->setClipRegion( clipRect_2 );
//        }

//                d_directPainter_2->drawSeries( d_curve_2,
//                    d_paintedPoints - 1, numPoints - 1 );

//                d_paintedPoints_2 = numPoints;
//    }
    //***********************************

    data->values().unlock();
//    data_2->values().unlock();
}

void Plot2::incrementInterval()
{
    d_interval = QwtInterval( d_interval.maxValue(),
        d_interval.maxValue() + d_interval.width() );

    CurveData2 *data = static_cast<CurveData2 *>( d_curve->data() );
    data->values().clearStaleValues( d_interval.minValue() );

//    CurveData2 *data_2 = static_cast<CurveData2 *>( d_curve_2->data() );
//    data_2->values().clearStaleValues( d_interval.minValue() );

    // To avoid, that the grid is jumping, we disable
    // the autocalculation of the ticks and shift them
    // manually instead.

    QwtScaleDiv scaleDiv = axisScaleDiv( QwtPlot::xBottom );
    scaleDiv.setInterval( d_interval );

    for ( int i = 0; i < QwtScaleDiv::NTickTypes; i++ )
    {
        QList<double> ticks = scaleDiv.ticks( i );
        for ( int j = 0; j < ticks.size(); j++ )
            ticks[j] += d_interval.width();
        scaleDiv.setTicks( i, ticks );
    }
    setAxisScaleDiv( QwtPlot::xBottom, scaleDiv );

    d_origin->setValue( d_interval.minValue() + d_interval.width() / 2.0, 0.0 );

    d_paintedPoints = 0;
//    d_paintedPoints_2 = 0 ;
    replot();
}

void Plot2::timerEvent( QTimerEvent *event )
{
    if ( event->timerId() == d_timerId )
    {
        updateCurve();

        const double elapsed = d_clock.elapsed() / 1000.0;
        if ( elapsed > d_interval.maxValue() )
            incrementInterval();

        return;
    }

    QwtPlot::timerEvent( event );
}

void Plot2::resizeEvent( QResizeEvent *event )
{
    d_directPainter->reset();
    QwtPlot::resizeEvent( event );
}

void Plot2::showEvent( QShowEvent * )
{
    replot();
}

bool Plot2::eventFilter( QObject *object, QEvent *event )
{
    if ( object == canvas() &&
        event->type() == QEvent::PaletteChange )
    {
        d_curve->setPen( canvas()->palette().color( QPalette::WindowText ) );

//        QPalette pal_2 = palette();
//    //    pal_2.setBrush( QPalette::Window, QBrush( yellow ) );
//        pal_2.setColor( QPalette::WindowText, Qt::yellow );
//        setPalette( pal_2 );
//        d_curve_2->setPen( canvas()->palette().color( QPalette::WindowText ) );
    }

    return QwtPlot::eventFilter( object, event );
}
