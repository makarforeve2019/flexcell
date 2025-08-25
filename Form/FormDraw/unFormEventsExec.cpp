#include "FormTypes.h"
#include "DB_Common.h"
#include "unFormEventsExec.h"

void TFormEventsExec::LoadFormEvents(TwwStringList& p) {
/*mak    std::unique_ptr<QueryUni> q(TQueryFactory::Create(nullptr));
    q->connection = DefDBconnect.Conn;

    q->SQL.Add("SELECT * FROM form_events WHERE form_id = :form_id ORDER BY ordern");
    q->ParamByName("form_id")->AsInteger() = formId;
    q->Open();

    while (!q->EoF()) {
        FormEvent formEvent;
        formEvent.eventclass = q->FieldByName("eventclass")->AsString();
        formEvent.event_id = q->FieldByName("event_id")->AsInteger();
        formEvent.eventrowcolSG = q->FieldByName("eventrowcol")->AsString();
        formEvent.eventrowcol = json::parse(formEvent.eventrowcolSG);
        formEvent.handlerlSG = q->FieldByName("handler")->AsString();
        formEvent.handler = json::parse(formEvent.handlerlSG);
        formEvents.push_back(formEvent);
        q->Next();
    }*/
}

void TFormEventsExec::doManInputs(void* FormShowF1, TwwStringList& p, int nRow, int nCol, string EditString,
                                  double oldValue, int& plan_idHandle, int& cell_idHandle, int& objectidHandle, int& vid_otcHandle, time_t& value_dateHandle){
  /*mak  for (size_t i = 0; i < p.FormEventsList->Size(); i++) {
        PFormEvent FormEvent = (PFormEvent)p->Objects[i];
        if (FormEvent->eventclass != "maninput") {
            continue;
        }
        TFormEventsExec fe;
        fe.FormEvent = FormEvent;
        fe.FormShowF1 = FormShowF1;
        fe.p = p;
        fe.nRow = nRow;
        fe.nCol = nCol;
        fe.oldValue = oldValue;
        fe.newValue = std::stod(EditString);
        fe.doInput1();

        if (fe.cell_id > 0) {
            plan_idHandle = fe.plan_id;
            cell_idHandle = fe.cell_id;
            vid_otcHandle = fe.vid_otc;
            value_dateHandle = fe.value_date;
            objectidHandle = fe.objectid;
        }
    }*/
}

void TFormEventsExec::doSysInputs(void* FormShowF1, TwwStringList& p, int nRow, int nCol, string EditString,
                                  double oldValue) {
    // Implementation of the doSysInputs method
}

void TFormEventsExec::doInput1() {
    // Проверяем Локализацию события по отношению к обработчику события
    // Сеачала про��еряем, тот ли столбец
    /*mak if (FormEvent.eventrowcol == nullptr) return;
    nlohmann::json clmns = FormEvent.eventrowcol["столбцы"];
    if (clmns.is_null()) return;
    bool b = false;
    for (int i = 0; i < clmns.size(); i++) {
        int n;
        if (clmns[i].get<int>(&n)) {
            b = (n == nCol); // проверяем соответствие по столбцу
            if (b) break;
        }
    }
    if (!b) // нет соответствия по столбцу nCol
        return;

    // далее проверяем, та ли строка
    nlohmann::json rows = FormEvent.eventrowcol["строки"];
    if (rows.is_null()) return;
    std::string metod, imj;
    int slka, owner_id;
    if (rows["метод"].get<std::string>(&metod) &&
        rows["имя"].get<std::string>(&imj) &&
        rows["ссылка"].get<int>(&slka)) {
        b = false;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(TFormLogicF1(TFormShowF1(FormShowF1).FormLogicF1), nRow, nCol);
        if (metod == "A") { // любая строка
            b = true;
        } else if (metod == "B") { // Строка ввода, имеющая номер
            b = (slka == nRow);
        } else if (metod == "C") { // Строка ввода с показателем
            b = (slka == vd->PokazId);
        } else if (metod == "D") { // Строка ввода с объектом
            b = (vd != nullptr) && (slka == vd->ObjectId);
        } else if (metod == "E") { // Любая строка ввода, подчиняющаяся вышестоящей строке с показателем
            owner_id = TDB_Pokaztl::GetPokazOwnerId(vd->PokazId);
            b = (vd != nullptr) && (slka == owner_id);
        } else if (metod == "F") { // ��юбая строка ввода, подчиняющаяся вышестоящей строке с объектом
            owner_id = TDB_Pokaztl::GetPokazOwnerId(vd->ObjectId);
            b = (vd != nullptr) && (slka == owner_id);
        }
        if (!b) // нет со��тветствия
            return;
    }
    // End of -> Проверяем Локализацию события по отношению к обработчику события

    // непосредственно вычисления в обрабочике
    doInput1Handle();*/
}

