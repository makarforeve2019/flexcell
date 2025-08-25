#ifndef FormDopInpH
#define FormDopInpH


#include <iostream>
#include <vector>
#include "../unMWSPageSetup/TMWSPageSetup.h"
#include "unFormLogicF1.h"
#include "../FormTypes.h"
#include "../DB/DB_Common.h"
#include "FormDopInpMF.h"
#include "../unDopAtrCol.h"

const std::string fNameC = "Arial";
// const int fSizeC = 200;
const int fSizeC = 10;



class TFormDopInp {
public:
    static std::string fName_;
    static int fSize_;
     // ========== ������� ��� ������������ ������ TValueData ==============
    // ��������� ������� � ������ TValueCellAtr � ������������� Protection ������ �����
    static PCellValueAtr AddValueCellAtr(TFormLogicF1 *FormLogicF1, int CurrRow, int CurrCol, int CellId, TDateTime ValueDate,
                                         int DataType, int VidOtch, int PlanId, int coef_Pokaz, int coef_Forma, int ObjectId,
                                         int PokazId, int cell_is_eval, int pokaz_type, int cell_period, int ist_fin_id);
    // ��������� ������� � ������ TValueCellAtr � ������������� Protection ������ ����� - ������ �������������
    static PCellValueAtr AddValueCellAtrComNP(TFormLogicF1 *FormLogicF1, int CurrRow, int CurrCol, int comnp_id, short god,
                                              int kvrtl, int mesjc, int den, int DataType, short VidOtch, int PlanId,
                                              int coef_Pokaz, int coef_Forma, int ObjectId, int PokazId, int cell_is_eval,
                                              int pokaz_type, int vid_period, std::string toTblName);
    // �������� ������� ������ TCellValueAtr
    static void CopyCellValueAtr(TCellValueAtr& ValueData, PCellValueAtr vd);
    // ���������� ���������� ���������
    static int GetCellValueAtrCount(TFormLogicF1* FormLogicF1);
    // ���������� ������ �� ������� ������ TCellValueAtr �� �������
    static PCellValueAtr GetPCellValueAtrByIndex(TFormLogicF1* FormLogicF1, int Index);
    // ���������� ������� ������ TCellValueAtr �� ����� - ��� ���� � ����
    static bool GetCellValueAtrByName(TFormLogicF1* FormLogicF1, std::string CellName, TCellValueAtr& ValueData);
    // ���������� ������� ������ TCellValueAtr �� ������ � �������
    static PCellValueAtr GetCellValueAtrByRC(TFormLogicF1* FormLogicF1, int Row, int Col);
    static int GetICellValueAtrByRC(TFormLogicF1* FormLogicF1, int Row, int Col);
    
