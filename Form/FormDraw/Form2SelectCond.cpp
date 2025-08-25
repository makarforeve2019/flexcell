#include "../SystFuncs/SystFuncsLogic.h"
#include "../Form/FormDraw/FormDopInp.h"
#include "TT_Korteg.h"
#include "../DB/unDefDBconnect.h"
#include "Form1.h"
#include "Form2Plus.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../DB/DB_FormStr.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/DB_PlanStr.h"
#include "../DB/DB_Plan.h"
#include "Form2find.h"
#include "../DB/DB_ComonNPokaz.h"
#include "../DB/DB_bkMetaDef.h"
#include "../DB/DB_EdIzm.h"
#include "../DB/DB_Cell.h"
#include "../Pokazateli/unIDsClassFieldCell.h"
#include "Form2SelectCond.h"

// Формирует подзапросы для включения в Where, чтобы проверить значения из CellValue на ограничения
// При этом предполагаем, что в соответствующие столбцы уже положены введенные значения - ограничения
// фильтр на столбец будет собран в subWhere и выглядит в виде:
//    AND (подзапрос), если столбец не состоит в OR конструкции
//    AND ((подзапрос), если столбец - первый операнд в OR конструкции
//    OR (подзапрос), если столбец - промежуточный операнд в OR конструкции
//    OR (подзапрос)), если столбец - последний операнд в OR конструкции
// Полный фильтр будет положен в aAndWhere путем конкатенирования каждого subWhere
bool TForm2SelectCond::doForm2SelectCond(int aPlan_id, std::string& aAndWhere) {
    int nClmn;
    std::string Stmnt, subWhere, subWhereBeg, subWhereEnd;
    std::string ws1, ws2, andWs2, ws3, ws4, ws5, ws6, ws7, ws8, ws9, wsOP;
    std::string sFrom, sCell_id, sPlan_id, sVid_otc;
    int wPeriodType = 0, wfield_id, wcell_idFiltr;
    std::string wShablon;
    int inOR = 0; // для включения OR конструкции. Работает так:
              // 0 - не OR конструкции
              // 1 - первый операнд OR конструкции
              // 2 - промежуточный операнд OR конструкции
              // 3 - последний операнд OR конструкции
    

    aAndWhere = "";

    for (nClmn = 1; nClmn <= p->MaxHrCol; nClmn++) {
        if (nClmn == p->PokazCol) continue;

        PAtrStmnt AtrStmnt = &(p->ListStmnt[nClmn]);
        Stmnt = AtrStmnt->Stmnt;

        // проверяем, что ЗНАЧЕНИЕСВ        
        if (!(AtrStmnt->nValidator >= 10 && AtrStmnt->nValidator <= 19))
            continue;

       // предполагаем, что в Value1, Value2 и Shablon положены значения
       //  тем самым будут в Value1, Value2 заменены значения-признаки, говорящие о смысле этого значения
       //  например, наследуемое, начало текущего года и т.д.
       
        wfield_id = 0;

        if (AtrStmnt->field_id > 0) // 27.04.2020
            sFrom = " FROM Cell_value WHERE objectid = b.pokaz_id";
        else {
            if (work_with == 4) // Это иерархия
                wfield_id = TFormDopInpMF::getIdFromNote2(AtrStmnt->note2);
            if (wfield_id > 0)
                sFrom = " FROM Cell_value WHERE objectid = b.pokaz_id";
            else if (AtrStmnt->nValidator == 15) // 28.11.2023
                sFrom = "";
            else
                sFrom = " FROM Cell_value WHERE objectid = 0";
        }

        if (AtrStmnt->cell_idFiltr > 0) // 27.04.2020
            sCell_id = " AND cell_id = " + std::to_string(AtrStmnt->cell_idFiltr);
        else {
            switch (AtrStmnt->PeriodType) {
            case 0: wPeriodType = 0; break;
            case 1: wPeriodType = 1; break;
            case 4: wPeriodType = 2; break;
            case 6: wPeriodType = 3; break;
            }

            if (work_with == 4 && wfield_id > 0) { // Это иерархия
                wcell_idFiltr = TDB_Cell::GetCellId(wfield_id, wPeriodType, AtrStmnt->ist_fin_grad_id);
                sCell_id = " AND cell_id = " + std::to_string(wcell_idFiltr);
                AtrStmnt->vid_otchFiltr = AtrStmnt->vid_otch; // needed later
            }
            else
                sCell_id = " AND cell_id = (SELECT cell_id FROM Cell WHERE Cell.pokaz_id = b.pokaz_id AND Cell.cell_period = " + std::to_string(wPeriodType) +
                    " AND Cell.ist_fin_id = " + std::to_string(AtrStmnt->ist_fin_grad_id) +
                    ")";
        }

        sPlan_id = " AND plan_id = " + std::to_string(aPlan_id);
        sVid_otc = " AND vid_otc = " + std::to_string(AtrStmnt->vid_otchFiltr);

        if (inOR == 1)
            inOR = 2;
        if (AtrStmnt->Shablon.substr(0, 2) == "$B") {
            if (inOR == 1 || inOR == 2) {
                // ОШИБКА - предыдущая OR конструкция не закрыта                
                throw std::runtime_error("ОШИБКА - предыдущая OR конструкция не закрыта, столбец " + std::to_string(nClmn));
            }
            inOR = 1;
        }
        if (AtrStmnt->Shablon.substr(0, 2) == "$E") {
            if (inOR == 0) {
                // ОШИБКА - OR конструкция не открыта                
                throw std::runtime_error("ОШИБКА - OR конструкция не открыта, столбец " + std::to_string(nClmn));
            }
            inOR = 3;
        }
        wShablon = AtrStmnt->Shablon;
        AnsiReplaceStr(wShablon, "$B", "");
        AnsiReplaceStr(wShablon, "$E", "");
        switch (inOR) {
        case 0:
            subWhereBeg = " AND ";
            subWhereEnd = "";
            break;
        case 1:
            subWhereBeg = " AND (";
            subWhereEnd = "";
            break;
        case 2:
            subWhereBeg = " OR ";
            subWhereEnd = "";
            break;
        case 3:
            subWhereBeg = " OR ";
            subWhereEnd = ")";
            inOR = 0;
            break;
        }

        ws3 = "'" + DateTimeToStr(AtrStmnt->value_dateFiltr) + "'";
        switch (GetDBType()) {
        case TDBType::MSSQL:
            ws2 = "CONVERT(datetime, " + ws3 + " ,104)";
            break;
        case TDBType::Postgre:
            ws2 = "to_date('" + ws3 + "', 'DD.MM.YYYY')";
            break;
        }
        if (AtrStmnt->PeriodType == 0) //  {условно пост}
            andWs2 = "";
        else
            andWs2 = " AND value_date = " + ws2;

        if (int N = AtrStmnt->vid_uslovij; N == 13 || N == 14) { // exists, not exists
            if (AtrStmnt->vid_uslovij == 13)
                subWhereBeg += "EXISTS ";
            else
                subWhereBeg += "NOT EXISTS ";
            subWhere = subWhereBeg + "(SELECT value" +
                sFrom + sCell_id + sPlan_id + sVid_otc +
                andWs2 + ")";
        }
        else
            switch (AtrStmnt->nValidator) {
            case 10: // number
                switch (AtrStmnt->vid_uslovij) {
                case 1: case 2: case 3: case 4: case 5: case 6:
                    switch (AtrStmnt->vid_uslovij) {
                    case 1: wsOP = " = "; break;
                    case 2: wsOP = " <> "; break;
                    case 3: wsOP = " < "; break;
                    case 4: wsOP = " <= "; break;
                    case 5: wsOP = " > "; break;
                    case 6: wsOP = " >= "; break;
                    }
                    subWhere = subWhereBeg + "(SELECT value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        wsOP + std::to_string(AtrStmnt->Value1);
                    break;
                case 7:
                    subWhere = subWhereBeg + "(SELECT value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " BETWEEN " + std::to_string(AtrStmnt->Value1) + " and " + std::to_string(AtrStmnt->Value2);
                    break;
                case 8:
                    subWhere = subWhereBeg + "(SELECT value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " IN (" + wShablon + ")";
                    break;
                case 9:
                    subWhere = subWhereBeg + "(SELECT value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " NOT IN (" + wShablon + ")";
                    break;
                case 15:
                    subWhere = subWhereBeg + "coalesce((SELECT value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + "), 0) = 0";
                    break;
                }
                break;
            case 11: // string
                switch (AtrStmnt->vid_uslovij) {
                case 1: case 2:
                    switch (AtrStmnt->vid_uslovij) {
                    case 1: wsOP = " = "; break;
                    case 2: wsOP = " <> "; break;
                    }
                    subWhere = subWhereBeg + "(SELECT str_value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        wsOP + "'" + wShablon + "'";
                    break;
                case 11:
                    subWhere = subWhereBeg + "(SELECT str_value" +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " LIKE '" + wShablon + "'";
                    break;
                }
                break;
            case 12: // enumeration
                switch (GetDBType()) {
                case TDBType::MSSQL:
                    ws1 = "CAST(value as int)";
                    break;
                case TDBType::Postgre:
                    ws1 = "CAST(value as integer)";
                    break;
                }
                switch (AtrStmnt->vid_uslovij) {
                case 8:
                    subWhere = subWhereBeg + "(SELECT " + ws1 +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " IN (" + wShablon + ")";
                    break;
                case 9:
                    subWhere = subWhereBeg + "(SELECT " + ws1 +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " NOT IN (" + wShablon + ")";
                    break;
                }
                break;
            case 13: // element from
                switch (GetDBType()) {
                case TDBType::MSSQL:
                    ws1 = "CAST(value as int)";
                    break;
                case TDBType::Postgre:
                    ws1 = "CAST(value as integer)";
                    break;
                }
                switch (AtrStmnt->vid_uslovij) {
                case 8:
                    subWhere = subWhereBeg + "(SELECT " + ws1 +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " IN (" + wShablon + ")";
                    break;
                case 9:
                    subWhere = subWhereBeg + "(SELECT " + ws1 +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " NOT IN (" + wShablon + ")";
                    break;
                }
                break;
            case 14: // date
                ws6 = "'" + std::to_string(AtrStmnt->Value1) + "'";
                ws7 = "'" + std::to_string(AtrStmnt->Value2) + "'";
                switch (GetDBType()) {
                case TDBType::MSSQL:
                    ws1 = "CONVERT(datetime, str_value ,104)";
                    ws4 = "CONVERT(datetime, " + ws6 + " ,104)";
                    ws5 = "CONVERT(datetime, " + ws7 + " ,104)";
                    break;
                case TDBType::Postgre:
                    ws1 = "to_date(str_value, 'DD.MM.YYYY')";
                    ws4 = "to_date('" + ws6 + "', 'DD.MM.YYYY')";
                    ws5 = "to_date('" + ws7 + "', 'DD.MM.YYYY')";
                    break;
                }
                switch (AtrStmnt->vid_uslovij) {
                case 10:
                    subWhere = subWhereBeg + "(SELECT " + ws1 +
                        sFrom + sCell_id + sPlan_id + sVid_otc +
                        andWs2 + ")" +
                        " BETWEEN " + ws4 + " and " + ws5;
                    break;
                }
                break;
            case 15: // KBK
                ws1 = "''";
                subWhere = subWhereBeg + sFrom + " b.pokaz_bk_code LIKE " + ws1 + AtrStmnt->Shablon + ws1;
                break;
            }

        subWhere += subWhereEnd;

        aAndWhere += subWhere;
    }

    if (inOR == 1 || inOR == 2) {
        // ERROR - last OR not closed
        throw std::runtime_error("ОШИБКА - последняя OR конструкция не закрыта");
    }

    return true;
}

