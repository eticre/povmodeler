/* This file is part of the KDE libraries
   Copyright (C) 2000 Max Judin <novaprint@mtu-net.ru>
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
#include "pmdockwidget.h"
#include "pmdockwidget_private.h"

#include <QApplication>
#include <QLayout>
#include <QPainter>
#include <QObject>
#include <QCursor>
#include <QTabWidget>
#include <QStyle>
#include <QStyleOption>
#include <QPixmap>
#include <QPaintEvent>
#include <QChildEvent>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QMouseEvent>

#include <QMessageBox>
#ifdef Q_WS_X11
#include <QX11Info>
#endif

#include <QMenu>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/X.h>
#include <X11/Xlib.h>
#endif

#include "pmdebug.h"

#include <QStatusBar>

#include <qapplication.h>
#include "assert.h"

#define DOCK_CONFIG_VERSION "0.0.5"

using namespace Qt;

static const char* const close_xpm[]={
	"7 7 2 1",
	"# c black",
	". c None",
	".......",
	"##..##.",
	".####..",
	"..##...",
	".####..",
	"##..##.",
	"......."};

static const char* const todesktop_xpm[]={
	"5 5 2 1",
	"# c black",
	". c None",
	"####.",
	"##...",
	"#.#..",
	"#..#.",
	"....#"};

static const char* const dockback_xpm[]={
	"5 5 2 1",
	"# c black",
	". c None",
	"#....",
	".#..#",
	"..#.#",
	"...##",
	".####"};

static const char* const not_close_xpm[]={
	"5 5 2 1",
	"# c black",
	". c None",
	"#####",
	"#...#",
	"#...#",
	"#...#",
	"#####"};

PMDockMain::PMDockMain(QWidget* parent)
		: QWidget(parent)
{
    m_pTop = nullptr;
}

void PMDockMain::setTopWidget(QWidget* top)
{
	m_pTop = top;
	if(m_pTop){
		if(m_pTop->parent() != this) m_pTop->setParent(this);
		m_pTop->setGeometry(QRect(QPoint(0,0),size()));
	}
}

void PMDockMain::resizeEvent(QResizeEvent *rsize)
{
	QWidget::resizeEvent(rsize);
	if(m_pTop) m_pTop->setGeometry(QRect(QPoint(0,0),size()));
}

class PMDockMainWindowPrivate
{
public:
	PMDockMainWindowPrivate()
	{
        //m_activePart = 0;
		m_bShellGUIActivated = false;
//		m_helpMenu = 0;
	}
	~PMDockMainWindowPrivate()
	{
	}

    //Part* m_activePart;
	bool m_bShellGUIActivated;
//	KHelpMenu *m_helpMenu;
};


/**
 * A special kind of KXmlGuiWindow that is able to have dockwidget child widgets.
 *
 * The main widget should be a dockwidget where other dockwidgets can be docked to
 * the left, right, top, bottom or to the middle.
 * Furthermore, the PMDockMainWindow has got the KDocManager and some data about the dock states.
 *
 * @author Max Judin.
 */
PMDockMainWindow::PMDockMainWindow( QWidget* parent, Qt::WindowFlags f )//pmshell start program
        :QMainWindow( parent, f )
{
	dockManager = new PMDockManager( this );
    mainDockWidget = nullptr;

    d = new PMDockMainWindowPrivate();
    //PartBase::setPartObject( this );

    m_pMain = new PMDockMain( this );
    QMainWindow::setCentralWidget( m_pMain ) ;
}

PMDockMainWindow::~PMDockMainWindow()
{
    qDebug() << "dockmainwindow desctructor";
   delete m_pMain;
   m_pMain = nullptr;
   delete dockManager;
   delete d;
}

void PMDockMainWindow::slotSetStatusBarText( const QString & text )
{
	statusBar()->showMessage( text );
}

void PMDockMainWindow::setMainDockWidget( PMDockWidget* mdw )
{
	if ( mainDockWidget == mdw ) return;
	mainDockWidget = mdw;
}

void PMDockMainWindow::setView( QWidget *view )
{
	//if ( view->isA("PMDockWidget") ){
	//	if ( view->parent() != this ) ((PMDockWidget*)view)->applyToWidget( this );
	//}
	m_pMain->setTopWidget(view);
}

PMDockWidget* PMDockMainWindow::createDockWidget( const QPixmap &pixmap, QWidget* parent, const QString& strCaption, const QString& strTabPageLabel)
{
	return new PMDockWidget( dockManager, pixmap, parent, strCaption, strTabPageLabel );
}

void PMDockMainWindow::makeDockVisible( PMDockWidget* dock )
{
    if ( dock != nullptr)
		dock->makeDockVisible();
}

void PMDockMainWindow::makeDockInvisible( PMDockWidget* dock )
{
    if ( dock != nullptr)
		dock->undock();
}

void PMDockMainWindow::makeWidgetDockVisible( QWidget* widget )
{
	makeDockVisible( dockManager->findWidgetParentDock(widget) );
}

void PMDockMainWindow::writeDockConfig(QDomElement &base)
{
	dockManager->writeConfig(base);
}

void PMDockMainWindow::readDockConfig(QDomElement &base)
{
	dockManager->readConfig(base);
}

void PMDockMainWindow::slotDockWidgetUndocked()
{
    QObject* pSender = static_cast<QObject*>(sender());
	if (!pSender->inherits("PMDockWidget")) return;
    PMDockWidget* pDW = static_cast<PMDockWidget*>(pSender);
	emit dockWidgetHasUndocked( pDW);
}

/*************************************************************************/
PMDockWidgetAbstractHeaderDrag::PMDockWidgetAbstractHeaderDrag( PMDockWidgetAbstractHeader* parent, PMDockWidget* dock )
		:QFrame( parent )
{
	dw = dock;
	installEventFilter( dock->dockManager() );
}
/*************************************************************************/
PMDockWidgetHeaderDrag::PMDockWidgetHeaderDrag( PMDockWidgetAbstractHeader* parent, PMDockWidget* dock )
		:PMDockWidgetAbstractHeaderDrag( parent, dock )
{
}

