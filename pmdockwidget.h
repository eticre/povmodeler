/* This file is part of the KDE libraries
   Copyright (C) 2000 Max Judin <novaprint@mtu-net.ru>
   Copyright (C) 2000 Falk Brettschneider <falk@kdevelop.org>
   Modified 2002 Andreas Zehender <zehender@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

/*
  activities:
  -----------
  03/2002                 : Copied from kdelibs. Slightly different docking
  ; behavior by Andreas Zehender <zehender@kde.org>
  05/2001 -               : useful patches, bugfixes by Christoph Cullmann <crossfire@babylon2k.de>,
  Joseph Wenninger <jowenn@bigfoot.com> and  Falk Brettschneider
  03/2001 - 05/2001       : maintained and enhanced by Falk Brettschneider <falk@kdevelop.org>
  03/2000                 : class documentation added by Falk Brettschneider <gigafalk@yahoo.com>
  10/1999 - 03/2000       : programmed by Max Judin <novaprint@mtu-net.ru>

  C++ classes in this file:
  -------------------------
  - PMDockWidgetAbstractHeader     - minor helper class
  - PMDockWidgetAbstractHeaderDrag - minor helper class
  - PMDockWidgetHeaderDrag         - drag panel in a dockwidget title bar
  - PMDockWidgetHeader             - dockwidget title bar containing the drag panel
  - PMDockTabGroup                 - minor helper class
  - PMDockWidget                   - IMPORTANT CLASS: the one and only dockwidget class
  - PMDockManager                  - helper class
  - PMDockMainWindow               - IMPORTANT CLASS: a special KXmlGuiWindow that can have dockwidgets
  - PMDockArea                     - like PMDockMainWindow but inherits just QWidget
*/

/*
  MODIFICATIONS (zehender)
  Added kparts ability from kparts/dockmainwindow
*/

#define _JOWENN_EXPERIMENTAL_


#ifndef KDOCKWIDGET_H
#define KDOCKWIDGET_H

#define _KDOCKWIDGET_2_2_

#include <QPoint>
#include <QList>
#include <QFrame>
#include <QtXml/QDomElement>
#include <QTabWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QMainWindow>


//#include "netwm_def.h"
#undef  EXPORT_DOCKCLASS
#define EXPORT_DOCKCLASS

class PMDockSplitter;
class PMDockManager;
class PMDockWidget;
class PMDockButton_Private;
class PMDockWidgetPrivate;
class PMDockArea;
class PMDockMainWindowPrivate;

class QVBoxLayout;
class QHBoxLayout;
class QPixmap;
class QMenu;


class PMDockMain : public QWidget
{
	Q_OBJECT
public:
	PMDockMain(QWidget* parent);
	void setTopWidget(QWidget* top);
public:
	virtual void resizeEvent(QResizeEvent *);
private:
	QWidget* m_pTop;
};

/**
 * An abstract base clase for all dockwidget headers (and member of the dockwidget class set).
 * See the class description of @ref PMDockWidgetHeader!
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class PMDockWidgetAbstractHeader : public QFrame
{
	Q_OBJECT
public:

	/**
	 * Constructs this.
	 *
	 * @param parent the parent widget (usually a dockwidget)
	 * @param name   the object instance name
	 */
	PMDockWidgetAbstractHeader( PMDockWidget* parent );

	/**
	 * Destructs this.
	 */
	virtual ~PMDockWidgetAbstractHeader(){}

	/**
	 * Provides things concerning to switching to toplevel mode. Must be overridden by an inheriting class.
	 */
	virtual void setTopLevel( bool ){}

private:
	class PMDockWidgetAbstractHeaderPrivate;
	PMDockWidgetAbstractHeaderPrivate *d;
};

/**
 * An abstract class for all dockwidget drag-panels of a dockwidgets (and member of the dockwidget class set).
 * See the class description of @ref PMDockWidgetHeaderDrag!
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class PMDockWidgetAbstractHeaderDrag : public QFrame
{
	Q_OBJECT
public:

	/**
	 * Constructs this.
	 *
	 * @param parent the parent widget (usually a dockwidget header)
	 * @param dock   the dockwidget where it belongs to
	 * @param name   the object instance name
	 */
	PMDockWidgetAbstractHeaderDrag( PMDockWidgetAbstractHeader* parent,
											  PMDockWidget* dock );

	/**
	 * Destructs this.
	 */
	virtual ~PMDockWidgetAbstractHeaderDrag(){}

	/**
	 * @return the dockwidget where this belongs to
	 */
	PMDockWidget* dockWidget() const { return dw; }

private:
	/**
	 * the dockwidget where this belongs to
	 */
	PMDockWidget* dw;
private:
	class PMDockWidgetAbstractHeaderDragPrivate;
	PMDockWidgetAbstractHeaderDragPrivate *d;
};

