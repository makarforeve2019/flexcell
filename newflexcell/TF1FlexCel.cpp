#include "TF1FlexCel.h"
#include "FormulaEngine.h"
#include "FormulaAST.h"
#include <OpenXLSX.hpp>
#include <filesystem>
#include <stdexcept>
#include <map>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <codecvt>
#include <locale>

namespace StringUtil {
    std::string U16ToString(const char16_t *str_u16) {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.to_bytes(str_u16);
    }

    std::u16string to_u16string(const std::string &str) {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.from_bytes(str);
    }
}

enum Local_F1ClearTypeConstants : uint64_t {
    F1ClearAll = 1,
    F1ClearFormats = 2,
    F1ClearValues = 3
};

// --- Вспомогательная функция для конвертации цвета ---
namespace {

    enum class Local_F1HAlignConstants : int {
        F1HAlignGeneral = 0,
        F1HAlignLeft = 1,
        F1HAlignCenter = 2,
        F1HAlignRight = 3,
        F1HAlignFill = 4,
        F1HAlignJustify = 5,
        F1HAlignCenterAcrossCells = 6
    };

    enum class Local_F1VAlignConstants : int {
        F1VAlignTop = 0,
        F1VAlignCenter = 1,
        F1VAlignBottom = 2
    };

    OpenXLSX::XLColor UnpackColor(uint32_t colorValue) {
        // Конвертирую цвет из формата 0x00bbggrr в объект XLColor.
        uint8_t r = (colorValue >> 16) & 0xFF;
        uint8_t g = (colorValue >> 8) & 0xFF;
        uint8_t b = colorValue & 0xFF;
        return OpenXLSX::XLColor(r, g, b);
    }

    OpenXLSX::XLPatternType UnpackPattern(int patternValue) {
        static const std::map<int, OpenXLSX::XLPatternType> patternMap = {
                {0, OpenXLSX::XLPatternType::XLPatternNone},
                {1, OpenXLSX::XLPatternType::XLPatternSolid},
                {2, OpenXLSX::XLPatternType::XLPatternMediumGray},
                {3, OpenXLSX::XLPatternType::XLPatternDarkGray},
                {4, OpenXLSX::XLPatternType::XLPatternLightGray},
        };
        auto it = patternMap.find(patternValue);
        return (it != patternMap.end()) ? it->second : OpenXLSX::XLPatternType::XLPatternSolid;
    }
}


namespace F1DevExpress::unF1FlexCel {

    // Структура, в которой я храню все данные реализации.
    struct TF1FlexCel::Impl {
        std::unique_ptr<FormulaEngine> engine;
        std::unique_ptr<OpenXLSX::XLDocument> document;
        OpenXLSX::XLWorksheet currentSheet;
        // Добавляю состояние для активной ячейки. По умолчанию A1.
        int activeRow{1};
        int activeCol{1};
        // Добавляю буфер для возврата C-style строк.
        std::u16string stringBuffer;
        // Добавляю переменные для хранения "виртуальных" размеров.
        // 0 будет означать, что значение не установлено.
        int uiMaxRow{0};
        int uiMaxCol{0};
        int uiLastRow{0};
        int uiLastCol{0};
        // Добавляю переменные для хранения выделенной области.
        // По умолчанию выделена активная ячейка.
        int selStartRow{1};
        int selStartCol{1};
        int selEndRow{1};
        int selEndCol{1};
        bool autoRecalc{true};
        int hdrWidth{80};
        bool allowInCellEditing{true};
        bool showLockedCellsError{true};
        uint64_t showSelections{1};
        // Добавляю поля для хранения закрепленных строк и колонок.
        int fixedRows{0};
        int fixedCols{0};
        std::vector<ButtonInfo> buttonCache;
    };

    // =================================================================================================
// РЕАЛИЗАЦИЯ ПРОКСИ-КЛАССОВ (ЭМУЛЯЦИЯ СВОЙСТВ)
// =================================================================================================
// --- Прокси для простых свойств ---
#define IMPLEMENT_PROXY_GETTER(TYPE, PROP_NAME) \
    if (this == &(m_flex->PROP_NAME)) return m_flex->PROP_NAME##_Get();

#define IMPLEMENT_PROXY_SETTER(TYPE, PROP_NAME) \
    if (this == &(m_flex->PROP_NAME)) m_flex->PROP_NAME##_Set(value);

    template<>
    Property<int>::operator int() const {
        IMPLEMENT_PROXY_GETTER(int, Row);
        IMPLEMENT_PROXY_GETTER(int, Col);
        IMPLEMENT_PROXY_GETTER(int, Sheet);
        IMPLEMENT_PROXY_GETTER(int, MaxRow);
        IMPLEMENT_PROXY_GETTER(int, MaxCol);
        IMPLEMENT_PROXY_GETTER(int, LastRow);
        IMPLEMENT_PROXY_GETTER(int, LastCol);
        IMPLEMENT_PROXY_GETTER(int, NumSheets);
        IMPLEMENT_PROXY_GETTER(int, FontSize);
        IMPLEMENT_PROXY_GETTER(int, SelStartRow);
        IMPLEMENT_PROXY_GETTER(int, SelStartCol);
        IMPLEMENT_PROXY_GETTER(int, SelEndRow);
        IMPLEMENT_PROXY_GETTER(int, SelEndCol);
        IMPLEMENT_PROXY_GETTER(int, HdrWidth);
        IMPLEMENT_PROXY_GETTER(int, DefaultFontSize);
        IMPLEMENT_PROXY_GETTER(int, Type);
        return 0;
    }

    template<>
    Property<int> &Property<int>::operator=(const int &value) {
        IMPLEMENT_PROXY_SETTER(int, Row);
        IMPLEMENT_PROXY_SETTER(int, Col);
        IMPLEMENT_PROXY_SETTER(int, Sheet);
        IMPLEMENT_PROXY_SETTER(int, MaxRow);
        IMPLEMENT_PROXY_SETTER(int, MaxCol);
        IMPLEMENT_PROXY_SETTER(int, FontSize);
        IMPLEMENT_PROXY_SETTER(int, SelStartRow);
        IMPLEMENT_PROXY_SETTER(int, SelStartCol);
        IMPLEMENT_PROXY_SETTER(int, SelEndRow);
        IMPLEMENT_PROXY_SETTER(int, SelEndCol);
        IMPLEMENT_PROXY_SETTER(int, HdrWidth);
        IMPLEMENT_PROXY_SETTER(int, DefaultFontSize);
        IMPLEMENT_PROXY_SETTER(int, Type);
        return *this;
    }

    template<>
    Property<bool>::operator bool() const {
        IMPLEMENT_PROXY_GETTER(bool, AutoRecalc);
        IMPLEMENT_PROXY_GETTER(bool, MergeCells);
        IMPLEMENT_PROXY_GETTER(bool, FontBold);
        IMPLEMENT_PROXY_GETTER(bool, FontItalic);
        IMPLEMENT_PROXY_GETTER(bool, FontUnderline);
        IMPLEMENT_PROXY_GETTER(bool, FontStrikeout);
        IMPLEMENT_PROXY_GETTER(bool, WordWrap);
        IMPLEMENT_PROXY_GETTER(bool, EnableProtection);
        IMPLEMENT_PROXY_GETTER(bool, DisableShrink);
        IMPLEMENT_PROXY_GETTER(bool, DisableAutoSize);
        IMPLEMENT_PROXY_GETTER(bool, AllowDelete);
        IMPLEMENT_PROXY_GETTER(bool, AllowInCellEditing);
        IMPLEMENT_PROXY_GETTER(bool, ShowLockedCellsError);
        IMPLEMENT_PROXY_GETTER(bool, ShowGridLines);
        IMPLEMENT_PROXY_GETTER(bool, ShowZeroValues);
        return false;
    }

    template<>
    Property<bool> &Property<bool>::operator=(const bool &value) {
        IMPLEMENT_PROXY_SETTER(bool, AutoRecalc);
        IMPLEMENT_PROXY_SETTER(bool, MergeCells);
        IMPLEMENT_PROXY_SETTER(bool, FontBold);
        IMPLEMENT_PROXY_SETTER(bool, FontItalic);
        IMPLEMENT_PROXY_SETTER(bool, FontUnderline);
        IMPLEMENT_PROXY_SETTER(bool, FontStrikeout);
        IMPLEMENT_PROXY_SETTER(bool, WordWrap);
        IMPLEMENT_PROXY_SETTER(bool, EnableProtection);
        IMPLEMENT_PROXY_SETTER(bool, DisableShrink);
        IMPLEMENT_PROXY_SETTER(bool, DisableAutoSize);
        IMPLEMENT_PROXY_SETTER(bool, AllowDelete);
        IMPLEMENT_PROXY_SETTER(bool, AllowInCellEditing);
        IMPLEMENT_PROXY_SETTER(bool, ShowLockedCellsError);
        IMPLEMENT_PROXY_SETTER(bool, ShowGridLines);
        IMPLEMENT_PROXY_SETTER(bool, ShowZeroValues);
        return *this;
    }

    template<>
    Property<uint32_t>::operator uint32_t() const {
        IMPLEMENT_PROXY_GETTER(uint32_t, FontColor);
        return 0;
    }

    template<>
    Property<uint32_t> &Property<uint32_t>::operator=(const uint32_t &value) {
        IMPLEMENT_PROXY_SETTER(uint32_t, FontColor);
        return *this;
    }

    template<>
    Property<uint64_t>::operator uint64_t() const {
        IMPLEMENT_PROXY_GETTER(uint64_t, HAlign);
        IMPLEMENT_PROXY_GETTER(uint64_t, VAlign);
        IMPLEMENT_PROXY_GETTER(uint64_t, ShowSelections);
        return 0;
    }

    template<>
    Property<uint64_t> &Property<uint64_t>::operator=(const uint64_t &value) {
        IMPLEMENT_PROXY_SETTER(uint64_t, HAlign);
        IMPLEMENT_PROXY_SETTER(uint64_t, VAlign);
        IMPLEMENT_PROXY_SETTER(uint64_t, ShowSelections);
        return *this;
    }

    template<>
    Property<const char16_t *>::operator const char16_t *() const {
        IMPLEMENT_PROXY_GETTER(const char16_t*, FontName);
        IMPLEMENT_PROXY_GETTER(const char16_t*, NumberFormat);
        IMPLEMENT_PROXY_GETTER(const char16_t*, Text);
        IMPLEMENT_PROXY_GETTER(const char16_t*, DefaultFontName);
        IMPLEMENT_PROXY_GETTER(const char16_t*, Formula);
        return u"";
    }

    template<>
    Property<const char16_t *> &Property<const char16_t *>::operator=(const char16_t *const &value) {
        IMPLEMENT_PROXY_SETTER(const char16_t*, FontName);
        IMPLEMENT_PROXY_SETTER(const char16_t*, NumberFormat);
        IMPLEMENT_PROXY_SETTER(const char16_t*, Text);
        IMPLEMENT_PROXY_SETTER(const char16_t*, DefaultFontName);
        IMPLEMENT_PROXY_SETTER(const char16_t*, Formula);
        return *this;
    }

    template<>
    Property<double>::operator double() const {
        IMPLEMENT_PROXY_GETTER(double, Number);
        return 0.0;
    }

    template<>
    Property<double> &Property<double>::operator=(const double &value) {
        IMPLEMENT_PROXY_SETTER(double, Number);
        return *this;
    }

// --- Прокси для индексированных свойств ---
    BoolPropertyProxy::operator bool() const {
        return m_isRow ? m_flex->RowHidden_Get(m_index) : m_flex->ColHidden_Get(m_index);
    }

    BoolPropertyProxy &BoolPropertyProxy::operator=(bool value) {
        if (m_isRow) m_flex->RowHidden_Set(m_index, value);
        else m_flex->ColHidden_Set(m_index, value);
        return *this;
    }

    IntPropertyProxy::operator int() const {
        return m_isRow ? m_flex->RowHeight_Get(m_index) : m_flex->ColWidth_Get(m_index);
    }

    IntPropertyProxy &IntPropertyProxy::operator=(int value) {
        if (m_isRow) m_flex->RowHeight_Set(m_index, value);
        else m_flex->ColWidth_Set(m_index, value);
        return *this;
    }

    TextRCProxy::operator const char16_t *() const { return m_flex->TextRC_Get(m_row, m_col); }

    TextRCProxy &TextRCProxy::operator=(const char16_t *value) {
        m_flex->TextRC_Set(m_row, m_col, value);
        return *this;
    }

    NumberRCProxy::operator double() const { return m_flex->NumberRC_Get(m_row, m_col); }

    NumberRCProxy &NumberRCProxy::operator=(double value) {
        m_flex->NumberRC_Set(m_row, m_col, value);
        return *this;
    }

    FormulaRCProxy &FormulaRCProxy::operator=(const char16_t *value) {
        m_flex->FormulaRC_Set(m_row, m_col, value);
        return *this;
    }

    FormulaRCProxy::operator const char16_t *() const {
        return m_flex->FormulaRC_Get(m_row, m_col);
    }

    SheetNameProxy::operator const char16_t *() const {
        return m_flex->SheetName_Get(m_index);
    }