void PMDockWidgetHeaderDrag::paintEvent( QPaintEvent* )
{
	QPainter paint;

	paint.begin( this );
	QStyleOption qso;
	qso.init(this);
	qso.rect = QRect( 0, 0, width(), height() );
    style()->drawPrimitive( QStyle::PE_FrameDockWidget, &qso, &paint, this );
	paint.end();
}
/*************************************************************************/
PMDockWidgetAbstractHeader::PMDockWidgetAbstractHeader( PMDockWidget* parent )
		:QFrame( parent )
{
}
/*************************************************************************/
PMDockWidgetHeader::PMDockWidgetHeader( PMDockWidget* parent )
		:PMDockWidgetAbstractHeader( parent )
{
	layout = new QHBoxLayout( this );
	layout->setSpacing( 0 );
	layout->setMargin( 0 );

	drag = new PMDockWidgetHeaderDrag( this, parent );

	closeButton = new PMDockButton_Private( this );
	closeButton->setObjectName("DockCloseButton");
    closeButton->setIcon( QIcon( QPixmap( close_xpm ) ) );
	int buttonWidth = 9, buttonHeight = 9;
	closeButton->setFixedSize(buttonWidth,buttonHeight);
	connect( closeButton, SIGNAL(clicked()), parent, SIGNAL(headerCloseButtonClicked()));
	// MODIFICATION (zehender)
	// The shell will delete the widget
	// undock is unnecessary
	// connect( closeButton, SIGNAL(clicked()), parent, SLOT(undock()));

	stayButton = new PMDockButton_Private( this );
	stayButton->setObjectName("DockStayButton");
	stayButton->setCheckable( true );
    stayButton->setIcon( QIcon( QPixmap( not_close_xpm ) ) ) ;
	stayButton->setFixedSize(buttonWidth,buttonHeight);
	connect( stayButton, SIGNAL(clicked()), this, SLOT(slotStayClicked()));
	stayButton->hide();

	dockbackButton = new PMDockButton_Private( this );
	dockbackButton->setObjectName("DockbackButton");
    dockbackButton->setIcon( QIcon( QPixmap( dockback_xpm ) ) );
	dockbackButton->setFixedSize(buttonWidth,buttonHeight);
	connect( dockbackButton, SIGNAL(clicked()), parent, SIGNAL(headerDockbackButtonClicked()));
	connect( dockbackButton, SIGNAL(clicked()), parent, SLOT(dockBack()));

	// MODIFICATION (zehender)
	// Add a button to undock the widget and dock it as top level
	// widget to the desktop
	toDesktopButton = new PMDockButton_Private( this );
	toDesktopButton->setObjectName("ToDesktopButton");
    toDesktopButton->setIcon( QIcon( QPixmap( todesktop_xpm ) ) );
	toDesktopButton->setFixedSize(buttonWidth,buttonHeight);
	connect( toDesktopButton, SIGNAL(clicked()), parent, SLOT(toDesktop()));

	layout->addWidget( drag );
	layout->addWidget( dockbackButton );
	layout->addWidget( toDesktopButton );
	layout->addWidget( stayButton );
	layout->addWidget( closeButton );
	layout->activate();
    drag->setFixedHeight( layout->minimumSize().height() );
}

void PMDockWidgetHeader::setTopLevel( bool isTopLevel )
{
	if ( isTopLevel ){
        PMDockWidget* par = static_cast<PMDockWidget*>(parent());
		if( par) {
			if( par->isDockBackPossible())
				dockbackButton->show();
			else
				dockbackButton->hide();
		}
		stayButton->hide();
		closeButton->hide();
		toDesktopButton->hide();
		drag->setEnabled( true );
	} else {
		dockbackButton->hide();
		stayButton->hide();
		closeButton->show();
		toDesktopButton->show();
	}
	layout->activate();
	updateGeometry();
}

void PMDockWidgetHeader::setDragPanel( PMDockWidgetHeaderDrag* nd )
{
	if ( !nd ) return;

	delete layout;
	layout = new QHBoxLayout( this );
	layout->setSizeConstraint( QLayout::SetMinimumSize );

	delete drag;
	drag = nd;

	layout->addWidget( drag );
	layout->addWidget( dockbackButton );
	layout->addWidget( toDesktopButton );
	layout->addWidget( stayButton );
	layout->addWidget( closeButton );
	layout->activate();
	drag->setFixedHeight( layout->minimumSize().height() );
}

void PMDockWidgetHeader::slotStayClicked()
{
	setDragEnabled( !stayButton->isChecked() );
}

bool PMDockWidgetHeader::dragEnabled() const
{
	return drag->isEnabled();
}

void PMDockWidgetHeader::setDragEnabled(bool b)
{
	stayButton->setChecked( !b );
	closeButton->setEnabled(b);
	drag->setEnabled(b);
}


/*************************************************************************/
PMDockWidget::PMDockWidget(PMDockManager* dockManager, const QPixmap &pixmap, QWidget* parent, const QString& strCaption, const QString& strTabPageLabel, WindowFlags f)
		: QWidget( parent, f )
        ,formerBrotherDockWidget(nullptr)
		,currentDockPos(DockNone)
		,formerDockPos(DockNone)
		,pix(new QPixmap(pixmap))
		,prevSideDockPosBeforeDrag(DockNone)
		,m_dragRectangle(PMDockWidget::DockNone)
{
	d = new PMDockWidgetPrivate();  // create private data

	d->_parent = parent;

	layout = new QVBoxLayout( this );
	layout->setSpacing( 0 );
	layout->setMargin( 0 );

	manager = dockManager;
	manager->childDock->append( this );
	installEventFilter( manager );
	connect(manager, SIGNAL(destroyed(QObject*)), SLOT(managerDestroyed(QObject*)));

    header = nullptr;
	setHeader( new PMDockWidgetHeader( this ) );
	setWindowTitle( strCaption );

	if( strTabPageLabel == " ")
		setTabPageLabel( windowTitle() );
	else
		setTabPageLabel( strTabPageLabel);

	eDocking = DockFullDocking;
	sDocking = DockFullSite;

	isGroup = false;
	isTabGroup = false;

	setWindowIcon( pixmap );
    widget = nullptr;

	// use styled frame with transparent hightlight color as background
	QFrame* frame = new QFrame(this);
	frame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
	frame->setBackgroundRole(QPalette::Highlight);
	frame->setAutoFillBackground(true);
	QPalette pal = frame->palette();
	QColor c = pal.color(QPalette::Highlight);
    c.setAlpha(80);
	pal.setColor(QPalette::Highlight, c);
	frame->setPalette(pal);
	frame->hide();
	m_pDragOverlay = frame;

    QObject::connect( this, SIGNAL( hasUndocked() ), manager->main, SLOT( slotDockWidgetUndocked() ) );
    applyToWidget( parent, QPoint( 0, 0 ) );
}

void PMDockWidget::slotSetCaption( const QString& str )
{
   setTabPageLabel( str );
   setWindowTitle( str );
}

PMDockWidget::~PMDockWidget()
{
	if(manager){
		if ( !manager->undockProcess ){
			d->blockHasUndockedSignal = true;
			undock();
			d->blockHasUndockedSignal = false;
		}
	}
	emit iMBeingClosed();
	if(manager) manager->childDock->removeAll( this );
	delete pix;
	delete d; // destroy private data
}

void PMDockWidget::setHeader( PMDockWidgetAbstractHeader* h )
{
	if ( !h ) return;

	if ( header ){
		delete header;
		delete layout;
		header = h;
		layout = new QVBoxLayout( this );
		layout->setSpacing( 0 );
		layout->setMargin( 0 );
		layout->addWidget( header );
		setWidget( widget );
	} else {
		header = h;
		layout->addWidget( header );
	}
}

void PMDockWidget::setEnableDocking( int pos )
{
	eDocking = pos;
	updateHeader();
}

void PMDockWidget::updateHeader()
{
	if(!manager) return;
	if ( parent() ){
		if ( (parent() == manager->main) || isGroup || (eDocking == PMDockWidget::DockNone) ){
			header->hide();
		} else {
			header->setTopLevel( false );
			header->show();
		}
	} else {
		header->setTopLevel( true );
		header->show();
	}
}

void PMDockWidget::applyToWidget( QWidget* s, const QPoint& p )
{
	if(!manager) return;
	if ( parent() != s )
	{
		hide();
		setParent( s );
	}

	if ( s && s->inherits("PMDockMain") ){
		((PMDockMain*)s)->setTopWidget( this );
	}

	if ( s == manager->main ){
      setGeometry( QRect(QPoint(0,0), manager->main->geometry().size()) );
	}

	if ( !s )
	{
		move(p);

#ifdef Q_WS_X11
		if (d->transient && d->_parent)
			XSetTransientForHint( QX11Info::display(), winId(), d->_parent->winId() );

		KWindowSystem::setType( winId(), d->windowType );
#endif

	}
	updateHeader();

	setWindowIcon( *pix );
}

