#ifndef UNPROGRESS_H
#define UNPROGRESS_H

#include <string>

#ifdef RestServer
#define Forms_H
#else
#include "../Form/FormDraw/unFormLogicF1.h"
#endif

class TunProgress {
public:
    static void doShow(TFormLogicF1* FormLogicF1, std::string aCaption);
    static void doHide(TFormLogicF1* FormLogicF1);
    static void ShowText(TFormLogicF1* FormLogicF1, std::string aText);
    static void doProcessMessages();
};

#endif // UNPROGRESS_H
