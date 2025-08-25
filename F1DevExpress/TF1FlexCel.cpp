#include "TF1FlexCel.h"
#include "TracerManager.h"

#ifndef WIN32

#define LOAD_LIBRARY_FILE "libF1FlexCel.so"

#include <dlfcn.h>

#else

#define LOAD_LIBRARY_FILE "F1FlexCel64.dll"
//#define LOAD_LIBRARY_FILE "C:\\Users\\Alex\\Desktop\\MWSRestAPI v2\\f1flexcel\\windows\\lib\\F1FlexCel64.dll

#include <windows.h>
#include <string>

#endif

namespace F1DevExpress::unF1FlexCel {
    using namespace std;

    TF1FlexCel::TF1FlexCel() {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::LibraryHandleConstructor();
        key = TF1FlexCel::libCreateDefault();
        TracerManager::Instance().EndF1LibTrace();
    }

    TF1FlexCel::TF1FlexCel(const char16_t *filename) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::LibraryHandleConstructor();
        key = TF1FlexCel::libCreateFromFile(filename);
        TracerManager::Instance().EndF1LibTrace();
    }

    TF1FlexCel::TF1FlexCel(int sheetCount) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::LibraryHandleConstructor();
        key = TF1FlexCel::libCreateWithSheetCount(sheetCount);
        TracerManager::Instance().EndF1LibTrace();
    }

    TF1FlexCel::~TF1FlexCel() {
        Free();
        TF1FlexCel::LibraryHandleDestructor();
    }

    void TF1FlexCel::LibraryHandleConstructor() {
        if (TF1FlexCel::FlexCelInstancesCount > 0) {
            TF1FlexCel::FlexCelInstancesCount++;
        } else {
            TF1FlexCel::LoadLib();
            TF1FlexCel::FlexCelInstancesCount = 1;
        }
    }

    void TF1FlexCel::LibraryHandleDestructor() {
        if (TF1FlexCel::FlexCelInstancesCount > 0) {
            if (--TF1FlexCel::FlexCelInstancesCount <= 0) {
                TF1FlexCel::UnloadLib();
            }
        }
    }

    void TF1FlexCel::LoadLib() {
#ifndef WIN32
        TF1FlexCel::LibHandle = dlopen(LOAD_LIBRARY_FILE, RTLD_LAZY);
        if (!TF1FlexCel::LibHandle) {
            fprintf(stderr, "%s\n", dlerror());
            exit(1);
        }
        dlerror();
#else
        TF1FlexCel::LibHandle = LoadLibrary(LOAD_LIBRARY_FILE);
        if (!TF1FlexCel::LibHandle) {
            printf("cannot open library");
            exit(1);
        }
#endif

        TF1FlexCel::HandleLibraryMethod("TestBoolean", &TF1FlexCel::libTestBoolean);
        TF1FlexCel::HandleLibraryMethod("TestString", &TF1FlexCel::libTestString);

        TF1FlexCel::HandleLibraryMethod("CreateDefault", &TF1FlexCel::libCreateDefault);
        TF1FlexCel::HandleLibraryMethod("CreateFromFile", &TF1FlexCel::libCreateFromFile);
        TF1FlexCel::HandleLibraryMethod("CreateWithSheetCount", &TF1FlexCel::libCreateWithSheetCount);
        TF1FlexCel::HandleLibraryMethod("Free", &TF1FlexCel::libFree);
        TF1FlexCel::HandleLibraryMethod("Save", &TF1FlexCel::libSave);
        TF1FlexCel::HandleLibraryMethod("GetCellValueType", &TF1FlexCel::libGetCellValueType);
        TF1FlexCel::HandleLibraryMethod("GetDisableAutoSize", &TF1FlexCel::libGetDisableAutoSize);
        TF1FlexCel::HandleLibraryMethod("SetDisableAutoSize", &TF1FlexCel::libSetDisableAutoSize);
        TF1FlexCel::HandleLibraryMethod("GetDisableShrink", &TF1FlexCel::libGetDisableShrink);
        TF1FlexCel::HandleLibraryMethod("SetDisableShrink", &TF1FlexCel::libSetDisableShrink);

        TF1FlexCel::HandleLibraryMethod("GetNumSheets", &TF1FlexCel::libGetNumSheets);
        TF1FlexCel::HandleLibraryMethod("SetNumSheets", &TF1FlexCel::libSetNumSheets);
        TF1FlexCel::HandleLibraryMethod("GetSheet", &TF1FlexCel::libGetSheet);
        TF1FlexCel::HandleLibraryMethod("SetSheet", &TF1FlexCel::libSetSheet);
        TF1FlexCel::HandleLibraryMethod("Get_TextRC", &TF1FlexCel::libGet_TextRC);
        TF1FlexCel::HandleLibraryMethod("Set_TextRC", &TF1FlexCel::libSet_TextRC);
        TF1FlexCel::HandleLibraryMethod("Get_NumberRC", &TF1FlexCel::libGet_NumberRC);
        TF1FlexCel::HandleLibraryMethod("Set_NumberRC", &TF1FlexCel::libSet_NumberRC);
        TF1FlexCel::HandleLibraryMethod("Get_ColHidden", &TF1FlexCel::libGet_ColHidden);
        TF1FlexCel::HandleLibraryMethod("Get_RowHidden", &TF1FlexCel::libGet_RowHidden);
        TF1FlexCel::HandleLibraryMethod("Set_ColHidden", &TF1FlexCel::libSet_ColHidden);
        TF1FlexCel::HandleLibraryMethod("Set_RowHidden", &TF1FlexCel::libSet_RowHidden);
        TF1FlexCel::HandleLibraryMethod("GetCol", &TF1FlexCel::libGetCol);
        TF1FlexCel::HandleLibraryMethod("GetRow", &TF1FlexCel::libGetRow);
        TF1FlexCel::HandleLibraryMethod("SetCol", &TF1FlexCel::libSetCol);
        TF1FlexCel::HandleLibraryMethod("SetRow", &TF1FlexCel::libSetRow);
        TF1FlexCel::HandleLibraryMethod("Get_ObjName", &TF1FlexCel::libGet_ObjName);
        TF1FlexCel::HandleLibraryMethod("Set_ObjName", &TF1FlexCel::libSet_ObjName);
        TF1FlexCel::HandleLibraryMethod("Get_ObjText", &TF1FlexCel::libGet_ObjText);
        TF1FlexCel::HandleLibraryMethod("Set_ObjText", &TF1FlexCel::libSet_ObjText);
        TF1FlexCel::HandleLibraryMethod("GetMaxCol", &TF1FlexCel::libGetMaxCol);
        TF1FlexCel::HandleLibraryMethod("GetMaxRow", &TF1FlexCel::libGetMaxRow);
        TF1FlexCel::HandleLibraryMethod("SetMaxCol", &TF1FlexCel::libSetMaxCol);
        TF1FlexCel::HandleLibraryMethod("SetMaxRow", &TF1FlexCel::libSetMaxRow);
        TF1FlexCel::HandleLibraryMethod("GetLastCol", &TF1FlexCel::libGetLastCol);
        TF1FlexCel::HandleLibraryMethod("GetLastRow", &TF1FlexCel::libGetLastRow);
        TF1FlexCel::HandleLibraryMethod("SetLastCol", &TF1FlexCel::libSetLastCol);
        TF1FlexCel::HandleLibraryMethod("SetLastRow", &TF1FlexCel::libSetLastRow);
        TF1FlexCel::HandleLibraryMethod("Get_SheetName", &TF1FlexCel::libGet_SheetName);
        TF1FlexCel::HandleLibraryMethod("Set_SheetName", &TF1FlexCel::libSet_SheetName);
        TF1FlexCel::HandleLibraryMethod("GetFixedCols", &TF1FlexCel::libGetFixedCols);
        TF1FlexCel::HandleLibraryMethod("SetFixedCols", &TF1FlexCel::libSetFixedCols);
        TF1FlexCel::HandleLibraryMethod("GetFixedRows", &TF1FlexCel::libGetFixedRows);
        TF1FlexCel::HandleLibraryMethod("SetFixedRows", &TF1FlexCel::libSetFixedRows);
        TF1FlexCel::HandleLibraryMethod("Get_ColText", &TF1FlexCel::libGet_ColText);
        TF1FlexCel::HandleLibraryMethod("Set_ColText", &TF1FlexCel::libSet_ColText);
        TF1FlexCel::HandleLibraryMethod("Get_NumberSRC", &TF1FlexCel::libGet_NumberSRC);
        TF1FlexCel::HandleLibraryMethod("Set_NumberSRC", &TF1FlexCel::libSet_NumberSRC);
        TF1FlexCel::HandleLibraryMethod("Get_TextSRC", &TF1FlexCel::libGet_TextSRC);
        TF1FlexCel::HandleLibraryMethod("Set_TextSRC", &TF1FlexCel::libSet_TextSRC);
        TF1FlexCel::HandleLibraryMethod("GetFontName", &TF1FlexCel::libGetFontName);
        TF1FlexCel::HandleLibraryMethod("SetFontName", &TF1FlexCel::libSetFontName);
        TF1FlexCel::HandleLibraryMethod("GetFontBold", &TF1FlexCel::libGetFontBold);
        TF1FlexCel::HandleLibraryMethod("GetFontItalic", &TF1FlexCel::libGetFontItalic);
        TF1FlexCel::HandleLibraryMethod("GetFontSize", &TF1FlexCel::libGetFontSize);
        TF1FlexCel::HandleLibraryMethod("SetFontBold", &TF1FlexCel::libSetFontBold);
        TF1FlexCel::HandleLibraryMethod("SetFontItalic", &TF1FlexCel::libSetFontItalic);
        TF1FlexCel::HandleLibraryMethod("SetFontSize", &TF1FlexCel::libSetFontSize);
        TF1FlexCel::HandleLibraryMethod("GetFontColor", &TF1FlexCel::libGetFontColor);
        TF1FlexCel::HandleLibraryMethod("SetFontColor", &TF1FlexCel::libSetFontColor);
        TF1FlexCel::HandleLibraryMethod("GetFontUnderline", &TF1FlexCel::libGetFontUnderline);
        TF1FlexCel::HandleLibraryMethod("SetFontUnderline", &TF1FlexCel::libSetFontUnderline);
        TF1FlexCel::HandleLibraryMethod("GetFontStrikeout", &TF1FlexCel::libGetFontStrikeout);
        TF1FlexCel::HandleLibraryMethod("SetFontStrikeout", &TF1FlexCel::libSetFontStrikeout);
        TF1FlexCel::HandleLibraryMethod("GetFormattedText", &TF1FlexCel::libGetFormattedText);
        TF1FlexCel::HandleLibraryMethod("GetFormula", &TF1FlexCel::libGetFormula);
        TF1FlexCel::HandleLibraryMethod("GetHAlign", &TF1FlexCel::libGetHAlign);
        TF1FlexCel::HandleLibraryMethod("GetSelEndCol", &TF1FlexCel::libGetSelEndCol);
        TF1FlexCel::HandleLibraryMethod("GetSelEndRow", &TF1FlexCel::libGetSelEndRow);
        TF1FlexCel::HandleLibraryMethod("GetSelStartCol", &TF1FlexCel::libGetSelStartCol);
        TF1FlexCel::HandleLibraryMethod("GetSelStartRow", &TF1FlexCel::libGetSelStartRow);
        TF1FlexCel::HandleLibraryMethod("GetText", &TF1FlexCel::libGetText);
        TF1FlexCel::HandleLibraryMethod("GetType_", &TF1FlexCel::libGetType_);
        TF1FlexCel::HandleLibraryMethod("GetVAlign", &TF1FlexCel::libGetVAlign);
        TF1FlexCel::HandleLibraryMethod("GetWordWrap", &TF1FlexCel::libGetWordWrap);
        TF1FlexCel::HandleLibraryMethod("SetFormattedText", &TF1FlexCel::libSetFormattedText);
        TF1FlexCel::HandleLibraryMethod("SetFormula", &TF1FlexCel::libSetFormula);
        TF1FlexCel::HandleLibraryMethod("SetHAlign", &TF1FlexCel::libSetHAlign);
        TF1FlexCel::HandleLibraryMethod("SetSelEndCol", &TF1FlexCel::libSetSelEndCol);
        TF1FlexCel::HandleLibraryMethod("SetSelEndRow", &TF1FlexCel::libSetSelEndRow);
        TF1FlexCel::HandleLibraryMethod("SetSelStartCol", &TF1FlexCel::libSetSelStartCol);
        TF1FlexCel::HandleLibraryMethod("SetSelStartRow", &TF1FlexCel::libSetSelStartRow);
        TF1FlexCel::HandleLibraryMethod("SetText", &TF1FlexCel::libSetText);
        TF1FlexCel::HandleLibraryMethod("SetType_", &TF1FlexCel::libSetType_);
        TF1FlexCel::HandleLibraryMethod("SetVAlign", &TF1FlexCel::libSetVAlign);
        TF1FlexCel::HandleLibraryMethod("SetWordWrap", &TF1FlexCel::libSetWordWrap);
        TF1FlexCel::HandleLibraryMethod("Get_ColWidth", &TF1FlexCel::libGet_ColWidth);
        TF1FlexCel::HandleLibraryMethod("Set_ColWidth", &TF1FlexCel::libSet_ColWidth);
        TF1FlexCel::HandleLibraryMethod("Get_RowHeight", &TF1FlexCel::libGet_RowHeight);
        TF1FlexCel::HandleLibraryMethod("Set_RowHeight", &TF1FlexCel::libSet_RowHeight);
        TF1FlexCel::HandleLibraryMethod("GetNumber", &TF1FlexCel::libGetNumber);
        TF1FlexCel::HandleLibraryMethod("SetNumber", &TF1FlexCel::libSetNumber);
        TF1FlexCel::HandleLibraryMethod("Get_ColWidthTwips", &TF1FlexCel::libGet_ColWidthTwips);
        TF1FlexCel::HandleLibraryMethod("Get_FormattedTextRC", &TF1FlexCel::libGet_FormattedTextRC);
        TF1FlexCel::HandleLibraryMethod("Get_FormulaRC", &TF1FlexCel::libGet_FormulaRC);
        TF1FlexCel::HandleLibraryMethod("Get_NumberFormat", &TF1FlexCel::libGet_NumberFormat);
        TF1FlexCel::HandleLibraryMethod("Get_TypeRC", &TF1FlexCel::libGet_TypeRC);
        TF1FlexCel::HandleLibraryMethod("Set_ColWidthTwips", &TF1FlexCel::libSet_ColWidthTwips);
        TF1FlexCel::HandleLibraryMethod("Set_FormulaRC", &TF1FlexCel::libSet_FormulaRC);
        TF1FlexCel::HandleLibraryMethod("Set_NumberFormat", &TF1FlexCel::libSet_NumberFormat);
        TF1FlexCel::HandleLibraryMethod("GetFontCharSet", &TF1FlexCel::libGetFontCharSet);
        TF1FlexCel::HandleLibraryMethod("SetFontCharSet", &TF1FlexCel::libSetFontCharSet);
        TF1FlexCel::HandleLibraryMethod("Get_SelectionEx", &TF1FlexCel::libGet_SelectionEx);
        TF1FlexCel::HandleLibraryMethod("GetEnableProtection", &TF1FlexCel::libGetEnableProtection);
        TF1FlexCel::HandleLibraryMethod("SetEnableProtection", &TF1FlexCel::libSetEnableProtection);
        TF1FlexCel::HandleLibraryMethod("GetShowGridLines", &TF1FlexCel::libGetShowGridLines);
        TF1FlexCel::HandleLibraryMethod("SetShowGridLines", &TF1FlexCel::libSetShowGridLines);
        TF1FlexCel::HandleLibraryMethod("GetAutoRecalc", &TF1FlexCel::libGetAutoRecalc);
        TF1FlexCel::HandleLibraryMethod("SetAutoRecalc", &TF1FlexCel::libSetAutoRecalc);
        TF1FlexCel::HandleLibraryMethod("GetSelectionCount", &TF1FlexCel::libGetSelectionCount);
        TF1FlexCel::HandleLibraryMethod("SetSelectionCount", &TF1FlexCel::libSetSelectionCount);
        TF1FlexCel::HandleLibraryMethod("GetColWidthUnits", &TF1FlexCel::libGetColWidthUnits);
        TF1FlexCel::HandleLibraryMethod("SetColWidthUnits", &TF1FlexCel::libSetColWidthUnits);
        TF1FlexCel::HandleLibraryMethod("GetAllowResize", &TF1FlexCel::libGetAllowResize);
        TF1FlexCel::HandleLibraryMethod("SetAllowResize", &TF1FlexCel::libSetAllowResize);
        TF1FlexCel::HandleLibraryMethod("GetHdrWidth", &TF1FlexCel::libGetHdrWidth);
        TF1FlexCel::HandleLibraryMethod("SetHdrWidth", &TF1FlexCel::libSetHdrWidth);
        TF1FlexCel::HandleLibraryMethod("Get_FormulaLocalRC", &TF1FlexCel::libGet_FormulaLocalRC);
        TF1FlexCel::HandleLibraryMethod("Set_FormulaLocalRC", &TF1FlexCel::libSet_FormulaLocalRC);
        TF1FlexCel::HandleLibraryMethod("Get_ObjVisible", &TF1FlexCel::libGet_ObjVisible);
        TF1FlexCel::HandleLibraryMethod("Set_ObjVisible", &TF1FlexCel::libSet_ObjVisible);
        TF1FlexCel::HandleLibraryMethod("GetShowZeroValues", &TF1FlexCel::libGetShowZeroValues);
        TF1FlexCel::HandleLibraryMethod("SetShowZeroValues", &TF1FlexCel::libSetShowZeroValues);
        TF1FlexCel::HandleLibraryMethod("GetAllowDelete", &TF1FlexCel::libGetAllowDelete);
        TF1FlexCel::HandleLibraryMethod("GetAllowInCellEditing", &TF1FlexCel::libGetAllowInCellEditing);
        TF1FlexCel::HandleLibraryMethod("GetDefaultFontName", &TF1FlexCel::libGetDefaultFontName);
        TF1FlexCel::HandleLibraryMethod("GetShowLockedCellsError", &TF1FlexCel::libGetShowLockedCellsError);
        TF1FlexCel::HandleLibraryMethod("GetShowSelections", &TF1FlexCel::libGetShowSelections);
        TF1FlexCel::HandleLibraryMethod("SetAllowDelete", &TF1FlexCel::libSetAllowDelete);
        TF1FlexCel::HandleLibraryMethod("SetAllowInCellEditing", &TF1FlexCel::libSetAllowInCellEditing);
        TF1FlexCel::HandleLibraryMethod("SetDefaultFontName", &TF1FlexCel::libSetDefaultFontName);
        TF1FlexCel::HandleLibraryMethod("SetShowLockedCellsError", &TF1FlexCel::libSetShowLockedCellsError);
        TF1FlexCel::HandleLibraryMethod("SetShowSelections", &TF1FlexCel::libSetShowSelections);
        TF1FlexCel::HandleLibraryMethod("GetDefaultFontSize", &TF1FlexCel::libGetDefaultFontSize);
        TF1FlexCel::HandleLibraryMethod("SetDefaultFontSize", &TF1FlexCel::libSetDefaultFontSize);
        TF1FlexCel::HandleLibraryMethod("SetCellFormat", &TF1FlexCel::libSetCellFormat);
        TF1FlexCel::HandleLibraryMethod("GetCellFormat", &TF1FlexCel::libGetCellFormat);
        TF1FlexCel::HandleLibraryMethod("Get_MergeCells", &TF1FlexCel::libGet_MergeCells);
        TF1FlexCel::HandleLibraryMethod("Set_MergeCells", &TF1FlexCel::libSet_MergeCells);
        // Get-метод не нужен, т.к. его суть это создание объекта TMWSPageSetup
//        TF1FlexCel::HandleLibraryMethod("GetPageSetup", &TF1FlexCel::libGetPageSetup);
        TF1FlexCel::HandleLibraryMethod("SetPageSetup", &TF1FlexCel::libSetPageSetup);
        TF1FlexCel::HandleLibraryMethod("Print", &TF1FlexCel::libPrint);
        TF1FlexCel::HandleLibraryMethod("PrintPage", &TF1FlexCel::libPrintPage);
        TF1FlexCel::HandleLibraryMethod("SetPrintTitlesFromSelection", &TF1FlexCel::libSetPrintTitlesFromSelection);
        TF1FlexCel::HandleLibraryMethod("SaveToPDF", &TF1FlexCel::libSaveToPDF);
        TF1FlexCel::HandleLibraryMethod("Recalc", &TF1FlexCel::libRecalc);
        TF1FlexCel::HandleLibraryMethod("DeleteSheets", &TF1FlexCel::libDeleteSheets);
        TF1FlexCel::HandleLibraryMethod("SetSelection", &TF1FlexCel::libSetSelection);
        TF1FlexCel::HandleLibraryMethod("GetProtection", &TF1FlexCel::libGetProtection);
        TF1FlexCel::HandleLibraryMethod("GetSelection", &TF1FlexCel::libGetSelection);
        TF1FlexCel::HandleLibraryMethod("ObjGetType", &TF1FlexCel::libObjGetType);
        TF1FlexCel::HandleLibraryMethod("ObjNextID", &TF1FlexCel::libObjNextID);
        TF1FlexCel::HandleLibraryMethod("ObjFirstID", &TF1FlexCel::libObjFirstID);
        TF1FlexCel::HandleLibraryMethod("InsertRange", &TF1FlexCel::libInsertRange);
        TF1FlexCel::HandleLibraryMethod("InsertSheets", &TF1FlexCel::libInsertSheets);
        TF1FlexCel::HandleLibraryMethod("EditInsertSheets", &TF1FlexCel::libEditInsertSheets);
        TF1FlexCel::HandleLibraryMethod("SetBorder", &TF1FlexCel::libSetBorder);
        TF1FlexCel::HandleLibraryMethod("GetBorder", &TF1FlexCel::libGetBorder);
        TF1FlexCel::HandleLibraryMethod("DrawBorders", &TF1FlexCel::libDrawBorders);
        TF1FlexCel::HandleLibraryMethod("GetPattern", &TF1FlexCel::libGetPattern);
        TF1FlexCel::HandleLibraryMethod("SetColWidthAuto", &TF1FlexCel::libSetColWidthAuto);
        TF1FlexCel::HandleLibraryMethod("SetPattern", &TF1FlexCel::libSetPattern);
        TF1FlexCel::HandleLibraryMethod("SetBackgroundColor", &TF1FlexCel::libSetBackgroundColor);
        TF1FlexCel::HandleLibraryMethod("SetRowHeightAuto", &TF1FlexCel::libSetRowHeightAuto);
        TF1FlexCel::HandleLibraryMethod("Sort", &TF1FlexCel::libSort);
        TF1FlexCel::HandleLibraryMethod("SetProtection", &TF1FlexCel::libSetProtection);
        TF1FlexCel::HandleLibraryMethod("SetColHidden", &TF1FlexCel::libSetColHidden);
        TF1FlexCel::HandleLibraryMethod("SetActiveCell", &TF1FlexCel::libSetActiveCell);
        TF1FlexCel::HandleLibraryMethod("ShowActiveCell", &TF1FlexCel::libShowActiveCell);
        TF1FlexCel::HandleLibraryMethod("CopyRange", &TF1FlexCel::libCopyRange);
        TF1FlexCel::HandleLibraryMethod("SS", &TF1FlexCel::libSS);
        TF1FlexCel::HandleLibraryMethod("SetFont", &TF1FlexCel::libSetFont);
        TF1FlexCel::HandleLibraryMethod("GetFont", &TF1FlexCel::libGetFont);
        TF1FlexCel::HandleLibraryMethod("DeleteRange", &TF1FlexCel::libDeleteRange);
        TF1FlexCel::HandleLibraryMethod("ClearRange", &TF1FlexCel::libClearRange);
        TF1FlexCel::HandleLibraryMethod("EditClear", &TF1FlexCel::libEditClear);
        TF1FlexCel::HandleLibraryMethod("setBorderStyle", &TF1FlexCel::libsetBorderStyle);
        TF1FlexCel::HandleLibraryMethod("getBorderStyle", &TF1FlexCel::libgetBorderStyle);
        TF1FlexCel::HandleLibraryMethod("setBorderColor", &TF1FlexCel::libsetBorderColor);
        TF1FlexCel::HandleLibraryMethod("getBorderColor", &TF1FlexCel::libgetBorderColor);
        TF1FlexCel::HandleLibraryMethod("InputButton", &TF1FlexCel::libInputButton);
        TF1FlexCel::HandleLibraryMethod("getBorderInfo", &TF1FlexCel::libgetBorderInfo);
        TF1FlexCel::HandleLibraryMethod("GetHAlignRC", &TF1FlexCel::libGetHAlignRC);
//        TF1FlexCel::HandleLibraryMethod("toHAlign", &TF1FlexCel::libtoHAlign);
//        TF1FlexCel::HandleLibraryMethod("toVAlign", &TF1FlexCel::libtoVAlign);
        TF1FlexCel::HandleLibraryMethod("getVAlignRC", &TF1FlexCel::libgetVAlignRC);
        TF1FlexCel::HandleLibraryMethod("GetobjlistCount", &TF1FlexCel::libGetobjlistCount);
        TF1FlexCel::HandleLibraryMethod("GetobjlistI_PropRC", &TF1FlexCel::libGetobjlistI_PropRC);
        TF1FlexCel::HandleLibraryMethod("getNFormatRC", &TF1FlexCel::libgetNFormatRC);
        TF1FlexCel::HandleLibraryMethod("getStyleNameRC", &TF1FlexCel::libgetStyleNameRC);
        TF1FlexCel::HandleLibraryMethod("getBackColorRC", &TF1FlexCel::libgetBackColorRC);
        TF1FlexCel::HandleLibraryMethod("getCellInfoRC", &TF1FlexCel::libgetCellInfoRC);
        TF1FlexCel::HandleLibraryMethod("getFontInfoRC", &TF1FlexCel::libgetFontInfoRC);
        TF1FlexCel::HandleLibraryMethod("SetF1CellFormat", &TF1FlexCel::libSetF1CellFormat);
        TF1FlexCel::HandleLibraryMethod("GetNVisibleCol", &TF1FlexCel::libGetNVisibleCol);
        TF1FlexCel::HandleLibraryMethod("GetTypeRC", &TF1FlexCel::libGetTypeRC);
        TF1FlexCel::HandleLibraryMethod("FormulaResultNumberRC", &TF1FlexCel::libFormulaResultNumberRC);
        TF1FlexCel::HandleLibraryMethod("FormulaResultTextRC", &TF1FlexCel::libFormulaResultTextRC);
    }

    void TF1FlexCel::UnloadLib() {
#ifndef WIN32
        dlclose(TF1FlexCel::LibHandle);
#else
        FreeLibrary((HINSTANCE) TF1FlexCel::LibHandle);
#endif
    }

    void TF1FlexCel::HandleLibraryMethod(const char *methodName, void *linkVar) {
        char *error;

#ifndef WIN32
        *(void **) (linkVar) = dlsym(TF1FlexCel::LibHandle, methodName);
        if ((error = dlerror()) != NULL) {
            fprintf(stderr, "%s\n", error);
            exit(1);
        }
#else
        *(void **) (linkVar) = GetProcAddress((HINSTANCE) TF1FlexCel::LibHandle, methodName);
//        *(void**) (linkVar) = reinterpret_cast<void*>(GetProcAddress((HINSTANCE) TF1FlexCel::LibHandle, methodName));
        if (linkVar == NULL) {
            printf("cannot load function %s\n", methodName);
        }
#endif
    }

    bool TF1FlexCel::TestBoolean() {
        return TF1FlexCel::libTestBoolean(key);
    }

    const char16_t *TF1FlexCel::TestString() {
        return TF1FlexCel::libTestString(key);
    }

    void TF1FlexCel::Free() {
        TF1FlexCel::libFree(key);
    }

    int TF1FlexCel::ColorReverse(int color) {
        TracerManager::Instance().StartF1LibTrace();
        TColorRec colorRec = TColorRec();
        colorRec.color = color;
        int t_ = SystFuncs::SystFuncsAltWin::GetColorFromARGB(colorRec.a, colorRec.b, colorRec.g, colorRec.r);
        //return SystFuncs::SystFuncsAltWin::GetColorFromARGB(colorRec.a, colorRec.b, colorRec.g, colorRec.r);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
    }

    void TF1FlexCel::Save(const char16_t *filename) {
        TF1FlexCel::libSave(key, filename);
    }

    TCellValueType TF1FlexCel::CellValueType_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        TCellValueType t_ = TCellValueType(TF1FlexCel::libGetCellValueType(key, nRow, nCol));
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TCellValueType(TF1FlexCel::libGetCellValueType(key, nRow, nCol));
    }

    bool TF1FlexCel::DisableAutoSize_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetDisableAutoSize(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetDisableAutoSize(key);
    }

    void TF1FlexCel::DisableAutoSize_Set(bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetDisableAutoSize(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::DisableShrink_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetDisableShrink(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetDisableShrink(key);
    }

    void TF1FlexCel::DisableShrink_Set(bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetDisableShrink(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::NumSheets_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = libGetNumSheets(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return libGetNumSheets(key);
    }

    void TF1FlexCel::NumSheets_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetNumSheets(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::Sheet_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetSheet(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetSheet(key);
    }

    void TF1FlexCel::Sheet_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSheet(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::TextRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_TextRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_TextRC(key, nRow, nCol);
    }

    void TF1FlexCel::TextRC_Set(int nRow, int nCol, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_TextRC(key, nRow, nCol, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), nRow, nCol, 2);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    double TF1FlexCel::NumberRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        double t_ = TF1FlexCel::libGet_NumberRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_NumberRC(key, nRow, nCol);
    }

    void TF1FlexCel::NumberRC_Set(int nRow, int nCol, const double value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_NumberRC(key, nRow, nCol, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), nRow, nCol, 1);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::ColHidden_Get(int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        bool  t_ = TF1FlexCel::libGet_ColHidden(key, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ColHidden(key, nCol);
    }

    bool TF1FlexCel::RowHidden_Get(int nRow) {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGet_RowHidden(key, nRow);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_RowHidden(key, nRow);
    }

    void TF1FlexCel::ColHidden_Set(int nCol, const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ColHidden(key, nCol, value);
#ifdef ACTIONVIEW
        ActionView.ColHidden(GetSheet(), nCol, value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::RowHidden_Set(int nRow, const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_RowHidden(key, nRow, value);
#ifdef ACTIONVIEW
        ActionView.RowHidden(GetSheet(), nRow, value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::Col_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetCol(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetCol(key);
    }

    int TF1FlexCel::Row_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetRow(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetRow(key);
    }

    void TF1FlexCel::Col_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetCol(key, value);
#ifdef ACTIONVIEW
        ActionView.Col(value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Row_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetRow(key, value);
#ifdef ACTIONVIEW
        ActionView.Row(value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::ObjName_Get(int ObjID) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ =  TF1FlexCel::libGet_ObjName(key, ObjID);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ObjName(key, ObjID);
    }

    void TF1FlexCel::ObjName_Set(int ObjID, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ObjName(key, ObjID, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::ObjText_Get(int ObjID) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_ObjText(key, ObjID);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ObjText(key, ObjID);
    }

    void TF1FlexCel::ObjText_Set(int ObjID, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ObjText(key, ObjID, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::MaxCol_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetMaxCol(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetMaxCol(key);
    }

    int TF1FlexCel::MaxRow_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetMaxRow(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetMaxRow(key);
    }

    void TF1FlexCel::MaxCol_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetMaxCol(key, value);
#ifdef ACTIONVIEW
        ActionView.MaxCol(value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::MaxRow_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetMaxRow(key, value);
#ifdef ACTIONVIEW
        ActionView.MaxRow(value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::LastCol_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetLastCol(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetLastCol(key);
    }

    int TF1FlexCel::LastRow_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetLastRow(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetLastRow(key);
    }

    void TF1FlexCel::LastCol_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetLastCol(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::LastRow_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetLastRow(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::SheetName_Get(int nSheet) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_SheetName(key, nSheet);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_SheetName(key, nSheet);
    }

    void TF1FlexCel::SheetName_Set(int nSheet, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_SheetName(key, nSheet, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::FixedCols_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetFixedCols(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFixedCols(key);
    }

    void TF1FlexCel::FixedCols_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFixedCols(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::FixedRows_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetFixedRows(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFixedRows(key);
    }

    void TF1FlexCel::FixedRows_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFixedRows(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::ColText_Get(int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_ColText(key, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ColText(key, nCol);
    }

    void TF1FlexCel::ColText_Set(int nCol, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ColText(key, nCol, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    double TF1FlexCel::NumberSRC_Get(int nSheet, int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        double t_ = TF1FlexCel::libGet_NumberSRC(key, nSheet, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_NumberSRC(key, nSheet, nRow, nCol);
    }

    void TF1FlexCel::NumberSRC_Set(int nSheet, int nRow, int nCol, const double value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_NumberSRC(key, nSheet, nRow, nCol, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), nRow, nCol, 1);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::TextSRC_Get(int nSheet, int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_TextSRC(key, nSheet, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_TextSRC(key, nSheet, nRow, nCol);
    }

    void TF1FlexCel::TextSRC_Set(int nSheet, int nRow, int nCol, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_TextSRC(key, nSheet, nRow, nCol, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), nRow, nCol, 2);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::FontName_Get() {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGetFontName(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontName(key);
    }

    void TF1FlexCel::FontName_Set(const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontName(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::FontBold_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetFontBold(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontBold(key);
    }

    bool TF1FlexCel::FontItalic_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetFontItalic(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontItalic(key);
    }

    int TF1FlexCel::FontSize_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetFontSize(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontSize(key);
    }

    void TF1FlexCel::FontBold_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontBold(key, value);
#ifdef ACTIONVIEW
        ActionView.FontBold(GetSheet(), SelStartRow, SelStartCol, SelEndRow, SelEndCol, value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::FontItalic_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontItalic(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::FontSize_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontSize(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::FontColor_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = ColorReverse(TF1FlexCel::libGetFontColor(key));
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return ColorReverse(TF1FlexCel::libGetFontColor(key));
    }

    void TF1FlexCel::FontColor_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontColor(key, ColorReverse(value));
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::FontUnderline_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetFontUnderline(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontUnderline(key);
    }

    void TF1FlexCel::FontUnderline_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontUnderline(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::FontStrikeout_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetFontStrikeout(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontStrikeout(key);
    }

    void TF1FlexCel::FontStrikeout_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontStrikeout(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::FormattedText_Get() {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGetFormattedText(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFormattedText(key);
    }

    const char16_t *TF1FlexCel::Formula_Get() {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGetFormula(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFormula(key);
    }

    uint64_t TF1FlexCel::HAlign_Get() {
        TracerManager::Instance().StartF1LibTrace();
        uint64_t t_ = TF1FlexCel::libGetHAlign(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetHAlign(key);
    }

    int TF1FlexCel::SelEndCol_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetSelEndCol(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetSelEndCol(key);
    }

    int TF1FlexCel::SelEndRow_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetSelEndRow(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetSelEndRow(key);
    }

    int TF1FlexCel::SelStartCol_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetSelStartCol(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetSelStartCol(key);
    }

    int TF1FlexCel::SelStartRow_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetSelStartRow(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetSelStartRow(key);
    }

    const char16_t *TF1FlexCel::Text_Get() {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGetText(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetText(key);
    }

    int TF1FlexCel::Type_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetType_(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetType_(key);
    }

    uint64_t TF1FlexCel::VAlign_Get() {
        TracerManager::Instance().StartF1LibTrace();
        uint64_t t_ = TF1FlexCel::libGetVAlign(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetVAlign(key);
    }

    bool TF1FlexCel::WordWrap_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetWordWrap(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetWordWrap(key);
    }

    void TF1FlexCel::FormattedText_Set(const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFormattedText(key, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), SelStartRow, SelStartCol, 2);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Formula_Set(const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFormula(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::HAlign_Set(const uint64_t value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetHAlign(key, value);
#ifdef ACTIONVIEW
        ActionView.HAlign(GetSheet(), SelStartRow, SelStartCol, SelEndRow, SelEndCol, value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::SelEndCol_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSelEndCol(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::SelEndRow_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSelEndRow(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::SelStartCol_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSelStartCol(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::SelStartRow_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSelStartRow(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Text_Set(const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetText(key, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), SelStartRow, SelStartCol, 2);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Type_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetType_(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::VAlign_Set(const uint64_t value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetVAlign(key, value);
#ifdef ACTIONVIEW
        ActionView.VAlign(GetSheet(), SelStartRow, SelStartCol, SelEndRow, SelEndCol, value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::WordWrap_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetWordWrap(key, value);
#ifdef ACTIONVIEW
        ActionView.WordWrap(GetSheet(), SelStartRow, SelStartCol, SelEndRow, SelEndCol, value);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::ColWidth_Get(int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGet_ColWidth(key, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ColWidth(key, nCol);
    }

    void TF1FlexCel::ColWidth_Set(int nCol, const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ColWidth(key, nCol, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::RowHeight_Get(int nRow) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGet_RowHeight(key, nRow);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_RowHeight(key, nRow);
    }

    void TF1FlexCel::RowHeight_Set(int nRow, const int value) {
        TracerManager::Instance().StartF1LibTrace();
#ifdef ACTIONVIEW
        if (value == -1)
            ActionView.RowHeight(nRow, value);
        else
#endif
        TF1FlexCel::libSet_RowHeight(key, nRow, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    double TF1FlexCel::Number_Get() {
        TracerManager::Instance().StartF1LibTrace();
        double t_ = TF1FlexCel::libGetNumber(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetNumber(key);
    }

    void TF1FlexCel::Number_Set(const double value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetNumber(key, value);
#ifdef ACTIONVIEW
        ActionView.CellValue(GetSheet(), SelStartRow, SelStartCol, 1);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::ColWidthTwips_Get(int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGet_ColWidthTwips(key, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ColWidthTwips(key, nCol);
    }

    const char16_t *TF1FlexCel::FormattedTextRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_FormattedTextRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_FormattedTextRC(key, nRow, nCol);
    }

    const char16_t *TF1FlexCel::FormulaRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_FormulaRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_FormulaRC(key, nRow, nCol);
    }

    const char16_t *TF1FlexCel::NumberFormat_Get() {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_NumberFormat(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_NumberFormat(key);
    }

    int TF1FlexCel::TypeRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGet_TypeRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_TypeRC(key, nRow, nCol);
    }

    void TF1FlexCel::ColWidthTwips_Set(int nCol, const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ColWidthTwips(key, nCol, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::FormulaRC_Set(int nRow, int nCol, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_FormulaRC(key, nRow, nCol, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::NumberFormat_Set(const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_NumberFormat(key, value);
#ifdef ACTIONVIEW
        int rowStart = SelStartRow;
        int rowEnd = SelEndRow;
        int nSheet = GetSheet();
        for (int nRow = rowStart; nRow <= rowEnd; nRow++) {
            ActionView.SetNumberFormat(nSheet, nRow, SelStartCol, value);
        }
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    uint8_t TF1FlexCel::FontCharSet_Get() {
        TracerManager::Instance().StartF1LibTrace();
        uint8_t t_ = TF1FlexCel::libGetFontCharSet(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetFontCharSet(key);
    }

    void TF1FlexCel::FontCharSet_Set(const uint8_t value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFontCharSet(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    TF1Range TF1FlexCel::SelectionEx_Get(int nSelection) {
        TracerManager::Instance().StartF1LibTrace();
        TF1Range range = TF1Range();
        TF1FlexCel::libGet_SelectionEx(key, nSelection, range.StartRow, range.EndRow, range.StartCol, range.EndCol);
        TracerManager::Instance().EndF1LibTrace();
        return range;
    }

    bool TF1FlexCel::EnableProtection_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetEnableProtection(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetEnableProtection(key);
    }

    void TF1FlexCel::EnableProtection_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetEnableProtection(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::ShowGridLines_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetShowGridLines(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetShowGridLines(key);
    }

    void TF1FlexCel::ShowGridLines_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetShowGridLines(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::AutoRecalc_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetAutoRecalc(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetAutoRecalc(key);
    }

    void TF1FlexCel::AutoRecalc_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetAutoRecalc(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::SelectionCount_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetSelectionCount(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetSelectionCount(key);
    }

    void TF1FlexCel::SelectionCount_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSelectionCount(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    uint64_t TF1FlexCel::ColWidthUnits_Get() {
        TracerManager::Instance().StartF1LibTrace();
        uint64_t t_ = TF1FlexCel::libGetColWidthUnits(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetColWidthUnits(key);
    }

    void TF1FlexCel::ColWidthUnits_Set(const uint64_t value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetColWidthUnits(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::AllowResize_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetAllowResize(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetAllowResize(key);
    }

    void TF1FlexCel::AllowResize_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetAllowResize(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::HdrWidth_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetHdrWidth(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetHdrWidth(key);
    }

    void TF1FlexCel::HdrWidth_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetHdrWidth(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    const char16_t *TF1FlexCel::FormulaLocalRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGet_FormulaLocalRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_FormulaLocalRC(key, nRow, nCol);
    }

    void TF1FlexCel::FormulaLocalRC_Set(int nRow, int nCol, const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_FormulaLocalRC(key, nRow, nCol, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::ObjVisible_Get(int ObjID) {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGet_ObjVisible(key, ObjID);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_ObjVisible(key, ObjID);
    }

    void TF1FlexCel::ObjVisible_Set(int ObjID, const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_ObjVisible(key, ObjID, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::ShowZeroValues_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetShowZeroValues(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetShowZeroValues(key);
    }

    void TF1FlexCel::ShowZeroValues_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetShowZeroValues(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    bool TF1FlexCel::AllowDelete_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetAllowDelete(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetAllowDelete(key);
    }

    bool TF1FlexCel::AllowInCellEditing_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetAllowInCellEditing(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetAllowInCellEditing(key);
    }

    const char16_t *TF1FlexCel::DefaultFontName_Get() {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libGetDefaultFontName(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetDefaultFontName(key);
    }

    bool TF1FlexCel::ShowLockedCellsError_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGetShowLockedCellsError(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetShowLockedCellsError(key);
    }

    uint64_t TF1FlexCel::ShowSelections_Get() {
        TracerManager::Instance().StartF1LibTrace();
        uint64_t t_ = TF1FlexCel::libGetShowSelections(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetShowSelections(key);
    }

    void TF1FlexCel::AllowDelete_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetAllowDelete(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::AllowInCellEditing_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetAllowInCellEditing(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::DefaultFontName_Set(const char16_t *value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetDefaultFontName(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ShowLockedCellsError_Set(const bool value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetShowLockedCellsError(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ShowSelections_Set(const uint64_t value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetShowSelections(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::DefaultFontSize_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetDefaultFontSize(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetDefaultFontSize(key);
    }

    void TF1FlexCel::DefaultFontSize_Set(const int value) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetDefaultFontSize(key, value);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::CellFormat_Set(int XF) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetCellFormat(key, XF);
#ifdef ACTIONVIEW
        ActionView.SetCellFormat(GetSheet(), SelStartRow, SelStartCol);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::CellFormat_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetCellFormat(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetCellFormat(key);
    }

    bool TF1FlexCel::MergeCells_Get() {
        TracerManager::Instance().StartF1LibTrace();
        bool t_ = TF1FlexCel::libGet_MergeCells(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGet_MergeCells(key);
    }

    void TF1FlexCel::MergeCells_Set(bool pMergeCells) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSet_MergeCells(key, pMergeCells);
#ifdef ACTIONVIEW
        ActionView.MergeCells(GetSheet(), SelStartRow, SelStartCol, SelEndRow, SelEndCol, pMergeCells);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    TMWSPageSetup TF1FlexCel::PageSetup_Get() {
        return {};
    }

    void TF1FlexCel::PageSetup_Set(TMWSPageSetup pageSetup) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetPageSetup(
                key,
                pageSetup.AutoPageNumber,
                pageSetup.FirstPageNumber,
                pageSetup.LeftMargin,
                pageSetup.TopMargin,
                pageSetup.BottomMargin,
                pageSetup.RightMargin,
                pageSetup.HeaderMargin,
                pageSetup.FooterMargin,
                pageSetup.CenterHoriz,
                pageSetup.CenterVert,
                pageSetup.Header,
                pageSetup.Footer,
                pageSetup.Landscape,
                pageSetup.FitPages,
                pageSetup.PrintScale,
                pageSetup.PagesWide,
                pageSetup.PagesTall,
                pageSetup.PaperSize,
                pageSetup.PrintTitles,
                pageSetup.GridLines,
                pageSetup.BlackAndWhite,
                pageSetup.RowHeadings,
                pageSetup.ColHeadings,
                pageSetup.LeftToRight,
                pageSetup.PrintArea
        );
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Print() {
        TF1FlexCel::libPrint(key);
    }

    void TF1FlexCel::PrintPage() {
        TF1FlexCel::libPrintPage(key);
    }

    void TF1FlexCel::PrintTitlesFromSelection_Set() {
        TF1FlexCel::libSetPrintTitlesFromSelection(key);
    }

    void TF1FlexCel::SaveToPDF(const char16_t *filename, const char16_t *documentProperties) {
        TF1FlexCel::libSaveToPDF(key, filename, documentProperties);
    }

    void TF1FlexCel::Recalc() {
        TF1FlexCel::libRecalc(key);
    }

    void TF1FlexCel::DeleteSheets(int nSheet, int nSheets) {
        TF1FlexCel::libDeleteSheets(key, nSheet, nSheets);
    }

    void TF1FlexCel::Selection_Set(int nRow1, int nCol1, int nRow2, int nCol2) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetSelection(key, nRow1, nCol1, nRow2, nCol2);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Protection_Get(bool &pLocked, bool &pHidden) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libGetProtection(key, pLocked, pHidden);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Selection_Get(int nSelection, int &pR1, int &pC1, int &pR2, int &pC2) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libGetSelection(key, nSelection, pR1, pC1, pR2, pC2);
        TracerManager::Instance().EndF1LibTrace();
    }

    uint64_t TF1FlexCel::ObjGetType(int ObjID) {
        TracerManager::Instance().StartF1LibTrace();
        uint64_t t_ = TF1FlexCel::libObjGetType(key, ObjID);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libObjGetType(key, ObjID);
    }

    int TF1FlexCel::ObjNextID(int ObjID) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libObjNextID(key, ObjID);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libObjNextID(key, ObjID);
    }

    int TF1FlexCel::ObjFirstID(int nSheet, int nRow, int nCol, char16_t &aCaption) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libObjFirstID(key, nSheet, nRow, nCol, aCaption);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libObjFirstID(key, nSheet, nRow, nCol, aCaption);
    }

    void TF1FlexCel::InsertRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t InsertType) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libInsertRange(key, nRow1, nCol1, nRow2, nCol2, InsertType);
#ifdef ACTIONVIEW
        ActionView.InsertRange(GetSheet(), nRow1, nCol1, nRow2, nCol2, InsertType);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::InsertSheets(int nSheet, int nSheets) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libInsertSheets(key, nSheet, nSheets);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::EditInsertSheets() {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libEditInsertSheets(key);
        TracerManager::Instance().EndF1LibTrace();
    }

    void
    TF1FlexCel::Border_Set(int nOutline, int nLeft, int nRight, int nTop, int nBottom, int nShade, int crOutline, uint32_t crLeft, uint32_t crRight, uint32_t crTop,
                          uint32_t crBottom) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetBorder(key, nOutline, nLeft, nRight, nTop, nBottom, nShade, crOutline, crLeft, crRight, crTop, crBottom);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Border_Get(int &pLeft, int &pRight, int &pTop, int &pBottom, int &pShade, int &pcrLeft, int &pcrRight, int &pcrTop, int &pcrBottom) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libGetBorder(key, pLeft, pRight, pTop, pBottom, pShade, pcrLeft, pcrRight, pcrTop, pcrBottom);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::DrawBorders(int row1, int col1, int row2, int col2, int borderStyle, int borderColor, bool exteriorBorders) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libDrawBorders(key, row1, col1, row2, col2, borderStyle, ColorReverse(borderColor), exteriorBorders);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Pattern_Get(int &pPattern, int &pcrFG, int &pcrBG) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libGetPattern(key, pPattern, pcrFG, pcrBG);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ColWidthAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetColWidthAuto(key, nRow1, nCol1, nRow2, nCol2, bSetDefaults);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Pattern_Set(int nPattern, uint32_t crFG, uint32_t crBG) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetPattern(key, nPattern, crFG, crBG);
#ifdef ACTIONVIEW
        int rowStart = SelStartRow;
        int rowEnd = SelEndRow;
        int colStart = SelStartCol;
        int colEnd = SelEndCol;
        int nSheet = GetSheet();
        for (int nRow = rowStart; nRow <= rowEnd; nRow++) {
            for (int nCol = colStart; nCol <= colEnd; nCol++) {
                ActionView.SetPattern(nSheet, nRow, nCol);
            }
        }
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::BackgroundColor_Set(uint32_t crBG) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetBackgroundColor(key, ColorReverse(crBG));
#ifdef ACTIONVIEW
        ActionView.SetBackgroundColor(GetSheet(), SelStartRow, SelStartCol, SelEndRow, SelEndCol, crBG);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::RowHeightAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetRowHeightAuto(key, nRow1, nCol1, nRow2, nCol2, bSetDefaults);
        TracerManager::Instance().EndF1LibTrace();
    }

    /**
     *
     * @param nR1
     * @param nC1
     * @param nR2
     * @param nC2
     * @param bSortByRows
     * @param keysString это строка типа "1,3,8,9", где через запятую перечислены ключи сортировки.
     */
    void TF1FlexCel::Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, const char16_t *keysString) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSort(key, nR1, nC1, nR2, nC2, bSortByRows, keysString);
#ifdef ACTIONVIEW
        if (ActionView.isSaveAllAction) {
            TCellValueType cellValueType;
            for (int r = nR1; r <= nR2; r++) {
                // Является ли GetMaxCol синонимом Xls.ColCount?
                for (int c = 1; c <= GetMaxCol(); c++) {
                    cellValueType = GetCellValueType(r, c);
                    switch (cellValueType) {
                        case TCellValueType::Number:
                            ActionView.CellValue(GetSheet(), r, c, 1);
                            break;
                        case TCellValueType::Formula:
                            ActionView.CellValue(GetSheet(), r, c, 3);
                            break;
                        default: //StringValue
                            ActionView.CellValue(GetSheet(), r, c, 2);
                            break;
                    }
                }
            }
        }
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    /**
     *
     * @param nR1
     * @param nC1
     * @param nR2
     * @param nC2
     * @param bSortByRows
     * @param Keys
     * @see SortKeys
     */
    void TF1FlexCel::Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, SortKeys Keys) {
        TracerManager::Instance().StartF1LibTrace();
        Sort(nR1, nC1, nR2, nC2, bSortByRows, Keys.GetKeysString());
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Protection_Set(bool bLocked, bool bHidden) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetProtection(key, bLocked, bHidden);
#ifdef ACTIONVIEW
        ActionView.SetProtection(GetSheet(), SelStartRow, SelStartCol, bLocked, bHidden);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ColHidden_Set(int nCol1, int nCol2, bool bHidden) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetColHidden(key, nCol1, nCol2, bHidden);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ActiveCell_Set(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetActiveCell(key, nRow, nCol);
#ifdef ACTIONVIEW
        ActionView.SetActiveCell(GetSheet(), nRow, nCol);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ShowActiveCell() {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libShowActiveCell(key);
#ifdef ACTIONVIEW
        ActionView.ShowActiveCell(GetSheet());
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::CopyRange(int nDstR1, int nDstC1, int nDstR2, int nDstC2, int hSrcSS, int nSrcR1, int nSrcC1, int nSrcR2, int nSrcC2) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libCopyRange(key, nDstR1, nDstC1, nDstR2, nDstC2, hSrcSS, nSrcR1, nSrcC1, nSrcR2, nSrcC2);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::SS() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libSS(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libSS(key);
    }

    void TF1FlexCel::Font_Set(const char16_t *pName, int nSize, bool bBold, bool bItalic, bool bUnderline, bool bStrikeout, uint32_t crColor, bool bOutline, bool bShadow) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetFont(key, pName, nSize, bBold, bItalic, bUnderline, bStrikeout, ColorReverse(crColor), bOutline, bShadow);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::Font_Get(char16_t &pName, int &pSize, bool &pBold, bool &pItalic, bool &pUnderline, bool &pStrikeout, int &pcrColor, bool &pOutline, bool &pShadow) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libGetFont(key, pName, pSize, pBold, pItalic, pUnderline, pStrikeout, pcrColor, pOutline, pShadow);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::DeleteRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ShiftType) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libDeleteRange(key, nRow1, nCol1, nRow2, nCol2, ShiftType);
#ifdef ACTIONVIEW
        ActionView.DeleteRange(GetSheet(), nRow1, nCol1, nRow2, nCol2, ShiftType);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::ClearRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ClearType) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libClearRange(key, nRow1, nCol1, nRow2, nCol2, ClearType);
#ifdef ACTIONVIEW
        ActionView.ClearRange(GetSheet(), nRow1, nCol1, nRow2, nCol2);
#endif
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::EditClear(uint64_t ClearType) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libEditClear(key, ClearType);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::BorderStyle_Set(int StyleTopBorder, int StyleLeftBorder, int StyleBottomBorder, int StyleRightBorder) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libsetBorderStyle(key, StyleTopBorder, StyleLeftBorder, StyleBottomBorder, StyleRightBorder);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::BorderStyle_Get(int &StyleTopBorder, int &StyleLeftBorder, int &StyleBottomBorder, int &StyleRightBorder) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libgetBorderStyle(key, StyleTopBorder, StyleLeftBorder, StyleBottomBorder, StyleRightBorder);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::BorderColor_Set(int ColorTopBorder, int ColorLeftBorder, int ColorBottomBorder, int ColorRightBorder) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libsetBorderColor(key, ColorReverse(ColorTopBorder), ColorReverse(ColorLeftBorder), ColorReverse(ColorBottomBorder), ColorReverse(ColorRightBorder));
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::BorderColor_Get(int &ColorTopBorder, int &ColorLeftBorder, int &ColorBottomBorder, int &ColorRightBorder) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libgetBorderColor(key, ColorTopBorder, ColorLeftBorder, ColorBottomBorder, ColorRightBorder);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::InputButton(int nRow1, int nCol1, int nRow2, int nCol2, const char16_t *aCaption, const char16_t *aObjName) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libInputButton(key, nRow1, nCol1, nRow2, nCol2, aCaption, aObjName);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libInputButton(key, nRow1, nCol1, nRow2, nCol2, aCaption, aObjName);
    }

    void TF1FlexCel::BorderInfo_Get(int nRow, int nCol, int &BorderTopStyle, int &BorderLeftStyle, int &BorderBottomStyle, int &BorderRightStyle, int &BorderTopColor,
                                   int &BorderLeftColor, int &BorderBottomColor, int &BorderRightColor) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libgetBorderInfo(key, nRow, nCol, BorderTopStyle, BorderLeftStyle, BorderBottomStyle, BorderRightStyle, BorderTopColor, BorderLeftColor, BorderBottomColor,
                                     BorderRightColor);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::HAlignRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetHAlignRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetHAlignRC(key, nRow, nCol);
    }

    THFlxAlignment TF1FlexCel::toHAlign(int aH) {
        TracerManager::Instance().StartF1LibTrace();
        THFlxAlignment t_ = THFlxAlignment(aH);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return THFlxAlignment(aH);
    }

    TVFlxAlignment TF1FlexCel::toVAlign(int aV) {
        TracerManager::Instance().StartF1LibTrace();
        TVFlxAlignment t_ = TVFlxAlignment(aV);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TVFlxAlignment(aV);
    }

    int TF1FlexCel::VAlignRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libgetVAlignRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libgetVAlignRC(key, nRow, nCol);
    }

    int TF1FlexCel::objlistCount_Get() {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetobjlistCount(key);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetobjlistCount(key);
    }

    void TF1FlexCel::objlistI_PropRC_Get(int aI, int &nRow, int &nCol, char16_t &objectName) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libGetobjlistI_PropRC(key, aI, nRow, nCol, objectName);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::NFormatRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libgetNFormatRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libgetNFormatRC(key, nRow, nCol);
    }

    const char16_t *TF1FlexCel::StyleNameRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libgetStyleNameRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libgetStyleNameRC(key, nRow, nCol);
    }

    uint32_t TF1FlexCel::BackColorRC_Get(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        uint32_t t_ = ColorReverse(TF1FlexCel::libgetBackColorRC(key, nRow, nCol));
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return ColorReverse(TF1FlexCel::libgetBackColorRC(key, nRow, nCol));
    }

    void TF1FlexCel::CellInfoRC_Get(int nRow, int nCol, char16_t &Format, bool &Locked, bool &WrapText) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libgetCellInfoRC(key, nRow, nCol, Format, Locked, WrapText);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::FontInfoRC_Get(int nRow, int nCol, uint32_t &Color, char16_t &FontName, int &FontSize, int &Underline, bool &Bold, bool &Italic) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libgetFontInfoRC(key, nRow, nCol, Color, FontName, FontSize, Underline, Bold, Italic);
        Color = ColorReverse(Color);
        TracerManager::Instance().EndF1LibTrace();
    }

    void TF1FlexCel::F1CellFormat_Set(
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
    ) {
        TracerManager::Instance().StartF1LibTrace();
        TF1FlexCel::libSetF1CellFormat(key, isAlignHorizontal, AlignHorizontal_, isWordWrap, WordWrap_, isAlignVertical, AlignVertical_, isBorderStyle, BorderStyle_, pShade_,
                                       isBorderColor, BorderColor_, isFontName, FontName_, isFontSize, FontSize_, isFontBold, FontBold_, isFontItalic, FontItalic_, isFontUnderline,
                                       FontUnderline_, isFontStrikeout, FontStrikeout_, isFontColor, FontColor_, isPatternStyle, PatternStyle_, PatternBG_, PatternFG_,
                                       isProtectionLocked, ProtectionLocked_, ProtectionHidden_, isNumberFormat, NumberFormat_, MergeCells_, isRotation, Rotation_);
        TracerManager::Instance().EndF1LibTrace();
    }

    int TF1FlexCel::NVisibleCol_Get(int &nVisibleCol1) {
        TracerManager::Instance().StartF1LibTrace();
        int t_ = TF1FlexCel::libGetNVisibleCol(key, nVisibleCol1);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libGetNVisibleCol(key, nVisibleCol1);
    }

    double TF1FlexCel::FormulaResultNumberRC(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        double t_ = TF1FlexCel::libFormulaResultNumberRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libFormulaResultNumberRC(key, nRow, nCol);
    }

    const char16_t *TF1FlexCel::FormulaResultTextRC(int nRow, int nCol) {
        TracerManager::Instance().StartF1LibTrace();
        const char16_t *t_ = TF1FlexCel::libFormulaResultTextRC(key, nRow, nCol);
        TracerManager::Instance().EndF1LibTrace();
        return t_;
//        return TF1FlexCel::libFormulaResultTextRC(key, nRow, nCol);
    }
}
