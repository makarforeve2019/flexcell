#include "SystFuncsLogic.h"
#include "FormDopInp.h"
#include "Form2Plus.h"
#include "FormGF_Logic.h"
#include "DB_Plan.h"
#include "DB_IstFin.h"
#include "DB_GurnProtokol.h"
#include "DB_CellValue.h"
#include "unEditValidation.h"

// Выполняются следующие действия:
// 1. Проверяет корректность (валидность) представленного данного AEditString
// 2. Если данное некорректно(невалидно), то
//    - выдаются все необходимые диагностические сообщения
//    - возвращается результат false
// Если исходная строка в процессе проверки было изменена (вместо пустой поставлен "0"), то она возвращается
bool TEditValidation::doEditValidation(int ASheet, int ARow, int ACol, std::string& AEditString, TFormLogicF1* FormLogicF1, std::string& aMsg, int& nNextRow, int& nNextCol) {
    TEditValidation EditValidation;
    EditValidation.Init(ASheet, ARow, ACol, AEditString, FormLogicF1);
    if (EditValidation.CheckValue()) {
        if (AEditString != EditValidation.EditString) {
            AEditString = EditValidation.EditString;
        }
        EditValidation.getActivateNextCell(nNextRow, nNextCol);
        return true;
    } else {
        aMsg = EditValidation.MessageText;
        return false;
    }
}

TEditValidation::TEditValidation() {
    CellAttributes = nullptr;

}

void TEditValidation::Init(int ASheet, int ARow, int ACol, string AEditString, TFormLogicF1* FormLogicF1)
{
    Sheet = ASheet;
    Row = ARow;
    Col = ACol;
    EditString = AEditString;
    this->FormLogicF1 = FormLogicF1;
    CellAttributes = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, Row, Col);
}

bool TEditValidation::CheckValue()
{
    bool Result = false;
    if (isGraphicFinans())
    {
        Result = CheckGraphicFinansValue();
    }
    else
    {
        Result = CheckDaleeValue(); // продолжаем проверять дальше
    }
    return Result;
}

// true - если ввод в ячейку графика финансирования
bool TEditValidation::isGraphicFinans()
{
    return CellAttributes->DataType == 30 + deltaDType;
}

// проверяем ячейку с графиком финансирования
bool TEditValidation::CheckGraphicFinansValue()
{
    bool Result = false;
    EditString = ShieldEmptyValue(EditString);
    Result = true;
    return Result;
}

// если строка пустая, то предполагаем '0'
string TEditValidation::ShieldEmptyValue(const string& AValue)
{
    string Result;
    if (AValue.length() == 0)
    {
        Result = "0";
    }
    else
    {
        Result = AValue;
    }
    return Result;
}

// продолжаем проверять дальше
bool TEditValidation::CheckDaleeValue()
{
    bool Result = false;
    TwwStringList* FormAttributes = TFormDopInp::GetTww(FormLogicF1);

    TValidator Validator = TValidator(*FormAttributes, Col);
    switch (CellAttributes->pokaz_type)
    {
        case 1:
        {
            Result = CheckSpecificNumberValue(Validator);
            break;
        }
        case 2:
        {
            Result = CheckSpecificTextValue(Validator);
            break;
        }
        default:
        {
            Result = true;
            break;
        }
    }

    return Result;
}

// проверка валидности числа
bool TEditValidation::CheckSpecificNumberValue(TValidator AValidator)
{
    bool Result = false;
    if ((CellAttributes->VidOtch == 4) && (CellAttributes->ObjectId == 0))
    {
        Result = CheckRevisedPlan();
        if (!Result)
        {
            return Result;
        }
    }
    double EditFloatValue;
    if (GetValidatedFloatValue(EditFloatValue))
    {
        Result = AValidator.doProvCisla(EditFloatValue, MessageText, FormLogicF1, CellAttributes->Row, CellAttributes->Col);
    }
    else
    {
        Result = false;
    }
    if (!Result)
    {
        return Result;
    }
    if (CellAttributes->DataType == (31 + deltaDType))
    {
        Result = CheckCashPlan();
    }
    return Result;
}

// в ячейку введен уточненный план - это недопустимо
// поэтому выдается предупреждение, что изменения делаются через перемещения
bool TEditValidation::CheckRevisedPlan()
{
    bool Result = false;
    if (!TDB_Plan::IsRazreseno(CellAttributes->PlanId, CellAttributes->ist_fin_id))
    {
        switch (CellAttributes->ist_fin_id)
        {
            case ist_fin_id_CBRTOkruga:
            {
                MessageText = "Уточненный план рекомендуется изменять с помощью функций из меню \"Из��енения уточ.плана\".";
                break;
            }
            case ist_fin_id_TrBudg:
            {
                MessageText = "Уточненный план возможно изменять только с помощью функций из меню \"Перемещения\"/\"Изменения уточ.плана\".";
                break;
            }
        }
        MessageText += "\n[После закрытия данного окна для анулирования введен��ого числа следует использовать клавишу Esc]";
    }
    else
    {
        Result = true;
    }
    return Result;
}