/**
 * This special widget is the panel one can grip with the mouses (and member of the dockwidget class set).
 * The widget for dragging, so to speak.
 * Usually it is located in the @ref PMDockWidgetHeader.
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class PMDockWidgetHeaderDrag : public PMDockWidgetAbstractHeaderDrag
{
	Q_OBJECT
public:

	/**
	 * Constructs this.
	 *
	 * @param parent the parent widget (usually a dockwidget header)
	 * @param dock   the dockwidget where it belongs to
	 * @param name   the object instance name
	 */
	PMDockWidgetHeaderDrag( PMDockWidgetAbstractHeader* parent, PMDockWidget* dock );

	/**
	 * Destructs this.
	 */
	virtual ~PMDockWidgetHeaderDrag(){}

protected:

	/**
	 * Draws the drag panel (a double line)
	 */
	virtual void paintEvent( QPaintEvent* );

private:
	class PMDockWidgetHeaderDragPrivate;
	PMDockWidgetHeaderDragPrivate *d;
};

/**
 * The header (additional bar) for a @ref PMDockWidget s (and member of the dockwidget class set).
 * It have got the buttons located there. And it is for recording and reading the button states.
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class PMDockWidgetHeader : public PMDockWidgetAbstractHeader
{
	Q_OBJECT
public:

	/**
	 * Constructs this.
	 *
	 * @param parent the parent widget (usually a dockwidget)
	 * @param name   the object instance name
	 */
	PMDockWidgetHeader( PMDockWidget* parent );

	/**
	 * Destructs this.
	 */
	virtual ~PMDockWidgetHeader(){}

	/**
	 * Hides the close button and stay button when switching to toplevel or vice versa shows them.
	 *
	 * @param t toplevel or not
	 */
	virtual void setTopLevel( bool t);

	/**
	 * Sets the drag panel of this header.
	 *
	 * @param nd A pointer to the new drag panel
	 */
	void setDragPanel( PMDockWidgetHeaderDrag* nd );

	bool dragEnabled() const;
	void setDragEnabled(bool b);

protected slots:
    /**
     * Sets dragging the dockwidget off when the stay button is pressed down and vice versa.
     */
   void slotStayClicked();

protected:

	/**
	 * A layout manager for placing the embedded buttons (close and stay)
	 */
	QHBoxLayout* layout;

	/**
	 * a little button for closing (undocking and hiding) the dockwidget
	 */
	PMDockButton_Private* closeButton;

	/**
	 * a little button for undocking the dockwidget and add it as
	 * level widget
	 */
	PMDockButton_Private* toDesktopButton;

	/**
	 * a little button for enabling/disabling dragging the dockwidget with the mouse
	 */
	PMDockButton_Private* stayButton;

	/**
	 * a little button for dock back the dockwidget to it's previous dockwidget
	 */
	PMDockButton_Private* dockbackButton;

	/**
	 * the drag panel (double line)
	 */
	PMDockWidgetHeaderDrag* drag;

private:
	class PMDockWidgetHeaderPrivate;
	PMDockWidgetHeaderPrivate *d;
};

/**
 * It just hides the special implementation of a dockwidget tab groups (and is member of the dockwidget class set).
 * An abstraction what it is currently.
 * In general it is like @ref QTabWidget but is more useful for the dockwidget class set.
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS PMDockTabGroup : public QTabWidget
{
	Q_OBJECT
public:
	/**
	 * Constructs this. It just calls the method of the base class.
	 */
    PMDockTabGroup( QWidget *parent = nullptr )
			:QTabWidget( parent ){}

	/**
	 * Destructs a PMDockTabGroup.
	 */
	virtual ~PMDockTabGroup(){}

private:
	class PMDockTabGroupPrivate;
	PMDockTabGroupPrivate *d;
};


