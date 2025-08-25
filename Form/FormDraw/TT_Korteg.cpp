#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "unDBType.h"
#include "dmGlobalVars.h"
#include "DB_Pokaztl.h"
#include "TT_Korteg.h"

// создать списки КБК для получ и распоряд счетов
void T_Korteg::CreateListKBK_Plc_Rsp(int CurrYear, int pokaz_idPlc) {
    T_Korteg::CreateListKBK_Plc(CurrYear, pokaz_idPlc);
    T_Korteg::CreateListKBK_Rsp(CurrYear, pokaz_idPlc);
}

// создать списки КБК для получ счетов
void T_Korteg::CreateListKBK_Plc(int CurrYear, int pokaz_idPlc) {
    std::string castName;
    int nPos;

    TDB_Pokaztl::DropTmpPokaztlTB();
    castName = TDB_Pokaztl::GetCastName(pokaz_idPlc);
    nPos = Pos("\\", castName);
    if (nPos > 0)
        castName = Copy(castName, 1, nPos-1);
    TDB_Pokaztl::CreateTmpPokaztlTB(CurrYear, castName);
//    GlobalVars->slPlcKorteg = new TStringList;
    //GlobalVars->slPlcKorteg->Sorted = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("SELECT *");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("FROM #PokaztlTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("FROM PokaztlTB");
            break;
    }
    q->SQL->Add("ORDER BY pokaz_bk_code");
    q->Open();
    while (!q->EoF()) {
        GlobalVars->slPlcKorteg->AddObject(q->FieldByName("pokaz_bk_code")->AsString(), q->FieldByName("pokaz_id")->AsInteger());
        q->Next();
    }
    delete q;
    if (GlobalVars->slPlcKorteg->Count() == 0)
        ///throw Exception("Не удалось создать список slPlc от pokaz_idPlc=" + IntToStr(pokaz_idPlc));
    TDB_Pokaztl::DropTmpPokaztlTB();
}

// создать списки КБК для получ счетов с учетом подчинения
void T_Korteg::CreateListKBK_PlcVert(int CurrYear, int pokaz_idPlc) {
    std::string castName;
    int nPos, pokaz_id;

    TDB_Pokaztl::DropTmpPokaztlTB();
    castName = TDB_Pokaztl::GetCastName(pokaz_idPlc);
    nPos = Pos("\\", castName);
    if (nPos > 0)
        castName = Copy(castName, 1, nPos-1);
    TDB_Pokaztl::CreateTmpPokaztlTB(CurrYear, castName);
//    GlobalVars->slPlcKorteg = new TStringList;
    //GlobalVars->slPlcKorteg->Sorted = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("SELECT *");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("FROM #PokaztlTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("FROM PokaztlTB");
            break;
    }
    q->SQL->Add("ORDER BY pokaz_bk_code");
    q->Open();
    while (!q->EoF()) {
        if (q->FieldByName("pokaz_vert")->AsInteger() == 0)
            pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        else
            pokaz_id = -q->FieldByName("pokaz_id")->AsInteger();
        GlobalVars->slPlcKorteg->AddObject(q->FieldByName("pokaz_bk_code")->AsString(), pokaz_id);
        q->Next();
    }
    delete q;
    if (GlobalVars->slPlcKorteg->Count() == 0)
        //throw Exception("Не удалось создать список slPlc от pokaz_idPlc=" + IntToStr(pokaz_idPlc));
    TDB_Pokaztl::DropTmpPokaztlTB();
}

// скопировать список КБК для получ счетов
bool T_Korteg::CopyListKBK_Plc(TStringList* aSl) {
    std::string pokaz_bk_code;
    int pokaz_id;

    if (GlobalVars->slPlcKorteg == nullptr) return false;
    if (aSl == nullptr) return false;

    for (int i = 0; i < GlobalVars->slPlcKorteg->Count(); ++i) {
        pokaz_bk_code = GlobalVars->slPlcKorteg->Strings[i];
        pokaz_id = *std::get<std::shared_ptr<int>>(GlobalVars->slPlcKorteg->Objects[i]);
        aSl->AddObject(pokaz_bk_code, pokaz_id);
    }

    return true;
}

// создать списки КБК для распоряд счетов
void T_Korteg::CreateListKBK_Rsp(int CurrYear, int pokaz_idPlc) {
    TDB_Pokaztl::DropTmpPokaztlTB();
    std::string castName = TDB_Pokaztl::GetCastName(pokaz_idPlc);
    int nPos = Pos("\\", castName);
    if (nPos > 0) {
        castName = Copy(castName, 1, nPos-1);
    }
    castName += ". Лимиты";
    TDB_Pokaztl::CreateTmpPokaztlTB(CurrYear, castName);
//    GlobalVars->slRspKorteg = new TStringList;
    //GlobalVars->slRspKorteg->Sorted = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("SELECT *");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("FROM #PokaztlTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("FROM PokaztlTB");
            break;
    }
    q->SQL->Add("ORDER BY pokaz_bk_code");
    q->Open();
    while (!q->EoF()) {
        GlobalVars->slRspKorteg->AddObject(q->FieldByName("pokaz_bk_code")->AsString(), q->FieldByName("pokaz_id")->AsInteger());
        q->Next();
    }
    delete q;
    if (GlobalVars->slRspKorteg->Count() == 0) {
        //throw Exception("Не удалось создать список GlobalVars.slRspKorteg от pokaz_idPlc=" + IntToStr(pokaz_idPlc));
    }
    TDB_Pokaztl::DropTmpPokaztlTB();
}