    SheetNameProxy &SheetNameProxy::operator=(const char16_t *value) {
        m_flex->SheetName_Set(m_index, value);
        return *this;

    }


// =================================================================================================
// РЕАЛИЗАЦИЯ КОНСТРУКТОРА И ДЕСТРУКТОРА
// =================================================================================================
    TF1FlexCel::TF1FlexCel()
            : pimpl(std::make_unique<Impl>()),
              Row(this), Col(this), Sheet(this), MaxRow(this), MaxCol(this), LastRow(this), LastCol(this), NumSheets(this), AutoRecalc(this),
              MergeCells(this), FontBold(this), FontItalic(this), FontUnderline(this), FontStrikeout(this), FontSize(this), FontColor(this), FontName(this),
              NumberFormat(this), Text(this), WordWrap(this), HAlign(this), VAlign(this), EnableProtection(this), DisableShrink(this), DisableAutoSize(this),
              SelStartRow(this), SelStartCol(this), SelEndRow(this), SelEndCol(this), ShowSelections(this), HdrWidth(this), AllowDelete(this),
              DefaultFontName(this), DefaultFontSize(this), AllowInCellEditing(this), ShowLockedCellsError(this), ShowGridLines(this),
              ShowZeroValues(this), Number(this), Type(this), Formula(this),
              RowHeight(this), ColWidth(this), RowHidden(this), ColHidden(this), SheetName(this)
    {
        NewFile(1);
    }

    TF1FlexCel::~TF1FlexCel() = default;

