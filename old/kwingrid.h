// -*- c++ -*-

#ifndef __kwingrid_h
#define __kwingrid_h 1

#include <qobject.h>
#include <kwinmodule.h>
#include <kwingrid_iface.h>

class KWinGrid 
    : public QObject, public virtual KWinGrid_Iface
{
    Q_OBJECT

public:
    KWinGrid(int hgap__, int vgap__, int hsplit__, int vsplit__);
    
    virtual void move(int __xslot, int __yslot);
    virtual void resize(int __xsize, int __ysize);
    virtual void moveResize(int __xslot, int __yslot, 
			    int __xsize, int __ysize);
    virtual void moveRelative(int __xdiff, int __ydiff);
    virtual void resizeRelative(int __xdiff, int __ydiff);
    virtual void toDesk(int __desk);

public slots:
    void move_TL();
    void move_TR();
    void move_BL();
    void move_BR();
    
    void resize_Q();
    void resize_H();
    void resize_V();
    void resize_F();

    void move_L();
    void move_R();
    void move_U();
    void move_D();
    
    void resize_IH();
    void resize_DH();
    void resize_IV();
    void resize_DV();

private:
    void initGeometry();
    void updateGeometry(QRect& __new);
    void applyGeometry();
    QRect doMoveResize(int __xslot, int __yslot, 
		       int __xsize, int __ysize);
    
    KWinModule* module_;
    DCOPClient* client_;

    int activeWindow_;
    QRect inner_;
    QRect outer_;
    QRect region_;

    int hsize_;
    int vsize_;

    int hgap_;
    int vgap_;
    int hsplit_;
    int vsplit_;
};

#endif
