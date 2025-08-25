#ifndef unXlsToStrJSONH
#define unXlsToStrJSONH

#include <stdint-gcc.h>
#include "SystFuncsAltWin.h"
#include "unFormLogicF1.h"
#include "FormTypes.h"
#include "FormDopInp.h"
#include "Form2Plus.h"
#include "unTF1CellFormat.h"
#include "unXlsToStrJSON.h"
#include "SystFuncsLogic.h"

bool TXlsToStrJSON::doXlsToStrJSON(TFormLogicF1* FormLogicF1_, int nSheet, json &serializer) {
    TracerManager::Instance().StartFunctionTrace("TXlsToStrJSON::doXlsToStrJSON");
    TFormLogicF1* FormLogicF1 = FormLogicF1_;
    TXlsToStrJSON* XlsToStrJSON = new TXlsToStrJSON();
    XlsToStrJSON->FormLogicF1 = FormLogicF1;
    XlsToStrJSON->f1Flex = FormLogicF1->f1Forms;

    bool Result = true;
    XlsToStrJSON->doStmntInsSheet(serializer, nSheet);
    delete XlsToStrJSON;
    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

TXlsToStrJSON::TXlsToStrJSON() {

}

TXlsToStrJSON::~TXlsToStrJSON() {
    ClearCommonStyles();
    NewStyleListClear();
}

void TXlsToStrJSON::ClearCommonStyles() {
    CommonStyleList.Clear();
}

void TXlsToStrJSON::NewStyleListClear() {
    NewStyleList.Clear();
}



void TXlsToStrJSON::doStmntInsSheet(nlohmann::json& aSheet, int nSheet) {
    InsertSheet(aSheet, nSheet);
}

void TXlsToStrJSON::InsertSheet(nlohmann::json& aSheet, int nSheet) {
    NewStyleListClear();

    f1Flex->Sheet_Set(nSheet);
    f1Flex->Recalc();

    nlohmann::json sheet = nlohmann::json::object();
    Parse_Sheet(sheet);
    //std::cout << "���������� sheet: " << sheet.dump(4) << std::endl;
    aSheet["Sheet"] = sheet;

    if (NewStyleList.Count() > 0) {
        nlohmann::json namedStyles = nlohmann::json::array();
        Parse_NamedStyles(namedStyles);
        aSheet["NamedStyles"] = namedStyles;

    }
}

void TXlsToStrJSON::Parse_Sheet(nlohmann::json& sheet) {
    nlohmann::json data;
    nlohmann::json rows, columns, buttons;
    buttons = nullptr;
    int wColWidthUnits;
    TRange ARange;
    int wi;



    ARange.nStartRow = 1;
    ARange.nStartCol = 1;
    ARange.nEndRow = f1Flex->MaxRow;
    wi = f1Flex->MaxRow_Get();
    ARange.nEndCol = f1Flex->MaxCol;

    std::string sheetName = char16ToUTF8(f1Flex->SheetName[f1Flex->Sheet]);

    sheet["name"] = sheetName;
    wi = f1Flex->MaxRow;
    sheet["rowCount"] = wi;
    wi = f1Flex->MaxCol;
    sheet["colCount"] = wi;

    sheet["selectedRow"] = f1Flex->Row - 1;




    if (f1Flex->Col > f1Flex->MaxRow) {
        sheet["selectedCol"] = f1Flex->MaxRow - 1;
    } else {
        sheet["selectedCol"] = f1Flex->Col - 1;
    }
    TwwStringList* p = FormLogicF1->FormsValuesArray->GetItem(f1Flex->Sheet - 1);
    if (p->form_begBodyRow-1 > 20) {
        wi = 0;
    } else {
        wi = p->form_begBodyRow-1;
    }
    sheet["frozenRowCount"] = wi;

    wi = p->is_noout_nstr;
    sheet["frozenColCount"] = wi;



    sheet["headerRowBegin"] = p->DeltaRow+1;
    sheet["headerRowEnd"] = p->form_begBodyRow-1;

    Parse_Rows(ARange, rows);
    sheet["rows"] = rows;

    Parse_Columns(ARange, columns);
    sheet["columns"] = columns;

    Parse_Buttons(buttons);
    sheet["buttons"] = buttons;


    Parse_Data(ARange, data);
    sheet["data"] = data;
    sheet["isProtected"] = true;

    sheet["index"] = f1Flex->Sheet - 1;
}

void TXlsToStrJSON::Parse_Rows(TRange& ARange, json& rows) {
    int dRH = 15;
    dRH = std::round(dRH / RowMult);

    for (int i = 1; i <= f1Flex->MaxRow; ++i) {
        nlohmann::json Element = nlohmann::json::object();
        if (f1Flex->RowHeight[i] == dRH){
            //mak ???f1Flex->AutofitRow(i, true, 1);
        }

        int height = f1Flex->RowHeight[i];
        if (height > 300) {
            height = 15;
        }
        Element["size"] = height;
        if (f1Flex->RowHidden[i])
            Element["visible"] = false;
        rows.push_back(Element);
    }
}

void TXlsToStrJSON::Parse_Buttons(json& buttons) {
    int objlistCount = f1Flex->objlistCount_Get();

    for (int i = 0; i < objlistCount; i++) {
        int nRow = 0, nCol = 0;
        std::string buttonText;
        const char16_t * buttonTextChar16 = u"";
        f1Flex->objlistI_PropRC_Get(i, nRow, nCol, (char16_t &)buttonTextChar16);
        buttonText = utf16_to_utf8(buttonTextChar16);
        buttonText = buttonText.substr(1, buttonText.length());

        json buttonElement = {
            {"row", nRow - 1},
            {"col", nCol - 1},
            {"text", buttonText}
        };
        buttons.push_back(buttonElement);
    }
}

void TXlsToStrJSON::Parse_Columns(TRange& ARange, json& columns){
    for (int i = 1; i <= f1Flex->LastCol; i++) {
        nlohmann::json Element = nlohmann::json::object();
        int ColWidth = f1Flex->ColWidth[i] + this->COL_WIDTH_WEB_DIFF;
        Element["size"] = ColWidth;
        if (f1Flex->ColHidden[i]) {
            Element["visible"] = false;
        }
        columns.push_back(Element);
    }
}

void TXlsToStrJSON::Parse_Data(TRange& ARange, json& data) {
    nlohmann::json dataTable = nlohmann::json::array();

    Parse_DataTable(ARange, dataTable);
    data["dataTable"] = dataTable;
    int wi = 1;
}

#include <cmath>

double formatNumberDouble(double value, int order) {
    int precision;

    // Устанавливаем точность на основе переданного порядкового номера
    switch (order) {
        case 0:  // По умолчанию
            precision = 1;
            break;
        case 1:  // Как целое
            precision = 0;
            break;
        case 2:  // До 1-й десятой
            precision = 1;
            break;
        case 3:  // До 2-х десятых
            precision = 2;
            break;
        case 4:  // До 3-х десятых
            precision = 3;
            break;
        case 5:  // До 4-х десятых
            precision = 4;
            break;
        case 6:  // До 5-и десятых
            precision = 5;
            break;
        case 7:  // До 6-и десятых
            precision = 6;
            break;
        case 8:  // До 7-и десятых
            precision = 7;
            break;
        case 9:  // До 8-и десятых
            precision = 8;
            break;
        case 10: // До 9-и десятых
            precision = 9;
            break;
        case 11: // До 2-х дес. - всегда 2 цифры
            precision = 2;
            break;
        case 12: // До 3-х дес. - всегда 3 цифры
            precision = 3;
            break;
        default:
            precision = 1;  // По умолчанию точность - 1 знак
            break;
    }

    // Округляем до нужной точности
    if (precision == 0) {
        return std::round(value);  // Округление до целого числа
    }

    // Используем строковый поток для точного форматирования
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;

    // Преобразуем обратно в double для возврата
    return std::stod(out.str());
}

std::string formatNumberString(double value, int order) {
    int precision;

    // Устанавливаем точность на основе переданного порядкового номера
    switch (order) {
        case 0:  // По умолчанию
            precision = 1;
            break;
        case 1:  // Как целое
            precision = 0;
            break;
        case 2:  // До 1-й десятой
            precision = 1;
            break;
        case 3:  // До 2-х десятых
            precision = 2;
            break;
        case 4:  // До 3-х десятых
            precision = 3;
            break;
        case 5:  // До 4-х десятых
            precision = 4;
            break;
        case 6:  // До 5-и десятых
            precision = 5;
            break;
        case 7:  // До 6-и десятых
            precision = 6;
            break;
        case 8:  // До 7-и десятых
            precision = 7;
            break;
        case 9:  // До 8-и десятых
            precision = 8;
            break;
        case 10: // До 9-и десятых
            precision = 9;
            break;
        case 11: // До 2-х дес. - всегда 2 цифры
            precision = 2;
            break;
        case 12: // До 3-х дес. - всегда 3 цифры
            precision = 3;
            break;
        default:
            precision = 1;  // По умолчанию точность - 1 знак
            break;
    }

    // Используем строковый поток для форматирования значения с нужной точностью
    std::ostringstream out;
    if (precision == 0) {
        out << std::fixed << std::setprecision(0) << std::round(value);  // Округление до целого числа
    } else {
        out << std::fixed << std::setprecision(precision) << value;
    }

    // Возвращаем строку с результатом
    return out.str();
}


bool TXlsToStrJSON::Parse_DataTable(TRange& ARange, json& dataTable) {
    bool result = false;
    std::string nf;
    int tocnost = 0;
    auto utocFormat = [this, &nf, &tocnost](int aRow, int aCol) -> bool {
        nf.clear();
        bool result = false;

        TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);


        auto AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow);
        if (AtrRow != nullptr) {
            tocnost = AtrRow->tocnost;
        }

        if (tocnost == 0) {
            tocnost = p->ListStmnt[aCol].tocnost;
        }

        if (tocnost == 0) {
            tocnost = p->tocnost;
        }

        if (tocnost > 0) {
            nf = TForm2Plus::CreateNumberFormat(tocnost, p->ListStmnt[aCol].CoefPlus1);
            result = true;
        }
        return result;
    };

    for (int r = ARange.nStartRow; r <= ARange.nEndRow; ++r) {
        json oRow = json::array();

        for (int c = ARange.nStartCol; c <= ARange.nEndCol; ++c) {
            f1Flex->Selection_Set(r, c, r, c);

            const char16_t * sName16 = FormLogicF1->f1Forms->StyleNameRC_Get(r, c);

            std::u16string str(sName16);

            std::string sName = utf16_to_utf8(str);

//            TStyle* AStyle = new TStyle();
            std::shared_ptr<TStyle> AStyle = std::make_shared<TStyle>(FormLogicF1, f1Flex, r, c);

            if (AStyle->Formatter() == "General") {
                if (utocFormat(r, c)) {
                    AStyle->Formatter() = nf;
                }
            }
            AStyle->Name() = sName;

            int index = -1;
            if (!CommonStyleList.Find(AStyle->Name(), index)) {
                CommonStyleList.Add(AStyle->Name());
                NewStyleList.AddObject(AStyle->Name(), AStyle);
            }

            json oCol = json::object();

            switch (FormLogicF1->f1Forms->TypeRC_Get(r, c)) {
                case 0:

                   oCol["value"] = nullptr;
                    break;
                case 1:
                case -1: {
//                    oCol["value"] = f1Flex->TextRC_Get(r,c);
//                    double valuedbl = ;
                    oCol["value"] = FloatToStr(f1Flex->NumberRC_Get(r, c), FormatWithPrecision(tocnost));
                    break;
                }
                case 2:
                case -2:

                    oCol["value"] = char16ToUTF8(f1Flex->TextRC_Get(r,c));
                    break;
                default:
                    oCol["value"] = char16ToUTF8(f1Flex->TextRC(r,c));
                    break;
            }

            oCol["style"] = sName;
            if (!result && f1Flex->MergeCells) {
                if (r == f1Flex->SelStartRow && c == f1Flex->SelStartCol) {
                    oCol["rowSpan"] = f1Flex->SelEndRow - r + 1;
                    oCol["colSpan"] = f1Flex->SelEndCol - c + 1;
                } else {
                    oCol["inSpan"] = "true";
                }
            }
            oRow.push_back(oCol);
        }

        dataTable.push_back(oRow);
    }
    return result;

}

