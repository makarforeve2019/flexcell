#ifndef DB_PLAN_H
#define DB_PLAN_H

#include <string>
#include <vector>
#include <unordered_map>
#include "DB_Common.h"

extern TStringList slPlans;

class TDB_Plan {
public:
    // чтение всех субъектов мониторинга, доступных текущему пользователю
    static void GetAllPlan_forUser(TQueryUni* qIn, int user_id);

    // добавить субъекта мониторинга
    static int AddPlan(int plan_id, const std::string plan_name, const std::string plan_short_name, int plan_owner_id,
                       int plan_order, int plan_is_rajon, const std::string PPP, const std::string FO, const std::string OKATO_kod,
                       int god_beg, int god_end, int isUKP_byKBK, const std::string email);

    // чтение субъекта мониторинга по plan_id
    static bool GetPlan(TQueryUni* qIn, int plan_id);

    // чтение наименования огруга по plan_id (который необязательно округ, может быть ему подчиненный) и ППП
    static std::string GetOkrugNamePPP(int plan_id, std::string PPP);

    // получить наименования огруга в дательном падеже - кому
    static std::string GetOkrugNameTo(std::string okrugName);

    // получить наименования огруга в родительском падеже - кого
    static std::string GetOkrugNameGo(std::string okrugName);

    // чтение наименования субъекта мониторинга по plan_id
    static std::string GetPlanName(int plan_id);

    // чтение краткого наименования субъекта мониторинга по plan_id
    static std::string GetPlanShortName(int plan_id);

    // чтение OKATO_kod субъекта мониторинга по plan_id
    static std::string GetOKATO_kod(int plan_id);

    // получение parent_id по plan_id
    static int GetParentIid(int plan_id, bool isShowMSG);

    // получение подведомств учреждения для Управы района plan_id
    static int GetPodVedomstvId(int plan_id, bool isShowMSG, int& plan_id_2podvedomstv);

    // чтение порядкового номера субъекта мониторинга по plan_id
    static int GetPlan_order(int plan_id);

    // чтение поля isUKP_byKBK по plan_id
    static int GetIsUKP_byKBK(int plan_id);

    // чтение Max значения plan_id
    static int GetMaxPlan_Id();

    // чтение Max действительного порядкового номера для plan_owner_id или 0
    static int GetMaxPlan_order(int plan_owner_id);

    // изменить порядок субъекта мониторинга
    static void UpdPlanOrder(int plan_id, int plan_order);

    // удалить субъект мониторинга
    static void DelPlan(int plan_id);

    // изменить характеристики субъекта мониторинга 1
    static void UpdPlan1(int plan_id, const std::string plan_name, const std::string plan_short_name, const std::string PPP, const std::string FO,
                         const std::string OKATO_kod, int isUKP_byKBK, const std::string email);

    // изменить характеристики субъекта мониторинга 2
    static void UpdPlan2(int plan_id, const std::string plan_name, int plan_is_rajon, const std::string plan_short_name,
                         const std::string FO, const std::string OKATO_kod, int god_beg, int god_end, const std::string email);

    // изменить parent_id
    static void UpdParentId(int plan_id, int parent_id);

    // изменить kodSPZ
    static void UpdkodSPZ(int plan_id, std::string kodSPZ);

    // найти plan_id по kodSPZ
    static int findBykodSPZ(std::string kodSPZ);

    // прочитать kodSPZ по plan_id
    static std::string getkodSPZ(int plan_id);

    // прочитать kodUBP по plan_id
    static std::string getKodUBP(int plan_id);

    // изменить kodUBP
    static void updKodUBP(int plan_id, std::string kodUBP);

    // прочитать sinonims по plan_id
    static std::string getSinonims(int plan_id);

    // изменить sinonims
    static void updSinonims(int plan_id, std::string sinonims);

    // прочитать UNK по plan_id
    static std::string getUNK(int plan_id);

