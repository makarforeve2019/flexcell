
#ifndef FormTypesH
#define FormTypesH

#include <vector>
#include <fstream>
#include <string>
#include "json.hpp"
#include "../system/ClassesUtils.h"
#include <optional>


using json = nlohmann::json;

const int deltaDType = 20;
const int MaxTIerarxS50Size = 50;

typedef std::vector<int> TIntArray;
typedef TIntArray* PIntArray;

class TStyleOforml {
public:
    int Rzd1MergeN;
    std::string Rzd1font;
    int Rzd1h;
    int Rzd1is_bold;
    int Rzd1fontColor;
    int Rzd1FonColor;
    int Rzd1VerAlign;
    int Rzd1HorAlign;

    int Itg1MergeN;
    std::string Itg1font;
    int Itg1h;
    int Itg1is_bold;
    int Itg1fontColor;
    int Itg1FonColor;
    int Itg1VerAlign;
    int Itg1HorAlign;

    int Rzd2MergeN;
    std::string Rzd2font;
    int Rzd2h;
    int Rzd2is_bold;
    int Rzd2fontColor;
    int Rzd2FonColor;
    int Rzd2VerAlign;
    int Rzd2HorAlign;

    int Itg2MergeN;
    std::string Itg2font;
    int Itg2h;
    int Itg2is_bold;
    int Itg2fontColor;
    int Itg2FonColor;
    int Itg2VerAlign;
    int Itg2HorAlign;

    int Rzd3MergeN;
    std::string Rzd3font;
    int Rzd3h;
    int Rzd3is_bold;
    int Rzd3fontColor;
    int Rzd3FonColor;
    int Rzd3VerAlign;
    int Rzd3HorAlign;

    int Itg3MergeN;
    std::string Itg3font;
    int Itg3h;
    int Itg3is_bold;
    int Itg3fontColor;
    int Itg3FonColor;
    int Itg3VerAlign;
    int Itg3HorAlign;

    bool isVTomCisleKursiv;

    void SetToNull();

    TStyleOforml(){};

    ~TStyleOforml() {
        SetToNull();
    };