// строку преобразуем в Double, заодно проверяем корректность строки
bool TEditValidation::GetValidatedFloatValue(double& FloatValue)
{
    bool Result = false;
    EditString = ShieldEmptyValue(EditString);
    if (EditString.length() <= 17)
    {
        EditString = AnsiReplaceStr(EditString, ".", ",");
        if (TryStrToFloat(EditString, FloatValue))
        {
            FloatValue = std::stod(EditString);
            Result = true;
        }
        else
        {
            MessageText = "Неверное числовое значение";
        }
    }
    else
    {
        MessageText = "Превышена максимальная длина числа";
    }
    return Result;
}

// проверка допустимости уточнения кассового плана
bool TEditValidation::CheckCashPlan()
{
    bool Result = false;
    EditString = ShieldEmptyValue(EditString);
    Result = true;
    return Result;
}

// проверка валидности строки
bool TEditValidation::CheckSpecificTextValue(TValidator AValidator)
{
    bool Result = AValidator.doProvStr(EditString, MessageText);
    return Result;
}

TValidator::TValidator(TwwStringList& FormAttributes, int ACol)
{
    nValidator = FormAttributes.ListStmnt[ACol].nValidator;
    vid_uslovij = FormAttributes.ListStmnt[ACol].vid_uslovij;
    Value1 = FormAttributes.ListStmnt[ACol].Value1;
    Value2 = FormAttributes.ListStmnt[ACol].Value2;
    Shablon = FormAttributes.ListStmnt[ACol].Shablon;
    Msg = FormAttributes.ListStmnt[ACol].Msg;
}

TValidator::TValidator(short nValidator_, short vid_uslovij_, double Value1_, double Value2_,
                       std::string Shablon_, std::string Msg_)
{
    nValidator = nValidator_;
    vid_uslovij = vid_uslovij_;
    Value1 = Value1_;
    Value2 = Value2_;
    Shablon = Shablon_;
    Msg = Msg_;
}

