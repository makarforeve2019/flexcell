#include "../SystFuncs/SystFuncsLogic.h"
#include "ProcFormula.h"


int ProcFormula::GetPrioritetStr(std::string s) {
    unsigned char Res = 5;
    if (s == "+" || s == "-") {
        Res = 1;
    } else if (s == "*" || s == "/") {
        Res = 3;
    } else if (s == "(") {
        Res = 7;
    } else if (s == ")") {
        Res = 0;
    }
    return Res;
}

int ProcFormula::GetPrioritetStack(std::stack<std::string*> aStack) {
    unsigned char Res = 6;
    std::string *ss = aStack.top();
    if (*ss == "+" || *ss == "-") {
        Res = 2;
    } else if (*ss == "*" || *ss == "/") {
        Res = 4;
    } else if (*ss == "(") {
        Res = 0;
    }
    return Res;
}

int ProcFormula::ComparePrioritet(std::string m_Str, std::stack<std::string*> m_Stack) {
    unsigned char aStr = GetPrioritetStr(m_Str);
    unsigned char aStack = ProcFormula::GetPrioritetStack(m_Stack);
    unsigned char result = 0;
    if (aStr > aStack) {
        result = 1;
    } else if (aStr == aStack) {
        result = 2;
    } else if (aStr < aStack) {
        result = 3;
    }
    return result;
}

std::string ProcFormula::GetNextElement(std::string aFormula, int& aI) {
    std::string Result;
    if (aI > aFormula.length()) {
        return Result;
    }
    if (aFormula[aI] == 'V') {
        std::string sCellId = "V";
        for (int i = aI + 1; i < aFormula.length(); i++) {
            if (!(aFormula[i] >= '0' && aFormula[i] <= '9')) {
                break;
            }
            sCellId += aFormula[i];
            aI = i + 1;
        }
        Result = sCellId;
        return Result;
    }
    if (aFormula[aI] == 'C') {
        std::string sCellId = "C";
        for (int i = aI + 1; i < aFormula.length(); i++) {
            if (aFormula[i] == '*' || aFormula[i] == '/' || aFormula[i] == '(' || aFormula[i] == ')' || aFormula[i] == 'V') {
                break;
            }
            if ((aFormula[i] == '+' || aFormula[i] == '-') && aFormula[i - 1] != 'C') {
                break;
            }
            sCellId += aFormula[i];
            aI = i + 1;
        }
        Result = sCellId;
        return Result;
    }
    aI++;
    Result = aFormula[aI - 1];
    return Result;
}

std::string ProcFormula::ProcFormula::GetPolishRec(std::string m_Formula) {
    std::string sOut, sTemp;
    int n = 1;
    std::stack<std::string*> Stack;
    std::string* s = new std::string("(");
    Stack.push(s);
    m_Formula += ")";
    while (true) {
        sTemp = GetNextElement(m_Formula, n);
        if (sTemp == "") {
            break;
        }
        while (sTemp != "") {
            switch (ProcFormula::ComparePrioritet(sTemp, Stack)) {
            case 1:
                s = new std::string(sTemp);
                Stack.push(s);
                sTemp = "";
                break;
            case 2:
                s = Stack.top();
                Stack.pop();
                delete s;
                sTemp = "";
                break;
            case 3:
                s = Stack.top();
                Stack.pop();
                sOut += *s;
                delete s;
                break;
            }
        }
    }
    while (!Stack.empty()) {
        s = Stack.top();
        Stack.pop();
        delete s;
    }
    return sOut;
}

double ProcFormula::GetValueElement(int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate){
    TQueryUni *qValue = new TQueryUni(DefDBconnect.Conn);
    qValue->SQL->Add("SELECT value FROM Cell_value");
    qValue->SQL->Add("WHERE cell_id = ?");
    qValue->SQL->Add("AND plan_id = ?");
    qValue->SQL->Add("AND vid_otc = ?");
    qValue->SQL->Add("AND value_date = ?");
    qValue->ParamByName("CellId")->AsInteger() = aCell_id;
    qValue->ParamByName("PlanId")->AsInteger() = aPlan_id;
    qValue->ParamByName("VidOtc")->AsInteger() = aVid_otc;
    qValue->ParamByName("Date")->AsDateTime() = aDate;
    qValue->Open();
    double CellValue = 0.0;
    if (!qValue->EoF()) {
        CellValue = qValue->FieldByName("value")->AsFloat();
    }
    return CellValue;
    delete qValue;
}