/**
 * Floatable widget that can be dragged around with the mouse and
 * encapsulate the actual widgets (and member of the dockwidget class
 * set).
 *
 * You just grip the double-lined panel, tear it off its parent
 * widget, drag it somewhere and let it loose. Depending on the
 * position where you leave it, the dockwidget becomes a toplevel
 * window on the desktop (floating mode) or docks to a new widget
 * (dock mode). Note: A PMDockWidget can only be docked to a
 * PMDockWidget.
 *
 * If you want to use this kind of widget, your main application
 * window has to be a @ref PMDockMainWindow.  That is because it has
 * got several additional dock management features, for instance a
 * @ref PMDockManager that has an overview over all dockwidgets and and
 * a dockmovemanager (internal class) that handles the dock process.
 *
 * Usually you create an PMDockWidget that covers the actual widget in this way:
 * <PRE>
 * ...
 * PMDockMainWindow* mainWidget;
 * ...
 * PMDockWidget* dock = 0L;
 * dock = mainWidget->createDockWidget( "Any window caption", nicePixmap, 0L, tr("window caption")); // 0L==no parent
 * QWidget* actualWidget = new QWidget( dock);
 * dock->setWidget( actualWidget); // embed it
 * dock->setToolTipString(tr("That's me")); // available when appearing as tab page
 * ...
 * </PRE>
 *
 * See @ref PMDockMainWindow how a dockwidget is docked in.
 *
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS PMDockWidget: public QWidget
{
	Q_OBJECT
	friend class PMDockManager;
	friend class PMDockSplitter;
	friend class PMDockMainWindow;
	friend class PMDockArea;

public:
	/**
	 * Construct a dockwidget.
	 *
	 * Initially, docking to another and docking to this is allowed for
	 * every @p DockPosition.  It is supposed to be no (tab) group. It will
	 * taken under control of its dockmanager.
	 *
	 * @param dockManager The responsible manager (dock helper)
	 * @param pixmap      An icon (for instance shown when docked centered)
	 * @param parent      Parent widget
	 * @param strCaption  Title of the dockwidget window (shown when toplevel)
	 * @param strTabPageLabel The title of the tab page (shown when in tab page mode), if it is "", only the icon will be shown, if it is 0L, the label is set to strCaption
	 * @param f Qt::WidgetFlags widget flags
	 */
	PMDockWidget( PMDockManager* dockManager,
                      const QPixmap &pixmap, QWidget* parent = nullptr, const QString& strCaption = QString(),
                      const QString& strTabPageLabel = QLatin1String( " " ), QFlag f=0);

	/**
	 * Destructs a dockwidget.
	 */
	virtual ~PMDockWidget();


	/**
	 * The possible positions where a dockwidget can dock to another dockwidget
	 */
	enum DockPosition
	{
      DockNone   = 0,
      DockTop    = 0x0001,
      DockLeft   = 0x0002,
      DockRight  = 0x0004,
      DockBottom = 0x0008,
      DockCenter = 0x0010,
      DockDesktop= 0x0020,
      DockCorner = DockTop | DockLeft | DockRight | DockBottom,
      DockFullSite = DockCorner | DockCenter,
      DockFullDocking = DockFullSite | DockDesktop
	};

	/**
	 * This is a key method of this class! Use it to dock dockwidgets to
	 * another dockwidget at the right position within its
	 * @ref PMDockMainWindow or a toplevel dockwidget.
	 *
	 *
	 * If the target is null, it will become a toplevel dockwidget at position pos;
	 * Note: Docking to another dockwidget means exactly:
	 * A new parent dockwidget will be created, that replaces the target dockwidget and contains another single helper widget (tab widget or panner)
	 * which contains both dockwidgets, this and the target dockwidget. So consider parent<->child relationships change completely during such actions.
	 *
	 * @param  target The dockwidget to dock to
	 * @param  dockPos One of the DockPositions this is going to dock to
	 * @param  spliPos The split relation (in percent, or percent*100 in high resolution) between both dockwidgets, target and this
	 * @param  pos The dock position, mainly of interest for docking to the desktop (as toplevel dockwidget)
	 * @param  check Only for internal use;
	 * @param  tabIndex The position index of the tab widget (when in tab page mode), -1 (default) means append
	 * @return result The group dockwidget that replaces the target dockwidget and will be grandparent of target and @p this.
	 */
	PMDockWidget* manualDock( PMDockWidget* target, DockPosition dockPos, int spliPos = 50, QPoint pos = QPoint(0,0), bool check = false, int tabIndex = -1);

	/**
	 * Specify where it is either possible or impossible for this to dock to another dockwidget.
	 *
	 * @param pos An OR'ed set of @p DockPositions
	 */
	void setEnableDocking( int pos );

	/**
	 * @return Where it is either possible or impossible for this to dock to another dockwidget (an OR'ed set of DockPositions).
	 */
	int enableDocking() const { return eDocking; }

	/**
	 * Specify where it is either possible or impossible for another dockwidget to dock to this.
	 *
	 * @param pos An OR'ed set of @p DockPositions
	 */
	void setDockSite( int pos ){ sDocking = pos;}

	/**
	 * @return There it is either possible or impossible for another dockwidget to dock to this (an OR'ed set of @p DockPositions).
	 */
	int dockSite() const
	{
		// no docking if a floating dock widget
		if( !parent() )
			return 0;
		return sDocking;
	}

	/**
	 * Sets the embedded widget.
	 *
	 * A QLayout takes care about proper resizing, automatically.
	 *
	 * @param w The pointer to the dockwidget's child widget.
	 */
	void setWidget( QWidget* w);

	/**
	 * Get the embedded widget.
	 *
	 * @return The pointer to the dockwidget's child widget, 0L if there's no such child.
	 */
	QWidget* getWidget() const { return widget; }

	/**
	 * Sets the header of this dockwidget.
	 *
	 * A @ref QLayout takes care about proper resizing, automatically.
	 * The header contains the drag panel, the close button and the stay button.
	 *
	 * @param ah A base class pointer to the dockwidget header
	 */
	void setHeader( PMDockWidgetAbstractHeader* ah);

	/**
	 * Normally it simply shows the dockwidget.
	 *
	 * But additionally, if it is docked to a tab widget (@p DockCenter), it is set as the active (visible) tab page.
	 */
	void makeDockVisible();

	/**
	 * @return If it may be possible to hide this.
	 *
	 * There are reasons that it's impossible:
	 * @li It is a (tab) group.
	 * @li It is already invisible ;-)
	 * @li The parent of this is the @ref PMDockMainWindow.
	 * @li It isn't able to dock to another widget.
	 */
	bool mayBeHide() const;

	/**
	 * @return If it may be possible to show this.
	 * There are reasons that it's impossible:
	 * @li It is a (tab) group.
	 * @li It is already visible ;-)
	 * @li The parent of this is the @p PMDockMainWindow.
	 */
	bool mayBeShow() const;

	/**
	 * @return The dockmanager that is responsible for this.
	 */
	PMDockManager* dockManager() const { return manager; }

	/**
	 * Stores a string for a tooltip.
	 *
	 * That tooltip string has only a meaning when this dockwidget is shown as tab page.
	 * In this case the tooltip is shown when one holds the mouse cursor on the tab page header.
	 * Such tooltip will for instance be useful, if you use only icons there.
	 * Note: Setting an empty string switches the tooltip off.
	 *
	 * @param ttStr A string for the tooltip on the tab.
	 */
	void setToolTipString(const QString& ttStr) { toolTipStr = ttStr; }

	/**
	 * @return The tooltip string being shown on the appropriate tab page header when in dock-centered mode.
	 */
	const QString& toolTipString() const { return toolTipStr; }

	/**
	 * @return result @p true, if a dockback is possible, otherwise @p false.
	 */
	bool isDockBackPossible() const;

	/**
	 * Sets a string that is used for the label of the tab page when in tab page mode
	 * @param label The new tab page label.
	 */
	void setTabPageLabel( const QString& label) { tabPageTitle = label; }

	/**
	 * @return A string that is used for the label of the tab page when in tab page mode.
	 */
	const QString& tabPageLabel() const { return tabPageTitle; }

	/**
	 * Catches and processes some @ref QWidget events that are interesting for dockwidgets.
	 */
	virtual bool event( QEvent * );

	/**
	 * Add dockwidget management actions to @ref QWidget::show.
	 */
	virtual void show();
	/**
	 * @return the parent widget of this if it inherits class PMDockTabGroup
	 */
	PMDockTabGroup* parentDockTabGroup() const;

