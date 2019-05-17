#ifndef QGSMAPTOOL_H
#define QGSMAPTOOL_H

#include <QCursor>
#include <QString>
#include <QObject>

#include "qmapinteractive_global.h"

class QWidget;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QPoint;
class QAction;
class QAbstractButton;
class QInteractiveManager;

/**
 * \ingroup gui
 * Abstract base class for all map tools.
 * Map tools are user interactive tools for manipulating the
 * map canvas. For example map pan and zoom features are
 * implemented as map tools.
 */
class QMAPINTERACTIVE_EXPORT QInteractiveMapTool : public QObject
{
    Q_OBJECT

  public:

    /**
     * Enumeration of flags that adjust the way the map tool operates
     */
    enum Flag
    {
      Transient = 1 << 1, /*!< Indicates that this map tool performs a transient (one-off) operation.
                               If it does, the tool can be operated once and then a previous map
                               tool automatically restored. */
      EditTool = 1 << 2, //!< Map tool is an edit tool, which can only be used when layer is editable
      AllowZoomRect = 1 << 3, //!< Allow zooming by rectangle (by holding shift and dragging) while the tool is active
    };
    Q_DECLARE_FLAGS( Flags, Flag )

    /**
     * Returns the flags for the map tool.
     */
    virtual Flags flags() const { return Flags(); }

    ~QInteractiveMapTool() ;

    //! Mouse move event for overriding. Default implementation does nothing.
    virtual void canvasMoveEvent( QMouseEvent *e );

    //! Mouse double-click event for overriding. Default implementation does nothing.
    virtual void canvasDoubleClickEvent( QMouseEvent *e );

    //! Mouse press event for overriding. Default implementation does nothing.
    virtual void canvasPressEvent( QMouseEvent *e );

    //! Mouse release event for overriding. Default implementation does nothing.
    virtual void canvasReleaseEvent( QMouseEvent *e );

    //! Mouse wheel event for overriding. Default implementation does nothing.
    virtual void wheelEvent( QWheelEvent *e );

    //! Key event for overriding. Default implementation does nothing.
    virtual void keyPressEvent( QKeyEvent *e );

    //! Key event for overriding. Default implementation does nothing.
    virtual void keyReleaseEvent( QKeyEvent *e );

    /**
     * Use this to associate a QAction to this maptool. Then when the setMapTool
     * method of mapcanvas is called the action state will be set to on.
     * Usually this will cause e.g. a toolbutton to appear pressed in and
     * the previously used toolbutton to pop out. */
    void setAction( QAction *action );

    //! Returns associated action with map tool or NULL if no action is associated
    QAction *action();

    /**
     * Returns if the current map tool active on the map canvas
     */
    bool isActive() const;

    /**
     * Use this to associate a button to this maptool. It has the same meaning
     * as setAction() function except it works with a button instead of an QAction. */
    void setButton( QAbstractButton *button );

    //! Returns associated button with map tool or NULL if no button is associated
    QAbstractButton *button();

    //! Sets a user defined cursor
    virtual void setCursor( const QCursor &cursor );

    //! make action and/or button active
    virtual void activate();

    //! make action and/or button active deactivated(使无效)
    virtual void deactivate();

    //! convenient method to clean members
    virtual void clean();

    /**
     * Emit map tool changed with the old tool
     */
    QString toolName() { return mToolName; }

  signals:
    //! emit signal to clear previous message
    void messageDiscarded();

    //! signal emitted once the map tool is activated
    void activated();

    //! signal emitted once the map tool is deactivated
    void deactivated();

  private slots:
    //! clear pointer when action is destroyed
    void actionDestroyed();

  protected:

    QInteractiveMapTool(QWidget* viewport, QInteractiveManager *iManager );

	QWidget* mViewport;

	QInteractiveManager* mIManager;

    //! cursor used in map tool
    QCursor mCursor;

    /**
     * optionally map tool can have pointer to action
     * which will be used to set that action as active
     */
    QAction *mAction;

    /**
     * optionally map tool can have pointer to a button
     * which will be used to set that action as active
     */
    QAbstractButton *mButton;

    //! translated name of the map tool
    QString mToolName;

};

Q_DECLARE_OPERATORS_FOR_FLAGS( QInteractiveMapTool::Flags )

#endif
