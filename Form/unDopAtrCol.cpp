#include "DB_bkMetaDef.h"
#include "unDopAtrCol.h"


void TDopAtrCol::SetDopAtrColNull() {
    is_UstGoda = false;
    is_set_god = 0;
    god_plus = 0;
    god_ravno = 0;
    is_UstPeriod = false;
    Period = 0;
    ValuePeriod = 0;
    is_UstIstFin = false;
    ist_fin_grad_id = 0;
    is_planId = false;
    plan_id = 0;
    H = 0;
    Is_bold = 0;
    FontColor = 0;
    fonColor = 16777215;
    is_NameReplValue = false;
    NameReplValue = "";
    colReplValue = 0;
    isVPredelaxDiapazonaKBK = false;

    // --wOwnerNomRow = 0;
    // --wCurNomRow = 0;
}

void TDopAtrCol::SetDopAtrColQuery(TQueryUni* aQ) {
    SetDopAtrColNull();
    if (!aQ->FieldByName("is_set_god")->IsNull()) {
        is_UstGoda = true;
        is_set_god = aQ->FieldByName("is_set_god")->AsInteger();
        god_plus = aQ->FieldByName("god_plus")->AsInteger();
        god_ravno = aQ->FieldByName("god_ravno")->AsInteger();
    }
    if (!aQ->FieldByName("Period")->IsNull()) {
        is_UstPeriod = true;
        Period = aQ->FieldByName("Period")->AsInteger();
        ValuePeriod = aQ->FieldByName("ValuePeriod")->AsInteger();
    }
    if (aQ->FieldByName("is_ist_fin")->AsInteger() == 1) {
        is_UstIstFin = true;
        ist_fin_grad_id = aQ->FieldByName("ist_fin_grad_id")->AsInteger();
    }
    if (aQ->FieldByName("plan_id")->AsInteger() > 0) {
        is_planId = true;
        plan_id = aQ->FieldByName("plan_id")->AsInteger();
    }
    font = aQ->FieldByName("font")->AsString();
    H = aQ->FieldByName("h")->AsInteger();
    Is_bold = aQ->FieldByName("is_bold")->AsInteger();
    FontColor = aQ->FieldByName("fontColor")->AsInteger();
    fonColor = aQ->FieldByName("patternBGcolor")->AsInteger();
    if (fonColor == 0)
        fonColor = 16777215;

    if ((aQ->FieldByName("attr_2")->AsInteger() > 0) && ((aQ->FieldByName("stmnt")->AsString() == "КОРТЕЖ") || (aQ->FieldByName("stmnt")->AsString() == "ПОКАЗАТЕЛЬ"))) {
        is_NameReplValue = true;
        NameReplValue = aQ->FieldByName("note1")->AsString();
        colReplValue = aQ->FieldByName("attr_2")->AsInteger();
    }
}


bool TDopAtrCol::compareKBK(TwwStringList* p, std::string aKBK) {
    if (aKBK.empty())
        return false;
    bool b = true;
    PAtrStmnt AtrStmnt = &(p->ListStmnt[isVPredelaxDiapazonaClmn]);
    std::string KBKshablon = AtrStmnt->Shablon;
    for (int i = 0; i < KBKshablon.length(); i++) {
        if (KBKshablon[i] == '%')
            return b;
        if (KBKshablon[i] == '_')
            continue;
        if (KBKshablon[i] != aKBK[i]) {
            b = false;
            break;
        }
    }
    return b;
}

bool checkKBKvid(int vid, std::string aKBK, std::string shablon, int god) {
    int nacalo_vKBK, dlina_vKBK;
    TDB_bkMetaDef::GetVidInfo(vid, nacalo_vKBK, dlina_vKBK, god);
    if (vid > 0 && shablon.length() < (nacalo_vKBK + dlina_vKBK - 1))
        return true;
    for (int i = nacalo_vKBK; i < nacalo_vKBK + dlina_vKBK; i++) {
        if (shablon[i] == '%')
            return true;
        if (shablon[i] == '_')
            continue;
        if (shablon[i] != aKBK[i]) {
            return false;
        }
    }
    return true;
}

bool TDopAtrCol::compareKBKbyStep(std::string aKBK, std::string shablon, int god) {
    if (aKBK.empty())
        return false;
    // проверка для Разд
    if (!checkKBKvid(0, aKBK, shablon, god))
        return false;
    // проверка для Цел ст
    if (!checkKBKvid(1, aKBK, shablon, god))
        return false;
    // проверка для Вида Расх
    if (!checkKBKvid(2, aKBK, shablon, god))
        return false;
    // проверка для Косгу
    if (!checkKBKvid(3, aKBK, shablon, god))
        return false;
    return true;
}

