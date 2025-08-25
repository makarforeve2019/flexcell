#ifndef DB_EDIZM_H
#define DB_EDIZM_H

#include <string>
#include "DB_Common.h"

class TDB_EdIzm {
public:
    static int GetId();
    static bool GetEdIzm(TQueryUni* qIn, int edizm_id);
    static void GetEdIzmAll(TQueryUni* qIn);
    static std::string GetEdIzmName(int edizm_id);
    static int GetEdIzmId(std::string edizm_name);
    static int GetEdizm_is_gr(int edizm_id);
    static int GetEdizm_order(int edizm_id);
    static int AddEdIzm(std::string edizm_name, int edizm_is_gr, int edizm_owner_id, int edizm_order, int edizm_coef);
    static void UpdEdIzm1(int edizm_id, int edizm_owner_id, int edizm_order);
    static void UpdEdIzm2(int edizm_id, std::string edizm_name);
    static void UpdEdIzm3(int edizm_id, std::string edizm_name, int edizm_coef);
    static void DelEdIzm(int edizm_id);
    static std::string GetEdIzmNameOpt(int edizm_id);
    static int GetCoef_PokazOpt(int edizm_id);
};

#endif // DB_EDIZM_H
