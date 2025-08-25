#ifndef F1FLEXCEL_CPPPROJECT_TF1FLEXCEL_H
#define F1FLEXCEL_CPPPROJECT_TF1FLEXCEL_H

#include <cstdint>
#include <memory>
#include <headers/XLStyles.hpp>
#include <headers/XLDrawing.hpp>

#define CREATE_PROPERTY(TYPE, NAME) \
    using NAME##Proxy = Property<TYPE>; \
    NAME##Proxy NAME

enum class TCellValueType {
    cvtBlank,
    cvtFormula,
    cvtNumber,
    cvtString,
    cvtBoolean,
    cvtError,
    cvtEmpty
};

struct TF1Range {
    int StartRow;
    int EndRow;
    int StartCol;
    int EndCol;
};

struct TMWSPageSetup {
    // Поля этой структуры нужно будет определить на основе вашего исходного кода
};

struct SortKeys {
    const char16_t* GetKeysString() { return u""; }
};

enum class THFlxAlignment {};
enum class TVFlxAlignment {};


namespace F1DevExpress::unF1FlexCel {

    class TF1FlexCel; // Предварительное объявление

    template<typename T>
    class Property {
    public:
        Property(TF1FlexCel* flex) : m_flex(flex) {}

        // Геттер: вызывается при чтении (например, int x = xls->Row;)
        operator T() const;

        // Сеттер: вызывается при записи (например, xls->Row = 5;)
        Property& operator=(const T& value);

    private:
        TF1FlexCel* m_flex;
    };

    // Прокси для индексированного свойства bool (чтение/запись)
    class BoolPropertyProxy {
    public:
        BoolPropertyProxy(TF1FlexCel* flex, int index, bool isRow)
                : m_flex(flex), m_index(index), m_isRow(isRow) {}

        operator bool() const; // Геттер
        BoolPropertyProxy& operator=(bool value); // Сеттер
    private:
        TF1FlexCel* m_flex;
        int m_index;
        bool m_isRow;
    };

    // Прокси для индексированного свойства int (чтение/запись)
    class IntPropertyProxy {
    public:
        IntPropertyProxy(TF1FlexCel* flex, int index, bool isRow)
                : m_flex(flex), m_index(index), m_isRow(isRow) {}

        operator int() const; // Геттер
        IntPropertyProxy& operator=(int value); // Сеттер
    private:
        TF1FlexCel* m_flex;
        int m_index;
        bool m_isRow;
    };

    class TextRCProxy {
    public:
        TextRCProxy(TF1FlexCel* flex, int row, int col) : m_flex(flex), m_row(row), m_col(col) {}
        operator const char16_t*() const;
        TextRCProxy& operator=(const char16_t* value);
    private:
        TF1FlexCel* m_flex; int m_row; int m_col;
    };

    class NumberRCProxy {
    public:
        NumberRCProxy(TF1FlexCel* flex, int row, int col) : m_flex(flex), m_row(row), m_col(col) {}
        operator double() const;
        NumberRCProxy& operator=(double value);
    private:
        TF1FlexCel* m_flex; int m_row; int m_col;
    };

    class FormulaRCProxy {
    public:
        FormulaRCProxy(TF1FlexCel* flex, int row, int col) : m_flex(flex), m_row(row), m_col(col) {}
        // Сеттер: f1Forms->FormulaRC(r, c) = "..."
        FormulaRCProxy& operator=(const char16_t* value);
        // Геттер (на всякий случай, если он понадобится)
        operator const char16_t*() const;
    private:
        TF1FlexCel* m_flex; int m_row; int m_col;
    };

    class SheetNameProxy {
    public:
        // Конструктор. Параметр isRow здесь не используется, но нужен для совместимости с шаблоном IndexedProperty.
        SheetNameProxy(TF1FlexCel* flex, int index, bool /*isRow*/)
                : m_flex(flex), m_index(index) {}

        // Геттер для чтения: f1Flex->SheetName[i]
        operator const char16_t*() const;

        // Сеттер для записи: f1Flex->SheetName[i] = "..."
        SheetNameProxy& operator=(const char16_t* value);

    private:
        TF1FlexCel* m_flex;
        int m_index;
    };

    // "Владелец" индексированных свойств, перегружающий operator[]
    template<typename TProxy, bool IsRow>
    class IndexedProperty {
    public:
        IndexedProperty(TF1FlexCel* flex) : m_flex(flex) {}
        TProxy operator[](int index) {
            return TProxy(m_flex, index, IsRow);
        }
    private:
        TF1FlexCel* m_flex;
    };

    class TF1FlexCel {
    public:
        explicit TF1FlexCel();
        explicit TF1FlexCel(const char16_t *filename);
        ~TF1FlexCel();

        CREATE_PROPERTY(int, Row); CREATE_PROPERTY(int, Col); CREATE_PROPERTY(int, Sheet);
        CREATE_PROPERTY(int, MaxRow); CREATE_PROPERTY(int, MaxCol); CREATE_PROPERTY(int, LastRow);
        CREATE_PROPERTY(int, LastCol); CREATE_PROPERTY(int, NumSheets);
        CREATE_PROPERTY(bool, AutoRecalc); CREATE_PROPERTY(bool, MergeCells);
        CREATE_PROPERTY(bool, FontBold); CREATE_PROPERTY(bool, FontItalic);
        CREATE_PROPERTY(bool, FontUnderline); CREATE_PROPERTY(bool, FontStrikeout);
        CREATE_PROPERTY(int, FontSize); CREATE_PROPERTY(uint32_t, FontColor);
        CREATE_PROPERTY(const char16_t*, FontName); CREATE_PROPERTY(const char16_t*, NumberFormat);
        CREATE_PROPERTY(const char16_t*, Text); CREATE_PROPERTY(bool, WordWrap);
        CREATE_PROPERTY(uint64_t, HAlign); CREATE_PROPERTY(uint64_t, VAlign);
        CREATE_PROPERTY(bool, EnableProtection); CREATE_PROPERTY(bool, DisableShrink);
        CREATE_PROPERTY(bool, DisableAutoSize);
        CREATE_PROPERTY(int, SelStartRow); CREATE_PROPERTY(int, SelStartCol);
        CREATE_PROPERTY(int, SelEndRow); CREATE_PROPERTY(int, SelEndCol);
        CREATE_PROPERTY(uint64_t, ShowSelections); CREATE_PROPERTY(int, HdrWidth);
        CREATE_PROPERTY(bool, AllowDelete); CREATE_PROPERTY(const char16_t*, DefaultFontName);
        CREATE_PROPERTY(int, DefaultFontSize); CREATE_PROPERTY(bool, AllowInCellEditing);
        CREATE_PROPERTY(bool, ShowLockedCellsError); CREATE_PROPERTY(bool, ShowGridLines);
        CREATE_PROPERTY(bool, ShowZeroValues); CREATE_PROPERTY(double, Number);
        CREATE_PROPERTY(int, Type); CREATE_PROPERTY(const char16_t*, Formula);


