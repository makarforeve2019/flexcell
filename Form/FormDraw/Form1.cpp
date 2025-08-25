
#include "Form1.h"
#include "../FormTypes.h"
#include "FormDopInp.h"
#include "../../DB/DB_NMrpr.h"
#include "../../DB/DB_FormStr.h"
#include "Form2Plus.h"
#include "unF1toDE.h"
#include "Form2.h"
#include "FormGF_Logic.h"
#include "../../DB/DB_FormStrTMP.h"
#include "../../SystFuncs/SystFuncsLogic.h"


// если IsParamForm = 1, то для параметрической формы, если 0, то нет
// ParamObjectId содержит ид объекта конкретизации
bool TForm1::FormInit(TInfoFormLogic* InfoFormLogic, TFormLogicF1* FormLogicF1, unsigned char IsParamForm, int ParamObjectId,
       std::string ParamObjectName, int FormId, int FormOrder, TQueryUni* quForms,
       std::string& ErrorMsg1, std::string& ErrorMsg2, TwwStringList* p)
{
    TracerManager::Instance().StartFunctionTrace("TForm1::FormInit");
    TFormatSettings FormatSettings;
    int PokazId;
    std::string QName, form_name;
    std::string font;
    int h;
    int is_bold;
//    TwwStringList* p;
    TFormatStyle* FormatStyle = new TFormatStyle();

    bool Result = true;
//	p = TFormDopInp::GetTww(FormLogicF1);
    TDB_NMrpr::SetCurDBName(TDB_NMrpr::GetCurDBNameFromDB());
    FormLogicF1->FormDM->quFormStr->Close();
    FormLogicF1->FormDM->quFormStr->ParamByName("CurrFormId")->AsInteger() = FormId;
    FormLogicF1->FormDM->quFormStr->Open();
    //{Не Объектная форма}
    if ((p->work_with != 2) && FormLogicF1->FormDM->quFormStr->EoF()) {
        FormLogicF1->FormDM->quFormStr->Close();
        //--TunProgress::doHide(FormLogicF1);
        throw std::runtime_error("Описание формы для указанного года пустое");
    }
    TDB_FormStr::GetFontInfo(FormId, 10000/*form_str_n*/, font, h, is_bold);
    TFormDopInp::SetFont(font, h);
    TForm2Plus::CreateForTopBottom(p);
    PokazId = quForms->FieldByName("pokaz_id")->AsInteger();
    FormLogicF1->CurrInputRow = p->MaxHrRow;

    // создаем строки заголовка
    TF1toDE::CreateNewRowsCells(p, FormLogicF1, 1, p->DeltaRow);
    if (quForms->FieldByName("form_k_zagtop")->AsInteger() > 0){
         TForm1::InitFormHeader(FormLogicF1, "_TOP", 0, IsParamForm, ParamObjectName, p); // формирование заголовка
    }
    // создаем строки шапки


    TF1toDE::CreateNewRowsCells(p, FormLogicF1, p->DeltaRow + 1, p->DeltaRow + p->form_k1);

    TForm1::InitFormHeader(FormLogicF1, "", p->DeltaRow, IsParamForm, ParamObjectName, p); // формирование шапки таблицы

    p->form_begBodyRow = FormLogicF1->CurrInputRow + 1; // формирование номера первой строки тела таблицы

    FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = PokazId;
    FormLogicF1->FormDM->quPokaz->ParamByName("God")->AsInteger() = FormLogicF1->CurrYear;
    p->RootPokazId = PokazId;
    switch (p->work_with) {
    case 3:
        p->RootPokazId = quForms->FieldByName("class_id")->AsInteger();
        if (p->IsParamForm == 1)
            p->RootPokazId = p->ParamObjectId;
        break;
    case 4:
        if (p->IsParamForm == 1)
            p->RootPokazId = p->ParamObjectId;
        break;
    case 5:
        p->ogr_class_id = quForms->FieldByName("class_id")->AsInteger();
        // в случае формы типа 5 поступаем особым образом
        // а  p.RootPokazId сформируется в дальнейшем        
        break;
    }

    try {
        switch (p->work_with) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            TForm2::InitFormRows(InfoFormLogic, FormLogicF1, IsParamForm, ParamObjectId);
            break;
        }
        p->form_endBodyRow = FormLogicF1->CurrInputRow;// формирование номера последней строки тела таблицы
        if (quForms->FieldByName("form_k_zagbottom")->AsInteger() > 0) {

            TForm1::InitFormHeader(FormLogicF1, "_BOTTOM", FormLogicF1->CurrInputRow, IsParamForm, ParamObjectName, p);  // формирование подвала
            FormLogicF1->CurrInputRow = FormLogicF1->CurrInputRow + quForms->FieldByName("form_k_zagbottom")->AsInteger();
        }

        if (int N = FormLogicF1->quForms->FieldByName("work_with_gf")->AsInteger(); N==10||N==13||N==14)
            TFormGF_Logic::strRashirit(FormLogicF1);
    }
    catch (std::exception& e) {
        Result = false;
        ErrorMsg1 = e.what();
        if ((FormLogicF1->f1Forms->MaxRow <= p->MaxHrRow) || (FormLogicF1->f1Forms->MaxCol <= p->MaxHrCol))
            ErrorMsg2 = "Нет данных для формы! Форма будет закрыта.";
        else
            ErrorMsg2 = "Описание формы неверно! Форма будет закрыта.";
        switch (p->work_with) {
        case 10:
        case 11:
        case 12:
            TDB_FormStrTMP::DropForm_strTMP();
            break;
        }
        TracerManager::Instance().AddSpanLog("Exception.TForm1::FormInit", {{"errordesr1", ErrorMsg1},
                                                                            {"errordesr2", ErrorMsg2}},
                                             TracerManager::LOG_LEVEL_ERROR);
        delete FormatStyle;
        TracerManager::Instance().EndFunctionTrace();
        return Result;
    }

    FormLogicF1->f1Forms->Recalc();
    FormLogicF1->f1Forms->AutoRecalc = true;

    if (p->work_with != 2) { //ky: Объектная форма
        TForm2Plus::SetNumberFormats_ReadOnlyCond(FormLogicF1); // устанавливает формат столбцов в стоим выражении
    }

    QName = "";
    if (p->ComplexForm) {
        switch (FormLogicF1->CurrQuarter) {
        case 1:
            QName = "I квартал ";
            break;
        case 2:
            QName = "1-ое полугодие ";
            break;
        case 3:
            QName = "9 месяцев ";
            break;
        case 4:
            QName = "год ";
            break;
        }
    }
    else {
        switch (FormLogicF1->CurrQuarter) {
        case 1:
            QName = "I квартал ";
            break;
        case 2:
            QName = "II квартал ";
            break;
        case 3:
            QName = "III квартал ";
            break;
        case 4:
            QName = "IV квартал ";
            break;
        }
    }
    switch (quForms->FieldByName("work_with_gf")->AsInteger()) {
    case 10:case 11:case 12:case 13:case 14:
        QName = FormatSettings.LongMonthNames(FormLogicF1->CurrMonth) + " ";
        break;
    }
    if (QName == "")
        QName = IntToStr(FormLogicF1->CurrYear) + " год";
    else
        QName = QName + IntToStr(FormLogicF1->CurrYear) + " года";
    if ((p->work_with != 2) && (quForms->FieldByName("form_k_zagtop")->AsInteger() == 0)) {
        form_name = quForms->FieldByName("form_name")->AsString();
        if (int N = p->work_with; (N==1||N==3) && (p->plan_id > 1))
            form_name = TForm2Plus::ZamenitGod(FormLogicF1, p->plan_name, FormLogicF1->CurrYear, form_name);
        else
            form_name = TForm2Plus::ZamenitGod(FormLogicF1, FormLogicF1->CurrPlanName, FormLogicF1->CurrYear, form_name);
        if (IsParamForm == 1) // если форма парметрическая, то, возможно, добавляем имя объект
            form_name = TForm2Plus::ZamenitNameObject(ParamObjectName, form_name);
        QName = " за " + QName;
        if (quForms->FieldByName("vid_form")->AsInteger() == 3) //аналитическая
            QName = "";
        FormatStyle->align_vert = FormLogicF1->FormDM->quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = FormLogicF1->FormDM->quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = FormLogicF1->FormDM->quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = 13;
        FormatStyle->is_bold = FormLogicF1->FormDM->quFormStr->FieldByName("is_bold")->AsInteger() | 0x01;
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, 2, 1, 2, p->MaxHrCol, form_name + QName, 1002, FormatStyle);
        if (int N = quForms->FieldByName("work_with_gf")->AsInteger(); N==11||N==12) {
            FormatStyle->setToNull();
            FormatStyle->is_bold = 0x01;
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, 3, 1, 3, 1, "Дата: " + std::to_string(std::time(nullptr)), 1003, FormatStyle);
        }
    }

    TForm2Plus::ExecEvalCells(FormLogicF1);// вычислить вычисляемые ячейки в заголовке и подвале формы
    // установка режима Только чтение
    switch (FormLogicF1->quForms->FieldByName("form_readonly")->AsInteger()) {
    case 2:
        TForm2Plus::SetReadOnlyByCondition(FormLogicF1);
        break;
    case 3:
    case 4:
        TForm2Plus::SetReadOnlyByStmntTest(FormLogicF1);
        break;
    case 5:
        TForm2Plus::SetReadOnlyForLog(FormLogicF1);
        break;
    }

    FormLogicF1->FormDM->quFormStr->Close();
    delete FormatStyle;
    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