    // Статический метод для десериализации из JSON строки
    static TStyleOforml* Deserialize(const std::string& jsonString) {
        try {
            nlohmann::json j = nlohmann::json::parse(jsonString);
            TStyleOforml *style;

            // Парсинг и заполнение всех полей
            style->Rzd1MergeN = j.at("Rzd1MergeN").get<int>();
            style->Rzd1font = j.at("Rzd1font").get<std::string>();
            style->Rzd1h = j.at("Rzd1h").get<int>();
            style->Rzd1is_bold = j.at("Rzd1is_bold").get<int>();
            style->Rzd1fontColor = j.at("Rzd1fontColor").get<int>();
            style->Rzd1FonColor = j.at("Rzd1FonColor").get<int>();
            style->Rzd1VerAlign = j.at("Rzd1VerAlign").get<int>();
            style->Rzd1HorAlign = j.at("Rzd1HorAlign").get<int>();

            style->Itg1MergeN = j.at("Itg1MergeN").get<int>();
            style->Itg1font = j.at("Itg1font").get<std::string>();
            style->Itg1h = j.at("Itg1h").get<int>();
            style->Itg1is_bold = j.at("Itg1is_bold").get<int>();
            style->Itg1fontColor = j.at("Itg1fontColor").get<int>();
            style->Itg1FonColor = j.at("Itg1FonColor").get<int>();
            style->Itg1VerAlign = j.at("Itg1VerAlign").get<int>();
            style->Itg1HorAlign = j.at("Itg1HorAlign").get<int>();

            style->Rzd2MergeN = j.at("Rzd2MergeN").get<int>();
            style->Rzd2font = j.at("Rzd2font").get<std::string>();
            style->Rzd2h = j.at("Rzd2h").get<int>();
            style->Rzd2is_bold = j.at("Rzd2is_bold").get<int>();
            style->Rzd2fontColor = j.at("Rzd2fontColor").get<int>();
            style->Rzd2FonColor = j.at("Rzd2FonColor").get<int>();
            style->Rzd2VerAlign = j.at("Rzd2VerAlign").get<int>();
            style->Rzd2HorAlign = j.at("Rzd2HorAlign").get<int>();

            style->Itg2MergeN = j.at("Itg2MergeN").get<int>();
            style->Itg2font = j.at("Itg2font").get<std::string>();
            style->Itg2h = j.at("Itg2h").get<int>();
            style->Itg2is_bold = j.at("Itg2is_bold").get<int>();
            style->Itg2fontColor = j.at("Itg2fontColor").get<int>();
            style->Itg2FonColor = j.at("Itg2FonColor").get<int>();
            style->Itg2VerAlign = j.at("Itg2VerAlign").get<int>();
            style->Itg2HorAlign = j.at("Itg2HorAlign").get<int>();

            style->Rzd3MergeN = j.at("Rzd3MergeN").get<int>();
            style->Rzd3font = j.at("Rzd3font").get<std::string>();
            style->Rzd3h = j.at("Rzd3h").get<int>();
            style->Rzd3is_bold = j.at("Rzd3is_bold").get<int>();
            style->Rzd3fontColor = j.at("Rzd3fontColor").get<int>();
            style->Rzd3FonColor = j.at("Rzd3FonColor").get<int>();
            style->Rzd3VerAlign = j.at("Rzd3VerAlign").get<int>();
            style->Rzd3HorAlign = j.at("Rzd3HorAlign").get<int>();

            style->Itg3MergeN = j.at("Itg3MergeN").get<int>();
            style->Itg3font = j.at("Itg3font").get<std::string>();
            style->Itg3h = j.at("Itg3h").get<int>();
            style->Itg3is_bold = j.at("Itg3is_bold").get<int>();
            style->Itg3fontColor = j.at("Itg3fontColor").get<int>();
            style->Itg3FonColor = j.at("Itg3FonColor").get<int>();
            style->Itg3VerAlign = j.at("Itg3VerAlign").get<int>();
            style->Itg3HorAlign = j.at("Itg3HorAlign").get<int>();

            style->isVTomCisleKursiv = j.at("isVTomCisleKursiv").get<bool>();

            return style;
        } catch (const std::exception& e) {
            throw std::runtime_error("Ошибка десериализации: " + std::string(e.what()));
        }
    }

};

struct TCellValueAtr {
    int Row;
    int Col;
    int CellId;
    int PokazId;
    int ObjectId;
    int OwnerId;
    int comnp_id;
    TDateTime ValueDate;
    short god;
    unsigned char kvrtl;
    unsigned char mesjc;
    short den;
    short DataType;
    int pokaz_type;
    int EnumId;
    short VidOtch;
    int PlanId;
    short EvalType;
    short cell_period;
    int BPId;
    int DPId;
    signed char coef_Pokaz=0;
    signed char coef_Forma=0;
    int class_id_ForKey;
    int ist_fin_id;
    int cell_id2;
    int cell_id3;
    int cell_id4;
    bool is_block;
    bool is_Prisv;
    bool read_onlyProg;
    unsigned char kakAgregirUpByField;
    std::string stmnt;
};
typedef TCellValueAtr* PCellValueAtr;

struct TAlignmentMerge {
    int rowVid;            // Field for selection
    int form_str_nSub;     // Field for selection (contains Class ID in case of hierarchy)
    int is_group;          // Indicates whether it's a group
    unsigned char Align_vert;   // Vertical alignment
    unsigned char Align_gor;    // Horizontal alignment
    std::string font;      // Font name
    unsigned char H;       // Height
    unsigned char Is_bold; // Indicates whether the font is bold
};
typedef TAlignmentMerge* PAlignmentMerge;

struct TFormatStyle {
    int align_vert;
    unsigned char align_gor;
    std::string font;
    unsigned char h;
    unsigned char is_bold;
    unsigned char isOrientation;
    unsigned char ofset; // отступ
    int FontColor;
    int fonColor;
    signed char borderStyle;
    int borderColor;
    std::string NumberFormat;

    TFormatStyle() {}


