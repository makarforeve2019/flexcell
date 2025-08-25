#ifndef DB_FORMSTRTMP_H
#define DB_FORMSTRTMP_H

#include <string>

#include "DB_Common.h"

class TDB_FormStrTMP {
public:
    static void CreateForm_strTMP();
    static void DropForm_strTMP();
    static int GetMaxNum(int form_id);
    static void AddPokaz(int form_id, int& aForm_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int is_filter, int period, int ist_fin_grad_id);
    static void GetAllPokaztl(TQueryUni* qIn, int form_id);
    static void AddProizStr(int form_id, int& aForm_str_n, std::string note1, int Level_sign, int nStolbec);
    static void DelStr(int str_id);
    static void SetValue(int str_id, double value);
    static void SetValueB(int str_id, double valueB);
    static void SetValue_Iparu(int str_id, int i, double value, double valueB);
    static double GetValue(int str_id);
    static double GetValueB(int str_id);
    static void GetValue_Iparu(int str_id, int i, double& value, double& valueB);
    static void SetStrValue(int str_id, std::string str_value);
    static std::string GetStrValue(int str_id);
    static void UpdNote1(int str_id, std::string note1);
    static void UpdForm_str_n(int form_id, int str_id);
    static void GetItemsN1N2(TQueryUni* qIn, int form_id, int N1, int N2);
    static void GetItemsAll(TQueryUni* qIn, int form_id);
    static void SetFont(int form_id, int form_str_n, std::string font, int h, int is_bold);
};

#endif // DB_FORMSTRTMP_H