void TForm1::InitFormHeader(TFormLogicF1* FormLogicF1, std::string ToStmnt, int initRow, unsigned char IsParamForm,
      std::string ParamObjectName, TwwStringList* p) 
{
    std::string Stmnt, stroka, note2;
    int CurrRow, CurrCol, CurrRow2, CurrCol2, BorderStyle, dd;
    TFormatStyle* FormatStyle = new TFormatStyle();

    if (ToStmnt == "") {// это шапка
        dd = 500;
    } else {
        dd = 1000;
    }
   // вначале создаем столбцы
    int ind=0;
    if (ToStmnt != "_BOTTOM") {
        FormLogicF1->FormDM->quFormStr->First();
        while (!FormLogicF1->FormDM->quFormStr->EoF()) {
            Stmnt = AnsiUpperCase(FormLogicF1->FormDM->quFormStr->FieldByName("stmnt")->AsString());
            if (Stmnt == "ШИРИНА") {
                CurrCol = FormLogicF1->FormDM->quFormStr->FieldByName("attr_1")->AsInteger();
                if (CurrCol <= p->MaxHrCol) {
                    TF1toDE::ColWidth(FormLogicF1, CurrCol, PixelsToTwips(FormLogicF1->FormDM->quFormStr->FieldByName("attr_2")->AsInteger()), FormLogicF1->FormDM->quFormStr->FieldByName("attr_2")->AsInteger());
                }
            }
            if (ind == 95) {
                string ggg = "";
            }
            FormLogicF1->FormDM->quFormStr->Next();
            ind= ind+1;
        }
    }

    FormLogicF1->FormDM->quFormStr->First();
    while (!FormLogicF1->FormDM->quFormStr->EoF()) {
        Stmnt = AnsiUpperCase(FormLogicF1->FormDM->quFormStr->FieldByName("stmnt")->AsString());
        if (Stmnt == ("ТЕКСТ" + ToStmnt)) {
            CurrRow = FormLogicF1->FormDM->quFormStr->FieldByName("attr_1")->AsInteger() + initRow;
            CurrCol = FormLogicF1->FormDM->quFormStr->FieldByName("attr_2")->AsInteger();
            CurrRow2 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_3")->AsInteger() + initRow;
            CurrCol2 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_4")->AsInteger();
            stroka = FormLogicF1->FormDM->quFormStr->FieldByName("note1")->AsString();
            note2 = FormLogicF1->FormDM->quFormStr->FieldByName("note2")->AsString();
            if (note2 == "") {
                stroka = TForm2Plus::ZamenitGod(FormLogicF1, FormLogicF1->CurrPlanName, FormLogicF1->CurrYear, stroka); // заменить год, район

                // формат такой {КБКфильтр} - КБК для фильтрации
                if (Pos("{КБКфильтр}", stroka) >= 0) {
                    p->isKBKfiltr = true; // если true - значит в заголовке есть параметр {КБКфильтр}
                    p->KBKfiltrRow = CurrRow; // номера строки и столбца этого параметра
                    p->KBKfiltrCol = CurrCol;
                } else {
                    if (Pos("{Сфильтр}", stroka) >= 0) {
                        p->isDatefiltr = true; // если true - значит в заголовке есть параметр {Сфильтр} или {ПОфильтр}
                        p->DatefiltrFromRow = CurrRow; // номера строки и столбца этого параметра
                        p->DatefiltrFromCol = CurrCol;
                    }
                    if (Pos("{ПОфильтр}", stroka) >= 0) {
                        p->isDatefiltr = true; // если true - значит в заголовке есть параметр {Сфильтр} или {ПОфильтр}
                        p->DatefiltrToRow = CurrRow; // номера строки и столбца этого параметра
                        p->DatefiltrToCol = CurrCol;
                    }
                }

                if (IsParamForm == 1) { // если форма парметрическая, то, возможно, добавляем имя объект
                    stroka = TForm2Plus::ZamenitNameObject(ParamObjectName, stroka);
                }
                FormatStyle->setToNull();
                FormatStyle->align_vert = FormLogicF1->FormDM->quFormStr->FieldByName("align_vert")->AsInteger();
                FormatStyle->align_gor = FormLogicF1->FormDM->quFormStr->FieldByName("align_gor")->AsInteger();
                FormatStyle->font = FormLogicF1->FormDM->quFormStr->FieldByName("font")->AsString();
                FormatStyle->h = FormLogicF1->FormDM->quFormStr->FieldByName("h")->AsInteger();
                FormatStyle->is_bold = FormLogicF1->FormDM->quFormStr->FieldByName("is_bold")->AsInteger();
                FormatStyle->FontColor = FormLogicF1->FormDM->quFormStr->FieldByName("fontColor")->AsInteger();
                FormatStyle->fonColor = FormLogicF1->FormDM->quFormStr->FieldByName("patternBGcolor")->AsInteger();
                FormatStyle->isOrientation = FormLogicF1->FormDM->quFormStr->FieldByName("isorientation")->AsInteger();

                if (ToStmnt == "") { // это шапка
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, CurrRow, CurrCol, CurrRow2, CurrCol2, 
                      stroka, dd, FormatStyle);
                } else {
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, CurrRow, CurrCol, CurrRow2, CurrCol2, 
                      stroka, dd, FormatStyle);
                }
                FormLogicF1->f1Forms->Selection_Set(CurrRow,CurrCol,CurrRow2,CurrCol2);
                FormLogicF1->f1Forms->Protection_Set(true, true);
            } else { // вычисляемая ячейка
                TForm2Plus::SetEvalCell(dd, FormLogicF1, CurrRow, CurrCol, CurrRow2, CurrCol2, FormLogicF1->FormDM->quFormStr);
            }
        }
        FormLogicF1->FormDM->quFormStr->Next();
    }

    FormLogicF1->FormDM->quFormStr->First();
    while (!FormLogicF1->FormDM->quFormStr->EoF()) {
        Stmnt = AnsiUpperCase(FormLogicF1->FormDM->quFormStr->FieldByName("stmnt")->AsString());
        if (Stmnt == ("ПРЯМУГК" + ToStmnt)) {
            CurrRow = FormLogicF1->FormDM->quFormStr->FieldByName("attr_1")->AsInteger() + initRow;
            p->isPU0 = true;
            CurrCol = FormLogicF1->FormDM->quFormStr->FieldByName("attr_2")->AsInteger();
            CurrRow2 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_3")->AsInteger() + initRow;
            CurrCol2 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_4")->AsInteger();

            FormLogicF1->f1Forms->Selection_Set(CurrRow, CurrCol, CurrRow2, CurrCol2);
            BorderStyle = FormLogicF1->FormDM->quFormStr->FieldByName("BorderStyle")->AsInteger();
            if (BorderStyle == 0) {
                BorderStyle = 1;
            }
            FormLogicF1->f1Forms->Border_Set(BorderStyle, BorderStyle, BorderStyle, BorderStyle, BorderStyle, 0, 0, 0, 0, 0, 0);
        }
        if (Stmnt == "ALIGNMENTOBJEDINIT") {
            p->form_str_n_AlignmentObjedinit = FormLogicF1->FormDM->quFormStr->FieldByName("form_str_n")->AsInteger();
        }
        FormLogicF1->FormDM->quFormStr->Next();
    }
    delete FormatStyle;
}

