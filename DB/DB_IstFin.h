#ifndef DB_ISTFIN_H
#define DB_ISTFIN_H

#include <string>
#include "DB_Common.h"

const int ist_fin_id_CBRTOkruga = 6;
const int ist_fin_id_TrBudg = 3;
const int ist_fin_id_FedBudg = 2;
const int ist_fin_id_Budg2  = 2;
const int ist_fin_id_CelSubsid  = 4;

class TDB_IstFin {
public:
    static void GetIstFin_All(TQueryUni* qIn);
    static int GetTermIstFin();
    static bool GetIstFin(TQueryUni* qIn, int ist_fin_id);
    static std::string getIstFinName(int ist_fin_id);
    static int getOrder(int ist_fin_id);
    static int GetIstFinIdBySubs(int SubsidPokazId);
    static void InsertVsego();
    static int GetIdVsego();
    static int InsertIstFin(std::string ist_name);
    static void ModIstFin(std::string ist_name, int ist_fin_id);
    static void Modorder(int ist_fin_order, int ist_fin_id);
    static int UnikName(std::string ist_name);
    static bool UnikName_Izm(std::string ist_name, int ist_fin_id);
    static void DelIstFin(int ist_fin_id);
    static int getIstFinIdByPokazId(int pokaz_id);
    static std::string getCodeCelSubsByIstFinId(int ist_fin_id);
    static int getPokazIdIFbyIstFinId(int ist_fin_id);
};

#endif // DB_ISTFIN_H
