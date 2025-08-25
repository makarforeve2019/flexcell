#ifndef F1FLEXCEL_CPPPROJECT_TF1FLEXCEL_H
#define F1FLEXCEL_CPPPROJECT_TF1FLEXCEL_H

#include <cstdint>

//#include "ActionView.h"

#include "Property.h"

#include "TF1Range.h"
#include "TMWSPageSetup.h"
#include "THFlxAlignment.h"
#include "TVFlxAlignment.h"
#include "SortKeys.h"
#include "TCellValueType.h"
#include "ColorTypes.h"
#include "SystFuncsAltWin.h"

/**
 * Если определено, то подразумевается, что в подключенной библиотеки в качестве типа передаваемого ключа используется ключ списка(Dictionary) типа int.
 * <br/>Если не определено, то библиотека передает ссылку на объект, которая хранится как ссылка на int.
 * <br/>Момент! Если в библиотеке используется список(Dictionary), то оба варианта будут работать, однако надежнее, и правильнее, если key будет хранить именно число,
 * а не ссылку на число, которое лежит в области памяти библиотеки.
 */
#define F1FLEXCEL_KEY_IS_DIC_CONTAINER_KEY

#ifdef F1FLEXCEL_KEY_IS_DIC_CONTAINER_KEY
using KeyType = int;
#else
using KeyType = int *;
#endif

namespace F1DevExpress::unF1FlexCel {
    using namespace DelphiAdapt;
    using namespace unMWSPageSetup;

    /**
     * Если метод в Delphi использует аргумент-ссылку(VAR или OUT) строкового типа, здесь аргумент обозначен как &char16_t,
     * <br/>а мы располагаем переменной: char_16_t *varName(указатель на начало строки), то следует передавать в метод это: (char16_t &) varName.
     */
    class TF1FlexCel {
    public:
        static inline int FlexCelInstancesCount;
        static inline void *LibHandle;

    protected:
        // Нет в Delphi-проекте
        static inline bool (*libTestBoolean)(KeyType key);

        static inline const char16_t *(*libTestString)(KeyType key);

        static inline KeyType (*libCreateDefault)();

        static inline KeyType (*libCreateFromFile)(const char16_t *filename);

        static inline KeyType (*libCreateWithSheetCount)(int sheetCount);

        static inline void (*libFree)(KeyType key);

        static inline void (*libSave)(KeyType key, const char16_t *filename);

        static inline int (*libGetCellValueType)(KeyType key, int nRow, int nCol);

        static inline bool (*libGetDisableAutoSize)(KeyType key);

        static inline void (*libSetDisableAutoSize)(KeyType key, bool value);

        static inline bool (*libGetDisableShrink)(KeyType key);

        static inline void (*libSetDisableShrink)(KeyType key, bool value);

        // По Delphi-проекту
        static inline int (*libGetNumSheets)(KeyType key);

        static inline void (*libSetNumSheets)(KeyType key, int value);

        static inline int (*libGetSheet)(KeyType key);

        static inline void (*libSetSheet)(KeyType key, int value);

        static inline const char16_t *(*libGet_TextRC)(KeyType key, int nRow, int nCol);

        static inline void (*libSet_TextRC)(KeyType key, int nRow, int nCol, const char16_t *value);

        static inline double (*libGet_NumberRC)(KeyType key, int nRow, int nCol);

        static inline void (*libSet_NumberRC)(KeyType key, int nRow, int nCol, const double value);

        static inline bool (*libGet_ColHidden)(KeyType key, int nCol);

        static inline bool (*libGet_RowHidden)(KeyType key, int nRow);

        static inline void (*libSet_ColHidden)(KeyType key, int nCol, const bool value);

        static inline void (*libSet_RowHidden)(KeyType key, int nRow, const bool value);

        static inline int (*libGetCol)(KeyType key);

        static inline int (*libGetRow)(KeyType key);

        static inline void (*libSetCol)(KeyType key, const int value);

        static inline void (*libSetRow)(KeyType key, const int value);

        static inline const char16_t *(*libGet_ObjName)(KeyType key, int ObjID);

        static inline void (*libSet_ObjName)(KeyType key, int ObjID, const char16_t *value);

        static inline const char16_t *(*libGet_ObjText)(KeyType key, int ObjID);

        static inline void (*libSet_ObjText)(KeyType key, int ObjID, const char16_t *value);

        static inline int (*libGetMaxCol)(KeyType key);

        static inline int (*libGetMaxRow)(KeyType key);

        static inline void (*libSetMaxCol)(KeyType key, const int value);

        static inline void (*libSetMaxRow)(KeyType key, const int value);

        static inline int (*libGetLastCol)(KeyType key);

        static inline int (*libGetLastRow)(KeyType key);

        static inline void (*libSetLastCol)(KeyType key, const int value);

        static inline void (*libSetLastRow)(KeyType key, const int value);

        static inline const char16_t *(*libGet_SheetName)(KeyType key, int nSheet);

        static inline void (*libSet_SheetName)(KeyType key, int nSheet, const char16_t *value);

        static inline int (*libGetFixedCols)(KeyType key);

        static inline void (*libSetFixedCols)(KeyType key, const int value);

        static inline int (*libGetFixedRows)(KeyType key);

        static inline void (*libSetFixedRows)(KeyType key, const int value);

        static inline const char16_t *(*libGet_ColText)(KeyType key, int nCol);

        static inline void (*libSet_ColText)(KeyType key, int nCol, const char16_t *value);

        static inline double (*libGet_NumberSRC)(KeyType key, int nSheet, int nRow, int nCol);

        static inline void (*libSet_NumberSRC)(KeyType key, int nSheet, int nRow, int nCol, const double value);

        static inline const char16_t *(*libGet_TextSRC)(KeyType key, int nSheet, int nRow, int nCol);

        static inline void (*libSet_TextSRC)(KeyType key, int nSheet, int nRow, int nCol, const char16_t *value);

        static inline const char16_t *(*libGetFontName)(KeyType key);

        static inline void (*libSetFontName)(KeyType key, const char16_t *value);

        static inline bool (*libGetFontBold)(KeyType key);

        static inline bool (*libGetFontItalic)(KeyType key);

        static inline int (*libGetFontSize)(KeyType key);

        static inline void (*libSetFontBold)(KeyType key, const bool value);

        static inline void (*libSetFontItalic)(KeyType key, const bool value);

        static inline void (*libSetFontSize)(KeyType key, const int value);

        static inline int (*libGetFontColor)(KeyType key);

        static inline void (*libSetFontColor)(KeyType key, const int value);

