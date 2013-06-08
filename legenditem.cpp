#include "legenditem.h"

LegendItem::LegendItem()
{
    setRenderHint( QwtPlotItem::RenderAntialiased );

    QColor color( Qt::white );

    setTextPen( color );
#if 1
    setBorderPen( color );

    QColor c( Qt::gray );
    c.setAlpha( 200 );

    setBackgroundBrush( c );
#endif
}