        IndexedProperty<IntPropertyProxy, true> RowHeight;
        IndexedProperty<IntPropertyProxy, false> ColWidth;
        IndexedProperty<BoolPropertyProxy, true> RowHidden;
        IndexedProperty<BoolPropertyProxy, false> ColHidden;
        IndexedProperty<SheetNameProxy, false> SheetName;

        // --- ПУБЛИЧНЫЕ МЕТОДЫ (API) ---
        void NewFile(int sheets = 1);
        void Save(const char16_t *filename);

        TextRCProxy TextRC(int nRow, int nCol);
        NumberRCProxy NumberRC(int nRow, int nCol);
        int TypeRC(int nRow, int nCol);
        FormulaRCProxy FormulaRC(int nRow, int nCol);
        /**
         * @brief Проверяет, отключено ли для активной ячейки свойство 'Сжимать по размеру' (Shrink to Fit).
         * @return true, если свойство выключено.
         */
        bool DisableAutoSize_Get();

        /**
         * @brief Устанавливает свойство 'Сжимать по размеру' (Shrink to Fit) для активной ячейки.
         * @param value true, чтобы выключить свойство (т.е. включить Shrink to Fit = false).
         */
        void DisableAutoSize_Set(bool value);

        /**
         * @brief Проверяет, отключено ли для активной ячейки свойство 'Сжимать по размеру' (Shrink to Fit).
         * @return true, если свойство выключено.
         */
        bool DisableShrink_Get();
        /**
        * @brief Устанавливает свойство 'Сжимать по размеру' (Shrink to Fit) для активной ячейки.
        * @param value true, чтобы выключить свойство.
        */
        void DisableShrink_Set(bool value);

        /**
         * @brief Получает текущее количество листов в документе.
         * @return Количество листов.
         */
        int NumSheets_Get();

        /**
         * @brief Устанавливает точное количество листов, добавляя новые или удаляя лишние.
         * @param value Требуемое количество листов.
         */
        void NumSheets_Set(const int value);
        /**
         * @brief Получает индекс активного листа.
         * @return Индекс активного листа (начиная с 1).
         */
        int Sheet_Get();

        /**
         * @brief Делает лист активным по его индексу.
         * @param value Индекс листа, который нужно сделать активным (начиная с 1).
         */
        void Sheet_Set(const int value);

        /**
         * @brief Получает текстовое представление значения ячейки.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Указатель на строку с текстовым значением.
         * @warning Возвращаемый указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* TextRC_Get(int nRow, int nCol);
        /**
         * @brief Устанавливает текстовое значение в ячейку.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @param value Текст для установки.
         */
        void TextRC_Set(int nRow, int nCol, const char16_t *value);

        /**
         * @brief Получает числовое значение из ячейки.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Числовое значение. Если в ячейке не число, вернёт 0.0.
         */
        double NumberRC_Get(int nRow, int nCol);

        /**
         * @brief Устанавливает числовое значение в ячейку.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @param value Число для установки.
         */
        void NumberRC_Set(int nRow, int nCol, const double value);
        /**
         * @brief Проверяет, скрыта ли колонка.
         * @param nCol Номер колонки (начиная с 1).
         * @return true, если колонка скрыта.
         */
        bool ColHidden_Get(int nCol);

        /**
         * @brief Проверяет, скрыта ли строка.
         * @param nRow Номер строки (начиная с 1).
         * @return true, если строка скрыта.
         */
        bool RowHidden_Get(int nRow);

        /**
         * @brief Устанавливает свойство "скрытый" для колонки.
         * @param nCol Номер колонки (начиная с 1).
         * @param value true, чтобы скрыть колонку, false, чтобы показать.
         */
        void ColHidden_Set(int nCol, const bool value);
        /**
                * @brief Устанавливает свойство "скрытый" для строки.
                * @param nRow Номер строки (начиная с 1).
                * @param value true, чтобы скрыть строку, false, чтобы показать.
                */
        void RowHidden_Set(int nRow, const bool value);

        /**
         * @brief Получает номер колонки активной ячейки.
         * @return Номер колонки (начиная с 1).
         */
        int Col_Get();

        /**
         * @brief Получает номер строки активной ячейки.
         * @return Номер строки (начиная с 1).
         */
        int Row_Get();
        /**
                * @brief Устанавливает номер колонки активной ячейки.
                * @param value Номер колонки (начиная с 1).
                */
        void Col_Set(const int value);

        /**
         * @brief Устанавливает номер строки активной ячейки.
         * @param value Номер строки (начиная с 1).
         */
        void Row_Set(const int value);

        /**
         * @brief Получает имя объекта по его ID.
         * @param ObjID Идентификатор объекта.
         * @return Указатель на строку с именем объекта.
         * @warning Текущая реализация является заглушкой.
         */
        const char16_t* ObjName_Get(int ObjID);
/**
         * @brief Устанавливает имя объекта по его ID.
         * @param ObjID Идентификатор объекта.
         * @param value Новое имя объекта.
         * @warning Текущая реализация является заглушкой.
         */
        void ObjName_Set(int ObjID, const char16_t *value);

        /**
         * @brief Получает текст объекта по его ID.
         * @param ObjID Идентификатор объекта.
         * @return Указатель на строку с текстом.
         * @warning Текущая реализация является заглушкой.
         */
        const char16_t* ObjText_Get(int ObjID);

        /**
         * @brief Устанавливает текст объекта по его ID.
         * @param ObjID Идентификатор объекта.
         * @param value Новый текст объекта.
         * @warning Текущая реализация является заглушкой.
         */
        void ObjText_Set(int ObjID, const char16_t *value);

