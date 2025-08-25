#include "SystFuncsLogic.h"
#include "../DB/DB_bkMetaDef.h"
#include "../Form/FormTypes.h"
#include "../system/ClassesUtils.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../DB/unDBType.h"

#include <cmath>
#include <string>

double PreobrValue(double Value, int DataType, int coef1, int coef2) {
    double Result;
    switch (DataType) {
        case 2:
        case 3:
        case 4:
        case 12+deltaDType:
        case 15+deltaDType:
            Result = Value;
            break;
        default:
            Result = (Value * pow(10, coef1)) / pow(10, coef2);
            break;
    }
    return Result;
}

// ������ ������ ���� ����� �������
double ClearNolDbl(double valD) {
    return StrToFloat(ClearNolStr(FloatToStr(valD)));
}

// ������ ������ ���� ����� �������
std::string ClearNolStr(std::string valS) {
    std::string Result = valS;
    int n = valS.length();
    for (int i = n; i >= 1; i--) {
        if (valS[i-1] == ',') {
            if ((n - i) > 3) {
                std::string S = valS.substr(0, i + 3);
                if (valS[i+3] >= '5') {
                    double d = std::stod(S);
                    if (d > 0) d += 0.001;
                    if (d < 0) d -= 0.001;
                    S = std::to_string(d);
                }
                Result = S;
            }
            break;
        }
    }
    return Result;
}

int PixelsToTwips(int pixels) {
    return round(pixels * 15.33);
}

int TwipsToPixel(int twips) {
    return round(twips / 15.33333);
}

// ������ ������ �� ����� ��
std::string DelSkobka(std::string w) {
    std::string Result = w;
    int n = Pos("(", w);
    if (n >= 0) Result = w.substr(0, n-1);
    Result = Trim(Result);
    return Result;
}

// �������� ����������� � ��� ��
std::string SetCodeBK_rzdltl(std::string pokaz_bk_code, int God) {
    std::string Result = "";
    int L = pokaz_bk_code.length();
    if (L == 0) return Result;
    if (L <= 4) {
        Result = pokaz_bk_code;
    } else if (L <= TDB_bkMetaDef::GetDl_R_CS(God)) {
        Result = TDB_bkMetaDef::GetRasdel(pokaz_bk_code, God) + " " +
                 TDB_bkMetaDef::GetCelSt(pokaz_bk_code, God);
    } else if (L <= TDB_bkMetaDef::GetDl_R_CS_VR(God)) {
        Result = TDB_bkMetaDef::GetRasdel(pokaz_bk_code, God) + " " +
                 TDB_bkMetaDef::GetCelSt(pokaz_bk_code, God) + " " +
                 TDB_bkMetaDef::GetVidRasx(pokaz_bk_code, God);
    } else if (L <= TDB_bkMetaDef::GetDl_All(God)) {
        Result = TDB_bkMetaDef::GetRasdel(pokaz_bk_code, God) + " " +
                 TDB_bkMetaDef::GetCelSt(pokaz_bk_code, God) + " " +
                 TDB_bkMetaDef::GetVidRasx(pokaz_bk_code, God) + " " +
                 TDB_bkMetaDef::GetKOSGU(pokaz_bk_code, God);
    } else {
        Result = pokaz_bk_code;
    }
    return Result;
}

// �������� ����������� � ��� �� - ������/���������
std::string SetCodeBK_RZD_rzdltl(std::string pokaz_bk_code, int Modifikator) {
    std::string ss1, ss2, Result;
    if (pokaz_bk_code.length() < 4) return pokaz_bk_code;
    ss1 = pokaz_bk_code.substr(0, 2);
    ss2 = pokaz_bk_code.substr(2, 2);
    switch (Modifikator) {
        case 0:
            Result = ss1 + " " + ss2;
            break;
        case 1:
            Result = ss1;
            break;
        case 2:
            Result = ss2;
            break;
        case 3:
            if (ss2 == "00") {
                Result = "";
            } else {
                Result = ss2;
            }
            break;
    }
    return Result;
}

// �������� ����������� � ��� �� - ������� ������
std::string SetCodeBK_CST_rzdltl(std::string pokaz_bk_code, int god, int Modifikator) {
    std::string ss1, ss2, ss3, ss4, Result;
    if (pokaz_bk_code.length() < 7) return pokaz_bk_code;
    if (god <= 2015) return pokaz_bk_code.substr(0, 2) + " " + pokaz_bk_code.substr(2, 1) + " " + pokaz_bk_code.substr(3, 4);
    if (pokaz_bk_code.length() < 10) return pokaz_bk_code;
    ss1 = pokaz_bk_code.substr(0, 2);
    ss2 = pokaz_bk_code.substr(2, 1);
    ss3 = pokaz_bk_code.substr(3, 2);
    ss4 = pokaz_bk_code.substr(5, 5);
    switch (Modifikator) {
        case 0:
            Result = ss1 + " " + ss2 + " " + ss3 + " " + ss4;
            break;
        case 1:
            Result = ss1;
            break;
        case 2:
            Result = ss2;
            break;
        case 3:
            if (ss2 == "0") {
                Result = "";
            } else {
                Result = ss2;
            }
            break;
        case 4:
            Result = ss3;
            break;
        case 5:
            if (ss3 == "00") {
                Result = "";
            } else {
                Result = ss3;
            }
            break;
        case 6:
            Result = ss1 + " " + ss2 + " " + ss3;
            break;
        case 7:
            Result = ss4;
            break;
    }
    return Result;
}

