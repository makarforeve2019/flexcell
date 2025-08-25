#include "unTF1CellFormat.h"
#include "ColorTypes.h"
#include "ClassesUtils.h"

uint32_t TF1CellFormat::AlignHorizontal_Get() {
    return AlignHorizontal_;
}

void TF1CellFormat::AlignHorizontal_Set(uint32_t pAlignHorizontal) {
    isAlignHorizontal = true;
    AlignHorizontal_ = pAlignHorizontal;
}

bool TF1CellFormat::WordWrap_Get() {
    return WordWrap_;
}

void TF1CellFormat::WordWrap_Set(bool pWordWrap) {
    isWordWrap = true;
    WordWrap_ = pWordWrap;
}

uint32_t TF1CellFormat::AlignVertical_Get() {
    return AlignVertical_;
}

void TF1CellFormat::AlignVertical_Set(uint32_t pAlignVertical) {
    isAlignVertical = true;
    AlignVertical_ = pAlignVertical;
}

F1BorderStyleConstants TF1CellFormat::BorderStyle_Get(F1BorderConstants aWhichBorder) {
    return static_cast<F1BorderStyleConstants>(BorderStyle_[static_cast<size_t>(aWhichBorder)]);
}

void TF1CellFormat::BorderStyle_Set(F1BorderConstants aWhichBorder, F1BorderStyleConstants pBorderStyle) {
    isBorderStyle = true;
    BorderStyle_[static_cast<size_t>(aWhichBorder)] = static_cast<uint32_t>(pBorderStyle);
}

DelphiAdapt::TColor TF1CellFormat::BorderColor_Get(F1BorderConstants aWhichBorder) {
    return BorderColor_[static_cast<unsigned long long int>(aWhichBorder)];
}

void TF1CellFormat::BorderColor_Set(F1BorderConstants aWhichBorder, DelphiAdapt::TColor pBorderColor) {
    isBorderColor = true;
    BorderColor_[static_cast<size_t>(aWhichBorder)] = pBorderColor;
}

std::string  TF1CellFormat::FontName_Get() {
    return FontName_;
}

void TF1CellFormat::FontName_Set(const std::string pFontName) {
    isFontName = true;
    FontName_ = pFontName;
}

uint32_t TF1CellFormat::FontSize_Get() {
    return FontSize_;
}

void TF1CellFormat::FontSize_Set(uint32_t pFontSize) {
    isFontSize = true;
    FontSize_ = pFontSize;
}

bool TF1CellFormat::FontBold_Get() {
    return FontBold_;
}

void TF1CellFormat::FontBold_Set(bool pFontBold) {
    isFontBold = true;
    FontBold_ = pFontBold;
}

bool TF1CellFormat::FontItalic_Get() {
    return FontItalic_;
}

void TF1CellFormat::FontItalic_Set(bool pFontItalic) {
    isFontItalic = true;
    FontItalic_ = pFontItalic;
}

bool TF1CellFormat::FontUnderline_Get() {
    return FontUnderline_;
}

void TF1CellFormat::FontUnderline_Set(bool pFontUnderline) {
    isFontUnderline = true;
    FontUnderline_ = pFontUnderline;
}

bool TF1CellFormat::FontStrikeout_Get() {
    return FontStrikeout_;
}

void TF1CellFormat::FontStrikeout_Set(bool pFontStrikeout) {
    isFontStrikeout = true;
    FontStrikeout_ = pFontStrikeout;
}

uint32_t TF1CellFormat::PatternStyle_Get() {
    return PatternStyle_;
}

void TF1CellFormat::PatternStyle_Set(uint32_t pPatternStyle) {
    isPatternStyle = true;
    PatternStyle_ = pPatternStyle;
}

DelphiAdapt::TColor TF1CellFormat::PatternFG_Get() {
    return PatternFG_;
}

void TF1CellFormat::PatternFG_Set(DelphiAdapt::TColor pPatternFG) {
    isPatternStyle = true;
    PatternFG_ = pPatternFG;
}

DelphiAdapt::TColor TF1CellFormat::PatternBG_Get() {
    return PatternBG_;
}

void TF1CellFormat::PatternBG_Set(DelphiAdapt::TColor pPatternBG) {
    isPatternStyle = true;
    PatternBG_ = pPatternBG;
}

bool TF1CellFormat::ProtectionLocked_Get() {
    return ProtectionLocked_;
}

void TF1CellFormat::ProtectionLocked_Set(bool pProtectionLocked) {
    isProtectionLocked = true;
    ProtectionLocked_ = pProtectionLocked;
}

std::string TF1CellFormat::NumberFormat_Get() {
    return NumberFormat_;
}

void TF1CellFormat::NumberFormat_Set(std::string pNumberFormat) {
    isNumberFormat = true;
    NumberFormat_ = pNumberFormat;
}

uint32_t TF1CellFormat::Rotation_Get() {
    return Rotation_;
}

void TF1CellFormat::Rotation_Set(uint32_t pRotation) {
    isRotation = true;
    Rotation_ = pRotation;
}

void TF1CellFormat::getPatternStyle(uint32_t& PatternStyle, int& PatternBG, int& PatternFG) {
    PatternStyle = PatternStyle_;
    PatternBG = PatternBG_;
    PatternFG = PatternFG_;
}