        /**
         * @brief Получает индекс последней используемой колонки на листе.
         * @return Индекс последней колонки (начиная с 1).
         */
        int MaxCol_Get();

        /**
         * @brief Получает индекс последней используемой строки на листе.
         * @return Индекс последней строки (начиная с 1).
         */
        int MaxRow_Get();
        /**
         * @brief Устанавливает "виртуальное" количество колонок для UI.
         * @param value Количество колонок для отображения.
         */
        void MaxCol_Set(const int value);

        /**
         * @brief Устанавливает "виртуальное" количество строк для UI.
         * @param value Количество строк для отображения.
         */
        void MaxRow_Set(const int value);

        // Get-методы остаются с прежними комментариями, но их реализация изменится
        int LastCol_Get();
        int LastRow_Get();

        /**
         * @brief Устанавливает "виртуальную" последнюю колонку для UI.
         * @param value Номер последней колонки.
         */
        void LastCol_Set(const int value);

        /**
         * @brief Устанавливает "виртуальную" последнюю строку для UI.
         * @param value Номер последней строки.
         */
        void LastRow_Set(const int value);
        /**
         * @brief Получает имя листа по его индексу.
         * @param nSheet Индекс листа (начиная с 1).
         * @return Указатель на строку с именем листа.
         * @warning Возвращаемый указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* SheetName_Get(int nSheet);

        /**
         * @brief Устанавливает имя листа по его индексу.
         * @param nSheet Индекс листа (начиная с 1).
         * @param value Новое имя для листа.
         */
        void SheetName_Set(int nSheet, const char16_t *value);

        /**
         * @brief Получает количество закреплённых колонок.
         * @return Количество закреплённых колонок.
         */
        int FixedCols_Get();

        /**
         * @brief Устанавливает количество закреплённых колонок.
         * @param value Количество колонок для закрепления.
         */
        void FixedCols_Set(const int value);

        /**
         * @brief Получает количество закреплённых строк.
         * @return Количество закреплённых строк.
         */
        int FixedRows_Get();

        /**
         * @brief Устанавливает количество закреплённых строк.
         * @param value Количество строк для закрепления.
         */
        void FixedRows_Set(const int value);

        /**
         * @brief Получает текст из ячейки в указанной колонке и текущей активной строке.
         * @param nCol Номер колонки (начиная с 1).
         * @return Указатель на строку со значением.
         * @warning Возвращаемый указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* ColText_Get(int nCol);

        /**
         * @brief Устанавливает текст в ячейку в указанной колонке и текущей активной строке.
         * @param nCol Номер колонки (начиная с 1).
         * @param value Текст для установки.
         */
        void ColText_Set(int nCol, const char16_t* value);

        /**
         * @brief Получает числовое значение из ячейки по полному адресу (лист, строка, колонка).
         * @param nSheet Индекс листа (начиная с 1).
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Числовое значение. Если в ячейке не число, вернёт 0.0.
         */
        double NumberSRC_Get(int nSheet, int nRow, int nCol);

        /**
         * @brief Устанавливает числовое значение в ячейку по полному адресу.
         * @param nSheet Индекс листа (начиная с 1).
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @param value Число для установки.
         */
        void NumberSRC_Set(int nSheet, int nRow, int nCol, const double value);

        /**
         * @brief Получает текст из ячейки по полному адресу.
         * @param nSheet Индекс листа (начиная с 1).
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Указатель на строку со значением.
         * @warning Возвращаемый указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* TextSRC_Get(int nSheet, int nRow, int nCol);

        /**
         * @brief Устанавливает текст в ячейку по полному адресу.
         * @param nSheet Индекс листа (начиная с 1).
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @param value Текст для установки.
         */
        void TextSRC_Set(int nSheet, int nRow, int nCol, const char16_t* value);
        /**
                * @brief Получает имя шрифта для активной ячейки.
                * @return Указатель на строку с именем шрифта.
                * @warning Указатель действителен до следующего вызова этого же метода.
                */
        const char16_t* FontName_Get();

        /**
         * @brief Устанавливает имя шрифта для активной ячейки.
         * @param value Новое имя шрифта.
         */
        void FontName_Set(const char16_t* value);

        /**
         * @brief Проверяет, является ли шрифт активной ячейки жирным.
         * @return true, если шрифт жирный.
         */
        bool FontBold_Get();

        /**
         * @brief Проверяет, является ли шрифт активной ячейки курсивным.
         * @return true, если шрифт курсивный.
         */
        bool FontItalic_Get();

        /**
         * @brief Получает размер шрифта для активной ячейки.
         * @return Размер шрифта.
         */
        int FontSize_Get();
        /**
                * @brief Устанавливает свойство "жирный" для шрифта активной ячейки.
                * @param value true, чтобы сделать шрифт жирным.
                */
        void FontBold_Set(const bool value);

        /**
         * @brief Устанавливает свойство "курсив" для шрифта активной ячейки.
         * @param value true, чтобы сделать шрифт курсивным.
         */
        void FontItalic_Set(const bool value);

        /**
         * @brief Устанавливает размер шрифта для активной ячейки.
         * @param value Размер шрифта.
         */
        void FontSize_Set(const int value);

        /**
         * @brief Получает цвет шрифта активной ячейки.
         * @return Цвет в виде целого числа (0x00bbggrr).
         */
        int FontColor_Get();

        /**
         * @brief Устанавливает цвет шрифта для активной ячейки.
         * @param value Цвет в виде целого числа (например, 0x00ff0000 для красного).
         */
        void FontColor_Set(const int value);
        /**
         * @brief Проверяет, является ли шрифт активной ячейки подчёркнутым.
         * @return true, если шрифт подчёркнутый.
         */
        bool FontUnderline_Get();

        /**
         * @brief Устанавливает свойство "подчёркнутый" для шрифта активной ячейки.
         * @param value true, чтобы сделать шрифт подчёркнутым.
         */
        void FontUnderline_Set(const bool value);

        /**
         * @brief Проверяет, является ли шрифт активной ячейки зачёркнутым.
         * @return true, если шрифт зачёркнутый.
         */
        bool FontStrikeout_Get();

        /**
         * @brief Устанавливает свойство "зачёркнутый" для шрифта активной ячейки.
         * @param value true, чтобы сделать шрифт зачёркнутым.
         */
        void FontStrikeout_Set(const bool value);