double ProcFormula::CalcPolishRec(std::string m_PolRec, int m_Plan_id, int m_Vid_otc, TDateTime m_Date) {
    std::stack<double*> Stack;
    double result = 0.0;
    if (m_PolRec.empty()) return result;
    int n = 1;
    while (true) {
        std::string sElem = GetNextElement(m_PolRec, n);
        if (sElem.empty()) {
            double* tempV = Stack.top();
            Stack.pop();
            result = *tempV;
            delete tempV;
            break;
        }
        if (sElem[0] == 'V') {
            sElem.erase(0, 1);
            double* CalcRes = new double;
            *CalcRes = GetValueElement(std::stoi(sElem), m_Plan_id, m_Vid_otc, m_Date);
            Stack.push(CalcRes);
        }
        else if (sElem[0] == 'C') {
            sElem.erase(0, 1);
            double* CalcRes = new double;
            *CalcRes = std::stod(sElem);
            Stack.push(CalcRes);
        }
        else {
            double* tempV = Stack.top();
            Stack.pop();
            double* tempV1 = Stack.top();
            Stack.pop();
            double* CalcRes = new double;
            switch (sElem[0]) {
            case '+':
                *CalcRes = *tempV1 + *tempV;
                break;
            case '-':
                *CalcRes = *tempV1 - *tempV;
                break;
            case '*':
                *CalcRes = *tempV1 * *tempV;
                break;
            case '/':
                if (*tempV == 0) {
                    *CalcRes = 0.0;
                }
                else {
                    *CalcRes = *tempV1 / *tempV;
                }
                break;
            }
            if (!(*CalcRes > 100000000000000)) {
                try {
                    *CalcRes = std::round(*CalcRes * 100000) / 100000;
                }
                catch (...) {
                    // Do nothing
                }
            }
            Stack.push(CalcRes);
            delete tempV;
            delete tempV1;
        }
    }
    return result;
}

double ProcFormula::CalcPolishRec(std::string m_PolRec, int nKv, int nMonth, int m_Plan_id, int m_Vid_otc, int m_Year) {
    double result = 0.0;
    if (m_PolRec.empty()) return result;
    std::stack<double*> Stack;
    int n = 1;
    while (true) {
        std::string sElem = GetNextElement(m_PolRec, n);
        if (sElem.empty()) {
            double* tempV = Stack.top();
            Stack.pop();
            result = *tempV;
            delete tempV;
            break;
        }
        if (sElem[0] == 'V') {
            sElem.erase(0, 1);
            double* CalcRes = new double;
            *CalcRes = GetValueElement(StrToInt(sElem), nKv, nMonth, m_Plan_id, m_Vid_otc, m_Year);
            Stack.push(CalcRes);
        }
        else if (sElem[0] == 'C') {
            sElem.erase(0, 1);
            double* CalcRes = new double;
            *CalcRes = std::stod(sElem);
            Stack.push(CalcRes);
        }
        else {
            double* tempV = Stack.top();
            Stack.pop();
            double* tempV1 = Stack.top();
            Stack.pop();
            double* CalcRes = new double;
            switch (sElem[0]) {
            case '+':
                *CalcRes = *tempV1 + *tempV;
                break;
            case '-':
                *CalcRes = *tempV1 - *tempV;
                break;
            case '*':
                *CalcRes = *tempV1 * *tempV;
                break;
            case '/':
                if (*tempV == 0) {
                    *CalcRes = 0.0;
                }
                else {
                    *CalcRes = *tempV1 / *tempV;
                }
                break;
            }
            *CalcRes = std::round(*CalcRes * 100000) / 100000;
            Stack.push(CalcRes);
            delete tempV;
            delete tempV1;
        }
    }
    return result;
}

