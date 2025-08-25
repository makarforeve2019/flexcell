#ifndef Form1H
#define Form1H
//#include "unFormLogicF1.h"
//#include "unF1toDE.h"
//#include "FormDopInp.h"
//#include "unF1FlexCel.h"
//#include "FormTypes.h"
//#include "DB_Common.h"
//#include "Form2.h"
//#include "Form2Plus.h"
//#include "unTF1CellFormat.h"

#include "unFormLogicF1.h"
#include "unTF1CellFormat.h"

class TForm1 {
public:
    static bool FormInit(TInfoFormLogic* InfoFormLogic, TFormLogicF1* FormLogicF1, unsigned char IsParamForm, int ParamObjectId,
                         std::string ParamObjectName, int FormId, int FormOrder, TQueryUni* quForms,
                         std::string& ErrorMsg1, std::string& ErrorMsg2, TwwStringList* p);
    static void InitFormHeader(TFormLogicF1* FormLogicF1, std::string ToStmnt, int initRow, unsigned char IsParamForm,
                               std::string ParamObjectName, TwwStringList* p) ;
    static void SetCellValue(TFormLogicF1* FormLogicF1, TF1FlexCel* aF1Book,
                             int aMaxHrCol, int aRow1, int aCol1, int aRow2, int aCol2, std::string Value,
                             short DataType, PFormatStyle FormatStyle);
    static void SetCellValue_Obnovl(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string Value, int aDataType, int nRow, int nCol);
    static void DateFormatir(TwwStringList* p, std::string& Value, int nRow, int nCol);
    static void SetBorderFilling(TF1FlexCel* aF1Book, int aRow, int aCol, F1BorderStyleConstants TopBorder, F1BorderStyleConstants LeftBorder, 
                                 F1BorderStyleConstants BottomBorder, F1BorderStyleConstants RightBorder, int TopColor, int LeftColor, 
                                 int BottomColor, int RightColor, int BGColor, int FGColor, int PatternStyle){};
};
#endif