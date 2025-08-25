

#include "UpdDBbyValue.h"
#include "../DB/DB_EdIzm.h"
#include "../DB/unDefDBconnect.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/DB_Pokaztl.h"
#include "ProcFormula.h"

double TUpdDBbyValue::SetValue(int aIst_fin_id, int aCell_period, TQueryUni* vPokaztl, int aCell_id, int aPlan_id,
                                      int aVid_otc, TDateTime aDate, double aValue, bool bDelta, bool bAddRec, int aObjectid, bool aIsMsg) {
    double SetValue;
    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn); 
    sp->storedProcName = "proc_SetValue";
    double SetValueFL = 0;
    double newvalue = 0;
    switch (GetDBType()) {
        case TDBType::MSSQL:
            sp->Params->CreateParam(ftInteger, "@aObjectid", ptInput);
            sp->Params->CreateParam(ftInteger, "@aCell_id", ptInput);
            sp->Params->CreateParam(ftInteger, "@aPlan_id", ptInput);
            sp->Params->CreateParam(ftInteger, "@aVid_otc", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aDate", ptInput);
            sp->Params->CreateParam(ftDouble, "@aValue", ptInput);
            sp->Params->CreateParam(ftInteger, "@aCoef", ptInput);
            sp->Params->CreateParam(ftInteger, "@bDelta", ptInput);
            sp->Params->CreateParam(ftInteger, "@bAddRec", ptInput);
            sp->Params->CreateParam(ftDouble, "@SetValue", ptOutput);
            sp->Params->CreateParam(ftDouble, "@newvalue", ptOutput);
    
            sp->ParamByName("@aObjectid")->AsInteger() = aObjectid;
            sp->ParamByName("@aCell_id")->AsInteger() = aCell_id;
            sp->ParamByName("@aPlan_id")->AsInteger() = aPlan_id;
            sp->ParamByName("@aVid_otc")->AsInteger() = aVid_otc;
            sp->ParamByName("@aDate")->AsDateTime() = aDate;
            sp->ParamByName("@aValue")->AsDouble() = aValue;
            if (vPokaztl->FieldByName("edizm_id")->AsInteger() == 0)
                sp->ParamByName("@aCoef")->AsInteger() = 0;
            else
                sp->ParamByName("@aCoef")->AsInteger() = TDB_EdIzm::GetCoef_PokazOpt(vPokaztl->FieldByName("edizm_id")->AsInteger());
    
            if (bDelta)
                sp->ParamByName("@bDelta")->AsInteger() = 1;
            else
                sp->ParamByName("@bDelta")->AsInteger() = 0;
            if (bAddRec)
                sp->ParamByName("@bAddRec")->AsInteger() = 1;
            else
                sp->ParamByName("@bAddRec")->AsInteger() = 0;
    
            sp->ExecProc();
    
            SetValue = sp->ParamByName("@SetValue")->AsDouble();
            newvalue = sp->ParamByName("@newvalue")->AsDouble();
            break;
        case TDBType::Postgre:
            sp->Params->CreateParam(ftInteger, "aObjectid", ptInput);
            sp->Params->CreateParam(ftInteger, "aCell_id", ptInput);
            sp->Params->CreateParam(ftInteger, "aPlan_id", ptInput);
            sp->Params->CreateParam(ftInteger, "aVid_otc", ptInput);
            sp->Params->CreateParam(ftDateTime, "aDate", ptInput);
            sp->Params->CreateParam(ftDouble, "aValue", ptInput);
            sp->Params->CreateParam(ftInteger, "aCoef", ptInput);
            sp->Params->CreateParam(ftInteger, "bDelta", ptInput);
            sp->Params->CreateParam(ftInteger, "bAddRec", ptInput);
            sp->Params->CreateParam(ftDouble, "SetValue", ptOutput);
            sp->Params->CreateParam(ftDouble, "newvalue", ptOutput);
    
            sp->ParamByName("aObjectid")->AsInteger() = aObjectid;
            sp->ParamByName("aCell_id")->AsInteger() = aCell_id;
            sp->ParamByName("aPlan_id")->AsInteger() = aPlan_id;
            sp->ParamByName("aVid_otc")->AsInteger() = aVid_otc;
//            string wsdt = DateTimeToStr(aDate);
//            sp->ParamByName("aDate")->AsDateTime() = aDate;
            sp->ParamByName("aDate")->SetValue(aDate);
            sp->ParamByName("aValue")->SetValue(static_cast<double>(aValue));
            if (vPokaztl->FieldByName("edizm_id")->AsInteger() == 0)
                sp->ParamByName("aCoef")->AsInteger() = 0;
            else
                sp->ParamByName("aCoef")->AsInteger() = TDB_EdIzm::GetCoef_PokazOpt(vPokaztl->FieldByName("edizm_id")->AsInteger());
    
            if (bDelta)
                sp->ParamByName("bDelta")->AsInteger() = 1;
            else
                sp->ParamByName("bDelta")->AsInteger() = 0;
            if (bAddRec)
                sp->ParamByName("bAddRec")->AsInteger() = 1;
            else
                sp->ParamByName("bAddRec")->AsInteger() = 0;
    
            sp->ExecProc();
            SetValueFL = sp->ParamByName("SetValue")->AsDouble();
            newvalue = sp->ParamByName("newvalue")->AsDouble();
//            SetValue = static_cast<double>();
//            newvalue = static_cast<double>();
            break;
    }

    delete sp;

    TUpdDBbyValue::ExecFormula(aIst_fin_id, aCell_period, vPokaztl, aCell_id, aPlan_id, aVid_otc, aDate, newvalue,
                                aObjectid, aIsMsg);

    return SetValueFL;
}


double TUpdDBbyValue::SetYearValue(int aIst_fin_id, int aCell_period, TQueryUni* vPokaztl,
                                   int aCell_id, int aPlan_id, int aVid_otc, int aGod, int aKv,
                                   int aMes, double aValue, bool bDelta, bool bAddRec, bool IsKvFormula,
                                   bool IsGodFormula, int aObjectid, bool aIsMsg){
    double delta;
    int kvCell_id;
    int godCell_id;
    TDateTime aDate;
    TDateTime dateKv;
    TDateTime dateGod;
    bool bIsKv, bIsGod;

    // по указанной дате вычисляем дату последнего дня месяца
    aDate = GetLastDayDate(aGod, aKv, aMes);
    delta = TUpdDBbyValue::SetValue(aIst_fin_id, aCell_period, vPokaztl, aCell_id, aPlan_id, aVid_otc, aDate, aValue, bDelta, bAddRec,
           aObjectid, aIsMsg);

    // по пришедшему значению cell_id
    // вычисляем cell_id для квартального значения и для годового
    bIsKv = (vPokaztl->FieldByName("pokaz_is_kv")->AsInteger() > 0);
    bIsGod = (vPokaztl->FieldByName("pokaz_is_god")->AsInteger() > 0);

    // если месяц был больше нуля, то изменение произошло в месячной ячейке
    // а если показатель имеет квартальную градацию, то надо изменить значение квартальной ячейки
    if ((aMes > 0) && (bIsKv)) {
        kvCell_id = aCell_id - 1;
        dateKv = GetLastDayDate(aGod, aKv, 0);
        // теперь произведем приращение значения найденной ячейки
        if (!IsKvFormula) // если не считается по формуле
            TUpdDBbyValue::SetValue(aIst_fin_id, 2, vPokaztl, kvCell_id, aPlan_id, aVid_otc, dateKv, delta, true, bAddRec,
            aObjectid, aIsMsg);
    }

    // если месяц был больше нуля или квартал был больше нуля,
    // то изменение произошло в месячной или квартальной ячейке
    // а если показатель имеет годовую градацию, то надо изменить значение годовой ячейки
    if (((aMes > 0) || (aKv > 0)) && (bIsGod)) {
        if (aMes > 0) godCell_id = aCell_id - 2; // это месячное значение - отнимаем два
        else godCell_id = aCell_id - 1;         // это квартальное значение - отнимаем один
        dateGod = GetLastDayDate(aGod, 0, 0);
        // теперь произведем приращение значения найденной ячейки
        if (!IsGodFormula) // если не считается по формуле
            TUpdDBbyValue::SetValue(aIst_fin_id, 1, vPokaztl, godCell_id, aPlan_id, aVid_otc, dateGod, delta, true, bAddRec,
            aObjectid, aIsMsg);
    }

    return delta;
}