void TXlsToStrJSON::Parse_NamedStyles(json& namedStyles) {
    int index;
    for (int i = 0; i < NewStyleList.Count(); ++i) {
        std::string name = NewStyleList[i];
        NewStyleList.Find(name, index);

        // Прямое обращение к указателю без создания wA
        if (std::shared_ptr<TStyle> style = std::get<std::shared_ptr<TStyle>>(NewStyleList.Objects[i]); style) {
            // Создаем JSON-объект, используя данные из style напрямую
            json Element = json::object();
            Element["backColor"] = style->BackColor() == "null" ? "#ffffff" : (style->BackColor() == "#000000" ? "#ffffff" : style->BackColor());
            Element["fontColor"] = style->FontColor() == "null" ? "#ffffff" : style->FontColor();
            Element["hAlign"] = style->HAlign();
            Element["vAlign"] = style->VAlign();
            Element["fontName"] = style->FontName();
            Element["fontSize"] = style->FontSize();
            Element["formatter"] = style->Formatter();

            if (style->BorderLeft().Style() == TLineStyle::Empty) {
                Element["borderLeft"] = nullptr;
            } else {
                json wo = json::object();
                wo["color"] = style->BorderLeft().Color();
                wo["style"] = style->BorderLeft().Style();
                Element["borderLeft"] = wo;
            }

            if (style->BorderTop().Style() == TLineStyle::Empty) {
                Element["borderTop"] = nullptr;
            } else {
                json wo = json::object();
                wo["color"] = style->BorderTop().Color();
                wo["style"] = style->BorderTop().Style();
                Element["borderTop"] = wo;
            }

            if (style->BorderRight().Style() == TLineStyle::Empty) {
                Element["borderRight"] = nullptr;
            } else {
                json wo = json::object();
                wo["color"] = style->BorderRight().Color();
                wo["style"] = style->BorderRight().Style();
                Element["borderRight"] = wo;
            }

            if (style->BorderBottom().Style() == TLineStyle::Empty) {
                Element["borderBottom"] = nullptr;
            } else {
                json wo = json::object();
                wo["color"] = style->BorderBottom().Color();
                wo["style"] = style->BorderBottom().Style();
                Element["borderBottom"] = wo;
            }

            Element["wordWrap"] = style->WordWrap();
            Element["fontBold"] = style->FontBold();
            Element["fontItalic"] = style->FontItalic();
            Element["fontUnderline"] = style->FontUnderline();
            Element["name"] = style->Name();

            // Добавляем JSON в список namedStyles
            namedStyles.push_back(Element);
        }
    }
}

