#ifndef __kwingrid_iface_h
#define __kwingrid_iface_h

#include <qstring.h>
#include <dcopobject.h>

class KWinGrid_Iface : virtual public DCOPObject
{
    K_DCOP
public:
    virtual ~KWinGrid_Iface() {}

k_dcop:
    virtual void move(int __xslot, int __yslot) = 0;
    virtual void resize(int __xsize, int __ysize) = 0;
    virtual void moveResize(int __xslot, int __yslot, 
			    int __xsize, int __ysize) = 0;
    virtual void moveRelative(int __xdiff, int __ydiff) = 0;
    virtual void resizeRelative(int __xdiff, int __ydiff) = 0;
    virtual void toDesk(int __desk) = 0;

    virtual void quit() = 0;
    
    virtual int activeWindow() = 0;
};

#endif
