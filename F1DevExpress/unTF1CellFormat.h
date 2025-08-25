#ifndef unF1CellFormat_H
#define unF1CellFormat_H

#include <string>
#include <vector>
#include <cstdint>
//#include "../SystFuncs/SystFuncsAltWin.h"
#include "TF1FlexCel.h"
#include <array>


// Constants for enum F1ClearTypeConstants
enum F1ClearTypeConstants {
    F1ClearDlg = 0x00000000,
    F1ClearAll = 0x00000001,
    F1ClearFormats = 0x00000002,
    F1ClearValues = 0x00000003
};

// Constants for F1ObjTypeConstants
enum class F1ObjTypeConstants : unsigned int {
    F1ObjLine = 0x00000001,
    F1ObjRectangle = 0x00000002,
    F1ObjOval = 0x00000003,
    F1ObjArc = 0x00000004,
    F1ObjChart = 0x00000005,
    F1ObjButton = 0x00000007,
    F1ObjPolygon = 0x00000008,
    F1ObjCheckBox = 0x00000009,
    F1ObjDropDown = 0x0000000A,
    F1ObjPicture = 0x0000000B
};

// Constants for F1ShiftTypeConstants
enum class F1ShiftTypeConstants : unsigned int {
    F1ShiftHorizontal = 0x00000001,
    F1ShiftVertical = 0x00000002,
    F1ShiftRows = 0x00000003,
    F1ShiftCols = 0x00000004,
    F1FixupNormal = 0x00000000,
    F1FixupPrepend = 0x00000010,
    F1FixupAppend = 0x00000020
};

// Constants for enum F1BorderConstants
enum class F1BorderConstants : uint32_t {
    F1HInsideBorder = 0xFFFFFFFE,
    F1VInsideBorder = 0xFFFFFFFF,
    F1TopBorder = 0x00000000,
    F1LeftBorder = 0x00000001,
    F1BottomBorder = 0x00000002,
    F1RightBorder = 0x00000003
};

// Constants for enum F1BorderStyleConstants
enum class F1BorderStyleConstants : uint32_t {
    F1BorderNone = 0x00000000,
    F1BorderThin = 0x00000001,
    F1BorderMedium = 0x00000002,
    F1BorderDashed = 0x00000003,
    F1BorderDotted = 0x00000004,
    F1BorderThick = 0x00000005,
    F1BorderDouble = 0x00000006,
    F1BorderHair = 0x00000007,
    F1BorderMediumDashed = 0x00000008,
    F1BorderDashDot = 0x00000009,
    F1BorderMediumDashDot = 0x0000000A,
    F1BorderDashDotDot = 0x0000000B,
    F1BorderMediumDashDotDot = 0x0000000C,
    F1BorderSlantedDashDot = 0x0000000D
};

// Constants for enum F1HAlignConstants
enum class F1HAlignConstants : uint32_t {
//    F1HAlignGeneral = 0x00000000,
//    F1HAlignLeft = 0x00000001,
//    F1HAlignCenter = 0x00000002,
//    F1HAlignRight = 0x00000003,
//    F1HAlignFill = 0x00000004,
//    F1HAlignJustify = 0x00000005,
//    F1HAlignCenterAcrossCells = 0x00000006
// Поменял 11.05.2024
    F1HAlignGeneral = 0,
    F1HAlignLeft = 1,
    F1HAlignCenter = 2,
    F1HAlignRight = 3,
    F1HAlignFill = 4,
    F1HAlignJustify = 5,
    F1HAlignCenterAcrossCells = 6
};

// Constants for enum F1VAlignConstants
enum class F1VAlignConstants : uint32_t {
//    F1VAlignTop = 0x00000001,
//    F1VAlignCenter = 0x00000002,
//    F1VAlignBottom = 0x00000003
// Поменял 11.05.2024
    F1VAlignTop = 0,
    F1VAlignCenter = 1,
    F1VAlignBottom = 2
};

class TF1CellFormat {
private:
    class TFlxFormat* cf;
    int32_t XF;
    bool isAlignHorizontal = false;
    uint32_t AlignHorizontal_;
    bool isWordWrap = false;
    bool WordWrap_ = false;
    bool isAlignVertical = false;
    uint32_t AlignVertical_;