    TFormatStyle(const std::string aFont, unsigned char aH, unsigned char aIs_bold, int aFontColor, int aFonColor)
            : font(aFont), h(aH), is_bold(aIs_bold), FontColor(aFontColor), fonColor(aFonColor) {
        setToNull();
        font = aFont;
        h = aH;
        is_bold = aIs_bold;
        FontColor = aFontColor;
        fonColor = aFonColor;
        if (fonColor == 0)
            fonColor = 16777215; // белый

    }

    TFormatStyle(const nlohmann::json& jo) {
        setToNull();
        for (auto& element : jo.items()) {
            std::string key = element.key();
            auto& value = element.value();
            if (key == "Valign")
                align_vert = value.get<int>();
            else if (key == "Halign")
                align_gor = value.get<int>();
            else if (key == "отступ")
                ofset = value.get<int>();
            else if (key == "фонт")
                font = value.get<std::string>();
            else if (key == "h")
                h = value.get<int>();
            else if (key == "стиль") {
                for (auto& style : value) {
                    std::string s = style.get<std::string>();
                    if (s == "bold")
                        is_bold |= 0x01;
                    if (s == "Italic")
                        is_bold |= 0x02;
                    if (s == "Underline")
                        is_bold |= 0x04;
                    if (s == "Strikeout")
                        is_bold |= 0x08;
                }
            }
            else if (key == "цв_т")
                FontColor = value.get<int>();
            else if (key == "цв_ф")
                fonColor = value.get<int>();
        }
    }

    void setToNull() {
        align_vert = 0;
        align_gor = 0;
        font = "";
        h = 0;
        is_bold = 0;
        isOrientation = 0;
        FontColor = 0;
        fonColor = 16777215; // белый
        borderStyle = 0;
        borderColor = 0;
        NumberFormat = "General";
    }

    void setTo(const std::string& aFont, unsigned char aH, unsigned char aIs_bold) {
        setToNull();
        font = aFont;
        h = aH;
        is_bold = aIs_bold;
    }

    unsigned char Align_vert;
    unsigned char Is_bold;
};
typedef TFormatStyle* PFormatStyle;

struct TAtrStmnt {
    int form_id;
    int form_str_n;
    std::string Stmnt;
    TFormatStyle FormatStyle;
    bool read_only;
    unsigned char readonly;
    unsigned char is_hidden;
    int IsGroup;
    int PeriodType;
    int vid_otch;
    int is_ist_fin;
    int ist_fin_grad_id;
    int is_group;
    int natur_vid;
    int ValuePeriod;
    std::string note1;
    std::string note2;
    int field_id;
    int CoefPlus1;
    unsigned char is_Check;
    unsigned char ColOnFilter2;
    int plan_id;
    int pokaz_id;
    int is_set_god;
    int god_plus;
    int god_ravno;
    std::string din_izm;
    unsigned char IsFieldsSprvcnik;
    std::string toTblName;
    std::string toColumnName1;
    short nValidator;
    short vid_uslovij;
    double Value1;
    double Value2;
    std::string Shablon;
    std::string Msg;
    int cell_idFiltr;
    short vid_otchFiltr;
    TDateTime value_dateFiltr;
    int NumberPP;
    short attr_3;
    short attr_4;
    unsigned char tocnost;
    unsigned char isGroupSelect;
    bool isColHidden;
    std::string DocumNum;
    TDateTime DocumDate;
    std::string DocumNote;
    int stmntPrisvoitLink;
    nlohmann::json CondFormats;
    bool isCellSizeFixed;
    std::string ModulOfMetodsOnClick;
};
typedef TAtrStmnt* PAtrStmnt;

struct TAtrRow {
    int Row;
    bool IsTerminal;
    int OwnerNomRow;
    int OwnerId;
    int ObjectId;
    int objekt_class_id;
    unsigned char CurRowVid;
    unsigned char level_sign;
    unsigned char pokaz_vert;
    unsigned char L;
    bool inSort;
    bool IsUpd;
    int str_id;
    unsigned char is_filter2;
    unsigned char ColOnFilter2;
    bool vrem_hidden;
    unsigned char isRowHidden;
    bool isReadOnly;
    bool isRefresh;
    unsigned char tocnost;
    std::string pokaz_bk_code;
    int ist_fin_grad_id;
    int plan_id;
    int god;
    int colReplValue;
    std::string NameReplValue;
    TFormatStyle FormatStyle;
    std::string msg;
    bool isPriglashKVstavke;
    int NomUrovRazdela;
    bool isRow1n;
    bool isRow1nReadOnly;
    bool isRow1nhidden;
    bool isMarked;
    int groupSpan;
    int groupMarkLevel;
    unsigned char is_filter;
};
typedef TAtrRow* PAtrRow;