void TFormEventsExec::doInput1Handle() {
    /*mak if (FormEvent.handler == nullptr) {
        return;
    }

    nlohmann::json jo = FormEvent.handler.GetValue("вариант");
    std::string imj;
    if (jo.get_to(imj)) {
        if (imj == "форма") {
            doInput1HandleIn();
        } else if (imj == "показатель") {
            doInput1HandlePkz();
        } else if (imj == "объект") {
            doInput1HandleObj();
        } else if (imj == "объект выше") {
            doInput1HandleObjUp();
        }
    }*/
}

void TFormEventsExec::doInput1HandleIn() {
    /*mak void metodA() {
        double summa = FormLogicF1.f1Forms.NumberRC[nRow+cellrow, cellcol] + newValue;
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdCell.DataType, vdCell.coef_Forma, vdCell.coef_pokaz);
        TUpdDBbyValue.UpdDBbyValue1(vdCell.CellId, vdCell.PlanId, vdCell.VidOtch, vdCell.ValueDate, saveSum, vdCell.ObjectId, false);
        FormLogicF1.f1Forms.NumberRC[nRow+cellrow, cellcol] = summa;
    }

    void metodB() {
        double summa = FormLogicF1.f1Forms.NumberRC[nRow+cellrow, cellcol] +
                        FormLogicF1.f1Forms.NumberRC[nRow+frmlrow, frmlcol];
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdCell.DataType, vdCell.coef_Forma, vdCell.coef_pokaz);
        TUpdDBbyValue.UpdDBbyValue1(vdCell.CellId, vdCell.PlanId, vdCell.VidOtch, vdCell.ValueDate, saveSum, vdCell.ObjectId, false);
        FormLogicF1.f1Forms.NumberRC[nRow+cellrow, cellcol] = summa;
    }

    void metodC() {
        double summa = FormLogicF1.f1Forms.NumberRC[nRow+frmlrow, frmlcol];
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdCell.DataType, vdCell.coef_Forma, vdCell.coef_pokaz);
        TUpdDBbyValue.UpdDBbyValue1(vdCell.CellId, vdCell.PlanId, vdCell.VidOtch, vdCell.ValueDate, saveSum, vdCell.ObjectId, false);
        FormLogicF1.f1Forms.NumberRC[nRow+cellrow, cellcol] = summa;
    }

    json sostav = FormEvent.handler.GetValue("состав").get<json>();
    std::string metod;
    sostav["метод"].get_to(metod);
    json cell = sostav["ячейка"].get<json>();
    int cellrow, cellcol;
    cell["row"].get_to(cellrow);
    cell["col"].get_to(cellcol);
    json frml = sostav["формула"].get<json>();
    int frmlrow, frmlcol;
    if (!frml.is_null()) {
        frml["row"].get_to(frmlrow);
        frml["col"].get_to(frmlcol);
    } else {
        frmlrow = -1;
        frmlcol = -1;
    }
    vdCell = TFormDopInp.GetCellValueAtrByRC(FormLogicF1, nRow+cellrow, cellcol);
    if (metod == "A") {
        metodA();
    } else if (metod == "B") {
        metodB();
    } else if (metod == "C") {
        metodC();
    }
    return 0;*/
}