void TF1CellFormat::setPatternStyle(uint32_t PatternStyle, int PatternBG, int PatternFG) {
    isPatternStyle = true;
    PatternStyle_ = PatternStyle;
    PatternBG_ = PatternBG;
    PatternFG_ = PatternFG;
}

int TF1CellFormat::FontColor_Get() {
    return FontColor_;
}

void TF1CellFormat::FontColor_Set(int pFontColor) {
    isFontColor = true;
    FontColor_ = pFontColor;
}


void TF1CellFormat::GetCellFormat(F1DevExpress::unF1FlexCel::TF1FlexCel& f1Forms){
    // Implement GetCellFormat method
    f1Forms.CellFormat_Get();
}

DelphiAdapt::THFlxAlignment TF1CellFormat::HAlign(uint32_t aH) {
    // Implement HAlign method
    switch (aH) {
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignGeneral):
            return DelphiAdapt::THFlxAlignment::haGeneral;
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignLeft):
            return DelphiAdapt::THFlxAlignment::haLeft;
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignCenter):
            return DelphiAdapt::THFlxAlignment::haCenter;
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignRight):
            return DelphiAdapt::THFlxAlignment::haRight;
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignFill):
            return DelphiAdapt::THFlxAlignment::haFill;
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignJustify):
            return DelphiAdapt::THFlxAlignment::haJustify;
        case static_cast<uint32_t>(F1HAlignConstants::F1HAlignCenterAcrossCells):
            return DelphiAdapt::THFlxAlignment::haCenterAcrossCells;
    }

    return DelphiAdapt::THFlxAlignment::haCenter;
}

uint32_t TF1CellFormat::HAlignObr(DelphiAdapt::THFlxAlignment aH) {
    switch (aH) {
        case DelphiAdapt::THFlxAlignment::haGeneral:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignGeneral);
        case DelphiAdapt::THFlxAlignment::haLeft:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignLeft);
        case DelphiAdapt::THFlxAlignment::haCenter:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignCenter);
        case DelphiAdapt::THFlxAlignment::haRight:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignRight);
        case DelphiAdapt::THFlxAlignment::haFill:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignFill);
        case DelphiAdapt::THFlxAlignment::haJustify:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignJustify);
        case DelphiAdapt::THFlxAlignment::haCenterAcrossCells:
            return static_cast<uint32_t>(F1HAlignConstants::F1HAlignCenterAcrossCells);
    }
}

DelphiAdapt::TVFlxAlignment TF1CellFormat::VAlign(uint32_t aV) {
    // Implement VAlign method
    switch (aV) {
        case static_cast<uint32_t>(F1VAlignConstants::F1VAlignTop):
            return DelphiAdapt::TVFlxAlignment::vaTop;
        case static_cast<uint32_t>(F1VAlignConstants::F1VAlignCenter):
            return DelphiAdapt::TVFlxAlignment::vaCenter;
        case static_cast<uint32_t>(F1VAlignConstants::F1VAlignBottom):
            return DelphiAdapt::TVFlxAlignment::vaBottom;
    }
    return DelphiAdapt::TVFlxAlignment::vaCenter;
}

uint32_t TF1CellFormat::VAlignObr(DelphiAdapt::TVFlxAlignment aV) {
    // Implement VAlignObr method
    switch (aV) {
        case DelphiAdapt::TVFlxAlignment::vaTop:
            return static_cast<uint32_t>(F1VAlignConstants::F1VAlignTop);
        case DelphiAdapt::TVFlxAlignment::vaCenter:
            return static_cast<uint32_t>(F1VAlignConstants::F1VAlignCenter);
        case DelphiAdapt::TVFlxAlignment::vaBottom:
            return static_cast<uint32_t>(F1VAlignConstants::F1VAlignBottom);
    }
}

void TF1CellFormat::SetCellFormat(F1DevExpress::unF1FlexCel::TF1FlexCel& f1Forms){
    // Implement SetCellFormat method
    f1Forms.F1CellFormat_Set(   this->isAlignHorizontal,
                                this->AlignHorizontal_,
                            this->isWordWrap,
                            this->WordWrap_,
                            this->isAlignVertical,
                            this->AlignVertical_,
                            this->isBorderStyle,
                            4,
                            this->pShade_,
                            this->isBorderColor,
                            0,
                            this->isFontName,
                                utf8_to_utf16(this->FontName_).c_str(),
                            this->isFontSize,
                            this->FontSize_,
                            this->isFontBold,
                            this->FontBold_,
                            this->isFontItalic,
                            this->FontItalic_,
                            this->isFontUnderline,
                            this->FontUnderline_,
                            this->isFontStrikeout,
                            this->FontStrikeout_,
                            this->isFontColor,
                            this->FontColor_,
                            this->isPatternStyle,
                            this->PatternStyle_,
                            this->PatternBG_,
                            this->PatternFG_,
                            this->isProtectionLocked,
                            this->ProtectionLocked_,
                            this->ProtectionHidden_,
                            this->isNumberFormat,
                                utf8_to_utf16(this->NumberFormat_).c_str(),
                            this->MergeCells_,
                            this->isRotation,
                            this->Rotation_);
}


