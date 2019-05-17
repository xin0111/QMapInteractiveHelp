#include "qinteractivemaptool.h"
#include "qinteractivemanager.h"

#include <QWidget>
#include <QDebug>
#include <QAction>
#include <QAbstractButton>
#include <QWheelEvent>


QInteractiveMapTool::QInteractiveMapTool( 
	QWidget* viewport,
	QInteractiveManager *iManager )
  : QObject(viewport),
  mViewport(viewport)
  , mIManager( iManager )
  , mCursor( Qt::CrossCursor ),
   mAction(nullptr),
   mButton(nullptr)
{
}


QInteractiveMapTool::~QInteractiveMapTool()
{
  mIManager->unsetMapTool( this );
}

void QInteractiveMapTool::activate()
{
  // make action and/or button active
  if ( mAction )
    mAction->setChecked( true );
  if ( mButton )
    mButton->setChecked( true );

  // set cursor (map tools usually set it in constructor)
  mViewport->setCursor( mCursor );
  qDebug() <<( QStringLiteral( "Cursor has been set" ) );

  emit activated();
}


void QInteractiveMapTool::deactivate()
{
  if ( mAction )
    mAction->setChecked( false );
  if ( mButton )
    mButton->setChecked( false );

  emit deactivated();
}

void QInteractiveMapTool::clean()
{

}

void QInteractiveMapTool::setAction( QAction *action )
{
  if ( mAction )
    disconnect( mAction, &QObject::destroyed, this,
	&QInteractiveMapTool::actionDestroyed );
  mAction = action;
  if ( mAction )
    connect( mAction, &QObject::destroyed, this, 
	&QInteractiveMapTool::actionDestroyed );
}

void QInteractiveMapTool::actionDestroyed()
{
  if ( mAction == sender() )
    mAction = nullptr;
}

QAction *QInteractiveMapTool::action()
{
  return mAction;
}

bool QInteractiveMapTool::isActive() const
{
  return mViewport && mIManager->mapTool() == this;
}

void QInteractiveMapTool::setButton( QAbstractButton *button )
{
  mButton = button;
}

QAbstractButton *QInteractiveMapTool::button()
{
  return mButton;
}

void QInteractiveMapTool::setCursor( const QCursor &cursor )
{
  mCursor = cursor;
  if ( isActive() )
    mViewport->setCursor( mCursor );
}


void QInteractiveMapTool::canvasMoveEvent( QMouseEvent *e )
{
  Q_UNUSED( e );
}

void QInteractiveMapTool::canvasDoubleClickEvent( QMouseEvent *e )
{
  Q_UNUSED( e );
}

void QInteractiveMapTool::canvasPressEvent( QMouseEvent *e )
{
  Q_UNUSED( e );
}

void QInteractiveMapTool::canvasReleaseEvent( QMouseEvent *e )
{
  Q_UNUSED( e );
}

void QInteractiveMapTool::wheelEvent( QWheelEvent *e )
{
  e->ignore();
}

void QInteractiveMapTool::keyPressEvent( QKeyEvent *e )
{
  Q_UNUSED( e );
}

void QInteractiveMapTool::keyReleaseEvent( QKeyEvent *e )
{
  Q_UNUSED( e );
}