double ProcFormula::GetValueElement(int aCell_id, int nKv, int nMonth, int m_Plan_id, int m_Vid_otc, int m_Year) {
    std::string DatabaseName = "EcDB";
    std::string SQL = "SELECT value FROM Cell_value WHERE cell_id = :CellId AND plan_id = :PlanId AND vid_otc = :VidOtc AND value_date = :Date";
    double CellValue = 0.0;

    TDateTime dtDate = EncodeDate(m_Year, 1, 31);
    int Month = nMonth - 1;
    if (nMonth == 0) {
        if (nKv == 0) {
            Month = 11;
        } else {
            Month = nKv * 3 - 1;
        }
    }

    if (Month > 0) {
        dtDate = IncMonth(dtDate, Month);
    }

    std::string CellId = std::to_string(aCell_id);
    std::string PlanId = std::to_string(m_Plan_id);
    std::string VidOtc = std::to_string(m_Vid_otc);
    std::string DateStr = DateTimeToStr(dtDate);

    TQueryUni *qValue = new TQueryUni(DefDBconnect.Conn);
    qValue->SQL->Add(SQL);
    qValue->ParamByName("CellId")->AsInteger() = aCell_id;
    qValue->ParamByName("PlanId")->AsInteger() = m_Plan_id;
    qValue->ParamByName("VidOtc")->AsInteger() = m_Vid_otc;
    qValue->ParamByName("Date")->AsDateTime() = dtDate;
    qValue->Open();
    if (!qValue->EoF()) {
        CellValue = qValue->FieldByName("value")->AsFloat();
    }

    return CellValue;
}

double ProcFormula::CalcSumPoKodAU(int nCellOut, std::string strFormula, int m_Plan_id, int m_Vid_otc, TDateTime m_Date) {
    std::string kodAU = strFormula.substr(1, strFormula.find('_') - 2);
    std::string strValue = strFormula;
    strValue.erase(0, strFormula.find('_') + 1);
    int kodIF = std::stoi(strValue.substr(0, strValue.find('_')));
    int kodPeriod = std::stoi(std::string(1, strValue.back()));

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("SELECT SUM(POWER(10,coef)*value) AS sumPoAu");
    q->SQL->Add("FROM Pokaztl, Cell, Cell_value");
    q->SQL->Add("WHERE Pokaztl.pokaz_code = :KodAU");
    q->SQL->Add("AND Cell.pokaz_id = Pokaztl.pokaz_id");
    q->SQL->Add("AND Cell.cell_id <> :CellId");
    q->SQL->Add("AND Cell.cell_period = :kodPeriod");
    q->SQL->Add("AND Cell.ist_fin_id = :kodIF");
    q->SQL->Add("AND Cell.cell_din_izm = :DinIzm");
    q->SQL->Add("AND Cell_value.cell_id = Cell.cell_id");
    q->SQL->Add("AND Cell_value.plan_id = :PlanId");
    q->SQL->Add("AND Cell_value.vid_otc = :VidOtc");
    q->SQL->Add("AND Cell_value.value_date = :Date");
    q->ParamByName("KodAU")->AsString()= kodAU;
    q->ParamByName("CellId")->AsInteger()= nCellOut;
    q->ParamByName("kodPeriod")->AsInteger()= kodPeriod;
    q->ParamByName("kodIF")->AsInteger()= kodIF;
    q->ParamByName("DinIzm")->AsString() = "";
    q->ParamByName("PlanId")->AsInteger()= m_Plan_id;
    q->ParamByName("VidOtc")->AsInteger() = m_Vid_otc;
    q->ParamByName("Date")->AsDateTime()= m_Date;
    q->Open();
    if (q->EoF()) {
        return 0.0;
    } else {
        return q->FieldByName("sumPoAu").AsFloat();
    }
    delete q;
}






