struct Tftb {
    int Row;
    int Col;
    int CellId; // In the case of EDIZM, it contains edizm_id
    int PokazId;
    int ObjectId;
    TDateTime ValueDate; // Assuming TDateTime is represented as double
    short DataType;   // Type of value
    unsigned char pokaz_type;
    short VidOtch;
    int PlanId;
    short EvalType;
    int BPId; // Budget recipient ID
    int DPId; // den_potok_id - for the schedule
    unsigned char CurRowVid; 
    signed char coef_Pokaz;  
    signed char coef_Forma;  
    std::string note2;
    int atr1;
    std::string note1;
    double value;      // Numeric value
    std::string str_value; // String value
    std::string docum_id;  // Document ID
};
typedef Tftb* Pftb;


#include <vector>
#include <string>
#include <list>

class TAtrStmnt; // Forward declaration for TAtrStmnt class

class TStyleOforml; // Forward declaration for TStyleOforml class

class TfilterByClmns; // Forward declaration for TfilterByClmns class

// Определение класса TwwStringList
class TwwStringList : public TStringList {
public:
    int FormId;
    int FormOrder;
    std::string FormObozn;
    int plan_id;
    std::string plan_name;
    int is_dublir_itogo;
    std::vector<TAtrStmnt> ListStmnt; // Список характеристик операторов
    TStringList* ForTopBottom;
    TList<PAtrRow>* ListRows;
    unsigned char IsParamForm;
    int ParamObjectId;
    std::string ParamObjectName;
    std::string ParamClassIdlist;
    unsigned char work_with;
    unsigned char work_with_gf;
    int form_is_main;
    int RootPokazId;
    int cast_id_UP;
    std::string cast_id_UP_name;
    unsigned char form_readonly;
    bool isRefreshRows;
    unsigned char is_add;
    unsigned char is_copy;
    unsigned char is_sort;
    unsigned char is_edIzm;
    unsigned char tocnost;
    std::string NameOwnerRaskrit;
    int ObjectIdOwnerRaskrit;
    int nRowRaskrit;
    bool IsRefreshOwner;
    std::string ColFuncs;
    unsigned char vid_form;
    //mak TList<TMyChart>* ChartList;
    int PokazCol;
    int PokazColAlter;
    int PokazColother;
    int nameColOkraska;
    int DeltaRow;
    int MaxHrRow;
    int MaxHrCol;
    int form_k_zagtop;
    int form_k_zagbottom;
    int form_k1;
    int form_begBodyRow;
    int form_endBodyRow;
    int isLandscape;
    int isShowZero;
    int nColForBP;
    int InitKolCol;
    std::shared_ptr<TObject> aa;
    int form_str_n_AlignmentObjedinit;
    std::string OgrUserOprcii;
    int ogr_class_id;
    int grnlutkp_id;
    int grnlutkp_id_ForRedactir;
    bool IsStmntPrisvoit;
    int nRowOfStmntTestReadOnly = 0;
    bool isStmntTestReadOnly = false;
    bool isPU0;
    bool ComplexForm;
    int nModulOfMetods;
    unsigned char isGroupSelect;
    int isShapkaColor, isLinesColor, isPanelSearch, ColorLockedCell;
    int is_noout_nstr;
    bool isOnActivForm_RefreshROcells;
    bool isExecAllVspomogForms;
    bool isDoOkraska;
    int oldRow;
    int pPattern, pPatternNew;
    int pcrFG, pcrFGNew;
    bool isKBKfiltr;
    int KBKfiltrRow, KBKfiltrCol;
    bool isDatefiltr;
    int DatefiltrFromRow, DatefiltrFromCol;
    int DatefiltrToRow, DatefiltrToCol;
    std::string stmntODINAKOVZNAC;
    bool isPlFctOstatok;
    bool isPriglashKVstavke;
    TStringList* FormEventsList;
    bool isISTZNACHVVODPK;
    unsigned char modeNumber;
    TDateTime selectedDate;
    int wwPlan_id;
    int OwnerFormPlanId;
    TStyleOforml* StyleOforml;
    std::shared_ptr<TfilterByClmns> filterByClmns;
    int colTree1;
    int isGroupSost;

