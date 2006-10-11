#include <stdlib.h>
#include <kuniqueapplication.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>
#include <kglobalaccel.h>
#include <qnamespace.h>

#include "kwingrid.h"

static KCmdLineOptions winGridOpts[] =
{
    { "+hgap", I18N_NOOP("hgap"), 0 },
    { "+vgap", I18N_NOOP("vgap"), 0 },
    { "+hsplit", I18N_NOOP("hsplit"), 0 },
    { "+vsplit", I18N_NOOP("vsplit"), 0 },
    { 0,0,0 }
};

/* Die optimalen werte für hgap und vgap berechnen sich zu:
 *
 * gap = size % split + n * split
 *
 * für n = 0,1,2,... und size die Bildschirmgröße in der jeweiligen Richtung
 */

int main(int argc, char **argv)
{
    KAboutData * aboutdata = new KAboutData("kwingrid","KWinGrid","0.5",
					    I18N_NOOP("Window Grid"),
					    KAboutData::License_GPL,
					    "(C) 1999,2000,2002, Stefan Bund",
					    "", "http://home.t-online.de/home/Stefan.Bund/");
    aboutdata->addAuthor("Stefan Bund",I18N_NOOP("Developer"),"asbund@gmx.de",
			 "http://home.t-online.de/home/Stefan.Bund");
  
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
  
    int hgap = atoi(args->arg(0));
    int vgap = atoi(args->arg(1));
    int hsplit = atoi(args->arg(2));
    int vsplit = atoi(args->arg(3));

    args->clear();

    KWinGrid * winGrid = new KWinGrid(hgap,vgap,hsplit,vsplit);

    KGlobalAccel * accel = new KGlobalAccel(app);

    accel->insert("move_TL",
		  I18N_NOOP("Move top-left"),
		  I18N_NOOP("Move active window top-left"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_I, KKey::QtWIN+Qt::SHIFT+Qt::Key_I,
		  winGrid, SLOT ( move_TL() ));
    accel->insert("move_TR",
		  I18N_NOOP("Move top-right"),
		  I18N_NOOP("Move active window top-right"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_O, KKey::QtWIN+Qt::SHIFT+Qt::Key_O,
		  winGrid, SLOT ( move_TR() ));
    accel->insert("move_BL",
		  I18N_NOOP("Move bottom-left"),
		  I18N_NOOP("Move active window bottom-left"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_K, KKey::QtWIN+Qt::SHIFT+Qt::Key_K,
		  winGrid, SLOT ( move_BL() ));
    accel->insert("move_BR",
		  I18N_NOOP("Move bottom-right"),
		  I18N_NOOP("Move active window bottom-right"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_L, KKey::QtWIN+Qt::SHIFT+Qt::Key_L,
		  winGrid, SLOT ( move_BR() ));

    accel->insert("resize_Q",
		  I18N_NOOP("Resize quarter"), I18N_NOOP("Resize quarter"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_I, KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_I,
		  winGrid, SLOT ( resize_Q() ));
    accel->insert("resize_H",
		  I18N_NOOP("Resize horizontal"), I18N_NOOP("Resize horizontal"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_O, KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_O,
		  winGrid, SLOT ( resize_H() ));
    accel->insert("resize_V",
		  I18N_NOOP("Resize vertical"), I18N_NOOP("Resize vertical"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_K, KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_K,
		  winGrid, SLOT ( resize_V() ));
    accel->insert("resize_F",
		  I18N_NOOP("Resize full"), I18N_NOOP("Resize full"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_L, KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_L,
		  winGrid, SLOT ( resize_F() ));

    accel->insert("move_L",
		  I18N_NOOP("Move left"), I18N_NOOP("Move left"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_Left, KKey::QtWIN+Qt::SHIFT+Qt::Key_Left,
		  winGrid, SLOT ( move_L() ));
    accel->insert("move_R",
		  I18N_NOOP("Move right"), I18N_NOOP("Move right"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_Right, KKey::QtWIN+Qt::SHIFT+Qt::Key_Right,
		  winGrid, SLOT ( move_R() ));
    accel->insert("move_U",
		  I18N_NOOP("Move up"), I18N_NOOP("Move up"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_Up, KKey::QtWIN+Qt::SHIFT+Qt::Key_Up,
		  winGrid, SLOT ( move_U() ));
    accel->insert("move_D",
		  I18N_NOOP("Move down"), I18N_NOOP("Move down"),
		  Qt::ALT+Qt::SHIFT+Qt::Key_Down, KKey::QtWIN+Qt::SHIFT+Qt::Key_Down,
		  winGrid, SLOT ( move_D() ));

    accel->insert("resize_IH",
		  I18N_NOOP("Increase horizontal size"), I18N_NOOP("Increase horizontal size"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Right,KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_Right,
		  winGrid, SLOT ( resize_IH() ));
    accel->insert("resize_IV",
		  I18N_NOOP("Increase vertical size"), I18N_NOOP("Increase vertical size"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Down,KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_Down,
		  winGrid, SLOT ( resize_IV() ));
    accel->insert("resize_DH",
		  I18N_NOOP("Decrease horizontal size"), I18N_NOOP("Decrease horizontal size"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Left,KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_Left,
		  winGrid, SLOT ( resize_DH() ));
    accel->insert("resize_DV",
		  I18N_NOOP("Decrease vertical size"), I18N_NOOP("Decrease vertical size"),
		  Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_Up,KKey::QtWIN+Qt::CTRL+Qt::SHIFT+Qt::Key_Up,
		  winGrid, SLOT ( resize_DV() ));
    
    
    accel->updateConnections();


    int ret = app->exec();

    delete app;
    return ret;
}

