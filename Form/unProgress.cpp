#include "unProgress.h"

void TunProgress::doShow(TFormLogicF1* FormLogicF1, std::string aCaption) {
#ifndef RestServer
    /*mak FormLogicF1->fmProgress->Caption = aCaption.c_str();
    FormLogicF1->fmProgress->Show();*/
#endif
}

void TunProgress::doHide(TFormLogicF1* FormLogicF1) {
#ifndef RestServer
    /*mak FormLogicF1->fmProgress->Hide();*/
#endif
}

void TunProgress::ShowText(TFormLogicF1* FormLogicF1, std::string aText) {
#ifndef RestServer
    /*mak FormLogicF1->fmProgress->edPokaz->Text = aText.c_str();
    Application_ProcessMessages();*/
#endif
}

void TunProgress::doProcessMessages() {
#ifndef RestServer
    /*mak Application_ProcessMessages();*/
#endif
}
