//#include "FormTypes.h"
//
//TwwStringList::~TwwStringList() {
//    // if (aa != nullptr) {
//    //     delete aa;
//    //     aa = nullptr;
//    // }
//    if (filterByClmns != nullptr) {
//        delete filterByClmns;
//        filterByClmns = nullptr;
//    }
//    for (auto& AtrStmnt : ListStmnt) {
//        if (AtrStmnt.CondFormats != nullptr) {
//            delete AtrStmnt.CondFormats;
//        }
//    }
//    if (ListRows != nullptr) {
//        for (int i = 0; i < ListRows->Count; ++i) {
//            delete ListRows->Items[i];
//        }
//        delete ListRows;
//        ListRows = nullptr;
//    }
//    if (ForTopBottom != nullptr) {
//        for (int i = 0; i < ForTopBottom->Count; ++i) {
//            delete static_cast<Pftb>(ForTopBottom->Objects[i]);
//        }
//        delete ForTopBottom;
//        ForTopBottom = nullptr;
//    }
//    if (ChartList != nullptr) {
//        for (int i = 0; i < ChartList->Count; ++i) {
//            delete static_cast<TMyChart*>(ChartList->Items[i]); // Assuming TMyChart is the correct type
//        }
//        delete ChartList;
//        ChartList = nullptr;
//    }
//    for (int i = 0; i < FormEventsList->Count; ++i) {
//        PFormEvent FormEvent = static_cast<PFormEvent>(FormEventsList->Objects[i]);
//        if (FormEvent->eventrowcol != nullptr) {
//            delete FormEvent->eventrowcol;
//        }
//        if (FormEvent->handler != nullptr) {
//            delete FormEvent->handler;
//        }
//        delete FormEvent;
//    }
//    delete FormEventsList;
//    for (int i = 0; i < Count; ++i) {
//        delete static_cast<PCellValueAtr>(Objects[i]);
//    }
//
//    if (StyleOforml != nullptr) {
//        delete StyleOforml;
//    }
//}
//
//void TFormatStyle::setToNull() {
//    align_vert = 0;
//    align_gor = 0;
//    font = "";
//    h = 0;
//    is_bold = 0;
//    isOrientation = 0;
//    FontColor = 0;
//    fonColor = 16777215; // white
//    borderStyle = 0;
//    borderColor = 0;
//    NumberFormat = "General";
//}
//
//void TFormatStyle::setTo(string aFont, byte aH, byte aIs_bold) {
//    setToNull();
//    font = aFont;
//    h = aH;
//    is_bold = aIs_bold;
//}
//
//int TCol1_Col2::getCol1(int aColAll) {
//    TCol1Col2 rec;
//    rec.colall = aColAll;
//    return rec.col1;
//}
//
//int TCol1_Col2::getCol2(int aColAl2) {
//    TCol1Col2 rec;
//    rec.colall = aColAll;
//    return rec.col12;
//}
//
//int TCol1_Col2::getColAll(int aCol1, int aCol2) {
//    TCol1Col2 rec;
//    rec.col1 = aCol1;
//    rec.col2 = aCol2;
//    return rec.colall;
//}
//
//void SetToNull1(int& aRzdMergeN, std::string& aRzdfont, int& aRzdh, int& aRzdis_bold,
//                int& aRzdfontColor, int& aRzdFonColor, int& aRzdVerAlign, int& aRzdHorAlign) {
//    aRzdMergeN = 1;
//    aRzdfont = "";
//    aRzdh = 0;
//    aRzdis_bold = 0;
//    aRzdfontColor = 0;
//    aRzdFonColor = 0;
//    aRzdVerAlign = 0;
//    aRzdHorAlign = 0;
//}
//
//void TStyleOforml::SetToNull() {
//    SetToNull1(Rzd1MergeN, Rzd1font, Rzd1h, Rzd1is_bold, Rzd1fontColor, Rzd1FonColor, Rzd1VerAlign, Rzd1HorAlign);
//    SetToNull1(Itg1MergeN, Itg1font, Itg1h, Itg1is_bold, Itg1fontColor, Itg1FonColor, Itg1VerAlign, Itg1HorAlign);
//
//    SetToNull1(Rzd2MergeN, Rzd2font, Rzd2h, Rzd2is_bold, Rzd2fontColor, Rzd2FonColor, Rzd2VerAlign, Rzd2HorAlign);
//    SetToNull1(Itg2MergeN, Itg2font, Itg2h, Itg2is_bold, Itg2fontColor, Itg2FonColor, Itg2VerAlign, Itg2HorAlign);
//
//    SetToNull1(Rzd3MergeN, Rzd3font, Rzd3h, Rzd3is_bold, Rzd3fontColor, Rzd3FonColor, Rzd3VerAlign, Rzd3HorAlign);
//    SetToNull1(Itg3MergeN, Itg3font, Itg3h, Itg3is_bold, Itg3fontColor, Itg3FonColor, Itg3VerAlign, Itg3HorAlign);
//
//    isVTomCisleKursiv = false;
//}
//
//void TExecuteSxemaIerarx::setNULL() {
//    PeredIerarx.clear();
//    for (int i = 0; i < 50; ++i) {
//        TIerarx& tierarx = IerarxS[i];
//        tierarx.PeredClass.clear();
//        tierarx.owner = 0;
//        tierarx.class_id = 0;
//        tierarx.field_id = 0;
//        tierarx.is_link = 0;
//        tierarx.is_ForObject = 0;
//        tierarx.level = 0;
//        tierarx.regimGenerac = 0;
//        tierarx.idsCikl = "";
//        tierarx.isMergeVert = 0;
//        tierarx.isNoOutOtsutstv = 0;
//        tierarx.isMergeHoriz = 0;
//        tierarx.is_filter2 = 0;
//        tierarx.ColOnFilter2 = 0;
//        tierarx.form_str_n = 0;
//        tierarx.PosleClass.clear();
//    }
//    PosleIerarx.clear();
//}

