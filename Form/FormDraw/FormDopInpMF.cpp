#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "DB_ClassofPart.h"
#include "FormDopInpMF.h"

std::string TFormDopInpMF::GetNextFormula(std::string aSS, int& aNCol, std::string& aForm) {
    std::string Result = aSS;
    if (aSS.empty()) {
        return Result;
    }
    if (aSS[0] != '[') {
        throw std::runtime_error("Список формул для свода задан неверно");
    }
    size_t pos1 = aSS.find(']');
    if (pos1 == std::string::npos) {
        throw std::runtime_error("Список формул для свода задан неверно");
    }
    std::string w = aSS.substr(1, pos1 - 2);
    if (w.empty()) {
        return Result;
    }
    try {
        aNCol = std::stoi(w);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Список формул для свода задан неверно");
    }
    Result = aSS.substr(pos1 + 1);
    size_t pos2 = Result.find('[');
    if (pos2 == std::string::npos) {
        aForm = Result;
        Result.clear();
    } else {
        aForm = Result.substr(0, pos2);
        Result = Result.substr(pos2);
    }
    return Result;
}

std::string TFormDopInpMF::GetSubFormulaByN(const std::string aSS, int aN) {
    std::string Result;
    if (aSS.empty()) {
        return Result;
    }
    if (aSS[0] != '[') {
        return Result;
    }
    std::string w = "[" + std::to_string(aN) + "]";
    size_t pos1 = aSS.find(w);
    if (pos1 == std::string::npos) {
        return Result;
    }
    pos1 += w.length();
    for (size_t i = pos1; i < aSS.length(); ++i) {
        if (aSS[i] == '[') {
            return Result;
        }
        Result += aSS[i];
    }
    return Result;
}

int TFormDopInpMF::GetField_id(int class_id, std::string note2) {
    int result = 0;
    if (class_id == 0) {
        return result;
    }
    std::stringstream id;
    id << class_id;
    int n = note2.find(id.str());
    if (n != std::string::npos) {
        std::string s = note2.substr(n + id.str().length() + 1, 15);
        n = s.find(";");
        if (n != std::string::npos) {
            s = s.substr(0, n);
        }
        result = std::stoi(s);
    }
    return result;
}

int TFormDopInpMF::getIdFromNote2(std::string note2) {
    int result = 0;
    int n = note2.find('.');
    if (n > 0) {
        std::string s = note2.substr(n + 1, 1000);
        n = s.find(';');
        if (n > 0) {
            s = s.substr(0, n);
        }
        result = std::stoi(s);
    }
    return result;
}

std::string TFormDopInpMF::GetFormulaForCol(std::string aSS, int aNCol) {
    std::string Result = "";
    if (aSS == "") return Result;
    std::string w = "[" + std::to_string(aNCol) + "]";
    int pos1 = aSS.find(w);
    if (pos1 == std::string::npos) return Result;
    int pos3 = pos1 + w.length();
    int pos2 = aSS.find("[", pos3);
    int L = (pos2 == std::string::npos) ? 10000 : pos2 - pos3;
    Result = aSS.substr(pos3, L);
    Result = "^" + Result;
    return Result;
}

int TFormDopInpMF::GetPokazIdForCol(std::string aSS, int aNCol) {
    int Result = 0;
    std::string s = TFormDopInpMF::GetFormulaForCol(aSS, aNCol);
    if (s.empty()) {
        return Result;
    }
    std::replace(s.begin(), s.end(), '^', ' ');
    char* endptr;
    int pokaz_id = static_cast<int>(std::strtol(s.c_str(), &endptr, 10));
    if (endptr == s.c_str()) {
        // No valid conversion could be performed
        return Result;
    }
    Result = pokaz_id;
    return Result;
}

int TFormDopInpMF::GetPokazIdForCol2(std::string aSS, int aNCol, bool& isSimvR) {
    int result = 0;
    isSimvR = false;
    std::string s = GetFormulaForCol(aSS, aNCol);
    if (s.empty()) {
        return result;
    }
    if (s[s.length() - 1] == 'r') {
        isSimvR = true;
    }
    s = AnsiReplaceStr(s, "r", "");
    s = AnsiReplaceStr(s, "^", " ");
    int pokaz_id;
    int code;
    Val(s,pokaz_id,code);
    return pokaz_id;
}

std::string TFormDopInpMF::ConvertColToSimvs(int aCol) {
    std::string s1 = "";
    if (aCol > 26) {
        aCol = aCol - 26;
        s1 = "A";
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "B";
        }
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "C";
        }
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "D";
        }
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "E";
        }
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "F";
        }
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "G";
        }
        if (aCol > 26) {
            aCol = aCol - 26;
            s1 = "H";
        }
    }
    s1 += static_cast<char>('A' + aCol - 1);
    return s1;
}