void TXlsToStrJSON::doStmntCellValue(json& aCellValue) {

    int NSheet = aCellValue.at("Sheet").get<int>();

    auto RowArr = aCellValue.at("Rows");
    auto ColArr = aCellValue.at("Cols");
    auto TypeArr = aCellValue.at("Types");


    if (RowArr.empty()) {
        return;
    }

    // ������� ������� ��� �������� � ������
    json ValuesArr = json::array();
    aCellValue["Values"] = ValuesArr;

    json StylesArr = json::array();
    aCellValue["Styles"] = StylesArr;

    // ������������� �������� ����
    f1Flex->Sheet = NSheet;

    // ������������ ������ ������� �������
    for (size_t i = 0; i < RowArr.size(); ++i) {
        int nRow = RowArr[i].get<int>();
        int nCol = ColArr[i].get<int>();
        int Type_ = TypeArr[i].get<int>();

        f1Flex->Selection_Set(nRow, nCol, nRow, nCol);

        // ��������� ��� �����
        std::string sName = char16ToUTF8(FormLogicF1->f1Forms->StyleNameRC_Get(nRow, nCol));
        StylesArr.push_back(sName);

        // ��������� ������ �������� � ����������� �� ����
        switch (Type_) {
            case 1: {
                ValuesArr.push_back(f1Flex->NumberRC(nRow, nCol).operator double());
                break;
            }
            case 2: {
                ValuesArr.push_back(HTMLEncode(char16ToUTF8(f1Flex->TextRC(nRow, nCol))));
                break;
            }
            default: {
                ValuesArr.emplace_back(nullptr);
                break;
            }
                /* Uncomment and adapt if needed
                case 3: {
                    std::string ws = f1Flex->FormulaRC(nRow, nCol);
                    if (!ws.empty() && ws[0] == '=') {
                        ws = ws.substr(1);
                    }
                    ws = "\"" + ws + "\"";
                    ValuesArr.push_back(ws);
                    break;
                }
                */
        }
    }

    // ��������� ������� � aCellValue
    aCellValue["Values"] = ValuesArr;
    aCellValue["Styles"] = StylesArr;
}