double TUpdDBbyValue::Set3DValue(TQueryUni* vCell, TQueryUni* vPokaztl, int aPlan_id, int aVid_otc, int aGod, int aKv,
                                 int aMes, double aValue, bool bDelta, bool bAddRec, bool IsKvFormula, bool IsGodFormula, int aObjectid, bool aIsMsg) {
    TQueryUni* qCellAlt = new TQueryUni(DefDBconnect.Conn);
    TQueryUni* qDinGrade = new TQueryUni(DefDBconnect.Conn);
    TQueryUni* qDinIzm = new TQueryUni(DefDBconnect.Conn);
    TQueryUni* qCellIstFinVs = new TQueryUni(DefDBconnect.Conn);
    int aCell_id = vCell->FieldByName("cell_id")->AsInteger();
    double delta = TUpdDBbyValue::SetYearValue(vCell->FieldByName("ist_fin_id")->AsInteger(),
                                   vCell->FieldByName("cell_period")->AsInteger(),
                                   vPokaztl, aCell_id, aPlan_id, aVid_otc,
                                   aGod, aKv, aMes, aValue, bDelta, bAddRec, IsKvFormula, IsGodFormula,
                                   aObjectid, aIsMsg);
    
    // if sources of financing are used, need to change the value of the "Total" cell
    if (vCell->FieldByName("ist_fin_id")->AsInteger() > 0) { 
        qCellIstFinVs->SQL->Add("SELECT a.ist_fin_id, cell_id FROM Ist_fin a, Cell b");
        qCellIstFinVs->SQL->Add("WHERE a.ist_fin_name = :Vsego");
        qCellIstFinVs->SQL->Add("AND b.pokaz_id = :PokazId");
        qCellIstFinVs->SQL->Add("AND b.cell_period = :Period");
        qCellIstFinVs->SQL->Add("AND b.ist_fin_id = a.ist_fin_id");
        qCellIstFinVs->SQL->Add("AND b.cell_din_izm = :DinIzm");
        qCellIstFinVs->ParamByName("Vsego")->AsString() = "Всего";
        qCellIstFinVs->ParamByName("PokazId")->AsInteger() = vCell->FieldByName("pokaz_id")->AsInteger();
        qCellIstFinVs->ParamByName("Period")->AsInteger() = vCell->FieldByName("cell_period")->AsInteger();
        qCellIstFinVs->ParamByName("DinIzm")->AsString() = vCell->FieldByName("cell_din_izm")->AsString();
        qCellIstFinVs->Open();
        if (!qCellIstFinVs->EoF()) {
            int newCellId = qCellIstFinVs->FieldByName("cell_id")->AsInteger();
            TUpdDBbyValue::SetYearValue(qCellIstFinVs->FieldByName("ist_fin_id")->AsInteger(),
                         vCell->FieldByName("cell_period")->AsInteger(),
                         vPokaztl, newCellId, aPlan_id, aVid_otc,
                         aGod, aKv, aMes, delta, true, bAddRec, false, false,
                         aObjectid, aIsMsg);
        }

    }

    std::string strDinIzm = vCell->FieldByName("cell_din_izm")->AsString();
    if (strDinIzm.empty()) {
        return delta;
    }
 
    qCellAlt->SQL->Add("SELECT * FROM Cell ");
    qCellAlt->SQL->Add("WHERE pokaz_id = :PokazId");
    qCellAlt->SQL->Add("AND cell_period = :Period");
    qCellAlt->SQL->Add("AND ist_fin_id = :IstFinId");
    qCellAlt->SQL->Add("AND cell_din_izm = :DinIzm");
 
    qDinGrade->SQL->Add("SELECT din_izm_id FROM Din_izm ");
    qDinGrade->SQL->Add("WHERE din_izm_owner_id = :DinOwner");
    qDinGrade->SQL->Add("AND din_izm_order = 1");
 
    qDinIzm->SQL->Add("SELECT din_izm_is_vsego FROM Din_izm");
    qDinIzm->SQL->Add("WHERE din_izm_id = :DinIzmId");

    std::string strDinOld = strDinIzm;
    int posInsDin = 0;
    while (!strDinIzm.empty() && strDinIzm[0] == '#') {
        int posA = strDinIzm.find('_');
        int nDinIzmId;
        std::stringstream(strDinIzm.substr(1, posA - 2)) >> nDinIzmId;
        strDinIzm.erase(0, posA);
        int lenDelDin = posA;
        posA = strDinIzm.find('#');
        if (posA == std::string::npos) {
            posA = strDinIzm.length() + 1;
        }
        strDinIzm.erase(0, posA);
        lenDelDin += posA - 1;
        posInsDin += lenDelDin;

        qDinIzm->ParamByName("DinIzmId")->AsInteger() = nDinIzmId;
        qDinIzm->Open();
        if (qDinIzm->EoF() || qDinIzm->FieldByName("din_izm_is_vsego")->AsInteger() == 0) {
            qDinIzm->Close();
            continue;
        }
        qDinIzm->Close();

        // find the subordinate node with order 1 ("Total")
        qDinGrade->ParamByName("DinOwner")->AsInteger() = nDinIzmId;
        qDinGrade->Open();
        if (qDinGrade->EoF()) {
            qDinGrade->Close();
            continue;
        }

        // build a new string of dynamic dimension
        std::stringstream ss;
        ss << nDinIzmId;
        std::string strTemp = ss.str();
        std::string strDinNew = strDinOld.substr(0, posInsDin - lenDelDin) + "#" + strTemp;
        ss.str("");
        ss << qDinGrade->FieldByName("din_izm_id")->AsInteger();
        strTemp = ss.str();
        qDinGrade->Close();
        strDinNew += "_" + strTemp + strDinIzm;

        // find the cell in the Cell table
        int nxPokazId = vCell->FieldByName("pokaz_id")->AsInteger();
        int nxPeriod = vCell->FieldByName("cell_period")->AsInteger();
        int nxIstFin = vCell->FieldByName("ist_fin_id")->AsInteger();
        qCellAlt->ParamByName("PokazId")->AsInteger() = nxPokazId;
        qCellAlt->ParamByName("Period")->AsInteger() = nxPeriod;
        qCellAlt->ParamByName("IstFinId")->AsInteger() = nxIstFin;
        qCellAlt->ParamByName("DinIzm")->AsString() = strDinNew;
        qCellAlt->Open();
        if (qCellAlt->EoF()) {
            qCellAlt->Close();
            continue;
        }
        int newCellId = qCellAlt->FieldByName("cell_id")->AsInteger();
        qCellAlt->Close();
        TUpdDBbyValue::SetYearValue(nxIstFin, nxPeriod,
                     vPokaztl, newCellId, aPlan_id, aVid_otc,
                     aGod, aKv, aMes, delta, true, bAddRec, false, false,
                     aObjectid, aIsMsg);
    }

    delete qCellAlt;
    delete qDinIzm;
    delete qDinGrade;
    delete qCellIstFinVs;

    return delta;
}

