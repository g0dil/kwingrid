#include <iostream>

#include <kapplication.h>
#include <dcopclient.h>
#include <kwin.h>
#include <Xlib.h>
#include <Xutil.h>
#include <netwm_def.h>

#include "kwingrid.h"
#include "kwingrid.moc"

KWinGrid::KWinGrid(int hgap__, int vgap__, int hsplit__, int vsplit__, int split__)
    : DCOPObject("grid"), split_(split__), 
      hgap_(hgap__), vgap_(vgap__), hsplit_(hsplit__), vsplit_(vsplit__)
{
    module_ = new KWinModule(KApplication::kApplication());
    connect(module_,SIGNAL(activeWindowChanged(WId)),this,SLOT(activeWindowChanged(WId)));
}

void KWinGrid::move(int __xslot, int __yslot)
{
    moveResize(__xslot, __yslot, -1, -1);
}

void KWinGrid::resize(int __xsize, int __ysize)
{
    moveResize(-1,-1,__xsize,__ysize);
}

void KWinGrid::toDesk(int __desk)
{
    int w = activeWindow();
    if (w) 
	KWin::setOnDesktop(w,__desk);
}

void KWinGrid::quit()
{
    KApplication::kApplication()->quit();
}

int KWinGrid::activeWindow()
{
    int av = module_->activeWindow();
    KWin::WindowInfo info = KWin::windowInfo(av,NET::WMWindowType);
    if (info.windowType(NET::AllTypesMask) == NET::Desktop) return 0;
    return av;
}

void KWinGrid::updateTimestamp(void)
{
    timestamp_ = QDateTime::currentDateTime();
}

void KWinGrid::activeWindowChanged(WId id)
{
    if (!activeWindow_ || timestamp_.isNull())
	return;

    QDateTime tm = QDateTime::currentDateTime();

    int deltaDays = timestamp_.date().daysTo(tm.date());
    int deltaMSecs = timestamp_.time().msecsTo(tm.time());
    
    if (deltaDays>2 || deltaDays<0) {
	activeWindow_ = 0;
	return;
    }
    
    deltaMSecs += deltaDays * 1000*(60*60*24);

    if (deltaMSecs <= 300 && deltaMSecs > 0)
	KWin::forceActiveWindow(activeWindow_);
    else
	activeWindow_ = 0;
}

void KWinGrid::moveResize(int __xslot, int __yslot,
			  int __xsize, int __ysize)
{
    initGeometry();
    if (activeWindow_) {
	QRect newGeometry = doMoveResize(__xslot,__yslot,__xsize,__ysize);
	updateGeometry(newGeometry);
	applyGeometry();
    }
}

void KWinGrid::moveRelative(int __xdiff, int __ydiff)
{
    initGeometry();
    if (activeWindow_) {
	int xSlot = (outer_.left()-region_.left()+hsize_/2)/hsize_;
	int ySlot = (outer_.top()-region_.top()+vsize_/2)/vsize_;
	xSlot += __xdiff;
	ySlot += __ydiff;
	if (xSlot<0) {
	    if (numScreens_ > 1 and screen_ > 0) {
		initGeometry(screen_-1);
		xSlot = hsplit_-1;
		ySlot = (outer_.top()-region_.top()+vsize_/2)/vsize_ + __ydiff;
	    } else 
		xSlot = 0;
	} else if (xSlot >= hsplit_) {
	    if (numScreens_ > 1 and screen_ < numScreens_-1) {
		initGeometry(screen_+1);
		xSlot = 0;
		ySlot = (outer_.top()-region_.top()+vsize_/2)/vsize_ + __ydiff;
	    } else
		xSlot = hsplit_-1;
	}
	if (ySlot<0)
	    ySlot = 0;
	else if (ySlot >= vsplit_)
	    ySlot = vsplit_-1;
	QRect newGeometry = doMoveResize(xSlot,ySlot,-1,-1);
	updateGeometry(newGeometry);
	applyGeometry();
    }
}