// Эта процедура просматривает описания столбцов в списке p.ListStmnt и выполняет следующие действия:
//   - Если значение параметр - наследуется (признак ?), то в диалоге м пользователем получает это значение
//   - в p.ListStmnt[столбец] вычисляет поля cell_idFiltr, vid_otchFiltr, value_dateFiltr, которые используются
//       в дальнейшем при построении запроса - чтения зачения из Cell_Value
//   - кладет в заголовок формы введенные (?) значения, если там есть такие параметры

// p: TwwStringList доступно напрямую
void TForm2SelectCond::doEvalAtrSelectCond(PDopAtrCol DopAtrCol, int god, int CurrVidOtch, int CurrQuarter, int CurrMonth, int condLayerNumber) {
    int nClmn, field_id, cell_period, cell_idFiltr, vid_otchFiltr, pokaz_type;
    std::string Stmnt, fieldName;
    PAtrStmnt AtrStmnt;
    std::string sFrom, sCell_id, sPlan_id, sVid_otc;
    TDateTime value_dateFiltr, DateDiapazonFrom, DateDiapazonTo;
    std::string Rzdl, CS, VR, KOSGU, codeKBK;
    int godNew, valuePeriodNew;


    if (condLayerNumber != 0) {
        PrepareAtrSelectCondShablon(DopAtrCol, god, CurrVidOtch, condLayerNumber);
    }

    for (nClmn = 1; nClmn <= p->MaxHrCol; nClmn++) {
        if (nClmn == p->PokazCol) continue;

        AtrStmnt = &(p->ListStmnt[nClmn]);
        Stmnt = AtrStmnt->Stmnt;

        if (!(AtrStmnt->nValidator >= 10 && AtrStmnt->nValidator <= 19))
            continue;

        if (!(Stmnt == "ЗНАЧЕНИЕСВ" || Stmnt == "КОДБК"))
            continue;

        if (Stmnt == "КОДБК") {
            // сейчас реализован вариант задания в диалоге шаблона КБК - случай, когда Value1 = 0
            // Поэтому пока никакой проверки не производится

            DopAtrCol->isVPredelaxDiapazonaKBK = true;
            DopAtrCol->isVPredelaxDiapazonaClmn = nClmn;

            if (GlobalVars->KBKfilter == "") {
                if (AtrStmnt->Value1 == 0) {
                    // --TVvodKBKFiltr.doVvodKBKFiltr(god, Rzdl, CS, VR, KOSGU, codeKBK);
                    Rzdl = InfoFormLogic->Rzdl;
                    CS = InfoFormLogic->CS;
                    VR = InfoFormLogic->VR;
                    KOSGU = InfoFormLogic->KOSGU;
                    codeKBK = InfoFormLogic->codeKBK;
                }
                if (codeKBK == "") {
                    Rzdl = Rzdl + std::string(4 - Rzdl.length(), '_');
                    CS = CS + std::string(TDB_bkMetaDef::GetDl_CS(god) - CS.length(), '_');
                    VR = VR + std::string(3 - VR.length(), '_');
                    KOSGU = KOSGU + std::string(3 - KOSGU.length(), '_');
                    codeKBK = Rzdl + CS + VR + KOSGU;
                }
                if (AtrStmnt->Value1 == 1)
                    codeKBK = AtrStmnt->Shablon; // берем заднное заранее
                AtrStmnt->Shablon = codeKBK;
                GlobalVars->KBKfilter = codeKBK;
            }
            else {
                AtrStmnt->Shablon = GlobalVars->KBKfilter;
            }
            TForm2Plus::ZamenitKBKFiltr(FormLogicF1, p, AtrStmnt->Shablon);
        }
        else if (Stmnt == "ЗНАЧЕНИЕСВ") {
            field_id = AtrStmnt->field_id;
            fieldName = TDB_Pokaztl::GetPokazName(field_id);
            switch (AtrStmnt->PeriodType) {
            case 0: cell_period = 0; break;
            case 1: cell_period = 1; break;
            case 4: cell_period = 2; break;
            case 6: cell_period = 3; break;
            }
            cell_idFiltr = TDB_Cell::GetCellId(field_id, cell_period, AtrStmnt->ist_fin_grad_id);
            vid_otchFiltr = AtrStmnt->vid_otch;
            if (vid_otchFiltr == 0)
                vid_otchFiltr = CurrVidOtch;

            godNew = god;
            switch (p->ListStmnt[nClmn].is_set_god) {
            case 0: // наследуется
                if (p->ListStmnt[nClmn].god_plus != 0)
                    godNew = god + p->ListStmnt[nClmn].god_plus;
                break;
            case 1: godNew = p->ListStmnt[nClmn].god_ravno; break;
            }

            value_dateFiltr = EncodeDate(godNew, 1, 31);
            valuePeriodNew = AtrStmnt->ValuePeriod;
            if (valuePeriodNew == 0) {
                switch (AtrStmnt->PeriodType) {
                case 4: valuePeriodNew = CurrQuarter; break;
                case 6: valuePeriodNew = CurrMonth; break;
                }
            }
            switch (AtrStmnt->PeriodType) {
            case 0:
                value_dateFiltr = EncodeDate(godNew, 01, 02);
                break;
            case 1:
                value_dateFiltr = EncodeDate(godNew, 12, 31);
                break;
            case 4:
                value_dateFiltr = IncMonth(value_dateFiltr, valuePeriodNew * 3 - 1);
                break;
            case 6:
                value_dateFiltr = IncMonth(value_dateFiltr, valuePeriodNew - 1);
                break;
            }
            
            AtrStmnt->cell_idFiltr = cell_idFiltr;
            AtrStmnt->vid_otchFiltr = vid_otchFiltr;
            AtrStmnt->value_dateFiltr = value_dateFiltr;

            pokaz_type = TDB_Pokaztl::GetTypePokaz(field_id);
            switch (pokaz_type) {
            case 1: // число
                // 2023.06.07 esm
                TForm2SelectCond::ParseShablonChislo(condLayerNumber, AtrStmnt);
                break;
            case 3: // перечисление
                break;
            case 5: // дата
                if (int N = AtrStmnt->vid_uslovij; N == 13 || N == 14) // 20.08.2021
                    continue;
                if (AtrStmnt->Value1 == 0 && AtrStmnt->Value2 == 0) { // 05.09.2021
                    if (!InfoFormLogic->isTimePeriod) {
                        AtrStmnt->Value1 = 0;
                        AtrStmnt->Value2 = 0;
                    }
                    else {
                        AtrStmnt->Value1 = ConvertToDouble(InfoFormLogic->DateDiapazonFrom);
                        AtrStmnt->Value2 = ConvertToDouble(InfoFormLogic->DateDiapazonTo);
                    }
                }
                else {
                    DateDiapazonFrom = AtrStmnt->Value1;
                    DateDiapazonTo = AtrStmnt->Value2;
                }
                TForm2Plus::ZamenitDateFiltr(FormLogicF1, p, DateDiapazonFrom, DateDiapazonTo);
                break;
            }
        }
    }
}