// ���������� �������� ���� �� ����, �������� � ������
// ������ �� ����������: ���(��������), �������(1..4) � �����(1..12).
// ���� ����� = 0 � aKv = 0, �� �������� ��������� ���� ���������� ���� (31 �������)
// ���� ����� = 0, � ������� ����� ��������, �� �������� ��������� ���� ���������� ��������
// ���� ����� ����� ��������, �� �������� ��������� ���� ���������� ������
//TDateTime GetLastDayDate(int aYear, int aKv, int aMes) {
//    TDateTime aDate = EncodeDate(aYear, 1, 31);
//    int Month = aMes-1;
//    if (aMes == 0) {
//        if (aKv == 0) {
//            Month = 11;
//        } else {
//            Month = aKv*3 - 1;
//        }
//    }
//    if (Month > 0) {
//        aDate = IncMonth(aDate, Month);
//    }
//    return aDate;
//}

// ���������� ���� �������� ������� - ��� ���������� � ������ ��������
void SetKeyObject(std::string aKey_value) {
    GlobalVars->key_value = aKey_value;
}

// ���������� ���� �������� ������� - ��� ���������� � ������ ��������
std::string GetKeyObject() {
    return GlobalVars->key_value;
}

// ������� ����� �������� �� ������
//int GetNKv(int Month) {
//    int Result;
//    switch (Month) {
//        case 1:
//        case 2:
//        case 3:
//            Result = 1;
//            break;
//        case 4:
//        case 5:
//        case 6:
//            Result = 2;
//            break;
//        case 7:
//        case 8:
//        case 9:
//            Result = 3;
//            break;
//        case 10:
//        case 11:
//        case 12:
//            Result = 4;
//            break;
//    }
//    return Result;
//}

// ������� ����� �������� �� ����
int GetNKvbyDate(TDateTime aDate) {
    int Year, Month, Day;
    DecodeDate(aDate, Year, Month, Day);
    return GetNKv(Month);
}

// ������� ������� ������
std::string getKBK_CS(std::string pokaz_bk_code, int God) {
    if (pokaz_bk_code.length() < 11) return pokaz_bk_code;
    return TDB_bkMetaDef::GetCelSt(pokaz_bk_code, God);
}

std::string getKBK_RZD(std::string pokaz_bk_code, int God) {
    if (pokaz_bk_code.length() < 4) return pokaz_bk_code;
    return TDB_bkMetaDef::GetRasdel(pokaz_bk_code, God);
}

std::string getKBK_VR(std::string pokaz_bk_code, int God) {
    if (pokaz_bk_code.length() < 14) return pokaz_bk_code;
    return TDB_bkMetaDef::GetVidRasx(pokaz_bk_code, God);
}

//���������� ������� Login
void SetUserLogin(std::string aUserName) {
    GlobalVars->UserName = aUserName;
}

//���������� ������� Login
std::string GetUserLogin() {
    return GlobalVars->UserName;
}

//���������� �������� ���������� UserId
void SetUserId(int aUserId) {
    GlobalVars->UserId = aUserId;
}

//���������� �������� ���������� UserId
int GetUserId() {
    return GlobalVars->UserId;
}

//������ ��� ���� ������
void SetDBType(TDBType aDBtype) {
    GlobalVars->DBType = aDBtype;
}

//������� ��� ���� ������
TDBType GetDBType() {
    if (GlobalVars == nullptr) {
        return TDBType::Postgre; // Или другое значение, если Postgre является константой
    }
    return GlobalVars->DBType;
}

// � �������� ���� TDatetime ����� ����� ���������� � ��
TDateTime ConvertToDateBgn(TDateTime dt) {
    int wYear, wMonth, wDay;
    DecodeDate(dt, wYear, wMonth, wDay);
    return EncodeDateTime(wYear, wMonth, wDay, 0, 0, 0, 0);
}

// � �������� ���� TDatetime ����� ����� ���������� � 59
TDateTime ConvertToDateEnd(TDateTime dt) {
    int wYear, wMonth, wDay;
    DecodeDate(dt, wYear, wMonth, wDay);
    return EncodeDateTime(wYear, wMonth, wDay, 23, 59, 59, 999);
}