std::string TXlsToStrJSON::HTMLEncode(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '<':
                result += "&lt;";
                break;
            case '>':
                result += "&gt;";
                break;
            case '&':
                result += "&amp;";
                break;
            case '"':
                result += "&quot;";
                break;
            case '\'':
                result += "&#39;";
                break;
            case '\\':
                result += "&#92;";
                break;
            default:
                result += c;
                break;
        }
    }
    
    // Replace newline characters with <br />
    size_t pos = 0;
    while ((pos = result.find("\r\n", pos)) != std::string::npos) {
        result.replace(pos, 2, "<br />");
        pos += 5;
    }
    
    // Replace tab characters with spaces
    pos = 0;
    while ((pos = result.find("\t", pos)) != std::string::npos) {
        result.replace(pos, 1, " ");
        pos++;
    }
    
    return result;
}

TStyle::TStyle(TFormLogicF1* FormLogicF1, TF1FlexCel* F1FlexCel, int nRow, int nCol)
        : FIndex(-1) // Инициализация FIndex значением по умолчанию
{
    SetStyle(FormLogicF1, F1FlexCel, nRow, nCol);
}

TStyle::~TStyle() {
}
void getRGB(uint32_t cl, uint8_t& ar, uint8_t& ag, uint8_t& ab) {
    ar = cl;
    ag = cl >> 8;
    ab = cl >> 16;
}
void TStyle::SetStyle(TFormLogicF1* FormLogicF1, TF1FlexCel* F1FlexCel, int nRow, int nCol) {
    // Background color
    unsigned int crBG = F1FlexCel->BackColorRC_Get(nRow, nCol);
    FBackColor = TPropertyConverter::ColorToString(crBG);
    // Font info
    unsigned int crFG;
    char16_t* wFontName;
    int wFontSize;
    int wUnderline;
    bool wBold, wItalic;
    F1FlexCel->FontInfoRC_Get(nRow, nCol, crFG, (char16_t &)wFontName, wFontSize, wUnderline, wBold, wItalic);
    FFontColor = TPropertyConverter::ColorToString(crFG);
    FFontName = utf16_to_utf8(wFontName);
    if (FFontName.find(' ') != string::npos) {
        FFontName = "\"" + FontName() + "\"";
    }
    FFontSize = to_string((wFontSize / 20) + 3) + "px";

    // Alignment
    if (nCol == 11){
        int wi = 0;
    }
    FHAlign  = static_cast<THorizontalAlign>(TPropertyConverter::GetHAlign(FormLogicF1->f1Forms, nRow, nCol));
    FVAlign = static_cast<TVerticalAlign>(TPropertyConverter::GetVAlign(FormLogicF1->f1Forms, nRow, nCol));

    // Cell type and format
    PCellValueAtr vd = nullptr;
    unsigned char isShowZero = 0;
    vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, nCol);
    isShowZero = TFormDopInp::GetTww(FormLogicF1)->isShowZero;
    int type_ = FormLogicF1->f1Forms->TypeRC_Get(nRow, nCol);
    bool NumberFormat = false;
    char16_t *wFormat;
    bool wLocked, wWrapText;
    if (type_ == 0) {
        NumberFormat = (vd != nullptr) && (vd->pokaz_type == 1);
    } else if (type_ == 1 || type_ == -1) {
        NumberFormat = true;
    }
    F1FlexCel->CellInfoRC_Get(nRow, nCol, (char16_t &)wFormat, wLocked, wWrapText);
    if (NumberFormat) {
        FFormatter = utf16_to_utf8(wFormat);
        if (FFormatter.empty()) {
            FFormatter= "#";
        }
        if (isShowZero == 0) {
            FFormatter += ";;@";
        }
    } else {
        FFormatter = "General";
        if (vd != nullptr) {
            if ((vd->DataType == 0 || vd->DataType == 2) && (vd->pokaz_type == 0 || vd->pokaz_type == 2)) {
                FFormatter = "@";
            }
        } else {
            if (type_ == 2) {
                FFormatter = "@";
            }
        }
    }

    // Borders
    int wBorderTopStyle, wBorderLeftStyle, wBorderBottomStyle, wBorderRightStyle;
    int wBorderTopColor, wBorderLeftColor, wBorderBottomColor, wBorderRightColor;
    F1FlexCel->BorderInfo_Get(nRow, nCol, wBorderTopStyle, wBorderLeftStyle, wBorderBottomStyle, wBorderRightStyle,
                             wBorderTopColor, wBorderLeftColor, wBorderBottomColor, wBorderRightColor);
    FBorderLeft.Color() = TPropertyConverter::ColorToString(wBorderLeftColor);
    FBorderLeft.Style() = static_cast<TLineStyle>(wBorderLeftStyle);
    FBorderTop.Color() = TPropertyConverter::ColorToString(wBorderTopColor);
    FBorderTop.Style() = static_cast<TLineStyle>(wBorderTopStyle);
    FBorderRight.Color() = TPropertyConverter::ColorToString(wBorderRightColor);
    FBorderRight.Style() = static_cast<TLineStyle>(wBorderRightStyle);
    FBorderBottom.Color() = TPropertyConverter::ColorToString(wBorderBottomColor);
    FBorderBottom.Style() = static_cast<TLineStyle>(wBorderBottomStyle);

    // Other properties
    FLocked = wLocked;
    FWordWrap = wWrapText;
    FFontUnderline = wUnderline;
    FFontBold = wBold;
    FFontItalic = wItalic;

    // Name remains empty
    FName = "";
}
std::string ColorToRGB(int Color) {
    unsigned char r, g, b;
    r = Color;
    g = Color >> 8;
    b = Color >> 16;
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(r)
       << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(g)
       << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
    return ss.str();
}
std::string TPropertyConverter::ColorToString(unsigned int Value) {
    return "#" + ColorToRGB(Value);
}



int TPropertyConverter::GetHAlign(TF1FlexCel* F1FlexCel, int nRow, int nCol) {
    int Value = F1FlexCel->HAlignRC_Get(nRow, nCol);
    switch (Value) {
        case 1: return 0;
        case 2: return 1;
        case 3: return 2;
        case 0:
        case 4:
        case 5:
        case 6:
        case 7: return 2;
        default: return 2;
    }
}

int TPropertyConverter::GetVAlign(TF1FlexCel* F1FlexCel, int nRow, int nCol) {
    int Value = F1FlexCel->VAlignRC_Get(nRow, nCol);
    switch (Value) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        default: return 2;
    }
}
#endif