// проверка на корректность указанного числа
bool TValidator::doProvCisla(double d, string& MessageText, TFormLogicF1* FormLogicF1, int nRow, int nCol)
{
    bool Result = true;
    switch (nValidator)
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            switch (vid_uslovij)
            {
                case 1:
                {
                    if (d != Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 2:
                {
                    if (d == Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 3:
                {
                    if (d >= Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 4:
                {
                    if (d > Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 5:
                {
                    if (d <= Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 6:
                {
                    if (d < Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 7:
                {
                    if ((d < Value1) || (d > Value2))
                    {
                        Result = false;
                    }
                    break;
                }
                case 9:
                case 10:
                {
                    bool Cancel = false;
                    if (!checkShablon(nRow, nCol, FloatToStr(d), FormLogicF1, Msg, Cancel))
                    {
                        Msg = "";
                        Result = false;
                    }
                    else
                    {
                        if (Cancel)
                        {
                            Result = false;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 2:
        case 3:
        {
            MessageText = "Для числового значения проверка задана как ��ля строки";
            break;
        }
    }
    if (!Result)
    {
        MessageText = Msg;
    }
    return Result;
}

bool TValidator::doProvStr(string str_value, string& MessageText)
{
    bool Result = true;
    switch (nValidator)
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            MessageText = "Для строкового значения проверка задана как для числа";
            break;
        }
        case 2:
        {
            int L = str_value.length();
            switch (vid_uslovij)
            {
                case 1:
                {
                    if (L != Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 2:
                {
                    if (L == Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 3:
                {
                    if (L >= Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 4:
                {
                    if (L > Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 5:
                {
                    if (L <= Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 6:
                {
                    if (L < Value1)
                    {
                        Result = false;
                    }
                    break;
                }
                case 7:
                {
                    if ((L < Value1) || (L > Value2))
                    {
                        Result = false;
                    }
                    break;
                }
            }
            break;
        }
        case 3:
        {
            //mak ??? Result = TRegExprMy::doProvStr(Shablon, str_value);
            break;
        }
    }
    if (!Result)
    {
        MessageText = Msg;
    }
    return Result;
}

bool TEditValidation::getActivateNextCell(int& nRow, int& nCol)
{
    if (FindNextAvailCell(nRow, nCol))
    {
        return true;
    }
    else
    {
        nRow = 0;
        nCol = 0;
        return false;
    }
}

bool TEditValidation::FindNextAvailCell(int& nRow, int& nCol)
{
    int TempRow = Row;
    int TempCol = Col;
    while (FindNextCell(TempRow, TempCol))
    {
        if (!CellLocked(TempRow, TempCol) && CellVisible(TempRow, TempCol))
        {
            nRow = TempRow;
            nCol = TempCol;
            return true;
        }
    }
    return false;
}

bool TEditValidation::FindNextCell(int& nRow, int& nCol)
{
    if (RowExist(nRow + 1))
    {
        nRow = nRow + 1;
        return true;
    }
    else if (ColExist(nCol + 1))
    {
        nRow = 1;
        nCol = nCol + 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool TEditValidation::CellLocked(int nRow, int nCol)
{
    int r1, c1, r2, c2;
    bool pLocked, pHidden;
    FormLogicF1->f1Forms->Selection_Get(0, r1, c1, r2, c2);
    FormLogicF1->f1Forms->Selection_Set(nRow, nCol, nRow, nCol);
    FormLogicF1->f1Forms->Protection_Get(pLocked, pHidden);
    FormLogicF1->f1Forms->Selection_Set(r1, c1, r2, c2);
    return pLocked;
}

bool TEditValidation::CellVisible(int nRow, int nCol)
{
    return !FormLogicF1->f1Forms->ColHidden[nCol] && !FormLogicF1->f1Forms->RowHidden[nRow];
}

bool TEditValidation::RowExist(int nRow)
{
    return (nRow <= FormLogicF1->f1Forms->LastRow) && (nRow > 0);
}

bool TEditValidation::ColExist(int nCol)
{
    return (nCol <= FormLogicF1->f1Forms->LastCol) && (nCol > 0);
}

bool TValidator::checkShablon(int ARow, int ACol, string AEditString, TFormLogicF1* FormLogicF1, string& vMsg, bool& Cancel)
{
    double d = std::stod(AEditString);
    TwwStringList* FormAttributes = TFormDopInp::GetTww(FormLogicF1);
    TValidator wValidator = TValidator(*FormAttributes, ACol);
    string wShablon = wValidator.Shablon;
    Cancel = false;
    bool Result = true;
    if (wShablon != "")
    {
        string wMsg = wValidator.Msg;
        AEditString = AnsiReplaceStr(AEditString, ",", "."); // 05.05.2022
        wShablon = StringReplace(wShablon, ("{Value}"), AEditString);
        AEditString = FloatToStr(d - TFormLogicF1(*FormLogicF1).f1Forms->NumberRC(ARow,ACol)); // 05.05.2022
        AEditString = AnsiReplaceStr(AEditString, ",", ".");
        wShablon = StringReplace(wShablon, ("{Delta}"), AEditString);
        if (wShablon.find("</") != std::string::npos)
        {
            for (int index = 1; index <= 12; index++)
            {
                int posBeginSb = wShablon.find("<" + std::to_string(index) + ">") + 3;
                int posEndSb = wShablon.find("</" + std::to_string(index) + ">");
                int posBeginMsg = wMsg.find("<" + std::to_string(index) + ">") + 3;
                int posEndMsg = wMsg.find("</" + std::to_string(index) + ">");
                string IndShablon = wShablon.substr(posBeginSb, posEndSb - posBeginSb);
                if (IndShablon == "")
                {
                    continue;
                }
                IndShablon = TFormDopInp::ReplaceNRowNCol(ARow, ACol, IndShablon);
                IndShablon = StringReplace(IndShablon, ("^"), "");
                string IndMsg = wMsg.substr(posBeginMsg, posEndMsg - posBeginMsg);
                try
                {
                    //mak ??? double resultIF = TFormLogicF1(*FormLogicF1).f1Forms->xls->RecalcExpression("=" + IndShablon);
                    double resultIF = 0;
                    if (resultIF == 0)
                    {
                        Cancel = true;
                        vMsg = IndMsg;
                        return Result;
                    }
                }
                catch (...)
                {
                    Result = false;
                    return Result;
                }
            }
        }
        else
        {
            try
            {
                wShablon = TFormDopInp::ReplaceNRowNCol(ARow, ACol, wShablon);
                wShablon = StringReplace(wShablon, ("^"), "");
                double resultIF = 0;//mak ??? = TFormLogicF1(FormLogicF1)->f1Forms->Xls->RecalcExpression("=" + wShablon);
                if (resultIF == 0)
                {
                    Cancel = true;
                    vMsg = wMsg;
                    return Result;
                }
            }
            catch (...)
            {
                Result = false;
                return Result;
            }
        }
    }
    return Result;
}



