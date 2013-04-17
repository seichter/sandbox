
#include "webwidget.h"

MyScriptSystem::MyScriptSystem( QObject* parent /*= 0*/ ) 
	: QObject(parent)
{
}


void MyScriptSystem::setPixmap(const QPixmap& pixmap)
{
	QImage image = pixmap.toImage();

	qDebug() << image.width() << "x" <<image.height() << " " << image.bytesPerLine() << " " << image.bitPlaneCount() << image.byteCount();
}

void MyScriptSystem::setImage( const QImage& image )
{
	qDebug() << image.width() << "x" <<image.height() << " " << image.bytesPerLine() << " " << image.bitPlaneCount() << image.byteCount();
}

MyScriptSystem::~MyScriptSystem()
{
}

#include "webwidget.moc"