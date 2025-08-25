#ifndef TT_KortegH
#define TT_KortegH

#include "../DB/DB_Common.h"

class T_Korteg
{
public:
    // Создать списки КБК для получ и распорядительных счетов
    static void CreateListKBK_Plc_Rsp(int CurrYear, int pokaz_idPlc);

    // Создать списки КБК для получательных счетов
    static void CreateListKBK_Plc(int CurrYear, int pokaz_idPlc);

    // Создать списки КБК для получательных счетов с учетом подчинения
    static void CreateListKBK_PlcVert(int CurrYear, int pokaz_idPlc);

    // Скопировать список КБК для получательных счетов
    static bool CopyListKBK_Plc(TStringList *aSl);

    // Создать списки КБК для распорядительных счетов
    static void CreateListKBK_Rsp(int CurrYear, int pokaz_idPlc);

    // Удалить списки КБК для получательных и распорядительных счетов
    static void DeleteListKBK_Plc_Rsp();

    // Удалить списки КБК для получательных счетов
    static void DeleteListKBK_Plc();

    // Удалить списки КБК для распорядительных счетов
    static void DeleteListKBK_Rsp();

    // Найти КБК для получательных или распорядительных счетов
    static int FindByKBK_Plc_Rsp(int otkuda, std::string KBK, int pokaz_idPlc, int CurrYear, bool isMsg);

    // Найти КБК для получательных счетов
    static int FindByKBK_Plc(std::string KBK, int pokaz_idPlc, int CurrYear, bool isMsg);

    // Найти КБК для распорядительных счетов
    static int FindByKBK_Rsp(std::string KBK, int pokaz_idPlc, int CurrYear, bool isMsg);

    // Получить количество элементов в списке GlobalVars::slRspKorteg
    static int getCountRsp();

    // Получить сведения о i-м элементе в списке GlobalVars::slRspKorteg - нумерация с 0
    static bool getNumI(int i, std::string &KBK, int &pokaz_id);
};

#endif