#ifndef NO_KDE2

	/**
	 * Sets the type of the dock window
	 *
	 * @param windowType is type of dock window
	 */
    //void setDockWindowType (NET::WindowType windowType);

#endif

	/**
	 * Sets the type of the dock window
	 *
	 * @param windowType is type of dock window
	 */
	void setDockWindowTransient (QWidget *parent, bool transientEnabled);

	// MODIFICATION (lpassos)
	/**
	 *
	 * Return the current dock position of the widget
	 */
	DockPosition getDockPosition() const { return currentDockPos; }
	/**
	 * Returns the position of the drag rectacle
	 */
	DockPosition dragRectangle() const { return m_dragRectangle; }
	/**
	 * Sets the new drag rectangle position
	 */
	void setDragRectangle(DockPosition pos);

public slots:
   /**
	 * Docks a dockwidget back to the dockwidget that was the neighbor
	 * widget before the current dock position.
	 */
   void dockBack();

	/**
	 * Toggles the visibility state of the dockwidget if it is able to be shown or to be hidden.
	 */
	void changeHideShowState();

	/**
	 * Undocks this. It means it becomes a toplevel widget framed by the system window manager.
	 * A small panel at the top of this undocked widget gives the possibility to drag it into
	 * another dockwidget by mouse (docking).
	 */
	void undock();

	// MODIFICATION (zehender)
	/**
	 * Docks the widget to the desktop (as a toplevel widget)
	 */
	void toDesktop();

	/**
	 * Sets the caption and tab label
	 */
	void slotSetCaption( const QString& );

protected:

	/**
	 * Checks some conditions and shows or hides the dockwidget header (drag panel).
	 * The header is hidden if:
	 * @li the parent widget is the PMDockMainWindow
	 * @li this is a (tab) group dockwidget
	 * @li it is not able to dock to another dockwidget
	 */
	void updateHeader();

	signals:
	/**
	 * Emitted when another dock widget is docking to this.
	 *
	 * @param dw the dockwidget that is docking to this
	 * @param dp the DockPosition where it wants to dock to
	 */
	void docking( PMDockWidget* dw, PMDockWidget::DockPosition dp);

	/**
	 * Signals that the dock default position is set.
	 */
	void setDockDefaultPos();

	/**
	 * Emitted when the close button of the panel (@ref PMDockWidgetHeader) has been clicked.
	 */
	void headerCloseButtonClicked();

	/**
	 * Emitted when the dockback button of the panel (@ref PMDockWidgetHeader) has been clicked.
	 */
	void headerDockbackButtonClicked();

	/**
	 * Emitted when the widget processes a close event.
	 */
	void iMBeingClosed();
	/**
	 * Emitted when the widget has undocked.
	 */
	void hasUndocked();

protected slots:
	/**
	 * Does several things here when it has noticed that the former brother widget (closest neighbor) gets lost.
	 * The former brother widget is needed for a possible dockback action, to speak with the Beatles:
	 * "To get back to where you once belonged" ;-)
	 */
   void loseFormerBrotherDockWidget();
	/**
	 * Called when the dock manager was deleted
	 */
	void managerDestroyed(QObject*);