#include "FormTypes.h"


void TStyleOforml::SetToNull() {
    auto SetToNull1 = [](int& aRzdMergeN, std::string& aRzdfont, int& aRzdh, int& aRzdis_bold, int& aRzdfontColor,
                         int& aRzdFonColor, int& aRzdVerAlign, int& aRzdHorAlign) {
        aRzdMergeN = 1;
        aRzdfont = "";
        aRzdh = 0;
        aRzdis_bold = 0;
        aRzdfontColor = 0;
        aRzdFonColor = 0;
        aRzdVerAlign = 0;
        aRzdHorAlign = 0;
    };

    SetToNull1(Rzd1MergeN, Rzd1font, Rzd1h, Rzd1is_bold, Rzd1fontColor, Rzd1FonColor, Rzd1VerAlign, Rzd1HorAlign);
    SetToNull1(Itg1MergeN, Itg1font, Itg1h, Itg1is_bold, Itg1fontColor, Itg1FonColor, Itg1VerAlign, Itg1HorAlign);

    SetToNull1(Rzd2MergeN, Rzd2font, Rzd2h, Rzd2is_bold, Rzd2fontColor, Rzd2FonColor, Rzd2VerAlign, Rzd2HorAlign);
    SetToNull1(Itg2MergeN, Itg2font, Itg2h, Itg2is_bold, Itg2fontColor, Itg2FonColor, Itg2VerAlign, Itg2HorAlign);

    SetToNull1(Rzd3MergeN, Rzd3font, Rzd3h, Rzd3is_bold, Rzd3fontColor, Rzd3FonColor, Rzd3VerAlign, Rzd3HorAlign);
    SetToNull1(Itg3MergeN, Itg3font, Itg3h, Itg3is_bold, Itg3fontColor, Itg3FonColor, Itg3VerAlign, Itg3HorAlign);

    isVTomCisleKursiv = false;
}