        /**
         * @brief Получает отформатированное текстовое представление значения активной ячейки.
         * @return Указатель на строку с текстом.
         * @warning Текущая реализация не применяет числовые форматы, возвращает сырое значение.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* FormattedText_Get();

        /**
         * @brief Получает строку формулы из активной ячейки.
         * @return Указатель на строку с формулой. Если формулы нет, вернёт пустую строку.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* Formula_Get();
        /**
         * @brief Получает горизонтальное выравнивание для активной ячейки.
         * @return Значение выравнивания из перечисления OpenXLSX::XLAlignmentStyle.
         */
        uint64_t HAlign_Get();

        /**
         * @brief Получает конечную колонку текущего выделения.
         * @return Номер колонки (начиная с 1).
         */
        int SelEndCol_Get();

        /**
         * @brief Получает конечную строку текущего выделения.
         * @return Номер строки (начиная с 1).
         */
        int SelEndRow_Get();

        /**
         * @brief Получает начальную колонку текущего выделения.
         * @return Номер колонки (начиная с 1).
         */
        int SelStartCol_Get();

        /**
         * @brief Получает начальную строку текущего выделения.
         * @return Номер строки (начиная с 1).
         */
        int SelStartRow_Get();
        /**
        * @brief Получает текстовое представление значения активной ячейки.
        * @return Указатель на строку с текстом.
        * @warning Указатель действителен только до следующего вызова этого же метода или TextRC_Get.
        */
        const char16_t* Text_Get();

        /**
         * @brief Получает тип значения в активной ячейке.
         * @return Целочисленное представление типа из TCellValueType.
         */
        int Type_Get();

        /**
         * @brief Получает вертикальное выравнивание для активной ячейки.
         * @return Значение выравнивания из перечисления OpenXLSX::XLAlignmentStyle.
         */
        uint64_t VAlign_Get();

        /**
         * @brief Проверяет, включён ли перенос по словам для активной ячейки.
         * @return true, если перенос по словам включён.
         */
        bool WordWrap_Get();
        /**
         * @brief Устанавливает текст в активную ячейку. Работает как TextRC_Set.
         * @param value Текст для установки.
         */
        void FormattedText_Set(const char16_t* value);

        /**
         * @brief Устанавливает формулу в активную ячейку.
         * @param value Строка с формулой (без знака '=' в начале).
         */
        void Formula_Set(const char16_t* value);

        /**
         * @brief Устанавливает горизонтальное выравнивание для активной ячейки.
         * @param value Значение выравнивания из OpenXLSX::XLAlignmentStyle.
         */
        void HAlign_Set(const uint64_t value);

        /**
         * @brief Устанавливает конечную колонку текущего выделения.
         * @param value Номер колонки (начиная с 1).
         */
        void SelEndCol_Set(const int value);

        /**
         * @brief Устанавливает конечную строку текущего выделения.
         * @param value Номер строки (начиная с 1).
         */
        void SelEndRow_Set(const int value);

        /**
         * @brief Устанавливает начальную колонку текущего выделения.
         * @param value Номер колонки (начиная с 1).
         */
        void SelStartCol_Set(const int value);

        /**
         * @brief Устанавливает начальную строку текущего выделения.
         * @param value Номер строки (начиная с 1).
         */
        void SelStartRow_Set(const int value);
        /**
        * @brief Устанавливает текст в активную ячейку.
        * @param value Текст для установки.
        */
        void Text_Set(const char16_t* value);

        /**
         * @brief Устанавливает "тип" ячейки.
         * @param value Тип ячейки.
         * @warning Текущая реализация является заглушкой, так как тип ячейки определяется её содержимым.
         */
        void Type_Set(const int value);

        /**
         * @brief Устанавливает вертикальное выравнивание для активной ячейки.
         * @param value Значение выравнивания из OpenXLSX::XLAlignmentStyle.
         */
        void VAlign_Set(const uint64_t value);

        /**
         * @brief Устанавливает свойство "перенос по словам" для активной ячейки.
         * @param value true, чтобы включить перенос по словам.
         */
        void WordWrap_Set(const bool value);

        /**
         * @brief Получает ширину указанной колонки.
         * @param nCol Номер колонки (начиная с 1).
         * @return Ширина колонки.
         */
        int ColWidth_Get(int nCol);

        /**
         * @brief Устанавливает ширину для указанной колонки.
         * @param nCol Номер колонки (начиная с 1).
         * @param value Новая ширина.
         */
        void ColWidth_Set(int nCol, const int value);
/**
         * @brief Получает высоту указанной строки.
         * @param nRow Номер строки (начиная с 1).
         * @return Высота строки.
         */
        int RowHeight_Get(int nRow);

        /**
         * @brief Устанавливает высоту для указанной строки.
         * @param nRow Номер строки (начиная с 1).
         * @param value Новая высота.
         */
        void RowHeight_Set(int nRow, const int value);

        /**
         * @brief Получает числовое значение из активной ячейки.
         * @return Числовое значение. Если в ячейке не число, вернёт 0.0.
         */
        double Number_Get();

        /**
         * @brief Устанавливает числовое значение в активную ячейку.
         * @param value Число для установки.
         */
        void Number_Set(const double value);
/**
         * @brief Получает ширину колонки в твипах.
         * @param nCol Номер колонки (начиная с 1).
         * @return Ширина колонки.
         * @warning Текущая реализация возвращает ширину не в твипах, а в условных единицах Excel.
         */
        int ColWidthTwips_Get(int nCol);

        /**
         * @brief Получает отформатированный текст из указанной ячейки.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Указатель на строку с текстом.
         * @warning Текущая реализация не применяет числовые форматы.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* FormattedTextRC_Get(int nRow, int nCol);

        /**
         * @brief Получает строку формулы из указанной ячейки.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Указатель на строку с формулой. Если формулы нет, вернёт пустую строку.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* FormulaRC_Get(int nRow, int nCol);

        /**
         * @brief Получает строку числового формата для активной ячейки.
         * @return Указатель на строку с форматом (например, "0.00%").
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* NumberFormat_Get();

        /**
         * @brief Получает тип значения в указанной ячейке.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Целочисленное представление типа из TCellValueType.
         */
        int TypeRC_Get(int nRow, int nCol);
        /**
         * @brief Устанавливает ширину колонки в твипах.
         * @param nCol Номер колонки (начиная с 1).
         * @param value Новая ширина.
         * @warning Текущая реализация устанавливает ширину в условных единицах Excel, а не в твипах.
         */
        void ColWidthTwips_Set(int nCol, const int value);