void KWinGrid::resizeRelative(int __xdiff, int __ydiff)
{
    initGeometry();
    if (activeWindow_) {
	int xSize = (outer_.width()+hsize_/2)/hsize_;
	int ySize = (outer_.height()+vsize_/2)/vsize_;
	xSize += __xdiff;
	ySize += __ydiff;
	if (xSize<1)
	    xSize = 1;
	if (xSize>hsplit_)
	    xSize = hsplit_;
	if (ySize<1)
	    ySize = 1;
	if (ySize>vsplit_)
	    ySize = vsplit_;
	QRect newGeometry = doMoveResize(-1,-1,xSize,ySize);
	updateGeometry(newGeometry);
	applyGeometry();
    }
}

QRect KWinGrid::doMoveResize(int __xslot, int __yslot,
			     int __xsize, int __ysize)
{
    QRect newGeometry(outer_);
    
    if (__xsize == -1) {
	__xsize = (outer_.width()+hsize_/2)/hsize_;
	if (__xsize<1) __xsize = 1;
	if (__xsize>hsplit_) __xsize = hsplit_;
    }
    if (__ysize == -1) {
	__ysize = (outer_.height()+vsize_/2)/vsize_;
	if (__ysize<1) __ysize = 1;
	if (__ysize>vsplit_) __ysize = vsplit_;
    }
    
    newGeometry.setWidth(__xsize*hsize_-hgap_);
    newGeometry.setHeight(__ysize*vsize_-vgap_);

    if (__xslot == -1) {
	__xslot = (outer_.left()-region_.left()+hsize_/2)/hsize_;
	if (__xslot<0) __xslot = 0;
	if (__xslot>=hsplit_) __xslot = hsplit_-1;
    }
    if (__yslot == -1) {
	__yslot = (outer_.top()-region_.top()+vsize_/2)/vsize_;
	if (__yslot<0) __yslot = 0;
	if (__yslot>=vsplit_) __yslot = vsplit_-1;
    }
    newGeometry.moveTopLeft(QPoint(region_.left() + __xslot*hsize_ + hgap_/2,
				   region_.top() + __yslot*vsize_ + vgap_/2));
    return newGeometry;
}

std::ostream& operator<<(std::ostream& os, QRect rect)
{
    os << '(' << rect.width() << 'x' << rect.height() << '+' << rect.x() << '+' << rect.y() << ')';
    return os;
}

std::ostream& operator<<(std::ostream& os, QPoint p)
{
    os << '(' << p.x() << ',' << p.y() << ')';
    return os;
}

std::ostream& operator<<(std::ostream& os, QSize s)
{
    os << '(' << s.width() << 'x' << s.height() << ')';
    return os;
}

void KWinGrid::initGeometry(int __forceScreen)
{
    activeWindowChanged(0);
    if (activeWindow_ == 0)
	activeWindow_ = activeWindow();
    if (activeWindow_) {
	KWin::WindowInfo info(KWin::windowInfo(activeWindow_));
	inner_ = info.geometry();
	outer_ = info.frameGeometry();
	orig_ = outer_;
	if (split_) {
	    if (__forceScreen == -1)
		screen_ = outer_.left()>=split_ ? 1 : 0;
	    else
		screen_ = __forceScreen;
	    region_ = QApplication::desktop()->screenGeometry();
	    if (screen_ == 1)
		region_.setLeft(split_);
	    else
		region_.setRight(split_-1);
	    numScreens_ = 2;
	} else {
	    if (__forceScreen == -1)
		screen_ = QApplication::desktop()->screenNumber(outer_.topLeft());
	    else
		screen_ = __forceScreen;
	    region_ = QApplication::desktop()->screenGeometry(screen_);
	    numScreens_ = QApplication::desktop()->numScreens();
	}
	QRect wa = module_->workArea();
	region_ = region_ & wa;

	hsize_ = (region_.width()-hgap_)/hsplit_;
	vsize_ = (region_.height()-vgap_)/vsplit_;

	int hdelta = region_.width()-hsize_*hsplit_;
	int vdelta = region_.height()-vsize_*vsplit_;
	QPoint topLeft(region_.topLeft());
	topLeft+=QPoint(hdelta/2,vdelta/2);
	region_.moveTopLeft(topLeft);
	region_.setSize(QSize(hsize_*hsplit_,vsize_*vsplit_));
    }
}

