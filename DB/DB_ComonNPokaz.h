#ifndef DB_COMONNPOKAZ_H
#define DB_COMONNPOKAZ_H

#include <string>
#include "DB_Common.h"

class TDB_ComonNPokaz {
public:
    static int GetId();
    static void GetAllItems(TQueryUni *qIn) ;
    static void GetComonNPokaz(TQueryUni *qIn, int comnp_id) ;
    static int GetTypeComonNPokaz(int comnp_id);
    static bool isComonNPokaz(int comnp_id);
    static std::string GetComnpName(int comnp_id);
    static int GetIsFieldsSprvcnik(int comnp_id);
    static int GetEnum_id(int comnp_id);
    static int GetorderN(int comnp_id);
    static bool UnikAddName(std::string comnp_name);
    static bool UnikUpdName(std::string comnp_name, int comnp_id);
    static int GetMaxOrderN();
    static void UpdOrderN1N2(int comnp_id1, int comnp_id2);
    static int AddComonNPokaz(std::string comnp_name, int comnp_type, int enum_id, int edizm_id, std::string primecanie, std::string toTblName,
                   int IsFieldsSprvcnik, std::string toColumnName1, int is_const, unsigned char is_mes, unsigned char is_kv, unsigned char is_god);
    static void UpdComonNPokaz(int comnp_id, std::string comnp_name, int edizm_id, std::string primecanie, int enum_id);
    static void UpdorderN(int comnp_id, int orderN);
    static void DelComonNPokaz(int comnp_id);
    static bool GetComnpIdByComnpName(std::string comnp_name, int& comnp_id);
    static void GetInfo(int comnp_id, unsigned char& IsFieldsSprvcnik, std::string& toTblName, std::string& toColumnName1);
    static int GetEdizm_id(int comnp_id);
    static int GetComnp_type(int comnp_id);
};

#endif // DB_COMONNPOKAZ_H