        /**
         * @brief Устанавливает формулу в указанную ячейку.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @param value Строка с формулой (без знака '=' в начале).
         */
        void FormulaRC_Set(int nRow, int nCol, const char16_t* value);

        /**
         * @brief Устанавливает числовой формат для активной ячейки.
         * @param value Строка формата (например, "0.00%").
         */
        void NumberFormat_Set(const char16_t* value);

        /**
         * @brief Получает кодировку шрифта активной ячейки.
         * @return Кодировка шрифта.
         */
        uint8_t FontCharSet_Get();

        /**
         * @brief Устанавливает кодировку для шрифта активной ячейки.
         * @param value Новая кодировка.
         */
        void FontCharSet_Set(const uint8_t value);
        /**
 * @brief Получает координаты выделенной области.
 * @param nSelection Индекс выделения (в текущей реализации игнорируется).
 * @return Структура TF1Range с координатами.
 * @warning В текущей реализации поддерживается только одно выделение.
 */
        TF1Range SelectionEx_Get(int nSelection);

        /**
         * @brief Проверяет, включена ли защита листа.
         * @return true, если лист защищён.
         */
        bool EnableProtection_Get();

        /**
         * @brief Включает или отключает защиту листа.
         * @param value true, чтобы включить защиту.
         */
        void EnableProtection_Set(const bool value);

        /**
         * @brief Проверяет, отображаются ли линии сетки.
         * @return true, если сетка видима.
         * @warning Текущая реализация является заглушкой.
         */
        bool ShowGridLines_Get();

        /**
         * @brief Включает или отключает отображение линий сетки.
         * @param value true, чтобы показать сетку.
         * @warning Текущая реализация является заглушкой.
         */
        void ShowGridLines_Set(const bool value);
        /**
         * @brief Проверяет, включен ли автоматический пересчет формул.
         * @return true, если автопересчет включен.
         */
        bool AutoRecalc_Get();

        /**
         * @brief Включает или отключает автоматический пересчет формул.
         * @param value true, чтобы включить автопересчет.
         */
        void AutoRecalc_Set(const bool value);

        /**
         * @brief Получает количество выделенных областей.
         * @return Количество выделений. В текущей реализации всегда возвращает 1.
         */
        int SelectionCount_Get();

        /**
         * @brief Устанавливает количество выделенных областей.
         * @param value Новое количество.
         * @warning Текущая реализация является заглушкой и не имеет эффекта.
         */
        void SelectionCount_Set(const int value);
        /**
         * @brief Получает единицы измерения ширины колонок.
         * @return Единицы измерения.
         * @warning Текущая реализация является заглушкой.
         */
        uint64_t ColWidthUnits_Get();

        /**
         * @brief Устанавливает единицы измерения ширины колонок.
         * @param value Новое значение.
         * @warning Текущая реализация является заглушкой.
         */
        void ColWidthUnits_Set(const uint64_t value);

        /**
         * @brief Проверяет, разрешено ли пользователю изменять размеры строк/колонок.
         * @return true, если разрешено.
         * @warning Текущая реализация является заглушкой.
         */
        bool AllowResize_Get();

        /**
         * @brief Разрешает или запрещает пользователю изменять размеры строк/колонок.
         * @param value true, чтобы разрешить.
         * @warning Текущая реализация является заглушкой.
         */
        void AllowResize_Set(const bool value);

        /**
         * @brief Получает ширину заголовка строк (область с номерами строк).
         * @return Ширина заголовка.
         */
        int HdrWidth_Get();

        /**
         * @brief Устанавливает ширину заголовка строк.
         * @param value Новая ширина.
         */
        void HdrWidth_Set(const int value);
        /**
         * @brief Получает строку формулы из указанной ячейки.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @return Указатель на строку с формулой.
         * @warning Текущая реализация не поддерживает локализованные формулы.
         */
        const char16_t* FormulaLocalRC_Get(int nRow, int nCol);

        /**
         * @brief Устанавливает формулу в указанную ячейку.
         * @param nRow Номер строки (начиная с 1).
         * @param nCol Номер колонки (начиная с 1).
         * @param value Строка с формулой.
         * @warning Текущая реализация не поддерживает локализованные формулы.
         */
        void FormulaLocalRC_Set(int nRow, int nCol, const char16_t* value);

        /**
         * @brief Проверяет, видима ли фигура (кнопка).
         * @param ObjID Индекс фигуры (начиная с 0).
         * @return true, если фигура видима.
         */
        bool ObjVisible_Get(int ObjID);

        /**
         * @brief Устанавливает видимость фигуры (кнопки).
         * @param ObjID Индекс фигуры (начиная с 0).
         * @param value true, чтобы сделать видимой.
         */
        void ObjVisible_Set(int ObjID, const bool value);

        /**
         * @brief Проверяет, включено ли отображение нулевых значений на листе.
         * @return true, если отображение включено.
         * @warning Текущая реализация является заглушкой.
         */
        bool ShowZeroValues_Get();

        /**
         * @brief Включает или отключает отображение нулевых значений на листе.
         * @param value true, чтобы включить отображение.
         * @warning Текущая реализация является заглушкой.
         */
        void ShowZeroValues_Set(const bool value);
        /**
        * @brief Проверяет, разрешено ли удаление строк/колонок на защищённом листе.
        * @return true, если удаление разрешено.
        */
        bool AllowDelete_Get();

        /**
         * @brief Разрешает или запрещает удаление строк/колонок на защищённом листе.
         * @param value true, чтобы разрешить.
         */
        void AllowDelete_Set(const bool value);

        /**
         * @brief Проверяет, разрешено ли редактирование в ячейке. Свойство для UI.
         * @return true, если разрешено.
         */
        bool AllowInCellEditing_Get();

        /**
         * @brief Разрешает или запрещает редактирование в ячейке. Свойство для UI.
         * @param value true, чтобы разрешить.
         */
        void AllowInCellEditing_Set(const bool value);