    // Конструктор
    TwwStringList() {
        FormEventsList = new TStringList();
        ListRows = new TList<PAtrRow>();
        StyleOforml = new TStyleOforml();
        ForTopBottom = nullptr; // По умолчанию null
    }

    ~TwwStringList() {
        ClearTww();
        Clear();
    }

    void ClearTww() {
        if (ListRows) {
            for (auto& row : *ListRows) {
                if (row != nullptr) {
                    delete row;  // Освобождаем каждый элемент, если он не равен nullptr
                    row = nullptr; // После освобождения указываем на nullptr для безопасности
                } // Освобождаем каждый элемент внутри ListRows
            }
            ListRows->Clear();  // Очищаем контейнер от всех указателей
            delete ListRows;     // Освобождаем память, выделенную для самого объекта ListRows
            ListRows = nullptr;  // Обнуляем указатель для избежания повторного доступа
        }


        // Очистка `FormEventsList`
        FormEventsList->Clear();
        delete FormEventsList;
        FormEventsList = nullptr;

        // Освобождение `StyleOforml`, если требуется
        if (StyleOforml != nullptr) {
            delete StyleOforml;
            StyleOforml = nullptr;
        }

        // Очистка ForTopBottom
        if (ForTopBottom) {
            for (auto& item : ForTopBottom->items) {
                if (auto obj = std::get_if<Pftb>(&item.second)) {
                    delete *obj;  // Удаляем объект Pftb
                }
            }
            ForTopBottom->Clear();  // Очищаем контейнер
            delete ForTopBottom;    // Удаляем объект
            ForTopBottom = nullptr; // Обнуляем указатель
        }

        // Освобождение всех объектов типа `PCellValueAtr` в items
        for (auto& item : items) {
            if (auto obj = std::get_if<Pftb>(&item.second)) {
                delete *obj;  // Удаляем объект, если это Pftb
            } else if (auto cell = std::get_if<PCellValueAtr>(&item.second)) {
                delete *cell;  // Удаляем объект, если это PCellValueAtr
            }
        }
    }
};


struct TRowStmnt {
    int form_id;
    int form_str_n;
    std::string stmnt;
    int nStolbec;
    std::string strName;
    std::string note2;
    int readonly;
    int Plan_Id;
    int level_sign;
    int Is_open;
    int is_hidden;
    int is_link; // noPrint
    int is_ForObject;
    int isMergeVert; // is_filter
    int isMergeHoriz;
    int is_filter2;
    int ColOnFilter2;
    int is_set_god;
    int god_plus;
    int god_ravno;
    int Period;
    int ValuePeriod;
    int tocnost;
    unsigned char align_vert;
    unsigned char align_gor;
    std::string font;
    int h;
    int is_bold;
    int is_ist_fin;
    int ist_fin_grad_id;
};
typedef TRowStmnt* PRowStmnt;