protected:
	/**
	 * earlier closest neighbor widget, so it's possible to dock back to it.
	 */
	PMDockWidget* formerBrotherDockWidget;
	/**
	 * the current dock position.
	 */
	DockPosition currentDockPos;
	/**
	 * the former dock position when it really was at another position before.
	 */
	DockPosition formerDockPos;
	/**
	 * a string used as tooltip for the tab page header when in dock-centered mode.
	 */
	QString toolTipStr;
	/**
	 * a string used as title of the tab page when in tab page mode
	 */
	QString tabPageTitle;

protected:
	virtual void resizeEvent(QResizeEvent *);

private:
	/**
	 * Sets the caption (window title) of the given tab widget.
	 *
	 * @param g the group (tab) widget
	 */
	void setDockTabName( PMDockTabGroup* g);

	/**
	 * Reparent to s or set this to the PMDockMainWindow's view if s is that dockmainwindow.
	 * If s is O, simply move the widget.
	 *
	 * @param s the target widget to reparent to
	 * @param p the point to move to (if it doesn't reparent)
	 */
	void applyToWidget( QWidget* s, const QPoint& p  = QPoint(0,0) );
	/**
	 * Updates position and size of drag overlay
	 */
	void updateDragOverlay();

	/**
	 * A base class pointer to the header of this dockwidget
	 */
	PMDockWidgetAbstractHeader* header;

	/**
	 * the embedded widget
	 */
	QWidget* widget;

	/**
	 * the layout manager that takes care about proper resizing and moving the embedded widget and the header
	 */
	QVBoxLayout* layout;

	/**
	 * the responsible dockmanager
	 */
	PMDockManager* manager;

	/**
	 * an icon for the tab widget header
	 */
	QPixmap* pix;

	/**
	 * Information about the ability for docking to another dockwidget.
	 */
	int eDocking;

	/**
	 * Information which site of this dockwidget is free for docking of other dockwidgets.
	 */
	int sDocking;

	/**
	 * Previous side (left,right,top,bottom) where this dockwidget was before a dragging action, none if it wasn't dragged before.
	 */
	PMDockWidget::DockPosition prevSideDockPosBeforeDrag;

	// GROUP data
	QString firstName;
	QString lastName;
	Qt::Orientation splitterOrientation;
	bool isGroup;
	bool isTabGroup;
	DockPosition m_dragRectangle;
	QWidget* m_pDragOverlay;

private:
	PMDockWidgetPrivate *d;
};

/**
 * The manager that knows all dockwidgets and handles the dock process (and member of the dockwidget class set).
 * More or less a helper class for the PMDockWidget class set but of interest for some functionality
 * that can be called within a @ref PMDockMainWindow or a @ref PMDockWidget .
 *
 * An important feature is the ability to read or save the current state of all things concerning to
 * dockwidgets to @ref KConfig .
 *
 * The dockmanager is also often used when a certain dockwidget or a child of such dockwidget must be found.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS PMDockManager: public QObject
{
	Q_OBJECT
	friend class PMDockWidget;
	friend class PMDockMainWindow;

public:
	/**
	 * Constructs a dockmanager. Some initialization happen:
	 * @li It installs an event filter for the main window,
	 * @li a control list for dock objects
	 * @li a control list for menu items concerning to menus provided by the dockmanager
	 * @li Some state variables are set
	 *
	 * @param mainWindow the main window controlled by this
	 * @param name the internal QOject name
	 */
	PMDockManager( QWidget* mainWindow );

	/**
	 * Destructs a dockmanager.
	 */
	virtual ~PMDockManager();

	/**
	 * Saves the current dock window layout into a DOM tree below the given element.
	 */
	void writeConfig(QDomElement &base);
	/**
	 * Reads the current dock window layout from a DOM tree below the given element.
	 */
	void readConfig(QDomElement &base);
	/**
	 * Shows all encapsulated widgets of all controlled dockwidgets and shows all dockwidgets which are
	 * parent of a dockwidget tab group.
	 */
	void activate();

	/**
	 * It's more or less a method that catches several events which are interesting for the dockmanager.
	 * Mainly mouse events during the drag process of a dockwidgets are of interest here.
	 *
	 * @param _ the object that sends the event
	 * @param _ the event
	 * @return the return value of the method call of the base class method
	 */
	virtual bool eventFilter( QObject *, QEvent * );

	/**
	 * This method finds out what a widgets' dockwidget is. That means the dockmanager has a look at all
	 * dockwidgets it knows and tells you when one of those dockwidgets covers the given widget.
	 *
	 * @param w any widget that is supposed to be encapsulated by one of the controlled dockwidgets
	 * @return the dockwidget that encapsulates that widget, otherwise 0
	 */
	PMDockWidget* findWidgetParentDock( QWidget* w) const;

	/**
	 * Works like makeDockVisible() but can be called for widgets that covered by a dockwidget.
	 *
	 * @param w the widget that is encapsulated by a dockwidget that turns to visible.
	 */
	void makeWidgetDockVisible( QWidget* w ){ findWidgetParentDock(w)->makeDockVisible(); }

	/**
	 * @return the popupmenu for showing/hiding dockwidgets
	 */
    QMenu* dockHideShowMenu() const { return menu; }

	/**
	 * @param dockName an internal QObject name
	 * @return the dockwidget that has got that internal QObject name
	 */
	PMDockWidget* getDockWidgetFromName( const QString& dockName );

	/**
	 * Enables opaque resizing. Opaque resizing is initially turned off.
	 * Call this method before you create any dock widgets!
	 */
	void setSplitterOpaqueResize(bool b=true);

	/**
	 * Returns TRUE if opaque resizing is enabled, FALSE otherwise.
	 */
	bool splitterOpaqueResize() const;

	/**
	 * Try to preserve the widget's size. Works like KeepSize resize mode
	 * of QSplitter. Off by default.
	 * Call this method before you create any dock widgets!
	 */
	void setSplitterKeepSize(bool b=true);

	/**
	 * Returns TRUE if the KeepSize is enabled, FALSE otherwise.
	 */
	bool splitterKeepSize() const;

	/**
	 * Operate the splitter with a higher resolution. Off by default.
	 * Call this method before you create any dock widgets!
	 * If high resolution is used all splitter position parameters
	 * are percent*100 instead of percent.
	 */
	void setSplitterHighResolution(bool b=true);

	/**
	 * Returns TRUE if the splitter uses the high resolution, FALSE otherwise.
	 */
	bool splitterHighResolution() const;

	/**
	 * Returns the main dock widget
	 */
	QWidget* dockMainWidget() const { return main; }

	signals:

	/**
	 * Signals changes of the docking state of a dockwidget. Usually the dock-toolbar will be updated then.
	 */
	void change();

	/**
	 * Signals a dockwidget is replaced with another one.
	 */
	void replaceDock( PMDockWidget* oldDock, PMDockWidget* newDock );

	/**
	 * Signals a dockwidget without parent (toplevel) is shown.
	 */
	void setDockDefaultPos( PMDockWidget* );

