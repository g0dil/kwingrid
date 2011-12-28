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
    winGridOpts.add("ignorestruts <screen>", ki18n("ignorestruts. -1 = all screens"), "");
    winGridOpts.add("reserve-north <n>", ki18n("reserve north"), "");
    winGridOpts.add("reserve-south <n>", ki18n("reserve south"), "");
    winGridOpts.add("reserve-west <n>", ki18n("reserve west"), "");
    winGridOpts.add("reserve-east <n>", ki18n("reserve east"), "");
    winGridOpts.add("southstrut <n>", ki18n("set south strut size"), "");
    winGridOpts.add("strutscreen <n>", ki18n("set screen to apply manual strut to. -1 = all screens"), "");
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
    int ignorestruts = -2;
    if (args->isSet("split"))
        split = args->getOption("split").toInt();

    if (args->isSet("ignorestruts"))
        ignorestruts = args->getOption("ignorestruts").toInt();

    if (args->count() != 4) {
        std::cerr << "invalid number of arguments" << std::endl;
        return 1;
    }

    int southstrut (0);
    int strutscreen (-2);
    if (args->isSet("southstrut"))
        southstrut = args->getOption("southstrut").toInt();
    if (args->isSet("strutscreen"))
        strutscreen = args->getOption("strutscreen").toInt();

    int rn = 0;
    int rs = 0;
    int re = 0;
    int rw = 0;

    if (args->isSet("reserve-north"))
        rn = args->getOption("reserve-north").toInt();
    if (args->isSet("reserve-south"))
        rs = args->getOption("reserve-south").toInt();
    if (args->isSet("reserve-west"))
        rw = args->getOption("reserve-west").toInt();
    if (args->isSet("reserve-east"))
        re = args->getOption("reserve-east").toInt();

    int hgap = args->arg(0).toInt();
    int vgap = args->arg(1).toInt();
    int hsplit = args->arg(2).toInt();
    int vsplit = args->arg(3).toInt();

    args->clear();

    KWinGrid * winGrid = new KWinGrid(hgap,vgap,hsplit,vsplit,split,ignorestruts,
                                      rn, rs, rw, re, southstrut, strutscreen);

    KActionCollection * actions = new KActionCollection(winGrid);

#define ACTION(pos, key, slot)                                          \
    KAction * slot = new KAction(winGrid);                              \
    actions->addAction("Move " pos, slot);                              \
    slot->setHelpText("Move active window" pos);                        \
    slot->setGlobalShortcut(KShortcut(                                  \
        Qt::ALT+Qt::SHIFT+Qt::Key_ ## key, Qt::META+Qt::SHIFT+Qt::Key_ ## key)); \
    QObject::connect(slot, SIGNAL(triggered(bool)), winGrid, SLOT(slot()))

    ACTION("top-left", I, move_TL);
    ACTION("top-right", O, move_TR);
    ACTION("bottom-left", K, move_BL);
    ACTION("bottom-right", L, move_BR);

    ACTION("top-left (3x2 grid)", T, move_00);
    ACTION("top-middle (3x2 grid)", Y, move_10);
    ACTION("top-right (3x2 grid)", U, move_20);
    ACTION("bottom-left (3x2 grid)", G, move_01);
    ACTION("bottom-middle (3x2 grid)", H, move_11);
    ACTION("bottom-right (3x2 grid)", J, move_21);

#undef ACTION
#define ACTION(size, key, slot)                                         \
    KAction * slot = new KAction(winGrid);                              \
    actions->addAction("Resize " size, slot);                           \
    slot->setHelpText("Resize " size);                                  \
    slot->setGlobalShortcut(KShortcut(                                  \
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_ ## key, Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_ ## key)); \
    QObject::connect(slot, SIGNAL(triggered(bool)), winGrid, SLOT(slot()));

    ACTION("quarter", I, resize_Q);

    ACTION("horizontal", O, resize_H);
    ACTION("vertical", K, resize_V);
    ACTION("full", L, resize_F);

    ACTION("top-left (3x2 grid)", T, resize_00);
    ACTION("top-middle (3x2 grid)", Y, resize_10);
    ACTION("top-right (3x2 grid)", U, resize_20);
    ACTION("bottom-left (3x2 grid)", G, resize_01);
    ACTION("bottom-middle (3x2 grid)", H, resize_11);
    ACTION("bottom-right (3x2 grid)", J, resize_21);

#undef ACTION
#define ACTION(dir, key, slot)                                          \
    KAction * slot = new KAction(winGrid);                              \
    actions->addAction("Move " dir, slot);                              \
    slot->setHelpText("Move " dir);                                     \
    slot->setGlobalShortcut(KShortcut(                                  \
        Qt::ALT+Qt::SHIFT+Qt::Key_ ## key, Qt::META+Qt::SHIFT+Qt::Key_ ## key)); \
    QObject::connect(slot, SIGNAL(triggered(bool)), winGrid, SLOT(slot()));

    ACTION("left", Left, move_L);
    ACTION("right", Right, move_R);
    ACTION("up", Up, move_U);
    ACTION("down", Down, move_D);

#undef ACTION
#define ACTION(resize, key, slot)                                       \
    KAction * slot = new KAction(winGrid);                              \
    actions->addAction(resize " size", slot);                           \
    slot->setHelpText(resize " size");                                  \
    slot->setGlobalShortcut(KShortcut(                                  \
        Qt::ALT+Qt::CTRL+Qt::SHIFT+Qt::Key_ ## key,Qt::META+Qt::CTRL+Qt::SHIFT+Qt::Key_ ## key)); \
    QObject::connect(slot, SIGNAL(triggered(bool)), winGrid, SLOT(slot()));

    ACTION("Increase horizontal", Right, resize_IH);
    ACTION("Increase vertical", Down, resize_IV);
    ACTION("Decrease horizontal", Left, resize_DH);
    ACTION("Decrease vertical", Up, resize_DV);

#undef ACTION

    int ret = app->exec();

    delete app;
    return ret;
}
