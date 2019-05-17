#ifndef QINTERACTIVEMANAGER_H__
#define QINTERACTIVEMANAGER_H__

#include <QObject>
#include "qmapinteractive_global.h"

class QWidget;
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class QInteractiveMapTool;

class QMAPINTERACTIVE_EXPORT QInteractiveManager : public QObject
{
	Q_OBJECT
public:
	QInteractiveManager(QWidget* viewport=0);
	~QInteractiveManager(void);

	//! Sets the map tool currently being used on the canvas
	void setMapTool( QInteractiveMapTool *tool, bool clean = false );

	QInteractiveMapTool* mapTool();

	void unsetMapTool( QInteractiveMapTool *tool );
public:

	void canvasMoveEvent( QMouseEvent *e );

	void canvasDoubleClickEvent( QMouseEvent *e );

	void canvasPressEvent( QMouseEvent *e );

	void canvasReleaseEvent( QMouseEvent *e );

	void wheelEvent( QWheelEvent *e );

	void keyPressEvent( QKeyEvent *e );

	void keyReleaseEvent( QKeyEvent *e );
  signals:
	   /**
     * Emit map tool changed with the old tool
     */
   void mapToolSet( QInteractiveMapTool *newTool, QInteractiveMapTool *oldTool );
private slots:
		//! called when current maptool is destroyed
	void mapToolDestroyed();
private:
	QWidget* mViewport;
	//! pointer to current map tool
	QInteractiveMapTool *mMapTool;
	//! previous tool if current is for zooming/panning
	QInteractiveMapTool *mLastNonZoomMapTool;
};

#endif // QINTERACTIVEMANAGER_H__
