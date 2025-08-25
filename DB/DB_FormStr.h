#ifndef DB_FORMSTR_H
#define DB_FORMSTR_H

#include <string>
#include "DB_Common.h"

#include <vector>

static int cMaxStrN = 9999;
static int groupRowsForm_str_n = 10001;
static std::string stmntGroupRows = "multiLevelGrupping";
static int Kolic = 82;
static std::vector<std::string> stmnt_all = {
        "ЗНАЧЕНИЕСВ",
        "ИСТЗНАЧВВОДПК", 
        "НАИМПОКАЗ",          
        "КОДБК",
        "ЕДИЗМ",
        "ЕДИЗМНВ",
        "ПРОЦЕНТСВ",
        "ЕДИЗМобщНП",      
        "ДатаСоздОбъекта",  
        "ЗНАЧЕНИЕСВ_ДООПРЕД",  
        "ЗНАЧЕНИЕСВНИ",
        "ЗНАЧЕНИЕНВНИ",
        "ПРОЦЕНТСВНИ",
        "ПРОЦЕНТНВНИ",
        "КБК-Дгвр:Сумма№БО",
        "КБК-Дгвр:СуммаБО",
        "КБК-Дгвр:Сумма№БОоз",
        "ЗНАЧЕНИЕСВИР",
        "ЗНАЧЕНИЕНВИР",
        "РАСЧСЧЕТ",
        "ПРОИЗВСТОЛБ",
        "ОТКЛОНЕНИЕСВНачКВ",
        "ОТКЛОНЕНИЕНВНачКВ",
        "ФОРМУЛАF1",
        "УТОЧНЕНИЕСВ",
        "Мрпр-Дгвр:Сумма№БО",         
        "УТОЧНЕНИЕСВНИ",
        "Мрпр-Дгвр:СуммаБО",   
        "ПлнОСТАТОК",
        "ФктОСТАТОК",
        "РАСЧСЧЕТ2",
        "КОДБКРздл",
        "КОДБКЦелСт",
        "КОДБКВидРасх",
        "КОДБКЭкКл",
        "НомерМрпр",
        "ПлнОСТАТОКНИ",
        "ИНН",
        "ЮрАдрес",
        "УТОЧНЕНИЕСВПосле",
        "ФактВсегоВНачале",
        "СуммаПриход",
        "СуммаВозврат",
        "СуммаПлГрНИВНачале",
        "Рспрж:УТВРЖДсНИ",
        "КБК-Дгвр:СуммаКЗ",
        "Рспрж:УТВРЖД",
        "Рспрж:ПРОЕКТ",
        "ФактКвДату",       
        "СуммаПлГрНИВМесяце",
        "ааЗначВырМнрг",
        "ааДельтаЭтлн",
        "ааИдентифТенд",
        "ЛицевойСчетТБ",
        "ааПрирост",
        "ааПроцПриростБаза",
        "УточПланВклДату",
        "КОДБК1",
        "КОДБКРздл1",
        "КОДБКЦелСт1",
        "КБК-Дгвр:Кол-во",
        "КБК-Дгвр:Кол-воБО",
        "БДЖПОЛУЧТЛЬ",
        "ПДГ_УТ_КассПл",
        "УТВ_УТ_КассПл",
        "Примечание",
        "Ведомство",
        "СуммаПлГрНИГод",
        "ЗНАЧЕНИЕобщНП",
        "БДЖПОЛУЧТЛЬ_нет()",
        "ХарактСВобъекта",
        "КонкатСтрок",
        "КонкатСтрокСВобъек",
        "№п/п",
        "Экономия/Резерв",
        "отчетCO_RP_015",
        "СведенияОбЭкономии",
        "НАИМЦелСт",
        "forUnikIdentif",
        "NamePlanВстроке",
        "ОперПрисвоить",
        "ПоступлВсегоВНачале"
    };