bool TUpdDBbyValue::SetOwnerBase(TQueryUni* vCell, TQueryUni* vPokaztl, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, double aValue, bool bAddRec, bool aIzNix, bool aIsMsg) {
        bool Result = true;
        TQueryUni *qPokazTemp = new TQueryUni(DefDBconnect.Conn);
        qPokazTemp->SQL->Add("SELECT * FROM Pokaztl ");
        qPokazTemp->SQL->Add("WHERE pokaz_id = :CurrPokazId");
        // qPokazTemp->Prepare();

        TQueryUni *qPokazCell = new TQueryUni(DefDBconnect.Conn);

        qPokazCell->SQL->Add("SELECT * FROM Pokaztl a, Cell b, Edizm");
        qPokazCell->SQL->Add("WHERE a.pokaz_id = :PokazId");
        qPokazCell->SQL->Add("AND b.pokaz_id = a.pokaz_id");
        qPokazCell->SQL->Add("AND b.cell_period = :Period");
        qPokazCell->SQL->Add("AND b.ist_fin_id = :IstFinId");
        qPokazCell->SQL->Add("AND b.cell_din_izm = :DinIzm");
        // qPokazCell->Prepare();

        int Pokaz_id = vPokaztl->FieldByName("pokaz_owner_id")->AsInteger();
        if (vPokaztl->FieldByName("pokaz_is_razd")->AsInteger() == 3) {
            Pokaz_id = TDB_Pokaztl::GetPokazOwnerId(Pokaz_id);
        }

        while (true) {
            if (Pokaz_id == 0) break;
            qPokazTemp->Close();
            qPokazTemp->ParamByName("CurrPokazId")->AsInteger() = Pokaz_id;
            qPokazTemp->Open();
            if (qPokazTemp->EoF()) break;

            if (qPokazTemp->FieldByName("pokaz_is_razd")->AsInteger() == 2) { // это показатель
                if (qPokazTemp->FieldByName("pokaz_vert")->AsInteger() != 1) {
                    if (qPokazTemp->FieldByName("pokaz_vert")->AsInteger() == 2) { // Вариант "из них"
                        TDateTime aDate = GetLastDayDate(aGod, aKv, aMes);
                        if (aIzNix) {
                            // TUpdDBbyValue::CheckIzNixBase(false, qPokazTemp->FieldByName("pokaz_id")->AsInteger(),
                            //     qPokazTemp->FieldByName("pokaz_name")->AsString(),
                            //     aPlan_id, aVid_otc, aDate, 0.0,
                            //     vCell->FieldByName("cell_period")->AsInteger(),
                            //     vCell->FieldByName("ist_fin_id")->AsInteger(),
                            //     vCell->FieldByName("cell_din_izm")->AsString(),
                            //     0); // отключил 27.02.2020
                        } else {
                            goto LLike_VTomCisle;
                        }
                    } else if (qPokazTemp->FieldByName("pokaz_vert")->AsInteger() == 3) { // Вариант Включает
                        TDateTime aDate = GetLastDayDate(aGod, aKv, aMes);
                        if (aIzNix) {
                            Result = TUpdDBbyValue::CheckVklucaetBase(false, qPokazTemp->FieldByName("pokaz_id")->AsInteger(),
                                qPokazTemp->FieldByName("pokaz_name")->AsString(),
                                aPlan_id, aVid_otc, aDate, aDelta, aValue,
                                vCell->FieldByName("cell_period")->AsInteger(),
                                vCell->FieldByName("ist_fin_id")->AsInteger(),
                                vCell->FieldByName("cell_din_izm")->AsString(),
                                0, aIsMsg);
                            Result = true; // 08.05.2020 - после сообщения вычисления должны продолжаться
                        }
                    }
                    break;
                }

                LLike_VTomCisle:
                qPokazCell->Close();
                qPokazCell->ParamByName("PokazId")->AsInteger() = Pokaz_id;
                qPokazCell->ParamByName("Period")->AsInteger() = vCell->FieldByName("cell_period")->AsInteger();
                qPokazCell->ParamByName("IstFinId")->AsInteger() = vCell->FieldByName("ist_fin_id")->AsInteger();
                qPokazCell->ParamByName("DinIzm")->AsString() = vCell->FieldByName("cell_din_izm")->AsString();
                qPokazCell->Open();
                if (qPokazCell->EoF()) {
                    // MessageDlg('В таблице Cell нет записи по родительскому показателю',mtInformation,[mbOK],0);
                    break;
                }

                TUpdDBbyValue::Set3DValue(qPokazCell, qPokazCell, aPlan_id, aVid_otc,
                    aGod, aKv, aMes, aDelta, true, bAddRec, false, false, 0, aIsMsg);
                Pokaz_id = qPokazTemp->FieldByName("pokaz_owner_id")->AsInteger();
                continue;
            }

            if (qPokazTemp->FieldByName("pokaz_is_razd")->AsInteger() == 1 && // это раздел
                qPokazTemp->FieldByName("pokaz_vsego_id")->AsInteger() > 0) { // имеет показатель "Итого по разделу"
                qPokazCell->Close();
                qPokazCell->ParamByName("PokazId")->AsInteger() = qPokazTemp->FieldByName("pokaz_vsego_id")->AsInteger();
                qPokazCell->ParamByName("Period")->AsInteger() = vCell->FieldByName("cell_period")->AsInteger();
                qPokazCell->ParamByName("IstFinId")->AsInteger() = vCell->FieldByName("ist_fin_id")->AsInteger();
                qPokazCell->ParamByName("DinIzm")->AsString() = vCell->FieldByName("cell_din_izm")->AsString();
                qPokazCell->Open();
                if (qPokazCell->EoF()) {
                    break;
                }

                TUpdDBbyValue::Set3DValue(qPokazCell, qPokazCell, aPlan_id, aVid_otc,
                    aGod, aKv, aMes, aDelta, true, bAddRec, false, false, 0, aIsMsg);
            }

            Pokaz_id = qPokazTemp->FieldByName("pokaz_owner_id")->AsInteger();
        }

        delete qPokazTemp;
        delete qPokazCell;
        return Result;
    }

void TUpdDBbyValue::SetOwnerBaseMulti(bool aB, TQueryUni* vCell, int aItogo_idMulti, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, bool bAddRec, bool aIsMsg) {
        if (aB) {
            TUpdDBbyValue::Set3DValue(vCell, vCell, aPlan_id, aVid_otc, aGod, aKv, aMes, aDelta, true, bAddRec, false, false, aItogo_idMulti, aIsMsg);
        }

        TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
        qPokaz->SQL->Add("SELECT c.pokaz_id, c.pokaz_is_razd");
        qPokaz->SQL->Add("FROM Pokaztl a2, Pokaztl a, Pokaztl b, Pokaztl c");
        qPokaz->SQL->Add("WHERE a2.pokaz_id = :pokaz_id");
        qPokaz->SQL->Add("AND   a.pokaz_id = a2.pokaz_owner_id"); // this is the section where the MultiTotal is contained
        qPokaz->SQL->Add("AND   b.pokaz_id = a.pokaz_owner_id"); // this is the section even higher
        qPokaz->SQL->Add("AND   c.pokaz_id = b.pokaz_vsego_id");
        qPokaz->ParamByName("pokaz_id")->AsInteger() = aItogo_idMulti;
        qPokaz->Open();

        int wpokaz_is_razd = 0;
        int wItogo_idMulti = 0;
        if (!qPokaz->EoF()) {
            wpokaz_is_razd = qPokaz->FieldByName("pokaz_is_razd")->AsInteger();
            wItogo_idMulti = qPokaz->FieldByName("pokaz_id")->AsInteger();
        }
        delete qPokaz;

        if (wpokaz_is_razd == 5) {
            TUpdDBbyValue::SetOwnerBaseMulti(true, vCell, wItogo_idMulti, aPlan_id, aVid_otc, aGod, aKv, aMes, aDelta, bAddRec, aIsMsg);
        }
    }

int TUpdDBbyValue::getOwnerFieldId(int OwnerClass_id, std::string idsOwnerFieldItogo) {
    int pos1, pos2, OwnerFieldItogoId, L;
    std::string strIdOwnerClass;

    int result = 0;
    std::stringstream ss;
    ss << OwnerClass_id << ".";
    strIdOwnerClass = ss.str();

    pos1 = idsOwnerFieldItogo.find(strIdOwnerClass);
    if (pos1 == std::string::npos) return result;

    pos1 += strIdOwnerClass.length();
    pos2 = idsOwnerFieldItogo.find(";", pos1);
    if (pos2 == std::string::npos) {
        L = idsOwnerFieldItogo.length() - pos1 + 1;
    } else {
        L = pos2 - pos1;
    }

    OwnerFieldItogoId = std::stoi(idsOwnerFieldItogo.substr(pos1, L));
    return OwnerFieldItogoId;
}