void KWinGrid::updateGeometry(QRect& __new)
{
    QRect newInner(inner_);
    newInner.moveTopLeft(QPoint(__new.left()+(inner_.left()-outer_.left()),
				__new.top()+(inner_.top()-outer_.top())));
    newInner.setSize(QSize(__new.width()-(outer_.width()-inner_.width()),
			   __new.height()-(outer_.height()-inner_.height())));
    inner_ = newInner;
    outer_ = __new;

    XSizeHints hints;
    long supplied;
    if (XGetWMNormalHints(KApplication::kApplication()->getDisplay(), 
			  activeWindow_, &hints, &supplied)) {
	hints.flags &= supplied;
	if (hints.flags & PResizeInc && hints.width_inc != 0 && hints.height_inc != 0) {
	    QSize base(0,0);
	    if (hints.flags & PBaseSize) {
		base.setWidth(hints.base_width);
		base.setHeight(hints.base_height);
	    } else if (hints.flags & PMinSize) {
		base.setWidth(hints.min_width);
		base.setHeight(hints.min_height);
	    }
	    QSize newSize(((inner_.width()-base.width())/hints.width_inc)*hints.width_inc + base.width(),
			  ((inner_.height()-base.height())/hints.height_inc)*hints.height_inc + base.height());
	    QSize delta(inner_.size() - newSize);
	    QPoint offset(delta.width()/2,delta.height()/2);
	    inner_.setSize(newSize);
	    outer_.setSize(outer_.size() - delta);
	    inner_.moveTopLeft(inner_.topLeft() + offset);
	    outer_.moveTopLeft(outer_.topLeft() + offset);
	}
    }
}

void KWinGrid::applyGeometry()
{
    updateTimestamp();
    if (orig_.topLeft() == outer_.topLeft())
	// If the position of the window did not change,
	// XMoveResizeWindow sometimes still moves the window a little
	// bit. Seems to have something todo with window gravity
	// ... we just leave the position allone in that case.
	XResizeWindow(KApplication::kApplication()->getDisplay(),activeWindow_,
		      inner_.width(),inner_.height());
    else {
	// I don't really know, whats all this stuff concerning window
	// gravity. I only know, this works for my openoffice windows,
	// which have StaticGravity. I have not found any window with
	// a window_gravity of neither StaticGravity nor
	// NorthWestGravity on my desktop so did not check other
	// window gravities.
	QPoint pos = outer_.topLeft();
	XWindowAttributes winAttributes;
	if (XGetWindowAttributes(KApplication::kApplication()->getDisplay(),activeWindow_, 
				 &winAttributes) && winAttributes.win_gravity == StaticGravity)
	    pos = inner_.topLeft();
	XMoveResizeWindow(KApplication::kApplication()->getDisplay(),activeWindow_,
			  pos.x(),pos.y(), inner_.width(),inner_.height());
    }
    //XSync(KApplication::kApplication()->getDisplay(),False);
    //KWin::forceActiveWindow(activeWindow_);
}

// slots

void KWinGrid::move_TL()
{
    move(0,0);
}

void KWinGrid::move_TR()
{
    move(hsplit_/2,0);
}

void KWinGrid::move_BL()
{
    move(0,vsplit_/2);
}

void KWinGrid::move_BR()
{
    move(hsplit_/2,vsplit_/2);
}

    
void KWinGrid::resize_Q()
{
    resize(vsplit_/2,hsplit_/2);
}

void KWinGrid::resize_H()
{
    resize(vsplit_,hsplit_/2);
}

void KWinGrid::resize_V()
{
    resize(vsplit_/2,hsplit_);
}

void KWinGrid::resize_F()
{
    resize(vsplit_,hsplit_);
}


void KWinGrid::move_L()
{
    moveRelative(-1,0);
}

void KWinGrid::move_R()
{
    moveRelative(1,0);
}

void KWinGrid::move_U()
{
    moveRelative(0,-1);
}

void KWinGrid::move_D()
{
    moveRelative(0,1);
}

    
void KWinGrid::resize_IH()
{
    resizeRelative(1,0);
}

void KWinGrid::resize_DH()
{
    resizeRelative(-1,0);
}

void KWinGrid::resize_IV()
{
    resizeRelative(0,1);
}

void KWinGrid::resize_DV()
{
    resizeRelative(0,-1);
}

