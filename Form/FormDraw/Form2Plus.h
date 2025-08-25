
#ifndef Form2PlusH
#define Form2PlusH

#include <string>
#include "unFormLogicF1.h"
#include "unF1toDE.h"
#include "FormDopInp.h"
#include "FormTypes.h"
#include "DB_Common.h"

//struct TKntrkt {
//    int pokaz_id;
//    std::string dogovor_N;
//    std::string KBK;
//    std::string nBO;
//};
//
//typedef TKntrkt* PKntrkt;

extern int LUnc;

class TForm2Plus
{
public:
    static std::string DefaultFormat(int EdIzmInColumn);
    static std::string CreateNumberFormat(int TocnostInForm, int EdIzmInColumn);
    static bool getDocumFilesInfo(const std::string& Docum_id, TStringList& fileFullNamesAndNotes);
    static void ReSumma(TFormLogicF1 * FormLogicF1, int OwnerNomRow, bool IsSumma, int CurRow, double &summa, int SummaCol);
    static std::string getFileDatesFromJSON(const std::string& JSONString);
    static void SetFactNoVvod(TFormLogicF1* FormLogicF1);
    static void SetReadOnlyByCondition(TFormLogicF1* FormLogicF1);
    static void SetReadOnlyByStmntTest(TFormLogicF1* FormLogicF1);
    static void SetReadOnlyForLog(TFormLogicF1* FormLogicF1);
    static void SetNumberFormats1(TFormLogicF1* FormLogicF1, int aRow, int aCol, TwwStringList* p);
    static void SetNumberFormats_ReadOnlyCond(TFormLogicF1* FormLogicF1);
    static void SetNumberFormatsToStr(TFormLogicF1* FormLogicF1);
    static void HiddenEmptyRazdel(TFormLogicF1* FormLogicF1, int is_dublir_itogo);
    static void SetColHidden(TFormLogicF1* FormLogicF1);
    static void SetIdRow(TFormLogicF1* FormLogicF1, int PokazCol, int BeginRow);
    static void SvodForm(TFormLogicF1* FormLogicF1);
    static void SetNumberPP(TFormLogicF1* FormLogicF1, int BegRow, int EndRow);
    static std::string ZamenitGod(TFormLogicF1* FormLogicF1, std::string CurrPlanName, int CurrYear, std::string stroka);
    static u16string ZamenitGod1(TFormLogicF1* FormLogicF1, u16string CurrPlanName, int CurrYear, u16string stroka);
    static void ZamenitKBKFiltr(TFormLogicF1* FormLogicF1, TwwStringList* p, std::string KBKshablon);
    static void ZamenitDateFiltr(TFormLogicF1* FormLogicF1, TwwStringList* p, TDateTime DateDiapazonFrom, TDateTime DateDiapazonTo);
    static std::string ZamenitNameObject(std::string NameObject, std::string stroka);
    static void CreateForTopBottom(TwwStringList* p);
    static void SetEvalCell(int dd, TFormLogicF1* FormLogicF1, int nRow, int nCol, int nRow2, int nCol2, TQueryUni* quFormStr);
    static void AddForTopBottomItem(TFormLogicF1* FormLogicF1, int Row, int Col, int CellId, TDateTime ValueDate,
      int DataType, int VidOtch, int PlanId, short coef_Pokaz, short coef_Forma,
      int ObjectId, int PokazId, unsigned char pokaz_type, std::string note2, int atr1, std::string note1);
    static void ExecEvalCells(TFormLogicF1* FormLogicF1);
    static void EvalSummaPoStolbcu(TFormLogicF1* FormLogicF1, Pftb ftb);
    static void EvalKolObjectov(TFormLogicF1* FormLogicF1, Pftb ftb);
    static void EvalZnacSV(TFormLogicF1* FormLogicF1, Pftb ftb);
    static void EvalField(TFormLogicF1* FormLogicF1, Pftb ftb);
    static double GetSumPrixodVozvrat(TFormLogicF1* FormLogicF1, bool isPrixod, PCellValueAtr vd);
    static double SummaUtoc(TFormLogicF1 * FormLogicF1, bool IsItog, int PlanId, TDateTime PokazDate, int cellId);
    static double Ostatok(TFormLogicF1 * FormLogicF1, int CurrPlanId, int CellIdKv, int CellIdMes, int CurrPokazId, int VidOtc, TDateTime EndMes, int objectid);
    static double OstatokNI(TFormLogicF1* FormLogicF1, int CurrPlanId, int CellIdKv, int CellIdMes, int CurrPokazId, int VidOtc, TDateTime EndMes, int objectid);
    static std::string evalKonkatStr(TFormLogicF1* FormLogicF1, PCellValueAtr vd, TwwStringList* p);
    static void SetXarakterSvjzObjecta(TFormLogicF1* FormLogicF1, int aRow, int id_SvjzClassa, int id_SvjzObjecta);
    static std::string evalKonkatStrSvObj(TFormLogicF1* FormLogicF1, PCellValueAtr vd, TwwStringList* p);
    static void SummaKONTRAKTeval15all(bool isBO15all, bool isKassRasxBO15all, double& dBO15all, double& dKassRasxBO15all,
                                       int god, int is_filter, std::string shablon, TDateTime lastdate_15fcu);
    static void SummaPatternKBK(TFormLogicF1* FormLogicF1, int aI, TwwStringList* p, int pokaz_idRoot);
    static void doSvodForCelPrgrm(TFormLogicF1* FormLogicF1);
    static void ClearNList(TFormLogicF1* FormLogicF1, TList<int> tl, int aRowVid, int aN, TStringList nl,
      int cKBK, int cKBKrd, int cKBKcl, int cKBKvr, int cKBKku);
    static void ExecForMonth(TFormLogicF1* FormLogicF1, int nRow);
    static void doOutputVTomCisle(TFormLogicF1* FormLogicF1, int nRow, std::string VertStr);
    static void doZamenaInForm2(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string note1, std::string note2);
    static void SetTextDocument(TFormLogicF1* FormLogicF1, int nRow, int nCol, bool isValue);

};

#endif