    // изменить UNK
    static void updUNK(int plan_id, std::string UNK);

    // прочитать nameGosZakazcik по plan_id
    static std::string getNameGosZakazcik(int plan_id);

    // изменить nameGosZakazcik
    static void updNameGosZakazcik(int plan_id, std::string nameGosZakazcik);

    // прочитать nameOIV по plan_id
    static std::string getNameOIV(int plan_id);

    // изменить nameOIV
    static void updNameOIV(int plan_id, std::string nameOIV);

    // чтение plan_is_rajon субъекта мониторинга по plan_id
    static int GetPlanIsRajon(int plan_id);

    // чтение plan_is_rajon субъекта мониторинга через parent_id для заданного plan_id
    static int GetPlanIsRajonForParent(int plan_id);

    // чтение всех субъектов мониторинга, кроме 1 и ORDER BY plan_owner_id, plan_order
    // для которых plan_is_rajon <=3
    static void GetPlan_All_03(TQueryUni* qIn);

    // чтение всех субъектов мониторинга, кроме 1 и ORDER BY plan_owner_id, plan_order
    // где на plan_is_rajon не ограничений
    static void GetPlan_All(TQueryUni* qIn);

    // чтение всех субъектов мониторинга, доступных текущему пользователю
    static void GetPlan_AllforUser(TQueryUni* qIn);

    // чтение всех субъектов мониторинга первых трех видов, подчиненных plan_owner_id
    static void GetPlan_ForOwner(TQueryUni* qIn, int plan_owner_id);

    // чтение всех субъектов мониторинга, подчиненных plan_owner_id
    static bool GetSubPlans(TQueryUni* qIn, int plan_owner_id);

    // чтение округа (plan_owner_id = 0 и plan_id > 1)
    static void GetPlanOkrug(TQueryUni* qIn);

    // выборка ППП округа
    static std::string GetPPPOkrug();

    // выборка ФО округа
    static std::string GetFOOkrug();
    static std::string GetOrFK(int plan_id);

    // выборка наименования ФО округа
    static std::string GetNameFOOkrug();

    // выборка plan_id округа
    static int GetIdOkrug();

    // выборка plan_id Префектуры округа
    static int GetIdPrefOkrug();

    // чтение краткого наименования округа
    static std::string GetShotNameOkrug();

    // выбор plan_id по коду РКАТО
    static int getIdByOKATO(const std::string aOKATO);

    // прочитать plan_owner_id
    static int getOwner(int plan_id);

    // запретить (разрешить) изменения уточ плана для всех plan_id
    static void SetIsNoUpdUtocPlan(bool isNo);

    // запретить (разрешить) изменения уточ плана для plan_id
    static void SetIsNoUpdUtocPlan1(int plan_id, bool isNo);

    // запретить (разрешить) изменения уточ плана для всех plan_id (Трад бюджет)
    static void SetIsNoUpdUtocPlanTB(bool isNo);

    // запретить (разрешить) изменения уточ плана для plan_id (Трад бюджет)
    static void SetIsNoUpdUtocPlanTB1(int plan_id, bool isNo);

    // вернуть значение isNoUpdUtocPlan для plan_id
    static void GetIsNoUpdUtocPlan1(int plan_id, int& isNoUpdUtocPlan, int& isNoUpdUtocPlanTB);

    // разрешено ли менять уточненный план для plan_id
    static bool IsRazreseno(int plan_id, int ist_fin_id);

    // найти plan_id по UNK
    static int findByUNK(const std::string UNK);

    // найти plan_id по plan_name
    static int findByPlanName(const std::string plan_name);

    // функция проверки уникальности имени при добавлении
    static bool UnikName(const std::string plan_name, int plan_owner_id);

    // функция проверки уникальности имени при изменении
    static bool UnikName_Izm(int plan_id, const std::string plan_name, int plan_owner_id);
};
#endif