        static inline bool (*libGetFontUnderline)(KeyType key);

        static inline void (*libSetFontUnderline)(KeyType key, const bool value);

        static inline bool (*libGetFontStrikeout)(KeyType key);

        static inline void (*libSetFontStrikeout)(KeyType key, const bool value);

        static inline const char16_t *(*libGetFormattedText)(KeyType key);

        static inline const char16_t *(*libGetFormula)(KeyType key);

        static inline uint64_t (*libGetHAlign)(KeyType key);

        static inline int (*libGetSelEndCol)(KeyType key);

        static inline int (*libGetSelEndRow)(KeyType key);

        static inline int (*libGetSelStartCol)(KeyType key);

        static inline int (*libGetSelStartRow)(KeyType key);

        static inline const char16_t *(*libGetText)(KeyType key);

        static inline int (*libGetType_)(KeyType key);

        static inline uint64_t (*libGetVAlign)(KeyType key);

        static inline bool (*libGetWordWrap)(KeyType key);

        static inline void (*libSetFormattedText)(KeyType key, const char16_t *value);

        static inline void (*libSetFormula)(KeyType key, const char16_t *value);

        static inline void (*libSetHAlign)(KeyType key, const uint64_t value);

        static inline void (*libSetSelEndCol)(KeyType key, const int value);

        static inline void (*libSetSelEndRow)(KeyType key, const int value);

        static inline void (*libSetSelStartCol)(KeyType key, const int value);

        static inline void (*libSetSelStartRow)(KeyType key, const int value);

        static inline void (*libSetText)(KeyType key, const char16_t *value);

        static inline void (*libSetType_)(KeyType key, const int value);

        static inline void (*libSetVAlign)(KeyType key, const uint64_t value);

        static inline void (*libSetWordWrap)(KeyType key, const bool value);

        static inline int (*libGet_ColWidth)(KeyType key, int nCol);

        static inline void (*libSet_ColWidth)(KeyType key, int nCol, const int value);

        static inline int (*libGet_RowHeight)(KeyType key, int nRow);

        static inline void (*libSet_RowHeight)(KeyType key, int nRow, const int value);

        static inline double (*libGetNumber)(KeyType key);

        static inline void (*libSetNumber)(KeyType key, const double value);

        static inline int (*libGet_ColWidthTwips)(KeyType key, int nCol);

        static inline const char16_t *(*libGet_FormattedTextRC)(KeyType key, int nRow, int nCol);

        static inline const char16_t *(*libGet_FormulaRC)(KeyType key, int nRow, int nCol);

        static inline const char16_t *(*libGet_NumberFormat)(KeyType key);

        static inline int (*libGet_TypeRC)(KeyType key, int nRow, int nCol);

        static inline void (*libSet_ColWidthTwips)(KeyType key, int nCol, const int value);

        static inline void (*libSet_FormulaRC)(KeyType key, int nRow, int nCol, const char16_t *value);

        static inline void (*libSet_NumberFormat)(KeyType key, const char16_t *value);

        /**
         * FontCharSet это Windows-система. Присваивается, и возвращается присвоенное значение, но эффекта в файле не наблюдается.
         * @param key
         * @return
         */
        static inline uint8_t (*libGetFontCharSet)(KeyType key);

        static inline void (*libSetFontCharSet)(KeyType key, const uint8_t value);

        static inline void (*libGet_SelectionEx)(
                KeyType key,
                int nSelection,
                int &StartRow,
                int &EndRow,
                int &StartCol,
                int &EndCol
        );

        static inline bool (*libGetEnableProtection)(KeyType key);

        static inline void (*libSetEnableProtection)(KeyType key, const bool value);

        static inline bool (*libGetShowGridLines)(KeyType key);

        static inline void (*libSetShowGridLines)(KeyType key, const bool value);

        static inline bool (*libGetAutoRecalc)(KeyType key);

        static inline void (*libSetAutoRecalc)(KeyType key, const bool value);

        static inline int (*libGetSelectionCount)(KeyType key);

        static inline void (*libSetSelectionCount)(KeyType key, const int value);

        static inline uint64_t (*libGetColWidthUnits)(KeyType key);

        static inline void (*libSetColWidthUnits)(KeyType key, const uint64_t value);

        static inline bool (*libGetAllowResize)(KeyType key);

        static inline void (*libSetAllowResize)(KeyType key, const bool value);

        static inline int (*libGetHdrWidth)(KeyType key);

        static inline void (*libSetHdrWidth)(KeyType key, const int value);

        static inline const char16_t *(*libGet_FormulaLocalRC)(KeyType key, int nRow, int nCol);

        static inline void (*libSet_FormulaLocalRC)(KeyType key, int nRow, int nCol, const char16_t *value);

        static inline bool (*libGet_ObjVisible)(KeyType key, int ObjID);

        static inline void (*libSet_ObjVisible)(KeyType key, int ObjID, const bool value);

        static inline bool (*libGetShowZeroValues)(KeyType key);

        static inline void (*libSetShowZeroValues)(KeyType key, const bool value);

        static inline bool (*libGetAllowDelete)(KeyType key);

        static inline bool (*libGetAllowInCellEditing)(KeyType key);

        static inline const char16_t *(*libGetDefaultFontName)(KeyType key);

        static inline bool (*libGetShowLockedCellsError)(KeyType key);

        static inline uint64_t (*libGetShowSelections)(KeyType key);

        static inline void (*libSetAllowDelete)(KeyType key, const bool value);

        static inline void (*libSetAllowInCellEditing)(KeyType key, const bool value);

        static inline void (*libSetDefaultFontName)(KeyType key, const char16_t *value);

        static inline void (*libSetShowLockedCellsError)(KeyType key, const bool value);

        static inline void (*libSetShowSelections)(KeyType key, const uint64_t value);

        static inline int (*libGetDefaultFontSize)(KeyType key);

        static inline void (*libSetDefaultFontSize)(KeyType key, const int value);

        static inline void (*libSetCellFormat)(KeyType key, int XF);

        static inline int (*libGetCellFormat)(KeyType key);

        static inline bool (*libGet_MergeCells)(KeyType key);

        static inline void (*libSet_MergeCells)(KeyType key, bool pMergeCells);

        // Get-метод не нужен, т.к. его суть это создание объекта TMWSPageSetup
//        static inline TMWSPageSetup (*libGetPageSetup) (KeyType key);

