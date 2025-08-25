
#include <string>
#include <vector>
#include <unordered_map>
#include "json.hpp"
#include "TF1FlexCel.h"

using json = nlohmann::json;

struct TRange {
    int nStartRow;
    int nStartCol;
    int nEndRow;
    int nEndCol;
};
typedef TRange* PRange;


enum class TImageLayout : int { Stretch = 0, Center = 1, Zoom = 2, None = 3 };
enum class THorizontalAlign : int { Left = 0, Center = 1, Right = 2, General = 3 };
enum class TVerticalAlign : int { Top = 0, Center = 1, Bottom = 2 };
enum class TLineStyle : int { Empty = 0, Thin = 1, Medium = 2, Dashed = 3, Dotted = 4, Thick = 5, Double = 6, Hair = 7, MediumDashed = 8, DashDot = 9, MediumDashDot = 10, DashDotDot = 11, MediumDashDotDot = 12, SlantedDashDot = 13 };

struct TLineBorder {
    std::string FColor; // "default" : "black"
    TLineStyle FStyle; // "default" : 0
    std::string& Color() { return FColor; }
    TLineStyle& Style() { return FStyle; }
};

enum class TErrorStyle : int { Stop = 0, Warning = 1, Information = 2 };
enum class TComparisonOperator : int { EqualsTo = 0, NotEqualsTo = 1, GreaterThan = 2, GreaterThanOrEqualsTo = 3, LessThan = 4, LessThanOrEqualsTo = 5, Between = 6, NotBetween = 7 };
enum class TCriteriaType : int { AnyValue = 0, WholeNumber = 1, DecimalValues = 2, List = 3, Date = 4, Time = 5, TextLength = 6, Custom = 7 };

enum class TImeMode : int { Disabled = 0, Auto = 1, Active = 2, Inactive = 4 };

class TStyle {
private:
    std::string FBackColor;
    std::string FFontColor;
    THorizontalAlign FHAlign;
    TVerticalAlign FVAlign;
    std::string FFontName;
    std::string FFontSize;
    std::string FFormatter;  // GeneralFormatter, CustomFormatter
    TLineBorder FBorderLeft;
    TLineBorder FBorderTop;
    TLineBorder FBorderRight;
    TLineBorder FBorderBottom;
    bool FLocked;
    bool FWordWrap;
    std::string FName;
    int FIndex;
    bool FFontBold;
    bool FFontItalic;
    bool FFontUnderline;

public:
    TStyle(TFormLogicF1* FormLogicF1, TF1FlexCel* F1FlexCel, int nRow, int nCol);
    TStyle(){};
    ~TStyle();
    void SetStyle(TFormLogicF1* FormLogicF1, TF1FlexCel* F1FlexCel, int nRow, int nCol);

    int& Index() { return FIndex; }

    std::string& BackColor() { return FBackColor; }
    std::string& FontColor() { return FFontColor; }
    THorizontalAlign& HAlign() { return FHAlign; }
    TVerticalAlign& VAlign() { return FVAlign; }
    std::string& FontName() { return FFontName; }
    std::string& FontSize() { return FFontSize; }
    std::string& Formatter() { return FFormatter; }
    TLineBorder& BorderLeft() { return FBorderLeft; }
    TLineBorder& BorderTop() { return FBorderTop; }
    TLineBorder& BorderRight() { return FBorderRight; }
    TLineBorder& BorderBottom() { return FBorderBottom; }
    bool& Locked() { return FLocked; }
    bool& WordWrap() { return FWordWrap; }
    std::string& Name() { return FName; }
    bool& FontBold() { return FFontBold; }
    bool& FontItalic() { return FFontItalic; }
    bool& FontUnderline() { return FFontUnderline; }
};

class TPropertyConverter {
public:
    static int GetHAlign(TF1FlexCel* F1FlexCel, int nRow, int nCol);
    static int GetVAlign(TF1FlexCel* F1FlexCel, int nRow, int nCol) ;
    static std::string ColorToString(unsigned int Value);
};

class TXlsToStrJSON {
public:
    static const int COL_WIDTH_WEB_DIFF = 3;
    static const int RowMult = 0xF; // 36.6;

private:
    TStringList NewStyleList;
    TStringList CommonStyleList;

public:
    std::string SpreadsheetJSName;
    TF1FlexCel* f1Flex;
    TFormLogicF1* FormLogicF1;

    TXlsToStrJSON();
    ~TXlsToStrJSON();
    void NewStyleListClear();
    void ClearCommonStyles();

    void doStmntInsSheet(json& aSheet, int nSheet);
    void doStmntCellValue(json& aCellValue);

    void InsertSheet(json& aSheet, int nSheet);

    void Parse_Sheet(nlohmann::json& sheet);
    void Parse_Rows(TRange& ARange, json& rows);
    void Parse_Columns(TRange& ARange, json& columns);
    void Parse_Data(TRange& ARange, json& data);
    bool Parse_DataTable(TRange& ARange, json& dataTable);
    void Parse_NamedStyles(json& namedStyles);
    void Parse_Buttons(json& buttons);
    std::string HTMLEncode(const std::string& str);

    static bool doXlsToStrJSON(TFormLogicF1* FormLogicF1, int nSheet, json &serializer);
};