void TUpdDBbyValue::SetOwnerObjectItogo(int aObjectid, TQueryUni* vCell, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta,
                                        double aValue, bool bAddRec, std::string idsOwnerFieldItogo, bool aIsMsg) {
        int objectOwnerId, objekt_class_id, idOwnerFieldItogo; 
        TQueryUni* qPokaz;
        int wpokaz_is_razd;
        int wItogo_idMulti;

        objectOwnerId = TDB_Pokaztl::GetPokazOwnerId(aObjectid);
        if (objectOwnerId == 0) return;

        objekt_class_id = TDB_Pokaztl::GetClassId(objectOwnerId);
        idOwnerFieldItogo = TUpdDBbyValue::getOwnerFieldId(objekt_class_id, idsOwnerFieldItogo);

        TQueryUni *qCellPokaz = new TQueryUni(DefDBconnect.Conn); 
        qCellPokaz->SQL->Add("SELECT * FROM Pokaztl a, Cell b, Edizm");
        qCellPokaz->SQL->Add("WHERE a.pokaz_id = :PokazId");
        qCellPokaz->SQL->Add("AND b.pokaz_id = a.pokaz_id");
        qCellPokaz->SQL->Add("AND b.cell_period = :Period");
        qCellPokaz->SQL->Add("AND b.ist_fin_id = :IstFinId");
        qCellPokaz->SQL->Add("AND b.cell_din_izm = :DinIzm");
        qCellPokaz->ParamByName("PokazId")->AsInteger() = idOwnerFieldItogo;
        qCellPokaz->ParamByName("Period")->AsInteger() = vCell->FieldByName("cell_period")->AsInteger();
        qCellPokaz->ParamByName("IstFinId")->AsInteger() = vCell->FieldByName("ist_fin_id")->AsInteger();
        qCellPokaz->ParamByName("DinIzm")->AsString() = vCell->FieldByName("cell_din_izm")->AsString();
        qCellPokaz->Open();
        if (!qCellPokaz->EoF()) {
            TUpdDBbyValue::Set3DValue(qCellPokaz, qCellPokaz, aPlan_id, aVid_otc, aGod, aKv, aMes, aDelta, true, bAddRec, false, false, objectOwnerId, aIsMsg);
        }

        if (qCellPokaz->FieldByName("pokaz_vsego_id")->AsInteger() == 2) {
            TUpdDBbyValue::SetOwnerObjectItogo(objectOwnerId, qCellPokaz, aPlan_id, aVid_otc, aGod, aKv, aMes, aDelta, aValue, bAddRec, qCellPokaz->FieldByName("idsOwnerFieldItogo")->AsString(), aIsMsg);
        } else {
            TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
            qPokaz->SQL->Add("SELECT c.pokaz_id, c.pokaz_is_razd");
            qPokaz->SQL->Add("FROM Pokaztl a, Pokaztl b, Pokaztl c");
            qPokaz->SQL->Add("WHERE a.pokaz_id = :pokaz_id");
            qPokaz->SQL->Add("AND   b.pokaz_id = a.pokaz_owner_id");
            qPokaz->SQL->Add("AND   c.pokaz_id = b.pokaz_vsego_id");
            qPokaz->ParamByName("pokaz_id")->AsInteger() = objectOwnerId;
            qPokaz->Open();
            wpokaz_is_razd = 0;
            wItogo_idMulti = 0;
            if (!qPokaz->EoF()) {
                wpokaz_is_razd = qPokaz->FieldByName("pokaz_is_razd")->AsInteger();
                wItogo_idMulti = qPokaz->FieldByName("pokaz_id")->AsInteger();
            }
            //MAK qPokaz->Free();
            delete qPokaz;
            if (wpokaz_is_razd == 3) {
                if (qCellPokaz->FieldByName("pokaz_vsego_id")->AsInteger() > 0) {
                    TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
                    TDB_Pokaztl::GetPokaz(qPokaz, objectOwnerId);
                    TUpdDBbyValue::SetOwnerBase(qCellPokaz, qPokaz, aPlan_id, aVid_otc, aGod, aKv, aMes, aDelta, aValue, bAddRec, false, aIsMsg);
                    //MAK qPokaz->Free();
                    delete qPokaz;
                }
            } else if (wpokaz_is_razd == 5) {
                TUpdDBbyValue::SetOwnerBaseMulti((objectOwnerId != wItogo_idMulti), qCellPokaz, wItogo_idMulti, aPlan_id, aVid_otc, aGod, aKv, aMes, aDelta, bAddRec, aIsMsg);
            }
        }

        //MAK qCellPokaz->Free();
        delete qCellPokaz;
    }

void TUpdDBbyValue::SetOwnerObjectAgregat(int aObjectid, TQueryUni* vCell, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, double aValue, bool bAddRec, bool aIsMsg){
    int objectOwnerId, objekt_class_id;
    int wpokaz_is_razd, wItogo_idMulti;

    objectOwnerId = TDB_Pokaztl::GetPokazOwnerId(aObjectid);
    if (objectOwnerId == 0) return;

    objekt_class_id = TDB_Pokaztl::GetClassId(objectOwnerId);
    if (objekt_class_id != TDB_Pokaztl::GetClassId(aObjectid)) // это не иерархия
        return;

    TUpdDBbyValue::Set3DValue(vCell, vCell,
                              aPlan_id, aVid_otc, aGod, aKv, aMes,
                              aDelta, true, bAddRec, false, false,    // надо суммировать
                              objectOwnerId, aIsMsg);

    TUpdDBbyValue::SetOwnerObjectAgregat(objectOwnerId, vCell, aPlan_id, aVid_otc,
                                         aGod, aKv, aMes, aDelta, aValue,
                                         bAddRec, aIsMsg);
}