class TDB_FormStr {
public:
    static void initSqlText();
    static std::string GetNote1(int form_id, int form_str_n);
    static void AddRow(int form_id, int& aForm_str_n, std::string Stmnt, int attr_1, int attr_2, int attr_3, int attr_4, int is_ist_fin, int ist_fin_grad_id,
                       std::string note1, int vid_otch, int natur_vid, std::string note2, int period, std::string din_izm,
                       int is_group, int align_vert, int align_gor, std::string font,
                       int h, int is_bold, int plan_id, int pokaz_id, int CoefPlus1, int ValuePeriod, int readonly, int is_hidden,
                       int is_set_god, int god_plus, int god_ravno, int field_id, int is_sort, int uroven_sort, int is_svod,
                       std::string name_svod, int is_filter, int is_Check, int is_filter2, int ColOnFilter2,
                       int nValidator, int vid_uslovij, double value1, double value2, std::string shablon, std::string Msg);
    static int AddRow1(int form_id, int form_str_n);
    static int AddRowFromOtherForm(int form_id, int form_idOther, int form_str_nOther, bool isNewstr);
    static void AddPokaz(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, int pokaz_id, int plan_id,
                          int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                          int is_filter, int is_filter2, int ColOnFilter2, int Filter2Variant = 0);
    static void AddKorteg(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, int pokaz_id, std::string note2,
                          int plan_id, int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                          int is_filter, int is_filter2, int ColOnFilter2);
    static void AddProizStr(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n,
                   std::string note1, int Level_sign, int nStolbec, int is_hidden);
    static void AddColForNamePokaz(int form_id, int ColForNamePokaz);
    static void AddFormF1(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1,
                   std::string note2, int Level_sign, int nStolbec, int is_hidden, int is_filter2, int ColOnFilter2,
                   int plan_id, int is_svod);
    static void AddZamena(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int is_filter2, int ColOnFilter2);
    static void AddPrisvoit(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int toCol, int plan_id, bool aIsDoOnlyByCellUpd, int isRow);
    static void AddPologit(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int toCol, int plan_id);
    static void AddBlock(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n,
                   std::string note1, int field_id, int is_filter2, int ColOnFilter2, int plan_id);
    static void AddDialogBlock(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n,
                   std::string note1, int field_id, int is_filter2, int ColOnFilter2);
    static void AddSsilkaNaStr(bool isInsert, int posleFormStrN, int form_id,
                   int& aForm_str_n, std::string note1, int Level_sign, int nStolbec);
    static void AddAgregat(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int field_id, int is_svod, int is_filter2, int ColOnFilter2);
    static void AddUslovie(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, int plan_id,
                   int Level_sign, int readonly, int is_hidden, int nStolbec, int prefToCol);
    static void AddSummaPatternKBK(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, std::string note2, int Level_sign,
                   int nStolbec, int is_hidden, int is_filter, std::string shablon, int plan_id, int pokaz_id,
                   int is_filter2, int ColOnFilter2);
    static void AddButton(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string& note1, std::string& note2,
                   int nCol, int h, std::string& msg);
    static void AddObjects1n(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int is_filter, int is_filter2, int ColOnFilter2);
    static void AddStyleOforml(bool isInsert, int posleFormStrN, int form_id, int& aForm_str_n,
       std::string note1, std::string note2);

    static void AddGroupRowsInfo(int form_id, std::string JSONGroupRows);
    static void UpdGroupRowsInfo(int form_id, std::string JSONGroupRows);
    static bool GetGroupRowsInfo(int form_id, std::string& JSONGroupRows);