void PMDockWidget::show()
{
	if(!manager) return;
	if ( parent() || manager->main->isVisible() )
    {
        if ( !parent() )
        {
			emit manager->setDockDefaultPos( this );
			emit setDockDefaultPos();
            if ( parent() )
            {
				makeDockVisible();
            } else
            {
				QWidget::show();
			}
		} else {
			QWidget::show();
		}
     }
}

/*
void PMDockWidget::setDockWindowType (NET::WindowType windowType)
{
	d->windowType = windowType;
	applyToWidget( parentWidget(), QPoint(0,0) );
}*/

void PMDockWidget::setDockWindowTransient (QWidget *parent, bool transientEnabled)
{
	d->_parent = parent;
	d->transient = transientEnabled;
	applyToWidget( parentWidget(), QPoint(0,0) );
}

bool PMDockWidget::event( QEvent* pevent )
{
	if(!manager) return false;

	switch ( pevent->type() )
	{
#undef FocusIn
		case QEvent::FocusIn:
			if (widget && !d->pendingFocusInEvent) {
				d->pendingFocusInEvent = true;
				widget->setFocus();
			}
			d->pendingFocusInEvent = false;
			break;
		case QEvent::ChildRemoved:
            if ( widget == ((QChildEvent*)pevent)->child() ) widget = nullptr;
			break;
		case QEvent::Show:
			if ( widget ) widget->show();
			emit manager->change();
			break;
		case QEvent::Hide:
			if ( widget ) widget->hide();
			emit manager->change();
			break;
		case QEvent::WindowTitleChange:
			if ( parentWidget() ){
				if ( parent()->inherits("PMDockSplitter") ){
					((PMDockSplitter*)(parent()))->updateName();
				}
				if ( parentDockTabGroup() ){
					setDockTabName( parentDockTabGroup() );
					parentDockTabGroup()->setTabText( parentDockTabGroup()->indexOf( this ), tabPageLabel() );
				}
			}
			break;
		case QEvent::Close:
			// MODIFICATION (zehender)
			// Top level widget is closed
			// emit same signal as if the widget is docked and closed with
			// the header button
			emit headerCloseButtonClicked();
			// emit iMBeingClosed();
			break;
		default:
			break;
	}
#undef KeyPress
	bool processed = QWidget::event( pevent );
	if( pevent->type() == QEvent::ShortcutOverride && !processed && !parent() ){
		// MODIFICATION (zehender)
		// floating widget, post event to main window
		processed = qApp->sendEvent( manager->dockMainWidget(), pevent );
	}
	return processed;
}

PMDockWidget* PMDockWidget::manualDock( PMDockWidget* target, DockPosition dockPos, int spliPos, QPoint pos, bool check, int tabIndex )
{
    if( !manager ) return nullptr;

    if ( this == target )
        return nullptr;  // docking to itself not possible

	bool succes = true; // tested flag

	// do not dock into a floating widget
    if( target && !target->parent() )
        target = nullptr;

	// check allowed this dock submit this operations
    if ( !( eDocking & (int)dockPos ) ){
		succes = false;
	}

	// check allowed target submit this operations
    if ( target && !(target->dockSite() & (int)dockPos) )
    {
		succes = false;
	}

    if ( parent() && !parent()->inherits("PMDockSplitter") && !parentDockTabGroup() )
    {
		succes = false;
	}

	// if docking to a tab group, and position is not center
	// dock to the parent of the tab group
    if( target && ( dockPos != PMDockWidget::DockCenter ) && ( dockPos != PMDockWidget::DockNone ) )
	{
		QWidget* pdt = target->parentDockTabGroup();
		if( pdt )
			return manualDock( ( PMDockWidget* ) ( pdt->parent() ),
									 dockPos, spliPos, pos, check, tabIndex );
	}

	if ( !succes ){
		// try to make another manualDock
        PMDockWidget* dock_result = nullptr;
		if ( target && !check ){
			PMDockWidget::DockPosition another__dockPos = PMDockWidget::DockNone;
			switch ( dockPos ){
				case PMDockWidget::DockLeft  : another__dockPos = PMDockWidget::DockRight ; break;
				case PMDockWidget::DockRight : another__dockPos = PMDockWidget::DockLeft  ; break;
				case PMDockWidget::DockTop   : another__dockPos = PMDockWidget::DockBottom; break;
				case PMDockWidget::DockBottom: another__dockPos = PMDockWidget::DockTop   ; break;
				default: break;
			}
			dock_result = target->manualDock( this, another__dockPos, spliPos, pos, true, tabIndex );
		}
		return dock_result;
	}
	// end check block

	d->blockHasUndockedSignal = true;
	undock();
	d->blockHasUndockedSignal = false;

	if ( !target ){
		move( pos );
		show();
		emit manager->change();
		return this;
	}

	PMDockTabGroup* parentTab = target->parentDockTabGroup();
	if ( parentTab ){
		// add to existing TabGroup
		applyToWidget( parentTab );
		parentTab->insertTab( tabIndex, this, windowIcon(), tabPageLabel() );
		setDockTabName( parentTab );
		if( !toolTipStr.isEmpty())
			parentTab->setTabToolTip( parentTab->indexOf( this ), toolTipStr);

		currentDockPos = PMDockWidget::DockCenter;
		emit manager->change();
		return (PMDockWidget*)parentTab->parent();
	}

	// MODIFICATION (Zehender):
	// If DockPosition is DockLeft or DockRight, add the widget
	// left or right of the target, so that a new vertical splitter
	// (a splitter with horizontal widget layout :-) is created
	// that spawns the full height of the main view

	if( ( dockPos == PMDockWidget::DockLeft ) ||
		 ( dockPos == PMDockWidget::DockRight ) )
	{
		PMDockWidget* newTarget = target;
		bool found = false;
		QWidget* wtarget = target;
		while( wtarget && !found )
		{
			if( wtarget->inherits( "PMDockWidget" ) )
			{
				PMDockWidget* dw = ( PMDockWidget* ) wtarget;
				newTarget = dw;
				QWidget* pw = wtarget->parentWidget();
				if( pw )
				{
					if( pw->inherits( "PMDockSplitter" ) )
					{
						PMDockSplitter* ds = ( PMDockSplitter* ) pw;
						if( ds->splitterOrientation() == Vertical )
							found = true;
					}
				}
			}
			wtarget = wtarget->parentWidget();
		}

		if( newTarget != target )
			return manualDock( newTarget, dockPos, spliPos, pos, check, tabIndex );
	}

	// END MODIFICATION

	// create a new dockwidget that will contain the target and this
	QWidget* parentDock = target->parentWidget();
	PMDockWidget* newDock = new PMDockWidget( manager, QPixmap(""), parentDock );
	newDock->currentDockPos = target->currentDockPos;

	if ( dockPos == PMDockWidget::DockCenter ){
		newDock->isTabGroup = true;
	} else {
		newDock->isGroup = true;
	}
	newDock->eDocking = (target->eDocking & eDocking) & (~(int)PMDockWidget::DockCenter);

	newDock->applyToWidget( parentDock );

	if ( !parentDock ){
		// dock to a toplevel dockwidget means newDock is toplevel now
		newDock->move( target->frameGeometry().topLeft() );
		newDock->resize( target->geometry().size() );
		if ( target->isVisible() ) newDock->show();
	}

	// redirect the dockback button to the new dockwidget
    if( target->formerBrotherDockWidget != nullptr) {
		newDock->formerBrotherDockWidget = target->formerBrotherDockWidget;
        if( formerBrotherDockWidget != nullptr)
			QObject::connect( newDock->formerBrotherDockWidget, SIGNAL(iMBeingClosed()),
									newDock, SLOT(loseFormerBrotherDockWidget()) );
      target->loseFormerBrotherDockWidget();
	}
	newDock->formerDockPos = target->formerDockPos;

	if ( dockPos == PMDockWidget::DockCenter )
	{
		PMDockTabGroup* tab = new PMDockTabGroup( newDock );
		tab->setObjectName("_dock_tab");
		QObject::connect(tab, SIGNAL(currentChanged(QWidget*)), d, SLOT(slotFocusEmbeddedWidget(QWidget*)));
		newDock->setWidget( tab );

		target->applyToWidget( tab );
		applyToWidget( tab );


		tab->addTab( target, target->windowIcon(), target->tabPageLabel() );
		if( !target->toolTipString().isEmpty())
			tab->setTabToolTip( tab->indexOf( target ), target->toolTipString());

		tab->insertTab( tabIndex, this, windowIcon(), tabPageLabel() );
		if( !toolTipString().isEmpty())
			tab->setTabToolTip( tab->indexOf( this ), toolTipString() );

		setDockTabName( tab );
		tab->show();

		currentDockPos = DockCenter;
		target->formerDockPos = target->currentDockPos;
		target->currentDockPos = DockCenter;
	}
	else {
		// if to dock not to the center of the target dockwidget,
		// dock to newDock
		PMDockSplitter* panner = 0L;
		if ( dockPos == PMDockWidget::DockTop  || dockPos == PMDockWidget::DockBottom ) panner = new PMDockSplitter( newDock, Qt::Horizontal, spliPos, manager->splitterHighResolution() );
		if ( dockPos == PMDockWidget::DockLeft || dockPos == PMDockWidget::DockRight  ) panner = new PMDockSplitter( newDock, Qt::Vertical , spliPos, manager->splitterHighResolution() );
		if( panner ) panner->setObjectName("_dock_split_");
		newDock->setWidget( panner );

		panner->setOpaqueResize(manager->splitterOpaqueResize());
		panner->setKeepSize(manager->splitterKeepSize());
		panner->setFocusPolicy( Qt::NoFocus );
		target->applyToWidget( panner );
		applyToWidget( panner );
		target->formerDockPos = target->currentDockPos;
		if ( dockPos == PMDockWidget::DockRight) {
			panner->activate( target, this );
			currentDockPos = PMDockWidget::DockRight;
			target->currentDockPos = PMDockWidget::DockLeft;
		}
		else if( dockPos == PMDockWidget::DockBottom) {
			panner->activate( target, this );
			currentDockPos = PMDockWidget::DockBottom;
			target->currentDockPos = PMDockWidget::DockTop;
		}
		else if( dockPos == PMDockWidget::DockTop) {
			panner->activate( this, target );
			currentDockPos = PMDockWidget::DockTop;
			target->currentDockPos = PMDockWidget::DockBottom;
		}
		else if( dockPos == PMDockWidget::DockLeft) {
			panner->activate( this, target );
			currentDockPos = PMDockWidget::DockLeft;
			target->currentDockPos = PMDockWidget::DockRight;
		}
		target->show();
		show();
		panner->show();
	}

	if ( parentDock ){
		if ( parentDock->inherits("PMDockSplitter") ){
			PMDockSplitter* sp = (PMDockSplitter*)parentDock;
			sp->deactivate();
			if ( sp->getFirst() == target )
                sp->activate( newDock, nullptr );
			else
                sp->activate( nullptr, newDock );
		}
	}

	newDock->show();
	emit target->docking( this, dockPos );
	emit manager->replaceDock( target, newDock );
	emit manager->change();

	return newDock;
}