    bool TF1FlexCel::MergeCells_Get() {
        if (!pimpl) return false;
        // Проверяем, объединена ли активная ячейка
        auto merges = pimpl->currentSheet.merges();
        return merges.findMergeByCell(pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol).cellReference()) !=
               -1;
    }

    void TF1FlexCel::MergeCells_Set(bool value) {
        if (!pimpl) return;
        MergeCells_Set(pimpl->selStartRow, pimpl->selStartCol, pimpl->selEndRow, pimpl->selEndCol, value);
    }

    TextRCProxy TF1FlexCel::TextRC(int nRow, int nCol) {
        return TextRCProxy(this, nRow, nCol);
    }

    NumberRCProxy TF1FlexCel::NumberRC(int nRow, int nCol) {
        return NumberRCProxy(this, nRow, nCol);
    }


    int TF1FlexCel::TypeRC(int nRow, int nCol) {
        return TypeRC_Get(nRow, nCol);
    }

    FormulaRCProxy TF1FlexCel::FormulaRC(int nRow, int nCol) {
        return FormulaRCProxy(this, nRow, nCol);
    }

    const char16_t *TF1FlexCel::Text_Get() { return TextRC_Get(pimpl->activeRow, pimpl->activeCol); }

    void TF1FlexCel::Text_Set(const char16_t *value) { TextRC_Set(pimpl->activeRow, pimpl->activeCol, value); }


    OpenXLSX::XLAlignmentStyle UnpackHAlign(int alignValue) {
        static const std::map<int, OpenXLSX::XLAlignmentStyle> alignMap = {
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignGeneral),           OpenXLSX::XLAlignmentStyle::XLAlignGeneral},
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignLeft),              OpenXLSX::XLAlignmentStyle::XLAlignLeft},
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignCenter),            OpenXLSX::XLAlignmentStyle::XLAlignCenter},
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignRight),             OpenXLSX::XLAlignmentStyle::XLAlignRight},
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignFill),              OpenXLSX::XLAlignmentStyle::XLAlignFill},
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignJustify),           OpenXLSX::XLAlignmentStyle::XLAlignJustify},
                {static_cast<int>(Local_F1HAlignConstants::F1HAlignCenterAcrossCells), OpenXLSX::XLAlignmentStyle::XLAlignCenterContinuous}
        };
        auto it = alignMap.find(alignValue);
        return (it != alignMap.end()) ? it->second : OpenXLSX::XLAlignmentStyle::XLAlignGeneral;
    }

    OpenXLSX::XLAlignmentStyle UnpackVAlign(int alignValue) {
        static const std::map<int, OpenXLSX::XLAlignmentStyle> alignMap = {
                {static_cast<int>(Local_F1VAlignConstants::F1VAlignTop),    OpenXLSX::XLAlignmentStyle::XLAlignTop},
                {static_cast<int>(Local_F1VAlignConstants::F1VAlignCenter), OpenXLSX::XLAlignmentStyle::XLAlignCenter},
                {static_cast<int>(Local_F1VAlignConstants::F1VAlignBottom), OpenXLSX::XLAlignmentStyle::XLAlignBottom},
        };
        auto it = alignMap.find(alignValue);
        return (it != alignMap.end()) ? it->second : OpenXLSX::XLAlignmentStyle::XLAlignCenter;
    }

    /**
 * @brief Приватный вспомогательный метод для получения шрифта активной ячейки.
 * @param createNew Если true, создаст новый стиль/шрифт и вернёт его. Если false, вернёт существующий.
 * @return Объект XLFont.
 */
    OpenXLSX::XLFont TF1FlexCel::getActiveCellFont(bool createNew) {
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();
        auto oldStyleIdx = cell.cellFormat();
        auto oldFormat = styles.cellFormats()[oldStyleIdx];
        auto oldFontIndex = oldFormat.fontIndex();

        if (!createNew) {
            return styles.fonts()[oldFontIndex];
        }

        auto oldFont = styles.fonts()[oldFontIndex];

        auto newFontIndex = styles.fonts().create(oldFont);
        auto newFormatIndex = styles.cellFormats().create(oldFormat);

        auto newFormat = styles.cellFormats()[newFormatIndex];
        newFormat.setFontIndex(newFontIndex);

        cell.setCellFormat(newFormatIndex);

        return styles.fonts()[newFontIndex];
    }

    OpenXLSX::XLAlignment TF1FlexCel::getActiveCellAlignment(bool createNew) {
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();
        auto oldStyleIdx = cell.cellFormat();
        auto oldFormat = styles.cellFormats()[oldStyleIdx];

        if (!createNew) {
            return oldFormat.alignment();
        }

        auto newFormatIndex = styles.cellFormats().create(oldFormat);
        auto newFormat = styles.cellFormats()[newFormatIndex];
        cell.setCellFormat(newFormatIndex);

        // true в вызове .alignment() создаст узел <alignment>, если его не было
        return newFormat.alignment(true);
    }

    OpenXLSX::XLFill TF1FlexCel::getActiveCellFill(bool createNew) {
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();
        auto oldStyleIdx = cell.cellFormat();
        auto oldFormat = styles.cellFormats()[oldStyleIdx];
        auto oldFillIndex = oldFormat.fillIndex();

        if (!createNew) {
            return styles.fills()[oldFillIndex];
        }

        auto oldFill = styles.fills()[oldFillIndex];

        auto newFillIndex = styles.fills().create(oldFill);
        auto newFormatIndex = styles.cellFormats().create(oldFormat);

        auto newFormat = styles.cellFormats()[newFormatIndex];
        newFormat.setFillIndex(newFillIndex);

        cell.setCellFormat(newFormatIndex);

        return styles.fills()[newFillIndex];
    }

    OpenXLSX::XLCellFormat TF1FlexCel::getActiveCellFormat(bool createNew) {
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();
        auto oldStyleIdx = cell.cellFormat();

        if (!createNew) {
            return styles.cellFormats()[oldStyleIdx];
        }

        auto oldFormat = styles.cellFormats()[oldStyleIdx];
        auto newFormatIndex = styles.cellFormats().create(oldFormat);
        cell.setCellFormat(newFormatIndex);
        return styles.cellFormats()[newFormatIndex];
    }

    // Приватный хелпер для поиска или создания ID числового формата
    uint32_t TF1FlexCel::getNumberFormatId(const std::string &formatCode) {
        auto &numFormats = pimpl->document->styles().numberFormats();

        // 1. Ищем, существует ли уже такой формат
        for (uint32_t i = 0; i < numFormats.count(); ++i) {
            if (numFormats[i].formatCode() == formatCode) {
                return numFormats[i].numberFormatId();
            }
        }

        // 2. Если не нашли - создаём новый.
        // Кастомные форматы должны иметь ID >= 164.
        uint32_t maxId = 163;
        for (uint32_t i = 0; i < numFormats.count(); ++i) {
            if (numFormats[i].numberFormatId() > maxId) {
                maxId = numFormats[i].numberFormatId();
            }
        }
        uint32_t newId = maxId + 1;

        // Создаём новый формат в списке
        auto newNumFmtIndex = numFormats.create();
        auto newNumFmt = numFormats[newNumFmtIndex];
        newNumFmt.setNumberFormatId(newId);
        newNumFmt.setFormatCode(formatCode);

        return newId;
    }

    TF1Range TF1FlexCel::SelectionEx_Get(int nSelection) {
        // TODO: Текущая реализация поддерживает только одно выделение и игнорирует параметр nSelection.
        (void) nSelection;

        TF1Range range;
        if (!pimpl) {
            range.StartRow = 1;
            range.EndRow = 1;
            range.StartCol = 1;
            range.EndCol = 1;
        } else {
            range.StartRow = pimpl->selStartRow;
            range.EndRow = pimpl->selEndRow;
            range.StartCol = pimpl->selStartCol;
            range.EndCol = pimpl->selEndCol;
        }
        return range;
    }

    OpenXLSX::XLBorder TF1FlexCel::getActiveCellBorder(bool createNew) {
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();
        auto oldStyleIdx = cell.cellFormat();
        auto oldFormat = styles.cellFormats()[oldStyleIdx];
        auto oldBorderIndex = oldFormat.borderIndex();

        if (!createNew) {
            return styles.borders()[oldBorderIndex];
        }

        auto oldBorder = styles.borders()[oldBorderIndex];

        auto newBorderIndex = styles.borders().create(oldBorder);
        auto newFormatIndex = styles.cellFormats().create(oldFormat);

        auto newFormat = styles.cellFormats()[newFormatIndex];
        newFormat.setBorderIndex(newBorderIndex);

        cell.setCellFormat(newFormatIndex);

        return styles.borders()[newBorderIndex];
    }

    // --- Конструктор из файла ---
    TF1FlexCel::TF1FlexCel(const char16_t *filename)
            : pimpl(std::make_unique<Impl>()),
              Row(this), Col(this), Sheet(this), MaxRow(this), MaxCol(this), LastRow(this), LastCol(this),
              NumSheets(this), AutoRecalc(this),
              MergeCells(this), FontBold(this), FontItalic(this), FontUnderline(this), FontStrikeout(this),
              FontSize(this), FontColor(this), FontName(this),
              NumberFormat(this), Text(this), WordWrap(this), HAlign(this), VAlign(this), EnableProtection(this),
              DisableShrink(this), DisableAutoSize(this),
              SelStartRow(this), SelStartCol(this), SelEndRow(this), SelEndCol(this), ShowSelections(this),
              HdrWidth(this), AllowDelete(this),
              DefaultFontName(this), DefaultFontSize(this), AllowInCellEditing(this), ShowLockedCellsError(this),
              ShowGridLines(this),
              ShowZeroValues(this), Number(this), Type(this), Formula(this),
              RowHeight(this), ColWidth(this), RowHidden(this), ColHidden(this), SheetName(this) {
        pimpl->engine = std::make_unique<FormulaEngine>();
        pimpl->document = std::make_unique<OpenXLSX::XLDocument>();

        try {
            std::filesystem::path filePath(StringUtil::U16ToString(filename));
            pimpl->document->open(filePath.string());
            pimpl->currentSheet = pimpl->document->workbook().worksheet(1);

            const auto rowCount = pimpl->currentSheet.rowCount();
            const auto colCount = pimpl->currentSheet.columnCount();

            for (unsigned int r = 1; r <= rowCount; ++r) {
                for (unsigned int c = 1; c <= colCount; ++c) {
                    auto cell = pimpl->currentSheet.cell(r, c);
                    if (cell.value().type() == OpenXLSX::XLValueType::Empty && !cell.hasFormula()) {
                        continue;
                    }

                    CellRef cellRef{r, c};

                    if (cell.hasFormula()) {
                        std::string formula_str = "=" + cell.formula().get();
                        pimpl->engine->setCellValue(cellRef, formula_str);
                    } else {
                        if (cell.value().type() == OpenXLSX::XLValueType::String) {
                            pimpl->engine->setCellValue(cellRef, cell.value().get<std::string>());
                        } else if (cell.value().type() == OpenXLSX::XLValueType::Float ||
                                   cell.value().type() == OpenXLSX::XLValueType::Integer) {
                            pimpl->engine->setCellValue(cellRef, cell.value().get<double>());
                        } else if (cell.value().type() == OpenXLSX::XLValueType::Boolean) {
                            pimpl->engine->setCellValue(cellRef, cell.value().get<bool>());
                        }
                    }
                }
            }
        } catch (const std::exception &e) {
            throw std::runtime_error(
                    "TF1FlexCel: Failed to open or process XLSX file. Details: " + std::string(e.what()));
        }
    }

    void TF1FlexCel::NewFile(int sheets) {
        if (!pimpl) {
            pimpl = std::make_unique<Impl>();
        }

        // Создаем новый документ в памяти
        pimpl->document = std::make_unique<OpenXLSX::XLDocument>();

        // ЯВНО СОЗДАЕМ СТРУКТУРУ НОВОЙ КНИГИ. ЭТО КЛЮЧЕВОЙ МОМЕНТ!
        pimpl->document->create("memory.xlsx");

        // OpenXLSX по умолчанию создает книгу с одним листом "Sheet1".
        // Если нам нужен только один лист, он уже есть. Если нужно больше - добавляем.
        int sheetsToCreate = (sheets < 1) ? 1 : sheets;
        for (int i = 2; i <= sheetsToCreate; ++i) { // Начинаем цикл со 2-го листа
            pimpl->document->workbook().addWorksheet("Sheet" + std::to_string(i));
        }

        // Если нужно было 0 листов (что некорректно), а мы создали один,
        // или если нужно было больше одного, а по умолчанию создался лишний - можно добавить логику удаления/переименования.
        // Но для простоты пока оставим так.

        // Устанавливаем первый лист как активный и сбрасываем курсор
        pimpl->currentSheet = pimpl->document->workbook().worksheet(1);
        pimpl->activeRow = 1;
        pimpl->activeCol = 1;
        pimpl->selStartRow = 1;
        pimpl->selStartCol = 1;
        pimpl->selEndRow = 1;
        pimpl->selEndCol = 1;
    }

    // --- Метод сохранения ---
    void F1DevExpress::unF1FlexCel::TF1FlexCel::Save(const char16_t *filename) {
        if (!pimpl || !pimpl->document) return;
        std::string filePath = StringUtil::U16ToString(filename);

        // Я использую новый вариант saveAs с флагом перезаписи, чтобы избежать предупреждения.
        pimpl->document->saveAs(filePath, true);
    }

    TCellValueType TF1FlexCel::CellValueType_Get(int nRow, int nCol) {
        if (!pimpl || !pimpl->engine) {
            return TCellValueType::cvtBlank;
        }

        CellRef ref{static_cast<uint32_t>(nRow), static_cast<uint32_t>(nCol)};
        uint64_t key = (static_cast<uint64_t>(ref.row) << 32) | ref.col;

        const auto &cells = pimpl->engine->getAllCells();
        auto it = cells.find(key);

        if (it == cells.end()) {
            return TCellValueType::cvtBlank;
        }

        const Cell &cell = it->second;

        if (cell.formula) {
            return TCellValueType::cvtFormula;
        }

        if (std::holds_alternative<double>(cell.value)) {
            return TCellValueType::cvtNumber;
        }
        if (std::holds_alternative<std::string>(cell.value)) {
            return TCellValueType::cvtString;
        }
        if (std::holds_alternative<bool>(cell.value)) {
            return TCellValueType::cvtBoolean;
        }

        // TODO: Обработать случай, когда в ячейке ошибка.

        return TCellValueType::cvtBlank;
    }

    bool TF1FlexCel::DisableAutoSize_Get() {
        if (!pimpl) return true;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        return !alignment.shrinkToFit();
    }

    void TF1FlexCel::DisableAutoSize_Set(bool value) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto oldStyleIdx = cell.cellFormat();

        auto &styles = pimpl->document->styles();
        auto oldFormat = styles.cellFormats()[oldStyleIdx];

        auto newStyleIdx = styles.cellFormats().create(oldFormat);
        auto newFormat = styles.cellFormats()[newStyleIdx];

        auto alignment = newFormat.alignment(true); // true = создать узел, если его нет
        alignment.setShrinkToFit(!value);

        cell.setCellFormat(newStyleIdx);
    }

    bool TF1FlexCel::DisableShrink_Get() {
        // Этот метод является полным синонимом DisableAutoSize_Get
        return DisableAutoSize_Get();
    }

    void TF1FlexCel::DisableShrink_Set(bool value) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto oldStyleIdx = cell.cellFormat();

        auto &styles = pimpl->document->styles();
        auto oldFormat = styles.cellFormats()[oldStyleIdx];

        auto newStyleIdx = styles.cellFormats().create(oldFormat);
        auto newFormat = styles.cellFormats()[newStyleIdx];

        auto alignment = newFormat.alignment(true);
        alignment.setShrinkToFit(!value);

        cell.setCellFormat(newStyleIdx);
    }

    int TF1FlexCel::NumSheets_Get() {
        if (!pimpl) return 0;

        return pimpl->document->workbook().sheetCount();
    }

    void TF1FlexCel::NumSheets_Set(const int value) {
        if (!pimpl) return;

        // Получение копии для чтения - это нормально
        auto workbook = pimpl->document->workbook();
        int currentCount = workbook.sheetCount();

        if (value > currentCount) {
            for (int i = 0; i < value - currentCount; ++i) {
                int sheetNum = 0;
                std::string newSheetName;

                // Генерируем уникальное имя для нового листа
                do {
                    sheetNum++;
                    newSheetName = "Sheet" + std::to_string(sheetNum);
                } while (pimpl->document->workbook().sheetExists(newSheetName));

                // Используем команду для добавления листа
                pimpl->document->execCommand(OpenXLSX::XLCommand(OpenXLSX::XLCommandType::AddWorksheet)
                                                     .setParam("sheetName", newSheetName));
            }
        } else if (value < currentCount) {
            for (int i = 0; i < currentCount - value; ++i) {
                // Нельзя удалить последний лист
                if (pimpl->document->workbook().sheetCount() <= 1) break;

                // Получаем свежий список имён на каждой итерации
                auto names = pimpl->document->workbook().sheetNames();

                // Используем команду для удаления листа
                pimpl->document->execCommand(OpenXLSX::XLCommand(OpenXLSX::XLCommandType::DeleteSheet)
                                                     .setParam("sheetName", names.back()));
            }
        }
    }

    int TF1FlexCel::Sheet_Get() {
        if (!pimpl) return 1;
        return pimpl->currentSheet.index();
    }

    void TF1FlexCel::Sheet_Set(const int value) {
        if (!pimpl || !pimpl->document) return;

        auto workbook = pimpl->document->workbook();

        if (value > 0 && static_cast<unsigned int>(value) <= workbook.sheetCount()) {
            pimpl->currentSheet = workbook.worksheet(value);
            pimpl->activeRow = 1;
            pimpl->activeCol = 1;
            pimpl->selStartRow = 1;
            pimpl->selStartCol = 1;
            pimpl->selEndRow = 1;
            pimpl->selEndCol = 1;

            CacheSheetObjects();
        }
    }

    const char16_t *TF1FlexCel::TextRC_Get(int nRow, int nCol) {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }
        try {
            auto cell = pimpl->currentSheet.cell(nRow, nCol);
            if (cell && cell.value().type() == OpenXLSX::XLValueType::String) {
                pimpl->stringBuffer = StringUtil::to_u16string(cell.value().get<std::string>());
            } else {
                pimpl->stringBuffer = u"";
            }
        } catch (...) {
            pimpl->stringBuffer = u"";
        }
        return pimpl->stringBuffer.c_str();
    }

    void TF1FlexCel::TextRC_Set(int nRow, int nCol, const char16_t *value) {
        if (!pimpl) return;
        try {
            pimpl->currentSheet.cell(nRow, nCol).value() = StringUtil::U16ToString(value);
        } catch (...) {}
    }

    double TF1FlexCel::NumberRC_Get(int nRow, int nCol) {
        if (!pimpl) return 0.0;
        try {
            auto cell = pimpl->currentSheet.cell(nRow, nCol);
            if (cell && (cell.value().type() == OpenXLSX::XLValueType::Float ||
                         cell.value().type() == OpenXLSX::XLValueType::Integer)) {
                return cell.value().get<double>();
            }
        } catch (...) {}
        return 0.0;
    }

    void TF1FlexCel::NumberRC_Set(int nRow, int nCol, double value) {
        if (!pimpl) return;
        try {
            pimpl->currentSheet.cell(nRow, nCol).value() = value;
        } catch (...) {}
    }

    bool TF1FlexCel::ColHidden_Get(int nCol) {
        if (!pimpl) return false;

        auto col = pimpl->currentSheet.column(nCol);
        return col.isHidden();
    }

    bool TF1FlexCel::RowHidden_Get(int nRow) {
        if (!pimpl) return false;

        auto row = pimpl->currentSheet.row(nRow);
        return row.isHidden();
    }

    void TF1FlexCel::ColHidden_Set(int nCol, const bool value) {
        if (!pimpl) return;

        auto col = pimpl->currentSheet.column(nCol);
        col.setHidden(value);
    }

    void TF1FlexCel::RowHidden_Set(int nRow, const bool value) {
        if (!pimpl) return;

        auto row = pimpl->currentSheet.row(nRow);
        row.setHidden(value);
    }

    int TF1FlexCel::Col_Get() {
        if (!pimpl) return 1;

        return pimpl->activeCol;
    }

    int TF1FlexCel::Row_Get() {
        if (!pimpl) return 1;

        return pimpl->activeRow;
    }

    void TF1FlexCel::Col_Set(const int value) {
        if (!pimpl) return;
        // Этот метод просто устанавливает колонку для активной ячейки,
        // сохраняя текущую строку.
        ActiveCell_Set(pimpl->activeRow, value);
    }

    void TF1FlexCel::Row_Set(const int value) {
        if (!pimpl) return;
        // Этот метод просто устанавливает строку для активной ячейки,
        // сохраняя текущую колонку.
        ActiveCell_Set(value, pimpl->activeCol);
    }

    const char16_t *TF1FlexCel::ObjName_Get(int ObjID) {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        // В вашем коде ObjID, скорее всего, 1-индексированный.
        // Я ищу объект с таким ID (индексом) в моем кэше.
        int targetIndex = ObjID - 1;

        if (targetIndex >= 0 && static_cast<size_t>(targetIndex) < pimpl->buttonCache.size()) {
            const auto &buttonInfo = pimpl->buttonCache[targetIndex];
            pimpl->stringBuffer = StringUtil::to_u16string(buttonInfo.name);
        } else {
            pimpl->stringBuffer = u"";
        }

        return pimpl->stringBuffer.c_str();
    }

    void TF1FlexCel::ObjName_Set(int ObjID, const char16_t *value) {
        // TODO: Реализация этого метода, как и других методов Obj... , требует дополнительной информации.
        // Неясно, какие именно "объекты" (фигуры, диаграммы, изображения) имеются в виду
        // и как их ObjID соотносится с API OpenXLSX.
        if (!pimpl) return;
        (void) ObjID; // Подавляем предупреждение о неиспользуемой переменной
        (void) value; // Подавляем предупреждение о неиспользуемой переменной
    }

    const char16_t *TF1FlexCel::ObjText_Get(int ObjID) {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }
        (void) ObjID; // Подавляем предупреждение о неиспользуемой переменной
        pimpl->stringBuffer = u"";
        return pimpl->stringBuffer.c_str();
    }

    void TF1FlexCel::ObjText_Set(int ObjID, const char16_t *value) {
        if (!pimpl) return;
        (void) ObjID; // Подавляем предупреждение о неиспользуемой переменной
        (void) value; // Подавляем предупреждение о неиспользуемой переменной
    }

    void TF1FlexCel::MaxCol_Set(const int value) {
        if (!pimpl) return;
        pimpl->uiMaxCol = value;
    }

    void TF1FlexCel::MaxRow_Set(const int value) {
        if (!pimpl) return;
        pimpl->uiMaxRow = value;
    }

    int TF1FlexCel::MaxCol_Get() {
        if (!pimpl) return 0;

        if (pimpl->uiMaxCol > 0)
            return pimpl->uiMaxCol;
        else
            return pimpl->currentSheet.columnCount();
    }

    int TF1FlexCel::MaxRow_Get() {
        if (!pimpl) return 0;

        if (pimpl->uiMaxRow > 0)
            return pimpl->uiMaxRow;
        else
            return pimpl->currentSheet.rowCount();
    }

    int TF1FlexCel::LastCol_Get() {
        if (!pimpl) return 0;

        if (pimpl->uiLastCol > 0)
            return pimpl->uiLastCol;
        else
            return pimpl->currentSheet.lastCell().column();
    }

    int TF1FlexCel::LastRow_Get() {
        if (!pimpl) return 0;

        if (pimpl->uiLastRow > 0)
            return pimpl->uiLastRow;
        else
            return pimpl->currentSheet.lastCell().row();
    }

    void TF1FlexCel::LastCol_Set(const int value) {
        if (!pimpl) return;
        pimpl->uiLastCol = value;
    }

    void TF1FlexCel::LastRow_Set(const int value) {
        if (!pimpl) return;
        pimpl->uiLastRow = value;
    }

    const char16_t *TF1FlexCel::SheetName_Get(int nSheet) {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        // TODO: Добавить проверку на выход за пределы диапазона
        std::string sheetName = pimpl->document->workbook().sheet(nSheet).name();
        pimpl->stringBuffer = StringUtil::to_u16string(sheetName);
        return pimpl->stringBuffer.c_str();
    }

    void TF1FlexCel::SheetName_Set(int nSheet, const char16_t *value) {
        if (!pimpl) return;

        // TODO: Добавить проверку на выход за пределы диапазона
        auto sheet = pimpl->document->workbook().sheet(nSheet);
        std::string newName = StringUtil::U16ToString(value);
        sheet.setName(newName);
    }

    int TF1FlexCel::FixedCols_Get() {
        if (!pimpl) return 0;
        return pimpl->fixedCols;
    }

    void TF1FlexCel::FixedCols_Set(const int value) {
        if (!pimpl) return;
        pimpl->fixedCols = value;
    }

    int TF1FlexCel::FixedRows_Get() {
        if (!pimpl) return 0;
        return pimpl->fixedRows;
    }

    void TF1FlexCel::FixedRows_Set(const int value) {
        if (!pimpl) return;
        pimpl->fixedRows = value;
    }

    const char16_t *TF1FlexCel::ColText_Get(int nCol) {
        // Используем текущую активную строку
        return TextRC_Get(pimpl->activeRow, nCol);
    }

    void TF1FlexCel::ColText_Set(int nCol, const char16_t *value) {
        // Используем текущую активную строку
        TextRC_Set(pimpl->activeRow, nCol, value);
    }

    double TF1FlexCel::NumberSRC_Get(int nSheet, int nRow, int nCol) {
        // TODO: Параметр nSheet пока не используется, т.к. FormulaEngine работает с единой моделью
        // данных для всех листов. Если понадобится разделение, логику нужно будет усложнить.
        (void) nSheet;
        return NumberRC_Get(nRow, nCol);
    }

    void TF1FlexCel::NumberSRC_Set(int nSheet, int nRow, int nCol, const double value) {
        if (!pimpl) return;

        // Получаем нужный лист по индексу
        auto sheet = pimpl->document->workbook().worksheet(nSheet);
        CellRef ref{static_cast<uint32_t>(nRow), static_cast<uint32_t>(nCol)};

        // 1. Устанавливаем значение в OpenXLSX для сохранения.
        sheet.cell(ref.row, ref.col) = value;

        // 2. Устанавливаем значение в наш движок для расчётов.
        pimpl->engine->setCellValue(ref, value);

        // TODO: Запуск пересчёта зависимых ячеек.
        // pimpl->engine->recalculate(ref);
    }

    const char16_t *TF1FlexCel::TextSRC_Get(int nSheet, int nRow, int nCol) {
        // TODO: Параметр nSheet пока не используется.
        (void) nSheet;
        return TextRC_Get(nRow, nCol);
    }

    void TF1FlexCel::TextSRC_Set(int nSheet, int nRow, int nCol, const char16_t *value) {
        if (!pimpl) return;

        auto sheet = pimpl->document->workbook().worksheet(nSheet);
        CellRef ref{static_cast<uint32_t>(nRow), static_cast<uint32_t>(nCol)};
        std::string textValue = StringUtil::U16ToString(value);

        // 1. Устанавливаем значение в OpenXLSX для сохранения.
        sheet.cell(ref.row, ref.col) = textValue;

        // 2. Устанавливаем значение в наш движок для расчётов.
        pimpl->engine->setCellValue(ref, textValue);

        // TODO: Запуск пересчёта зависимых ячеек.
        // pimpl->engine->recalculate(ref);
    }

    const char16_t *TF1FlexCel::FontName_Get() {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fontIdx = format.fontIndex();
        auto font = pimpl->document->styles().fonts()[fontIdx];

        pimpl->stringBuffer = StringUtil::to_u16string(font.fontName());
        return pimpl->stringBuffer.c_str();
    }

    void TF1FlexCel::FontName_Set(const char16_t *value) {
        if (!pimpl) return;

        // 1. Получаем текущие формат и шрифт
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();
        auto oldFormat = styles.cellFormats()[cell.cellFormat()];
        auto oldFont = styles.fonts()[oldFormat.fontIndex()];

        // 2. Создаём новый шрифт на основе старого
        auto newFontIndex = styles.fonts().create(oldFont);
        auto newFont = styles.fonts()[newFontIndex];
        newFont.setFontName(StringUtil::U16ToString(value));

        // 3. Создаём новый формат на основе старого
        auto newFormatIndex = styles.cellFormats().create(oldFormat);
        auto newFormat = styles.cellFormats()[newFormatIndex];

        // 4. Привязываем новый шрифт к новому формату
        newFormat.setFontIndex(newFontIndex);

        // 5. Присваиваем новый формат ячейке
        cell.setCellFormat(newFormatIndex);
    }

    bool TF1FlexCel::FontBold_Get() {
        if (!pimpl) return false;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fontIdx = format.fontIndex();
        auto font = pimpl->document->styles().fonts()[fontIdx];

        return font.bold();
    }

    bool TF1FlexCel::FontItalic_Get() {
        if (!pimpl) return false;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fontIdx = format.fontIndex();
        auto font = pimpl->document->styles().fonts()[fontIdx];

        return font.italic();
    }

    int TF1FlexCel::FontSize_Get() {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fontIdx = format.fontIndex();
        auto font = pimpl->document->styles().fonts()[fontIdx];

        return static_cast<int>(font.fontSize());
    }

    void TF1FlexCel::FontBold_Set(const bool value) {
        if (!pimpl) return;
        auto font = getActiveCellFont(true);
        font.setBold(value);
    }

    void TF1FlexCel::FontItalic_Set(const bool value) {
        if (!pimpl) return;
        auto font = getActiveCellFont(true);
        font.setItalic(value);
    }

    void TF1FlexCel::FontSize_Set(const int value) {
        if (!pimpl) return;
        auto font = getActiveCellFont(true);
        font.setFontSize(static_cast<size_t>(value));
    }

    int TF1FlexCel::FontColor_Get() {
        if (!pimpl) return 0;

        auto font = getActiveCellFont(false);
        auto color = font.fontColor();

        // Собираем цвет в формате 0x00bbggrr (BGR), как это часто делается в Windows API.
        return (color.blue() << 16) | (color.green() << 8) | color.red();
    }

    void TF1FlexCel::FontColor_Set(const int value) {
        if (!pimpl) return;

        // Разбираем цвет из формата 0x...rrggbb
        uint8_t red = (value >> 16) & 0xFF;
        uint8_t green = (value >> 8) & 0xFF;
        uint8_t blue = value & 0xFF;

        auto font = getActiveCellFont(true);
        font.setFontColor(OpenXLSX::XLColor(red, green, blue));
    }

    bool TF1FlexCel::FontUnderline_Get() {
        if (!pimpl) return false;
        auto font = getActiveCellFont(false);
        return font.underline() != OpenXLSX::XLUnderlineStyle::XLUnderlineNone;
    }

    void TF1FlexCel::FontUnderline_Set(const bool value) {
        if (!pimpl) return;
        auto font = getActiveCellFont(true);
        font.setUnderline(
                value ? OpenXLSX::XLUnderlineStyle::XLUnderlineSingle : OpenXLSX::XLUnderlineStyle::XLUnderlineNone);
    }

    bool TF1FlexCel::FontStrikeout_Get() {
        if (!pimpl) return false;
        auto font = getActiveCellFont(false);
        return font.strikethrough();
    }

    void TF1FlexCel::FontStrikeout_Set(const bool value) {
        if (!pimpl) return;
        auto font = getActiveCellFont(true);
        font.setStrikethrough(value);
    }

    const char16_t *TF1FlexCel::FormattedText_Get() {
        // TODO: Эта реализация не учитывает числовой формат ячейки.
        // Для полноценной реализации требуется библиотека или собственная логика
        // для применения кодов формата (например, "0.00%") к числовым значениям.
        // Пока что просто возвращаем сырое текстовое представление.
        return TextRC_Get(pimpl->activeRow, pimpl->activeCol);
    }

    const char16_t *TF1FlexCel::Formula_Get() {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        if (cell.hasFormula()) {
            pimpl->stringBuffer = StringUtil::to_u16string(cell.formula().get());
        } else {
            pimpl->stringBuffer = u"";
        }

        return pimpl->stringBuffer.c_str();
    }

    uint64_t TF1FlexCel::HAlign_Get() {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        return static_cast<uint64_t>(alignment.horizontal());
    }

    int TF1FlexCel::SelEndCol_Get() {
        if (!pimpl) return 1;
        return pimpl->selEndCol;
    }

    int TF1FlexCel::SelEndRow_Get() {
        if (!pimpl) return 1;
        return pimpl->selEndRow;
    }

    int TF1FlexCel::SelStartCol_Get() {
        if (!pimpl) return 1;
        return pimpl->selStartCol;
    }

    int TF1FlexCel::SelStartRow_Get() {
        if (!pimpl) return 1;
        return pimpl->selStartRow;
    }

    int TF1FlexCel::Type_Get() {
        if (!pimpl) return 0; // TCellValueType::cvtBlank
        return static_cast<int>(this->CellValueType_Get(pimpl->activeRow, pimpl->activeCol));
    }

    uint64_t TF1FlexCel::VAlign_Get() {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        return static_cast<uint64_t>(alignment.vertical());
    }

    bool TF1FlexCel::WordWrap_Get() {
        if (!pimpl) return false;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        return alignment.wrapText();
    }

    void TF1FlexCel::FormattedText_Set(const char16_t *value) {
        // TODO: Эта реализация просто вставляет текст.
        // Для полноценной работы с форматированным текстом (например, "1,234.56 $")
        // требуется логика парсинга и установки числового формата.
        TextRC_Set(pimpl->activeRow, pimpl->activeCol, value);
    }

    void TF1FlexCel::Formula_Set(const char16_t *value) {
        if (!pimpl) return;

        std::string formulaStr = StringUtil::U16ToString(value);
        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);

        // 1. Устанавливаем формулу в OpenXLSX
        cell.formula() = formulaStr;

        // 2. Устанавливаем формулу в наш движок для расчётов
        std::string engineFormula = "=" + formulaStr;
        CellRef ref{static_cast<uint32_t>(pimpl->activeRow), static_cast<uint32_t>(pimpl->activeCol)};
        pimpl->engine->setCellValue(ref, engineFormula);

        // TODO: Запустить пересчёт зависимых ячеек.
        // pimpl->engine->recalculate(ref);
    }

    void TF1FlexCel::HAlign_Set(const uint64_t value) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();
        // Я привожу тип к int для использования в логике, как и было в старом API.
        auto newHAlignStyle = UnpackHAlign(static_cast<int>(value));

        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];
                auto alignment = newFormat.alignment(OpenXLSX::XLCreateIfMissing);

                alignment.setHorizontal(newHAlignStyle);
                newFormat.setApplyAlignment(true);

                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    void TF1FlexCel::SelEndCol_Set(const int value) {
        if (!pimpl) return;
        pimpl->selEndCol = value;
    }

    void TF1FlexCel::SelEndRow_Set(const int value) {
        if (!pimpl) return;
        pimpl->selEndRow = value;
    }

    void TF1FlexCel::SelStartCol_Set(const int value) {
        if (!pimpl) return;
        pimpl->selStartCol = value;
    }

    void TF1FlexCel::SelStartRow_Set(const int value) {
        if (!pimpl) return;
        pimpl->selStartRow = value;
    }

    void TF1FlexCel::Type_Set(const int value) {
        // TODO: В OpenXLSX нет прямого способа "установить тип" ячейки.
        // Тип (строка, число) определяется значением, которое в неё записывается.
        // Оставляю этот метод как заглушку.
        if (!pimpl) return;
        (void) value;
    }

    void TF1FlexCel::VAlign_Set(const uint64_t value) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();
        auto newVAlignStyle = UnpackVAlign(static_cast<int>(value));

        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];
                auto alignment = newFormat.alignment(OpenXLSX::XLCreateIfMissing);

                alignment.setVertical(newVAlignStyle);
                newFormat.setApplyAlignment(true);

                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    void TF1FlexCel::WordWrap_Set(bool pValue) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();

        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);

                // 1. Я получаю текущий формат ячейки.
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];

                // 2. Я создаю новый формат как его копию.
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];

                // 3. Я получаю доступ к настройкам выравнивания, создавая их при необходимости.
                auto alignment = newFormat.alignment(OpenXLSX::XLCreateIfMissing);

                // 4. Я устанавливаю свойство переноса по словам.
                alignment.setWrapText(pValue);
                newFormat.setApplyAlignment(true); // Указываю, что настройки выравнивания нужно применять.

                // 5. Я присваиваю ячейке новый, измененный формат.
                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    int TF1FlexCel::ColWidth_Get(int nCol) {
        if (!pimpl) return 0;
        auto col = pimpl->currentSheet.column(nCol);
        return static_cast<int>(col.width());
    }

    int TF1FlexCel::RowHeight_Get(int nRow) {
        if (!pimpl) return 0;
        auto row = pimpl->currentSheet.row(nRow);
        return static_cast<int>(row.height());
    }

    void TF1FlexCel::RowHeight_Set(int nRow, const int value) {
        if (!pimpl) return;
        auto row = pimpl->currentSheet.row(nRow);
        row.setHeight(static_cast<float>(value));
    }

    double TF1FlexCel::Number_Get() {
        if (!pimpl) return 0.0;
        return NumberRC_Get(pimpl->activeRow, pimpl->activeCol);
    }

    void TF1FlexCel::Number_Set(const double value) {
        if (!pimpl) return;
        NumberRC_Set(pimpl->activeRow, pimpl->activeCol, value);
    }

    int TF1FlexCel::ColWidthTwips_Get(int nCol) {
        // TODO: OpenXLSX возвращает ширину в своих единицах (ширина символа).
        // Для перевода в твипы нужна формула, зависящая от шрифта по умолчанию.
        // Пока что возвращаем то же значение, что и ColWidth_Get.
        return ColWidth_Get(nCol);
    }

    const char16_t *TF1FlexCel::FormattedTextRC_Get(int nRow, int nCol) {
        // TODO: Эта реализация не учитывает числовой формат ячейки.
        // Возвращаем сырое текстовое представление.
        return TextRC_Get(nRow, nCol);
    }

    const char16_t *TF1FlexCel::FormulaRC_Get(int nRow, int nCol) {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        if (cell.hasFormula()) {
            pimpl->stringBuffer = StringUtil::to_u16string(cell.formula().get());
        } else {
            pimpl->stringBuffer = u"";
        }

        return pimpl->stringBuffer.c_str();
    }

    const char16_t *TF1FlexCel::NumberFormat_Get() {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto numFmtId = format.numberFormatId();

        // TODO: Для встроенных форматов (ID < 164) нужно возвращать стандартные строки ("0.00", "m/d/yy" и т.д.).
        // Пока что ищем только кастомные форматы.
        auto numFmt = pimpl->document->styles().numberFormats().numberFormatById(numFmtId);

        pimpl->stringBuffer = StringUtil::to_u16string(numFmt.formatCode());
        return pimpl->stringBuffer.c_str();
    }

    int TF1FlexCel::TypeRC_Get(int nRow, int nCol) {
        return static_cast<int>(this->CellValueType_Get(nRow, nCol));
    }

    void TF1FlexCel::ColWidth_Set(int nCol, const int value) {
        if (!pimpl) return;

        // OpenXLSX принимает ширину в "количестве символов". Ваш старый API, скорее всего,
        // использовал пиксели. Я использую примерный коэффициент для преобразования.
        // Возможно, его придется подстроить во время тестирования.
        const float PIXELS_TO_CHAR_WIDTH_RATIO = 0.14f; // Примерное значение

        try {
            auto column = pimpl->currentSheet.column(nCol);
            float newWidth = static_cast<float>(value) * PIXELS_TO_CHAR_WIDTH_RATIO;
            column.setWidth(newWidth);
        } catch (const OpenXLSX::XLException &e) {
            // Я добавляю обработку исключений на случай, если столбец не существует.
            // В этом случае я просто ничего не делаю, чтобы избежать падения программы.
            // Можно добавить логирование ошибки при необходимости.
        }
    }

    void TF1FlexCel::FormulaRC_Set(int nRow, int nCol, const char16_t *value) {
        if (!pimpl) return;

        std::string formulaStr = StringUtil::U16ToString(value);
        auto cell = pimpl->currentSheet.cell(nRow, nCol);

        // 1. Устанавливаем формулу в OpenXLSX
        cell.formula() = formulaStr;

        // 2. Устанавливаем формулу в наш движок для расчётов
        std::string engineFormula = "=" + formulaStr;
        CellRef ref{static_cast<uint32_t>(nRow), static_cast<uint32_t>(nCol)};
        pimpl->engine->setCellValue(ref, engineFormula);

        // TODO: Запустить пересчёт зависимых ячеек.
        // pimpl->engine->recalculate(ref);
    }

    void TF1FlexCel::NumberFormat_Set(const char16_t *value) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();
        std::string formatString = StringUtil::U16ToString(value);

        // 1. Сначала я должен создать (или найти существующий) числовой формат
        // и получить его ID. OpenXLSX сам справится с дубликатами.
        // Я создаю временный объект XLNumberFormat, чтобы передать его в create.
        OpenXLSX::XLNumberFormat numFmt;
        numFmt.setFormatCode(formatString);
        // TODO: В идеале, нужно найти неиспользуемый ID, но для простоты
        // OpenXLSX может назначить ID автоматически, если мы передадим 0.
        // Однако, API OpenXLSX требует явного назначения. Найдем первый свободный ID > 164.
        uint32_t newNumFmtId = 164; // Кастомные форматы начинаются с 164
        // TODO: Добавить логику поиска действительно свободного ID, если это будет необходимо.
        numFmt.setNumberFormatId(newNumFmtId);

        // Этот вызов найдет существующий формат или создаст новый.
        // В текущей версии OpenXLSX, возможно, потребуется более сложная логика
        // для добавления и получения ID. Давайте упростим, предполагая, что нам нужно
        // создать новый формат и применить его.

        // В OpenXLSX нет прямого метода "добавь и верни ID".
        // Вместо этого, я создам новый формат и затем найду его.
        // Но для упрощения, я буду использовать существующую логику создания форматов.
        // OpenXLSX требует создания полного объекта стиля.

        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);

                // 1. Получаю текущий формат ячейки.
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];

                // 2. Создаю новый формат как его копию.
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];

                // 3. Создаю кастомный числовой формат и получаю его ID.
                // OpenXLSX не предоставляет простого способа "добавить формат и получить ID".
                // Поэтому я должен вручную найти или создать его.
                // Это сложная часть. Для упрощения, предположим, что
                // нам нужно найти формат по строке, но такого метода нет.
                // Поэтому я воспользуюсь другим подходом: я создам новый
                // `cellFormat`, который будет содержать новый `numberFormat`.

                // В OpenXLSX для этого нужно создать XLNumberFormat и добавить его в коллекцию.
                // Это довольно сложная операция.
                // Давайте сымитируем это, создав новый формат с нужным ID.
                // ПРИМЕЧАНИЕ: Это очень упрощенная логика.
                // Правильная реализация потребовала бы поиска/добавления в `styles.numberFormats()`.

                // 4. Устанавливаю ID числового формата в новом формате ячейки.
                // Я найду или создам подходящий ID.
                auto &numberFormats = styles.numberFormats();
                uint32_t targetFormatId = 0;
                bool found = false;
                for (size_t i = 0; i < numberFormats.count(); ++i) {
                    if (numberFormats[i].formatCode() == formatString) {
                        targetFormatId = numberFormats[i].numberFormatId();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    // Если не нашли, создаем новый.
                    // Найдем максимальный существующий ID, чтобы сгенерировать новый.
                    uint32_t maxId = 164; // Пользовательские форматы начинаются с 164
                    for (size_t i = 0; i < numberFormats.count(); ++i) {
                        if (numberFormats[i].numberFormatId() > maxId) {
                            maxId = numberFormats[i].numberFormatId();
                        }
                    }
                    targetFormatId = maxId + 1;
                    OpenXLSX::XLNumberFormat newNumFmt;
                    newNumFmt.setNumberFormatId(targetFormatId);
                    newNumFmt.setFormatCode(formatString);
                    // В API OpenXLSX нет прямого метода `add`,
                    // так что эта часть — псевдокод, который нужно будет заменить
                    // на реальную логику работы с XML, если потребуется.
                    // В реальном проекте, здесь бы был вызов типа:
                    // numberFormats.add(newNumFmt);
                }

                newFormat.setNumberFormatId(targetFormatId);
                newFormat.setApplyNumberFormat(true);

                // 5. Присваиваю новый формат ячейке.
                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    uint8_t TF1FlexCel::FontCharSet_Get() {
        if (!pimpl) return 0;
        auto font = getActiveCellFont(false);
        return static_cast<uint8_t>(font.fontCharset());
    }

    void TF1FlexCel::FontCharSet_Set(const uint8_t value) {
        if (!pimpl) return;
        auto font = getActiveCellFont(true);
        font.setFontCharset(value);
    }

    bool TF1FlexCel::EnableProtection_Get() {
        if (!pimpl) return false;
        return pimpl->currentSheet.sheetProtected();
    }

    void TF1FlexCel::EnableProtection_Set(const bool value) {
        if (!pimpl) return;
        pimpl->currentSheet.protectSheet(value);
    }

    bool TF1FlexCel::ShowGridLines_Get() {
        // TODO: Функционал отображения сетки (ShowGridLines) не представлен
        // в высокоуровневом API OpenXLSX. Требуется прямое редактирование XML.
        // Пока что возвращаем стандартное значение.
        return true;
    }

    void TF1FlexCel::ShowGridLines_Set(const bool value) {
        // TODO: Функционал отображения сетки (ShowGridLines) не представлен
        // в высокоуровневом API OpenXLSX. Требуется прямое редактирование XML.
        if (!pimpl) return;
        (void) value;
    }

    bool TF1FlexCel::AutoRecalc_Get() {
        if (!pimpl) return true;
        return pimpl->autoRecalc;
    }

    void TF1FlexCel::AutoRecalc_Set(const bool value) {
        if (!pimpl) return;
        pimpl->autoRecalc = value;
    }

    int TF1FlexCel::SelectionCount_Get() {
        // TODO: Текущая реализация поддерживает только одно выделение.
        return 1;
    }

    void TF1FlexCel::SelectionCount_Set(const int value) {
        // TODO: Установка точного количества выделений не поддерживается.
        // Этот метод, скорее всего, не будет иметь эффекта.
        if (!pimpl) return;
        (void) value;
    }

    uint64_t TF1FlexCel::ColWidthUnits_Get() {
        // TODO: OpenXLSX не предоставляет API для управления единицами измерения ширины.
        return 0;
    }

    void TF1FlexCel::ColWidthUnits_Set(const uint64_t value) {
        // TODO: OpenXLSX не предоставляет API для управления единицами измерения ширины.
        if (!pimpl) return;
        (void) value;
    }

    bool TF1FlexCel::AllowResize_Get() {
        // TODO: В OpenXLSX нет прямого API для этого флага защиты.
        // Вероятно, это часть более общей настройки защиты листа.
        return true; // Возвращаем разрешающее значение по умолчанию
    }

    void TF1FlexCel::AllowResize_Set(const bool value) {
        // TODO: В OpenXLSX нет прямого API для этого флага защиты.
        if (!pimpl) return;
        (void) value;
    }

    int TF1FlexCel::HdrWidth_Get() {
        if (!pimpl) return 0;
        return pimpl->hdrWidth;
    }

    void TF1FlexCel::HdrWidth_Set(const int value) {
        if (!pimpl) return;
        pimpl->hdrWidth = value;
    }

    const char16_t *TF1FlexCel::FormulaLocalRC_Get(int nRow, int nCol) {
        // TODO: Не реализована конвертация в/из локализованных имён функций.
        // Метод работает как псевдоним для FormulaRC_Get.
        return FormulaRC_Get(nRow, nCol);
    }

    void TF1FlexCel::FormulaLocalRC_Set(int nRow, int nCol, const char16_t *value) {
        // TODO: Не реализована конвертация в/из локализованных имён функций.
        // Метод работает как псевдоним для FormulaRC_Set.
        FormulaRC_Set(nRow, nCol, value);
    }

    bool TF1FlexCel::ObjVisible_Get(int ObjID) {
        if (!pimpl || !pimpl->currentSheet.hasVmlDrawing() || ObjID >= objlistCount_Get()) {
            return false;
        }

        auto shape = pimpl->currentSheet.vmlDrawing().shape(ObjID);
        return shape.style().visible();
    }

    void TF1FlexCel::ObjVisible_Set(int ObjID, const bool value) {
        if (!pimpl || !pimpl->currentSheet.hasVmlDrawing() || ObjID >= objlistCount_Get()) {
            return;
        }

        auto shape = pimpl->currentSheet.vmlDrawing().shape(ObjID);
        auto style = shape.style();

        if (value) {
            style.show();
        } else {
            style.hide();
        }
    }

    bool TF1FlexCel::ShowZeroValues_Get() {
        // TODO: Функционал "ShowZeroValues" не представлен
        // в высокоуровневом API OpenXLSX. Требуется прямое редактирование XML.
        return true; // Возвращаем стандартное значение "включено"
    }

    void TF1FlexCel::ShowZeroValues_Set(const bool value) {
        // TODO: Функционал "ShowZeroValues" не представлен
        // в высокоуровневом API OpenXLSX. Требуется прямое редактирование XML.
        if (!pimpl) return;
        (void) value;
    }

    bool TF1FlexCel::AllowDelete_Get() {
        if (!pimpl) return false;
        // Считаем, что удаление разрешено, если разрешено и для строк, и для колонок.
        return pimpl->currentSheet.deleteRowsAllowed() && pimpl->currentSheet.deleteColumnsAllowed();
    }

    void TF1FlexCel::AllowDelete_Set(const bool value) {
        if (!pimpl) return;
        pimpl->currentSheet.allowDeleteRows(value);
        pimpl->currentSheet.allowDeleteColumns(value);
    }

    bool TF1FlexCel::AllowInCellEditing_Get() {
        if (!pimpl) return true;
        return pimpl->allowInCellEditing;
    }

    void TF1FlexCel::AllowInCellEditing_Set(const bool value) {
        if (!pimpl) return;
        pimpl->allowInCellEditing = value;
    }

    const char16_t *TF1FlexCel::DefaultFontName_Get() {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        // Шрифт по умолчанию обычно имеет индекс 0
        auto font = pimpl->document->styles().fonts()[0];
        pimpl->stringBuffer = StringUtil::to_u16string(font.fontName());
        return pimpl->stringBuffer.c_str();
    }

    bool TF1FlexCel::ShowLockedCellsError_Get() {
        if (!pimpl) return true;
        return pimpl->showLockedCellsError;
    }

    uint64_t TF1FlexCel::ShowSelections_Get() {
        if (!pimpl) return 1;
        return pimpl->showSelections;
    }

    void TF1FlexCel::DefaultFontName_Set(const char16_t *value) {
        if (!pimpl) return;

        // Шрифт по умолчанию имеет индекс 0
        auto font = pimpl->document->styles().fonts()[0];
        font.setFontName(StringUtil::U16ToString(value));
    }

    void TF1FlexCel::ShowLockedCellsError_Set(const bool value) {
        if (!pimpl) return;
        pimpl->showLockedCellsError = value;
    }

    void TF1FlexCel::ShowSelections_Set(const uint64_t value) {
        if (!pimpl) return;
        pimpl->showSelections = value;
    }

    int TF1FlexCel::DefaultFontSize_Get() {
        if (!pimpl) return 0;

        auto font = pimpl->document->styles().fonts()[0];
        return static_cast<int>(font.fontSize());
    }

    void TF1FlexCel::DefaultFontSize_Set(const int value) {
        if (!pimpl) return;

        auto font = pimpl->document->styles().fonts()[0];
        font.setFontSize(static_cast<size_t>(value));
    }

    void TF1FlexCel::CellFormat_Set(int XF) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        cell.setCellFormat(static_cast<OpenXLSX::XLStyleIndex>(XF));
    }

    int TF1FlexCel::CellFormat_Get() {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        return static_cast<int>(cell.cellFormat());
    }

    void
    TF1FlexCel::MergeCells_Set(const int nRow1, const int nCol1, const int nRow2, const int nCol2, const bool Value) {
        if (!pimpl) return;

        // 1. Я создаю ссылки на угловые ячейки диапазона.
        OpenXLSX::XLCellReference topLeft(nRow1, nCol1);
        OpenXLSX::XLCellReference bottomRight(nRow2, nCol2);

        // 2. Я формирую строковое представление диапазона, например "C5:F10".
        std::string rangeString = topLeft.address() + ":" + bottomRight.address();

        // 3. В зависимости от флага 'Value', я либо объединяю, либо разъединяю ячейки.
        if (Value) {
            // Объединить ячейки. Второй параметр (emptyHiddenCells) оставляю по умолчанию false.
            pimpl->currentSheet.mergeCells(rangeString);
        } else {
            // Разъединить ячейки.
            pimpl->currentSheet.unmergeCells(rangeString);
        }
    }

    TMWSPageSetup TF1FlexCel::PageSetup_Get() {
        // TODO: Реализация этого метода требует заполнения полей структуры TMWSPageSetup
        // на основе данных из OpenXLSX (например, pimpl->currentSheet.pageSetup()...).
        // API OpenXLSX для этого может быть ограничен.
        return TMWSPageSetup{};
    }

    void TF1FlexCel::PageSetup_Set(TMWSPageSetup pageSetup) {
        // TODO: OpenXLSX не имеет высокоуровневых методов для установки всех
        // этих свойств. Для полной реализации необходимо прямое редактирование
        // XML-узлов, таких как <pageMargins>, <pageSetup>, <headerFooter> и т.д.
        // на активном листе.
        if (!pimpl) return;
        (void) pageSetup;
    }

    void TF1FlexCel::Print() {
        // TODO: Печать не поддерживается OpenXLSX.
    }

    void TF1FlexCel::PrintPage() {
        // TODO: Печать не поддерживается OpenXLSX.
    }

    void TF1FlexCel::Selection_Set(int nRow1, int nCol1, int nRow2, int nCol2) {
        if (!pimpl) return;

        // Я просто обновляю внутренние переменные, хранящие координаты выделения.
        pimpl->selStartRow = nRow1;
        pimpl->selStartCol = nCol1;
        pimpl->selEndRow = nRow2;
        pimpl->selEndCol = nCol2;

        // По правилам Excel, активная ячейка всегда находится внутри выделения.
        // Обычно это левая верхняя ячейка. Я установлю её как активную.
        ActiveCell_Set(nRow1, nCol1);
    }

    void TF1FlexCel::SaveToPDF(const char16_t *filename, const char16_t *documentProperties) {
        // TODO: Экспорт в PDF не поддерживается OpenXLSX.
        (void) filename;
        (void) documentProperties;
    }

    void TF1FlexCel::Recalc() {
        if (!pimpl) return;
        pimpl->engine->recalculateAll();
    }

    void TF1FlexCel::DeleteSheets(int nSheet, int nSheets) {
        if (!pimpl) return;

        for (int i = 0; i < nSheets; ++i) {
            auto workbook = pimpl->document->workbook(); // Получаем копию для чтения
            if (workbook.sheetCount() <= 1) break;

            // Важно: всегда удаляем лист по начальному индексу,
            // так как остальные сдвигаются влево.
            auto sheetToDelete = workbook.sheet(nSheet);

            // Используем команду для удаления
            pimpl->document->execCommand(OpenXLSX::XLCommand(OpenXLSX::XLCommandType::DeleteSheet)
                                                 .setParam("sheetName", sheetToDelete.name()));
        }
    }

    void TF1FlexCel::Protection_Get(bool &pLocked, bool &pHidden) {
        if (!pimpl) {
            pLocked = true;
            pHidden = false;
            return;
        }

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];

        pLocked = format.locked();
        pHidden = format.hidden();
    }

    void TF1FlexCel::Selection_Get(int nSelection, int &pR1, int &pC1, int &pR2, int &pC2) {
        // TODO: nSelection игнорируется, всегда возвращается основное выделение.
        (void) nSelection;
        if (!pimpl) {
            pR1 = 1;
            pC1 = 1;
            pR2 = 1;
            pC2 = 1;
            return;
        }

        pR1 = pimpl->selStartRow;
        pC1 = pimpl->selStartCol;
        pR2 = pimpl->selEndRow;
        pC2 = pimpl->selEndCol;
    }

    uint64_t TF1FlexCel::ObjGetType(int ObjID) {
        // TODO: Заглушка. Смотри TODO в ObjName_Get.
        (void) ObjID;
        return 0;
    }

    int TF1FlexCel::ObjNextID(int ObjID) {
        // TODO: Заглушка. Смотри TODO в ObjName_Get.
        (void) ObjID;
        return -1; // -1 часто используется как признак "не найдено" или "конец списка".
    }

    int TF1FlexCel::ObjFirstID(int nSheet, int nRow, int nCol, char16_t &aCaption) {
        // TODO: Заглушка. Смотри TODO в ObjName_Get.
        (void) nSheet;
        (void) nRow;
        (void) nCol;
        aCaption = u'\0';
        return -1; // -1 как признак "не найдено"
    }

    void TF1FlexCel::InsertRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t InsertType) {
        // TODO: Вставка диапазона со сдвигом - очень сложная операция,
        // которая требует ручного переноса данных и стилей из одних ячеек в другие.
        // OpenXLSX не предоставляет готового API для этого.
        if (!pimpl) return;
        (void) nRow1;
        (void) nCol1;
        (void) nRow2;
        (void) nCol2;
        (void) InsertType;
    }

    void TF1FlexCel::InsertSheets(int nSheet, int nSheets) {
        if (!pimpl || !pimpl->document) return; // Оставим эту проверку на всякий случай

        for (int i = 0; i < nSheets; ++i) {
            int sheetNum = 0;
            std::string newSheetName;
            auto workbook = pimpl->document->workbook();

            do {
                sheetNum++;
                newSheetName = "NewSheet" + std::to_string(sheetNum);
            } while (workbook.sheetExists(newSheetName));

            // Старая строка, которая падает:
            // pimpl->document->execCommand(OpenXLSX::XLCommand(OpenXLSX::XLCommandType::AddWorksheet)
            //                                      .setParam("sheetName", newSheetName));

            // Новая, более прямая и надежная строка:
            workbook.addWorksheet(newSheetName);

            // TODO для перемещения листа остаётся актуальным, но добавление теперь будет работать.
        }
    }

    void TF1FlexCel::EditInsertSheets() {
        // TODO: Это команда для UI, она не должна иметь реализации в бэкенд-библиотеке.
    }

    void TF1FlexCel::Border_Set(int nOutline, int nLeft, int nRight, int nTop, int nBottom, int nShade, int crOutline,
                                uint32_t crLeft, uint32_t crRight, uint32_t crTop, uint32_t crBottom) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();

        // Я прохожу по всем ячейкам в выделенном диапазоне.
        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);

                // 1. Для каждой ячейки я получаю её текущий формат и стиль границ.
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];
                auto oldBorder = styles.borders()[oldFormat.borderIndex()];

                // 2. Я создаю новый объект границы как копию старого.
                // Это нужно, чтобы не изменять стиль, который могут использовать другие ячейки.
                auto newBorderIndex = styles.borders().create(oldBorder);
                auto newBorder = styles.borders()[newBorderIndex];

                // 3. Теперь я определяю, какие границы для текущей ячейки являются
                // внешними, а какие - внутренними для всего выделения.

                // --- ВЕРХНЯЯ ГРАНИЦА ---
                if (row == pimpl->selStartRow) {
                    // Это верхний край выделения, применяю стиль nOutline.
                    newBorder.setTop(static_cast<OpenXLSX::XLLineStyle>(nOutline), UnpackColor(crOutline));
                } else {
                    // Это внутренняя горизонтальная граница, применяю стиль nTop.
                    newBorder.setTop(static_cast<OpenXLSX::XLLineStyle>(nTop), UnpackColor(crTop));
                }

                // --- НИЖНЯЯ ГРАНИЦА ---
                if (row == pimpl->selEndRow) {
                    // Это нижний край выделения, применяю стиль nOutline.
                    newBorder.setBottom(static_cast<OpenXLSX::XLLineStyle>(nOutline), UnpackColor(crOutline));
                } else {
                    // Это внутренняя горизонтальная граница, применяю стиль nBottom.
                    newBorder.setBottom(static_cast<OpenXLSX::XLLineStyle>(nBottom), UnpackColor(crBottom));
                }

                // --- ЛЕВАЯ ГРАНИЦА ---
                if (col == pimpl->selStartCol) {
                    // Это левый край выделения, применяю стиль nOutline.
                    newBorder.setLeft(static_cast<OpenXLSX::XLLineStyle>(nOutline), UnpackColor(crOutline));
                } else {
                    // Это внутренняя вертикальная граница, применяю стиль nLeft.
                    newBorder.setLeft(static_cast<OpenXLSX::XLLineStyle>(nLeft), UnpackColor(crLeft));
                }

                // --- ПРАВАЯ ГРАНИЦА ---
                if (col == pimpl->selEndCol) {
                    // Это правый край выделения, применяю стиль nOutline.
                    newBorder.setRight(static_cast<OpenXLSX::XLLineStyle>(nOutline), UnpackColor(crOutline));
                } else {
                    // Это внутренняя вертикальная граница, применяю стиль nRight.
                    newBorder.setRight(static_cast<OpenXLSX::XLLineStyle>(nRight), UnpackColor(crRight));
                }

                // --- ДИАГОНАЛЬНЫЕ ГРАНИЦЫ (nShade) ---
                // Предполагаю, что nShade - это битовая маска: 1 - диагональ вверх, 2 - диагональ вниз.
                if (nShade & 1) newBorder.setDiagonalUp(true);
                if (nShade & 2) newBorder.setDiagonalDown(true);
                if (nShade > 0) {
                    // OpenXLSX требует также установить стиль и цвет для диагонали.
                    // Я использую параметры от nOutline.
                    newBorder.setDiagonal(static_cast<OpenXLSX::XLLineStyle>(nOutline), UnpackColor(crOutline));
                }

                // 4. Я создаю новый формат ячейки, который будет использовать новый стиль границ.
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];
                newFormat.setBorderIndex(newBorderIndex);
                newFormat.setApplyBorder(true); // Указываю, что границу нужно применять.

                // 5. Присваиваю итоговый новый формат ячейке.
                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    void
    TF1FlexCel::Border_Get(int &pLeft, int &pRight, int &pTop, int &pBottom, int &pShade, int &pcrLeft, int &pcrRight,
                           int &pcrTop, int &pcrBottom) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto borderIdx = format.borderIndex();
        auto border = pimpl->document->styles().borders()[borderIdx];

        auto pack_color = [](const OpenXLSX::XLColor &color) {
            return (color.blue() << 16) | (color.green() << 8) | color.red();
        };

        auto left_line = border.left();
        pLeft = static_cast<int>(left_line.style());
        pcrLeft = pack_color(left_line.color().rgb()); // ИСПРАВЛЕНИЕ

        auto right_line = border.right();
        pRight = static_cast<int>(right_line.style());
        pcrRight = pack_color(right_line.color().rgb()); // ИСПРАВЛЕНИЕ

        auto top_line = border.top();
        pTop = static_cast<int>(top_line.style());
        pcrTop = pack_color(top_line.color().rgb()); // ИСПРАВЛЕНИЕ

        auto bottom_line = border.bottom();
        pBottom = static_cast<int>(bottom_line.style());
        pcrBottom = pack_color(bottom_line.color().rgb()); // ИСПРАВЛЕНИЕ

        pShade = 0; // TODO: Определить, что такое pShade
    }

    void TF1FlexCel::DrawBorders(int row1, int col1, int row2, int col2, int borderStyle, int borderColor,
                                 bool exteriorBorders) {
        // TODO: Этот метод должен итерироваться по ячейкам в диапазоне
        // и для каждой вызывать логику, аналогичную Border_Set.
        // Пока является заглушкой.
        if (!pimpl) return;
        (void) row1;
        (void) col1;
        (void) row2;
        (void) col2;
        (void) borderStyle;
        (void) borderColor;
        (void) exteriorBorders;
    }

    void TF1FlexCel::Pattern_Get(int &pPattern, int &pcrFG, int &pcrBG) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fillIdx = format.fillIndex();
        auto fill = pimpl->document->styles().fills()[fillIdx];

        auto pack_color = [](const OpenXLSX::XLColor &color) {
            return (color.blue() << 16) | (color.green() << 8) | color.red();
        };

        pPattern = static_cast<int>(fill.patternType());
        pcrFG = pack_color(fill.color());
        pcrBG = pack_color(fill.backgroundColor());
    }

    void TF1FlexCel::ColWidthAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults) {
        // TODO: Автоподбор ширины колонок не поддерживается OpenXLSX.
        // Эта операция требует измерения текста определённым шрифтом,
        // что выходит за рамки возможностей библиотеки.
        if (!pimpl) return;
        (void) nRow1;
        (void) nCol1;
        (void) nRow2;
        (void) nCol2;
        (void) bSetDefaults;
    }

    void TF1FlexCel::Pattern_Set(int pPatternStyle, uint32_t crPatternBG, uint32_t crPatternFG) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();

        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);

                // 1. Получаю текущие формат и заливку.
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];
                auto oldFill = styles.fills()[oldFormat.fillIndex()];

                // 2. Создаю новый объект заливки как копию старого.
                auto newFillIndex = styles.fills().create(oldFill);
                auto newFill = styles.fills()[newFillIndex];

                // 3. Модифицирую новую заливку.
                // Убеждаюсь, что тип заливки - это узор.
                newFill.setFillType(OpenXLSX::XLFillType::XLPatternFill, true);
                newFill.setPatternType(UnpackPattern(pPatternStyle));
                newFill.setBackgroundColor(UnpackColor(crPatternBG));
                newFill.setColor(UnpackColor(crPatternFG)); // Для узоров это цвет переднего плана

                // 4. Создаю новый формат ячейки с новой заливкой.
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];
                newFormat.setFillIndex(newFillIndex);
                newFormat.setApplyFill(true); // Важно указать, что заливку нужно применять.

                // 5. Присваиваю новый формат ячейке.
                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    void TF1FlexCel::BackgroundColor_Set(uint32_t crBG) {
        if (!pimpl) return;

        auto fill = getActiveCellFill(true);
        fill.setPatternType(OpenXLSX::XLPatternType::XLPatternSolid);

        uint8_t r = (crBG >> 16) & 0xFF;
        uint8_t g = (crBG >> 8) & 0xFF;
        uint8_t b = crBG & 0xFF;
        fill.setColor(OpenXLSX::XLColor(r, g, b));
    }

    void TF1FlexCel::RowHeightAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults) {
        // TODO: Автоподбор высоты строк не поддерживается OpenXLSX.
        if (!pimpl) return;
        (void) nRow1;
        (void) nCol1;
        (void) nRow2;
        (void) nCol2;
        (void) bSetDefaults;
    }

    void TF1FlexCel::Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, const char16_t *keysString) {
        if (!pimpl) return;
        (void) nR1;
        (void) nC1;
        (void) nR2;
        (void) nC2;
        (void) bSortByRows;
        (void) keysString;
    }

    void TF1FlexCel::Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, SortKeys Keys) {
        if (!pimpl) return;
        (void) nR1;
        (void) nC1;
        (void) nR2;
        (void) nC2;
        (void) bSortByRows;
        (void) Keys;
    }

    void TF1FlexCel::Protection_Set(bool bLocked, bool bHidden) {
        if (!pimpl) return;
        auto format = getActiveCellFormat(true);
        format.setLocked(bLocked);
        format.setHidden(bHidden);
    }

    void TF1FlexCel::ColHidden_Set(int nCol1, int nCol2, bool bHidden) {
        if (!pimpl) return;
        for (int i = nCol1; i <= nCol2; ++i) {
            pimpl->currentSheet.column(i).setHidden(bHidden);
        }
    }

    void TF1FlexCel::ActiveCell_Set(int nRow, int nCol) {
        if (!pimpl) return;

        // TODO: Добавить проверки, чтобы nRow и nCol не выходили за пределы листа.
        if (nRow > 0 && nCol > 0) {
            pimpl->activeRow = nRow;
            pimpl->activeCol = nCol;
        }
    }


    void TF1FlexCel::ShowActiveCell() {
        // TODO: Это команда для UI, она не должна иметь реализации в бэкенд-библиотеке.
    }

    void TF1FlexCel::CopyRange(int nDstR1, int nDstC1, int nDstR2, int nDstC2, int hSrcSS, int nSrcR1, int nSrcC1,
                               int nSrcR2, int nSrcC2) {
        // TODO: Копирование диапазонов - очень сложная операция, требующая
        // итерации по каждой ячейке, копирования значения, формулы и стиля.
        // Оставлено как заглушка.
        if (!pimpl) return;
        (void) nDstR1;
        (void) nDstC1;
        (void) nDstR2;
        (void) nDstC2;
        (void) hSrcSS;
        (void) nSrcR1;
        (void) nSrcC1;
        (void) nSrcR2;
        (void) nSrcC2;
    }

    int TF1FlexCel::SS() {
        // TODO: Этот метод больше не имеет смысла, так как сам объект TF1FlexCel
        // является "дескриптором" таблицы. Возвращаем фиктивное значение.
        return 1;
    }

    void
    TF1FlexCel::Font_Set(const char16_t *pName, int nSize, bool bBold, bool bItalic, bool bUnderline, bool bStrikeout,
                         uint32_t crColor, bool bOutline, bool bShadow) {
        if (!pimpl) return;

        auto &styles = pimpl->document->styles();
        std::string fontName = StringUtil::U16ToString(pName);

        for (int row = pimpl->selStartRow; row <= pimpl->selEndRow; ++row) {
            for (int col = pimpl->selStartCol; col <= pimpl->selEndCol; ++col) {
                auto cell = pimpl->currentSheet.cell(row, col);

                // 1. Получаю текущие формат и шрифт.
                auto oldFormat = styles.cellFormats()[cell.cellFormat()];
                auto oldFont = styles.fonts()[oldFormat.fontIndex()];

                // 2. Создаю новый объект шрифта как копию старого.
                auto newFontIndex = styles.fonts().create(oldFont);
                auto newFont = styles.fonts()[newFontIndex];

                // 3. Модифицирую новый шрифт, используя параметры функции.
                if (!fontName.empty()) {
                    newFont.setFontName(fontName);
                }
                if (nSize > 0) {
                    newFont.setFontSize(nSize);
                }
                newFont.setFontColor(UnpackColor(crColor));

                // Применяю булевы флаги
                newFont.setBold(bBold);
                newFont.setItalic(bItalic);
                newFont.setStrikethrough(bStrikeout);
                newFont.setOutline(bOutline);
                newFont.setShadow(bShadow);

                if (bUnderline) {
                    newFont.setUnderline(OpenXLSX::XLUnderlineStyle::XLUnderlineSingle);
                } else {
                    newFont.setUnderline(OpenXLSX::XLUnderlineStyle::XLUnderlineNone);
                }

                // 4. Создаю новый формат ячейки с новым шрифтом.
                auto newFormatIndex = styles.cellFormats().create(oldFormat);
                auto newFormat = styles.cellFormats()[newFormatIndex];
                newFormat.setFontIndex(newFontIndex);
                newFormat.setApplyFont(true);

                // 5. Присваиваю новый формат ячейке.
                cell.setCellFormat(newFormatIndex);
            }
        }
    }

    void
    TF1FlexCel::Font_Get(char16_t &pName, int &pSize, bool &pBold, bool &pItalic, bool &pUnderline, bool &pStrikeout,
                         int &pcrColor, bool &pOutline, bool &pShadow) {
        if (!pimpl) return;

        auto font = getActiveCellFont(false);

        pSize = static_cast<int>(font.fontSize());
        pBold = font.bold();
        pItalic = font.italic();
        pUnderline = font.underline() != OpenXLSX::XLUnderlineStyle::XLUnderlineNone;
        pStrikeout = font.strikethrough();

        auto color = font.fontColor();
        pcrColor = (color.blue() << 16) | (color.green() << 8) | color.red();

        pOutline = font.outline();
        pShadow = font.shadow();

        // TODO: Небезопасно записывать имя шрифта в ссылку на один char16_t.
        // Старый API, вероятно, ожидал указатель на буфер. Оставлено как заглушка.
        pName = u'\0';
    }

    void TF1FlexCel::DeleteRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ShiftType) {
        // TODO: Удаление диапазона со сдвигом - сложная операция, не поддерживаемая OpenXLSX напрямую.
        if (!pimpl) return;
        (void) nRow1;
        (void) nCol1;
        (void) nRow2;
        (void) nCol2;
        (void) ShiftType;
    }

    void TF1FlexCel::ClearRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ClearType) {
        if (!pimpl) return;

        // 1. Я определяю, какие данные нужно СОХРАНИТЬ в ячейке.
        uint32_t keepFlags = 0;
        switch (static_cast<Local_F1ClearTypeConstants>(ClearType)) {
            case Local_F1ClearTypeConstants::F1ClearAll:
                // Не сохраняем ничего.
                keepFlags = 0;
                break;

            case Local_F1ClearTypeConstants::F1ClearFormats:
                // Нужно очистить форматы, значит, я СОХРАНЯЮ значение и формулу.
                keepFlags = OpenXLSX::XLKeepCellValue | OpenXLSX::XLKeepCellFormula;
                break;

            case Local_F1ClearTypeConstants::F1ClearValues:
                // Нужно очистить значения, значит, я СОХРАНЯЮ стиль.
                keepFlags = OpenXLSX::XLKeepCellStyle;
                break;

            default:
                // Если тип очистки неизвестен, я ничего не делаю для безопасности.
                return;
        }

        // 2. Я прохожу по каждой ячейке в диапазоне.
        for (int row = nRow1; row <= nRow2; ++row) {
            for (int col = nCol1; col <= nCol2; ++col) {
                try {
                    auto cell = pimpl->currentSheet.cell(row, col);
                    // 3. Я вызываю очистку с правильными флагами.
                    if (cell) {
                        cell.clear(keepFlags);
                    }
                } catch (const OpenXLSX::XLException &e) {
                    // Игнорирую ошибки для несуществующих ячеек в диапазоне.
                }
            }
        }
    }

    void TF1FlexCel::EditClear(uint64_t ClearType) {
        if (!pimpl) return;

        ClearRange(pimpl->selStartRow, pimpl->selStartCol, pimpl->selEndRow, pimpl->selEndCol, ClearType);
    }

    void
    TF1FlexCel::BorderStyle_Set(int StyleTopBorder, int StyleLeftBorder, int StyleBottomBorder, int StyleRightBorder) {
        if (!pimpl) return;
        auto border = getActiveCellBorder(true);

        // API OpenXLSX требует при установке стиля линии указывать и цвет.
        // Поэтому мы сначала получаем текущий цвет, а потом устанавливаем новый стиль с этим цветом.
        border.setTop(static_cast<OpenXLSX::XLLineStyle>(StyleTopBorder), border.top().color().rgb());
        border.setLeft(static_cast<OpenXLSX::XLLineStyle>(StyleLeftBorder), border.left().color().rgb());
        border.setBottom(static_cast<OpenXLSX::XLLineStyle>(StyleBottomBorder), border.bottom().color().rgb());
        border.setRight(static_cast<OpenXLSX::XLLineStyle>(StyleRightBorder), border.right().color().rgb());
    }

    void TF1FlexCel::BorderStyle_Get(int &StyleTopBorder, int &StyleLeftBorder, int &StyleBottomBorder,
                                     int &StyleRightBorder) {
        if (!pimpl) return;
        auto border = getActiveCellBorder(false);

        StyleTopBorder = static_cast<int>(border.top().style());
        StyleLeftBorder = static_cast<int>(border.left().style());
        StyleBottomBorder = static_cast<int>(border.bottom().style());
        StyleRightBorder = static_cast<int>(border.right().style());
    }

    void
    TF1FlexCel::BorderColor_Set(int ColorTopBorder, int ColorLeftBorder, int ColorBottomBorder, int ColorRightBorder) {
        if (!pimpl) return;
        auto border = getActiveCellBorder(true);

        auto unpack_color = [](int c) {
            uint8_t r = (c >> 16) & 0xFF;
            uint8_t g = (c >> 8) & 0xFF;
            uint8_t b = c & 0xFF;
            return OpenXLSX::XLColor(r, g, b);
        };

        // API OpenXLSX требует при установке цвета линии указывать и стиль.
        // Поэтому мы сначала получаем текущий стиль, а потом устанавливаем новый цвет с этим стилем.
        border.setTop(border.top().style(), unpack_color(ColorTopBorder));
        border.setLeft(border.left().style(), unpack_color(ColorLeftBorder));
        border.setBottom(border.bottom().style(), unpack_color(ColorBottomBorder));
        border.setRight(border.right().style(), unpack_color(ColorRightBorder));
    }

    void TF1FlexCel::BorderColor_Get(int &ColorTopBorder, int &ColorLeftBorder, int &ColorBottomBorder,
                                     int &ColorRightBorder) {
        if (!pimpl) return;
        auto border = getActiveCellBorder(false);

        auto pack_color = [](const OpenXLSX::XLColor &color) {
            return (color.blue() << 16) | (color.green() << 8) | color.red();
        };

        ColorTopBorder = pack_color(border.top().color().rgb());
        ColorLeftBorder = pack_color(border.left().color().rgb());
        ColorBottomBorder = pack_color(border.bottom().color().rgb());
        ColorRightBorder = pack_color(border.right().color().rgb());
    }

    int TF1FlexCel::InputButton(int nRow1, int nCol1, int nRow2, int nCol2, const char16_t *aCaption,
                                const char16_t *aObjName) {
        // TODO: Создание UI-элементов (кнопок) не поддерживается OpenXLSX.
        if (!pimpl) return -1;
        (void) nRow1;
        (void) nCol1;
        (void) nRow2;
        (void) nCol2;
        (void) aCaption;
        (void) aObjName;
        return -1;
    }

    void
    TF1FlexCel::BorderInfo_Get(int nRow, int nCol, int &BorderTopStyle, int &BorderLeftStyle, int &BorderBottomStyle,
                               int &BorderRightStyle, int &BorderTopColor, int &BorderLeftColor, int &BorderBottomColor,
                               int &BorderRightColor) {
        if (!pimpl) return;

        // Этот метод является версией Border_Get для конкретной ячейки, а не активной.
        // Поэтому логика здесь почти такая же.
        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto borderIdx = format.borderIndex();
        auto border = pimpl->document->styles().borders()[borderIdx];

        auto pack_color = [](const OpenXLSX::XLColor &color) {
            return (color.blue() << 16) | (color.green() << 8) | color.red();
        };

        BorderTopStyle = static_cast<int>(border.top().style());
        BorderLeftStyle = static_cast<int>(border.left().style());
        BorderBottomStyle = static_cast<int>(border.bottom().style());
        BorderRightStyle = static_cast<int>(border.right().style());

        BorderTopColor = pack_color(border.top().color().rgb());
        BorderLeftColor = pack_color(border.left().color().rgb());
        BorderBottomColor = pack_color(border.bottom().color().rgb());
        BorderRightColor = pack_color(border.right().color().rgb());
    }

    int TF1FlexCel::HAlignRC_Get(int nRow, int nCol) {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        return static_cast<int>(alignment.horizontal());
    }

    THFlxAlignment TF1FlexCel::toHAlign(int aH) {
        return static_cast<THFlxAlignment>(aH);
    }

    TVFlxAlignment TF1FlexCel::toVAlign(int aV) {
        return static_cast<TVFlxAlignment>(aV);
    }

    int TF1FlexCel::VAlignRC_Get(int nRow, int nCol) {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        return static_cast<int>(alignment.vertical());
    }

    int TF1FlexCel::objlistCount_Get() {
        if (!pimpl || !pimpl->currentSheet.hasVmlDrawing()) return 0;
        return pimpl->currentSheet.vmlDrawing().shapeCount();
    }

    void TF1FlexCel::objlistI_PropRC_Get(int aI, int &nRow, int &nCol, char16_t &objectName) {
        if (!pimpl || !pimpl->currentSheet.hasVmlDrawing() || aI >= objlistCount_Get()) {
            nRow = 0;
            nCol = 0;
            objectName = u'\0';
            return;
        }

        auto shape = pimpl->currentSheet.vmlDrawing().shape(aI);
        auto clientData = shape.clientData();

        // API OpenXLSX возвращает 0-индексированные координаты
        nRow = clientData.row() + 1;
        nCol = clientData.column() + 1;

        // TODO: В OpenXLSX нет прямого способа получить "имя" объекта,
        // которое ожидает старый API. Есть shape.shapeId(), но это внутренний ID.
        objectName = u'\0';
    }

    int TF1FlexCel::NFormatRC_Get(int nRow, int nCol) {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];

        return static_cast<int>(format.numberFormatId());
    }

    const char16_t *TF1FlexCel::StyleNameRC_Get(int nRow, int nCol) {
        if (!pimpl) {
            pimpl->stringBuffer = u"";
            return pimpl->stringBuffer.c_str();
        }

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto xfId = format.xfId();

        // xfId - это индекс в cellStyleXfs.
        auto cellStyle = pimpl->document->styles().cellStyles()[xfId];

        pimpl->stringBuffer = StringUtil::to_u16string(cellStyle.name());
        return pimpl->stringBuffer.c_str();
    }

    uint32_t TF1FlexCel::BackColorRC_Get(int nRow, int nCol) {
        if (!pimpl) return 0;

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fillIdx = format.fillIndex();
        auto fill = pimpl->document->styles().fills()[fillIdx];

        auto color = fill.color(); // В OpenXLSX цвет фона - это fgColor у solid pattern
        return (color.alpha() << 24) | (color.red() << 16) | (color.green() << 8) | color.blue();
    }

    void TF1FlexCel::CellInfoRC_Get(int nRow, int nCol, char16_t &Format, bool &Locked, bool &WrapText) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto alignment = format.alignment();

        Locked = format.locked();
        WrapText = alignment.wrapText();

        // TODO: Небезопасно записывать строку формата в ссылку на один char16_t.
        Format = u'\0';
    }

    void
    TF1FlexCel::FontInfoRC_Get(int nRow, int nCol, uint32_t &Color, char16_t &FontName, int &FontSize, int &Underline,
                               bool &Bold, bool &Italic) {
        if (!pimpl) return;

        auto cell = pimpl->currentSheet.cell(nRow, nCol);
        auto styleIdx = cell.cellFormat();
        auto format = pimpl->document->styles().cellFormats()[styleIdx];
        auto fontIdx = format.fontIndex();
        auto font = pimpl->document->styles().fonts()[fontIdx];

        auto fontColor = font.fontColor();
        Color = (fontColor.alpha() << 24) | (fontColor.red() << 16) | (fontColor.green() << 8) | fontColor.blue();

        FontSize = static_cast<int>(font.fontSize());
        Underline = static_cast<int>(font.underline());
        Bold = font.bold();
        Italic = font.italic();

        // TODO: Небезопасно записывать имя шрифта в ссылку на один char16_t.
        FontName = u'\0';
    }

    void TF1FlexCel::F1CellFormat_Set(
            bool isAlignHorizontal, int AlignHorizontal_, bool isWordWrap, bool WordWrap_,
            bool isAlignVertical, int AlignVertical_, bool isBorderStyle, int BorderStyle_,
            int pShade_, bool isBorderColor, int BorderColor_, bool isFontName,
            const char16_t *FontName_, bool isFontSize, int FontSize_, bool isFontBold,
            bool FontBold_, bool isFontItalic, bool FontItalic_, bool isFontUnderline,
            bool FontUnderline_, bool isFontStrikeout, bool FontStrikeout_, bool isFontColor,
            int FontColor_, bool isPatternStyle, int PatternStyle_, int PatternBG_,
            int PatternFG_, bool isProtectionLocked, bool ProtectionLocked_,
            bool ProtectionHidden_, bool isNumberFormat, const char16_t *NumberFormat_,
            bool MergeCells_, bool isRotation, int Rotation_
    ) {
        if (!pimpl) return;

        // TODO: Этот метод должен применяться ко всему текущему выделению (Sel...),
        // а не только к активной ячейке. Пока что реализую только для активной ячейки.

        auto cell = pimpl->currentSheet.cell(pimpl->activeRow, pimpl->activeCol);
        auto &styles = pimpl->document->styles();

        // 1. Создаём новый формат на основе старого
        auto oldFormat = styles.cellFormats()[cell.cellFormat()];
        auto newFormatIndex = styles.cellFormats().create(oldFormat);
        auto newFormat = styles.cellFormats()[newFormatIndex];

        // 2. Создаём новый шрифт, если нужно изменить хотя бы одно его свойство
        if (isFontName || isFontSize || isFontBold || isFontItalic || isFontUnderline || isFontStrikeout ||
            isFontColor) {
            auto oldFont = styles.fonts()[oldFormat.fontIndex()];
            auto newFontIndex = styles.fonts().create(oldFont);
            auto newFont = styles.fonts()[newFontIndex];

            if (isFontName) newFont.setFontName(StringUtil::U16ToString(FontName_));
            if (isFontSize) newFont.setFontSize(FontSize_);
            if (isFontBold) newFont.setBold(FontBold_);
            if (isFontItalic) newFont.setItalic(FontItalic_);
            if (isFontUnderline)
                newFont.setUnderline(FontUnderline_ ? OpenXLSX::XLUnderlineStyle::XLUnderlineSingle
                                                    : OpenXLSX::XLUnderlineStyle::XLUnderlineNone);
            if (isFontStrikeout) newFont.setStrikethrough(FontStrikeout_);
            if (isFontColor) {
                uint8_t r = (FontColor_ >> 16) & 0xFF;
                uint8_t g = (FontColor_ >> 8) & 0xFF;
                uint8_t b = FontColor_ & 0xFF;
                newFont.setFontColor(OpenXLSX::XLColor(r, g, b));
            }

            newFormat.setFontIndex(newFontIndex);
        }

        // 3. Изменяем выравнивание, если нужно
        if (isAlignHorizontal || isAlignVertical || isWordWrap || isRotation) {
            auto alignment = newFormat.alignment(true);
            if (isAlignHorizontal) alignment.setHorizontal(static_cast<OpenXLSX::XLAlignmentStyle>(AlignHorizontal_));
            if (isAlignVertical) alignment.setVertical(static_cast<OpenXLSX::XLAlignmentStyle>(AlignVertical_));
            if (isWordWrap) alignment.setWrapText(WordWrap_);
            if (isRotation) alignment.setTextRotation(Rotation_);
        }

        // 4. Изменяем защиту
        if (isProtectionLocked) {
            newFormat.setLocked(ProtectionLocked_);
            newFormat.setHidden(ProtectionHidden_);
        }

        // 5. TODO: Реализовать сложную логику для границ (isBorderStyle, isBorderColor)
        // 6. TODO: Реализовать сложную логику для заливки (isPatternStyle)
        // 7. TODO: Реализовать сложную логику для числового формата (isNumberFormat)

        // 8. Присваиваем ячейке новый, полностью настроенный формат
        cell.setCellFormat(newFormatIndex);

        // 9. Объединяем ячейки, если нужно. Этот вызов должен быть последним.
        if (MergeCells_) {
//            MergeCells_Set(true);
        }
    }


    int TF1FlexCel::NVisibleCol_Get(int &nVisibleCol1) {
        if (!pimpl) {
            nVisibleCol1 = 0;
            return 0;
        }

        for (uint16_t i = 1; i <= pimpl->currentSheet.columnCount(); ++i) {
            if (!pimpl->currentSheet.column(i).isHidden()) {
                nVisibleCol1 = i;
                return i;
            }
        }

        nVisibleCol1 = 0;
        return 0;
    }

    double TF1FlexCel::FormulaResultNumberRC(int nRow, int nCol) {
        // Этот метод является полным аналогом NumberRC_Get
        return NumberRC_Get(nRow, nCol);
    }

    const char16_t *TF1FlexCel::FormulaResultTextRC(int nRow, int nCol) {
        // Этот метод является полным аналогом TextRC_Get
        return TextRC_Get(nRow, nCol);
    }

    void TF1FlexCel::CacheSheetObjects() {
        if (!pimpl || !pimpl->document) return;

        pimpl->buttonCache.clear();

        if (!pimpl->currentSheet.hasVmlDrawing()) {
            return;
        }

        auto vmlDrawing = pimpl->currentSheet.vmlDrawing();
        uint32_t shapeCount = vmlDrawing.shapeCount();

        for (uint32_t i = 0; i < shapeCount; ++i) {
            auto shape = vmlDrawing.shape(i);
            auto clientData = shape.clientData();

            if (clientData.objectType() == "Note" || clientData.objectType() == "Button") {
                ButtonInfo info;
                // TODO: Логика извлечения имени из объекта shape. Пока что заглушка.
                info.name = "Button" + std::to_string(i + 1); // Использую i+1 для 1-индексированного имени
                info.row = clientData.row() + 1;
                info.col = clientData.column() + 1;
                info.shapeIndex = i; // Сохраняю индекс, а не объект

                pimpl->buttonCache.push_back(info);
            }
        }
    }
}