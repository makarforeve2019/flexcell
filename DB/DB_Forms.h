#ifndef DB_FormsH
#define DB_FormsH

#include "DB_Common.h"

class TDB_Forms {
public:
    static int GetId();
    static void GetAllFormAlbum(TQueryUni *qIn, int album_id, TDateTime byDate);
    static void Get1FormAlbum(TQueryUni *qIn, int album_id, int form_id, TDateTime byDate);
    static void Get1FormWithoutAlbum(TQueryUni *qIn, int album_id, TDateTime byDate);
    static bool getNextFormAtr(int album_id, int form_idCur, int form_orderCur, int &form_idNext, int &form_orderNext, TDateTime byDate);
    static bool GetForm(TQueryUni *qIn, int form_id, bool isMsg);
    static bool isFormExist(int form_id);
    static int GetVidForm(int form_id);
    static int GetWorkWith(int form_id);
    static int GetFormK2(int form_id);
    static std::string GetNameForm(int form_id);
    static int GetFormIdByObozn(std::string form_obozn, int album_id);
    static int GetPlanIdForm(int form_id);
    static void GetAllForm(TQueryUni *qIn, int userctgr_id);
    static void GetAllAlbForm(TQueryUni *qIn, int userctgr_id);
    static int AddForm(int plan_id, std::string form_obozn, std::string form_name, TDateTime form_beg_date, TDateTime form_end_date, int pokaz_id, int col_n, int form_readonly, int form_k1, int form_k2, int form_k_zagtop, int form_k_zagbottom, int is_dublir_itogo, int userctgr_id, int work_with, int class_id, int ogr_class_id, int for_class, int is_add, int is_copy, int is_sort, int vid_form, int is_edIzm, int NZamorog, int is_svod, int tocnost, int isLandscape, int isShowZero, std::string OgrUserOprcii, int isShapkaColor, int isLinesColor);
    static void UpdFormGrFin(int form_id, int work_with_gf);
    static void Upd_Work_with_gf(int form_id, int work_with_gf);
    static void Upd_Work_with(int form_id, int work_with);
    static void UpdForm(int form_id, std::string form_obozn, std::string form_name, TDateTime form_beg_date, TDateTime form_end_date, int pokaz_id, int col_n, int form_readonly, int is_dublir_itogo, int class_id, int ogr_class_id, int for_class, int is_add, int is_copy, int is_sort, int is_edIzm, int NZamorog, int is_svod, std::string name_svod, std::string formula_svod, int tocnost, int isLandscape, int isShowZero, std::string OgrUserOprcii, int isShapkaColor, int isLinesColor);
    static void UpdForm1(int form_id, std::string name_svod, std::string formula_svod);
    static void UpdForm2(int form_id, int plan_id);
    static void DelForm(int form_id);
    static void DelFormAll(int userctgr_id);
    static void UpdUserCtgr(int form_id, int userctgr_id);
    static bool UnikName(std::string form_obozn, int userctgr_id, int &form_id);
    static bool UnikNameIzm(std::string form_obozn, int userctgr_id, int form_id);
    static void UpdForm_vid_period(int form_id, int form_vid_period);
    static void UpdForm_k2(int form_id, int d);
    static void UpdForm_k1(int form_id, int d);
    static void UpdForm_k_zagtop(int form_id, int d);
    static void UpdForm_k_zagbottom(int form_id, int d);
    static void UpdSheetObozn(int form_id, std::string SheetObozn);
    static void UpdColorLockedCell(int form_id, int ColorLockedCell);
    static void getInfoNamePokazClmn(int form_id, int &aNamePokazClmn, bool &isHidden);
    static void SetK1(int form_id, int k1);
    static void SetK2(int form_id, int k2);
    static void GetK1K2(int form_id, int &k1, int &k2);
};

#endif