struct TIerarx {
    std::vector<TRowStmnt> PeredClass;
    int owner; // номер элемента этого массива - владельца
    int class_id; // сохраняю в plan_id
    int field_id; // сохраняю в field_id, в случае связи: указывает на поле класса типа связь с объектом другого класса
    unsigned char is_link; // сохраняю в is_group, означает: 0-подчинение, 1-связь
    unsigned char is_ForObject; // сохраняю в isGroupSelect{было is_bold}, означает: 0 - для вставки объекта, 1- для перехода
    int level; // уровень в дереве, сохраняю в CoefPlus1{было align_vert}
    int regimGenerac; // сохраняю в vid_uslovij: Определяет режим генерации строк для объекта класса:
                      //    = 0 - одна строка, = 1 - несколько строк посредством итерации
    std::string idsCikl;  // сохраняю в shablon: определяет итерацию в виде id;id; и т.д.
    int isMergeVert; // сохраняю в is_sort{is_filter}:  = 1 - если задан "Объединить ячейки (по Вертикали) для одной и той же характеристики объекта, содержащие одинаковые значения"
    int isNoOutOtsutstv; // сохраняю в is_Check: = 1 - если задан "Не выводить строки с отсутствующими значениями, зависящими от переменной цикла"
    int isMergeHoriz; // сохраняю в uroven_sort {было is_filter2}: = 1 - если задан "Объединить ячейки (по Горизонтали) для одной и той же характеристики объекта, содержащие одинаковые значения"
    unsigned char is_filter2; // это копия is_filter2 в описании строки
    unsigned char ColOnFilter2; // это копия ColOnFilter2 в описаниистроки
    int is_hidden;
    int form_str_n; // номер строки в FormStr
    std::vector<TRowStmnt> PosleClass;
};
typedef TIerarx* PIerarx;
typedef TIerarx TIerarxS50[MaxTIerarxS50Size];
typedef TIerarxS50* PIerarxS50;
class TExecuteSxemaIerarx {
public:
    std::vector<TRowStmnt> PeredIerarx;   // Динамический массив структур TRowStmnt
    std::array<TIerarx, 50> IerarxS;      // Статический массив из 50 элементов TIerarx
    std::vector<TRowStmnt> PosleIerarx;   // Динамический массив структур TRowStmnt

    void setNULL() {
        // Очистка векторов
        PeredIerarx.clear();
        PosleIerarx.clear();
        }
};

// Использование указателя на TExecuteSxemaIerarx
extern TExecuteSxemaIerarx* PExecuteSxemaIerarx;

struct TCol1Col2 {
    union {
        struct {
            short col1;
            short col2;
        };
        int colall;
    };
};

class TCol1_Col2 {
public:
    static int getCol1(int aColAll) {
        TCol1Col2 rec;
        rec.colall = aColAll;
        return rec.col1;
    }

    static int getCol2(int aColAll) {
        TCol1Col2 rec;
        rec.colall = aColAll;
        return rec.col2;
    }

    static int getColAll(short aCol1, short aCol2) {
        TCol1Col2 rec;
        rec.col1 = aCol1;
        rec.col2 = aCol2;
        return rec.colall;
    }
};



struct TFormEvent {
    int event_id;
    std::string eventclass;
    std::string eventrowcolSG;
    nlohmann::json eventrowcol;
    std::string handlerlSG;
    nlohmann::json handler;
};
typedef TFormEvent* PFormEvent;

struct TInfoAlbumLogic {
    std::string CurrAlbumName;
    int accessalbum_idSelBlockSubj;
    int CurrPlanId;
    std::string CurrPlanName;
    bool CanEdit;
    unsigned char CurrVidOtch;
    int CurrYear;
    unsigned char CurrQuarter;
    unsigned char CurrMonth;
    TDateTime CurrDate;
    std::string aShablonKBK;
    int aId_Owner_link1n;
};

class TCallBackMethodPointer;

struct TInfoFormLogic {
    bool isISTZNACHVVODPK;
    int modeNumber;
    TDateTime selectedDate;

    bool isCallBackMethod;
    TCallBackMethodPointer* CallBackMethod;

    TCallBackMethodPointer* doLoadFromXLS;

    bool isOtlogenPokaz;
    int OtlogenPokazId;

    bool isShablonKBK;
    std::string Rzdl;
    std::string CS;
    std::string VR;
    std::string KOSGU;
    std::string codeKBK;

    bool isTimePeriod;
    TDateTime DateDiapazonFrom;
    TDateTime DateDiapazonTo;

    bool isDopGrFin;
    int isSelect;
    TDateTime Dt1;
    TDateTime Dt2;
    int id_FromPref;
};


const int cCellContentNum = 1;
const int cCellContentNumTop = 200;
const int cCellContentNumBottom = 300;
const int cColWidthNum = 500;
const int cColDestNum = 1000;
const int cCellContentNumNextBeg = 11000;
const int cCellContentNumNextEnd = 21000;





#endif