        /**
         * @brief Получает имя шрифта по умолчанию для документа.
         * @return Указатель на строку с именем шрифта.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* DefaultFontName_Get();

        /**
         * @brief Проверяет, нужно ли показывать ошибку при клике на заблокированную ячейку. Свойство для UI.
         * @return true, если нужно.
         */
        bool ShowLockedCellsError_Get();

        /**
         * @brief Получает режим отображения выделения. Свойство для UI.
         * @return Режим отображения.
         */
        uint64_t ShowSelections_Get();
        /**
         * @brief Устанавливает имя шрифта по умолчанию для всего документа.
         * @param value Новое имя шрифта.
         */
        void DefaultFontName_Set(const char16_t* value);

        /**
         * @brief Устанавливает, нужно ли показывать ошибку при клике на заблокированную ячейку. Свойство для UI.
         * @param value true, если нужно.
         */
        void ShowLockedCellsError_Set(const bool value);

        /**
         * @brief Устанавливает режим отображения выделения. Свойство для UI.
         * @param value Новый режим.
         */
        void ShowSelections_Set(const uint64_t value);

        /**
         * @brief Получает размер шрифта по умолчанию для документа.
         * @return Размер шрифта.
         */
        int DefaultFontSize_Get();

        /**
         * @brief Устанавливает размер шрифта по умолчанию для всего документа.
         * @param value Новый размер шрифта.
         */
        void DefaultFontSize_Set(const int value);

        /**
         * @brief Устанавливает индекс формата (стиля) для активной ячейки.
         * @param XF Индекс формата (стиля).
         */
        void CellFormat_Set(int XF);

        /**
         * @brief Получает индекс формата (стиля) для активной ячейки.
         * @return Индекс формата (стиля).
         */
        int CellFormat_Get();
        /**
         * @brief Проверяет, является ли активная ячейка частью объединенного диапазона.
         * @return true, если ячейка объединена.
         */
        bool MergeCells_Get();

        /**
         * @brief Объединяет или разъединяет ячейки в текущем выделении.
         * @param pMergeCells true, чтобы объединить, false, чтобы разъединить.
         */
        void MergeCells_Set(bool value);
        void MergeCells_Set(const int nRow1, const int nCol1, const int nRow2, const int nCol2, const bool Value);

        /**
         * @brief Получает настройки страницы.
         * @return Структура с настройками страницы.
         * @warning Текущая реализация является заглушкой.
         */
        TMWSPageSetup PageSetup_Get();
        /**
         * @brief Устанавливает настройки страницы.
         * @param pageSetup Структура с настройками страницы.
         * @warning Текущая реализация является заглушкой.
         */
        void PageSetup_Set(TMWSPageSetup pageSetup);
        /**
         * @brief Печатает документ.
         * @warning Текущая реализация является заглушкой.
         */
        void Print();

        /**
         * @brief Печатает страницу.
         * @warning Текущая реализация является заглушкой.
         */
        void PrintPage();

        /**
         * @brief Устанавливает заголовки печати из текущего выделения.
         * @warning Текущая реализация является заглушкой.
         */
        void PrintTitlesFromSelection_Set();

        /**
         * @brief Сохраняет документ в PDF.
         * @param filename Имя файла.
         * @param documentProperties Свойства документа.
         * @warning Текущая реализация является заглушкой.
         */
        void SaveToPDF(const char16_t* filename, const char16_t* documentProperties);

        /**
         * @brief Запускает полный пересчет всех формул в документе.
         */
        void Recalc();

        /**
         * @brief Удаляет несколько листов, начиная с указанного.
         * @param nSheet Индекс первого листа для удаления (начиная с 1).
         * @param nSheets Количество листов для удаления.
         */
        void DeleteSheets(int nSheet, int nSheets);

        /**
         * @brief Устанавливает область выделения.
         * @param nRow1 Начальная строка.
         * @param nCol1 Начальная колонка.
         * @param nRow2 Конечная строка.
         * @param nCol2 Конечная колонка.
         */
        void Selection_Set(int nRow1, int nCol1, int nRow2, int nCol2);

        /**
         * @brief Получает флаги защиты для активной ячейки.
         * @param pLocked Сюда будет записан флаг блокировки.
         * @param pHidden Сюда будет записан флаг скрытия формулы.
         */
        void Protection_Get(bool& pLocked, bool& pHidden);

        /**
         * @brief Получает координаты выделенной области.
         * @param nSelection Индекс выделения (игнорируется).
         * @param pR1 Сюда будет записана начальная строка.
         * @param pC1 Сюда будет записана начальная колонка.
         * @param pR2 Сюда будет записана конечная строка.
         * @param pC2 Сюда будет записана конечная колонка.
         * @warning В текущей реализации поддерживается только одно выделение.
         */
        void Selection_Get(int nSelection, int& pR1, int& pC1, int& pR2, int& pC2);

        /**
         * @brief Получает тип объекта.
         * @param ObjID Идентификатор объекта.
         * @return Тип объекта.
         * @warning Текущая реализация является заглушкой.
         */
        uint64_t ObjGetType(int ObjID);

        /**
         * @brief Получает ID следующего объекта.
         * @param ObjID Идентификатор текущего объекта.
         * @return ID следующего объекта.
         * @warning Текущая реализация является заглушкой.
         */
        int ObjNextID(int ObjID);
        /**
         * @brief Находит ID первого объекта в ячейке.
         * @param nSheet Индекс листа.
         * @param nRow Номер строки.
         * @param nCol Номер колонки.
         * @param aCaption Выходной параметр для заголовка объекта.
         * @return ID объекта.
         * @warning Текущая реализация является заглушкой.
         */
        int ObjFirstID(int nSheet, int nRow, int nCol, char16_t& aCaption);

        /**
         * @brief Вставляет диапазон ячеек со сдвигом существующих.
         * @param nRow1 Начальная строка.
         * @param nCol1 Начальная колонка.
         * @param nRow2 Конечная строка.
         * @param nCol2 Конечная колонка.
         * @param InsertType Тип вставки (сдвиг вправо или вниз).
         * @warning Текущая реализация является заглушкой.
         */
        void InsertRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t InsertType);

        /**
         * @brief Вставляет указанное количество листов перед листом с индексом nSheet.
         * @param nSheet Индекс, перед которым вставлять листы (начиная с 1).
         * @param nSheets Количество листов для вставки.
         */
        void InsertSheets(int nSheet, int nSheets);