PMDockTabGroup* PMDockWidget::parentDockTabGroup() const
{
    if ( !parent() ) return nullptr;
	QWidget* candidate = parentWidget()->parentWidget();
	if ( candidate && candidate->inherits("PMDockTabGroup") ) return (PMDockTabGroup*)candidate;
    return nullptr;
}

void PMDockWidget::toDesktop()
{
   QPoint p = mapToGlobal( QPoint( -30, -30 ) );
   if( p.x() < 0 )
      p.setX( 0 );
   if( p.y() < 0 )
      p.setY( 0 );
   manualDock( nullptr, DockDesktop, 50, p );
}

void PMDockWidget::undock()
{
	if(!manager) return;

	QWidget* parentW = parentWidget();
	if ( !parentW ){
		hide();
		if (!d->blockHasUndockedSignal)
			emit hasUndocked();
		return;
	}

	formerDockPos = currentDockPos;
	currentDockPos = PMDockWidget::DockDesktop;

	manager->blockSignals(true);
	manager->undockProcess = true;

	bool isV = parentW->isVisible();

	PMDockTabGroup* parentTab = parentDockTabGroup();
	if ( parentTab ){
		d->index = parentTab->indexOf( this); // memorize the page position in the tab widget
		parentTab->removeTab( d->index );
		formerBrotherDockWidget = (PMDockWidget*)parentTab->widget( 0 );
		QObject::connect( formerBrotherDockWidget, SIGNAL(iMBeingClosed()),
								this, SLOT(loseFormerBrotherDockWidget()) );
        applyToWidget( nullptr );
		if ( parentTab->count() == 1 ){

			// last subdock widget in the tab control
			PMDockWidget* lastTab = (PMDockWidget*)parentTab->widget( 0 );
			parentTab->removeTab( parentTab->indexOf( lastTab ) );
            lastTab->applyToWidget( nullptr );
			lastTab->move( parentTab->mapToGlobal(parentTab->frameGeometry().topLeft()) );

			// PMDockTabGroup always have a parent that is a PMDockWidget
			PMDockWidget* parentOfTab = (PMDockWidget*)parentTab->parent();
			delete parentTab; // PMDockTabGroup

			QWidget* parentOfDockWidget = parentOfTab->parentWidget();
            if ( parentOfDockWidget == nullptr ){
				if ( isV ) lastTab->show();
			} else {
				if ( parentOfDockWidget->inherits("PMDockSplitter") ){
					PMDockSplitter* split = (PMDockSplitter*)parentOfDockWidget;
					lastTab->applyToWidget( split );
					split->deactivate();
					if ( split->getFirst() == parentOfTab ){
						split->activate( lastTab );
						if ( ((PMDockWidget*)split->parent())->splitterOrientation == Qt::Vertical )
							emit ((PMDockWidget*)split->getAnother(parentOfTab))->docking( parentOfTab, PMDockWidget::DockLeft );
						else
							emit ((PMDockWidget*)split->getAnother(parentOfTab))->docking( parentOfTab, PMDockWidget::DockTop );
					} else {
                        split->activate( nullptr, lastTab );
						if ( ((PMDockWidget*)split->parent())->splitterOrientation == Qt::Vertical )
							emit ((PMDockWidget*)split->getAnother(parentOfTab))->docking( parentOfTab, PMDockWidget::DockRight );
						else
							emit ((PMDockWidget*)split->getAnother(parentOfTab))->docking( parentOfTab, PMDockWidget::DockBottom );
					}
					split->show();
				} else {
					lastTab->applyToWidget( parentOfDockWidget );
				}
				lastTab->show();
			}
			manager->blockSignals(false);
			emit manager->replaceDock( parentOfTab, lastTab );
			lastTab->currentDockPos = parentOfTab->currentDockPos;
			emit parentOfTab->iMBeingClosed();
			manager->blockSignals(true);
			delete parentOfTab;

		} else {
			setDockTabName( parentTab );
		}
	} else {
/*********************************************************************************************/
		if ( parentW->inherits("PMDockSplitter") ){
			PMDockSplitter* parentSplitterOfDockWidget = (PMDockSplitter*)parentW;
			d->splitPosInPercent = parentSplitterOfDockWidget->separatorPos();

			PMDockWidget* secondWidget = (PMDockWidget*)parentSplitterOfDockWidget->getAnother( this );
			PMDockWidget* group        = (PMDockWidget*)parentSplitterOfDockWidget->parentWidget();
			formerBrotherDockWidget = secondWidget;
            applyToWidget( nullptr );
			group->hide();

            if( formerBrotherDockWidget != nullptr)
				QObject::connect( formerBrotherDockWidget, SIGNAL(iMBeingClosed()),
										this, SLOT(loseFormerBrotherDockWidget()) );

			if ( !group->parentWidget() ){
                secondWidget->applyToWidget( nullptr, group->frameGeometry().topLeft() );
				secondWidget->resize( group->width(), group->height() );
			} else {
				QWidget* obj = group->parentWidget();
				secondWidget->applyToWidget( obj );
				if ( obj->inherits("PMDockSplitter") ){
					PMDockSplitter* parentOfGroup = (PMDockSplitter*)obj;
					parentOfGroup->deactivate();

					if ( parentOfGroup->getFirst() == group )
						parentOfGroup->activate( secondWidget );
					else
                        parentOfGroup->activate( nullptr, secondWidget );
				}
			}
			secondWidget->currentDockPos = group->currentDockPos;
			secondWidget->formerDockPos  = group->formerDockPos;
			delete parentSplitterOfDockWidget;
			manager->blockSignals(false);
			emit manager->replaceDock( group, secondWidget );
			emit group->iMBeingClosed();
			manager->blockSignals(true);
			delete group;

			if ( isV ) secondWidget->show();
		} else {
            applyToWidget( nullptr );
		}
/*********************************************************************************************/
	}
	manager->blockSignals(false);
	if (!d->blockHasUndockedSignal)
		emit manager->change();
	manager->undockProcess = false;

	if (!d->blockHasUndockedSignal)
		emit hasUndocked();
}

