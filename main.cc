#include <iostream>
#include <kuniqueapplication.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>
#include <kglobalaccel.h>
#include <kactioncollection.h>

#include "kwingrid.h"

/* Die optimalen werte für hgap und vgap berechnen sich zu:
 *
 * gap = size%split + n*split
 *
 * für n = 0,1,2,... und size die Bildschirmgröße in der jeweiligen Richtung
 */

int main(int argc, char **argv)
{
    KAboutData * aboutdata = new KAboutData("kwingrid",
					    "KWinGrid",
					    ki18n("Window Grid"),
					    "0.1",
					    KLocalizedString(),
					    KAboutData::License_GPL,
					    ki18n("(C) 1999,2000,2002,2004 Stefan Bund"));
    aboutdata->addAuthor(ki18n("Stefan Bund"),ki18n("Developer"),"stefab@j32.de",
			 "http://www.j32.de");
  
    KCmdLineOptions winGridOpts;
    winGridOpts.add("split <width>", ki18n("split"), 0);
    winGridOpts.add("ignorestruts <screen>", ki18n("ignorestruts"), "");
    winGridOpts.add("+hgap", ki18n("hgap"), 0);
    winGridOpts.add("+vgap", ki18n("vgap"), 0);
    winGridOpts.add("+hsplit", ki18n("hsplit"), 0);
    winGridOpts.add("+vsplit", ki18n("vsplit"), 0);

    KCmdLineArgs::init(argc, argv, aboutdata);
    KCmdLineArgs::addCmdLineOptions(winGridOpts);
    KUniqueApplication::addCmdLineOptions();

    if (! KUniqueApplication::start()) {
	kdError() << "KWinGrid is already running!" << endl;
	return 0;
    }

    KApplication * app = new KUniqueApplication;
    KCmdLineArgs * args = KCmdLineArgs::parsedArgs();

    if (args->count()!=4) {
	kdError() << "Invalid arguments. Try --help\n";
	return 0;
    }

    int split = 0;
    int ignorestruts = -1;
    if (args->isSet("split"))
	split = args->getOption("split").toInt();
    
    if (args->isSet("ignorestruts"))
	ignorestruts = args->getOption("ignorestruts").toInt();
    
    if (args->count() != 4) {
	std::cerr << "invalid number of arguments" << std::endl;
	return 1;
    }
    int hgap = args->arg(0).toInt();
    int vgap = args->arg(1).toInt();
    int hsplit = args->arg(2).toInt();
    int vsplit = args->arg(3).toInt();

    args->clear();

    KWinGrid * winGrid = new KWinGrid(hgap,vgap,hsplit,vsplit,split,ignorestruts);

    KActionCollection * actions = new KActionCollection(winGrid);
    
    KAction * move_TL = new KAction(winGrid);
    actions->addAction("Move top-left", move_TL);
    move_TL->setHelpText("Move active window top-left");
    move_TL->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_I, Qt::META+Qt::SHIFT+Qt::Key_I));
    QObject::connect(move_TL, SIGNAL(triggered(bool)), winGrid, SLOT(move_TL()));

    KAction * move_TR = new KAction(winGrid);
    actions->addAction("Move top-right", move_TR);
    move_TR->setHelpText("Move active window top-right");
    move_TR->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_O, Qt::META+Qt::SHIFT+Qt::Key_O));
    QObject::connect(move_TR, SIGNAL(triggered(bool)), winGrid, SLOT(move_TR()));

    KAction * move_BL = new KAction(winGrid);
    actions->addAction("Move bottom-left", move_BL);
    move_BL->setHelpText("Move active window bottom-left");
    move_BL->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_K, Qt::META+Qt::SHIFT+Qt::Key_K));
    QObject::connect(move_BL, SIGNAL(triggered(bool)), winGrid, SLOT(move_BL()));

    KAction * move_BR = new KAction(winGrid);
    actions->addAction("Move bottom-right", move_BR);
    move_BR->setHelpText("Move active window bottom-right");
    move_BR->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_L, Qt::META+Qt::SHIFT+Qt::Key_L));
    QObject::connect(move_BR, SIGNAL(triggered(bool)), winGrid, SLOT(move_BR()));

    KAction * resize_Q = new KAction(winGrid);
    actions->addAction("Resize quarter", resize_Q);
    resize_Q->setHelpText("Resize quarter");
    resize_Q->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_I, Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_I));
    QObject::connect(resize_Q, SIGNAL(triggered(bool)), winGrid, SLOT(resize_Q()));

    KAction * resize_H = new KAction(winGrid);
    actions->addAction("Resize horizontal", resize_H);
    resize_H->setHelpText("Resize horizontal");
    resize_H->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_O, Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_O));
    QObject::connect(resize_H, SIGNAL(triggered(bool)), winGrid, SLOT(resize_H()));

    KAction * resize_V = new KAction(winGrid);
    actions->addAction("Resize vertical", resize_V);
    resize_V->setHelpText("Resize vertical");
    resize_V->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_K, Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_K));
    QObject::connect(resize_V, SIGNAL(triggered(bool)), winGrid, SLOT(resize_V()));

    KAction * resize_F = new KAction(winGrid);
    actions->addAction("Resize full", resize_F);
    resize_F->setHelpText("Resize full");
    resize_F->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_L, Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_L));
    QObject::connect(resize_F, SIGNAL(triggered(bool)), winGrid, SLOT(resize_F()));

    KAction * move_L = new KAction(winGrid);
    actions->addAction("Move left", move_L);
    move_L->setHelpText("Move left");
    move_L->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_Left, Qt::META+Qt::SHIFT+Qt::Key_Left));
    QObject::connect(move_L, SIGNAL(triggered(bool)), winGrid, SLOT(move_L()));

    KAction * move_R = new KAction(winGrid);
    actions->addAction("Move right", move_R);
    move_R->setHelpText("Move right");
    move_R->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_Right, Qt::META+Qt::SHIFT+Qt::Key_Right));
    QObject::connect(move_R, SIGNAL(triggered(bool)), winGrid, SLOT(move_R()));

    KAction * move_U = new KAction(winGrid);
    actions->addAction("Move up", move_U);
    move_U->setHelpText("Move up");
    move_U->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_Up, Qt::META+Qt::SHIFT+Qt::Key_Up));
    QObject::connect(move_U, SIGNAL(triggered(bool)), winGrid, SLOT(move_U()));

    KAction * move_D = new KAction(winGrid);
    actions->addAction("Move down", move_D);
    move_D->setHelpText("Move down");
    move_D->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::SHIFT+Qt::Key_Down, Qt::META+Qt::SHIFT+Qt::Key_Down));
    QObject::connect(move_D, SIGNAL(triggered(bool)), winGrid, SLOT(move_D()));

    KAction * resize_IH = new KAction(winGrid);
    actions->addAction("Increase horizontal size", resize_IH);
    resize_IH->setHelpText("Increase horizontal size");
    resize_IH->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Right,Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_Right));
    QObject::connect(resize_IH, SIGNAL(triggered(bool)), winGrid, SLOT(resize_IH()));

    KAction * resize_IV = new KAction(winGrid);
    actions->addAction("Increase vertical size", resize_IV);
    resize_IV->setHelpText("Increase vertical size");
    resize_IV->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Down,Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_Down));
    QObject::connect(resize_IV, SIGNAL(triggered(bool)), winGrid, SLOT(resize_IV()));

    KAction * resize_DH = new KAction(winGrid);
    actions->addAction("Decrease horizontal size", resize_DH);
    resize_DH->setHelpText("Decrease horizontal size");
    resize_DH->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Left,Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_Left));
    QObject::connect(resize_DH, SIGNAL(triggered(bool)), winGrid, SLOT(resize_DH()));

    KAction * resize_DV = new KAction(resize_DV);
    actions->addAction("Decrease vertical size", resize_DV);
    resize_DV->setHelpText("Decrease vertical size");
    resize_DV->setGlobalShortcut(KShortcut(
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Up,Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_Up));
    QObject::connect(resize_DV, SIGNAL(triggered(bool)), winGrid, SLOT(resize_DV()));

    int ret = app->exec();

    delete app;
    return ret;
}