// параметр is_bold побитный: $01 - Bold, $02 - Italic, $04 - Underline, $08 - StrikeOut
// В случае Шапки параметр DataType начинается с 500
// В случае Заголовка и Подвала параметр DataType начинается с 1000
//    при этом 1000 - строка, 1001 - число, 1002 - название формы, 1003 - дата, 1004 - формула
void TForm1::SetCellValue(TFormLogicF1* FormLogicF1, TF1FlexCel* aF1Book, 
     int aMaxHrCol, int aRow1, int aCol1, int aRow2, int aCol2, std::string Value,
     short DataType, PFormatStyle FormatStyle)
{
    TF1CellFormat cf;
    string newName, fName;
    short newSize, fSize;
    bool newBold, newItalic, newUnderline, newStrikeout;
    int fColor;
    bool bBold, bItalic, bUnderline, bStrikeout, bOutline, bShadow;
    string s;
    TwwStringList* p;
    PAtrRow AtrRow;
    p = TFormDopInp::GetTww(FormLogicF1);
    if (p->work_with == 2)//ky: Объектная форма
    {
        if (!(aRow1 > 0 && aRow2 > 0 && aCol1 > 0 && aCol2 > 0))
        {
            //throw Exception("Описание формы неверно! Исключение произошло в TForm1.InputText в выражении <not ((CurrRow > 0) and (CurrRow2 > 0) and (CurrCol > 0) and (CurrCol2 > 0))> ");
            return;
        }
    }
    else
    {
        if (!(aRow1 > 0 && aRow2 > 0 && aCol1 > 0 && aCol2 > 0 && aCol1 <= aMaxHrCol && aCol2 <= aMaxHrCol))
        {
            //throw Exception("Описание формы неверно! Исключение произошло в TForm1.InputText в выражении <not ((CurrRow > 0) and (CurrRow2 > 0) and (CurrCol > 0) and (CurrCol2 > 0) and (CurrCol <= aMaxHrCol) and (CurrCol2 <= aMaxHrCol))> ");
            return;
        }
    }

    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow1);
    if (FormatStyle->font == "" && FormatStyle->h > 0)
        FormatStyle->font = "Arial"; // при описании формы
                          // при h > 0 фонт, равный 'Arial' сохраняется, как ''
                          // это с целью минимизации хранения
    aF1Book->Selection_Set(aRow1, aCol1, aRow2, aCol2);
    TFormDopInp::GetFont(fName, fSize, bBold, bItalic, bUnderline, bStrikeout, fColor, bOutline, bShadow);
    if (aF1Book->FontBold) // {если для ячейки ранее был установлен Bold = true}
        bBold = true;

    if (!(aRow1 == aRow2 && aCol1 == aCol2))
        FormLogicF1->f1Forms->MergeCells = true;

    try {
        cf.GetCellFormat(*FormLogicF1->f1Forms);
    } catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TForm1::SetCellValue", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    cf.WordWrap_Set(true);
    if (aF1Book->ColHidden[aCol1])
        cf.WordWrap_Set(false); // если столбец невидимый, то False, чтобы высота строки была нормальной
    if (DataType == 0)
    {
        cf.WordWrap_Set(false);
        if (Value.length() > 1 && Value.substr(1, 1) == "^")
        {
            if (AnsiContainsText(Value, "CONCATENATE") && !aF1Book->ColHidden[aCol1])
                cf.WordWrap_Set(true);
            // специальное включение CLEAN(текст) вызывает WordWrap.  CLEAN означает исключение непечатываемых символов
            if (AnsiContainsText(Value, "CLEAN") && !aF1Book->ColHidden[aCol1])
                cf.WordWrap_Set(true);
        }
    }
    if (DataType < 200)
    {
        switch (FormatStyle->align_vert)
        {
            case 0:
            case 1:
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignTop));
                break;
            case 2:
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter));
                break;
            case 3:
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignBottom));
                break;
        }
        switch (FormatStyle->align_gor)
        {
            case 0:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter));
                break;
            case 1:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignLeft));
                break;
            case 2:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter));
                break;
            case 3:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignRight));
                break;
        }
        switch (DataType)
        {
            case 1:// число
                if (FormatStyle->align_gor == 0)
                    cf.AlignHorizontal_Set(static_cast<uint32_t>(F1HAlignConstants::F1HAlignRight));
                if (FormatStyle->align_vert == 0)
                    cf.AlignVertical_Set(static_cast<uint32_t>(F1VAlignConstants::F1VAlignTop));
                break;
            case 5:
            case 6:// дата, гиперссылка
                if (FormatStyle->align_gor == 0)
                    cf.AlignHorizontal_Set(static_cast<uint32_t>(F1HAlignConstants::F1HAlignCenter));
                if (FormatStyle->align_vert == 0)
                    cf.AlignVertical_Set(static_cast<uint32_t>(F1VAlignConstants::F1VAlignTop));
                break;
            case 2:
            case 3:
            case 4:
            case 12://{строка-список из}
                if (FormatStyle->align_gor == 0)
                    cf.AlignHorizontal_Set(static_cast<uint32_t>(F1HAlignConstants::F1HAlignLeft));
                if (FormatStyle->align_vert == 0)
                    cf.AlignVertical_Set(static_cast<uint32_t>(F1VAlignConstants::F1VAlignTop));
                break;
        }
        if (aRow1 != aRow2)
            cf.WordWrap_Set(true);

        if (FormatStyle->font != "")
            newName = FormatStyle->font;
        else
            newName = fName;

        newBold = bBold;
        newItalic = bItalic;
    }
    else
    {
        switch (FormatStyle->align_vert)
        {
            case 0:
            case 1:
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignTop));
                break;
            case 2:
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter));
                break;
            case 3:
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignBottom));
                break;
        }
        cf.AlignHorizontal_Set(FormatStyle->align_gor /*+ 1*/);
        if (FormatStyle->is_bold & 0x01)
            cf.FontBold_Set(true);
        switch (DataType)
        {
            case 200: // наименование показателя
                cf.WordWrap_Set(true);
                break;
            case 500: // это в шапке
                cf.WordWrap_Set(false);
                break;
            case 1000:
                cf.WordWrap_Set(true);
                break;
            case 1002: // название формы
                cf.WordWrap_Set(false);
                cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter));
                cf.AlignVertical_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter));
                break;
            case 1003: // дата
                if (aRow1 == 1)
                {
                    cf.WordWrap_Set(false);
                    cf.AlignVertical_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignTop));
                }
                cf.AlignVertical_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignLeft));
                break;
            case 1004: // формула
                cf.WordWrap_Set(false);
                if (Value.length() > 1 && Value.substr(1, 1) == "^")
                    cf.WordWrap_Set(true);
                cf.NumberFormat_Set(FormatStyle->NumberFormat);
                break;
        }
        if (aRow1 != aRow2)
            cf.WordWrap_Set(true);
        if (FormatStyle->font != "")
            newName = FormatStyle->font;
        else
            newName = fName;
    }

    if (FormatStyle->isOrientation > 0)
        cf.Rotation_Set(FormatStyle->isOrientation);

  // Используем битовые операции
  // $01 - устновлен Bold, $02 - устновлен Italic,
  // $04 - устновлен Underline, $08 - устновлен StrikeOut
  // if Value and (1 shl N) <> 0 then ... установлен
  // if Value and (1 shl N) = 0 then ... не установлен
    newBold = (FormatStyle->is_bold & 0x01) != 0;
    newItalic = (FormatStyle->is_bold & 0x02) != 0;
    newUnderline = (FormatStyle->is_bold & 0x04) != 0;
    newStrikeout = (FormatStyle->is_bold & 0x08) != 0;

    cf.FontBold_Set(newBold);
    cf.FontItalic_Set(newItalic);
    cf.FontUnderline_Set(newUnderline);
    cf.FontStrikeout_Set(newStrikeout);

    if (FormatStyle->h > 0)
        newSize = FormatStyle->h;
    else
        newSize = fSize;
    cf.FontSize_Set(newSize);
    if (AtrRow != NULL && AtrRow->FormatStyle.h != 0 && AtrRow->FormatStyle.h > newSize)
        cf.FontSize_Set(AtrRow->FormatStyle.h);

    // установка цвета
    cf.FontColor_Set(FormatStyle->FontColor);
    if (AtrRow != NULL && AtrRow->FormatStyle.FontColor != 0)
        cf.FontColor_Set(AtrRow->FormatStyle.FontColor);

    if (FormatStyle->fonColor != 16777215 && FormatStyle->fonColor != 0)
    {
        cf.PatternBG_Set(FormatStyle->fonColor);
        cf.PatternStyle_Set( 0); // приходится ставить
        cf.PatternFG_Set(cf.PatternBG_Get());
    }
    if (AtrRow != NULL && AtrRow->FormatStyle.fonColor != 16777215 && AtrRow->FormatStyle.fonColor != 0)
    {
        cf.PatternBG_Set(AtrRow->FormatStyle.fonColor);
        cf.PatternStyle_Set( 0); // приходится ставить
        cf.PatternFG_Set(cf.PatternBG_Get());
    }

    // установка границ ячеек
    if (FormatStyle->borderStyle != 0)
    {
        cf.BorderStyle_Set(static_cast<F1BorderConstants>(cf.BorderStyle[0]), static_cast<F1BorderStyleConstants>(FormatStyle->borderStyle));
        cf.BorderStyle_Set(static_cast<F1BorderConstants>(cf.BorderStyle[1]), static_cast<F1BorderStyleConstants>(FormatStyle->borderStyle));
        cf.BorderStyle_Set(static_cast<F1BorderConstants>(cf.BorderStyle[2]), static_cast<F1BorderStyleConstants>(FormatStyle->borderStyle));
        cf.BorderStyle_Set(static_cast<F1BorderConstants>(cf.BorderStyle[3]), static_cast<F1BorderStyleConstants>(FormatStyle->borderStyle));
    }
    if (FormatStyle->borderColor != 0)
    {
        cf.BorderColor_Set(static_cast<F1BorderConstants>(cf.BorderColor[0]), FormatStyle->borderColor);
        cf.BorderColor_Set(static_cast<F1BorderConstants>(cf.BorderColor[1]), FormatStyle->borderColor);
        cf.BorderColor_Set(static_cast<F1BorderConstants>(cf.BorderColor[2]), FormatStyle->borderColor);
        cf.BorderColor_Set(static_cast<F1BorderConstants>(cf.BorderColor[3]), FormatStyle->borderColor);
    }

    if (DataType == 6)
    {
        cf.FontColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(0, 0, 255));
        cf.FontUnderline_Set( true);
    }
    if (AtrRow != NULL && AtrRow->FormatStyle.is_bold != 0)
    {
        if (AtrRow->FormatStyle.is_bold & 0x01)
            cf.FontBold_Set(true);
        if (AtrRow->FormatStyle.is_bold & 0x02)
            cf.FontItalic_Set(true);
        if (AtrRow->FormatStyle.is_bold & 0x04)
            cf.FontUnderline_Set( true);
        if (AtrRow->FormatStyle.is_bold & 0x08)
            cf.FontStrikeout_Set(true);
    }

    if (newName == "Calibri")
        newName = FontArial;
    if (newName == "Arial")
        newName = FontArial;
    cf.FontName_Set(newName);

    if (AtrRow != NULL && AtrRow->FormatStyle.font != "")
        cf.FontName_Set(AtrRow->FormatStyle.font);

    if (Value.length() > 1 && Value.substr(0, 1) == "^") // это формула F1
    {
        try
        {
            string wsString = Value.substr(1, 10000);
            std::u16string string16 = utf8_to_utf16(wsString);
            const char16_t * wshar16_t = string16.c_str();

            aF1Book->Formula_Set(wshar16_t);
            if (aF1Book->Type_Get() == -2 && p->ListStmnt[aCol1].Stmnt != "ПРОИЗВСТОЛБ" && !aF1Book->ColHidden_Get(aCol1))
                cf.WordWrap_Set(true);
        }
        catch (exception& E)
        {
            TracerManager::Instance().AddSpanLog("Exception.TForm1::SetCellValue", {{"errordesr1", E.what()},
                                                                                    {"errordesr2", "Ошибка в формуле"}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            //s = "Форма: " + aF1Book->SheetName_Get(aF1Book->Sheet) + ", строка " + IntToStr(aRow1) + ", столбец " + IntToStr(aCol1) +
            //    ". Ошибка в формуле. Информация об ошибке: " + E.Message;
            //throw Exception(s + ". Неверная формула");
        }
    }
    else
    {
        SetCellValue_Obnovl(p, FormLogicF1, Value, DataType, aRow1, aCol1);
    }

    if (p->work_with != 2)
    {
        cf.SetCellFormat(*FormLogicF1->f1Forms);
    }
}



void TForm1::SetCellValue_Obnovl(TwwStringList* p, TFormLogicF1* FormLogicF1, std::string Value, int aDataType, int nRow, int nCol) {
    double wd;
    int DataType, pos;
    TF1FlexCel* F1Book = FormLogicF1->f1Forms;

    DataType = aDataType;
	if (aDataType < 0) {
    DataType = -aDataType; // отрицательный тип значения посылается, чтобы указать, что в БД значение не найдено
	}
    if (int N = DataType; N== 1||N==10||N == 11 + deltaDType || N == 12 + deltaDType ||
      N == 13 + deltaDType || N == 14 + deltaDType || 
      (N >= 15 + deltaDType && DataType <= 29 + deltaDType) || 
      (N >= 31 + deltaDType && N <= 32 + deltaDType) || 
      (N >= 40 + deltaDType && N <= 45 + deltaDType) || 
       N == 51 + deltaDType || N == 53 + deltaDType || 
      (N >= 58 + deltaDType && N <= 64 + deltaDType)) 
	{
        if (Value.empty()) {
            switch (p->isShowZero) {
            case 0:
                F1Book->EnableProtection = false;
                //++mak F1Book->ClearRange(nRow, nCol, nRow, nCol, F1ClearValues);
                F1Book->EnableProtection = true;
                break;
            case 1:
                if (aDataType > 0) {//{значение в БД есть}
                    F1Book->NumberRC_Set(nRow, nCol, 0);
                } else {
                    F1Book->EnableProtection = false;
                    F1Book->ClearRange(nRow, nCol, nRow, nCol, F1ClearValues);
                    F1Book->EnableProtection = true;
                }
                break;
            case 2:
                F1Book->NumberRC_Set(nRow, nCol, 0.000000000000000000001);
                break;
            }
        } else {
            try {
                wd = StrToFloat(Value);
                if (SameValue(wd, 0, 0.000001)) {
                    wd = 0;
                }
                F1Book->NumberRC_Set(nRow, nCol, wd);
            } catch (std::exception &e) {
                TracerManager::Instance().AddSpanLog("Exception.TForm1::SetCellValue_Obnovl", {{"errordesr", e.what()}},
                                                     TracerManager::LOG_LEVEL_ERROR);
            }
        }
    } else {
        if (DataType == 5) { //{Дата}
            pos = Pos(" 00:00:00", Value);
            if (pos >= 0) {
                Value = Value.substr(0, pos);
            }
            TForm1::DateFormatir(p, Value, nRow, nCol);
        }
        if (!Value.empty()) {
            if (Value.find("{\rtf") == 0) {
                //mak??? Value = TConvRTFtoHTML::doConvRTFtoHTML(Value);
                //mak??? F1Book->xls.SetCellFromHtml(nRow, nCol, Value);
            } else {

                std::u16string wst = stringToChar16(Value);
                const char16_t *char16Ptr = wst.c_str();


                F1Book->TextRC_Set(nRow, nCol, char16Ptr);
            }
        } else {
            F1Book->EnableProtection = false;
            F1Book->ClearRange(nRow, nCol, nRow, nCol, F1ClearValues);
            F1Book->EnableProtection = true;
        }
    }
}

void TForm1::DateFormatir(TwwStringList* p, std::string& Value, int nRow, int nCol) {
    
    PAtrStmnt AtrStmnt;
    std::string ws, st;

    auto yyyy_mm_dd = [&]() {
        ws = "..........";
        ws[0] = Value[6];
        ws[1] = Value[7];
        ws[2] = Value[8];
        ws[3] = Value[9];
        ws[5] = Value[3];
        ws[6] = Value[4];
        ws[8] = Value[0];
        ws[9] = Value[1];
    };

    auto hh_mm = [&]() {
        if (Value.length() <= 10)
            st = " 00:00";
        else
            st = " " + Value.substr(11, 5);
    };

    if (Value.empty())
        return;

    AtrStmnt = &(p->ListStmnt[nCol]);
    if (!(AtrStmnt->tocnost == 4 || AtrStmnt->tocnost == 5)) {
        if (Value.length() < 10)
            return;
    }
    switch (AtrStmnt->tocnost) {
    case 0:
        return;
    case 1: // dd.mm.yy
        Value = Value.substr(0, 8);
        break;
    case 2: //// yyyy.mm.dd
        yyyy_mm_dd();
        Value = ws;
        break;
    case 3: // yy.mm.dd
        ws = "........";
        ws[0] = Value[6];
        ws[1] = Value[7];
        ws[3] = Value[3];
        ws[4] = Value[4];
        ws[6] = Value[0];
        ws[7] = Value[1];
        Value = ws;
        break;
    case 4:
        ws = Value.substr(0, 10);
        hh_mm();
        Value = ws + st;
        break;
    case 5:
        yyyy_mm_dd();
        hh_mm();
        Value = ws + st;
        break;
    }
}

























