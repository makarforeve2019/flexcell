
#ifndef DB_BPolucRajH
#define DB_BPolucRajH

#include <string>
#include "DB_Common.h" 


static int wplan_id;
static int wbudget_poluc_id;
static std::string wshort_name;

class TDB_BPolucRaj
{
public:
    // по Лицевому счету найти Бюджетополучателя и plan_id
    static bool GetBP_IdByLicScet(std::string rasc_scet, int &budget_poluc_id, int &kod_oshibki, int &plan_id);
    
    // по plan_id выбрать характеристики первого Бюджетополучателя - Наименование полное
    static int getBP1forTBpolnoe(int god, int plan_id, std::string &rasc_scet, std::string &short_name, std::string &INN, std::string &KPP);
    
    // по ИНН найти Бюджетополучателя и plan_id
    static bool GetBP_IdByINN(std::string INN, int &budget_poluc_id, int &kod_oshibki, int &plan_id);
    
    // по plan_id есть ли Бюджетополучателя
    static bool IsBPByPlanId(int plan_id);
    
    // по plan_id выбрать характеристики первого Бюджетополучателя для ТБ (по 1 в 6-ом разряде в лиц счете)
    static int getBP1forTB(int god, int plan_id, std::string &rasc_scet, std::string &short_name, std::string &INN);
    
    // по plan_id и budget_poluc_id найти другой район, к которому прикреплен БП
    static std::string IsBPOtherPlanId(int plan_id, int budget_poluc_id);
    
    // дать имя БП по plan_id и шаблону (получат-тб)
    static std::string GetBPNamebyPlanIdTBPlc(int god, int plan_id);
    
    // дать расчетный получательный счет по plan_id и шаблону (тб)
    static std::string RSPlc_GetBPbyPlanIdTB(int god, int plan_id);
    
    // дать расчетный распределительный счет по plan_id и шаблону (тб)
    static std::string RSRsp_GetBPbyPlanIdTB(int plan_id);
    
    // дать Бюджетополучателя (id) - распорядителя по plan_id и шаблону (тб)
    static int idRsp_GetBPbyPlanIdTB(int plan_id, std::string &short_name, int &god_end); // overload
    
    // дать Бюджетополучателя (id) - распорядителя по plan_id и шаблону (тб)
    static int idRsp_GetBPbyPlanIdTB(int plan_id, std::string &short_name); // overload
    
    // дать Бюджетополучателя (id) - получателя по plan_id и шаблону (тб)
    static int idPlc_GetBPbyPlanIdTB(int god, int plan_id, std::string &short_name);
    
    // по plan_id и шаблону (получат-тб) выбрать БП
    static void GetBPbyPlanIdTBPlc(int god, TQueryUni *qIn, int plan_id);
    
    // по plan_id и шаблону (распред-тб) выбрать БП
    static void GetBPbyPlanIdTBRsp(TQueryUni *qIn, int plan_id);
    
    // дать бюджетополучателя по plan_id и шаблону (тб)
    static int BPPlcTB_GetBPbyPlanId(int god, int plan_id, std::string &short_name);
    
    // получить расчетный счет по plan_id
    static std::string GetRsByPlan_id(int plan_id);
};

#endif