        /**
         * @brief Вызывает диалог вставки листов.
         * @warning Команда для UI, не имеет реализации в библиотеке.
         */
        void EditInsertSheets();
        /**
        * @brief Устанавливает границы для активной ячейки.
        * @param ...Параметры... Определяют стиль и цвет для каждой границы.
        */
        void Border_Set(int nOutline, int nLeft, int nRight, int nTop, int nBottom, int nShade, int crOutline, uint32_t crLeft, uint32_t crRight, uint32_t crTop, uint32_t crBottom);

        /**
         * @brief Получает параметры границ для активной ячейки.
         * @param ...Параметры... Сюда будут записаны стили и цвета границ.
         */
        void Border_Get(int& pLeft, int& pRight, int& pTop, int& pBottom, int& pShade, int& pcrLeft, int& pcrRight, int& pcrTop, int& pcrBottom);

        /**
         * @brief Рисует границы для указанного диапазона ячеек.
         * @param ...Параметры... Координаты, стиль и цвет.
         * @warning Текущая реализация является заглушкой.
         */
        void DrawBorders(int row1, int col1, int row2, int col2, int borderStyle, int borderColor, bool exteriorBorders);

        /**
         * @brief Получает параметры заливки (узора) для активной ячейки.
         * @param pPattern Сюда будет записан тип узора.
         * @param pcrFG Сюда будет записан основной цвет.
         * @param pcrBG Сюда будет записан фоновый цвет.
         */
        void Pattern_Get(int& pPattern, int& pcrFG, int& pcrBG);

        /**
         * @brief Выполняет автоподбор ширины для диапазона колонок.
         * @param ...Параметры... Координаты диапазона.
         * @warning Текущая реализация является заглушкой.
         */
        void ColWidthAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults);

        /**
         * @brief Устанавливает узор и цвета заливки для активной ячейки.
         * @param nPattern Тип узора из перечисления OpenXLSX::XLPatternType.
         * @param crFG Цвет переднего плана (узора).
         * @param crBG Цвет фона.
         */
        void Pattern_Set(int nPattern, uint32_t crFG, uint32_t crBG);

        /**
         * @brief Устанавливает цвет фона для активной ячейки.
         * @param crBG Цвет фона.
         */
        void BackgroundColor_Set(uint32_t crBG);

        /**
         * @brief Выполняет автоподбор высоты для диапазона строк.
         * @param ...Параметры... Координаты диапазона.
         * @warning Текущая реализация является заглушкой.
         */
        void RowHeightAuto_Set(int nRow1, int nCol1, int nRow2, int nCol2, bool bSetDefaults);