bool TUpdDBbyValue::UpdDBbyValueX(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, bool bAddRec, bool aIzNix, bool aIsMsg, int aObjectid) {
    TracerManager::Instance().StartFunctionTrace("TUpdDBbyValue::UpdDBbyValueX");
    bool UpdDBbyValueX = true;
    int aMes = 0;
    int aKv = 0;
    double delta = 0;

    TQueryUni *qCellPokaz = new TQueryUni(DefDBconnect.Conn);

    bool IsMesFormula = false;
    bool IsKvFormula = false;
    bool IsGodFormula = false;
    qCellPokaz->SQL->Add("SELECT c.cell_id, c.pokaz_id, c.cell_period, c.ist_fin_id, c.cell_din_izm, c.cell_is_eval,");
    qCellPokaz->SQL->Add("  b.pokaz_name, b.pokaz_is_razd, b.pokaz_owner_id, b.pokaz_code, b.pokaz_vert, b.pokaz_type, b.edizm_id,");
    qCellPokaz->SQL->Add("  b.pokaz_is_mes, b.pokaz_is_kv, b.pokaz_is_god, b.pokaz_is_ist_fin, b.pokaz_is_din_izm,");
    qCellPokaz->SQL->Add("  b.pokaz_base_id, b.pokaz_vsego_id, b.idsOwnerFieldItogo, b.np_vid_id, b.level_sign, edizm_coef");
    qCellPokaz->SQL->Add("FROM Cell a, Pokaztl b, Cell c, Edizm");
    qCellPokaz->SQL->Add("WHERE a.cell_id = :CurrCellId");
    qCellPokaz->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    qCellPokaz->SQL->Add("AND c.pokaz_id = b.pokaz_id");
    qCellPokaz->SQL->Add("AND (b.pokaz_is_mes = 1 AND c.cell_period = 3");
    qCellPokaz->SQL->Add("    OR b.pokaz_is_kv = 1 AND c.cell_period = 2");
    qCellPokaz->SQL->Add("    OR b.pokaz_is_god = 1 AND c.cell_period = 1)");
    qCellPokaz->SQL->Add("AND Edizm.edizm_id = b.edizm_id");
    qCellPokaz->SQL->Add("ORDER BY c.cell_period DESC");
    qCellPokaz->ParamByName("CurrCellId")->AsInteger() = aCell_id;
    qCellPokaz->Open();

    if (qCellPokaz->EoF()) {
        //MAK qCellPokaz->Free();
        delete qCellPokaz;
        TracerManager::Instance().EndFunctionTrace();
        return false;
    }

    // Determine the periodicity of the data
    qCellPokaz->First();
    while (!qCellPokaz->EoF()) {
        if (qCellPokaz->FieldByName("cell_period")->AsInteger() == 3 && qCellPokaz->FieldByName("cell_is_eval")->AsInteger() == 2)
            IsMesFormula = true;
        if (qCellPokaz->FieldByName("cell_period")->AsInteger() == 2 && qCellPokaz->FieldByName("cell_is_eval")->AsInteger() == 2)
            IsKvFormula = true;
        if (qCellPokaz->FieldByName("cell_period")->AsInteger() == 1 && qCellPokaz->FieldByName("cell_is_eval")->AsInteger() == 2)
            IsGodFormula = true;
        qCellPokaz->Next();
    }

    // Find the current row in the Cell table
    qCellPokaz->First();
    while (!qCellPokaz->EoF()) {
        if (qCellPokaz->FieldByName("cell_id")->AsInteger() == aCell_id)
            break;
        qCellPokaz->Next();
    }

    int wMonth,wDay,aGod ;
    DecodeDate(aDate,aGod,wMonth,wDay);
    switch (qCellPokaz->FieldByName("cell_period")->AsInteger()) {
    case 1: // годовая
        aMes = 0;
        aKv = 0;
        break;
    case 2: // квартальная
        aMes = 0;
        aKv = (wMonth  - 1) / 3 + 1;
        break;
    case 3: // месячная
        aMes = wMonth;
        aKv = (wMonth - 1) / 3 + 1;
        break;
    }

    bool rtnB = true;
    // Check the value before saving to the database
    if (qCellPokaz->FieldByName("pokaz_base_id")->AsInteger() == 0) {
        // If the indicator is in monetary terms
        if (qCellPokaz->FieldByName("pokaz_vert")->AsInteger() == 2) {
            if (aIzNix)
                rtnB = true; // TUpdDBbyValue::CheckIzNixBase(...)
        } else if (qCellPokaz->FieldByName("pokaz_vert")->AsInteger() == 3) {
            if (aIzNix) {
                rtnB = TUpdDBbyValue::CheckVklucaetBase(true, qCellPokaz->FieldByName("pokaz_id")->AsInteger(),
                                                       qCellPokaz->FieldByName("pokaz_name")->AsString(),
                                                       aPlan_id, aVid_otc, aDate, 0.0, aValue,
                                                       qCellPokaz->FieldByName("cell_period")->AsInteger(),
                                                       qCellPokaz->FieldByName("ist_fin_id")->AsInteger(),
                                                       qCellPokaz->FieldByName("cell_din_izm")->AsString(),
                                                       0, aIsMsg);
                rtnB = true; // 02.04.2020 - continue calculations after the message
            }
        }
        if (!rtnB) {
            //MAK qCellPokaz->Free();
            delete qCellPokaz;
            TracerManager::Instance().EndFunctionTrace();
            return false;
        }

        delta = TUpdDBbyValue::Set3DValue(qCellPokaz, qCellPokaz,
                                                aPlan_id, aVid_otc, aGod, aKv, aMes,
                                                aValue, false, bAddRec, IsKvFormula, IsGodFormula,
                                                aObjectid, aIsMsg);

        // Process higher-level indicators / sections / objects
        if (aObjectid == 0) {
            UpdDBbyValueX = TUpdDBbyValue::SetOwnerBase(qCellPokaz, qCellPokaz, aPlan_id, aVid_otc, aGod, aKv, aMes, delta, aValue, bAddRec, aIzNix, aIsMsg);
        } else {
            if (qCellPokaz->FieldByName("pokaz_vsego_id")->AsInteger() == 2) {
                TUpdDBbyValue::SetOwnerObjectItogo(aObjectid, qCellPokaz, aPlan_id, aVid_otc,
                                                  aGod, aKv, aMes, delta, aValue,
                                                  bAddRec, qCellPokaz->FieldByName("idsOwnerFieldItogo")->AsString(), aIsMsg);
            } else if (qCellPokaz->FieldByName("pokaz_vsego_id")->AsInteger() == 4) {
                TUpdDBbyValue::SetOwnerObjectAgregat(aObjectid, qCellPokaz, aPlan_id, aVid_otc,
                                                    aGod, aKv, aMes, delta, aValue,
                                                    bAddRec, aIsMsg);
            } else {
                TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
                qPokaz->SQL->Add("SELECT c.pokaz_id, c.pokaz_is_razd");
                qPokaz->SQL->Add("FROM Pokaztl a, Pokaztl b, Pokaztl c");
                qPokaz->SQL->Add("WHERE a.pokaz_id = :pokaz_id");
                qPokaz->SQL->Add("AND   b.pokaz_id = a.pokaz_owner_id");
                qPokaz->SQL->Add("AND   c.pokaz_id = b.pokaz_vsego_id");
                qPokaz->ParamByName("pokaz_id")->AsInteger() = aObjectid;
                qPokaz->Open();
                int wpokaz_is_razd = 0;
                int wItogo_idMulti = 0;
                if (!qPokaz->EoF()) {
                    wpokaz_is_razd = qPokaz->FieldByName("pokaz_is_razd")->AsInteger();
                    wItogo_idMulti = qPokaz->FieldByName("pokaz_id")->AsInteger();
                }
                //MAK qPokaz->Free();
                delete qPokaz;
                if (wpokaz_is_razd == 3) {
                    if (qCellPokaz->FieldByName("pokaz_vsego_id")->AsInteger() == 1) { // In the case of a characteristic - a data supplier in the Total for the section
                        TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
                        TDB_Pokaztl::GetPokaz(qPokaz, aObjectid);
                        TUpdDBbyValue::SetOwnerBase(qCellPokaz, qPokaz, aPlan_id, aVid_otc, aGod, aKv, aMes,
                                                    delta, aValue, bAddRec, aIzNix, aIsMsg);
                        //MAK qPokaz->Free();
                        delete qPokaz;
                    }
                } else if (wpokaz_is_razd == 5) {
                    TUpdDBbyValue::SetOwnerBaseMulti((aObjectid != wItogo_idMulti), qCellPokaz, wItogo_idMulti, aPlan_id, aVid_otc, aGod, aKv, aMes, delta, bAddRec, aIsMsg);
                }
            }
        }
    } else {
        // If the indicator is in natural terms
        TUpdDBbyValue::SetOwnerNatur(qCellPokaz, qCellPokaz, aPlan_id, aVid_otc, aGod, aKv, aMes, delta, bAddRec, aIzNix, aIsMsg);
    }

    //MAK qCellPokaz->Free();
    delete qCellPokaz;
    TracerManager::Instance().EndFunctionTrace();
    return UpdDBbyValueX;
}

bool TUpdDBbyValue::UpdDBbyValue1(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIsMsg) {
    if ((aValue <= 0) && IsZero(aValue, 0.000001)) aValue = 0; // Проверяем, является ли значение по существу нулевым

    bool result = TUpdDBbyValue::UpdDBbyValueX(aCell_id, aPlan_id, aVid_otc,
                                               aDate, aValue, true, true, aIsMsg, objectid); // Передаем аргументы в другую функцию
    return result;
}

bool TUpdDBbyValue::UpdDBbyValue1(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue,
                                  int objectid, bool aIsMsg, json &JSONresult) {
    TracerManager::Instance().StartFunctionTrace("TUpdDBbyValue::UpdDBbyValue1");
    if ((aValue <= 0) && IsZero(aValue, 0.000001)) aValue = 0; // Проверяем, является ли значение по существу нулевым

    bool result = TUpdDBbyValue::UpdDBbyValueX(aCell_id, aPlan_id, aVid_otc,
                                               aDate, aValue, true, true, aIsMsg, objectid); // Передаем аргументы в другую функцию
    TracerManager::Instance().EndFunctionTrace();
    return result;
}

bool TUpdDBbyValue::UpdDBbyValue2(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIsMsg) {
    if ((aValue <= 0) && IsZero(aValue, 0.000001)) {
        aValue = 0; // Установка значения в 0, если оно близко к 0
    }

    // Вызов функции обновления базы данных
    bool result = TUpdDBbyValue::UpdDBbyValueX(aCell_id, aPlan_id, aVid_otc,
                                               aDate, aValue, false, true, aIsMsg, objectid);
    return result;
}