// idsClassFields - ����� ��� ClassId.FieldId; � �.�.
// �������� ����
int getFieldId(int Class_id, std::string idsClassFields) {
    std::string strIdOwnerClass = std::to_string(Class_id) + ".";
    int pos1 = idsClassFields.find(strIdOwnerClass);
    if (pos1 == std::string::npos) return 0;
    pos1 += strIdOwnerClass.length();
    int pos2 = idsClassFields.find(';', pos1);
    int L;
    if (pos2 == std::string::npos) {
        L = idsClassFields.length() - pos1 + 1;
    } else {
        L = pos2 - pos1;
    }
    int FieldId = std::stoi(idsClassFields.substr(pos1, L));
    return FieldId;
}

// idsClassFields - ����� ��� ClassId.FieldId; � �.�.
// �������� ����, �� � ������� �� ���������� ������� id ������ �� �����
int getFieldId2(int& Offset, std::string idsClassFields) {
    std::string strIdOwnerClass = ".";
    int pos1 = idsClassFields.find(strIdOwnerClass, Offset);
    if (pos1 == std::string::npos) return 0;
    pos1 += strIdOwnerClass.length();
    int pos2 = idsClassFields.find(';', pos1);
    int L;
    if (pos2 == std::string::npos) {
        L = idsClassFields.length() - pos1 + 1;
        Offset = idsClassFields.length() + 1;
    } else {
        L = pos2 - pos1;
        Offset = pos2 + 1;
    }
    int FieldId = std::stoi(idsClassFields.substr(pos1, L));
    return FieldId;
}

// ������� ���� �� ���������� �������� Windows
std::string c_GetTempPath() {
//???    return System.IOUtils.TPath.GetTempPath();
    return "";
}

// ���������� �������, ����������� �� ���������� � ����� ������ ����������
void SetisDocumConected(bool isDocumConected) {
    GlobalVars->isDocumConected = isDocumConected;
}

// ������� �������, ����������� �� ���������� � ����� ������ ����������
bool GetisDocumConected() {
    return GlobalVars->isDocumConected;
}

// ������� �����
std::string getKBK_KOSGU(std::string pokaz_bk_code, int god) {
    if (pokaz_bk_code.length() < 17) return pokaz_bk_code;
    return TDB_bkMetaDef::GetKOSGU(pokaz_bk_code, god);
}

// ������� �� ������ ����� �������
int LetterToColumn(std::string ALetter) {
    std::string Letter = ALetter;
    std::transform(Letter.begin(), Letter.end(), Letter.begin(), ::toupper);
    int sum = 0;
    for (int i = 0; i < Letter.length(); i++) {
        sum = sum * 26;
        sum = sum + (Letter[i] - 'A' + 1);
    }
    return sum;
}

// ������� �� ������ ����� ������
int LetterToRow(std::string ALetter) {
    std::string Letter = ALetter;
    std::string ws = "";
    for (int i = 0; i < Letter.length(); i++) {
        if (Letter[i] >= '0' && Letter[i] <= '9') {
            ws += Letter[i];
        }
    }
    if (ws == "") return 0;
    return std::stoi(ws);
}

// ������� �� �������� ������� ����������, ���� �� � ��� ��������.
// ����������: true-� ������� ���� ��������, false-� ������� ������������ �������
bool IsOpInFormula(std::string aFormula) {
    for (int i = 0; i < aFormula.length(); i++) {
        if (aFormula[i] != 'V' && (aFormula[i] < '0' || aFormula[i] > '9')) {
            return true;
        }
    }
    return false;
}

// ������� �� ����� ����������� ������� F1
std::string getOboznColumn(int aCol) {
    std::string s1 = "";
    if (aCol > 26) {
        aCol -= 26;
        s1 = "A";
        if (aCol > 26) {
            aCol -= 26;
            s1 = "B";
        }
        if (aCol > 26) {
            aCol -= 26;
            s1 = "C";
        }
        if (aCol > 26) {
            aCol -= 26;
            s1 = "D";
        }
        if (aCol > 26) {
            aCol -= 26;
            s1 = "E";
        }
        if (aCol > 26) {
            aCol -= 26;
            s1 = "F";
        }
        if (aCol > 26) {
            aCol -= 26;
            s1 = "G";
        }
        if (aCol > 26) {
            aCol -= 26;
            s1 = "H";
        }
    }
    s1 += (char)('A' + aCol - 1);
    return s1;
}

// ������� �� ����� ����������� ������� F1 � ���� ���� � (�����)
std::string getOboznColumn2(int aCol) {
    return getOboznColumn(aCol) + "(" + std::to_string(aCol) + ")";
}