void PMDockWidget::setWidget( QWidget* mw )
{
	if ( !mw ) return;

	if ( mw->parent() != this ){
		mw->setParent( this );
	}

	widget = mw;
	delete layout;

	layout = new QVBoxLayout( this );
	layout->setSpacing( 0 );
	layout->setMargin( 0 );

	layout->addWidget( header );
	layout->addWidget( widget,1 );
}

void PMDockWidget::setDockTabName( PMDockTabGroup* tab )
{
	QString listOfName;
	QString listOfCaption;
	for ( int i = 0; i < tab->count(); ++i ) {
		QWidget *w = tab->widget( i );
		listOfCaption.append( w->windowTitle() ).append(",");
		listOfName.append( w->objectName() ).append(",");
	}
	listOfCaption.remove( listOfCaption.length()-1, 1 );
	listOfName.remove( listOfName.length()-1, 1 );

	tab->parentWidget()->setObjectName( listOfName );
	tab->parentWidget()->setWindowTitle( listOfCaption );

	tab->parentWidget()->repaint(); // PMDockWidget->repaint
	if ( tab->parentWidget()->parent() )
		if ( tab->parentWidget()->parent()->inherits("PMDockSplitter") )
			((PMDockSplitter*)(tab->parentWidget()->parent()))->updateName();
}

bool PMDockWidget::mayBeHide() const
{
	if(!manager) return false;

	bool f = (parent() != manager->main);
	return ( !isGroup && !isTabGroup && f && isVisible() && ( eDocking != (int)PMDockWidget::DockNone ) );
}

bool PMDockWidget::mayBeShow() const
{
	if(!manager) return false;

	bool f = (parent() != manager->main);
	return ( !isGroup && !isTabGroup && f && !isVisible() );
}

void PMDockWidget::changeHideShowState()
{
	if(!manager) return;

	if ( mayBeHide() ){
		undock();
		return;
	}

	if ( mayBeShow() ){
		if ( manager->main->inherits("PMDockMainWindow") ){
			((PMDockMainWindow*)manager->main)->makeDockVisible(this);
		} else {
			makeDockVisible();
		}
	}
}

void PMDockWidget::makeDockVisible()
{
	if ( parentDockTabGroup() ){
		parentDockTabGroup()->setCurrentIndex( parentDockTabGroup()->indexOf( this ) );
	}
	if ( isVisible() ) return;

	QWidget* p = parentWidget();
	while ( p ){
		if ( !p->isVisible() )
			p->show();
		p = p->parentWidget();
	}
    if( parent() == nullptr) // is undocked
		dockBack();
	show();
}

void PMDockWidget::loseFormerBrotherDockWidget()
{
    if( formerBrotherDockWidget != nullptr)
		QObject::disconnect( formerBrotherDockWidget, SIGNAL(iMBeingClosed()),
									this, SLOT(loseFormerBrotherDockWidget()) );
    formerBrotherDockWidget = nullptr;
	repaint();
}

void PMDockWidget::dockBack()
{
	if(!manager) return;

	if( formerBrotherDockWidget) {
		// search all children if it tries to dock back to a child
		bool found = false;
		QList<PMDockWidget *> cl = findChildren<PMDockWidget *>();
		foreach ( PMDockWidget *obj, cl ) {
            while ( !found && obj!= nullptr ) {
				QWidget* widg = qobject_cast<QWidget*>(obj);
				if( widg == formerBrotherDockWidget)
					found = true;
			}
		}

		if( !found) {
			// can dock back to the former brother dockwidget
			manualDock( formerBrotherDockWidget, formerDockPos, d->splitPosInPercent, QPoint(0,0), false, d->index);
            formerBrotherDockWidget = nullptr;
			makeDockVisible();
			return;
		}
	}

	// else dockback to the dockmainwindow (default behaviour)
	manualDock( ((PMDockMainWindow*)manager->main)->getMainDockWidget(), formerDockPos, d->splitPosInPercent, QPoint(0,0), false, d->index);
    formerBrotherDockWidget = nullptr;
	if (parent())
		makeDockVisible();
}

bool PMDockWidget::isDockBackPossible() const
{
    if( (formerBrotherDockWidget == nullptr) || !(formerBrotherDockWidget->dockSite() & formerDockPos))
		return false;
	else
		return true;
}

void PMDockWidget::managerDestroyed(QObject* obj){
    if(manager == obj) manager = nullptr;
}