/**
         * @brief Сортирует диапазон ячеек.
         * @warning Текущая реализация является заглушкой.
         */
        void Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, const char16_t* keysString);

        /**
         * @brief Сортирует диапазон ячеек.
         * @warning Текущая реализация является заглушкой.
         */
        void Sort(int nR1, int nC1, int nR2, int nC2, bool bSortByRows, SortKeys Keys);

        /**
         * @brief Устанавливает флаги защиты (Locked, Hidden) для активной ячейки.
         * @param bLocked true, чтобы заблокировать ячейку.
         * @param bHidden true, чтобы скрыть формулу в ячейке.
         */
        void Protection_Set(bool bLocked, bool bHidden);

        /**
         * @brief Скрывает или показывает диапазон колонок.
         * @param nCol1 Начальная колонка.
         * @param nCol2 Конечная колонка.
         * @param bHidden true, чтобы скрыть.
         */
        void ColHidden_Set(int nCol1, int nCol2, bool bHidden);

        /**
         * @brief Устанавливает активную ячейку.
         * @param nRow Номер строки.
         * @param nCol Номер колонки.
         */
        void ActiveCell_Set(int nRow, int nCol);

        /**
         * @brief Показывает активную ячейку (прокручивает к ней).
         * @warning Команда для UI, не имеет реализации в библиотеке.
         */
        void ShowActiveCell();
        /**
         * @brief Копирует диапазон ячеек.
         * @warning Текущая реализация является заглушкой.
         */
        void CopyRange(int nDstR1, int nDstC1, int nDstR2, int nDstC2, int hSrcSS, int nSrcR1, int nSrcC1, int nSrcR2, int nSrcC2);

        /**
         * @brief Возвращает дескриптор таблицы.
         * @warning Метод унаследован от старого API и не имеет смысла в новой реализации.
         */
        int SS();

        /**
         * @brief Устанавливает все свойства шрифта для активной ячейки за один вызов.
         */
        void Font_Set(const char16_t* pName, int nSize, bool bBold, bool bItalic, bool bUnderline, bool bStrikeout, uint32_t crColor, bool bOutline, bool bShadow);

        /**
         * @brief Получает все свойства шрифта активной ячейки за один вызов.
         * @warning Не реализовано получение имени шрифта из-за неясного типа параметра pName.
         */
        void Font_Get(char16_t& pName, int& pSize, bool& pBold, bool& pItalic, bool& pUnderline, bool& pStrikeout, int& pcrColor, bool& pOutline, bool& pShadow);

        /**
         * @brief Удаляет диапазон ячеек со сдвигом.
         * @warning Текущая реализация является заглушкой.
         */
        void DeleteRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ShiftType);

        /**
         * @brief Очищает содержимое и/или форматирование диапазона ячеек.
         * @param ClearType Тип очистки (содержимое, форматы и т.д.).
         */
        void ClearRange(int nRow1, int nCol1, int nRow2, int nCol2, uint64_t ClearType);

        /**
         * @brief Очищает содержимое и/или форматирование текущего выделения.
         * @param ClearType Тип очистки.
         */
        void EditClear(uint64_t ClearType);
        /**
        * @brief Устанавливает стили линий для каждой из четырёх границ активной ячейки.
        */
        void BorderStyle_Set(int StyleTopBorder, int StyleLeftBorder, int StyleBottomBorder, int StyleRightBorder);

        /**
         * @brief Получает стили линий для каждой из четырёх границ активной ячейки.
         */
        void BorderStyle_Get(int& StyleTopBorder, int& StyleLeftBorder, int& StyleBottomBorder, int& StyleRightBorder);

        /**
         * @brief Устанавливает цвета для каждой из четырёх границ активной ячейки.
         */
        void BorderColor_Set(int ColorTopBorder, int ColorLeftBorder, int ColorBottomBorder, int ColorRightBorder);

        /**
         * @brief Получает цвета для каждой из четырёх границ активной ячейки.
         */
        void BorderColor_Get(int& ColorTopBorder, int& ColorLeftBorder, int& ColorBottomBorder, int& ColorRightBorder);

        /**
         * @brief Создаёт объект "Кнопка".
         * @warning Текущая реализация является заглушкой.
         */
        int InputButton(int nRow1, int nCol1, int nRow2, int nCol2, const char16_t* aCaption, const char16_t* aObjName);

        /**
         * @brief Получает полную информацию о границах для указанной ячейки.
         */
        void BorderInfo_Get(int nRow, int nCol, int& BorderTopStyle, int& BorderLeftStyle, int& BorderBottomStyle, int& BorderRightStyle, int& BorderTopColor, int& BorderLeftColor, int& BorderBottomColor, int& BorderRightColor);

        /**
         * @brief Получает горизонтальное выравнивание для указанной ячейки.
         * @return Значение выравнивания.
         */
        int HAlignRC_Get(int nRow, int nCol);

        /**
         * @brief Преобразует целое число в тип горизонтального выравнивания.
         */
        THFlxAlignment toHAlign(int aH);

        /**
         * @brief Преобразует целое число в тип вертикального выравнивания.
         */
        TVFlxAlignment toVAlign(int aV);

        /**
         * @brief Получает вертикальное выравнивание для указанной ячейки.
         * @return Значение выравнивания.
         */
        int VAlignRC_Get(int nRow, int nCol);

        /**
         * @brief Получает общее количество фигур (кнопок) на листе.
         * @return Количество фигур.
         */
        int objlistCount_Get();
        /**
        * @brief Получает свойства фигуры (кнопки) по её индексу в списке.
        * @param aI Индекс фигуры (начиная с 0).
        * @param nRow Сюда будет записана строка, к которой привязана фигура.
        * @param nCol Сюда будет записана колонка.
        * @param objectName Сюда будет записано имя/ID фигуры.
        * @warning Получение имени (objectName) пока не реализовано.
        */
        void objlistI_PropRC_Get(int aI, int& nRow, int& nCol, char16_t& objectName);

        /**
         * @brief Получает ID числового формата для указанной ячейки.
         * @return ID формата.
         */
        int NFormatRC_Get(int nRow, int nCol);

        /**
         * @brief Получает имя стиля для указанной ячейки.
         * @return Указатель на строку с именем стиля.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* StyleNameRC_Get(int nRow, int nCol);

        /**
         * @brief Получает цвет фона для указанной ячейки.
         * @return Цвет в формате ARGB.
         */
        uint32_t BackColorRC_Get(int nRow, int nCol);

        /**
         * @brief Получает несколько свойств указанной ячейки.
         * @warning Получение строки формата (Format) не реализовано.
         */
        void CellInfoRC_Get(int nRow, int nCol, char16_t& Format, bool& Locked, bool& WrapText);

        /**
         * @brief Получает несколько свойств шрифта для указанной ячейки.
         * @warning Получение имени шрифта (FontName) не реализовано.
         */
        void FontInfoRC_Get(int nRow, int nCol, uint32_t& Color, char16_t& FontName, int& FontSize, int& Underline, bool& Bold, bool& Italic);

        /**
         * @brief Комплексный метод для установки всех свойств формата для текущего выделения.
         * @param is... Флаги, указывающие, какие свойства нужно применить.
         * @param ..._  Значения для применяемых свойств.
         */
        void F1CellFormat_Set(
                bool isAlignHorizontal, int AlignHorizontal_, bool isWordWrap, bool WordWrap_,
                bool isAlignVertical, int AlignVertical_, bool isBorderStyle, int BorderStyle_,
                int pShade_, bool isBorderColor, int BorderColor_, bool isFontName,
                const char16_t* FontName_, bool isFontSize, int FontSize_, bool isFontBold,
                bool FontBold_, bool isFontItalic, bool FontItalic_, bool isFontUnderline,
                bool FontUnderline_, bool isFontStrikeout, bool FontStrikeout_, bool isFontColor,
                int FontColor_, bool isPatternStyle, int PatternStyle_, int PatternBG_,
                int PatternFG_, bool isProtectionLocked, bool ProtectionLocked_,
                bool ProtectionHidden_, bool isNumberFormat, const char16_t* NumberFormat_,
                bool MergeCells_, bool isRotation, int Rotation_
        );
        /**
        * @brief Находит индекс первой видимой колонки.
        * @param nVisibleCol1 Сюда будет записан индекс первой видимой колонки.
        * @return Индекс первой видимой колонки или 0, если все скрыты.
        */
        int NVisibleCol_Get(int& nVisibleCol1);

        /**
         * @brief Получает числовой результат вычисления формулы в ячейке.
         * @return Числовое значение.
         */
        double FormulaResultNumberRC(int nRow, int nCol);

        /**
         * @brief Получает текстовый результат вычисления формулы в ячейке.
         * @return Указатель на строку с результатом.
         * @warning Указатель действителен только до следующего вызова этого же метода.
         */
        const char16_t* FormulaResultTextRC(int nRow, int nCol);


    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
        OpenXLSX::XLFont getActiveCellFont(bool createNew);
        OpenXLSX::XLAlignment getActiveCellAlignment(bool createNew);
        OpenXLSX::XLFill getActiveCellFill(bool createNew);
        OpenXLSX::XLCellFormat getActiveCellFormat(bool createNew);
        OpenXLSX::XLBorder getActiveCellBorder(bool createNew);
        // Вспомогательный метод для поиска или создания ID числового формата
        uint32_t getNumberFormatId(const std::string& formatCode);
        struct ButtonInfo {
            std::string name;
            int row;
            int col;
            uint32_t shapeIndex; // Храню индекс, а не сам объект
        };
        void CacheSheetObjects();
        TCellValueType CellValueType_Get(int nRow, int nCol);
        template<typename T> friend class Property;
        friend class BoolPropertyProxy;
        friend class IntPropertyProxy;
        friend class TextRCProxy;
        friend class NumberRCProxy;
        friend class SheetNameProxy;
        friend class FormulaRCProxy;
    };
}

#endif //F1FLEXCEL_CPPPROJECT_TF1FLEXCEL_H