#include "unDefDBconnect.h"
#include "SystFuncsLogic.h"
#include "FormData.h"

TFormDM::TFormDM()
{

    // 12 окт 2016 пр переходе на FireDac вместо TADOQueryUni использую TQueryUni
    quFormStr = new TQueryUni(DefDBconnect.Conn);
    quFormStr->SQL->Add("SELECT * FROM Form_str");
    quFormStr->SQL->Add("WHERE form_id = :CurrFormId");
    quFormStr->SQL->Add("AND ((form_str_n < 1500) or (form_str_n BETWEEN 11000 AND 21000))");

    quValue = new TQueryUni(DefDBconnect.Conn);
    quValue->SQL->Add("SELECT * FROM Cell_value");
    quValue->SQL->Add("WHERE objectid = :objectid");
    quValue->SQL->Add("AND cell_id = :CurrCellId AND plan_id = :CurrPlanId");
    quValue->SQL->Add("AND vid_otc = :CurrVidOtch AND value_date  = :CurrValueDate");

    quPokaztl = new TQueryUni(DefDBconnect.Conn);
    quPokaztl->SQL->Add("SELECT pokaz_id, pokaz_type, edizm_id, enum_id FROM Pokaztl");
    quPokaztl->SQL->Add("WHERE pokaz_base_id = :PokazId AND np_vid_id = :NPVIDId");

    quIstFin = new TQueryUni(DefDBconnect.Conn);
    quIstFin->SQL->Add("SELECT ist_fin_id FROM Ist_fin");
    quIstFin->SQL->Add("ORDER BY ist_fin_order");

    quPokaz = new TQueryUni(DefDBconnect.Conn);
    quPokaz->SQL->Add("SELECT * FROM Pokaztl");
    quPokaz->SQL->Add("WHERE pokaz_id = :PokazID");
//    quPokaz->SQL->Add("AND :God BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
    quPokaz->SQL->Add("AND :God BETWEEN date_part(''year'', pokaz_beg_date) AND date_part(''year'', pokaz_end_date)");

    quDinIzm = new TQueryUni(DefDBconnect.Conn);
    quDinIzm->SQL->Add("SELECT din_izm_id FROM Din_izm");
    quDinIzm->SQL->Add("WHERE din_izm_owner_id = :DinIzmOwnerId");
    quDinIzm->SQL->Add("ORDER BY din_izm_order");

    quValuePeriod = new TQueryUni(DefDBconnect.Conn);
    quValuePeriod->SQL->Add("SELECT value_date, value FROM Cell_value");
    quValuePeriod->SQL->Add("WHERE objectid = :objectid");
    quValuePeriod->SQL->Add("AND cell_id = :CurrCellId AND plan_id = :CurrPlanId");
    quValuePeriod->SQL->Add("AND vid_otc = :CurrVidOtch");
//    quValuePeriod->SQL->Add("AND DATEPART(YEAR, value_date) = :CurrYear");
    quValuePeriod->SQL->Add("AND date_part(''year'', value_date) = :CurrYear");

    quSumPotok = new TQueryUni(DefDBconnect.Conn);
    quSumPotok->SQL->Add("SELECT -SUM(summa_otkuda) as sumval FROM Utoc_plana");
    quSumPotok->SQL->Add("WHERE plan_id = :PlanId");
    quSumPotok->SQL->Add("AND god = :God");
    quSumPotok->SQL->Add("AND kv_otkuda >= :kv0");
    quSumPotok->SQL->Add("AND kv_otkuda <= :kv");
    quSumPotok->SQL->Add("AND cell_id_otkuda = :cellId");
    quSumPotok->SQL->Add("AND budget_poluc_id = 0");
    quSumPotok->SQL->Add("UNION");
    quSumPotok->SQL->Add("SELECT SUM(summa_kuda) as sumval FROM Utoc_plana");
    quSumPotok->SQL->Add("WHERE plan_id = :PlanId");
    quSumPotok->SQL->Add("AND god = :God");
    quSumPotok->SQL->Add("AND kv_kuda >= :kv0");
    quSumPotok->SQL->Add("AND kv_kuda <= :kv");
    quSumPotok->SQL->Add("AND cell_id_kuda = :cellId");
    quSumPotok->SQL->Add("AND budget_poluc_id = 0");

    quFormColmn = new TQueryUni(DefDBconnect.Conn);
    quFormColmn->SQL->Add("SELECT * FROM Pokaztl");
    quFormColmn->SQL->Add("WHERE pokaz_id =1");

    quOstatok = new TQueryUni(DefDBconnect.Conn);
    quOstatok->SQL->Add("SELECT 1 as vid, value FROM Cell_value");
    quOstatok->SQL->Add("WHERE objectid = :objectid");
    quOstatok->SQL->Add("AND cell_id = :CellIdKv AND plan_id = :CurrPlanId");
    quOstatok->SQL->Add("AND vid_otc = 4 AND value_date  = :ValueDateKv");
    quOstatok->SQL->Add("UNION");
    quOstatok->SQL->Add("SELECT 2 as vid, sum(value) as value FROM Denegn_potok");
    quOstatok->SQL->Add("WHERE cell_id = :CellIdMes");
    quOstatok->SQL->Add("AND objectid = :objectid");
    quOstatok->SQL->Add("AND pokaz_id = :CurrPokazId");
    quOstatok->SQL->Add("AND   plan_id = :CurrPlanId");
    quOstatok->SQL->Add("AND   vid_otc = :VidOtc");
    quOstatok->SQL->Add("AND   value_date BETWEEN :BegDate AND :EndDate");

}

TFormDM::~TFormDM()
{
    delete(quFormStr);
    delete(quValue);
    delete(quPokaztl);
    delete(quIstFin);
    delete(quPokaz);
    delete(quDinIzm);
    delete(quValuePeriod);
    delete(quSumPotok);
    delete(quFormColmn);
    delete(quOstatok); 
    
}