/*
void PMDockWidget::drawDragRectangle(const QRect& r){
	// TODO use transparent overlay widget
	d->drawRectangle = true;
	d->rect = r;
	bool clipped = testAttribute(Qt::WA_PaintUnclipped);
	setAttribute( Qt::WA_PaintUnclipped );
	repaint();
	setAttribute( Qt::WA_PaintUnclipped, clipped );
}

void PMDockWidget::paintEvent( QPaintEvent* ev){
	if(d->drawRectangle){
		QPainter paint;
		if(paint.begin( this )){
			paint.setCompositionMode(QPainter::CompositionMode_Xor);
			// draw the rectangle
			paint.drawRect(d->rect.x(), d->rect.y(), d->rect.width(), d->rect.height());
			paint.end();
		}
		d->drawRectangle = false;
	} else {
		QWidget::paintEvent( ev );
	}
}
*/

void PMDockWidget::setDragRectangle(PMDockWidget::DockPosition pos){
	if(m_dragRectangle != pos){
		m_dragRectangle = pos;
		if(m_dragRectangle){
			updateDragOverlay();
			m_pDragOverlay->raise();
			m_pDragOverlay->show();
			m_pDragOverlay->update();
		} else {
			m_pDragOverlay->hide();
		}
	}
}

void PMDockWidget::resizeEvent(QResizeEvent *rsize)
{
	QWidget::resizeEvent(rsize);
	updateDragOverlay();
}
//glitch
void PMDockWidget::updateDragOverlay(){
	QRect base = QRect(QPoint(0,0), size());
	if(widget) base = widget->geometry();
	QRect rect = base;

	switch(m_dragRectangle){
		case DockTop:
			rect.setHeight(base.height()/5-1);
			m_pDragOverlay->setGeometry(rect);
			break;
		case DockBottom:
			rect.setHeight(base.height()/5-1);
			rect.moveBottom(base.bottom());
			m_pDragOverlay->setGeometry(rect);
			break;
		case DockLeft:
			rect.setWidth(base.width()/5-1);
			m_pDragOverlay->setGeometry(rect);
			break;
		case DockRight:
			rect.setWidth(base.width()/5-1);
			rect.moveRight(base.right());
			m_pDragOverlay->setGeometry(rect);
			break;
		case DockCenter:
			rect.setHeight(base.height()/3-1);
			rect.setWidth(base.width()/3-1);
			rect.moveCenter(base.center());
			m_pDragOverlay->setGeometry(rect);
			break;
		default:
			break;
	}
}

/**************************************************************************************/

class PMDockManager::PMDockManagerPrivate
{
public:
    /**
     * These flags store information about the splitter behaviour
     */
    MouseButton button;
    bool splitterOpaqueResize;
    bool splitterKeepSize;
    bool splitterHighResolution;
   /**
	 * This flag stores the information if dragging is ready to start. Used between mousePress and mouseMove event.
	 */
	bool readyToDrag;

	/**
	 * This variable stores the offset of the mouse cursor to the upper left edge of the current drag widget.
	 */
	QPoint dragOffset;
};

PMDockManager::PMDockManager( QWidget* mainWindow  )
		:QObject( mainWindow )
		,main(mainWindow)
        ,currentDragWidget(nullptr)
        ,currentMoveWidget(nullptr)
        ,childDockWidgetList(nullptr)
        ,autoCreateDock(nullptr)
		,storeW(0)
		,storeH(0)
		,draging(false)
		,undockProcess(false)
		,dropCancel(true)
{
	d = new PMDockManagerPrivate;
	d->splitterOpaqueResize = false;
	d->splitterKeepSize = false;
	d->splitterHighResolution = false;
	d->readyToDrag = false;

	main->installEventFilter( this );

	undockProcess = false;

    menu = new QMenu();

    connect( menu, SIGNAL( aboutToShow() ), SLOT( slotMenuPopup() ) );
    connect( menu, &QMenu::triggered, this, &PMDockManager::slotMenuActivated );

	childDock = new QList<QObject*>();
}

PMDockManager::~PMDockManager()
{
	delete menu;

	/*
	// TODO: porting, segfaults here, children already deleted?
	PMDockWidget * obj;
	foreach ( QObject *o, *childDock ) {
  	obj=(PMDockWidget*)o;
	delete obj;
	}
	*/
	delete childDock;
	delete d;
}

void PMDockManager::activate()
{
	PMDockWidget * obj;
	foreach ( QObject *o, *childDock ) {
		obj=(PMDockWidget*)o;
		if ( obj->widget ) obj->widget->show();
		if ( !obj->parentDockTabGroup() ){
			obj->show();
		}
	}
	if ( !main->inherits("QDialog") ) main->show();
}

bool PMDockManager::eventFilter( QObject *obj, QEvent *event )
{
	if ( obj->inherits("PMDockWidgetAbstractHeaderDrag") ){
        PMDockWidget* pDockWdgAtCursor = nullptr;
		PMDockWidget* curdw = ((PMDockWidgetAbstractHeaderDrag*)obj)->dockWidget();
		switch ( event->type() ){
			case QEvent::MouseButtonDblClick:
				if (curdw->currentDockPos == PMDockWidget::DockDesktop)  curdw->dockBack();
				else curdw->toDesktop();
				break;
			case QEvent::MouseButtonPress:
				if ( ((QMouseEvent*)event)->button() == Qt::LeftButton ){
					if ( curdw->eDocking != (int)PMDockWidget::DockNone ){
						dropCancel = true;
						curdw->setFocus();
						qApp->processEvents( QEventLoop::WaitForMoreEvents );

						currentDragWidget = curdw;
                        currentMoveWidget = nullptr;
						childDockWidgetList = new QWidgetList();
						childDockWidgetList->append( curdw );
						findChildDockWidget( curdw, childDockWidgetList );

						d->readyToDrag = true;
						d->dragOffset = QCursor::pos()-currentDragWidget->mapToGlobal(QPoint(0,0));
					}
				}
				break;
			case QEvent::MouseButtonRelease:
				if ( ((QMouseEvent*)event)->button() == Qt::LeftButton ){
					if ( draging ){
						if ( !dropCancel )
							drop();
						else
							cancelDrop();
					}
					if (d->readyToDrag) {
						if(currentDragWidget) currentDragWidget->setDragRectangle(PMDockWidget::DockNone);
						if(currentMoveWidget) currentMoveWidget->setDragRectangle(PMDockWidget::DockNone);
						d->readyToDrag = false;
                        currentDragWidget = nullptr;
						delete childDockWidgetList;
                        childDockWidgetList = nullptr;
					}
					draging = false;
					dropCancel = true;
				}
				break;
			case QEvent::MouseMove:
				if ( draging ) {
					pDockWdgAtCursor = findDockWidgetAt( QCursor::pos() );
					PMDockWidget* oldMoveWidget = currentMoveWidget;
					if ( currentMoveWidget && pDockWdgAtCursor == currentMoveWidget ) { //move
						dragMove( currentMoveWidget, currentMoveWidget->mapFromGlobal( QCursor::pos() ) );
						break;
					} else {
						if(currentDragWidget) currentDragWidget->setDragRectangle(PMDockWidget::DockNone);
						if(currentMoveWidget) currentMoveWidget->setDragRectangle(PMDockWidget::DockNone);
					}

					if ( !pDockWdgAtCursor && (curdw->eDocking & (int)PMDockWidget::DockDesktop) == 0 ){
						// just moving at the desktop
						if(currentMoveWidget) currentMoveWidget->setDragRectangle(PMDockWidget::DockNone);
						currentMoveWidget = pDockWdgAtCursor;
						curPos = PMDockWidget::DockDesktop;
					} else {
						if ( oldMoveWidget && pDockWdgAtCursor != currentMoveWidget ) { //leave
							if(currentMoveWidget) currentMoveWidget->setDragRectangle(PMDockWidget::DockNone);
							currentMoveWidget = pDockWdgAtCursor;
							curPos = PMDockWidget::DockDesktop;
						}
					}

					if ( oldMoveWidget != pDockWdgAtCursor && pDockWdgAtCursor ) { //enter pDockWdgAtCursor
						if(currentMoveWidget) currentMoveWidget->setDragRectangle(PMDockWidget::DockNone);
						currentMoveWidget = pDockWdgAtCursor;
						curPos = PMDockWidget::DockDesktop;
					}
				} else {
                    if ( d->readyToDrag ) {
						d->readyToDrag = false;
                        if ( curdw->eDocking != ( int )PMDockWidget::DockNone ) {
                            startDrag( curdw );
						}
					}
				}
				break;
			default:
				break;
		}
	}
	return QObject::eventFilter( obj, event );
}