private slots:

   /**
	 * Clears the popupmenu for showing/hiding dockwidgets and fills it with the current states of all controlled dockwidgets.
	 */
   void slotMenuPopup();

	/**
	 * This method assumes a menuitem of the popupmenu for showing/hiding dockwidgets is selected and toggles that state.
	 *
	 * @param id the popupmenu id of the selected menuitem
	 */
    void slotMenuActivated( QAction *act );

private:

	/**
	 * A data structure containing data about every dockwidget that is under control.
	 */
	struct MenuDockData
	{
		MenuDockData( PMDockWidget* _dock, bool _hide )
		{
			dock = _dock;
			hide = _hide;
		}
		~MenuDockData(){}

		PMDockWidget* dock;
		bool hide;
	};

	/**
	 * Finds the PMDockWidget at the position given as parameter
	 *
	 * @param pos global (desktop) position of the wanted dockwidget
	 * @return the dockwidget at that position
	 */
	PMDockWidget* findDockWidgetAt( const QPoint& pos );

	/**
	 * Finds the QWidget recursively at the position given as parameter
	 *
	 * @param w a variable where the method puts the QWidget at that position (instead of a return value)
	 * @param p the parent widget where the recursive search should start from
	 * @param pos global (desktop) position of the wanted dockwidget
	 */
	void findChildDockWidget( QWidget*& w, const QWidget* p, const QPoint& pos );
	// MODIFICATION (lpassos)
	// Made findChildDockWidget public
public:
	/**
	 * Finds all dockwidgets which are child, grandchild and so on of p.
	 *
	 * @param p the parent widget where the recursive search starts from
	 * @param l the widget list that contains the search result after the return of this method
	 */
	void findChildDockWidget( const QWidget* p, QWidgetList*& l);
	/**
	 * Returns all floating dock widgets
	 */
	QList<PMDockWidget*> findFloatingWidgets();

private:
	/**
	 * Sets a dockwidget in drag mode.
	 */
	void startDrag( PMDockWidget* );

	/**
	 * Moves a dockwidget that is in drag mode.
	 *
	 * @param d the dockwidget which is dragged
	 * @param pos the new position of the dragged dockwidget
	 */
	void dragMove( PMDockWidget* d, QPoint pos );

	/**
	 * Aborts the drag mode. Restores the cursor and hides the drag indicator.
	 */
	void cancelDrop();

	/**
	 * Finishes the drag mode. If the user let it drop on an other dockwidget, it will possibly be docked (if allowed),
	 * if the user drops it outside of the application window it becomes toplevel.
	 */
	void drop();

	// class members

	/**
	 * Usually the PMDockMainWindow but not necessarily.
	 */
	QWidget* main;

	/**
	 * The dockwidget that is being dragged at the moment
	 */
	PMDockWidget* currentDragWidget;

	/**
	 * The target dockwidget where the currentDragWidget is dropped
	 */
	PMDockWidget* currentMoveWidget; // widget where mouse moving

	/**
	 * It is of interest during the dock process. Then it contains all child dockwidgets.
	 */
	QWidgetList* childDockWidgetList;

	/**
	 * The dockposition where the dockwidget would be docked to, if we dropped it here.
	 */
	PMDockWidget::DockPosition curPos;

	/**
	 * A QList of all objects that are important for docking.
	 * Some serve as group widgets of dockwidgets, others encapsulate normal widgets.
	 */
	QObjectList* childDock;

	/**
	 * Contains dockwidgets that are created automatically by the dockmanager. For internal use.
	 */
	QObjectList* autoCreateDock;

	/**
	 * For storing the width during the dragging of a dockwidget.
	 */
	int storeW;

	/**
	 *  For storing the height during the dragging of a dockwidget.
	 */
	int storeH;

	/**
	 * State variable if there is a drag process active.
	 */
	bool draging;

	/**
	 * State variable if there is an undock process active
	 */
	bool undockProcess;

	/**
	 * The dockmanager sets it to true if the user cancels the drag by moving the cursor
	 * on a invalid drop place
	 */
	bool dropCancel;

	/**
	 * A popup menu that contains one menuitem for each dockwidget that shows the current visibility state and
	 * to show or hide the appropriate dockwidget.
	 */
    QMenu* menu;

	/**
	 * An internal list containing data for the menuitems for the visibility popup menu.
	 */
	QList<MenuDockData> menuData;