    bool isBorderStyle = false;
    std::array<uint32_t, 4> BorderStyle_;
    uint32_t pShade_;
    bool isBorderColor = false;
    std::array<uint32_t, 4> BorderColor_;

    bool isFontName = false;
    std::string FontName_;
    bool isFontSize = false;
    uint32_t FontSize_;
    bool isFontBold = false;
    bool FontBold_ = false;
    bool isFontItalic = false;
    bool FontItalic_ = false;
    bool isFontUnderline = false;
    bool FontUnderline_ = false;
    bool isFontStrikeout = false;
    bool FontStrikeout_ = false;
    bool isFontColor = false;
    int FontColor_;

    bool isPatternStyle = false;
    uint32_t PatternStyle_;
    int PatternBG_;
    int PatternFG_;

    bool isProtectionLocked = false;
    bool ProtectionLocked_ = false;
    bool ProtectionHidden_ = false;

    bool isNumberFormat = false;
//    std::string NumberFormat_ = "# ### ##0.0#;-# ### ##0.0#";
    std::string NumberFormat_;

    bool MergeCells_ = false;
    bool isRotation = false;
    uint32_t Rotation_;

    uint32_t AlignHorizontal;
    bool WordWrap;
    uint32_t AlignVertical;
    std::string FontName;
    uint32_t FontSize;
    bool FontBold;
    bool FontItalic;
    bool FontUnderline;
    bool FontStrikeout;
    int FontColor;
    uint32_t PatternStyle;
    int PatternFG;
    int PatternBG;
    bool ProtectionLocked;
    std::string NumberFormat;
    uint32_t Rotation;
    bool MergeCells;


public:
    uint32_t AlignHorizontal_Get();
    void AlignHorizontal_Set(uint32_t pAlignHorizontal);
    bool WordWrap_Get();
    void WordWrap_Set(bool pWordWrap);
    uint32_t AlignVertical_Get();
    void AlignVertical_Set(uint32_t pAlignVertical);
    F1BorderStyleConstants BorderStyle_Get(F1BorderConstants aWhichBorder);
    void BorderStyle_Set(F1BorderConstants aWhichBorder, F1BorderStyleConstants pBorderStyle);
    int BorderColor_Get(F1BorderConstants aWhichBorder);
    void BorderColor_Set(F1BorderConstants aWhichBorder, int pBorderColor);
    std::string  FontName_Get();
    void FontName_Set(const std::string pFontName);
    uint32_t FontSize_Get();
    void FontSize_Set(uint32_t pFontSize);
    bool FontBold_Get();
    void FontBold_Set(bool pFontBold);
    bool FontItalic_Get();
    void FontItalic_Set(bool pFontItalic);
    bool FontUnderline_Get();
    void FontUnderline_Set(bool pFontUnderline);
    bool FontStrikeout_Get();
    void FontStrikeout_Set(bool pFontStrikeout);
    int FontColor_Get();
    void FontColor_Set(int pFontColor);
    uint32_t PatternStyle_Get();
    void PatternStyle_Set(uint32_t pPatternStyle);
    int PatternFG_Get();
    void PatternFG_Set(int pPatternFG);
    int PatternBG_Get();
    void PatternBG_Set(int pPatternBG);
    bool ProtectionLocked_Get();
    void ProtectionLocked_Set(bool pProtectionLocked);
    std::string NumberFormat_Get();
    void NumberFormat_Set(const std::string pNumberFormat);
    uint32_t Rotation_Get();
    void Rotation_Set(uint32_t pRotation);



    void getPatternStyle(uint32_t& PatternStyle, int& PatternBG, int& PatternFG);
    void setPatternStyle(uint32_t PatternStyle, int PatternBG, int PatternFG);

    static DelphiAdapt::THFlxAlignment HAlign(uint32_t aH);
    static uint32_t HAlignObr(DelphiAdapt::THFlxAlignment aH);

    static DelphiAdapt::TVFlxAlignment VAlign(uint32_t aV);
    static uint32_t VAlignObr(DelphiAdapt::TVFlxAlignment aV);

    void GetCellFormat(F1DevExpress::unF1FlexCel::TF1FlexCel& f1Forms) ;
    void SetCellFormat(F1DevExpress::unF1FlexCel::TF1FlexCel& f1Forms) ;

    F1BorderStyleConstants BorderStyle[4];
    int BorderColor[4];
};

#endif