bool TUpdDBbyValue::UpdDBbyValue3(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIzNix, bool aIsMsg) {
    if ((aValue <= 0.0) && (std::fabs(aValue) < 0.000001)) {
        aValue = 0.0;
    }
    return TUpdDBbyValue::UpdDBbyValueX(aCell_id, aPlan_id, aVid_otc, aDate, aValue, true, aIzNix, aIsMsg, objectid);
}

void TUpdDBbyValue::ExecFormula(int aIst_fin_id, int aCell_period, TQueryUni* vPokaztl, int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate, double aValue, int aObjectid, bool aIsMsg) {
    TQueryUni* qInfluence = new TQueryUni(DefDBconnect.Conn);
        qInfluence->SQL->Add("SELECT out_cell_id FROM Influence ");
        qInfluence->SQL->Add("WHERE in_cell_id = :CellId");
        qInfluence->SQL->Add("OR in_cell_id = :Kod"); // for the sum by the AU code
        qInfluence->ParamByName("CellId")->AsString() = std::to_string(aCell_id);
        // calculating the code
        // a negative number will be placed in influence, which contains: 1-sign of the sum operation by the AU code,
        // AU code, zeros, system code of the source of financing, periodicity code (one digit)
        std::string s = "1" + vPokaztl->FieldByName("pokaz_code")->AsString();
        // s contains '1' + AU code
        std::string s1 = std::to_string(aIst_fin_id) + std::to_string(aCell_period);
        // s1 contains the system code of the source of financing, periodicity code (one digit)
        if (s.length() + s1.length() <= 10)
            s += std::string(10 - s.length() - s1.length(), '0') + s1; // as it was
        else
            s += std::string(19 - s.length() - s1.length(), '0') + s1; // for longer AU codes
        int Kod;
        std::istringstream(s) >> Kod; Kod = -Kod;
        std::string sKod = "-" + s;
        qInfluence->ParamByName("Kod")->AsString() = sKod;
        qInfluence->Open();
        if (qInfluence->EoF()) {
            // no dependent cells
            delete qInfluence;
            return;
        }

        // iterate through all dependent cells
        qInfluence->First();
        while (!qInfluence->EoF()) {
            int nCellOut = qInfluence->FieldByName("out_cell_id")->AsInteger();
            if (nCellOut != aCell_id) { // protection against a loop, especially for the sum by the AU code
                // selection of the formula for calculating nCellOut
                TQueryUni* qFormulAlt = new TQueryUni(DefDBconnect.Conn);
                qFormulAlt->SQL->Add("SELECT formula, edizm_coef FROM Cell a,Formula b, Pokaztl, Edizm");
                qFormulAlt->SQL->Add("WHERE a.cell_id = :CellId");
                qFormulAlt->SQL->Add("AND b.pokaz_id = a.pokaz_id");
                qFormulAlt->SQL->Add("AND b.cell_id = a.cell_id");
                qFormulAlt->SQL->Add("AND Pokaztl.pokaz_id = a.pokaz_id");
                qFormulAlt->SQL->Add("AND Edizm.edizm_id = Pokaztl.edizm_id");
                qFormulAlt->ParamByName("CellId")->AsInteger() = nCellOut;
                qFormulAlt->Open();
                std::string strFormula = "";
                int Coef = 0;
                if (!qFormulAlt->EoF()) {
                    strFormula = qFormulAlt->FieldByName("formula")->AsString();
                    Coef = qFormulAlt->FieldByName("edizm_coef")->AsInteger();
                }
                delete qFormulAlt;
                // End of selection of the formula for calculating nCellOut
                // calculations by the formula
                if (IsOpInFormula(strFormula)) { // if there are operations in the formula
                    double fValue;
                    if (strFormula[0] == 'S') // formula = sum by AU code
                        fValue = ProcFormula::CalcSumPoKodAU(nCellOut, strFormula, aPlan_id, aVid_otc, aDate) / pow(10, Coef);
                    else
                        fValue = ProcFormula::CalcPolishRec(ProcFormula::GetPolishRec(strFormula), aPlan_id, aVid_otc, aDate);
                    TUpdDBbyValue::UpdDBbyValueX(nCellOut, aPlan_id, aVid_otc, aDate, fValue, true, false, aIsMsg, 0); // From Them is not checked
                }
                else { // the formula has one operand
                    TUpdDBbyValue::UpdDBbyValueX(nCellOut, aPlan_id, aVid_otc, aDate, aValue, true, false, aIsMsg, 0); // From Them is not checked
                }
                // End of calculations by the formula
            }
            qInfluence->Next();
        }
        // End of iterating through all dependent cells
        delete qInfluence;
    }

bool TUpdDBbyValue::CheckIzNixBase(bool aIsValue, int aPokaz_id, std::string aPokaz_name, int aPlan_id, unsigned char aVid_otc,
                                   TDateTime aValue_date, double aValue, int aCell_period, int aIst_fin_id, std::string aCell_din_izm, int objectid){
        bool CheckIzNixBase = true;
        // выборка значения
        if (!aIsValue) {
            aValue = 0.0;
            TQueryUni *qVal = new TQueryUni(DefDBconnect.Conn);
            qVal->SQL->Add("SELECT value FROM Cell a, Cell_value b");
            qVal->SQL->Add("WHERE a.pokaz_id = :Pokaz_id");
            qVal->SQL->Add("AND a.cell_period = :Cell_period");
            qVal->SQL->Add("AND a.ist_fin_id = :Ist_fin_id");
            qVal->SQL->Add("AND a.cell_din_izm = :Cell_din_izm");
            qVal->SQL->Add("AND b.cell_id = a.cell_id");
            qVal->SQL->Add("AND b.plan_id = :Plan_id");
            qVal->SQL->Add("AND b.vid_otc = :Vid_otc");
            qVal->SQL->Add("AND b.value_date = :Value_date");
            qVal->ParamByName("Pokaz_id")->AsInteger() = aPokaz_id;
            qVal->ParamByName("Cell_period")->AsInteger() = aCell_period;
            qVal->ParamByName("Ist_fin_id")->AsInteger() = aIst_fin_id;
            qVal->ParamByName("Cell_din_izm")->AsString() = aCell_din_izm;
            qVal->ParamByName("Plan_id")->AsInteger() = aPlan_id;
            qVal->ParamByName("Vid_otc")->AsInteger() = aVid_otc;
            qVal->ParamByName("Value_date")->AsDateTime() = aValue_date;
            qVal->Open();
            if (!qVal->EoF())
                aValue = qVal->FieldByName("value")->AsFloat();
            delete qVal;
        }
        // End of выборка значения
        TQueryUni *qVse = new TQueryUni(DefDBconnect.Conn);
        double sum = 0.0;
        qVse->SQL->Add("SELECT SUM(value) FROM Pokaztl a, Cell b, Cell_value c");
        qVse->SQL->Add("WHERE a.pokaz_owner_id = :Pokaz_id");
        qVse->SQL->Add("AND a.pokaz_type = 1");
        qVse->SQL->Add("AND b.pokaz_id = a.pokaz_id");
        qVse->SQL->Add("AND b.cell_period = :Cell_period");
        qVse->SQL->Add("AND b.ist_fin_id = :Ist_fin_id");
        qVse->SQL->Add("AND b.cell_din_izm = :Cell_din_izm");
        qVse->SQL->Add("AND c.cell_id = b.cell_id");
        qVse->SQL->Add("AND c.plan_id = :Plan_id");
        qVse->SQL->Add("AND c.vid_otc = :Vid_otc");
        qVse->SQL->Add("AND c.value_date = :Value_date");
        qVse->ParamByName("Pokaz_id")->AsInteger() = aPokaz_id;
        qVse->ParamByName("Cell_period")->AsInteger() = aCell_period;
        qVse->ParamByName("Ist_fin_id")->AsInteger() = aIst_fin_id;
        qVse->ParamByName("Cell_din_izm")->AsString() = aCell_din_izm;
        qVse->ParamByName("Plan_id")->AsInteger() = aPlan_id;
        qVse->ParamByName("Vid_otc")->AsInteger() = aVid_otc;
        qVse->ParamByName("Value_date")->AsDateTime() = aValue_date;
        qVse->Open();
        if (!qVse->EoF())
            sum = qVse->Fields[0].AsFloat();
        //--if sum > aValue then begin
        if (std::fabs(sum - aValue) > 0.000001) {
            std::string s = "Сумма значений подпоказателей, равная " + std::to_string(sum) + ", " +
                           "превышает значение Из Них - показателя <" + aPokaz_name + ">, равное " + std::to_string(aValue) +
                           ". ИСПРАВЬТЕ значение";
            //SystFuncs.MessageBox('Сумма значений подпоказателей, равная '+floattostr(sum)+', '
            //  +'превышает значение Из Них - показателя <'+aPokaz_name+'>, равное '+floattostr(aValue),
            //--SystFuncs.MessageBoxExclamation(Pchar(s), MsgVnim); Сейчас не проверяем 11.03.2024
            CheckIzNixBase = false;
            //--raise Exception.Create('Ошибка в Из-Них');  // 08.10.2019
        }
        delete qVse;
        return CheckIzNixBase;
    }