        static inline void (*libSetPageSetup)(
                KeyType key,
                bool AutoPageNumber,
                int FirstPageNumber,
                double LeftMargin,
                double TopMargin,
                double BottomMargin,
                double RightMargin,
                double HeaderMargin,
                double FooterMargin,
                bool CenterHoriz,
                bool CenterVert,
                const char16_t *Header,
                const char16_t *Footer,
                bool Landscape,
                bool FitPages,
                int PrintScale,
                int PagesWide,
                int PagesTall,
                int PaperSize,
                const char16_t *PrintTitles,
                bool GridLines,
                bool BlackAndWhite,
                bool RowHeadings,
                bool ColHeadings,
                bool LeftToRight,
                const char16_t *PrintArea
        );

        static inline void (*libPrint)(KeyType key);

        static inline void (*libPrintPage)(KeyType key);

        static inline void (*libSetPrintTitlesFromSelection)(KeyType key);

        static inline void (*libSaveToPDF)(KeyType key, const char16_t *filename, const char16_t *documentProperties);

        static inline void (*libRecalc)(KeyType key);

        static inline void (*libDeleteSheets)(KeyType key, int nSheet, int nSheets);

        static inline void (*libSetSelection)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2);

        static inline void (*libGetProtection)(KeyType key, bool &pLocked, bool &pHidden);

        static inline void (*libGetSelection)(KeyType key, int nSelection, int &pR1, int &pC1, int &pR2, int &pC2);

        static inline uint64_t (*libObjGetType)(KeyType key, int ObjID);

        static inline int (*libObjNextID)(KeyType key, int ObjID);

        static inline int (*libObjFirstID)(KeyType key, int nSheet, int nRow, int nCol, char16_t &aCaption);

        static inline void (*libInsertRange)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2, uint64_t InsertType);

        static inline void (*libInsertSheets)(KeyType key, int nSheet, int nSheets);

        static inline void (*libEditInsertSheets)(KeyType key);

        static inline void
        (*libSetBorder)(KeyType key, int nOutline, int nLeft, int nRight, int nTop, int nBottom, int nShade, int crOutline, uint32_t crLeft, uint32_t crRight, uint32_t crTop,
                        uint32_t crBottom);

        static inline void (*libGetBorder)(KeyType key, int &pLeft, int &pRight, int &pTop, int &pBottom, int &pShade, int &pcrLeft, int &pcrRight, int &pcrTop, int &pcrBottom);

        static inline void (*libDrawBorders)(KeyType key, int row1, int col1, int row2, int col2, int borderStyle, int borderColor, bool exteriorBorders);

        static inline void (*libGetPattern)(KeyType key, int &pPattern, int &pcrFG, int &pcrBG);

        static inline void (*libSetColWidthAuto)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults);

        static inline void (*libSetPattern)(KeyType key, int nPattern, uint32_t crFG, uint32_t crBG);

        static inline void (*libSetBackgroundColor)(KeyType key, uint32_t crBG);

        static inline void (*libSetRowHeightAuto)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults);

        // Sort. OleVariant формируется в библиотеке на основе keysString, которая представляет собой строку склеенных запятой номеров. Например, "1,3,8,9".
        static inline void (*libSort)(KeyType key, int nR1, int nC1, int nR2, int nC2, bool bSortByRows, const char16_t *keysString);

        static inline void (*libSetProtection)(KeyType key, bool bLocked, bool bHidden);

        static inline void (*libSetColHidden)(KeyType key, int nCol1, int nCol2, bool bHidden);

        static inline void (*libSetActiveCell)(KeyType key, int nRow, int nCol);

        static inline void (*libShowActiveCell)(KeyType key);

        static inline void (*libCopyRange)(KeyType key, int nDstR1, int nDstC1, int nDstR2, int nDstC2, int hSrcSS, int nSrcR1, int nSrcC1, int nSrcR2, int nSrcC2);

        static inline int (*libSS)(KeyType key);

        static inline void
        (*libSetFont)(KeyType key, const char16_t *pName, int nSize, bool bBold, bool bItalic, bool bUnderline, bool bStrikeout, uint32_t crColor, bool bOutline, bool bShadow);

        // TODO: Не возвращает цвет. В Delphi написано это: pcrColor := 0 { fnt.Color.RGB }; // 21.09.2023
        /**
         *
         * @param key
         * @param pName
         * @param pSize
         * @param pBold
         * @param pItalic
         * @param pUnderline
         * @param pStrikeout
         * @param pcrColor
         * @param pOutline
         * @param pShadow
         */
        static inline void
        (*libGetFont)(KeyType key, char16_t &pName, int &pSize, bool &pBold, bool &pItalic, bool &pUnderline, bool &pStrikeout, int &pcrColor, bool &pOutline, bool &pShadow);

        static inline void (*libDeleteRange)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ShiftType);

        static inline void (*libClearRange)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ClearType);

        static inline void (*libEditClear)(KeyType key, uint64_t ClearType);

        static inline void (*libsetBorderStyle)(KeyType key, int StyleTopBorder, int StyleLeftBorder, int StyleBottomBorder, int StyleRightBorder);

        static inline void (*libgetBorderStyle)(KeyType key, int &StyleTopBorder, int &StyleLeftBorder, int &StyleBottomBorder, int &StyleRightBorder);

        static inline void (*libsetBorderColor)(KeyType key, int ColorTopBorder, int ColorLeftBorder, int ColorBottomBorder, int ColorRightBorder);

        static inline void (*libgetBorderColor)(KeyType key, int &ColorTopBorder, int &ColorLeftBorder, int &ColorBottomBorder, int &ColorRightBorder);

        static inline int (*libInputButton)(KeyType key, int nRow1, int nCol1, int nRow2, int nCol2, const char16_t *aCaption, const char16_t *aObjName);

        static inline void
        (*libgetBorderInfo)(KeyType key, int nRow, int nCol, int &BorderTopStyle, int &BorderLeftStyle, int &BorderBottomStyle, int &BorderRightStyle, int &BorderTopColor,
                            int &BorderLeftColor, int &BorderBottomColor, int &BorderRightColor);

        static inline int (*libGetHAlignRC)(KeyType key, int nRow, int nCol);

        // THFlxAlignment, TVFlxAlignment. Нет никакого смысла запрашивать библиотеку. Возвращает значение перечисления, определенного уже здесь.
//        static inline THFlxAlignment (*libtoHAlign) (KeyType key, int aH);

