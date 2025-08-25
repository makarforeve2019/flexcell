
#ifndef PROCFORMULA_H
#define PROCFORMULA_H

#include <stack>

class ProcFormula {
public:
    static int GetPrioritetStr(std::string s);
    static int GetPrioritetStack(std::stack<std::string*> aStack);
    static int ComparePrioritet(std::string m_Str, std::stack<std::string*> m_Stack);
    static std::string GetNextElement(std::string aFormula, int& aI);
    static double GetValueElement(int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate);
    static double GetValueElement(int aCell_id, int nKv, int nMonth, int m_Plan_id, int m_Vid_otc, int m_Year);
    static std::string GetPolishRec(std::string m_Formula);
    static double CalcPolishRec(std::string m_PolRec, int m_Plan_id, int m_Vid_otc, TDateTime m_Date);
    static double CalcPolishRec(std::string m_PolRec, int nKv, int nMonth, int m_Plan_id, int m_Vid_otc, int m_Year);
    static double CalcSumPoKodAU(int nCellOut, std::string strFormula, int m_Plan_id, int m_Vid_otc, TDateTime m_Date);
};

#endif // PROCFORMULA_H