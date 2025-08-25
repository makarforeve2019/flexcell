#ifndef FormSortedH
#define FormSortedH

#include "unF1toDE.h"
#include "unFormLogicF1.h"
#include "FormDopInp.h"
#include "FormTypes.h"

struct TDefColSort {
    int uroven_sort;
    int NCol;
    int is_sort;
    int is_svod;
    std::string name_svod;
};

struct TDefColSum {
    bool is_sum;
};

const int n10 = 10;

class TFormSorted {
public:
    TFormLogicF1* FormLogicF1;
    int BegRow, ncol1, EndRow, ncol2;
    bool isKusocno;
    TwwStringList* p;
    int form_str_n, uroven_sort, MaxUS, is_sort, NameCol;
    TDefColSort DefColSorts[n10];
    TDefColSum DefColSum[100];
    TList<PAtrRow> ListRows;
    bool IsInsert;
    int gde_svod;
    bool isFormReadonly, isSortSvod, isFormReadonlySortSvod;
    unordered_map<int, std::string> oldZnachArr[n10];
    unordered_map<int, std::string> oldZnachParentArr[n10];

    static void SortedForm(TFormLogicF1* FormLogicF1, int BegRow, int ncol1, int EndRow, int ncol2, bool isKusocno);
    void SetBold(bool aBold);
    bool IsGoodStmnt(std::string aStmnt);
    std::string GetLicScet(int anUS, int aRow, int& aCol);
    std::string GetBKCode(int anUS, int aRow, int& aCol);
    void SetFormula(int anUS, int toRow, int toCol, double aD);
    void Sumiruem(int anUS, int aRow);
    bool wGetListRowsItem(int aRow);
    void Sumiruem2(int anUS, int aRow);
    void Perenumerovati(int nBegRow, int nMaxRow);
    void InsertRow(int anUS, int aRow);
    void InsertIntoRow(int anUS, int aRow);
    bool Sravnim(int anUS, int aRow);
};

#endif