private:
	class PMDockManagerPrivate;
	PMDockManagerPrivate *d;
};

/**
 * A special kind of @ref KXmlGuiWindow that is able to have dockwidget child widgets (and member of the dockwidget class set).
 *
 * The main widget should be a @ref PMDockWidget where other @ref PMDockWidget can be docked to
 * the left, right, top, bottom or to the middle.
 * Note: dock to the middle means to drop on a dockwidget and to unite them to a new widget, a tab control.
 *
 * Furthermore, the PMDockMainWindow has got the @ref PMDockManager and some data about the dock states.
 *
 * If you've got some dockwidgets, you can dock them to the dockmainwindow to initialize a start scene:
 * Here an example:
 * <PRE>
 * DockApplication::DockApplication( const char* name) : PMDockMainWindow( name)
 * {
 *   ...
 *   PMDockWidget* mainDock;
 *   mainDock = createDockWidget( "Falk's MainDockWidget", mainPixmap, 0L, "main_dock_widget");
 *   AnyContentsWidget* cw = new AnyContentsWidget( mainDock);
 *   mainDock->setWidget( cw);
 *   // allow others to dock to the 4 sides
 *   mainDock->setDockSite(PMDockWidget::DockCorner);
 *   // forbit docking abilities of mainDock itself
 *   mainDock->setEnableDocking(PMDockWidget::DockNone);
 *   setView( mainDock); // central widget in a KDE mainwindow
 *   setMainDockWidget( mainDock); // master dockwidget
 *   ...
 *   PMDockWidget* dockLeft;
 *   dockLeft = createDockWidget( "Intially left one", anyOtherPixmap, 0L, tr("The left dockwidget"));
 *   AnotherWidget* aw = new AnotherWidget( dockLeft);
 *   dockLeft->setWidget( aw);
 *   dockLeft->manualDock( mainDock,              // dock target
 *                         PMDockWidget::DockLeft, // dock site
 *                         20 );                  // relation target/this (in percent)
 *   ...
 * </PRE>
 *
 * Docking is fully dynamical at runtime. That means you can always move dockwidgets via drag and drop.
 *
 * And last but not least you can use the popupmenu for showing or hiding any controlled dockwidget
 * of this class and insert it to your main menu bar or anywhere else.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
//class EXPORT_DOCKCLASS PMDockMainWindow : public KParts::MainWindow
class EXPORT_DOCKCLASS PMDockMainWindow : public QMainWindow
{
	Q_OBJECT

	friend class PMDockManager;

public:

	/**
	 * Constructs a dockmainwindow. It calls its base class constructor and does additional things concerning
	 * to the dock stuff:
	 * @li information about the dock state of this' children gets initialized
	 * @li a dockmanager is created...
	 * @li ...and gets initialized
	 * @li the main dockwidget is set to 0
	 *
	 * @param name object name
	 */
    PMDockMainWindow( QWidget* parent = nullptr, QFlag f=0 );

    //PMDockMainWindow( QWidget* parent = 0L, Qt::WindowFlags f=KDE_DEFAULT_WINDOWFLAGS );


	/**
	 * Destructs a dockmainwindow.
	 */
	virtual ~PMDockMainWindow();

	/**
	 * Returns the dockmanager of this. (see @ref PMDockManager)
	 * @return pointer to the wanted dockmanager
	 */
	PMDockManager* manager() const { return dockManager; }

	/**
	 * Sets a new main dockwidget.
	 * Additionally, the toolbar is re-initialized.
	 *
	 * @param _ dockwidget that become the new main dockwidget
	 */
	void setMainDockWidget( PMDockWidget* );

	/**
	 * Returns the main dockwidget.
	 *
	 * @return pointer to the main dockwidget
	 */
	PMDockWidget* getMainDockWidget() const { return mainDockWidget; }

	/**
	 * This is one of the most important methods!
	 * The PMDockMainWindow creates a new dockwidget object here that usually should encapsulate the user's widget.
	 * The new dockwidget is automatically taken under control by the dockmanager of the dockmainwindow.
	 *
	 * @param name   QObject name (default dockwidget caption)
	 * @param pixmap window icon (for instance shown when docked as tabwidget entry)
	 * @param parent parent widget for the new dockwidget
	 * @param strCaption  window title (shown when toplevel)
	 * @param strTabPageLabel title of the tab page (visible when in tab page mode), if it is "", only the icon will be shown; if it is 0L, the label is set to strCaption
	 * @return    a pointer to the new created dockwidget
	 */
    PMDockWidget* createDockWidget( const QPixmap &pixmap, QWidget* parent = nullptr,
											  const QString& strCaption = QString(), const QString& strTabPageLabel = QLatin1String( " " ) );

	/**
	 * Saves the current dock window layout into a DOM tree below the given element.
	 */
	void writeDockConfig(QDomElement &base);
	/**
	 * Reads the current dock window layout from a DOM tree below the given element.
	 */
	void readDockConfig(QDomElement &base);

	/**
	 * It runs through all dockwidgets which are under control of the dockmanager and calls show() for every
	 * encapsulated widget and show() for the dockwidget itself if it is not in tab mode.
	 * Additionally, if the main dockwidget is not a QDialog, it will be shown.
	 */
	void activateDock(){ dockManager->activate(); }

	/**
	 * Returns a popup menu that contains entries for all controlled dockwidgets making hiding and showing
	 * them possible.
	 *
	 * @return the wanted popup menu
	 */
    QMenu* dockHideShowMenu() const { return dockManager->dockHideShowMenu(); }

	/**
	 * This method shows the given dockwidget.
	 * The clue is that it also considers the dockwidget could be a tab page
	 * and must set to be the activate one.
	 *
	 * @param dock the dockwidget that is to be shown
	 */
	void makeDockVisible( PMDockWidget* dock );

	/**
	 * This method hides the given dockwidget.
	 *
	 * @param dock the dockwidget that is to be shown
	 */
	void makeDockInvisible( PMDockWidget* dock );

	/**
	 * This is an overloaded member function, provided for convenience.
	 * It differs from the above function only in what argument(s) it accepts.
	 */
	void makeWidgetDockVisible( QWidget* widget );

	/**
	 * This method calls the base class method.
	 * If the given widget inherits PMDockWidget, applyToWidget(this) is called.
	 *
	 * @param _ any widget that should become the main view
	 */
	void setView( QWidget* );

	signals:
	/**
	 * Signals a certain dockwidget is undocked now.
	 */
	void dockWidgetHasUndocked(PMDockWidget*);