// удалить списки КБК для получ и распоряд счетов
void T_Korteg::DeleteListKBK_Plc_Rsp() {
    T_Korteg::DeleteListKBK_Plc();
    T_Korteg::DeleteListKBK_Rsp();
}

// удалить списки КБК для получ счетов
void T_Korteg::DeleteListKBK_Plc() {
    if (GlobalVars->slPlcKorteg == nullptr) return;
//    delete GlobalVars->slPlcKorteg;
    GlobalVars->slPlcKorteg = nullptr;
}

// удалить списки КБК для распоряд счетов
void T_Korteg::DeleteListKBK_Rsp() {
    if (GlobalVars->slRspKorteg == nullptr) return;
//    delete GlobalVars->slRspKorteg;
    GlobalVars->slRspKorteg = nullptr;
}

// найти КБК для получ или распоряд счетов
int T_Korteg::FindByKBK_Plc_Rsp(int otkuda, std::string KBK, int pokaz_idPlc, int CurrYear, bool isMsg) {
    if (KBK == "") return pokaz_idPlc;
    switch (otkuda) {
        case 0: return 0;
        case 1: return T_Korteg::FindByKBK_Plc(KBK, pokaz_idPlc, CurrYear, isMsg);
        case 2: return T_Korteg::FindByKBK_Rsp(KBK, pokaz_idPlc, CurrYear, isMsg);
        case 3: return T_Korteg::FindByKBK_Rsp(KBK, pokaz_idPlc, CurrYear, isMsg);
        case 4: return T_Korteg::FindByKBK_Plc(KBK, pokaz_idPlc, CurrYear, isMsg);
    }
    return 0; // Default return, in case none of the cases match
}

// найти КБК для получ счетов
int T_Korteg::FindByKBK_Plc(std::string KBK, int pokaz_idPlc, int CurrYear, bool isMsg) {
    if (GlobalVars->slPlcKorteg == nullptr)
        T_Korteg::CreateListKBK_Plc(CurrYear, pokaz_idPlc);
    int Index = -1;
    if (GlobalVars->slPlcKorteg->Find(KBK, Index)) {
        return *std::get<std::shared_ptr<int>>(GlobalVars->slPlcKorteg->Objects[Index]);
    } else {
        T_Korteg::DeleteListKBK_Plc();
        if (isMsg) {
            ////SystFuncs.MessageBox(PChar('При поиске в списке slPlc не найден элемент для КБК '+KBK), MsgVnim, MB_OK or MB_ICONEXCLAMATION);
            //throw Exception("При поиске в списке slPlc не найден элемент для КБК " + KBK);
        }
        return 0;
    }
}

// найти КБК для распоряд счетов
int T_Korteg::FindByKBK_Rsp(std::string KBK, int pokaz_idPlc, int CurrYear, bool isMsg) {
    if (GlobalVars->slRspKorteg == nullptr)
        T_Korteg::CreateListKBK_Rsp(CurrYear, pokaz_idPlc);
    int Index = -1;
    if (GlobalVars->slRspKorteg->Find(KBK, Index)) {
        return *std::get<std::shared_ptr<int>>(GlobalVars->slRspKorteg->Objects[Index]);
    } else {
        T_Korteg::DeleteListKBK_Rsp();
        if (isMsg) {
            ////SystFuncs.MessageBox(PChar('При поиске в списке GlobalVars.slRspKorteg не найден элемент для КБК '+KBK), MsgVnim, MB_OK or MB_ICONEXCLAMATION);
            //throw Exception("При поиске в списке GlobalVars.slRspKorteg не найден элемент для КБК " + KBK);
        }
        return 0;
    }
}

// дать количество элементов в списке GlobalVars.slRspKorteg
int T_Korteg::getCountRsp() {
    if (GlobalVars->slRspKorteg == nullptr) {
        return 0;
    }
    return GlobalVars->slRspKorteg->Count();
}

// дать сведения об i-м элементе в списке GlobalVars.slRspKorteg - нумерация с 0
bool T_Korteg::getNumI(int i, std::string& KBK, int& pokaz_id) {
    if (GlobalVars->slRspKorteg == nullptr || i < 0 || i >= GlobalVars->slRspKorteg->Count()) {
        return false;
    }
    KBK = GlobalVars->slRspKorteg->Strings[i];
    pokaz_id = *std::get<std::shared_ptr<int>>(GlobalVars->slRspKorteg->Objects[i]);
    return true;
}