void TForm2SelectCond::PrepareAtrSelectCondShablon(PDopAtrCol DopAtrCol, int god, int CurrVidOtch, int CondLayerNumber) {
    for (int iCol = 1; iCol <= p->MaxHrCol; iCol++) {
        if (iCol == p->PokazCol) {
            continue;
        }

        PAtrStmnt AtrStmnt = &(p->ListStmnt[iCol]);

        // Если валидатор больше 1000,
        // значит это заглушка от выполнения Вариантов шаблонов в строке, где определены просто условия.      
        if (AtrStmnt->nValidator > 1000) {
            AtrStmnt->nValidator -= 1000;
        }
    }
}

// Если нужно отделить условия, заданные в Вариантах от обычных условий на стольцы:
// nValidator += 1000. Затем, в PrepareAtrSelectCondShablon возвращаем назад: nValidator -= 1000.
void TForm2SelectCond::ParseShablonChislo(int condLayerNumber, PAtrStmnt AtrStmnt) {
        std::string shablonString = AtrStmnt->Shablon;
        shablonString = AnsiReplaceStr(shablonString, "$B", "");
        shablonString = AnsiReplaceStr(shablonString, "$E", "");

        // Если у строки не настроен фильтр по вариантам условий на стольцы, а, например, просто условия на стольцы.        
        if (condLayerNumber == 0) {
          // и шаблоня можно определиь как json-строк(начинается на "{").
          // тогда ставим заглушку от выполнения условия          
            if (AnsiStartsText("{", shablonString)) {
                AtrStmnt->nValidator = 1010;
            }
            return;
        }

        bool ORBegin = false;
        bool OREnd = false;

        try {
            nlohmann::json shablon = nlohmann::json::parse(shablonString);

            // Если есть в колонке описание варианта с нужным нам номером            
            if (shablon.contains(std::to_string(condLayerNumber))) {
                nlohmann::json layer = shablon[std::to_string(condLayerNumber)];
                AtrStmnt->vid_uslovij = layer["vid_uslovij"].get<int>();
                if (layer.contains("Value1")) {
                    AtrStmnt->Value1 = layer["Value1"].get<double>();
                }
                if (layer.contains("Value2")) {
                    AtrStmnt->Value2 = layer["Value2"].get<double>();
                }
                if (layer.contains("OR")) {
                    std::string orValue = layer["OR"].get<std::string>();
                    if (orValue == "begin") {
                        ORBegin = true;
                    } else if (orValue == "end") {
                        OREnd = true;
                    }
                }

                if (ORBegin) {
                    AtrStmnt->Shablon = "$B" + shablonString;
                }
                if (OREnd) {
                    AtrStmnt->Shablon = "$E" + shablonString;
                }
            }
            // Если в шаблоне нет нужного нам варианта или шаблон не прочитался,
            // ставим заглушку.            
            else {
                AtrStmnt->nValidator = 1010;
            }
        } catch (...) {
            ;
            //????????????shablon.Free;
        }
    }