    static void UpdPokaz(int form_id, int form_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int is_filter, int is_filter2, int ColOnFilter2, int Filter2Variant = 0);
    static void UpdPokazUstGod(int form_id, int form_str_n, int is_set_god, int god_plus, int god_ravno);
    static void UpdPokazUstPeriod(int form_id, int form_str_n, int period, int ValuePeriod);
    static void UpdIstFinGradId(int form_id, int form_str_n, int is_ist_fin, int ist_fin_grad_id);
    static void UpdTocnost(int form_id, int form_str_n, int tocnost);
    static void AddAlignmentObjedinit(int form_id, int& aForm_str_n, int is_group,
      int align_vert, int align_gor, std::string font, int h, int is_bold);
    static void AddOdinakovZnac(int form_id, int& aForm_str_n, std::string note2);
    static void UpdPokazId(int form_id, int form_str_n, int pokaz_id);
    static void UpdFieldId(int form_id, int form_str_n, int field_id);
    static bool getFieldId(int form_id, int form_str_n, int& field_id);
    static void UpdPlanId(int form_id, int form_str_n, int plan_id);
    static void UpdNIerarxii(int form_id, int form_str_n, int nIerarxii);
    static void UpdKorteg(int form_id, int form_str_n, std::string note1, int pokaz_id, std::string note2, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int is_filter, int is_filter2, int ColOnFilter2);
    static void UpdProizStr(int form_id, int form_str_n, std::string note1, int Level_sign,
          int nStolbec, int is_hidden);
    static void UpdFormF1(int form_id, int form_str_n, std::string note1, std::string note2, int Level_sign,
                    int nStolbec, int is_hidden, int is_filter2, int ColOnFilter2,
                    int plan_id, int is_svod);
    static void UpdZamena(int form_id, int form_str_n, std::string note1, std::string note2);
    static void UpdPrisvoit(int form_id, int form_str_n, std::string note1, std::string note2, int Level_sign,
                    int nStolbec, int is_hidden, int toCol, int plan_id, bool aIsDoOnlyByCellUpd, int isRow);
    static void UpdSsilkaNaStr(int form_id, int form_str_n, std::string note1, int Level_sign, int nStolbec);
    static void UpdAgregat(int form_id, int form_str_n, std::string note1, int pokaz_id, int plan_id,
                   int Level_sign, int Is_open, int readonly, int is_hidden, int nStolbec, int noPrint,
                   int field_id, int is_svod, int is_filter2, int ColOnFilter2);
    static void UpdButton(int form_id, int form_str_n, std::string& note1, std::string& note2,
                   int nCol, int h, std::string& msg);
    static void UpdUslovie(int form_id, int form_str_n, std::string note1, int plan_id,
                   int Level_sign, int readonly, int is_hidden, int nStolbec, int prefToCol);
    static void UpdSummaPatternKBK(int form_id, int form_str_n, std::string note1, std::string note2, int Level_sign,
                  int nStolbec, int is_hidden, int is_filter, std::string shablon, int plan_id, int pokaz_id,
                  int is_filter2, int ColOnFilter2);
    static void UpdBlock(int form_id, int form_str_n, std::string note1, int field_id,
        int is_filter2, int ColOnFilter2, int plan_id);
    static void UpdDialogBlock(int form_id, int form_str_n, std::string note1, int field_id,
        int is_filter2, int ColOnFilter2);
    static void UpdStyleOforml(int form_id, int form_str_n, const std::string note2);
    static void UpdStmnt(int form_id, int form_str_n, const std::string Stmnt);
    static void UpdNote2(int form_id, int form_str_n, const std::string note2);
    static std::string GetNote2(int form_id, int form_str_n);
    static void UpdFilter2(int form_id, int form_str_n, int is_filter2, int ColOnFilter2);
    static void GetFilter2(int form_id, int form_str_n, int& is_filter2, int& ColOnFilter2);
    static void DelObjAtrs(int form_id);
    static void DelAllItems(int form_id);
    static void DelItemsN1N2(int form_id, int N1, int N2);
    static int GetMaxNum0(int form_id);
    static int GetMaxNum1(int form_id);
    static int GetMaxNum(int form_id);
    static void GetItemsN1N2(TQueryUni* qIn, int form_id, int N1, int N2);
    static void GetItemsN1N2WithFieldName(TQueryUni* qIn, int form_id, int N1, int N2);
    static void GetItemsN1N2_N3N4(TQueryUni* qIn, int form_id, int N1, int N2, int N3, int N4);
    static void GetItemsAll(TQueryUni* qIn, int form_id);
    static void GetStmntItemsN1N2(TQueryUni* qIn, int form_id, int N1, int N2, const std::string& stmnt);
    static void GetStmnt(TQueryUni* qIn, int form_id, int form_str_n);
    static void GetRow(TQueryUni* qIn, int form_id, int form_str_n);
    static int GetAttr_3(int form_id, int form_str_n);
    static int GetAttr_2(int form_id, int form_str_n);
    static std::string GetVidStmnt(int form_id, int form_str_n, int& pokaz_id);
    static void DelSort(int form_id);
    static void AddSort(int form_id, int NCol, int is_sort, int uroven_sort, int is_svod, const std::string& name_svod);
    static bool IsClmn(int form_id, int NCol);
    static void GetCellStmnt(TQueryUni* qIn, int form_id, const std::string& stmntText, const std::string& stmnt, int Col, int Row);
    static int GetForm_str_n(int form_id, const std::string& stmnt, int CellContentNum);
    static void GetStmntAll(TQueryUni* qIn, int form_id);
    static void GetFontInfo(int form_id, int form_str_n, std::string& font, int& h, int& is_bold);
    static void SetFont(int form_id, int form_str_n, const std::string& font, int h, int is_bold);
    static void AddFont(int form_id, int form_str_n);
    static int getfontColor(int form_id, int form_str_n);
    static void setfontColor(int form_id, int form_str_n, int fontColor);
    static int getBGcolor(int form_id, int form_str_n);
    static void setBGcolor(int form_id, int form_str_n, int BGcolor);
    static int getBorderStyle(int form_id, int form_str_n);
    static void setBorderStyle(int form_id, int form_str_n, int borderStyle);
    static int getBorderColor(int form_id, int form_str_n);
    static void setBorderColor(int form_id, int form_str_n, int borderColor);
    static void GetAlignment(int form_id, int form_str_n, int& align_vert, int& align_gor);
    static void SetAlignment(int form_id, int form_str_n, int align_vert, int align_gor);
    static int GetIsHidden(int form_id, int form_str_n);
    static void SetOdinakovZnac(int form_id, int form_str_n, const std::string& note2);
    static void GetObjedinitKolCell(int form_id, int form_str_n, int& is_group);
    static void SetObjedinitKolCell(int form_id, int form_str_n, int is_group);
    static void SetNValidator(int form_id, int form_str_n, int nValidator);
    static void UpdForm_str_n(int form_id, int form_str_n, int new_form_str_n);
    static void UpdForm_str_nAttr_1(int form_id, int form_str_n, int new_form_str_n, int attr_1);
    static void UpdAttr_2(int form_id, int form_str_n, int attr_2);
    static void NaznacCol_LeftOrRight(int form_id, int aCol, bool isLeft);
    static int GetIndexByStmnt(std::string stmnt);
    static void UpdValidator(int form_id, int form_str_n, int nValidator, int vid_uslovij, double value1, double value2, const std::string& shablon, const std::string& Msg);
    static void UpdShablon(int form_id, int form_str_n, std::string shablon);
    static std::string getNumberFormat(int form_id, int form_str_n);
    static void UpdnumberFormat(int form_id, int form_str_n, const std::string& numberFormat);
    static void GetColDestList(TQueryUni* qIn, int form_id);
    static std::string getName_svod(int form_id, int form_str_n);
    static void setName_svod(int form_id, int form_str_n, const std::string& name_svod);

    static void SetRangeMerged(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, bool merged);
    static TQueryUni* GetRangeField(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2);
    static bool RangeExists(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2);
    static void SetRangeText(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmnt, const std::string& text);
    static void SetRangeCellFormat(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& fontName, int fontSize, int fontStyle, int fontColor, int backgroundColor, int hAlign, int vAlign, int borderStyle, int borderColor);
    static void SetRangeField(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmnt, int fieldId, int readonly, int tocnost, int isIstFin, int istFinGradId, int period, int formStrNSub, int vidOtch, const std::string& dinIzm, int isSetGod, int godPlus, int godRavno, int planId, int coefPlus1, int valuePeriod, int checkOnClose, int checkValidator, int checkConditionType, double checkValue1, double checkValue2, const std::string& checkShablon, const std::string& checkMsg, const std::string& additionalCellStyle);

    static bool IntersectRangeExists(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmntText);

    static void IntersectRangeDelete(int formId, int formStrNFrom, int formStrNTo, int row1, int col1, int row2, int col2, const std::string& stmntText);
    static void LoadTostmntList(TStringList* stmntList);
};

#endif // DB_FORMSTR_H
