// -*- c++ -*-

#ifndef __kwingrid_h
#define __kwingrid_h 1

#include <Xlib.h>
#include <Xutil.h>

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QRect>

class KWinGrid : public QObject
{
    Q_OBJECT

public:
    KWinGrid(int hgap__, int vgap__, int hsplit__, int vsplit__, int split__=0,
             int ignorestruts__=-1,
             int reserveNorth__=0, int reserveSouth__=0, int reserveWest__=0, int reserveEast__=0,
             int southstrut__=0, int strutscreen__=-2);

    virtual void move(int __xslot, int __yslot);
    virtual void resize(int __xsize, int __ysize);
    virtual void moveResize(int __xslot, int __yslot,
                            int __xsize, int __ysize);
    virtual void moveRelative(int __xdiff, int __ydiff);
    virtual void resizeRelative(int __xdiff, int __ydiff);
    virtual void toScreen(int screen);
    virtual void toDesk(int __desk);

    virtual void quit();

    virtual int activeWindow();

public slots:
    void move_TL();
    void move_TR();
    void move_BL();
    void move_BR();

    void resize_Q();
    void resize_H();
    void resize_V();
    void resize_F();

    void move32_00();
    void move32_10();
    void move32_20();
    void move32_01();
    void move32_11();
    void move32_21();
    void resize32_00();
    void resize32_10();
    void resize32_20();
    void resize32_01();
    void resize32_11();
    void resize32_21();

    void move43_00();
    void move43_10();
    void move43_20();
    void move43_30();
    void move43_01();
    void move43_11();
    void move43_21();
    void move43_31();
    void move43_02();
    void move43_12();
    void move43_22();
    void move43_32();
    void resize43_00();
    void resize43_10();
    void resize43_20();
    void resize43_30();
    void resize43_01();
    void resize43_11();
    void resize43_21();
    void resize43_31();
    void resize43_02();
    void resize43_12();
    void resize43_22();
    void resize43_32();

    void move_L();
    void move_R();
    void move_U();
    void move_D();

    void resize_IH();
    void resize_DH();
    void resize_IV();
    void resize_DV();

    void move_Screen0();
    void move_Screen1();

public slots:
    void activeWindowChanged(WId id);

private:
    void initGeometry(int __forceScreens=-1);
    void updateGeometry(QRect& __new);
    void applyGeometry();
    QRect doMoveResize(int __xslot, int __yslot,
                       int __xsize, int __ysize);
    void moveSlot(int nx, int ny, int posx, int posy);
    void resizeSlot(int nx, int ny, int szx, int szy);

    void updateTimestamp(void);

    int split_;
    int ignorestruts_;
    int reserveNorth_;
    int reserveSouth_;
    int reserveEast_;
    int reserveWest_;

    int activeWindow_;
    QRect inner_;
    QRect outer_;
    QRect orig_;
    QRect region_;
    XSizeHints hints_;
    int screen_;
    int numScreens_;

    int hsize_;
    int vsize_;

    int hgap_;
    int vgap_;
    int hsplit_;
    int vsplit_;

    int southstrut_;
    int strutscreen_;

    QDateTime timestamp_;
};

#endif
