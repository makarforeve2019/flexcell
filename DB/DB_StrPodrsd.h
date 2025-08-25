#ifndef DB_StrPodrsdH
#define DB_StrPodrsdH


#include "DB_Common.h"
#include <string>

class TDB_StrPodrsd {
public:
    // возвращает strpodr_id добавленной записи
    static int GetId();

    // добавление структурного подразделения возвращает strpodr_id
    static int AddStrPodrsd(int plan_id, std::string strpodrName, std::string strpodrCode);

    // чтение всех структурных подразделений
    static void GetAllStrPodrsd(TQueryUni *qIn);

    // чтение стр подразделения по strpodr_id
    static bool GetStrPodrsdById(TQueryUni *qIn, int strpodr_id);

    // возвращает plan_id по strpodr_id
    static int GetPlanId(int strpodr_id);

    // изменение записи для strpodr_id
    static void UpdStrPodrsd(int strpodr_id, std::string strpodrName, std::string strpodrCode);

    // проверяет уникальность кода добавлении
    static bool UnikCode(std::string strpodrCode);

    // проверяет уникальность кода измнении
    static bool UnikCodeIzm(std::string strpodrCode, int strpodr_id);

    // удаление стр подразделение
    static void DelStrPodrsd(int strpodr_id);

    // чтение сопоставления структурного подразделения пользователю
    static int GetStrPodrsdForUser(int user_id, std::string &strpodrName, std::string &strpodrCode);

    // изменить сопоставление структурного подразделения пользователю
    static void UpdStrPodrsdForUser(int user_id, int strpodr_id);
};

#endif
