#include <kapplication.h>
#include <dcopclient.h>
#include <kwin.h>
#include <Xlib.h>

#include "kwingrid.h"
#include "kwingrid.moc"

KWinGrid::KWinGrid(int hgap__, int vgap__, int hsplit__, int vsplit__)
    : DCOPObject("grid"), hgap_(hgap__), vgap_(vgap__), hsplit_(hsplit__), vsplit_(vsplit__)
{
    module_ = new KWinModule(KApplication::kApplication());
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
    int w = module_->activeWindow();
    if (w) 
	KWin::setOnDesktop(w,__desk);
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
	if (xSlot<0)
	    xSlot = 0;
	else if (xSlot >= hsplit_)
	    xSlot = hsplit_-1;
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
	
    if (__xsize != -1) 
	newGeometry.setWidth(__xsize*hsize_-hgap_);
    if (__ysize != -1)
	newGeometry.setHeight(__ysize*vsize_-vgap_);
    QSize size = newGeometry.size();
    if (__xslot != -1) {
	QPoint p(newGeometry.topLeft());
	p.setX(region_.left() 
	       + __xslot*hsize_
	       + hgap_/2);
	newGeometry.moveTopLeft(p);
    }
    if (__yslot != -1) {
	QPoint p(newGeometry.topLeft());
	p.setY(region_.top()
	       + __yslot*vsize_
	       + vgap_/2);
	newGeometry.moveTopLeft(p);
    }
	
    return newGeometry;
}

void KWinGrid::initGeometry()
{
    activeWindow_ = module_->activeWindow();
    if (activeWindow_) {
	KWin::Info info(KWin::info(activeWindow_));
	inner_ = info.geometry;
	outer_ = info.frameGeometry;
	region_ = module_->workArea();

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
    newInner.moveTopLeft(QPoint(__new.top()+(inner_.top()-outer_.top()),
				__new.left()+(inner_.left()-outer_.left())));
    newInner.setSize(QSize(__new.width()-(outer_.width()-inner_.width()),
			   __new.height()-(outer_.height()-inner_.height())));
    inner_ = newInner;
    outer_ = __new;
}

void KWinGrid::applyGeometry()
{
    XMoveResizeWindow(KApplication::kApplication()->getDisplay(),activeWindow_,
		      outer_.x(),outer_.y(),
		      inner_.width(),inner_.height());
    KWin::setActiveWindow(activeWindow_);
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