//        static inline TVFlxAlignment (*libtoVAlign) (KeyType key, int aV);

        static inline int (*libgetVAlignRC)(KeyType key, int nRow, int nCol);

        static inline int (*libGetobjlistCount)(KeyType key);

        static inline void (*libGetobjlistI_PropRC)(KeyType key, int aI, int &nRow, int &nCol, char16_t &objectName);

        static inline int (*libgetNFormatRC)(KeyType key, int nRow, int nCol);

        static inline const char16_t *(*libgetStyleNameRC)(KeyType key, int nRow, int nCol);

        static inline uint32_t (*libgetBackColorRC)(KeyType key, int nRow, int nCol);

        static inline void (*libgetCellInfoRC)(KeyType key, int nRow, int nCol, char16_t &Format, bool &Locked, bool &WrapText);

        static inline void (*libgetFontInfoRC)(KeyType key, int nRow, int nCol, uint32_t &Color, char16_t &FontName, int &FontSize, int &Underline, bool &Bold, bool &Italic);

        /**
         * array of integer: BorderStyle_ и BorderColor_ не используются в Delphi. Поэтому передаем простой int. В Delphi принимаем integer.
         * @param key
         * @param isAlignHorizontal
         * @param AlignHorizontal_
         * @param isWordWrap
         * @param WordWrap_
         * @param isAlignVertical
         * @param AlignVertical_
         * @param isBorderStyle
         * @param BorderStyle_ Передаем любое значение. Оно все равно не используется.
         * @param pShade_
         * @param isBorderColor
         * @param BorderColor_ Передаем любое значение. Оно все равно не используется.
         * @param isFontName
         * @param FontName_
         * @param isFontSize
         * @param FontSize_
         * @param isFontBold
         * @param FontBold_
         * @param isFontItalic
         * @param FontItalic_
         * @param isFontUnderline
         * @param FontUnderline_
         * @param isFontStrikeout
         * @param FontStrikeout_
         * @param isFontColor
         * @param FontColor_
         * @param isPatternStyle
         * @param PatternStyle_
         * @param PatternBG_
         * @param PatternFG_
         * @param isProtectionLocked
         * @param ProtectionLocked_
         * @param ProtectionHidden_
         * @param isNumberFormat
         * @param NumberFormat_
         * @param MergeCells_
         * @param isRotation
         * @param Rotation_
         */
        static inline void (*libSetF1CellFormat)(
                KeyType key,
                bool isAlignHorizontal,
                int AlignHorizontal_,
                bool isWordWrap,
                bool WordWrap_,
                bool isAlignVertical,
                int AlignVertical_,
                bool isBorderStyle,
                int BorderStyle_,
                int pShade_,
                bool isBorderColor,
                int BorderColor_,
                bool isFontName,
                const char16_t *FontName_,
                bool isFontSize,
                int FontSize_,
                bool isFontBold,
                bool FontBold_,
                bool isFontItalic,
                bool FontItalic_,
                bool isFontUnderline,
                bool FontUnderline_,
                bool isFontStrikeout,
                bool FontStrikeout_,
                bool isFontColor,
                int FontColor_,
                bool isPatternStyle,
                int PatternStyle_,
                int PatternBG_,
                int PatternFG_,
                bool isProtectionLocked,
                bool ProtectionLocked_,
                bool ProtectionHidden_,
                bool isNumberFormat,
                const char16_t *NumberFormat_,
                bool MergeCells_,
                bool isRotation,
                int Rotation_
        );

        static inline int (*libGetNVisibleCol)(KeyType key, int &nVisibleCol1);

        static inline int (*libGetTypeRC)(KeyType key, int nRow, int nCol);

        static inline double (*libFormulaResultNumberRC)(KeyType key, int nRow, int nCol);

        static inline const char16_t *(*libFormulaResultTextRC)(KeyType key, int nRow, int nCol);

    protected:
        KeyType key{};


#ifdef ACTIONVIEW
    public:
        TActionView ActionView;