PMDockWidget* PMDockManager::findDockWidgetAt( const QPoint& pos )
{
	dropCancel = true;

    if ( !currentDragWidget )
        return nullptr; // pointer access safety

    if ( currentDragWidget->eDocking == ( int )PMDockWidget::DockNone ) return nullptr;

	QWidget* w = QApplication::widgetAt( pos );
    while( w && !w->inherits( "PMDockWidget") ) w = w->parentWidget();

	if ( !w ) {
		dropCancel = false;
        return nullptr;
	}

    if ( w->findChild<PMDockSplitter*>( "_dock_split_" ) ) return nullptr;
    if ( w->findChild<PMDockTabGroup*>( "_dock_tab" ) ) return nullptr;

    if ( !childDockWidgetList ) return nullptr;
    if ( childDockWidgetList->indexOf( w ) != -1 ) return nullptr;
    if ( currentDragWidget->isGroup && ( ( PMDockWidget* )w )->parentDockTabGroup() ) return 0L;

	PMDockWidget* www = (PMDockWidget*)w;
    if ( www->dockSite() == ( int )PMDockWidget::DockNone ) return nullptr;

	PMDockWidget::DockPosition curPos = PMDockWidget::DockDesktop;
	QPoint cpos  = www->mapFromGlobal( pos );

    int ww = www->widget->width() / 3;
    int hh = www->widget->height() / 3;

    if ( cpos.y() <= hh ){
		curPos = PMDockWidget::DockTop;
	} else
        if ( cpos.y() >= 2*hh ){
			curPos = PMDockWidget::DockBottom;
		} else
            if ( cpos.x() <= ww ){
				curPos = PMDockWidget::DockLeft;
			} else
                if ( cpos.x() >= 2*ww ){
					curPos = PMDockWidget::DockRight;
				} else
					curPos = PMDockWidget::DockCenter;

    if ( !( www->dockSite() & ( int )curPos ) ) return nullptr;
    if ( !( currentDragWidget->eDocking & ( int )curPos ) ) return nullptr;
    if ( www->manager != this ) return nullptr;

	dropCancel = false;
	return www;
}

void PMDockManager::findChildDockWidget( QWidget*& ww, const QWidget* p, const QPoint& pos )
{
	if ( p->children().isEmpty() ) {
		QWidget *w;
		foreach( QObject* o, p->children() )
		{
			if ( o->isWidgetType() ) {
				w = (QWidget*)o;
				if ( w->isVisible() && w->geometry().contains(pos) ) {
					if ( w->inherits("PMDockWidget") ) ww = w;
					findChildDockWidget( ww, w, w->mapFromParent(pos) );
					return;
				}
			}
		}
	}
	return;
}

void PMDockManager::findChildDockWidget( const QWidget* p, QWidgetList*& list )
{
	if ( !p->children().isEmpty() ) {
		QWidget *w;
		foreach( QObject* o, p->children() )
		{
			if ( o->isWidgetType() ) {
				w = (QWidget*)o;
				if ( w->isVisible() ) {
					if ( w->inherits("PMDockWidget") ) list->append( w );
					findChildDockWidget( w, list );
				}
			}
		}
	}
	return;
}

QList<PMDockWidget*> PMDockManager::findFloatingWidgets()
{
	QList<PMDockWidget*> result;
   foreach ( QObject *o, *childDock ) {
      if( o->inherits( "PMDockWidget" ) && !o->parent() )
         result.append( ( PMDockWidget* ) o );
   }
	return result;
}

void PMDockManager::startDrag( PMDockWidget* w )
{
    if( ( w->currentDockPos == PMDockWidget::DockLeft ) ||
        ( w->currentDockPos == PMDockWidget::DockRight ) ||
        ( w->currentDockPos == PMDockWidget::DockTop ) ||
        ( w->currentDockPos == PMDockWidget::DockBottom ) )
    {
		w->prevSideDockPosBeforeDrag = w->currentDockPos;
        if ( w->parentWidget()->inherits( "PMDockSplitter" ) )
        {
            PMDockSplitter* parentSplitterOfDockWidget = (PMDockSplitter*)(w->parentWidget() );
            w->d->splitPosInPercent = parentSplitterOfDockWidget->separatorPos();
		}
	}

	curPos = PMDockWidget::DockDesktop;
	draging = true;

    QApplication::setOverrideCursor( QCursor( Qt::SizeAllCursor ) );
}

void PMDockManager::dragMove( PMDockWidget* dw, QPoint pos )
{
    //QPoint p = dw->mapToGlobal( dw->widget->pos() );

    QSize r = dw->widget->size();
    if ( dw->parentDockTabGroup() ){
		curPos = PMDockWidget::DockCenter;
		dw->setDragRectangle(PMDockWidget::DockCenter);
		return;
	}

    int w = r.width() / 3;
    int h = r.height() / 3;

    //PMDockMainWindow* mw = ( PMDockMainWindow* ) parent();
    //QWidget* cw = mw->centralWidget();
    //QPoint cwp = cw->mapToGlobal( QPoint( 0, 0 ) );

    if ( pos.y() <= h )
	{
		curPos = PMDockWidget::DockTop;
	}
    else if ( pos.y() >= 2*h )
	{
		curPos = PMDockWidget::DockBottom;
	}
    else if ( pos.x() <= w )
	{
		curPos = PMDockWidget::DockLeft;
	}
    else if ( pos.x() >= 2*w )
	{
		curPos = PMDockWidget::DockRight;
	}
	else
	{
		curPos = PMDockWidget::DockCenter;
	}
    dw->setDragRectangle( curPos );
}


void PMDockManager::cancelDrop()
{
    QApplication::restoreOverrideCursor();

	delete childDockWidgetList;
    childDockWidgetList = nullptr;

    if( currentDragWidget && currentDragWidget->dragRectangle() ) currentDragWidget->setDragRectangle(PMDockWidget::DockNone);
    if( currentMoveWidget && currentMoveWidget->dragRectangle() ) currentMoveWidget->setDragRectangle(PMDockWidget::DockNone);
}


