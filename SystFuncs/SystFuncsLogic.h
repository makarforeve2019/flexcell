
#ifndef SYSTFUNCSLOGIC_H
#define SYSTFUNCSLOGIC_H

#include <string>
#include "../DB/unDBType.h"
#include "../Form/FormTypes.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../system/ClassesUtils.h"


const std::string FontTimesNewRoman = "Times New Roman";
const std::string FontArial = "Arial";
const std::string PIV_ASU_GF = "ПИВ АСУ ГФ";

double PreobrValue(double Value, int DataType, int coef1, int coef2);
double ClearNolDbl(double valD);
std::string ClearNolStr(std::string valS);
int PixelsToTwips(int pixels);
int TwipsToPixel(int twips);
std::string DelSkobka(std::string w);
std::string SetCodeBK_rzdltl(std::string pokaz_bk_code, int god);
std::string SetCodeBK_RZD_rzdltl(std::string pokaz_bk_code, int Modifikator);
std::string SetCodeBK_CST_rzdltl(std::string pokaz_bk_code, int god, int Modifikator);
TDateTime GetLastDayDate(int aYear, int aKv, int aMes);
std::string GetKeyObject();
void SetKeyObject(std::string aKey_value);
int GetNKv(int Month);
int GetNKvbyDate(TDateTime aDate);
std::string getKBK_CS(std::string pokaz_bk_code, int God);
std::string getKBK_RZD(std::string pokaz_bk_code, int God);
std::string getKBK_VR(std::string pokaz_bk_code, int God);
std::string GetUserLogin();
void SetUserLogin(std::string aUserName);
int GetUserId();
void SetUserId(int aUserId);
TDBType GetDBType();
void SetDBType(TDBType aDBtype);
TDateTime ConvertToDateBgn(TDateTime dt);
TDateTime ConvertToDateEnd(TDateTime dt);
int getFieldId(int Class_id, std::string idsClassFields);
int getFieldId2(int& Offset, std::string idsClassFields);
std::string c_GetTempPath();
void SetisDocumConected(bool isDocumConected);
bool GetisDocumConected();
std::string getKBK_KOSGU(std::string pokaz_bk_code, int god);
int LetterToColumn(std::string ALetter);
int LetterToRow(std::string ALetter);
bool IsOpInFormula(std::string aFormula);
std::string getOboznColumn(int aCol);
std::string getOboznColumn2(int aCol);

#endif // SYSTFUNCSLOGIC_H