#endif

    public:
        explicit TF1FlexCel();

        explicit TF1FlexCel(const char16_t *filename);

        explicit TF1FlexCel(int sheetCount);

        ~TF1FlexCel();

    protected:
        /**
         * Метод освобождения памяти в библиотеке(Delphi).
         * <br/>Вызывается в деструкторе.
         * <br/>Лучше пусть будет приватным.
         */
        void Free();

        /**
         * Дело в том, что в Delphi используется порядок ABGR,вместо распространенного ARGB.
         * <br/>Поэтому на входе в Delphi и на выходе из него, все значения цвета надо преобразовывать.
         * @param color Int-представление hex-цвета.
         * @return перевернутые байты, отвечающие за RGB.
         */
        int ColorReverse(int color);

    protected:
        static void LibraryHandleConstructor();

        static void LibraryHandleDestructor();

        static void LoadLib();

        static void UnloadLib();

        static void HandleLibraryMethod(const char *methodName, void *linkVar);

    public:
        // В Delphi эти property обращаются напрямую к свойствам объекта.
        Property<bool, TF1FlexCel> DisableAutoSize = Property<bool, TF1FlexCel>(this, &TF1FlexCel::DisableAutoSize_Get, &TF1FlexCel::DisableAutoSize_Set);
        Property<bool, TF1FlexCel> DisableShrink = Property<bool, TF1FlexCel>(this, &TF1FlexCel::DisableShrink_Get, &TF1FlexCel::DisableShrink_Set);

        Property<int, TF1FlexCel> NumSheets = Property<int, TF1FlexCel>(this, &TF1FlexCel::NumSheets_Get, &TF1FlexCel::NumSheets_Set);
        Property<int, TF1FlexCel> Sheet = Property<int, TF1FlexCel>(this, &TF1FlexCel::Sheet_Get, &TF1FlexCel::Sheet_Set);
        Property_Arr_1D<const char16_t *, int, TF1FlexCel> SheetName = Property_Arr_1D<const char16_t *, int, TF1FlexCel>(this, &TF1FlexCel::SheetName_Get, &TF1FlexCel::SheetName_Set);
        Property_Arr_2D<const char16_t *, int, int, TF1FlexCel> TextRC = Property_Arr_2D<const char16_t *, int, int, TF1FlexCel>(this, &TF1FlexCel::TextRC_Get, &TF1FlexCel::TextRC_Set);
        Property_Arr_2D<double, int, int, TF1FlexCel> NumberRC = Property_Arr_2D<double, int, int, TF1FlexCel>(this, &TF1FlexCel::NumberRC_Get, &TF1FlexCel::NumberRC_Set);
        Property_Arr_1D<bool, int, TF1FlexCel> ColHidden = Property_Arr_1D<bool, int, TF1FlexCel>(this, &TF1FlexCel::ColHidden_Get, &TF1FlexCel::ColHidden_Set);
        Property_Arr_1D<bool, int, TF1FlexCel> RowHidden = Property_Arr_1D<bool, int, TF1FlexCel>(this, &TF1FlexCel::RowHidden_Get, &TF1FlexCel::RowHidden_Set);
        Property<int, TF1FlexCel> Col = Property<int, TF1FlexCel>(this, &TF1FlexCel::Col_Get, &TF1FlexCel::Col_Set);
        Property<int, TF1FlexCel> Row = Property<int, TF1FlexCel>(this, &TF1FlexCel::Row_Get, &TF1FlexCel::Row_Set);
        Property_Arr_1D<const char16_t *, int, TF1FlexCel> ObjName = Property_Arr_1D<const char16_t *, int, TF1FlexCel>(this, &TF1FlexCel::ObjName_Get, &TF1FlexCel::ObjName_Set);
        Property_Arr_1D<const char16_t *, int, TF1FlexCel> ObjText = Property_Arr_1D<const char16_t *, int, TF1FlexCel>(this, &TF1FlexCel::ObjText_Get, &TF1FlexCel::ObjText_Set);
        Property<int, TF1FlexCel> MaxCol = Property<int, TF1FlexCel>(this, &TF1FlexCel::MaxCol_Get, &TF1FlexCel::MaxCol_Set);
        Property<int, TF1FlexCel> MaxRow = Property<int, TF1FlexCel>(this, &TF1FlexCel::MaxRow_Get, &TF1FlexCel::MaxRow_Set);
        Property<int, TF1FlexCel> LastCol = Property<int, TF1FlexCel>(this, &TF1FlexCel::LastCol_Get, &TF1FlexCel::LastCol_Set);
        Property<int, TF1FlexCel> LastRow = Property<int, TF1FlexCel>(this, &TF1FlexCel::LastRow_Get, &TF1FlexCel::LastRow_Set);
        Property<int, TF1FlexCel> FixedCols = Property<int, TF1FlexCel>(this, &TF1FlexCel::FixedCols_Get, &TF1FlexCel::FixedCols_Set);
        Property<int, TF1FlexCel> FixedRows = Property<int, TF1FlexCel>(this, &TF1FlexCel::FixedRows_Get, &TF1FlexCel::FixedRows_Set);
        Property_Arr_1D<const char16_t *, int, TF1FlexCel> ColText = Property_Arr_1D<const char16_t *, int, TF1FlexCel>(this, &TF1FlexCel::ColText_Get, &TF1FlexCel::ColText_Set);
        Property_Arr_1D<int, int, TF1FlexCel> RowHeight = Property_Arr_1D<int, int, TF1FlexCel>(this, &TF1FlexCel::RowHeight_Get, &TF1FlexCel::RowHeight_Set);
        Property_Arr_3D<double, int, int, int, TF1FlexCel> NumberSRC = Property_Arr_3D<double, int, int, int, TF1FlexCel>(this, &TF1FlexCel::NumberSRC_Get, &TF1FlexCel::NumberSRC_Set);
        Property_Arr_3D<const char16_t *, int, int, int, TF1FlexCel> TextSRC = Property_Arr_3D<const char16_t *, int, int, int, TF1FlexCel>(this, &TF1FlexCel::TextSRC_Get, &TF1FlexCel::TextSRC_Set);
        Property<const char16_t *, TF1FlexCel> FontName = Property<const char16_t *, TF1FlexCel>(this, &TF1FlexCel::FontName_Get, &TF1FlexCel::FontName_Set);
        Property<int, TF1FlexCel> FontSize = Property<int, TF1FlexCel>(this, &TF1FlexCel::FontSize_Get, &TF1FlexCel::FontSize_Set);
        Property<bool, TF1FlexCel> FontBold = Property<bool, TF1FlexCel>(this, &TF1FlexCel::FontBold_Get, &TF1FlexCel::FontBold_Set);
        Property<bool, TF1FlexCel> FontItalic = Property<bool, TF1FlexCel>(this, &TF1FlexCel::FontItalic_Get, &TF1FlexCel::FontItalic_Set);
        Property<bool, TF1FlexCel> FontUnderline = Property<bool, TF1FlexCel>(this, &TF1FlexCel::FontUnderline_Get, &TF1FlexCel::FontUnderline_Set);
        Property<bool, TF1FlexCel> FontStrikeout = Property<bool, TF1FlexCel>(this, &TF1FlexCel::FontStrikeout_Get, &TF1FlexCel::FontStrikeout_Set);
        Property<int, TF1FlexCel> FontColor = Property<int, TF1FlexCel>(this, &TF1FlexCel::FontColor_Get, &TF1FlexCel::FontColor_Set);
        Property<uint8_t, TF1FlexCel> FontCharSet = Property<uint8_t, TF1FlexCel>(this, &TF1FlexCel::FontCharSet_Get, &TF1FlexCel::FontCharSet_Set);
        Property<uint64_t, TF1FlexCel> HAlign = Property<uint64_t, TF1FlexCel>(this, &TF1FlexCel::HAlign_Get, &TF1FlexCel::HAlign_Set);
        Property<bool, TF1FlexCel> WordWrap = Property<bool, TF1FlexCel>(this, &TF1FlexCel::WordWrap_Get, &TF1FlexCel::WordWrap_Set);
        Property<uint64_t, TF1FlexCel> VAlign = Property<uint64_t, TF1FlexCel>(this, &TF1FlexCel::VAlign_Get, &TF1FlexCel::VAlign_Set);
        Property<int, TF1FlexCel> Type = Property<int, TF1FlexCel>(this, &TF1FlexCel::Type_Get, &TF1FlexCel::Type_Set);
        Property_Arr_2D<const char16_t *, int, int, TF1FlexCel> FormulaRC = Property_Arr_2D<const char16_t *, int, int, TF1FlexCel>(this, &TF1FlexCel::FormulaRC_Get, &TF1FlexCel::FormulaRC_Set);
        Property<double, TF1FlexCel> Number = Property<double, TF1FlexCel>(this, &TF1FlexCel::Number_Get, &TF1FlexCel::Number_Set);
        Property<const char16_t *, TF1FlexCel> Text = Property<const char16_t *, TF1FlexCel>(this, &TF1FlexCel::Text_Get, &TF1FlexCel::Text_Set);
        PropertyRO_Arr_2D<const char16_t *, int, int, TF1FlexCel> FormattedTextRC = PropertyRO_Arr_2D<const char16_t *, int, int, TF1FlexCel>(this, &TF1FlexCel::FormattedTextRC_Get);
        PropertyRO_Arr_2D<int, int, int, TF1FlexCel> TypeRC = PropertyRO_Arr_2D<int, int, int, TF1FlexCel>(this, &TF1FlexCel::TypeRC_Get);
        Property<const char16_t *, TF1FlexCel> Formula = Property<const char16_t *, TF1FlexCel>(this, &TF1FlexCel::Formula_Get, &TF1FlexCel::Formula_Set);
        Property<const char16_t *, TF1FlexCel> NumberFormat = Property<const char16_t *, TF1FlexCel>(this, &TF1FlexCel::NumberFormat_Get, &TF1FlexCel::NumberFormat_Set);
        Property_Arr_1D<int, int, TF1FlexCel> ColWidthTwips = Property_Arr_1D<int, int, TF1FlexCel>(this, &TF1FlexCel::ColWidthTwips_Get, &TF1FlexCel::ColWidthTwips_Set);
        Property_Arr_1D<int, int, TF1FlexCel> ColWidth = Property_Arr_1D<int, int, TF1FlexCel>(this, &TF1FlexCel::ColWidth_Get, &TF1FlexCel::ColWidth_Set);
        Property<int, TF1FlexCel> SelEndCol = Property<int, TF1FlexCel>(this, &TF1FlexCel::SelEndCol_Get, &TF1FlexCel::SelEndCol_Set);
        Property<int, TF1FlexCel> SelEndRow = Property<int, TF1FlexCel>(this, &TF1FlexCel::SelEndRow_Get, &TF1FlexCel::SelEndRow_Set);
        Property<int, TF1FlexCel> SelStartCol = Property<int, TF1FlexCel>(this, &TF1FlexCel::SelStartCol_Get, &TF1FlexCel::SelStartCol_Set);
        Property<int, TF1FlexCel> SelStartRow = Property<int, TF1FlexCel>(this, &TF1FlexCel::SelStartRow_Get, &TF1FlexCel::SelStartRow_Set);
        Property<const char16_t *, TF1FlexCel> FormattedText = Property<const char16_t *, TF1FlexCel>(this, &TF1FlexCel::FormattedText_Get, &TF1FlexCel::FormattedText_Set);
        PropertyRO_Arr_1D<TF1Range, int, TF1FlexCel> SelectionEx = PropertyRO_Arr_1D<TF1Range, int, TF1FlexCel>(this, &TF1FlexCel::SelectionEx_Get);
        Property<bool, TF1FlexCel> EnableProtection = Property<bool, TF1FlexCel>(this, &TF1FlexCel::EnableProtection_Get, &TF1FlexCel::EnableProtection_Set);
        Property<bool, TF1FlexCel> ShowGridLines = Property<bool, TF1FlexCel>(this, &TF1FlexCel::ShowGridLines_Get, &TF1FlexCel::ShowGridLines_Set);
        Property<bool, TF1FlexCel> AutoRecalc = Property<bool, TF1FlexCel>(this, &TF1FlexCel::AutoRecalc_Get, &TF1FlexCel::AutoRecalc_Set);
        Property<int, TF1FlexCel> SelectionCount = Property<int, TF1FlexCel>(this, &TF1FlexCel::SelectionCount_Get, &TF1FlexCel::SelectionCount_Set);
        Property<uint64_t, TF1FlexCel> ColWidthUnits = Property<uint64_t, TF1FlexCel>(this, &TF1FlexCel::ColWidthUnits_Get, &TF1FlexCel::ColWidthUnits_Set);
        Property<bool, TF1FlexCel> AllowResize = Property<bool, TF1FlexCel>(this, &TF1FlexCel::AllowResize_Get, &TF1FlexCel::AllowResize_Set);
        Property<int, TF1FlexCel> HdrWidth = Property<int, TF1FlexCel>(this, &TF1FlexCel::HdrWidth_Get, &TF1FlexCel::HdrWidth_Set);
        Property_Arr_2D<const char16_t *, int, int, TF1FlexCel> FormulaLocalRC = Property_Arr_2D<const char16_t *, int, int, TF1FlexCel>(
                this, &TF1FlexCel::FormulaLocalRC_Get,
                &TF1FlexCel::FormulaLocalRC_Set
        );
        Property_Arr_1D<bool, int, TF1FlexCel> ObjVisible = Property_Arr_1D<bool, int, TF1FlexCel>(this, &TF1FlexCel::ObjVisible_Get, &TF1FlexCel::ObjVisible_Set);
        Property<bool, TF1FlexCel> ShowZeroValues = Property<bool, TF1FlexCel>(this, &TF1FlexCel::ShowZeroValues_Get, &TF1FlexCel::ShowZeroValues_Set);
        Property<uint64_t, TF1FlexCel> ShowSelections = Property<uint64_t, TF1FlexCel>(this, &TF1FlexCel::ShowSelections_Get, &TF1FlexCel::ShowSelections_Set);
        Property<bool, TF1FlexCel> AllowDelete = Property<bool, TF1FlexCel>(this, &TF1FlexCel::AllowDelete_Get, &TF1FlexCel::AllowDelete_Set);
        Property<const char16_t *, TF1FlexCel> DefaultFontName = Property<const char16_t *, TF1FlexCel>(this, &TF1FlexCel::DefaultFontName_Get, &TF1FlexCel::DefaultFontName_Set);
        Property<bool, TF1FlexCel> AllowInCellEditing = Property<bool, TF1FlexCel>(this, &TF1FlexCel::AllowInCellEditing_Get, &TF1FlexCel::AllowInCellEditing_Set);
        Property<bool, TF1FlexCel> ShowLockedCellsError = Property<bool, TF1FlexCel>(this, &TF1FlexCel::ShowLockedCellsError_Get, &TF1FlexCel::ShowLockedCellsError_Set);
        Property<int, TF1FlexCel> DefaultFontSize = Property<int, TF1FlexCel>(this, &TF1FlexCel::DefaultFontSize_Get, &TF1FlexCel::DefaultFontSize_Set);
        Property<bool, TF1FlexCel> MergeCells = Property<bool, TF1FlexCel>(this, &TF1FlexCel::MergeCells_Get, &TF1FlexCel::MergeCells_Set);

    public:
        bool TestBoolean();

        const char16_t *TestString();

        void Save(const char16_t *filename);

        TCellValueType CellValueType_Get(int nRow, int nCol);

        bool DisableAutoSize_Get();

        void DisableAutoSize_Set(bool value);

        bool DisableShrink_Get();

        void DisableShrink_Set(bool value);

        int NumSheets_Get();

        void NumSheets_Set(int value);

        int Sheet_Get();

        void Sheet_Set(int value);

        const char16_t *TextRC_Get(int nRow, int nCol);

        void TextRC_Set(int nRow, int nCol, const char16_t *value);

        double NumberRC_Get(int nRow, int nCol);

        void NumberRC_Set(int nRow, int nCol, double value);

        bool ColHidden_Get(int nCol);

        bool RowHidden_Get(int nRow);

        void ColHidden_Set(int nCol, bool value);

        void RowHidden_Set(int nRow, bool value);

        int Col_Get();

        int Row_Get();

        void Col_Set(int value);

        void Row_Set(int value);

        const char16_t *ObjName_Get(int ObjID);

        void ObjName_Set(int ObjID, const char16_t *value);

        const char16_t *ObjText_Get(int ObjID);

        void ObjText_Set(int ObjID, const char16_t *value);

        int MaxCol_Get();

        int MaxRow_Get();

        void MaxCol_Set(int value);

        void MaxRow_Set(int value);

        int LastCol_Get();

        int LastRow_Get();

        void LastCol_Set(int value);

        void LastRow_Set(int value);

        const char16_t *SheetName_Get(int nSheet);

        void SheetName_Set(int nSheet, const char16_t *value);

        int FixedCols_Get();

        void FixedCols_Set(int value);

        int FixedRows_Get();

        void FixedRows_Set(int value);

        const char16_t *ColText_Get(int nCol);

        void ColText_Set(int nCol, const char16_t *value);

        double NumberSRC_Get(int nSheet, int nRow, int nCol);

        void NumberSRC_Set(int nSheet, int nRow, int nCol, double value);

        const char16_t *TextSRC_Get(int nSheet, int nRow, int nCol);

        void TextSRC_Set(int nSheet, int nRow, int nCol, const char16_t *value);

        const char16_t *FontName_Get();

        void FontName_Set(const char16_t *value);

        bool FontBold_Get();

        bool FontItalic_Get();

        int FontSize_Get();

        void FontBold_Set(bool value);

        void FontItalic_Set(bool value);

        void FontSize_Set(int value);

        // TODO: FontColor_Get возвращает фигню.
        /**
         * Не работает как надо. Запросив несколько раз подряд этот метод, получаешь два разных результата. Оба не те, что задавал методом FontColor_Set.
         * <br/>Первый вызов - один результат. Дальнейшие - второй.
         * @return Int представление rgb цвета.
         */
        int FontColor_Get();

        void FontColor_Set(int value);

        bool FontUnderline_Get();

        void FontUnderline_Set(bool value);

        bool FontStrikeout_Get();

        void FontStrikeout_Set(bool value);

        const char16_t *FormattedText_Get();

        const char16_t *Formula_Get();

        uint64_t HAlign_Get();

        int SelEndCol_Get();

        int SelEndRow_Get();

        int SelStartCol_Get();

        int SelStartRow_Get();

        const char16_t *Text_Get();

        int Type_Get();

        uint64_t VAlign_Get();

        bool WordWrap_Get();

        void FormattedText_Set(const char16_t *value);

        void Formula_Set(const char16_t *value);

        void HAlign_Set(const uint64_t value);

        void SelEndCol_Set(int value);

        void SelEndRow_Set(int value);

        void SelStartCol_Set(int value);

        void SelStartRow_Set(int value);

        void Text_Set(const char16_t *value);

        void Type_Set(int value);

        void VAlign_Set(uint64_t value);

        void WordWrap_Set(bool value);

        int ColWidth_Get(int nCol);

        void ColWidth_Set(int nCol, int value);

        int RowHeight_Get(int nRow);

        void RowHeight_Set(int nRow, int value);

        double Number_Get();

        void Number_Set(const double value);

        int ColWidthTwips_Get(int nCol);

        const char16_t *FormattedTextRC_Get(int nRow, int nCol);

        const char16_t *FormulaRC_Get(int nRow, int nCol);

        const char16_t *NumberFormat_Get();

        int TypeRC_Get(int nRow, int nCol);

        void ColWidthTwips_Set(int nCol, int value);

        /**
         * Важный момент. Если нужно указать в формуле именно целое число, то указывать стоит с точкой, например 125.0.
         * <br>Иначе при получении этой формулы методом FormulaRC_Get, получим непонятный набор байтов.
         * @param nRow Строка. Нумерация с 1.
         * @param nCol Колонка. Нумерация с 1.
         * @param value Значение формулы без знака "=". Если формула со строкой, кавычки экранируем.
         * <br/>Например, если хотим вписать формулу ="привет", передать сюда надо u"\\"привет\\"".
         */
        void FormulaRC_Set(int nRow, int nCol, const char16_t *value);

        void NumberFormat_Set(const char16_t *value);

        uint8_t FontCharSet_Get();

        void FontCharSet_Set(uint8_t value);

        TF1Range SelectionEx_Get(int nSelection);

        bool EnableProtection_Get();

        void EnableProtection_Set(bool value);

        bool ShowGridLines_Get();

        void ShowGridLines_Set(bool value);

        bool AutoRecalc_Get();

        void AutoRecalc_Set(bool value);

        int SelectionCount_Get();

        void SelectionCount_Set(int value);

        uint64_t ColWidthUnits_Get();

        void ColWidthUnits_Set(uint64_t value);

        bool AllowResize_Get();

        void AllowResize_Set(bool value);

        int HdrWidth_Get();

        void HdrWidth_Set(int value);

        const char16_t *FormulaLocalRC_Get(int nRow, int nCol);

        void FormulaLocalRC_Set(int nRow, int nCol, const char16_t *value);

        bool ObjVisible_Get(int ObjID);

        void ObjVisible_Set(int ObjID, bool value);

        bool ShowZeroValues_Get();

        void ShowZeroValues_Set(bool value);

        bool AllowDelete_Get();

        bool AllowInCellEditing_Get();

        const char16_t *DefaultFontName_Get();

        bool ShowLockedCellsError_Get();

        uint64_t ShowSelections_Get();

        void AllowDelete_Set(bool value);

        void AllowInCellEditing_Set(bool value);

        void DefaultFontName_Set(const char16_t *value);

        void ShowLockedCellsError_Set(bool value);

        void ShowSelections_Set(uint64_t value);

        int DefaultFontSize_Get();

        void DefaultFontSize_Set(int value);

        void CellFormat_Set(int XF);

        int CellFormat_Get();

        bool MergeCells_Get();

        void MergeCells_Set(bool pMergeCells);

        TMWSPageSetup PageSetup_Get();

        void PageSetup_Set(TMWSPageSetup pageSetup);

        void Print();

        void PrintPage();

        void PrintTitlesFromSelection_Set();

        /**
         * Сохранение spredsheet'а в pdf-документ.
         * @param filename Имя файла, в который будет записан документ
         * @param documentProperties список свойств документа в виде json-строки
         */
        void SaveToPDF(const char16_t *filename, const char16_t *documentProperties = u"");

        void Recalc();

        void DeleteSheets(int nSheet, int nSheets);

        void Selection_Set(int nRow1, int nCol1, int nRow2, int nCol2);

        void Protection_Get(bool &pLocked, bool &pHidden);

        void Selection_Get(int nSelection, int &pR1, int &pC1, int &pR2, int &pC2);

        uint64_t ObjGetType(int ObjID);

        int ObjNextID(int ObjID);

        int ObjFirstID(int nSheet, int nRow, int nCol, char16_t &aCaption);

        void InsertRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t InsertType);

        void InsertSheets(int nSheet, int nSheets);

        void EditInsertSheets();

        void Border_Set(int nOutline, int nLeft, int nRight, int nTop, int nBottom, int nShade, int crOutline, uint32_t crLeft, uint32_t crRight, uint32_t crTop, uint32_t crBottom);

        void Border_Get(int &pLeft, int &pRight, int &pTop, int &pBottom, int &pShade, int &pcrLeft, int &pcrRight, int &pcrTop, int &pcrBottom);

        void DrawBorders(int row1, int col1, int row2, int col2, int borderStyle, int borderColor, bool exteriorBorders);

        void Pattern_Get(int &pPattern, int &pcrFG, int &pcrBG);

        void ColWidthAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults);

        void Pattern_Set(int nPattern, uint32_t crFG, uint32_t crBG);

        void BackgroundColor_Set(uint32_t crBG);

        void RowHeightAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults);

        void Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, const char16_t *keysString);

        void Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, SortKeys Keys);

        void Protection_Set(bool bLocked, bool bHidden);

        void ColHidden_Set(int nCol1, int nCol2, bool bHidden);

        void ActiveCell_Set(int nRow, int nCol);

        void ShowActiveCell();

        void CopyRange(int nDstR1, int nDstC1, int nDstR2, int nDstC2, int hSrcSS, int nSrcR1, int nSrcC1, int nSrcR2, int nSrcC2);

        int SS();

        void Font_Set(const char16_t *pName, int nSize, bool bBold, bool bItalic, bool bUnderline, bool bStrikeout, uint32_t crColor, bool bOutline, bool bShadow);

        void Font_Get(char16_t &pName, int &pSize, bool &pBold, bool &pItalic, bool &pUnderline, bool &pStrikeout, int &pcrColor, bool &pOutline, bool &pShadow);

        void DeleteRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ShiftType);

        void ClearRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ClearType);

        void EditClear(uint64_t ClearType);

        void BorderStyle_Set(int StyleTopBorder, int StyleLeftBorder, int StyleBottomBorder, int StyleRightBorder);

        void BorderStyle_Get(int &StyleTopBorder, int &StyleLeftBorder, int &StyleBottomBorder, int &StyleRightBorder);

        // TODO: Как-то непонятно работает. То красит только верх, то вообще ничего не красит. Цвета надо переворачивать.
        /**
         *
         * @param ColorTopBorder
         * @param ColorLeftBorder
         * @param ColorBottomBorder
         * @param ColorRightBorder
         */
        void BorderColor_Set(int ColorTopBorder, int ColorLeftBorder, int ColorBottomBorder, int ColorRightBorder);

        // TODO: В Delphi для всех 3 аргументов идет такое: pcrLeft := 100. Поэтому переворачивать тут не надо. Это просто Blue = 100.
        /**
         *
         * @param ColorTopBorder
         * @param ColorLeftBorder
         * @param ColorBottomBorder
         * @param ColorRightBorder
         */
        void BorderColor_Get(int &ColorTopBorder, int &ColorLeftBorder, int &ColorBottomBorder, int &ColorRightBorder);

        int InputButton(int nRow1, int nCol1, int nRow2, int nCol2, const char16_t *aCaption, const char16_t *aObjName);

        void BorderInfo_Get(int nRow, int nCol, int &BorderTopStyle, int &BorderLeftStyle, int &BorderBottomStyle, int &BorderRightStyle, int &BorderTopColor, int &BorderLeftColor,
                           int &BorderBottomColor, int &BorderRightColor);

        int HAlignRC_Get(int nRow, int nCol);

        THFlxAlignment toHAlign(int aH);

        TVFlxAlignment toVAlign(int aV);

        int VAlignRC_Get(int nRow, int nCol);

        int objlistCount_Get();

        void objlistI_PropRC_Get(int aI, int &nRow, int &nCol, char16_t &objectName);

        int NFormatRC_Get(int nRow, int nCol);

        const char16_t *StyleNameRC_Get(int nRow, int nCol);

        uint32_t BackColorRC_Get(int nRow, int nCol);

        void CellInfoRC_Get(int nRow, int nCol, char16_t &Format, bool &Locked, bool &WrapText);

        void FontInfoRC_Get(int nRow, int nCol, uint32_t &Color, char16_t &FontName, int &FontSize, int &Underline, bool &Bold, bool &Italic);

        void F1CellFormat_Set(
                bool isAlignHorizontal,
                int AlignHorizontal_,
                bool isWordWrap,
                bool WordWrap_,
                bool isAlignVertical,
                int AlignVertical_,
                bool isBorderStyle,
                int BorderStyle_,
                int pShade_,
                bool isBorderColor,
                int BorderColor_,
                bool isFontName,
                const char16_t *FontName_,
                bool isFontSize,
                int FontSize_,
                bool isFontBold,
                bool FontBold_,
                bool isFontItalic,
                bool FontItalic_,
                bool isFontUnderline,
                bool FontUnderline_,
                bool isFontStrikeout,
                bool FontStrikeout_,
                bool isFontColor,
                int FontColor_,
                bool isPatternStyle,
                int PatternStyle_,
                int PatternBG_,
                int PatternFG_,
                bool isProtectionLocked,
                bool ProtectionLocked_,
                bool ProtectionHidden_,
                bool isNumberFormat,
                const char16_t *NumberFormat_,
                bool MergeCells_,
                bool isRotation,
                int Rotation_
        );

        int NVisibleCol_Get(int &nVisibleCol1);

        double FormulaResultNumberRC(int nRow, int nCol);

        const char16_t *FormulaResultTextRC(int nRow, int nCol);
    };
}

#endif //F1FLEXCEL_CPPPROJECT_TF1FLEXCEL_H
