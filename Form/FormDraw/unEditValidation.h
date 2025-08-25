
#ifndef UNEDITVALIDATION_H
#define UNEDITVALIDATION_H

#include <string>
#include <vector>
#include <unordered_map>
//class TFormLogicF1;

class TValidator {
private:
    short nValidator;
    short vid_uslovij;
    double Value1;
    double Value2;
    std::string Shablon;
    std::string Msg;

public:
    TValidator(TwwStringList& FormAttributes, int ACol);
    TValidator(short nValidator_, short vid_uslovij_, double Value1_, double Value2_, 
               std::string Shablon_, std::string Msg_);
    bool doProvCisla(double d, std::string& MessageText, TFormLogicF1* FormLogicF1, int nRow, int nCol);
    bool doProvStr(std::string str_value, std::string& MessageText);
    bool checkShablon(int ARow, int ACol, std::string AEditString, TFormLogicF1* FormLogicF1,
                      std::string& vMsg, bool& Cancel);
};

class TEditValidation {
private:
    TFormLogicF1* FormLogicF1;
    int Sheet;
    int Row;
    int Col;
    std::string EditString;
    double EditFloatValue;
    PCellValueAtr CellAttributes;
    TwwStringList FormAttributes;
    std::string MessageText;

protected:
    bool isGraphicFinans();
    std::string ShieldEmptyValue(const std::string& AValue);
    bool GetValidatedFloatValue(double& FloatValue);
    bool CheckRevisedPlan();
    bool CheckGraphicFinansValue();
    bool CheckCashPlan();
    bool CheckDaleeValue();
    bool CheckSpecificNumberValue(TValidator AValidator);
    bool CheckSpecificTextValue(TValidator AValidator);

public:
    TEditValidation();
    void Init(int ASheet, int ARow, int ACol, std::string AEditString, TFormLogicF1* AFormLogicF1);
    bool CheckValue();
    static bool doEditValidation(int ASheet, int ARow, int ACol, std::string& AEditString,
                                 TFormLogicF1* FormLogicF1, std::string& aMsg, int& nNextRow, int& nNextCol);
    bool getActivateNextCell(int& nRow, int& nCol);
    bool FindNextAvailCell(int& nRow, int& nCol);
    bool FindNextCell(int& nRow, int& nCol);
    bool CellLocked(int nRow, int nCol);
    bool CellVisible(int nRow, int nCol);
    bool RowExist(int nRow);
    bool ColExist(int nCol);
};

#endif // UNEDITVALIDATION_H

