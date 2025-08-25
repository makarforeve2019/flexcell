
#ifndef UNDOPATRCOL_H
#define UNDOPATRCOL_H

#include "DB_Common.h"
#include "FormTypes.h"


struct TDopAtrCol {
    bool is_UstGoda;
    unsigned char is_set_god;
    int god_plus;
    int god_ravno;

    bool is_UstPeriod;
    int Period;
    int ValuePeriod;

    bool is_UstIstFin;
    int ist_fin_grad_id;

    bool is_planId;
    int plan_id;

    std::string font;
    unsigned char H;
    unsigned char Is_bold;
    int FontColor;
    int fonColor;

    bool is_NameReplValue;
    std::string NameReplValue;
    int colReplValue;

    bool isVPredelaxDiapazonaKBK;
    unsigned char isVPredelaxDiapazonaClmn;

    unsigned char OtkudaOwner;
    int owner_id_1N;
    int class_idItems;
    bool isIgnor;
    int readonly;
    int is_hidden;
    int is_filter;
    int is_filter2;
    int ColOnFilter2;

    void SetDopAtrColNull();
    void SetDopAtrColQuery(TQueryUni* aQ);
    bool compareKBK(TwwStringList* p, std::string aKBK);
    bool compareKBKbyStep(std::string aKBK, std::string shablon, int god);
};
typedef TDopAtrCol* PDopAtrCol;
#endif // UNDOPATRCOL_H