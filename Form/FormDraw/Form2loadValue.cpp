
#include "Form2Plus.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "dmGlobalVars.h"
#include "../unProgress.h"
#include "../DB/DB_CellValue.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/DB_Enum.h"
#include "../DB/DB_BPoluc.h"
#include "../DB/DB_NMrpr.h"
#include "../DB/DB_UtocPlana.h"
#include "../DB/DB_PlanStr.h"
#include "../DB/DB_FormStrTMP.h"
#include "../DB/DB_BPolucRaj.h"
#include "../DB/DB_Plan.h"
#include "../DB/DB_Dgvr.h"
#include "../DB/DB_UtocKP.h"
#include "../DB/DB_bkKBK.h"
#include "../DB/DB_LinksObjects.h"
#include "../DB/DB_IstFin.h"
#include "Form2PlusPatternKBK.h"
#include "FormDopInp.h"
#include "Form1.h"
#include "TT_ValForMesjc.h"
#include "TT_ValForPeriod.h"
#include "TT_PostuplForPeriod.h"
#include "evalComonNPokaz_Logic.h"
#include "TT_SummaBySubjects.h"
#include "FormSsilkaNaStr.h"
#include "unDB_RP_067eval.h"
#include "unCO_RP_015eval.h"
#include "unEkonomijBPSeval.h"
#include "unPeremescenij.h"
#include "Form2loadValue.h"

// устанавливает первоначальные значения
void TForm2loadValue::SetInputValue(TFormLogicF1* FormLogicF1, TwwStringList* p, bool isRowMode, int aRow) {
    TracerManager::Instance().StartFunctionTrace("TForm2loadValue::SetInputValue");
    TValForMesjc::CreateObjValForMesjc(); // создает объект класса TValForMesjc
    TValForPeriod::CreateObjValForPeriod(); // создает объект класса TValForPeriod
    TPostuplForPeriod::CreateObjValForPeriod(); // создает объект класса TPostuplForPeriod
    TSummaBySubjects::CreateObjSummaBySubjects();// создает объект класса TSummaBySubjects
    //mak ??? TValForAgregat::CreateStringList();
    TDB_RP_067eval::createObj();
    TCO_RP_015eval::createObj();
    //mak ??? TEkonomijBPSeval::createObj();

    TForm2loadValue Form2loadValue;
    Form2loadValue.SetInputValueDalee(FormLogicF1, p, isRowMode, aRow);
    //--delete Form2loadValue;

    TValForMesjc::FreeObjValForMesjc();// освобождает объект класса TValForMesjc
    TPostuplForPeriod::FreeObjValForPeriod();// освобождает объект класса TPostuplForPeriod
    TValForPeriod::FreeObjValForPeriod(); // освобождает объект класса TValForPeriod
    TSummaBySubjects::FreeObjSummaBySubjects();// освобождает объект класса TSummaBySubjects
    //mak ??? TValForAgregat::FreeStringList();
   TDB_RP_067eval::freeObj();
   TCO_RP_015eval::freeObj();
    //mak ??? TEkonomijBPSeval::freeObj(TEkonomijBPSeval* obj);
    TracerManager::Instance().EndFunctionTrace();
}