bool TUpdDBbyValue::CheckVklucaetBase(bool aIsValue, int aPokaz_id, std::string aPokaz_name, int aPlan_id, unsigned char aVid_otc, TDateTime aValue_date,
                                      double aDelta, double aValue, int aCell_period, int aIst_fin_id, std::string aCell_din_izm, int objectid, bool aIsMsg){
    bool Result = true;
    // выборка значения
    double aValueOwner = 0.0;
    if (!aIsValue) { // вычисляем значнеие владельца
        TQueryUni *qVal = new TQueryUni(DefDBconnect.Conn);
        qVal->SQL->Add("SELECT value FROM Cell a, Cell_value b");
        qVal->SQL->Add("WHERE a.pokaz_id = :Pokaz_id");
        qVal->SQL->Add("AND a.cell_period = :Cell_period");
        qVal->SQL->Add("AND a.ist_fin_id = :Ist_fin_id");
        qVal->SQL->Add("AND a.cell_din_izm = :Cell_din_izm");
        qVal->SQL->Add("AND b.cell_id = a.cell_id");
        qVal->SQL->Add("AND b.plan_id = :Plan_id");
        qVal->SQL->Add("AND b.vid_otc = :Vid_otc");
        qVal->SQL->Add("AND b.value_date = :Value_date");
        qVal->ParamByName("Pokaz_id")->AsInteger() = aPokaz_id;
        qVal->ParamByName("Cell_period")->AsInteger() = aCell_period;
        qVal->ParamByName("Ist_fin_id")->AsInteger() = aIst_fin_id;
        qVal->ParamByName("Cell_din_izm")->AsString() = aCell_din_izm;
        qVal->ParamByName("Plan_id")->AsInteger() = aPlan_id;
        qVal->ParamByName("Vid_otc")->AsInteger() = aVid_otc;
        qVal->ParamByName("Value_date")->AsDateTime() = aValue_date;
        qVal->Open();
        if (!qVal->EoF())
            aValueOwner = qVal->FieldByName("value")->AsFloat();
        delete qVal;
    }
    else
        aValueOwner = aValue; // значение владельца пришло
    // End of выборка значения

    double aValuePodcin = 0.0;
    TQueryUni *qVse = new TQueryUni(DefDBconnect.Conn);
    qVse->SQL->Add("SELECT MAX(value) FROM Pokaztl a, Cell b, Cell_value c");
    qVse->SQL->Add("WHERE a.pokaz_owner_id = :Pokaz_id");
    qVse->SQL->Add("AND a.pokaz_type = 1");
    qVse->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    qVse->SQL->Add("AND b.cell_period = :Cell_period");
    qVse->SQL->Add("AND b.ist_fin_id = :Ist_fin_id");
    qVse->SQL->Add("AND b.cell_din_izm = :Cell_din_izm");
    qVse->SQL->Add("AND c.cell_id = b.cell_id");
    qVse->SQL->Add("AND c.plan_id = :Plan_id");
    qVse->SQL->Add("AND c.vid_otc = :Vid_otc");
    qVse->SQL->Add("AND c.value_date = :Value_date");
    qVse->ParamByName("Pokaz_id")->AsInteger() = aPokaz_id;
    qVse->ParamByName("Cell_period")->AsInteger() = aCell_period;
    qVse->ParamByName("Ist_fin_id")->AsInteger() = aIst_fin_id;
    qVse->ParamByName("Cell_din_izm")->AsString() = aCell_din_izm;
    qVse->ParamByName("Plan_id")->AsInteger() = aPlan_id;
    qVse->ParamByName("Vid_otc")->AsInteger() = aVid_otc;
    qVse->ParamByName("Value_date")->AsDateTime() = aValue_date;
    qVse->Open();
    if (!qVse->EoF())
        aValuePodcin = qVse->Fields[0].AsFloat();
    delete qVse;

    if (std::abs(aValuePodcin - aValueOwner) > 0.000001) {
        std::string s;
        if (aIsValue) // введено во владельца
            s = "Введенное значение показателя <" + aPokaz_name + ">, равное " + std::to_string(aValueOwner) +
                " меньше максимального значения подпоказателей, равного " + std::to_string(aValuePodcin) +
                ". ИСПРАВЬТЕ значение";
        else
            s = "Введенное (вычисленное) значение подпоказателя, равное " + std::to_string(aValuePodcin) + ", " +
                "превышает значение показателя - владельца  <" + aPokaz_name + ">, равное " + std::to_string(aValueOwner) +
                ". ИСПРАВЬТЕ значение";
        Result = false;
        if (!aIsValue && aDelta < 0)
            Result = true; // 06.04.2020 - уменьшение разрешаем
    }

    return Result;
}