void PMDockManager::drop()
{
    if( currentDragWidget && currentDragWidget->dragRectangle() ) currentDragWidget->setDragRectangle (PMDockWidget::DockNone );
    if( currentMoveWidget && currentMoveWidget->dragRectangle() ) currentMoveWidget->setDragRectangle( PMDockWidget::DockNone );

	QApplication::restoreOverrideCursor();

	delete childDockWidgetList;
    childDockWidgetList = nullptr;

	if ( dropCancel ) return;
    if ( !currentMoveWidget && (  (currentDragWidget->eDocking & ( int )PMDockWidget::DockDesktop ) == 0 ) ) {
		// TODO is this neede here? we already cleared the drag rect
		/*
		d->dragRect = QRect();  // cancel drawing
		drawDragRectangle();    // only the old rect will be deleted
		*/
		return;
	}
	if ( !currentMoveWidget && !currentDragWidget->parent() ) {
		currentDragWidget->move( QCursor::pos() - d->dragOffset );
	}
	else if (currentDragWidget) {
		int splitPos = currentDragWidget->d->splitPosInPercent;
		// do we have to calculate 100%-splitPosInPercent?
		if( (curPos != currentDragWidget->prevSideDockPosBeforeDrag) && (curPos != PMDockWidget::DockCenter) && (curPos != PMDockWidget::DockDesktop)) {
			switch( currentDragWidget->prevSideDockPosBeforeDrag) {
				case PMDockWidget::DockLeft:   if(curPos != PMDockWidget::DockTop)    splitPos = 100-splitPos; break;
				case PMDockWidget::DockRight:  if(curPos != PMDockWidget::DockBottom) splitPos = 100-splitPos; break;
				case PMDockWidget::DockTop:    if(curPos != PMDockWidget::DockLeft)   splitPos = 100-splitPos; break;
				case PMDockWidget::DockBottom: if(curPos != PMDockWidget::DockRight)  splitPos = 100-splitPos; break;
				default: break;
			}
		}
		currentDragWidget->manualDock( currentMoveWidget, curPos , splitPos, QCursor::pos() - d->dragOffset );
		currentDragWidget->makeDockVisible();
	}
}

void PMDockManager::writeConfig(QDomElement&)
{
}

void PMDockManager::readConfig(QDomElement&)
{
}

PMDockWidget* PMDockManager::getDockWidgetFromName( const QString& dockName )
{
	QObjectList::iterator it = childDock->begin();
	PMDockWidget * obj;
	while ( (obj=(PMDockWidget*)(*it)) ) {
		++it;
		if ( obj->objectName() == dockName ) return obj;
	}

    PMDockWidget* autoCreate = nullptr;
	if ( autoCreateDock ){
		autoCreate = new PMDockWidget( this, QPixmap("") );
		autoCreateDock->append( autoCreate );
	}
	return autoCreate;
}
void PMDockManager::setSplitterOpaqueResize(bool b)
{
	d->splitterOpaqueResize = b;
}

bool PMDockManager::splitterOpaqueResize() const
{
	return d->splitterOpaqueResize;
}

void PMDockManager::setSplitterKeepSize(bool b)
{
	d->splitterKeepSize = b;
}

bool PMDockManager::splitterKeepSize() const
{
	return d->splitterKeepSize;
}

void PMDockManager::setSplitterHighResolution(bool b)
{
	d->splitterHighResolution = b;
}

bool PMDockManager::splitterHighResolution() const
{
	return d->splitterHighResolution;
}

void PMDockManager::slotMenuPopup()
{
	menu->clear();
	menuData.clear();

	QObjectList::iterator it = childDock->begin();
	PMDockWidget * obj;
	while ( (obj=(PMDockWidget*)(*it)) ) {
		++it;
		if ( obj->mayBeHide() )
		{
			menu->addAction( obj->windowIcon(), QString("Hide ") + obj->windowTitle() );
			menuData.append( MenuDockData( obj, true ) );
		}

		if ( obj->mayBeShow() )
		{
			menu->addAction( obj->windowIcon(), QString("Show ") + obj->windowTitle() );
			menuData.append( MenuDockData( obj, false ) );
		}
	}
}

void PMDockManager::slotMenuActivated( QAction* act )
{
    MenuDockData data = menuData.at( 1 );
	data.dock->changeHideShowState();
}

PMDockWidget* PMDockManager::findWidgetParentDock( QWidget* w ) const
{
	QObjectList::iterator it = childDock->begin();
	PMDockWidget * dock;
    PMDockWidget * found = nullptr;

	while ( (dock=(PMDockWidget*)(*it)) ) {
		++it;
		if ( dock->widget == w ){ found  = dock; break; }
	}
	return found;
}

#ifdef _JOWENN_EXPERIMENTAL_

PMDockArea::PMDockArea( QWidget* parent)
		:QWidget( parent)
{
	QString new_name = objectName() + QString("_DockManager");
	dockManager = new PMDockManager( this );
	dockManager->setObjectName(new_name);
    mainDockWidget = nullptr;
}

PMDockArea::~PMDockArea()
{
	delete dockManager;
}

PMDockWidget* PMDockArea::createDockWidget( const QPixmap &pixmap, QWidget* parent, const QString& strCaption, const QString& strTabPageLabel)
{
	return new PMDockWidget( dockManager, pixmap, parent, strCaption, strTabPageLabel );
}

void PMDockArea::makeDockVisible( PMDockWidget* dock )
{
    if ( dock != nullptr)
		dock->makeDockVisible();
}

void PMDockArea::makeDockInvisible( PMDockWidget* dock )
{
    if ( dock != nullptr)
		dock->undock();
}

void PMDockArea::makeWidgetDockVisible( QWidget* widget )
{
	makeDockVisible( dockManager->findWidgetParentDock(widget) );
}

void PMDockArea::writeDockConfig(QDomElement &base)
{
	dockManager->writeConfig(base);
}

void PMDockArea::readDockConfig(QDomElement &base)
{
	dockManager->readConfig(base);
}

void PMDockArea::slotDockWidgetUndocked()
{
	QObject* pSender = (QObject*) sender();
	if (!pSender->inherits("PMDockWidget")) return;
	PMDockWidget* pDW = (PMDockWidget*) pSender;
	emit dockWidgetHasUndocked( pDW);
}

void PMDockArea::resizeEvent(QResizeEvent *rsize)
{
  QWidget::resizeEvent(rsize);
  if (!children().isEmpty()){
#ifndef NO_KDE2
//    qDebug(282)<<"K3DockArea::resize";
#endif
    QList<QWidget *> list = findChildren<QWidget*>();

    foreach( QWidget *w, list )
    {
      w->setGeometry(QRect(QPoint(0,0),size()));
    }
#if 0
    K3DockSplitter *split;
//    for (unsigned int i=0;i<children()->count();i++)
    {
//    	QPtrList<QObject> list(children());
//       QObject *obj=((QPtrList<QObject*>)children())->at(i);
	QObject *obj=children()->getFirst();
       if (split = dynamic_cast<K3DockSplitter*>(obj))
       {
          split->setGeometry( QRect(QPoint(0,0), size() ));
//	  break;
       }
    }
#endif
   }
}

void PMDockArea::setMainDockWidget( PMDockWidget* mdw )
{
	if ( mainDockWidget == mdw ) return;
	mainDockWidget = mdw;
	mdw->applyToWidget(this);
}


#endif


#ifndef NO_INCLUDE_MOCFILES // for Qt-only projects, because tmake doesn't take this name
#include "pmdockwidget.moc"
#endif