// если isRowMode = true, то обрабатывать только ячейки строки aRow
void TForm2loadValue::SetInputValueDalee(TFormLogicF1* FormLogicF1, TwwStringList* p, bool isRowMode, int aRow) {
    // ввод данных
    sc.BeginAdd();
    sc.NBlock = 0;
    // цикл ввода данных, условно-постоянных
    scUP.BeginAdd();
    scUP.NBlock = 0;
    // цикл ввода данных для суммы
    scSum.BeginAdd();
    scSum.NBlock = 0;

    scAgr.BeginAdd();
    scAgr.NBlock = 0;

    scAgrSum.BeginAdd();
    scAgrSum.NBlock = 0;

    scComNP.BeginAdd();
    scComNP.NBlock = 0;

    scSumObAlb.BeginAdd();
    scSumObAlb.NBlock = 0;

    scComNpSumObAlb.BeginAdd();
    scComNpSumObAlb.NBlock = 0;

    scHistory.BeginAdd();
    scHistory.NBlock = 0;
    auto stringList = TFormDopInp::GetTww(FormLogicF1);
    auto begin = stringList->items.begin();
    auto end = stringList->items.end();

    for (auto it = begin; it != end; ++it) {
        PCellValueAtr vd = std::get<PCellValueAtr>(it->second);
        union
        {
            struct
            {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.Ref.nRow = vd->Row;
        RowCol.Ref.nCol = vd->Col;


        if (isRowMode && vd->Row != aRow) {
            continue; // не та строка, пропускаем
        }

        // не условно-постоянных
        if (vd->DataType <= (10 + deltaDType) && vd->cell_period > 0) {
//            std::cout << "ne uslovno post" << endl;
            doSC(FormLogicF1, p, RowCol.number, vd);

        }

        // условно-постоянных
        if (vd->DataType <= (10 + deltaDType) && vd->cell_period == 0) {
            doSCUP(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "условно-постоянных" << endl;
        }

        // сумма
        if (vd->DataType == 16 + deltaDType || vd->DataType == 20 + deltaDType || vd->DataType == 41 + deltaDType) {
            doSCSum(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "сумма" << endl;
        }

        if (vd->DataType == 23 + deltaDType && p->ListStmnt[vd->Col].PeriodType != 7) {
            doSCAgr(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "vd->DataType == 23 + deltaDType && p->ListStmnt[vd->Col].PeriodType != 7" << endl;
        }

        if (vd->DataType == 61 + deltaDType || vd->DataType == 62 + deltaDType) {
            doSCAgrSum(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "vd->DataType == 61 + deltaDType || vd->DataType == 62 + deltaDType" << endl;
        }

        if (vd->DataType == 54 + deltaDType) {
            doSCComNP(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "vd->DataType == 54 + deltaDType" << endl;
        }

        if (vd->DataType == 63 + deltaDType && vd->pokaz_type == 1) {
            doSCSumObAlb(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "vd->DataType == 63 + deltaDType && vd->pokaz_type == 1" << endl;
        }

        if (vd->DataType == 64 + deltaDType && vd->pokaz_type == 1) {
            doSCComNPSumObAlb(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "vd->DataType == 64 + deltaDType && vd->pokaz_type == 1" << endl;
        }

        // измерение - условно-постоянное
        if (vd->DataType == 11 && vd->cell_period == 0) {
            doSCUP(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "измерение - условно-постоянное" << endl;
        }

        // Проверка на ячейки с показом Истории изменения значения
        if (vd->EvalType == 3) {
            doSCHistory(FormLogicF1, p, RowCol.number, vd);
            //std::cout << "Проверка на ячейки с показом Истории изменения значения" << endl;
        }

        if (p->work_with != 2) {
            this->doNevicisl(FormLogicF1, p, vd);
        }

        this->doVicisl(FormLogicF1, p, vd);
    }
    sc.Execute(); // получить данные
    sc.sp->Open(); // доступ к записям
    sc.GetItems(FormLogicF1, p); // цикл выборки записей

    scUP.Execute(); // получить данные
    scUP.sp->Open(); // доступ к записям
    scUP.GetItems(FormLogicF1, p); // цикл выборки записей

    if (scSum.CurrNumb > 0) {
        scSum.Execute(); // получить данные
        scSum.sp->Open(); // доступ к записям
        scSum.GetItems(FormLogicF1, p); // цикл выборки записей
    }

    scAgr.Execute();
    scAgr.sp->Open();
    scAgr.GetItems(FormLogicF1, p);

    scAgrSum.Execute();
    scAgrSum.sp->Open();
    scAgrSum.GetItems(FormLogicF1, p);

    scComNP.Execute();
    scComNP.sp->Open();
    scComNP.GetItems(FormLogicF1, p);

    if (scSumObAlb.CurrNumb > 0) {
        scSumObAlb.Execute();
        scSumObAlb.sp->Open();
        scSumObAlb.GetItems(FormLogicF1, p);
    }

    if (scComNpSumObAlb.CurrNumb > 0) {
        scComNpSumObAlb.Execute();
        scComNpSumObAlb.sp->Open();
        scComNpSumObAlb.GetItems(FormLogicF1, p);
    }

    if (scHistory.CurrNumb > 0) {
        scHistory.Execute();
        scHistory.sp->Open();
        scHistory.GetItems(FormLogicF1, p);
    }

    if (p->work_with != 2) {
        for (int i = 0; i < FormLogicF1->f1Forms->LastRow; i++) {
            PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
            if (AtrRow == nullptr) {
                continue;
            }
            switch (AtrRow->CurRowVid) {
                case 15:
                    TForm2PlusPatternKBK::SummaPatternKBK(FormLogicF1, i, p, AtrRow->ObjectId, AtrRow->pokaz_bk_code);
                    break;
            }
        }
    }
}

void TForm2loadValue::doSC(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    sc.AddCell(i, vd);
    if (sc.CurrNumb >= 1000) {
        sc.Execute(); // получить данные
        sc.sp->Open(); // доступ к записям
        sc.GetItems(FormLogicF1, p); // цикл выборки записей
        sc.sp->Close();
        //MAK sc.NBlock = i;
        sc.NBlock = 0;
        sc.BeginAdd();
    }
}

void TForm2loadValue::doSCUP(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    std::string Stmnt;
    PCellValueAtr vd1;
    int Id_Znac;
    TFormatStyle* FormatStyle = new TFormatStyle();
    std::string Value;
    PAtrRow atrRow;

    atrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
    if (p->work_with != 2) {//ky: Объектная форма
        if (atrRow->CurRowVid == 11 || atrRow->CurRowVid == 23) {
            // ФОРМУЛАF1
            delete FormatStyle;
            return;
        }
        if (atrRow->CurRowVid == 14) {
            // ПРИСВОИТЬ
            delete FormatStyle;
            return;
        }
        Stmnt = p->ListStmnt[vd->Col].Stmnt;
    } else {
        Stmnt = "ЗНАЧЕНИЕСВ";//ky: Объектная форма: брать из vd, но в vd еще нет...
    }

    if (Stmnt != "ЗНАЧЕНИЕСВ" && Stmnt != "НАИМПОКАЗальтерн") {
        delete FormatStyle;
        return;
    }

    switch (vd->pokaz_type) {
    case 8: {// связь договора с мероприятием
        Id_Znac = TDB_LinksObjects::GetdPokazByObject(vd->PlanId, vd->ObjectId, vd->PokazId, Value);
        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, vd->Row, vd->Col);
        vd1->EnumId = Id_Znac;
        TFormDopInp::SetAttribByNMrpr(FormLogicF1, vd->Row, vd->Col, //{ ячейка с мероприятием }
                      Id_Znac);
         // устновки для номера мероприятия
        if (!Value.empty()) {
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, *FormatStyle);
            TForm1::SetCellValue(FormLogicF1, nullptr, p->ListStmnt.size(), vd->Row, vd->Col, vd->Row, vd->Col, Value, vd->DataType, FormatStyle);
        }
        delete FormatStyle;
        return;
    }
    }

    scUP.AddCell(i, vd);
    if (scUP.CurrNumb >= 1000) {
        scUP.Execute();
        scUP.sp->Open();
        scUP.GetItems(FormLogicF1, p);
        scUP.sp->Close();
        //MAK scUP.NBlock = i;
        scUP.NBlock = 0;
        scUP.BeginAdd();
    }
    delete FormatStyle;
}


void TForm2loadValue::doSCSum(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    std::string Stmnt = p->ListStmnt[vd->Col].Stmnt;
    int mode;
    if (Stmnt == "Мрпр-Дгвр:Сумма№БО")
        mode = 0;
    else if (Stmnt == "Мрпр-Дгвр:СуммаБО")
        mode = 1;
    else if (Stmnt == "КБК-Дгвр:Сумма№БО")
        mode = 2;
    else if (Stmnt == "КБК-Дгвр:СуммаБО")
        mode = 3;
    else if (Stmnt == "КБК-Дгвр:Сумма№БОоз")
        mode = 4;
    else if (Stmnt == "КБК-Дгвр:СуммаКЗ")
        mode = 5;
    else
        return;

    scSum.AddCell(mode, i, vd);
    if (scSum.CurrNumb >= 1000) {
        scSum.Execute(); // получить данные
        scSum.sp->Open(); // доступ к записям
        scSum.GetItems(FormLogicF1, p); // цикл выборки записей
        scSum.sp->Close();
        //MAK scSum.NBlock = i;
        scSum.NBlock = 0;
        scSum.BeginAdd();
    }
}

void TForm2loadValue::doSCAgr(TFormLogicF1* FormLogicF1, TwwStringList*  p, int i, PCellValueAtr vd) {
    scAgr.AddCell(i, FormLogicF1, vd);
    if (scAgr.CurrNumb >= 1000) {
        scAgr.Execute(); // получить данные
        scAgr.sp->Open(); // доступ к записям
        scAgr.GetItems(FormLogicF1, p); // цикл выборки записей
        scAgr.sp->Close();
        //MAK scAgr.NBlock = i;
        scAgr.NBlock = 0;
        scAgr.BeginAdd();
    }
}

void TForm2loadValue::doSCAgrSum(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    int mode;
    if (vd->DataType == (61 + deltaDType))
        mode = 2;
    else if (vd->DataType == (62 + deltaDType))
        mode = 3;
    else
        return;
    scAgrSum.AddCell(mode, i, FormLogicF1, vd);
    if (scAgrSum.CurrNumb >= 1000) {
        scAgrSum.Execute(); // получить данные
        scAgrSum.sp->Open(); // доступ к записям
        scAgrSum.GetItems(FormLogicF1, p); // цикл выборки записей
        scAgrSum.sp->Close();
        //MAK scAgrSum.NBlock = i;
        scAgrSum.NBlock = 0;
        scAgrSum.BeginAdd();
    }
}

void TForm2loadValue::doSCComNP(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    std::string Stmnt = p->ListStmnt[vd->Col].Stmnt;
    if (Stmnt == "ЗНАЧЕНИЕобщНПИР") {
        return;
    }

    scComNP.AddCell(i, vd);
    if (scComNP.CurrNumb >= 1000) {
        scComNP.Execute(); // получить данные
        scComNP.sp->Open(); // доступ к записям
        scComNP.GetItems(FormLogicF1, p); // цикл выборки записей
        scComNP.sp->Close();
        //MAK scComNP.NBlock = i;
        scComNP.NBlock = 0;
        scComNP.BeginAdd();
    }
}


void TForm2loadValue::doSCSumObAlb(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    bool isSrednee = false;
    if (scSumObAlb.CurrNumb == 0) {
        isSrednee = (p->ListStmnt[vd->Col].Stmnt == "СВОДпоСУБЪвА/n");
        scSumObAlb.AddListPlanIds(FormLogicF1->CurrAlbumId, isSrednee, FormLogicF1->accessalbum_idSelBlockSubj);
    }
    scSumObAlb.AddCell(i, vd);
    if (scSumObAlb.CurrNumb >= 1000) {
        scSumObAlb.Execute(); // получить данные
        scSumObAlb.sp->Open(); // доступ к записям
        scSumObAlb.GetItems(FormLogicF1, p); // цикл выборки записей
        scSumObAlb.sp->Close();
        //MAK  scSumObAlb.NBlock = i;
        scSumObAlb.NBlock = 0;
        scSumObAlb.BeginAdd();
    }
}

void TForm2loadValue::doSCComNPSumObAlb(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    bool isSrednee;
    if (scComNpSumObAlb.CurrNumb == 0) {
        isSrednee = (p->ListStmnt[vd->Col].Stmnt == "СВОДобщНПпоСУБЪвА/n");
        scComNpSumObAlb.AddListPlanIds(FormLogicF1->CurrAlbumId, isSrednee, FormLogicF1->accessalbum_idSelBlockSubj);
    }
    scComNpSumObAlb.AddCell(i, vd);
    if (scComNpSumObAlb.CurrNumb >= 1000) {
        scComNpSumObAlb.Execute(); // получить данные
        scComNpSumObAlb.sp->Open(); // доступ к записям
        scComNpSumObAlb.GetItems(FormLogicF1, p); // цикл выборки записей
        scComNpSumObAlb.sp->Close();
        //MAK scComNpSumObAlb.NBlock = i;
        scComNpSumObAlb.NBlock = 0;
        scComNpSumObAlb.BeginAdd();
    }
}

void setslStmntOboznac1() {
    TStringList* slStmntOboznac1;
    slStmntOboznac1 = GlobalVars->slStmntOboznac1;
    slStmntOboznac1->AddObject("НАИМПОКАЗ", 1);
    slStmntOboznac1->AddObject("forUnikIdentif", 2);
    slStmntOboznac1->AddObject("ПолныйНомер", 3);
    slStmntOboznac1->AddObject("ДатаСоздОбъекта", 4);
    slStmntOboznac1->AddObject("НАИМЦелСт", 5);
    slStmntOboznac1->AddObject("КОДБК", 6);
    slStmntOboznac1->AddObject("КОДБК1", 7);
    slStmntOboznac1->AddObject("Примечание", 8);
    slStmntOboznac1->AddObject("Владелец", 9);
    slStmntOboznac1->AddObject("Ведомство", 10);
    slStmntOboznac1->AddObject("КОДБКРздл", 11);
    slStmntOboznac1->AddObject("КОДБКРздл1", 12);
    slStmntOboznac1->AddObject("КОДБКЦелСт", 13);
    slStmntOboznac1->AddObject("КОДБКЦелСт1", 14);
    slStmntOboznac1->AddObject("КОДБКВидРасх", 15);
    slStmntOboznac1->AddObject("КОДБКЭкКл", 16);
    slStmntOboznac1->AddObject("НомерМрпр", 17);
    slStmntOboznac1->AddObject("БДЖПОЛУЧТЛЬ", 18);
    slStmntOboznac1->AddObject("БДЖПОЛУЧТЛЬ_нет()", 19);
    slStmntOboznac1->AddObject("РАСЧСЧЕТ", 20);
    slStmntOboznac1->AddObject("РАСЧСЧЕТ2", 21);
    slStmntOboznac1->AddObject("ЛицевойСчетТБ", 22);
    slStmntOboznac1->AddObject("ЕДИЗМ", 23);
    slStmntOboznac1->AddObject("ЕДИЗМНВ", 24);
    slStmntOboznac1->AddObject("ЕДИЗМобщНП", 25);
    slStmntOboznac1->AddObject("ИмяИстФин", 26);
    slStmntOboznac1->AddObject("ФОРМУЛАF1", 27);
    slStmntOboznac1->AddObject("КБК-Дгвр:Кол-во", 28);
    slStmntOboznac1->AddObject("КБК-Дгвр:Кол-воБО", 29);
    slStmntOboznac1->AddObject("ЗНАЧЕНИЕобщНПИР", 30);
    slStmntOboznac1->AddObject("NamePlanВстроке", 31);
    slStmntOboznac1->AddObject("OKTMOPlanВстроке", 32);
    slStmntOboznac1->AddObject("ГодВстроке", 33);
    slStmntOboznac1->AddObject("ОткрытьФормуПодчинОб", 34);
    slStmntOboznac1->AddObject("ОткрытьФормуТекущОб", 34);
};

int getStmntCode1(string aStmnt) {
    TStringList* slStmntOboznac1;
    int index;
    slStmntOboznac1 = GlobalVars->slStmntOboznac1;
    if (slStmntOboznac1->Find(aStmnt, index))
        return *std::get<std::shared_ptr<int>>(slStmntOboznac1->Objects[index]);
    else
        return -1;
};

int getStmntCode2(string aStmnt) {
    TStringList* slStmntOboznac2;
    int index;
    slStmntOboznac2 = GlobalVars->slStmntOboznac2;
    if (slStmntOboznac2->Find(aStmnt, index))
        return *std::get<std::shared_ptr<int>>(slStmntOboznac2->Objects[index]);
    else
        return -1;
};

// вставляем невычисляемые значения
void TForm2loadValue::doNevicisl(TFormLogicF1* FormLogicF1, TwwStringList* p, PCellValueAtr vd) {
    string Stmnt;
    PCellValueAtr vd1;
    TFormatStyle* FormatStyle = new TFormatStyle();
    string Value;
    int ValueI;
    PAtrRow AtrRow;
    int wCurRowVid;
    int Id_Znac;
    string wshort_name;
    int StmntCode;

    if (GlobalVars->slStmntOboznac1->Count() == 0)
        setslStmntOboznac1();

    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, *FormatStyle);
    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
    wCurRowVid = AtrRow->CurRowVid;
    switch (wCurRowVid) {// вид текущей строки
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 12:
        case 13:
            Value = "";
            Stmnt = p->ListStmnt[vd->Col].Stmnt;
            StmntCode = getStmntCode1(Stmnt);
            switch (StmntCode) {
                case 1:
                case 2:
                case 3:
                case 4: {// if (Stmnt = 'НАИМПОКАЗ') or (Stmnt = 'forUnikIdentif') or (Stmnt = 'ПолныйНомер') or (Stmnt = 'ДатаСоздОбъекта')
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                                         char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 2, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 5: { // else if (Stmnt = 'НАИМЦелСт') then
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 52 + deltaDType, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 6:
                case 7: {// else if (Stmnt = 'КОДБК') or (Stmnt = 'КОДБК1') then
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 8:
                case 9: { // else if (Stmnt = 'Примечание') or (Stmnt = 'Владелец') then
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 52 + deltaDType, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 10: { // else if (Stmnt = 'Ведомство') then
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16: { // else if (Stmnt = 'КОДБКРздл') or (Stmnt = 'КОДБКРздл1') or
                                  // Stmnt = 'КОДБКЦелСт') or (Stmnt = 'КОДБКЦелСт1') or
                                  //(Stmnt = 'КОДБКВидРасх') or (Stmnt = 'КОДБКЭкКл')
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 17: { // else if Stmnt = 'НомерМрпр' then
                    if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2) {
                        Value = TDB_NMrpr::PokazidToNMP(vd->PokazId, FormLogicF1->CurrYear, Id_Znac);
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                            Value, 0, FormatStyle);
                        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                        FormLogicF1->f1Forms->Protection_Set(true, true);
                    }
                    break;
                }
                case 18:
                case 19: { // else if (Stmnt = 'БДЖПОЛУЧТЛЬ') or (Stmnt = 'БДЖПОЛУЧТЛЬ_нет()')
                    if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2) {
                        switch (TDB_Pokaztl::isTBCast_Limit_OrNo(vd->PokazId)) {
                            case 1:
                                Id_Znac = TDB_BPolucRaj::idRsp_GetBPbyPlanIdTB(vd->PlanId, wshort_name);
                                break;
                            case 2:
                                Id_Znac = TDB_BPolucRaj::idPlc_GetBPbyPlanIdTB(FormLogicF1->CurrYear, vd->PlanId, wshort_name);
                                break;
                            case 3:
                                Id_Znac = 0;
                                wshort_name = "";
                                break;
                        }
                        Value = wshort_name;
                        if (Stmnt == "БДЖПОЛУЧТЛЬ_нет()")
                            Value = DelSkobka(Value);
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                            Value, vd->DataType, FormatStyle);
                        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, vd->Row, vd->Col);
                        vd1->BPId = Id_Znac;
                        TFormDopInp::SetAttribByBP(FormLogicF1, vd->Row, vd->Col);
                        p->nColForBP = vd->Col;
                    }
                    break;
                }
                case 20: { // else if Stmnt = 'РАСЧСЧЕТ' then
                    if (FormLogicF1->CurrYear >= 2008) {
                        if (TFormDopInp::isEstColumnforBP(FormLogicF1, vd->Row) == 0) {
                            Value = "";
                            if (!(TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 3 || TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 5)) {
                                if (!TDB_Pokaztl::isTBCastLimit(vd->PokazId)) {
                                    Value = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(FormLogicF1->CurrYear, vd->PlanId);
                                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                                        Value, 0, FormatStyle);
                                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                                    FormLogicF1->f1Forms->Protection_Set(true, true);
                                }
                            }
                        }
                    }
                    //Вычисляем сразу же как только рассчитан бюджетополучаетль
                    break;
                }
                case 21: { // else if Stmnt = 'РАСЧСЧЕТ2' then
                    if (FormLogicF1->CurrYear < 2008)
                        Value = TDB_BPoluc::GetRascScet2(vd->PlanId);
                    else {
                        Value = "";
                        if (TDB_Pokaztl::isTBCastLimit(vd->PokazId))
                            Value = TDB_BPolucRaj::RSRsp_GetBPbyPlanIdTB(vd->PlanId);
                    }
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        Value, 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 22: { // else if Stmnt = 'ЛицевойСчетТБ' then
                    if (TDB_Pokaztl::isTBCastLimit(vd->PokazId))
                        Value = TDB_BPolucRaj::RSRsp_GetBPbyPlanIdTB(vd->PlanId);
                    else
                        Value = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(FormLogicF1->CurrYear, vd->PlanId);
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        Value, 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    break;
                }
                case 23:
                case 24:
                case 25: { // else if (Stmnt = 'ЕДИЗМ') or (Stmnt = 'ЕДИЗМНВ') or (Stmnt = 'ЕДИЗМобщНП') then
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col)), 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    FormLogicF1->f1Forms->WordWrap = true;
                    break;
                }
                case 26: {// else if Stmnt = 'ИмяИстФин' then
                    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
                    if (AtrRow != NULL) {
                        Value = TDB_IstFin::getIstFinName(AtrRow->ist_fin_grad_id);
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                            Value, 0, FormatStyle);
                        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                        FormLogicF1->f1Forms->Protection_Set(true, true);
                        FormLogicF1->f1Forms->WordWrap = true;
                    }
                    break;
                }
                case 27: // else if Stmnt = 'ФОРМУЛАF1' then
                    // формула уже вставлена
                    break;
                case 28:
                case 29: { // else if (Stmnt = 'КБК-Дгвр:Кол-во') or (Stmnt = 'КБК-Дгвр:Кол-воБО') then
                    if (vd->ObjectId > 0)
                        //throw Exception("Операция Количество договоров не может быть задана для объекта.");
                    if (vd->PokazId > 0) {
                        if (Stmnt == "КБК-Дгвр:Кол-во")
                            ValueI = TDB_Dgvr::GetKolDgvrByKBK(TDB_Pokaztl::GetCodeBK(vd->PokazId), vd->PlanId, FormLogicF1->CurrYear);
                        if (Stmnt == "КБК-Дгвр:Кол-воБО")
                            ValueI = TDB_Dgvr::GetKolDgvrByKBK_NBO(TDB_Pokaztl::GetCodeBK(vd->PokazId), vd->PlanId, FormLogicF1->CurrYear);
                        Value = to_string(ValueI);
                        if (ValueI == 0)
                            Value = "";
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                            Value, 0, FormatStyle);
                        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                        FormLogicF1->f1Forms->Protection_Set(true, true);
                    }
                    break;
                }
                case 30: { // else if (Stmnt = 'ЗНАЧЕНИЕобщНПИР') then
                    TevalComonNPokaz_Logic::showZnacenijCNPallPlans(p, FormLogicF1, vd, vd->Row, vd->Col);
                    break;
                }
                case 31:
                case 32:
                case 33: { // else if (Stmnt = 'NamePlanВстроке') or (Stmnt = 'OKTMOPlanВстроке') or (Stmnt = 'ГодВстроке') then
                    if (Stmnt == "NamePlanВстроке" && AtrRow->plan_id != 0)
                        Value = TDB_Plan::GetPlanName(AtrRow->plan_id);
                    else if (Stmnt == "OKTMOPlanВстроке" && AtrRow->plan_id != 0)
                        Value = TDB_Plan::GetOKATO_kod(AtrRow->plan_id);
                    else if (Stmnt == "ГодВстроке")
                        Value = to_string(AtrRow->god);
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                        Value, 0, FormatStyle);
                    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                    FormLogicF1->f1Forms->WordWrap = true;
                    break;
                }
                case 34: {
                    if (wCurRowVid == 4) {
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                            p->ListStmnt[vd->Col].note1, 0, FormatStyle);
                        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                        FormLogicF1->f1Forms->Protection_Set(true, true);
                        FormLogicF1->f1Forms->WordWrap = true;
                    }
                    break;
                }
            }
            break;

        case 18:
        case 19:
        case 20:
        case 21: {
            Value = "";
            Stmnt = p->ListStmnt[vd->Col].Stmnt;
            if (Stmnt == "КОДБКЦелСт" || Stmnt == "КОДБКЦелСт1") {
                Value = TDB_bkKBK::GetCodeKBK(vd->EnumId);
                if (Stmnt == "КОДБКЦелСт1")
                    Value = SetCodeBK_CST_rzdltl(Value, FormLogicF1->CurrYear, 0);
            }
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                Value, 0, FormatStyle);
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            break;
        }
    }
    if (AtrRow != NULL) {
        if (vd->Col == AtrRow->colReplValue) {
            Value = AtrRow->NameReplValue;
            FormLogicF1->f1Forms->TextRC(vd->Row,vd->Col) = utf8_to_utf16(Value).c_str();
        }
    }

    delete FormatStyle;
}

void setslStmntOboznac2() {
    TStringList* slStmntOboznac2;
    slStmntOboznac2 = GlobalVars->slStmntOboznac2;
    slStmntOboznac2->AddObject("ЗНАЧЕНИЕобщНП", /*(TObject*)*/1);
    slStmntOboznac2->AddObject("ЗНАЧЕНИЕобщНПИР", /*(TObject*)*/2);
    slStmntOboznac2->AddObject("ХарактСВобъекта", /*(TObject*)*/3);
    slStmntOboznac2->AddObject("КонкатСтрокСВобъек", /*(TObject*)*/4);
    slStmntOboznac2->AddObject("Экономия/Резерв", /*(TObject*)*/5);
    slStmntOboznac2->AddObject("отчетCO_RP_015", /*(TObject*)*/6);
    slStmntOboznac2->AddObject("СведенияОбЭкономии", /*(TObject*)*/7);
    slStmntOboznac2->AddObject("ПлнОСТАТОК", /*(TObject*)*/8);
    slStmntOboznac2->AddObject("ФктОСТАТОК", /*(TObject*)*/9);
    slStmntOboznac2->AddObject("ПлнОСТАТОКНИ", /*(TObject*)*/10);
    slStmntOboznac2->AddObject("УТОЧНЕНИЕСВ", /*(TObject*)*/11);
    slStmntOboznac2->AddObject("УТОЧНЕНИЕСВНИ", /*(TObject*)*/12);
    slStmntOboznac2->AddObject("КБК-Дгвр:Сумма№БО", /*(TObject*)*/13);
    slStmntOboznac2->AddObject("КБК-Дгвр:СуммаБО", /*(TObject*)*/14);
    slStmntOboznac2->AddObject("ФактКвДату", /*(TObject*)*/15);
    slStmntOboznac2->AddObject("УТОЧНЕНИЕСВПосле", /*(TObject*)*/16);
    slStmntOboznac2->AddObject("ФактВсегоВНачале", /*(TObject*)*/17);
    slStmntOboznac2->AddObject("ПоступлВсегоВНачале", /*(TObject*)*/18);
    slStmntOboznac2->AddObject("УточПланВклДату", /*(TObject*)*/19);
    slStmntOboznac2->AddObject("СуммаПриход", /*(TObject*)*/20);
    slStmntOboznac2->AddObject("СуммаВозврат", /*(TObject*)*/21);
    slStmntOboznac2->AddObject("СуммаПлГрНИВНачале", /*(TObject*)*/22);
    slStmntOboznac2->AddObject("СуммаПлГрНИГод", /*(TObject*)*/23);
    slStmntOboznac2->AddObject("СуммаПлГрНИВМесяце", /*(TObject*)*/24);
    slStmntOboznac2->AddObject("ПДГ_УТ_КассПл", /*(TObject*)*/25);
    slStmntOboznac2->AddObject("УТВ_УТ_КассПл", /*(TObject*)*/26);
    slStmntOboznac2->AddObject("ЗНАЧЕНИЕСВИР", /*(TObject*)*/27);
    slStmntOboznac2->AddObject("ЗНАЧЕНИЕНВИР", /*(TObject*)*/28);
    slStmntOboznac2->AddObject("ПризнакУчастияВСвязи", /*(TObject*)*/29);
    slStmntOboznac2->AddObject("ЗНАЧЕНИЕСВНИ", /*(TObject*)*/30);
    slStmntOboznac2->AddObject("ЗНАЧЕНИЕНВНИ", /*(TObject*)*/31);
    slStmntOboznac2->AddObject("ПРОЦЕНТСВНИ", /*(TObject*)*/32);
    slStmntOboznac2->AddObject("ПРОЦЕНТНВНИ", /*(TObject*)*/33);
    slStmntOboznac2->AddObject("ОТКЛОНЕНИЕСВНачКВ", /*(TObject*)*/34);
    slStmntOboznac2->AddObject("ОТКЛОНЕНИЕНВНачКВ", /*(TObject*)*/35);
    slStmntOboznac2->AddObject("КонкатСтрок", /*(TObject*)*/36);
};


// Вставляем вычисляемые значения
void TForm2loadValue::doVicisl(TFormLogicF1* FormLogicF1, TwwStringList* p, PCellValueAtr vd) {
    string Stmnt;
    TFormatStyle* FormatStyle = new TFormatStyle();
    int PeriodType, ValuePeriod;
    string Value;
    double ValueD, ValueD2;
    int Year = 0, Month, Day;
    TDateTime OsobDate, BegDate;
    int wCellid;
    PAtrRow AtrRow;
    int StmntCode;

    if (GlobalVars->slStmntOboznac2->Count() == 0)
        setslStmntOboznac2();

    if (vd->DataType <= (10 + deltaDType)) {
        delete FormatStyle;
        return;
    }

    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, *FormatStyle);
    PeriodType = p->ListStmnt[vd->Col].PeriodType;
    ValuePeriod = p->ListStmnt[vd->Col].ValuePeriod;

    Stmnt = p->ListStmnt[vd->Col].Stmnt;
    StmntCode = getStmntCode2(Stmnt);
    switch (StmntCode) {
        case 1:
        case 2: // if (Stmnt = 'ЗНАЧЕНИЕобщНП') or (Stmnt = 'ЗНАЧЕНИЕобщНПИР') then
        {
            delete FormatStyle;
            return;// обрабатывается выше
        }
        case 3:
        case 4: // if (Stmnt = 'ХарактСВобъекта') or (Stmnt = 'КонкатСтрокСВобъек') then
        {
            delete FormatStyle;
            return; // обрабатывается раньше
        }
    }
    Value = "";
    ValueD = 0;
    ValueD2 = 0;
    FormLogicF1->FormDM->quValue->ParamByName("objectid")->AsInteger() = vd->ObjectId;
    FormLogicF1->FormDM->quValue->ParamByName("CurrCellId")->AsInteger() = vd->CellId;
    FormLogicF1->FormDM->quValue->ParamByName("CurrPlanId")->AsInteger() = vd->PlanId;
    FormLogicF1->FormDM->quValue->ParamByName("CurrVidOtch")->AsInteger() = vd->VidOtch;
    FormLogicF1->FormDM->quValue->ParamByName("CurrValueDate")->AsDateTime() = vd->ValueDate;
    if (vd->DataType == (23 + deltaDType)) { // Агрегат для 'ЗНАЧЕНИЕСВ' и 'ЗНАЧЕНИЕСВНИ' и 'ЗНАЧЕНИЕСВИР'
        if (PeriodType == 7) {// { на дату }
            DecodeDate(IncDay(FormLogicF1->CurrDate, -1), Year, Month, Day);
            /*mak ??? ValueD = TValForPeriodAgregat.Getrcrd0(vd->PlanId,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerId,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->ObjectId,// { показатель основа }
                vd->VidOtch, Year, Month, Day, vd->ist_fin_id, vd->CellId);*/
        }
    }
    else if (vd->DataType == (11 + deltaDType)) {// было так (Stmnt = 'ЗНАЧЕНИЕСВ') or (vd.DataType = (11+deltaDType))
      // фактические данные за месяц - их вычисляем
      // уточненный план за месяц (т.е. план-график финансирования) - их вычисляем
      // все вычисляем по таблице Denegn_potok
        DecodeDate(vd->ValueDate, Year, Month, Day);
        if (vd->ObjectId > 0 && vd->ist_fin_id == 1) {
            ValueD = GlobalVars->ValForMesjc.Getrcrd(vd->PlanId, vd->ObjectId, vd->PokazId,
                vd->VidOtch, Year, Month, 2, vd->CellId + 6,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert) +
                GlobalVars->ValForMesjc.Getrcrd(vd->PlanId, vd->ObjectId, vd->PokazId, vd->VidOtch,
                Year, Month, 3, vd->CellId + 3,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
        }
        else {
            ValueD = GlobalVars->ValForMesjc.Getrcrd(vd->PlanId, vd->ObjectId, vd->PokazId,
                vd->VidOtch, Year, Month, vd->ist_fin_id, vd->CellId,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
        }
    }
    else {
        switch (StmntCode) {
            case 5: // if (Stmnt = 'Экономия/Резерв') then
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 13) {
                    ValueD = TDB_RP_067eval::doEval(vd->PlanId, vd->PokazId, vd->VidOtch,
                        YearOf(vd->ValueDate), p->ListStmnt[vd->Col].natur_vid);
                }
                break;
            case 6: // if (Stmnt = 'отчетCO_RP_015') then
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 13) {
                    GlobalVars->vCO_RP_015eval.lastdate_15fcu = FormLogicF1->CurrDate;
                    ValueD = TCO_RP_015eval::doEval(vd->PlanId, vd->PokazId, vd->VidOtch,
                        YearOf(vd->ValueDate), p->ListStmnt[vd->Col].natur_vid);
                }
                break;
            case 7: // if (Stmnt = 'СведенияОбЭкономии') then
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 13) {
                    ValueD = TEkonomijBPSeval::doEval(vd->PlanId, vd->PokazId, vd->VidOtch,
                        YearOf(vd->ValueDate), p->ListStmnt[vd->Col].natur_vid);
                }
                break;
            case 8: // if (Stmnt = 'ПлнОСТАТОК') then
                if (vd->cell_period != 3) {
                    //mak throw Exception("Плановый остаток указан не для месяца.");
                }
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 4) {
                    ValueD = TForm2Plus::Ostatok(FormLogicF1, vd->PlanId,
                        vd->CellId - 1, vd->CellId, vd->PokazId, 4,
                        vd->ValueDate, vd->ObjectId);
                }
                break;
            case 9: // if (Stmnt = 'ФктОСТАТОК') then
                if (vd->cell_period != 3) {
                    //mak throw Exception("Фактический остаток указан не для месяца.");
                }
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 4) {
                    ValueD = TForm2Plus::Ostatok(FormLogicF1, vd->PlanId,
                        vd->CellId - 1, vd->CellId, vd->PokazId, 5,
                        vd->ValueDate, vd->ObjectId);
                }
                break;
            case 10: // if (Stmnt = 'ПлнОСТАТОКНИ') then
                if (vd->cell_period != 3) {
                    //mak  throw Exception("Плановый остаток с нар.итогом указан не для месяца.");
                }
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 4) {
                    ValueD = TForm2Plus::OstatokNI(FormLogicF1, vd->PlanId,
                        vd->CellId - 1, vd->CellId, vd->PokazId, 4,
                        vd->ValueDate, vd->ObjectId);
                }
                break;
            case 11: // if (Stmnt = 'УТОЧНЕНИЕСВ') then
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 2 ||
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid == 13) {
                    ValueD = TPeremescenij::doEval(vd->PlanId, vd->PokazId, vd->VidOtch,
                        YearOf(vd->ValueDate), p->ListStmnt[vd->Col].natur_vid);
                }
                break;
            case 12: // if (Stmnt = 'УТОЧНЕНИЕСВНИ') then
                ValueD = TForm2Plus::SummaUtoc(FormLogicF1, true, vd->PlanId, vd->ValueDate,
                    vd->CellId);
                break;
            case 13: // if (Stmnt = 'КБК-Дгвр:Сумма№БО') then  // это сумма объемов финснирования с № БО/ДО по КБК
            {
                delete FormatStyle;
                return;  // обрабатывается выше
            }
            case 14: // if (Stmnt = 'КБК-Дгвр:СуммаБО') then  // это сумма объемов финснирования, а не БО по КБК
            {
                delete FormatStyle;
                return;  // обрабатывается выше
            }
            case 15: // if (Stmnt = 'ФактКвДату' { 'УТОЧНЕНИЕСВдф' } ) then
                if (PeriodType == 7) {//{ за период/на дату } 
                    DecodeDate(IncDay(FormLogicF1->CurrDate, -1), Year, Month, Day);
                }
                ValueD = TValForPeriod::valFactKvNaDatu(vd->PlanId, vd->PokazId, Year, Month,
                    Day, FormLogicF1->CurrQuarter);
                break;
            case 16: // if (Stmnt = 'УТОЧНЕНИЕСВПосле') then
                ValueD = TDB_UtocPlana::GetSumUtocPosleKvartala(vd);
                break;
            case 17:
            case 18: // if (Stmnt = 'ФактВсегоВНачале') or (Stmnt = 'ПоступлВсегоВНачале') then
                if (!(vd->cell_period == 2 || vd->cell_period == 3)) {// { кв,месяц }
                    //throw Exception("\"" + Stmnt + "\"" + " указан не для кв. или месяца.");
                }
                if (vd->VidOtch != 5) {
                    //throw Exception("\"" + Stmnt + "\"" + " указан не для фактических данных.");
                }
                DecodeDate(IncDay(vd->ValueDate, -1), Year, Month, Day);
                if (PeriodType == 7) {// { за период/на дату }
                    DecodeDate(IncDay(FormLogicF1->CurrDate, -1), Year, Month, Day);
                }
                if (PeriodType == 6) {//{ за месяц }
                    DecodeDate(vd->ValueDate, Year, Month, Day);
                }
                if (PeriodType == 6) {//{ за месяц }
                    BegDate = EncodeDate(Year, Month, 1);
                }
                else {
                    BegDate = EncodeDate(Year, 1, 1);
                }
                switch (vd->cell_period) {
                    case 2:
                        wCellid = vd->CellId + 1;
                        break;
                    case 3:
                        wCellid = vd->CellId;
                        break;
                }
                if (Stmnt == "ФактВсегоВНачале") {
                    if (p->vid_form == 2 && PeriodType == 6) {
                        ValueD = TValForPeriod::valFactToBegPeriod(vd->PlanId, vd->PokazId, Year,
                                                                   Month, Day);
                        if (Month == 12) {
                            if (Year == (FormLogicF1->CurrYear - 1)) {
                                ValueD = 0;
                            }
                        }
                    }
                    else if (vd->ist_fin_id == 1) {
                        ValueD = GlobalVars->ValForPeriod.Getrcrd(vd->PlanId, vd->ObjectId,
                            vd->PokazId, vd->VidOtch, Year, Month, Day, 2, vd->cell_id2,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert) +
                            GlobalVars->ValForPeriod.Getrcrd(vd->PlanId, vd->ObjectId,
                            vd->PokazId, vd->VidOtch, Year, Month, Day, 3, vd->cell_id3,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert) +
                            GlobalVars->ValForPeriod.Getrcrd(vd->PlanId, vd->ObjectId,
                            vd->PokazId, vd->VidOtch, Year, Month, Day, 4, vd->cell_id4,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
                    }
                    else {
                        ValueD = GlobalVars->ValForPeriod.Getrcrd(vd->PlanId, vd->ObjectId,
                            vd->PokazId, vd->VidOtch, Year, Month, Day, vd->ist_fin_id, wCellid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
                    }
                }
                else {
                    if (vd->ist_fin_id == 1) {//{ всего }
                        ValueD = GlobalVars->PostuplForPeriod.Getrcrd(vd->PlanId, vd->PokazId,
                            vd->VidOtch, BegDate, Year, Month, Day, 2, vd->cell_id2,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert) +
                            GlobalVars->PostuplForPeriod.Getrcrd(vd->PlanId, vd->PokazId,
                            vd->VidOtch, BegDate, Year, Month, Day, 3, vd->cell_id3,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert) +
                            GlobalVars->PostuplForPeriod.Getrcrd(vd->PlanId, vd->PokazId,
                            vd->VidOtch, BegDate, Year, Month, Day, 4, vd->cell_id4,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
                    }
                    else {
                        ValueD = GlobalVars->PostuplForPeriod.Getrcrd(vd->PlanId, vd->PokazId,
                            vd->VidOtch, BegDate, Year, Month, Day, vd->ist_fin_id, wCellid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
                    }
                }
                break;
            case 19: // if (Stmnt = 'УточПланВклДату') then
                if (!(vd->cell_period == 2 || vd->cell_period == 3)) {//{ кв,месяц }
                    //throw Exception("УточПлан к началу указан не для кв. или месяца.");
                }
                if (vd->VidOtch != 4) {
                    //throw Exception("УточПлан к началу указан не для уточненного плана.");
                }
                DecodeDate(IncDay(vd->ValueDate, -1), Year, Month, Day);
                if (PeriodType == 7) {
                    DecodeDate(IncDay(FormLogicF1->CurrDate, -1), Year, Month, Day);
                }
                switch (vd->cell_period) {
                    case 2:
                        wCellid = vd->CellId + 1;
                        break;
                    case 3:
                        wCellid = vd->CellId;
                        break;
                }
                ValueD = GlobalVars->ValForPeriod.Getrcrd(vd->PlanId, vd->ObjectId, vd->PokazId,
                    vd->VidOtch, Year, Month, Day, vd->ist_fin_id, wCellid,
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                    TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
                break;
            case 20: // if (Stmnt = 'СуммаПриход') then
                ValueD = TForm2Plus::GetSumPrixodVozvrat(FormLogicF1, true, vd);
                break;
            case 21: // if (Stmnt = 'СуммаВозврат') then
                ValueD = TForm2Plus::GetSumPrixodVozvrat(FormLogicF1, false, vd);
                break;
            case 22:
            case 23:
            case 24: //if (Stmnt = 'СуммаПлГрНИВНачале') or (Stmnt = 'СуммаПлГрНИГод') or (Stmnt = 'СуммаПлГрНИВМесяце') then
                if (!(vd->cell_period == 2 || vd->cell_period == 3)) {//{ кв,месяц }
                    //throw Exception("ПлГрНИ к началу указан не для кв. или месяца.");
                }
                if (vd->VidOtch != 4) {
                    //throw Exception("ПлГрНИ к началу указан не для уточненных данных.");
                }
                DecodeDate(IncDay(vd->ValueDate, -1), Year, Month, Day);
                if (PeriodType == 7) {//{ за период }
                    DecodeDate(IncDay(FormLogicF1->CurrDate, -1), Year, Month, Day);
                }
                switch (vd->cell_period) {
                    case 2:
                        wCellid = vd->CellId + 1;
                        break;
                    case 3:
                        wCellid = vd->CellId;
                        break;
                }
                if (Stmnt == "СуммаПлГрНИВМесяце") {
                	// семантика сейчас такая 'Сумма планов-гр. уже отправленных в течение месяца' - на текущую дату Но Не На указанную
                    ValueD = TValForMesjc::GetPlGrFinZaMesjc(FormLogicF1, vd->PlanId, vd->ObjectId,
                        vd->PokazId, vd->VidOtch, YearOf(vd->ValueDate), MonthOf(vd->ValueDate),
                        vd->ist_fin_id, vd->CellId);
                }
                if (Stmnt == "СуммаПлГрНИВНачале") {
                	// семантика сейчас такая 'Сумма планов-гр. уже отправленных в течение квартала'
                    ValueD = TValForMesjc::GetPlGrFinZaKvrtl(FormLogicF1, vd->PlanId, vd->ObjectId,
                        vd->PokazId, vd->VidOtch, YearOf(vd->ValueDate), MonthOf(vd->ValueDate),
                        vd->ist_fin_id, vd->CellId);
                }
                if (Stmnt == "СуммаПлГрНИГод") {
                    ValueD = TValForMesjc::GetPlGrFinZaPeriod(FormLogicF1, vd->PlanId, vd->ObjectId,
                        vd->PokazId, vd->VidOtch, YearOf(vd->ValueDate), MonthOf(vd->ValueDate),
                        vd->ist_fin_id, vd->CellId, true);
                }
                break;
            case 25: // if (Stmnt = 'ПДГ_УТ_КассПл') then
                ValueD = TDB_UtocKP::GetUtKassPl_pdg(vd);
                break;
            case 26: // if (Stmnt = 'УТВ_УТ_КассПл') then
                ValueD = TDB_UtocKP::GetUtKassPl_utv(FormLogicF1, vd);
                break;
            case 27:
            case 28: // if (Stmnt = 'ЗНАЧЕНИЕСВИР') or (Stmnt = 'ЗНАЧЕНИЕНВИР') then
                if (PeriodType == 7) {//{ на дату }
                    switch (vd->cell_period) {
                        case 2:
                            wCellid = vd->CellId + 1;
                            break;
                        case 3:
                            wCellid = vd->CellId;
                            break;
                    }
                    DecodeDate(IncDay(FormLogicF1->CurrDate, -1), Year, Month, Day);
                    ValueD = GlobalVars->SummaBySubjects.Getrcrd(vd->ObjectId, vd->PokazId, vd->VidOtch,
                        Year, Month, Day, vd->ist_fin_id, wCellid,
                        TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid,
                        TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert);
                }
                else {
                    ValueD = GlobalVars->SummaBySubjects.Val_SumToGodKv(vd->CellId, vd->VidOtch, vd->ValueDate);
                }
                break;
            case 29: // if (Stmnt = 'ПризнакУчастияВСвязи') then
                ValueD = 0; // 04.06.2021 отказ от Stmnt 'ПризнакУчастияВСвязи'
                break;
            case 30:
            case 31: // if (Stmnt = 'ЗНАЧЕНИЕСВНИ') or (Stmnt = 'ЗНАЧЕНИЕНВНИ') then
                p->ComplexForm = true;
                /*mak FormLogicF1->FormDM->quValuePeriod->ParamByName("objectid")->AsInteger() =
                    FormLogicF1->FormDM->quValue->ParamByName("objectid")->AsInteger();
                FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrCellId")->AsInteger() =
                    FormLogicF1->FormDM->quValue->ParamByName("CurrCellId")->AsInteger();
                FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrPlanId")->AsInteger() =
                    FormLogicF1->FormDM->quValue->ParamByName("CurrPlanId")->AsInteger();
                DecodeDate(vd->ValueDate, Year, Month, Day);*/
                FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrYear")->AsInteger() = Year;
                switch (vd->pokaz_type) {
                    case 1: // для числа
                        // Расчет происходит по Cell_Value, поэтому результат с точностью до квартала
                        /*mak FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrVidOtch")->AsInteger() =
                            FormLogicF1->FormDM->quValue->ParamByName("CurrVidOtch")->AsInteger();*/
                        FormLogicF1->FormDM->quValuePeriod->Open();
                        while (!FormLogicF1->FormDM->quValuePeriod->EoF()) {
                            if (FormLogicF1->FormDM->quValuePeriod->FieldByName("value_date")->AsDateTime() > vd->ValueDate)
                                break;
                            ValueD += FormLogicF1->FormDM->quValuePeriod->FieldByName("value")->AsFloat();
                            FormLogicF1->FormDM->quValuePeriod->Next();
                        }
                        FormLogicF1->FormDM->quValuePeriod->Close();
                        break;
                    case 2: // для строки
                        Value = TDB_CellValue::GetNI_byStr(vd->ValueDate,
                            FormLogicF1->FormDM->quValue->ParamByName("objectid")->AsInteger(),
                            FormLogicF1->FormDM->quValue->ParamByName("CurrCellId")->AsInteger(),
                            FormLogicF1->FormDM->quValue->ParamByName("CurrPlanId")->AsInteger(),
                            FormLogicF1->FormDM->quValue->ParamByName("CurrVidOtch")->AsInteger(), Year);
                        break;
                }
                break;
            case 32:
            case 33:
            case 34:
            case 35: // if (Stmnt = 'ПРОЦЕНТСВНИ') or (Stmnt = 'ПРОЦЕНТНВНИ') or (Stmnt = 'ОТКЛОНЕНИЕСВНачКВ') or (Stmnt = 'ОТКЛОНЕНИЕНВНачКВ') then
                p->ComplexForm = true;
                OsobDate = vd->ValueDate;
                if (Stmnt == "ОТКЛОНЕНИЕСВНачКВ" || Stmnt == "ОТКЛОНЕНИЕНВНачКВ") {
                    DecodeDate(IncMonth(OsobDate, -2), Year, Month, Day);
                    OsobDate = EncodeDate(Year, Month, 1);
                }
                FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrVidOtch")->AsInteger() = 5;
                // Факт
                FormLogicF1->FormDM->quValuePeriod->Open();
                while (!FormLogicF1->FormDM->quValuePeriod->EoF()) {
                    if (FormLogicF1->FormDM->quValuePeriod->FieldByName("value_date")->AsDateTime() > OsobDate)
                        break;
                    ValueD += FormLogicF1->FormDM->quValuePeriod->FieldByName("value")->AsFloat();
                    FormLogicF1->FormDM->quValuePeriod->Next();
                }
                FormLogicF1->FormDM->quValuePeriod->Close();
                FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrVidOtch")->AsInteger() = 4;
                // Ут. план
                FormLogicF1->FormDM->quValuePeriod->Open();
                while (!FormLogicF1->FormDM->quValuePeriod->EoF()) {
                    if (FormLogicF1->FormDM->quValuePeriod->FieldByName("value_date")->AsDateTime() > OsobDate)
                        break;
                    ValueD2 += FormLogicF1->FormDM->quValuePeriod->FieldByName("value")->AsFloat();
                    FormLogicF1->FormDM->quValuePeriod->Next();
                }
                FormLogicF1->FormDM->quValuePeriod->Close();
                if (Stmnt == "ПРОЦЕНТСВНИ" || Stmnt == "ПРОЦЕНТНВНИ") {
                    if (ValueD2 != 0) {
                        ValueD = ValueD / ValueD2 * 100;
                        ValueD = round(ValueD * 100) / 100;
                    }
                    else {
                        ValueD = 0;
                    }
                }
                else {
                    ValueD = ValueD - ValueD2;
                }
                break;
            case 36: // if (Stmnt = 'КонкатСтрок') then
                ValueD = 0;
                Value = TForm2Plus::evalKonkatStr(FormLogicF1, vd, p);
                break;
        }
    }
    if (ValueD != 0) {
        Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
            Value, vd->DataType, FormatStyle);
    }
    delete FormatStyle;
}


void TForm2loadValue::doSCHistory(TFormLogicF1* FormLogicF1, TwwStringList* p, int i, PCellValueAtr vd) {
    scHistory.AddCell(i, vd);
    if (scHistory.CurrNumb >= 1000) {
        scHistory.Execute();
        scHistory.sp->Open();
        scHistory.GetItems(FormLogicF1, p);
        scHistory.sp->Close();
        //MAK scHistory.NBlock = i;
        scHistory.NBlock = 0;
        scHistory.BeginAdd();
    }
}