    static PCellValueAtr GetCellValueAtrByRC_p(TwwStringList* p, int Row, int Col);
    // ������� ������� ������ TCellValueAtr �� ������ � �������
    static void DelCellValueAtrByRC(TFormLogicF1* FormLogicF1, int Row, int Col);
    // ���������� ������ �� ������
    static int GetObjectByRow(TFormLogicF1* FormLogicF1, int Row);
    // ========== ������� ��� ������� ��������� ==============
    // ���������� ������������ ������� ��������� � ������� ��� ��� � ����� ���� 0
    static std::string GetSSedIzmName(TFormLogicF1* FormLogicF1, int edizm_id);
    // ========== ������� ��� ������������ ������ ListRows ==============
    // ������� ������ ListRows
    static void CreateListRows(TFormLogicF1* FormLogicF1);
    // ������������ ������� AtrRow � ������ ListRows
    static void ListRowsSetAtrRow(TFormLogicF1* FormLogicF1, int Row, bool IsTerminal, int OwnerNomRow, int Ownerid, int ObjectId);
    // �������� ������� ������ ListRows
    static void CopyListRowsItem(TFormLogicF1* FormLogicF1, PAtrRow AtrRow, int DblRow);
    // ������� ������� �� ������ ListRows
    static void DelListRowsItem(TFormLogicF1* FormLogicF1, int Row);
    // ���������� ������� �� ������ ListRows
    static PAtrRow GetListRowsItem(TFormLogicF1* FormLogicF1, int Row);
    // �������������, ��� ������� ������ ������������-��� ����������� �����������
    static void SetToTerminal(TFormLogicF1* FormLogicF1, int Row);
    // ������������� � ������� ������ ����� ������ ��������� ������� ��� ������������� ����
    static void SetToStrIdTEMP(TFormLogicF1* FormLogicF1, int Row, int str_id);
    // �������������, ��� ������� ������ � ����������
    static void SetInSort(TFormLogicF1* FormLogicF1, int Row);
    // ������������� �������� ��������� ������
    static void SetListRowsItemAtr(TFormLogicF1* FormLogicF1, int Row, int CurRowVid, int level_sign, int pokaz_vert, int Ownerid, int ObjectId, int objekt_class_id, std::string pokaz_bk_code, int aNomUrovRazdela = 0);
    // ������������� ��������� is_filter2, ColOnFilter2, is_filter ��� ������� ������
    static void SetFilter2OnRow(TFormLogicF1 *FormLogicF1, int Row, int is_filter2, int ColOnFilter2, int is_filter);
    // ������������� ��������� tocnost ��� ������� ������
    static void SetTocnostOnRow(TFormLogicF1* FormLogicF1, int Row, int tocnost);
    // ������������� �������������� �������� �� DopAtrCol � �������� ������� ������
    static void SetDopAtrToAtrRow(TFormLogicF1* FormLogicF1, int Row, PDopAtrCol aDopAtrCol);
    // ������������� �������������� �������� � ������ ����� �� ����� 1:N
    static void SetDopAtrToAtrRow1n(TFormLogicF1* FormLogicF1, int Row, PDopAtrCol aDopAtrCol);
    // ������������� �������� �������������� ��� ��������� ������
    static void SetFormatStyle(TFormLogicF1* FormLogicF1, int Row, TFormatStyle FormatStyle);
    // ���������� OwnerId ��� �������� ������
    static int GetOwnerId(TFormLogicF1* FormLogicF1, int Row);
    // ���������� ObjectId ��� �������� ������
    static int GetObjectId(TFormLogicF1* FormLogicF1, int Row);
    // ���������� objekt_class_id ��� �������� ������
    static int GetObjekt_class_id(TFormLogicF1* FormLogicF1, int Row);
    // ���������� plan_id ��� �������� ������
    static int GetPlanId(TFormLogicF1* FormLogicF1, int Row);
    // ������� ������� TAtrRow.IsUpd
    static void IsUpdAtrRow(TFormLogicF1* FormLogicF1, int pokaz_meropr_id);
    // ���������, ���� �� IsUpd � true
    static bool IsUpdinTrue(TFormLogicF1* FormLogicF1);
    // ���������� IsUpd � False - ��� ��������� ������ �� ����� - ��� � FormGF.SetIsUpdToFalse
    static void SetIsUpdToFalse(TFormLogicF1* FormLogicF1);
    // ����� ����������� ������ ���� � ��� �����
    static bool getUpRowVTomCisle(TFormLogicF1* FormLogicF1, int aRow, int& ObjectIdUp, int& nRowUp);
    // ========== ������� ��� ������������ ������ ListStmnt ==============
    // ������� ������ ���������� �������� �����
    static void CreateListStmnt(TFormLogicF1* FormLogicF1);
    // ���������� �������� ��������������, ����������� ��� �������
    static void getColFrmtrAtr(TFormLogicF1* FormLogicF1, int aTypeData, int aCol, TFormatStyle& FormatStyle);
    static void getColFrmtrAtr(PAtrStmnt AtrStmnt, int aTypeData, TFormatStyle& FormatStyle);
    // ���������� �������� �������������� ��� ������� � ������������� ����������
    static bool getColNameFrmtrAtr(TFormLogicF1* FormLogicF1, int aCol, TFormatStyle& FormatStyle);
    // ���������� isCellSizeFixed = true - ��� ������� ����� ����� ������� �����������
    static void setCellSizeFixed(TFormLogicF1* FormLogicF1, int aCol);
    // ========== ��������� ������� ==============
    // ��������� ������������ ��� ������������� ������
    static std::string ExcIdRow(std::string aName, TFormLogicF1* FormLogicF1, int nSheet, int Row, int PokazCol);
    // �������� #MnsNNN � ������ NNN - �����
    static std::string ReplaceMnsNNN(std::string aSS, int CurrInputRow, int CurrCol);
    // �������� #PlsNNN � ������ NNN - �����
    static std::string ReplacePlsNNN(std::string aSS, int CurrInputRow, int CurrCol);
    // �������� #StrNNN � ������ NNN - �����
    static std::string ReplaceStrNNN(std::string aSS, int CurrInputRow, int CurrCol);
    // �������� #Row � #Col � ������
    static std::string ReplaceNRowNCol(int CurrInputRow, int CurrCol, std::string note1);
    // �������� ��� offset �� ������ ���� �������������
    static std::string ReplaceOffSet(std::string aSS);
    
