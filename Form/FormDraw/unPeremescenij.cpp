#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Plan.h"
#include "unPeremescenij.h"

double TPeremescenij::doEval(int plan_id, int pokaz_id, int VidOtc, int god, int NValue) {
    double Result = 0.0;
    // проверки
    if ((NValue == 1 || NValue == 2 || NValue == 3) && plan_id == 0) {
        //--SystFuncs.MessageBoxExclamation('При вычислении значения Объема перемещений значение plan_id оказалось пустым', MsgVnim);
        return Result;
    }

    switch (NValue) {
        case 1:
            Result = TPeremescenij::doEval1(plan_id, pokaz_id, VidOtc, god);
            break;
        case 2:
            Result = TPeremescenij::doEvalSnjto(plan_id, pokaz_id, VidOtc, god);
            break;
        case 3:
            Result = TPeremescenij::doEvalPologeno(plan_id, pokaz_id, VidOtc, god);
            break;
        case 4:
            Result = TPeremescenij::doEval1(1, pokaz_id, VidOtc, god);
            break;
        case 5:
            Result = TPeremescenij::doEvalSnjto(1, pokaz_id, VidOtc, god);
            break;
        case 6:
            Result = TPeremescenij::doEvalPologeno(1, pokaz_id, VidOtc, god);
            break;
        case 7:
            Result = TPeremescenij::doEvalSnjtoRaspred(plan_id, pokaz_id, VidOtc, god);
            break;
        case 8:
            Result = TPeremescenij::doEvalPologenoRaspred(plan_id, pokaz_id, VidOtc, god);
            break;
        case 9:
            Result = TPeremescenij::doEvalSnjtoRaspred(1, pokaz_id, VidOtc, god);
            break;
        case 10:
            Result = TPeremescenij::doEvalPologenoRaspred(1, pokaz_id, VidOtc, god);
            break;
    }

    return Result;
}

double TPeremescenij::doEval1(int plan_id, int pokaz_id, int VidOtc, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    double sumvalue = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(summa_otkuda) as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    // q->SQL->Add("WHERE a.god = :god");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0"); // 13 янв 2015        
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_otkuda > 0");
    if (plan_id > 1)
        q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_otkuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    if (plan_id > 1)
        q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
  
    if (!q->EoF())
        sumvalue += (-q->FieldByName("summa")->AsFloat());
    else 
        sumvalue = 0;
  
    q->Close();

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(summa_kuda) as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    // q->SQL->Add("WHERE a.god = :god");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0"); // 13 янв 2015        
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_kuda > 0");
    if (plan_id > 1)
        q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_kuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    if (plan_id > 1)
        q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
  
    if (!q->EoF())
        sumvalue += q->FieldByName("summa")->AsFloat();
    else
        sumvalue = 0;
  
    q->Close();

    delete q;
    return sumvalue;
}

double TPeremescenij::doEvalSnjto(int plan_id, int pokaz_id, int VidOtc, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    double sumvalue = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(summa_otkuda) as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    // q->SQL->Add("WHERE a.god = :god");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0"); // 13 янв 2015        
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND a.isRezervir = 1");
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_otkuda > 0");
    if (plan_id > 1)
        q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_otkuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    if (plan_id > 1)
        q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF())
        sumvalue += q->FieldByName("summa")->AsFloat();
    else
        sumvalue = 0;

    q->Close();
    delete q;
    return sumvalue;
}

double TPeremescenij::doEvalPologeno(int plan_id, int pokaz_id, int VidOtc, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    double sumvalue = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(summa_kuda) as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    // q->SQL->Add("WHERE a.god = :god");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0"); // 13 янв 2015        
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND a.isRezervir = 1");
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_kuda > 0");
    if (plan_id > 1)
        q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_kuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    if (plan_id > 1)
        q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF())
        sumvalue += q->FieldByName("summa")->AsFloat();
    else
        sumvalue = 0;

    q->Close();
    delete q;
    return sumvalue;
}

double TPeremescenij::doEvalSnjtoRaspred(int plan_id, int pokaz_id, int VidOtc, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    double sumvalue = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(summa_otkuda) as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    // q->SQL->Add("WHERE a.god = :god");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0"); // 13 янв 2015        
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND a.isRezervir = 2");
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_otkuda > 0");
    if (plan_id > 1)
        q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_otkuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    if (plan_id > 1)
        q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF())
        sumvalue += q->FieldByName("summa")->AsFloat();
    else
        sumvalue = 0;

    q->Close();
    delete q;
    return sumvalue;
}

double TPeremescenij::doEvalPologenoRaspred(int plan_id, int pokaz_id, int VidOtc, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    double sumvalue = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(summa_kuda) as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    // q->SQL->Add("WHERE a.god = :god");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0"); // 13 янв 2015        
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND a.isRezervir = 2");
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_kuda > 0");
    if (plan_id > 1)
        q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_kuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    if (plan_id > 1)
        q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF())
        sumvalue += q->FieldByName("summa")->AsFloat();
    else
        sumvalue = 0;

    q->Close();
    delete q;
    return sumvalue;
}



