#include "qinteractivemanager.h"
#include "qinteractivemaptool.h"

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>


QInteractiveManager::QInteractiveManager(QWidget* viewport)
	:QObject(viewport),
	mMapTool(nullptr),
	mLastNonZoomMapTool(nullptr)
{

}


QInteractiveManager::~QInteractiveManager(void)
{
	if ( mMapTool )
	{
		mMapTool->deactivate();
		mMapTool = nullptr;
	}
	mLastNonZoomMapTool = nullptr;
}

void QInteractiveManager::setMapTool(QInteractiveMapTool *tool,
	bool clean /*= false */)
{
	if ( !tool )
		return;

	if ( mMapTool )
	{
		if ( clean )
			mMapTool->clean();

		disconnect( mMapTool, &QObject::destroyed, this, 
			&QInteractiveManager::mapToolDestroyed );
		mMapTool->deactivate();
	}

	if ( ( tool->flags() & QInteractiveMapTool::Transient )
		&& mMapTool && !( mMapTool->flags() & QInteractiveMapTool::Transient ) )
	{
		// if zoom or pan tool will be active, save old tool
		// to bring it back on right click
		// (but only if it wasn't also zoom or pan tool)
		mLastNonZoomMapTool = mMapTool;
	}
	else
	{
		mLastNonZoomMapTool = nullptr;
	}

	QInteractiveMapTool *oldTool = mMapTool;

	// set new map tool and activate it
	mMapTool = tool;
	if ( mMapTool )
	{
		connect( mMapTool, &QObject::destroyed, this,
			&QInteractiveManager::mapToolDestroyed );
		mMapTool->activate();
	}

	emit mapToolSet( mMapTool, oldTool );
}

QInteractiveMapTool* QInteractiveManager::mapTool()
{
	return mMapTool;
}


void QInteractiveManager::unsetMapTool( QInteractiveMapTool *tool )
{
	if ( mMapTool && mMapTool == tool )
	{
		mMapTool->deactivate();
		mMapTool = nullptr;
		emit mapToolSet( nullptr, mMapTool );
		mViewport->setCursor( Qt::ArrowCursor );
	}

	if ( mLastNonZoomMapTool && mLastNonZoomMapTool == tool )
	{
		mLastNonZoomMapTool = nullptr;
	}
}

void QInteractiveManager::canvasMoveEvent(QMouseEvent *e)
{
	// call handler of current map tool
	if ( mMapTool )
	{
		mMapTool->canvasMoveEvent( e );
	}
}

void QInteractiveManager::canvasDoubleClickEvent(QMouseEvent *e)
{
	if ( mMapTool )
	{
		mMapTool->canvasDoubleClickEvent( e );
	}
}

void QInteractiveManager::canvasPressEvent(QMouseEvent *e)
{
	if ( mMapTool )
	{
		mMapTool->canvasPressEvent( e );
	}
}

void QInteractiveManager::canvasReleaseEvent(QMouseEvent *e)
{
	// call handler of current map tool
	if ( mMapTool )
	{
		// right button was pressed in zoom tool? return to previous non zoom tool
		if ( e->button() == Qt::RightButton && 
			mMapTool->flags() & QInteractiveMapTool::Transient )
		{
			qDebug()<<( QStringLiteral( "Right click in map tool zoom or pan, last tool is %1." ).arg(
				mLastNonZoomMapTool ? QStringLiteral( "not null" ) : QStringLiteral( "null" ) ) );

			// change to older non-zoom tool
			if ( mLastNonZoomMapTool
				&& ( !( mLastNonZoomMapTool->flags() & QInteractiveMapTool::EditTool )
				 ) )
			{
				QInteractiveMapTool *t = mLastNonZoomMapTool;
				mLastNonZoomMapTool = nullptr;
				setMapTool( t );
			}
			return;
		}
		
		mMapTool->canvasReleaseEvent( e );
	}
}

void QInteractiveManager::wheelEvent(QWheelEvent *e)
{	
	if ( mMapTool )
	{
		mMapTool->wheelEvent( e );
	}
}

void QInteractiveManager::keyPressEvent(QKeyEvent *e)
{
	if ( mMapTool )
	{
		mMapTool->keyPressEvent( e );
	}
}

void QInteractiveManager::keyReleaseEvent(QKeyEvent *e)
{
	if ( mMapTool )
	{
		mMapTool->keyReleaseEvent( e );
	}
}

void QInteractiveManager::mapToolDestroyed()
{
	qDebug()<< ( QStringLiteral( "maptool destroyed" ) );
	mMapTool = nullptr;
}