    // ���������� TwwStringList ��� ������� �����
    static TwwStringList* GetTww(TFormLogicF1* FormLogicF1);
    // ���������� ����� ������� � ������������� ��� ������� �����
    static int GetNameCol(TFormLogicF1* FormLogicF1);
    // ���������� �������������� ����� ������� � ������������� ��� ������� �����
    static int GetNameColAlter(TFormLogicF1* FormLogicF1);
    // ���������� ����� ������� � ������������� ��� ����� � ��������� �������
    static int GetNameCol_byN(TFormLogicF1* FormLogicF1, int aNSheet);
    // ���������� FormId ������� �����
    static int GetFormId(TFormLogicF1* FormLogicF1);
    // ���������� FormId ����� � ��������� �������
    static int GetFormId_byN(TFormLogicF1* FormLogicF1, int aNSheet);
    // ���������� FormOrder ������� �����
    static int GetFormOrder(TFormLogicF1* FormLogicF1);
    // ���������� FormOrder ����� � ��������� �������
    static int GetFormOrder_byN(TFormLogicF1* FormLogicF1, int aNSheet);
    // ���������� MaxHrCol ������� �����
    static int GetMaxHrCol(TFormLogicF1* FormLogicF1);
    // ���������� MaxHrCol ����� � ��������� �������
    static int GetMaxHrCol_byN(TFormLogicF1* FormLogicF1, int aNSheet);
    // ����� ������� ��������� � ������ ������� ������
    static bool GetFactVsegoKNacPerid(TFormLogicF1* FormLogicF1, int nRow, TDateTime& dt);
     // ������ ������������ ������� ���������
    static void SetCol1Hidden(TFormLogicF1* FormLogicF1, int BegRow, int nCol);
    // ���������� ���������� ����� � ����� � ���������
    static int GetKolStrIn_ShapZag(TFormLogicF1* FormLogicF1);
    // ������� ������� ������
    static void SetSelectionById(TFormLogicF1* FormLogicF1, int ObjectId);
    
    // ========== ������� ��� ����������� ����� ==============
    // ��������� �������� ����������������� � �������� ����������� � �����
    static void SetAttribByBP(TFormLogicF1 * FormLogicF1, int nRow, int nCol);
    // ��������� ������� ���� �������� � ������ �������� ����� ������ �����������������
    static void SetAttrByBP(TFormLogicF1 * FormLogicF1, int nRow, int nCol, string stmnt);
    // ����������, ���� �� � ������� ������ ������� � ��
    static int isEstColumnforBP(TFormLogicF1 * FormLogicF1, int nRow);
    // ��������� �������� ����������� ��������
    static void SetAttribByNMrpr(TFormLogicF1 * FormLogicF1, int nRow, int nCol, int idMeropr);

    // ========== ������� ��� ��������� ��������� ������� ==============
    // ������� ��� �������� �������� �����
    static string GetSheetName(TFormLogicF1 * FormLogicF1);
    // ����� �� ������� ����� ��� ����� � �� ������� ��������� ���������
    static void GetForUnRaskrit(TFormLogicF1 * FormLogicF1, string & FormName, int & ObjectId, bool & IsRefreshOwner);
    // ������� ����� (�� �����) �������
    static bool SetCurSheet(TFormLogicF1 * FormLogicF1, string FormName);
    
    // ========== ������ ������� ==============
    // ���� ������� ��� ������� aN, ������� ���������
    static std::string GetSubFormulaByNpar(std::string aSS, int aN);
    // ��� ������ � �������� ������������ ����� - ������������� �������
    // ���������� ����� ������� [, ��� 0 (�����)
    static int GetNextFormulaConc(std::string aSS, int& aId, std::string& aRazd, int Offset);
    static void GetFont(std::string& fName, short& fSize, bool& bBold, bool& bItalic, bool& bUnderline, bool& bStrikeout, int& fColor, bool& bOutline, bool& bShadow);
    // ���������� ��� ������� ����� ����� �� ���������
    static void SetFont(std::string aFName, short aFSize);
    // �������� �������� � ����� �������� ��������� ����� ��� ������ �����
    static void GetRootPokaztlForm(TFormLogicF1* FormLogicF1, int& wPokaz_id, std::string& wPokaz_name, int& wPokaz_is_razd, int& wPokaz_type, int& wPokaz_vert);
    // ���� ������� ��� ������� aN, ������� ���������
    static std::string GetFormulaForColpar(std::string aSS, int nCol);
};
#endif