protected:

	/**
	 * A pointer to the main dockwidget (where one can manualDock() to
	 */
	PMDockWidget* mainDockWidget;

	/**
	 * A pointer to the manager for the dock process
	 */
	PMDockManager* dockManager;

protected slots:
   /**
	 * Called whenever one of the dockwidgets of this has been undocked.
	 */
   void slotDockWidgetUndocked();

	// kparts/dockmainwindow stuff
protected slots:
   /**
	 * Called when the active part wants to change the statusbar message
	 * Reimplement if your dock-mainwindow has a complex statusbar
	 * (with several items)
	 */
	virtual void slotSetStatusBarText( const QString & );

private:
	PMDockMainWindowPrivate *d;
	PMDockMain* m_pMain;
};

#ifdef _JOWENN_EXPERIMENTAL_
/* Joseph Wenninger jowenn@kde.org Experimental (Just all KXmlGuiWindow references changed to QWidget, otherwise nearly exactly the
   same as PMDockMainWindow*/

class EXPORT_DOCKCLASS PMDockArea : public QWidget
{
	Q_OBJECT

	friend class PMDockManager;

public:


	PMDockArea( QWidget* parent = 0L);

	virtual ~PMDockArea();

	PMDockManager* manager(){ return dockManager; }


	void setMainDockWidget( PMDockWidget* );
	PMDockWidget* getMainDockWidget(){ return mainDockWidget; }

	PMDockWidget* createDockWidget( const QPixmap &pixmap, QWidget* parent = 0L,
											  const QString& strCaption = QString(), const QString& strTabPageLabel = QLatin1String( " " ) );

	void writeDockConfig(QDomElement &base);
	void readDockConfig(QDomElement &base);

	void activateDock(){ dockManager->activate(); }
    QMenu* dockHideShowMenu(){ return dockManager->dockHideShowMenu(); }
	void makeDockVisible( PMDockWidget* dock );
	void makeDockInvisible( PMDockWidget* dock );
	void makeWidgetDockVisible( QWidget* widget );

	signals:
	/**
	 * Signals a certain dockwidget is undocked now.
	 */
	void dockWidgetHasUndocked(PMDockWidget*);

protected:

	PMDockWidget* mainDockWidget;
	PMDockManager* dockManager;

protected slots:
   void slotDockWidgetUndocked();

protected:
	virtual void resizeEvent(QResizeEvent *);

private:
	class PMDockMainWindowPrivate;
	PMDockMainWindowPrivate *d;
};

#endif

#endif


