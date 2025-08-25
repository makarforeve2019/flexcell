#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "unDBType.h"
#include "FormDopInp.h"
#include "Form1.h"
#include "DB_Plan.h"
#include "DB_Pokaztl.h"
#include "DB_PlanStr.h"
#include "Form2find.h"
#include "unIDsClassFieldCell.h"
#include "DB_EdIzm.h"
#include "Form2ExecPokaz1clmn.h"

// ВАЖНО!!!!!!!!!!!!!
// AtrStmnt:PAtrStmnt и AtrRow:PAtrRow передаются параметром
// внутри метода их НЕ ВЫЧИСЛЯТЬ

// Для идентификации Range пользоваться только параметрами nRow: integer; nClmn: integer;

void TForm2ExecPokaz1clmn::ExecPokaz1clmn(int nRow, int nClmn, PAtrStmnt AtrStmnt, PAtrRow AtrRow, TQueryUni* aQOwnerPlanStr, int aPlan_id, PDopAtrCol aDopAtrCol, std::string aStmntRow)
{
    std::string Stmnt; // текущий обрабатываемый в ExecPokaz показатель
    int stmntClass;
    int I, Index;
    TQueryUni* qDefPokaz;
    int pokaz_is_razd;
    bool isDoopred;
    TDateTime PokazDate;
    std::string pokaz_bk_codeKorteg, ws;
    int pokaz_idKorteg;
    int otkuda;
    TFormatStyle* FormatStyle = new TFormatStyle();
    int plan_id_2podvedomstv;
    int wpokaz_id, wwpokaz_id, wplan_id, newCurrPlanId, wn, wm;
    PCellValueAtr vd;
    PCellValueAtr wvd;
    PCellValueAtr wValueData;
    bool etlnLocked, etlnHidden;
    
    Stmnt = AtrStmnt->Stmnt;
    isDoopred = (Stmnt == "ЗНАЧЕНИЕСВ_ДООПРЕД");
    qDefPokaz = aQOwnerPlanStr;
    pokaz_is_razd = aQOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
    switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger())
    {
        case 1:
        {
            if (isDoopred)
            {
                ws = aQOwnerPlanStr->FieldByName("pokaz_name")->AsString();
                wpokaz_id = AtrStmnt->pokaz_id;
                wplan_id = AtrStmnt->plan_id;
                wwpokaz_id = TDB_Pokaztl::GetIdbyNameYear(wpokaz_id, ws, FormLogicF1->CurrYear);
                if (wwpokaz_id == 0)
                {
                    //throw Exception("Не удалось доопределить столбец по наименованию " + ws);
                }
                CurrPokazId = wwpokaz_id;
                if (qDefPokazList.Find(IntToStr(CurrPokazId), Index))
                {
                    qDefPokaz = std::get<TQueryUni *>(qDefPokazList.Objects[Index]);
                    qDefPokaz->Close();
                    qDefPokaz->Open();
                }
                else
                {
                    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
                    qDefPokazList.AddObject(IntToStr(CurrPokazId), reinterpret_cast<PCellValueAtr>(qDefPokaz));
                    qDefPokaz->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
                    qDefPokaz->SQL->Add("WHERE plan_id IN (1, " + IntToStr(wplan_id) + ")");
                    qDefPokaz->SQL->Add("AND b.pokaz_id = " + IntToStr(CurrPokazId));
                    qDefPokaz->SQL->Add("AND a.pokaz_id = b.pokaz_id");
                    switch (GetDBType())
                    {
                        case TDBType::MSSQL:
                        {
                            qDefPokaz->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                            break;
                        }
                        case TDBType::Postgre:
                        {
                            qDefPokaz->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) + " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                            break;
                        }
                    }
                    qDefPokaz->Open();
                }
            }
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        case 4:
        case 5:
        {
            if (pokaz_is_razd == 2 || pokaz_is_razd == 3)  //это показатель, а не объект
            {
                if (FormLogicF1->quForms->FieldByName("work_with")->AsInteger() == 5)
                {
                    if (AtrStmnt->field_id == -1)
                    {
                        CurrPokazId = qDefPokaz->FieldByName("pokaz_id")->AsInteger();
                    }
                    else if (AtrStmnt->field_id >= 1 && AtrStmnt->field_id <= 10) // системная характеристика объекта
                    {
                        CurrPokazId = qDefPokaz->FieldByName("pokaz_id")->AsInteger();
                    }
                    else if (AtrStmnt->field_id > 0)
                    {
                    	// если пользователь явно указал совмещение описания столбца для мероприят и объекта:
            			//  это attr_4 = 1, сохранен в AtrStmnt
            			// нужно просто идти дальше, а не переходить на LNext
                        if (AtrStmnt->attr_4 == 0)
                        {
                            goto LNext;
                        }
                    }
                }
                goto LLEnd;
            }
            CurrPokazId = AtrStmnt->field_id;
            newCurrPlanId = FormLogicF1->quForms->FieldByName("plan_id")->AsInteger();
            if (FormLogicF1->quForms->FieldByName("work_with")->AsInteger() == 4)
            {
            	// AtrStmnt.note2 имеет вид id-класса точка id-характеристики
        		//   поскольку в случае иерархии выводятся вразные строки данные разных классов
                if (Stmnt == "ЗНАЧЕНИЕСВ")
                {
                    CurrPokazId = getFieldId(aQOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger(), AtrStmnt->note2);
                }
            }
            qDefPokaz = FormLogicF1->FormDM->quFormColmn;
            if (Stmnt == "ХарактСВобъекта" || Stmnt == "КонкатСтрокС��объек")
            {
            	// В случае Иерархии: AtrStmnt.pokaz_id содержит id класса, AtrStmnt.field_id - id харктеристики класса
                CurrPokazId = AtrStmnt->field_id;
                class_idXarakterSvjzObjecta = AtrStmnt->pokaz_id;
                if (qSvjzClass->Active())
                {
                    if (qSvjzClass->FieldByName("plan_str_owner_id")->AsInteger() != class_idXarakterSvjzObjecta)
                    {
                        qSvjzClass->Active( false );
                    }
                }
                if (!qSvjzClass->Active())
                {
                    TDB_PlanStr::GetAllSubClassItem(qSvjzClass, class_idXarakterSvjzObjecta, Date());
                }
                qDefPokaz = qSvjzClass;
            }
            if (Stmnt == "КонкатСтрок" || Stmnt == "КонкатСтрокСВобъек")
            {
                TFormDopInp::GetNextFormulaConc(AtrStmnt->note1, CurrPokazId, ws, 1);
                // здесь из формулы выделяем только первый field_id, чтобы вычислить его cell_id,
        		// при вычислении формулы довычислим остальные cell_id
            }
            if (CurrPokazId > 0)
            {
                qDefPokaz->Locate("pokaz_id", CurrPokazId);
            }
            LLEnd:; //переход сюда, если это показатель
            break;
        }
    }
    // обработка итого по разделу в списке объектов
  	// !это к МультиИтого не относится (его мы обрабатываем как объект с теми же полями)
    if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); pokaz_is_razd == 3 && (N == 3 || N == 5))
    {
        qDefPokaz = aQOwnerPlanStr;
        CurrPokazId = qDefPokaz->FieldByName("pokaz_id")->AsInteger();
        CurrObject.pokaz_id = 0;
    }
    PlanId = AtrStmnt->plan_id;
    if (PlanId == 0)
    {
        PlanId = FormLogicF1->CurrPlanId;
    }
    if (aPlan_id > 0)
    {
        PlanId = aPlan_id;
    }
    if (FormLogicF1->quForms->FieldByName("work_with")->AsInteger() == 4 && PlanId == 0 && newCurrPlanId > 0)
    {
        PlanId = newCurrPlanId;
    }
    if (PlanId == 1)
    {
        PlanId = TDB_Plan::GetPodVedomstvId(FormLogicF1->CurrPlanId, false, plan_id_2podvedomstv);
    }
    if ((PlanId == 0 || PlanId == 1) && aDopAtrCol->is_planId)
    {
        PlanId = aDopAtrCol->plan_id;
    }
    VidOtch = AtrStmnt->vid_otch;
    if (VidOtch == 0)
    {
        VidOtch = FormLogicF1->CurrVidOtch;
    }
    PokazId = CurrPokazId;
    PokazType = qDefPokaz->FieldByName("pokaz_type")->AsInteger();
    if (PokazType == 4)
    {
        EnumId = qDefPokaz->FieldByName("enum_id")->AsInteger();
    }
    if (PokazType == 11) //измерение
    {
        EnumId = qDefPokaz->FieldByName("enum_id")->AsInteger(); // id измерения
    }
    if (pokaz_is_razd == 1)
    {
        if (Stmnt == "КОДБК" || Stmnt == "КОДБК1" || Stmnt == "Примечание" || Stmnt == "Владелец" ||
            Stmnt == "КОДБКРздл" || Stmnt == "КОДБКРздл1" || Stmnt == "КОДБКЦелСт" || Stmnt == "КОДБКЦелСт1" ||
            Stmnt == "КОДБКВидРасх" || Stmnt == "КОДБКЭкКл" || Stmnt == "НАИМПОКАЗ" || Stmnt == "ПолныйНомер" ||
            Stmnt == "forUnikIdentif" || Stmnt == "ДатаСоздОбъекта" || Stmnt == "СистХарОбъекта" || Stmnt == "СистХарПоказтля")
        {
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 2, VidOtch, PlanId,
                0, 0, CurrObject.pokaz_id, PokazId,
                0, 0, 0, 0);
        }
    }
    if (pokaz_is_razd > 1)
    {
        stmntClass = TForm2find::findByStmnt(Stmnt);
        if (stmntClass == 1)
        {
            TFormDopInp::setCellSizeFixed(FormLogicF1, nClmn);
            // Stmnt = 'ПРОИЗВСТОЛБ'
        }
        else if (stmntClass == 2)
        {
            //(Stmnt = 'ааЗначВырМнрг') or (Stmnt = 'ааДельтаЭтлн') or (Stmnt = 'ааИдентифТенд')
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 0, VidOtch, PlanId, 0,
                0, CurrObject.pokaz_id, PokazId,
                0, 0, 0, 0);
        }
        else if (stmntClass == 3)
        {
            //(Stmnt = 'ааПрирост') or (Stmnt = 'ааПроцПриростБаза')
            switch (AtrStmnt->is_set_god)
            {
                case 0: // {наследуется}
                {
                    PokazDate = EncodeDate(FormLogicF1->CurrYear + AtrStmnt->god_plus, 12, 31);
                    break;
                }
                case 1: // {задается}
                {
                    PokazDate = EncodeDate(AtrStmnt->god_ravno, 12, 31);
                    break;
                }
            }
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, PokazDate, 0, VidOtch, PlanId, 0,
                0, CurrObject.pokaz_id, PokazId,
                0, 0, 0, 0);
        }
        else if (stmntClass == 4)
        {
        	// (Stmnt = 'РАСЧСЧЕТ') or (Stmnt = 'ИНН') or (Stmnt = 'ЮрАдрес')
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 2, VidOtch, PlanId, 0,
                0, CurrObject.pokaz_id, PokazId,
                1, 0, 0, 0);
        }
        else if (stmntClass == 5)
        {
            // (Stmnt = 'БДЖПОЛУЧТЛЬ') or (Stmnt = 'БДЖПОЛУЧТЛЬ_нет()')
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 2, VidOtch, PlanId, 0,
                0, CurrObject.pokaz_id, PokazId,
                1, 0, 0, 0);
        }
        else if (stmntClass == 6)
        {
            // (Stmnt = 'РАСЧСЧЕТ2') or (Stmnt = 'ЛицевойСчетТБ')
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 0, VidOtch, PlanId, 0,
                0, CurrObject.pokaz_id, PokazId,
                1, 0, 0, 0);
        }
        else if (stmntClass == 7)
        {
          //(Stmnt = 'НАИМПОКАЗ') or ( = 'НАИМЦелСт') or (Stmnt = 'КОДБК') or
	      // (Stmnt = 'КОДБК1') or (Stmnt = 'КОДБКРздл') or (Stmnt = 'КОДБКРздл1') or (Stmnt = 'КОДБКЦелСт') or
    	  // (Stmnt = 'КОДБКЦелСт1') or (Stmnt = 'КОДБКВидРасх') or (Stmnt = 'КОДБКЭкКл') or
      	  //(Stmnt = 'НомерМрпр') or (Stmnt = 'КБК-Дгвр:Кол-во') or (Stmnt = 'КБК-Дгвр:Кол-воБО') or
      	  // (Stmnt = 'Владелец') or (Stmnt = 'forUnikIdentif') or (Stmnt = 'ПолныйНомер') or
          // (Stmnt = 'ИмяИстФин') or (Stmnt = 'ДатаСоздОбъекта') 'ОткрытьФормуПодчинОб' 'ОткрытьФормуТекущОб'
          // 'TreeImgShow/Hide1' 'TreeImgShow/HideNext'
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 0, VidOtch, PlanId,
                0, 0, CurrObject.pokaz_id, PokazId,
                1, 0, 0, 0);
            if (Stmnt == "КОДБК" || Stmnt == "КОДБК1" || Stmnt == "КОДБКРздл" || Stmnt == "КОДБКРздл1" || Stmnt == "КОДБКЦелСт" ||
                Stmnt == "КОДБКЦелСт1" || Stmnt == "КОДБКВидРасх" || Stmnt == "КОДБКЭкКл" ||
                Stmnt == "НомерМрпр" || Stmnt == "КБК-Дгвр:Кол-во" || Stmnt == "КБК-Дгвр:Кол-воБО" ||
                Stmnt == "forUnikIdentif" || Stmnt == "ПолныйНомер" || Stmnt == "ДатаСоздОбъекта")
            {
                TFormDopInp::setCellSizeFixed(FormLogicF1, nClmn);
            }
        }
        else if (stmntClass == 14)
        {
            //(stmnt = 'Примечание') or (stmnt = 'Ведомство') or
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 0, VidOtch, PlanId,
                0, 0, CurrObject.pokaz_id, PokazId,
                0, 0, 0, 0);
        }
        else if (stmntClass == 16)
        {
            //(stmnt = 'СистХарОбъекта') или (stmnt = 'СистХарПоказтля')
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            switch (AtrStmnt->field_id) //{это усл ид системной хар}
            {
                case 1:// 'Наименование'
                {
                    wn = 2;
                    wm = 0;
                    break;
                }
                case 2:// 'Код'
                {
                    wn = 2;
                    wm = 0;
                    break;
                }
                case 3:// 'Id класса'
                {
                    wn = 1;
                    wm = 1;
                    break;
                }
                case 4:// 'Вид показателя/объекта'
                {
                    wn = 1;
                    wm = 1;
                    break;
                }
                case 5:// 'Вид строки
                {
                    wn = 1;
                    wm = 1;
                    break;
                }
                case 6:// 'Вид строки
                {
                    wn = 1;
                    wm = 1;
                    break;
                }
                case 7:// Номер строки владельца
                {
                    wn = 1;
                    wm = 1;
                    break;
                }
            }
            vd = TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, wn, VidOtch, PlanId,
                0, 0, CurrObject.pokaz_id, PokazId,
                wm, wn, 0, 0);
            vd->DPId = AtrStmnt->field_id;
        }
        else if (stmntClass == 17)
        {
        	//(stmnt = 'ВводЗначения')
            vd = TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, 0, 1, 4, 1,
                0, 0, 0, 0,
                0, 1, 0, 0);
            FormLogicF1->f1Forms->Protection_Set(false, false); // чтобы можно было вводить значение в ячейку
            TFormDopInp::getColFrmtrAtr(AtrStmnt, 1, *FormatStyle);
            FormatStyle->NumberFormat = "# ### ### ##0.00;-# ### ### ##0.00"; // 26.08.2020
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nClmn,
                nRow, nClmn, "", 0, FormatStyle); // 26.08.2020
        }
        else if (stmntClass == 18)
        {
        	//(stmnt = 'ОперПрисвоить')
        }
        else if (stmntClass == 8)
        {
            // Stmnt = "ЕДИЗМ"
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                aQOwnerPlanStr->FieldByName("edizm_id")->AsInteger(), // CellId
                0, // ValueDate
                0, // DataType
                VidOtch,
                PlanId,
                0, // coef_Pokaz
                0, // coef_Forma
                CurrObject.pokaz_id, // ObjectId
                PokazId,
                1, // cell_is_eval
                0,
                0, // cell_period
                0 // ist_fin_id
            );
        }
        else if (stmntClass == 9)
        {
            // Stmnt = 'ЕДИЗМНВ'
            if (!GetNaturVid())
            {
                goto LNext;
            }
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                FormLogicF1->FormDM->quPokaztl->FieldByName("edizm_id")->AsInteger(), // CellId
                0, // ValueDate
                0, // DataType
                VidOtch,
                PlanId,
                0, // coef_Pokaz
                0, // coef_Forma
                CurrObject.pokaz_id, // ObjectId
                PokazId,
                1, // cell_is_eval
                0,
                0, // cell_period
                0 // ist_fin_id
            );
        }
        else if (stmntClass == 10)
        {
            // Stmnt = "ЕДИЗМобщНП"
            doStmnt(nClmn, nRow, aQOwnerPlanStr, Stmnt, nClmn);
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                TIDsClassFieldCell::getCNPedizm(AtrStmnt->natur_vid), // CellId
                0, // ValueDate
                0, // DataType
                VidOtch,
                PlanId,
                0, // coef_Pokaz
                0, // coef_Forma
                CurrObject.pokaz_id, // ObjectId
                PokazId,
                1, // cell_is_eval
                0,
                0, // cell_period
                0 // ist_fin_id
            );
        }
        else if (stmntClass == 11)
        {
            // (Stmnt = 'ФОРМУЛАF1')
            TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn,
                0, // cell_id
                0, // ValueDate
                0, // DataType
                VidOtch,
                PlanId,
                0, // coef_Pokaz
                0, // coef_Forma
                CurrObject.pokaz_id, // ObjectId
                PokazId,
                1, // cell_is_eval
                0,
                0, // cell_period
                0 // ist_fin_id
            );
            // вставляем формулу в F1
            ws = AtrStmnt->note1;
            if (!ws.empty() && ws[0] == '[')
            {
                ws = TFormDopInp::GetSubFormulaByNpar(ws, nRow);
            }
            Value = TFormDopInp::ReplaceNRowNCol(nRow, nClmn, ws);
            TFormDopInp::getColFrmtrAtr(AtrStmnt, 1, *FormatStyle);
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nClmn,
                nRow, nClmn, Value, 0, FormatStyle);
        }
        else if (stmntClass == 12)
        {
            //(Stmnt = 'ЗНАЧЕНИЕСВ') or (Stmnt = 'ЗНАЧЕНИЕСВ_ДООПРЕД') or (Stmnt = 'ЗНАЧЕНИЕНВ') or
            //(Stmnt = 'ХарактСВобъекта') or (Stmnt = 'КонкатСтрокСВобъек') or
       		//(Stmnt = 'ПРОЦЕНТСВ') or (Stmnt = 'ПРОЦЕНТНВ') or
       		//(Stmnt = 'ОТКЛОНЕНИЕСВ') or (Stmnt = 'ОТКЛОНЕНИЕНВ') or
       		//(Stmnt = 'ЗНАЧЕНИЕСВНИ') or (Stmnt = 'ЗНАЧЕНИЕНВНИ') or
       		//(Stmnt = 'ПРОЦЕНТСВНИ') or (Stmnt = 'ПРОЦЕНТНВНИ') or
       		//(Stmnt = 'КБК-Дгвр:Сумма№БО'{'ОТКЛОНЕНИЕСВНИ'}) or (Stmnt = 'КБК-Дгвр:СуммаБО'{'ОТКЛОНЕНИЕНВНИ'}) or
       		//(Stmnt = 'ОТКЛОНЕНИЕСВНачКВ') or (Stmnt = 'ОТКЛОНЕНИЕНВНачКВ') or
       		//(Stmnt = 'ЗНАЧЕНИЕСВИР') or (Stmnt = 'ЗНАЧЕНИЕНВИР') or
       		//(Stmnt = 'УТОЧНЕНИЕСВ') or (Stmnt = 'УТОЧНЕНИЕСВНИ') or (Stmnt = 'УТОЧНЕНИЕСВПосле') or
       		//(Stmnt = 'Мрпр-Дгвр:Сумма№БО'{'УТОЧНЕНИЕНВ'}) or (Stmnt = 'Мрпр-Дгвр:СуммаБО'{'УТОЧНЕНИЕНВНИ'}) or
       		//(Stmnt = 'ФактКвДату'{'УТОЧНЕНИЕСВдф'}) or (Stmnt = 'СуммаПлГрНИВМесяце'{'УТОЧНЕНИЕСВНИдф'}) or
       		//(Stmnt = 'ПлнОСТАТОК') or (Stmnt = 'ФктОСТАТОК') or (Stmnt = 'ПлнОСТАТОКНИ') or (Stmnt = 'ФактВсегоВНачале') or (Stmnt = 'УточПланВклДату') or
       		//(Stmnt = 'СуммаПриход') or (Stmnt = 'СуммаВозврат') or (Stmnt = 'СуммаПлГрНИВНачале') or (Stmnt = 'СуммаПлГрНИГод') or
       		//(Stmnt = 'ПДГ_УТ_КассПл') or (Stmnt = 'УТВ_УТ_КассПл') or (Stmnt = 'Экономия/Резерв') or
       		//(Stmnt = 'отчетCO_RP_015') or (Stmnt = 'СведенияОбЭкономии') or
       		//(Stmnt = 'КонкатСтрок'{специально пускаем в обработку, чтобы найти первый cell_id})
            
            coef_Pokaz = TDB_EdIzm::GetCoef_PokazOpt(aQOwnerPlanStr->FieldByName("edizm_id")->AsInteger()); // выбор Coef_Pokaz
            // здесь нельзя использовать stmntClass, т.к. перечисленные ниже входят в 12 класс
            if (Stmnt == "ПРОЦЕНТНВ" ||
                Stmnt == "ЗНАЧЕНИЕНВНИ" || Stmnt == "ПРОЦЕНТНВНИ" ||
                Stmnt == "ОТКЛОНЕНИЕНВНачКВ" || Stmnt == "ЗНАЧЕНИЕНВИР")
            {
                if (!GetNaturVid())
                {
                    goto LNext;
                }
            }
            if (Stmnt == "ЗНАЧЕНИЕСВ" && work_with == 3)
            {
                if (pokaz_is_razd == 3) // {это простое итого}
                {
                    ; // это чтобы идти дальше а не дублировать предыдущее для нового объекта 04.04.2022
                }
            }
            if (Stmnt == "ЗНАЧЕНИЕСВ" && AtrRow->CurRowVid == 4)
            {
                wn = findRowByClassId(AtrRow->objekt_class_id, nRow);
                if (wn != 0)
                {
                    wvd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, wn, nClmn);
                    if (wvd != NULL)
                    {
                        wValueData = new TCellValueAtr;
                        TFormDopInp::CopyCellValueAtr(*wValueData, wvd);
                        wValueData->Row = nRow;
                        wValueData->ObjectId = CurrObject.pokaz_id;
                        wValueData->PlanId = PlanId;
                        p->AddObject("R" + IntToStr(wValueData->Row) + "C" + IntToStr(wValueData->Col), (TObject*)wValueData);
                        FormLogicF1->f1Forms->Selection_Set(wvd->Row, wvd->Col, wvd->Row, wvd->Col);
                        FormLogicF1->f1Forms->Protection_Get(etlnLocked, etlnHidden);
                        FormLogicF1->f1Forms->Selection_Set(wValueData->Row, wValueData->Col, wValueData->Row, wValueData->Col);
                        FormLogicF1->f1Forms->Protection_Set(etlnLocked, etlnHidden);
                        // установка ReadONly для характеристики агрегатора
                        if (qDefPokaz->FieldByName("pokaz_vsego_id")->AsInteger() == 4) //{признак агрегатора}
                        {
                            if (AtrRow->CurRowVid == 4 && AtrRow->pokaz_vert == 1) //{в том числе}
                            {
                                FormLogicF1->f1Forms->Protection_Set(true, true);
                            }
                        }
                        // End of -> установка ReadONly для характеристики агрегатора
                        return;
                    }
                }
            }
            coef_Forma = AtrStmnt->CoefPlus1 - 1;
            if (coef_Forma < 0)
            {
                coef_Forma = coef_Pokaz; //// вариант Как у показателя
            }
            else
            {
                coef_Forma = coef_Forma * 3;
            }
            PeriodType = AtrStmnt->PeriodType;
            period = PeriodType;
            ValuePeriod = AtrStmnt->ValuePeriod;
            if (aDopAtrCol->is_UstPeriod)
            {
                PeriodType = aDopAtrCol->Period;
                period = PeriodType;
                ValuePeriod = aDopAtrCol->ValuePeriod;
            }
            FormLogicF1->quCell->Params0 = PokazId;
            FormLogicF1->quCell->Params3 = AtrStmnt->din_izm;
            PeriodT = 0;
            Cnt = 1;
            switch (PeriodType)
            {
                case 1:
                case 2:
                case 3:
                {
                    FormLogicF1->quCell->Params1 = 1;
                    Kv = 0;
                    Mes = 0;
                    switch (PeriodType)
                    {
                        case 2:
                        {
                            PeriodT = 2;
                            break;
                        }
                        case 3:
                        {
                            PeriodT = 3;
                            break;
                        }
                    }
                    break;
                }
                case 4:
                case 5:
                {
                    FormLogicF1->quCell->Params1 = 2;
                    Kv = FormLogicF1->CurrQuarter;
                    Mes = 0;
                    if (PeriodType == 5)
                    {
                        PeriodT = 4;
                    }
                    break;
                }
                case 6:
                case 7:
                {
                    FormLogicF1->quCell->Params1 = 3;
                    Kv = 0;
                    Mes = FormLogicF1->CurrMonth;
                    if (PeriodType == 7)
                    {
                        Mes = 1; // это условно
                    }
                    break;
                }
                default:
                {
                    FormLogicF1->quCell->Params1 = 0;
                    Kv = 0;
                    Mes = 0;
                    break;
                }
            }
            switch (PeriodT)
            {
                case 2:
                {
                    Cnt = 5;
                    break;
                }
                case 3:
                {
                    Cnt = 13;
                    break;
                }
                case 4:
                {
                    Cnt = 4;
                    PeriodT = 3;
                    break;
                }
            }
            if (PeriodT == 0)
            {
                PeriodType = FormLogicF1->quCell->Params1;
            }
            else
            {
                PeriodType = PeriodT;
            }
            PT = 0;
            if (qDefPokaz->FieldByName("pokaz_is_god")->AsInteger() == 1)
            {
                PT = 1;
            }
            if (qDefPokaz->FieldByName("pokaz_is_kv")->AsInteger() == 1)
            {
                PT = 2;
            }
            if (qDefPokaz->FieldByName("pokaz_is_mes")->AsInteger() == 1)
            {
                PT = 3;
            }
            IsIstFin = AtrStmnt->is_ist_fin;
            if (IsIstFin == 1)
            {
                IstFinGradId = AtrStmnt->ist_fin_grad_id;
                if (qDefPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger() == 0)
                {
                    IstFinGradId = 0;
                    IsIstFin = 0;
                }
            }
            else
            {
                IstFinGradId = 0;
            }
            if (aDopAtrCol->is_UstIstFin)
            {
                if (qDefPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger() == 0)
                {
                    IstFinGradId = 0;
                    IsIstFin = 0;
                }
                else
                {
                    IsIstFin = 1;
                    IstFinGradId = aDopAtrCol->ist_fin_grad_id;
                    AtrRow->ist_fin_grad_id = IstFinGradId;
                }
            }
            IsGroup = AtrStmnt->IsGroup;
            CurrObject.isSimvR = false; // 04.12.2020
            for (I = 1; I <= Cnt; I++)
            {
                if (IsIstFin == 1 && IstFinGradId <= 0)
                {
                    if (!FormLogicF1->FormDM->quIstFin->Active())
                    {
                        FormLogicF1->FormDM->quIstFin->Open();
                    }
                    FormLogicF1->FormDM->quIstFin->First();
                    if (!FormLogicF1->FormDM->quIstFin->EoF())
                    {
                        while (!FormLogicF1->FormDM->quIstFin->EoF())
                        {
                            IstFinGradId = FormLogicF1->FormDM->quIstFin->FieldByName("ist_fin_id")->AsInteger();
                            InputWithDinIzm(nRow, nClmn, AtrStmnt, AtrRow, Stmnt, qDefPokaz, aDopAtrCol);
                            FormLogicF1->FormDM->quIstFin->Next();
                        }
                    }
                }
                else
                {
                    InputWithDinIzm(nRow, nClmn, AtrStmnt, AtrRow, Stmnt, qDefPokaz, aDopAtrCol);
                }
                switch (PeriodT)
                {
                    case 2:
                    {
                        Kv = I;
                        Mes = 0;
                        break;
                    }
                    case 3:
                    {
                        Kv = 0;
                        Mes = I;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                if (I == 1)
                {
                    FormLogicF1->quCell->Params1 = PeriodT;
                }
            }
        }
        else if (Stmnt == "ЗНАЧЕНИЕобщНП" || Stmnt == "ЗНАЧЕНИЕобщНПИР" ||
                 Stmnt == "СВОДобщНПпоСУБЪвАльб" || Stmnt == "СВОДобщНПпоСУБЪвА/n")
        {
            if (!(pokaz_is_razd == 2 || pokaz_is_razd == 3 || pokaz_is_razd == 4)) // 26.05.2023 - итого по разделу может иметь общ натур показ
            {
                goto LNext;
            }
            coef_Forma = AtrStmnt->CoefPlus1 - 1;
            if (coef_Forma < 0)
            {
                coef_Forma = coef_Pokaz;
            }
            else
            {
                coef_Forma = coef_Forma * 3;
            }
            SetValueDataForComNP(nClmn, Stmnt, aDopAtrCol);
        }
        else if (Stmnt == "ПризнакУчастияВСвязи")
        {
            SetValueDataForPriznak(nClmn, aDopAtrCol);
        }
        if (isDoopred)
        {
            CurrPokazId = aQOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
        }
    }
    LNext: ;
}

TForm2ExecPokaz1clmn::TForm2ExecPokaz1clmn() {

}