void TFormEventsExec::doInput1HandlePkz() {
    /*mak void metodA() {
        double summa = newValue - oldValue;
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdVvod.DataType, vdVvod.coef_Forma, vdVvod.coef_pokaz);
        double summaInDB = TDB_CellValue.GetValue(0, cell_id, plan_id, vid_otc, value_date);
        saveSum = saveSum + summaInDB;
        TUpdDBbyValue.UpdDBbyValue1(cell_id, plan_id, vid_otc, value_date, saveSum, 0, false);
    }

    void metodB() {
        double summa = newValue;
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdVvod.DataType, vdVvod.coef_Forma, vdVvod.coef_pokaz);
        double summaInDB = TDB_CellValue.GetValue(0, cell_id, plan_id, vid_otc, value_date);
        saveSum = saveSum + summaInDB;
        TUpdDBbyValue.UpdDBbyValue1(cell_id, plan_id, vid_otc, value_date, saveSum, 0, false);
    }

    void metodC() {
        double summa = FormLogicF1->f1Forms.NumberRC[nRow+frmlrow, frmlcol];
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdVvod.DataType, vdVvod.coef_Forma, vdVvod.coef_pokaz);
        double summaInDB = TDB_CellValue.GetValue(0, cell_id, plan_id, vid_otc, value_date);
        saveSum = saveSum + summaInDB;
        TUpdDBbyValue.UpdDBbyValue1(cell_id, plan_id, vid_otc, value_date, saveSum, 0, false);
    }

    void metodD() {
        double summa = newValue;
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdVvod.DataType, vdVvod.coef_Forma, vdVvod.coef_pokaz);
        TUpdDBbyValue.UpdDBbyValue1(cell_id, plan_id, vid_otc, value_date, saveSum, 0, false);
    }

    void metodE() {
        double summa = FormLogicF1->f1Forms.NumberRC[nRow+frmlrow, frmlcol];
        double saveSum = SystFuncsLogic.PreobrValue(summa, vdVvod.DataType, vdVvod.coef_Forma, vdVvod.coef_pokaz);
        TUpdDBbyValue.UpdDBbyValue1(cell_id, plan_id, vid_otc, value_date, saveSum, 0, false);
    }
    json sostav = FormEvent.handler.GetValue("состав");
    json orto = FormEvent.handler.GetValue("орто");
    json status = FormEvent.handler.GetValue("Статус");
    json zaperiod = FormEvent.handler.GetValue("За период");
    json frml = sostav.value("формула", nullptr);

    std::string metod, imj;
    sostav.at("метод").get_to(metod);
    sostav.at("имя").get_to(imj);
    int pokaz_id;
    sostav.at("ссылка").get_to(pokaz_id);

    int frmlrow = -1, frmlcol = -1;
    if (frml != nullptr) {
        frml.at("row").get_to(frmlrow);
        frml.at("col").get_to(frmlcol);
    }

    vdVvod = TFormDopInp.GetCellValueAtrByRC(FormLogicF1, nRow, nCol);
    int god = vdVvod.ValueDate.year();

    int plan_id = vdVvod.PlanId;
    int slka;
    orto.at("Субъект").get_to(slka);
    if (slka > 0)
        plan_id = slka;

    json status_obj = orto.at("Статус");
    int vid_otc;
    status_obj.at("номер").get_to(vid_otc);
    vid_otc--;
    int ist_fin_id = 0;
    status_obj.at("Ист фин").get_to(slka);
    if (slka > 0)
        ist_fin_id = slka;

    json zaperiod_obj = orto.at("За период");
    std::string ws = zaperiod_obj.at(0).value();
    int cell_period;
    std::tm value_date;
    if (ws == "Усл. пост") {
        cell_period = 0;
        value_date = {0, 1, 2, 0, 0, 0, 0, 0, -1};
    } else if (ws == "Год") {
        cell_period = 1;
        value_date = {0, 12, 31, 0, 0, 0, 0, 0, -1};
    } else if (ws == "Квартал") {
        cell_period = 2;
        int n;
        zaperiod_obj.at("Квартал").get_to(n);
        if (n == 0)
            n = SystFuncsLogic.GetNKvbyDate(vdVvod.ValueDate);
        value_date = GetLastDayDate(god, n, 0);
    } else if (ws == "Месяц") {
        cell_period = 3;
        int n;
        zaperiod_obj.at("Месяц").get_to(n);
        if (n == 0)
            n = vdVvod.ValueDate.month();
        value_date = GetLastDayDate(god, 0, n);
    }
    int cell_id = TDB_Cell.GetCellId(pokaz_id, cell_period, ist_fin_id);

    this->plan_id = plan_id;
    this->cell_id = cell_id;
    this->vid_otc = vid_otc;
    this->value_date = value_date;
    objectid = 0;

    if (metod == "A")
        metodA();
    else if (metod == "B")
        metodB();
    else if (metod == "C")
        metodC();
    else if (metod == "D")
        metodD();
    else if (metod == "E")
        metodE();*/
}

void TFormEventsExec::doInput1HandleObj() {
//mak  TFormLogicF1* FormLogicF1;
  //-=-FormLogicF1 = static_cast<TFormLogicF1*>(static_cast<TFormShowF1*>(FormShowF1)->FormLogicF1);
}

void TFormEventsExec::doInput1HandleObjUp() {
//mak  TFormLogicF1* FormLogicF1;
  //-=-FormLogicF1 = static_cast<TFormLogicF1*>(static_cast<TFormShowF1*>(FormShowF1)->FormLogicF1);
}