bool TUpdDBbyValue::CheckIzNixNatur(bool aIsValue, std::string aPokaz_name, int aNp_vid_id, int aPokaz_base_id, int aPlan_id, unsigned char aVid_otc,
                                    TDateTime aValue_date, double aValue, int aCell_period, int aIst_fin_id, std::string aCell_din_izm) {
        bool CheckIzNixNatur = true;
        double sum;
        // выборка значения
        if (!aIsValue) {
            aValue = 0.0;
            TQueryUni *qVal = new TQueryUni(DefDBconnect.Conn);
            qVal->SQL->Add("SELECT value FROM Pokaztl a, Pokaztl b, Cell c, Cell_value d");
            qVal->SQL->Add("WHERE a.pokaz_id = :Pokaz_base_id");
            qVal->SQL->Add("AND b.pokaz_base_id = a.pokaz_id");
            qVal->SQL->Add("AND b.pokaz_type = 1");
            qVal->SQL->Add("AND b.np_vid_id = :Np_vid_id");
            qVal->SQL->Add("AND c.pokaz_id = b.pokaz_id");
            qVal->SQL->Add("AND c.cell_period = :Cell_period");
            qVal->SQL->Add("AND c.ist_fin_id = :Ist_fin_id");
            qVal->SQL->Add("AND c.cell_din_izm = :Cell_din_izm");
            qVal->SQL->Add("AND d.cell_id = c.cell_id");
            qVal->SQL->Add("AND d.plan_id = :Plan_id");
            qVal->SQL->Add("AND d.vid_otc = :Vid_otc");
            qVal->SQL->Add("AND d.value_date = :Value_date");
            qVal->ParamByName("Pokaz_base_id")->AsInteger() = aPokaz_base_id;
            qVal->ParamByName("Np_vid_id")->AsInteger() = aNp_vid_id;
            qVal->ParamByName("Cell_period")->AsInteger() = aCell_period;
            qVal->ParamByName("Ist_fin_id")->AsInteger() = aIst_fin_id;
            qVal->ParamByName("Cell_din_izm")->AsString() = aCell_din_izm;
            qVal->ParamByName("Plan_id")->AsInteger() = aPlan_id;
            qVal->ParamByName("Vid_otc")->AsInteger() = aVid_otc;
            qVal->ParamByName("Value_date")->AsDateTime() = aValue_date;
            qVal->Open();
            if (!qVal->EoF())
                aValue = qVal->FieldByName("value")->AsDouble();
            delete qVal;
        }
        // End of выборка значения

        TQueryUni *qVse = new TQueryUni(DefDBconnect.Conn);
        qVse->SQL->Add("SELECT SUM(value) FROM Pokaztl a, Pokaztl b, Cell c, Cell_value d");
        qVse->SQL->Add("WHERE a.pokaz_owner_id = :Pokaz_base_id");
        qVse->SQL->Add("AND b.pokaz_base_id = a.pokaz_id");
        qVse->SQL->Add("AND b.pokaz_type = 1");
        qVse->SQL->Add("AND b.np_vid_id = :Np_vid_id");
        qVse->SQL->Add("AND c.pokaz_id = b.pokaz_id");
        qVse->SQL->Add("AND c.cell_period = :Cell_period");
        qVse->SQL->Add("AND c.ist_fin_id = :Ist_fin_id");
        qVse->SQL->Add("AND c.cell_din_izm = :Cell_din_izm");
        qVse->SQL->Add("AND d.cell_id = c.cell_id");
        qVse->SQL->Add("AND d.plan_id = :Plan_id");
        qVse->SQL->Add("AND d.vid_otc = :Vid_otc");
        qVse->SQL->Add("AND d.value_date = :Value_date");
        qVse->ParamByName("Pokaz_base_id")->AsInteger() = aPokaz_base_id;
        qVse->ParamByName("Np_vid_id")->AsInteger() = aNp_vid_id;
        qVse->ParamByName("Cell_period")->AsInteger() = aCell_period;
        qVse->ParamByName("Ist_fin_id")->AsInteger() = aIst_fin_id;
        qVse->ParamByName("Cell_din_izm")->AsString() = aCell_din_izm;
        qVse->ParamByName("Plan_id")->AsInteger() = aPlan_id;
        qVse->ParamByName("Vid_otc")->AsInteger() = aVid_otc;
        qVse->ParamByName("Value_date")->AsDateTime() = aValue_date;
        qVse->Open();
        if (!qVse->EoF())
            sum = qVse->Fields[0].AsFloat();
        delete qVse;

        if (std::fabs(sum - aValue) > 0.000001) {
            std::string s = "Сумма значений подпоказателей, равная " + std::to_string(sum) + ", " +
                           "превышает значение Из Них - показателя <" + aPokaz_name + ">, равное " + std::to_string(aValue) +
                           ". ИСПРАВЬТЕ значение";
            throw std::runtime_error("Ошибка в Из-Них");
            CheckIzNixNatur = false;
        }

        return CheckIzNixNatur;
    }

void TUpdDBbyValue::SetOwnerNatur(TQueryUni* vCell, TQueryUni* vPokaztl, int aPlan_id, int aVid_otc, int aGod,
                                  int aKv, int aMes, double aDelta, bool bAddRec, bool aIzNix, bool aIsMsg) {
    int vNp_vid_id, Pokaz_base_id, Pokaz_owner_id;
    TDateTime aDate;
    vNp_vid_id = vPokaztl->FieldByName("np_vid_id")->AsInteger();
    Pokaz_base_id = vPokaztl->FieldByName("pokaz_base_id")->AsInteger();
    if (Pokaz_base_id == 0) return;
    TQueryUni *qPokazTemp = new TQueryUni(DefDBconnect.Conn);
    qPokazTemp->SQL->Add("SELECT * FROM Pokaztl ");
    qPokazTemp->SQL->Add("WHERE pokaz_id = :CurrPokazId");

    qPokazTemp->ParamByName("CurrPokazId")->AsInteger() = Pokaz_base_id;
    qPokazTemp->Open();
    if (qPokazTemp->EoF()) {
        delete qPokazTemp;
        return;
    }

    if (qPokazTemp->FieldByName("pokaz_vert")->AsInteger() == 2) {
        aDate = GetLastDayDate(aGod, aKv, aMes);
        if (aIzNix) {
            // TUpdDBbyValue::CheckIzNixNatur(false, qPokazTemp->FieldByName("pokaz_name")->AsString(),
            //           vNp_vid_id, qPokazTemp->FieldByName("pokaz_id")->AsInteger(),
            //           aPlan_id, aVid_otc, aDate, 0.0,
            //           vCell->FieldByName("cell_period")->AsInteger(),
            //           vCell->FieldByName("ist_fin_id")->AsInteger(),
            //           vCell->FieldByName("cell_din_izm")->AsString());
        }
    }

    TQueryUni *qPokazCell = new TQueryUni(DefDBconnect.Conn);
    qPokazCell->SQL->Add("SELECT * FROM Pokaztl a, Cell b, Edizm");
    qPokazCell->SQL->Add("WHERE a.pokaz_base_id = :PokazBaseId");
    qPokazCell->SQL->Add("AND a.np_vid_id = :NpVidId");
    qPokazCell->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    qPokazCell->SQL->Add("AND b.cell_period = :Period");
    qPokazCell->SQL->Add("AND b.ist_fin_id = :IstFinId");
    qPokazCell->SQL->Add("AND b.cell_din_izm = :DinIzm");
    qPokazCell->SQL->Add("AND Edizm.edizm_id = a.edizm_id");

    TQueryUni *qPokazVsegoCell = new TQueryUni(DefDBconnect.Conn);
    qPokazVsegoCell->SQL->Add("SELECT * FROM Pokaztl a, Pokaztl b, Cell c, Edizm");
    qPokazVsegoCell->SQL->Add("WHERE a.pokaz_id = :CurrPokazId");
    qPokazVsegoCell->SQL->Add("AND b.pokaz_base_id = a.pokaz_id");
    qPokazVsegoCell->SQL->Add("AND b.np_vid_id = :NpVidId");
    qPokazVsegoCell->SQL->Add("AND c.pokaz_id = b.pokaz_id");
    qPokazVsegoCell->SQL->Add("AND c.cell_period = :Period");
    qPokazVsegoCell->SQL->Add("AND c.ist_fin_id = :IstFinId");
    qPokazVsegoCell->SQL->Add("AND c.cell_din_izm = :DinIzm");
    qPokazVsegoCell->SQL->Add("AND Edizm.edizm_id = b.edizm_id");

    while (true) {
        Pokaz_owner_id = qPokazTemp->FieldByName("pokaz_owner_id")->AsInteger();
        if (Pokaz_owner_id == 0) break;
        qPokazTemp->Close();
        qPokazTemp->ParamByName("CurrPokazId")->AsInteger() = Pokaz_owner_id;
        qPokazTemp->Open();
        if (qPokazTemp->EoF()) break;

        if (qPokazTemp->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
            aDate = GetLastDayDate(aGod, aKv, aMes);
            if (aIzNix) {
                // TUpdDBbyValue::CheckIzNixNatur(false, qPokazTemp->FieldByName("pokaz_name")->AsString(),
                //           vNp_vid_id, qPokazTemp->FieldByName("pokaz_id")->AsInteger(),
                //           aPlan_id, aVid_otc, aDate, 0.0,
                //           vCell->FieldByName("cell_period")->AsInteger(),
                //           vCell->FieldByName("ist_fin_id")->AsInteger(),
                //           vCell->FieldByName("cell_din_izm")->AsString());
            }
        }

        if (qPokazTemp->FieldByName("pokaz_is_razd")->AsInteger() == 1 && qPokazTemp->FieldByName("pokaz_vsego_id")->AsInteger() > 0) {
            qPokazVsegoCell->Close();
            qPokazVsegoCell->ParamByName("CurrPokazId")->AsInteger() = qPokazTemp->FieldByName("pokaz_vsego_id")->AsInteger();
            qPokazVsegoCell->Open();
            if (qPokazVsegoCell->EoF()) break;

            TUpdDBbyValue::Set3DValue(qPokazVsegoCell, qPokazVsegoCell, aPlan_id, aVid_otc, aGod, aKv, aMes,
                                       aDelta, true, bAddRec, false, false, 0, aIsMsg);
        }
    }
    delete qPokazTemp;
    delete qPokazCell;
    delete qPokazVsegoCell;
}






























