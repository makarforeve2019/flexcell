#ifndef DB_POKAZTL_H
#define DB_POKAZTL_H

#include <string>
#include <vector>
#include "DB_Common.h"

const std::string CastNameTBSmets = "Горбюджет: Смета расходов";
const std::string CastNameTBSmetsLimiti = "Горбюджет: Смета расходов. Лимиты";
const std::string CastNameTBMrpr = "Перечень мероприятий ТБ";
const std::string CastNameReestrDgvrs = "Реестр договоров";
const std::string CastNameReestrDgvrsTB = "Реестр договоров по традиционному бюджету";
const std::string CastNameTBPostupl = "Горбюджет: Поступления";

// это части для бюджетной росписи
const std::string CastNameFD_BA = "Федбюджет: БА";
const std::string CastNameFD_LBO = "Федбюджет: ЛБО";
const std::string CastNameFD_POF = "Федбюджет: ПОФ";
// это части для сводной бюджетной росписи
const std::string CastNameFD_SVBA = "Федбюджет: СвБР БА";
const std::string CastNameFD_SVLBO = "Федбюджет: СвБР ЛБО";
const std::string CastNameFD_SVPOF = "Федбюджет: СвБР ПОФ";

struct TLinkClasses {
    int pokaz_id;
    int class_id;
    std::string class_name;
};

typedef TLinkClasses TArrLinkClasses[50];
typedef TArrLinkClasses* PArrLinkClasses;

static int FieldidToMeropr = 0;


class TDB_Pokaztl {
public:
    static void ProcessPokazId(int pokaz_id, int plan_id);
    static std::string getNameEntity(int pokaz_is_razd, int typeEntity);
    static std::string getNameEntityRP(int pokaz_is_razd, int typeEntity);
    static int getTypeEntity(int pokaz_id);
    static void updTypeEntity(int pokaz_id, int typeEntity);
    static void GetSostCasts(TQueryUni *qIn, int VidCast);
    static void GetAllSostCasts(TQueryUni *qIn, int VidCast);
    static int AddSostCast(int VidCast, std::string pokazName, int pokaz_owner_id,
       int pokaz_beg_date, int pokaz_end_date, std::string Primecanie);
    static int AddSostCastBK(int VidCast, std::string pokazName, int pokaz_owner_id,
       int pokaz_beg_date, int pokaz_end_date, int edizm_id, int pokaz_is_god, int pokaz_is_kv);
    static int AddCastClass(int VidCast, std::string pokazName, int pokaz_owner_id, int pokaz_beg_date,
       int pokaz_end_date, int level_sign, int pokaz_vert, std::string vert_str, int is_list, int pokaz_is_god);
    static int AddCastSetOfStr(int VidCast, std::string pokazName, int pokaz_owner_id, int pokaz_beg_date,
       int pokaz_end_date, std::string ListTables, std::string PredicatSelect, std::string Primecanie);
    static void AddParentId(int class_id, int parent_id);
    static void ReNameSostCast(int pokaz_id, std::string pokazName,
       int pokaz_beg_date, int pokaz_end_date, std::string Primecanie);
    static void UpdCastClass(int pokaz_id, std::string pokazName, int pokaz_beg_date,
       int pokaz_end_date, int level_sign, int pokaz_vert, std::string vert_str, int is_list, int pokaz_is_god);
    static void UpdCastSetOfStr(int CastSetId, std::string pokazName, int pokaz_beg_date, int pokaz_end_date,
      std::string ListTables, std::string PredicatSelect, std::string Primecanie);
    static bool IsSostCastEmpty(int pokaz_id);
    static bool GetPokaz(TQueryUni *qIn, int pokaz_id);
    static bool isExist(int pokaz_id);
    static bool GetPokazByOgr(TQueryUni *qIn, int pokaz_id, TDateTime date_beg, TDateTime date_end, int plan_id);
    static bool GetPokazForCell(TQueryUni *qIn, int cell_id);
    static std::string GetCodeAU(int pokaz_id);
    static std::string GetCodeBK(int pokaz_id);
    static std::string GetCodeBK_rzdltl(int pokaz_id, int god);
    static int GetTypePokaz(int pokaz_id);
    static int GetPokazIsRazd(int pokaz_id);
    static std::string GetPokazCode(int pokaz_id);
    static std::string GetPokazName(int pokaz_id);
    static std::string GetPokazName_EdIzm(int pokaz_id, int &edizm_id);
    static int GetPokazOwnerId(int pokaz_id);
    static bool GetRootPokaztl(int aPokaz_id, int &wPokaz_id, std::string &wPokaz_name,
                               int &wPokaz_is_razd, int &wPokaz_type, int &wPokaz_vert);
    static int GetEnumId(int pokaz_id);
    static int GetInCast(int pokaz_id);
    static int GetPokazVert(int pokaz_id);
    static void GetVert_Level(int class_id, int &pokaz_vert, std::string &vert_str, int &level_sign);
    static void GetBegEndLevel(int pokaz_id, TDateTime &pokaz_beg_date, TDateTime &pokaz_end_date, int &level_sign);
    static void SetBegEnd(int pokaz_id, TDateTime pokaz_beg_date, TDateTime pokaz_end_date);
    static void GetIs_List(int pokaz_id, int &is_list);
    static void SetIs_List(int pokaz_id, int is_list);
    static int GetClassId(int pokaz_id);
    static void UpdClassId(int pokaz_id, int objekt_class_id);
    static int GetRootCastId(int pokaz_id);
    static int GetKBK_Id(int pokaz_id);
    static int GetPrevPokazId(int pokaz_id);
    static int GetPokazIdInCast(int aPokaz_id);
    static int GetPokazIdInCast1(int aPokaz_id);
    static std::string GetCastName(int pokazId);
    static std::string GetPrimecanie(int pokaz_id);
    static void SetPrimecanie(int pokaz_id, std::string Primecanie);
    static void SetTableColumnNames(int pokaz_id, std::string tableName, std::string columnName);
    static int GetIsSurgtName(int pokaz_id);
    static void UpdIsSurgtName(int pokaz_id, int isSurgtName);
    static int AddPart(int subject_id, std::string pokaz_name, int pokaz_owner_id,
          TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign, int in_cast);
    static void UpdPart(int pokaz_id, std::string pokaz_name, TDateTime pokaz_beg_date,
          TDateTime pokaz_end_date, int level_sign);
    static void UpdPartBK(int pokaz_id, std::string pokaz_bk_code);
    static int AddParam(bool isItogo, int subject_id, std::string pokaz_name,
          int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
          std::string pokaz_code, std::string pokaz_bk_code, int pokaz_type, int enum_id,
          int edizm_id, TDateTime pokaz_beg_date, TDateTime pokaz_end_date,
          int level_sign, std::string vert_str, int is_list, int in_cast);
    static int AddParamOnlyPokaztl(std::string pokaz_name,
          int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
          std::string pokaz_code, std::string pokaz_bk_code, int pokaz_type, int enum_id,
          int edizm_id, TDateTime pokaz_beg_date, TDateTime pokaz_end_date,
          int level_sign, std::string vert_str, int is_list, int in_cast);
    static void UpdParam(int pokaz_id, std::string pokaz_name,
          int pokaz_vert, std::string pokaz_code, std::string pokaz_bk_code, int enum_id, int edizm_id,
          TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
          std::string vert_str);
    static int AddObjekt(bool isItogo, int subject_id, std::string pokaz_name,
          int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
          std::string pokaz_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
          std::string vert_str, int objekt_class_id, int in_cast, std::string pokaz_bk_code);
    static int AddOnlyObjekt(std::string pokaz_name,
          int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
          std::string pokaz_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
          std::string vert_str, int objekt_class_id, int in_cast, std::string pokaz_bk_code);
    static int AddObjekt2(bool isItogo, int subject_id, std::string pokaz_name,
          int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
          std::string pokaz_bk_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
          std::string vert_str, int objekt_class_id, int in_cast);
    static void UpdObjekt(int pokaz_id, std::string pokaz_name,
          int pokaz_vert, std::string pokaz_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
          std::string vert_str, std::string pokaz_bk_code);
    static void UpdObjekt2(int pokaz_id, std::string pokaz_name, int pokaz_vert, std::string pokaz_bk_code,
          TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign, std::string vert_str);
    static void UpdOwnerId(int plan_id, int pokaz_id, int pokaz_owner_id);
    static void UpdLevelText(int pokaz_id, std::string level_text);
    static std::string GetLevelText(int pokaz_id);
    static void UpdlevelSign(int pokaz_id, int level_sign);
    static void UpdPokazName(int pokaz_id, std::string pokaz_name);
    static void UpdcreateInUpd(int pokaz_id, int createInUpd);
    static int GetCreateInUpd(int pokaz_id);
    static void UpdCodeBK(int pokaz_id, std::string pokaz_bk_code);
    static void UpdPokazVertTo1(int pokaz_id);
    static void UpdPokazVertTo3(int pokaz_id);
    static void UpdTypePokaz(int pokaz_id, int pokaz_type);
    static void UpdPokazVert(int pokaz_id, int pokaz_vert);
    static void UpdPokaz_is_kv(int pokaz_id);
    static void UpdPokaz_is_mes(int pokaz_id, int kv);
    static int getPokaz_vsego_id(int pokaz_id);
    static void UpdPokaz_vsego_id(int pokaz_id, int pokaz_vsego_id);
    static void UpdOrtIzm(int pokaz_id, int pokaz_is_mes, int pokaz_is_kv,
          int pokaz_is_god, int pokaz_is_ist_fin, int pokaz_is_din_izm, int pokaz_is_const);
    static void GetOrtIzm(int pokaz_id_, int& pokaz_is_mes, int& pokaz_is_kv,
          int& pokaz_is_god, int& pokaz_is_ist_fin, int& pokaz_is_din_izm, int& pokaz_is_const);
    static void UpdOrtIzm1(int pokaz_id, int pokaz_is_ist_fin);
    static void UpdEdIzm_PokazNP(int pokaz_base_id, int np_vid_id, int edizm_id);
    static std::string GetforUnikIdentif(int pokaz_id);
    static int findByUnikIdentif(std::string forUnikIdentif);
    static int findByUnikIdentif(std::string forUnikIdentif, int plan_id, int god);
    static int findByUnikIdentifGod(std::string forUnikIdentif, int god);
    static void UpdforUnikIdentif(int pokaz_id, std::string forUnikIdentif);
    static void UpdforMultipartDgvr(int dgvr_id, int objectid1, int is_list);
    static void UpdforMultiItogo(int pokaz_id, std::string forMultiItogo, int objekt_class_id);
    static bool isInforMultiItogo(int Itogo_idMulti, int field_id);
    static int getIsListForDgvr(int objectid, int& enum_id);
    static int AddNaturP(int pokaz_base_id, int pokaz_is_razd, int pokaz_type,
                   int enum_id, int edizm_id, int np_vid_id, int in_cast);
    static bool DeletePokaztl(int pokazId, int pokazOwnerId, bool isMsg);
    static bool IsklucitPokaztl(int plan_id, int pokazId, int pokazOwnerId);
    static void DelPokaztl(int aPokazId);
    static void IsklPokaztl(int aPlan_id, int aPokazId);
    static void GetNaturP(TQueryUni *qIn, int pokaz_base_id, int np_vid_id);
    static void Set_level_text(int plan_id, int owner_id, int pokaz_id, int level_sign);
    static bool UnikName(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name);
    static bool UnikName2(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name, int edizm_id);
    static bool UnikNameIzm(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name, int pokaz_id);
    static bool UnikNameIzm2(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name, int pokaz_id,
        int edizm_id);
    static int GetId();
    static int GetIdbyName(int pokaz_owner_id, std::string pokaz_name);
    static TDateTime GetDateCreate(int pokaz_id);
    static int GetIdbyNameYear(int pokaz_owner_id, std::string pokaz_name, int CurrYear);
    static int GetIdClass(std::string class_name, bool isMSG);
    static int GetIdNP(int pokaz_base_id, int np_vid_id);
    static int GetIdNP_BP(int pokaz_base_id, int np_vid_id);
    static int getNp_vid_id(int pokaz_id);
    static void updNp_vid_id(int pokaz_id, int newNp_vid_id);
    static bool IsBaseClass(int pokaz_id);
    static bool IsSubItem(int pokaz_id);
    static int GetLinkClass(int class_id, PArrLinkClasses LinkClasses);
    static void GetLinkClass1(int pokaz_id, int& N, PArrLinkClasses pLinkClasses);
    static int getPeriodicn(int pokaz_id);
    static void SetNewEndYear(int pokaz_id, int Year);
    static void SetKeyField(int pokaz_id, int class_id);
    static void SetKeyFieldAvtoNumber(int pokaz_id, int class_id);
    static void UnSetKeyField(int pokaz_id);
    static void SetFieldToItogo(int pokaz_id, int class_id);
    static void UnSetFieldToItogo(int pokaz_id);
    static void SetFieldToAgregator(int pokaz_id, int class_id);
    static void SetFieldToAgregatorMrpr(int pokaz_id, int class_id);
    static void UnSetFieldToAgregator(int pokaz_id);
    static int findFieldToItogo(int class_id);
    static int GetPokazIdbyPokazBkCode(std::string pokaz_bk_code, int CurrYear, std::string aCastName);
    static void CreateTmpPokaztlTB(int CurrYear, std::string aCastNameTB);
    static void DropTmpPokaztlTB();
    static int GetIdByKBKinTmpPokaztlTB(std::string KBK);
    static int GetIdItogoRazd(int pokaz_id_razd);
    static bool isTBCastLimit(int aPokaz_id);
    static int isTBCast_Limit_OrNo(int aPokaz_id);
    static bool isTBCastLimit0(int cast_id);
    static bool isTBCast(int aPokaz_id);
    static bool EstLiPokazBKCode(int pokaz_owner_id, std::string pokaz_bk_code, int god);
    static int getSvobodnijPokazId();
    static void AddIdSvob(bool isItogo, int aPokaz_id, int apokaz_owner_id, int aVidCast, int apokaz_is_razd,
           int pokaz_type, int is_list);
    static int GetFieldidToMeropr(int aClass_id);
    static std::string evalSetPolnijNomer(int plan_str_owner_id, int plan_id, int pokaz_id);
    static void UpdidSlagaemoeForItogo(int pokaz_id, int field_id);
    static void UpdIdsOwnerFieldItogo(int pokaz_id, std::string idsOwnerFieldItogo);
    static void UpdidItogoForSlagaemoer(int pokaz_id, int field_id);
    static int getUpParentByClassId(int currObjectId, int class_id_UpParent);
    static void updSystFieldObj(std::string stmnt, int ObjectId, int pokaz_id, int systField_id, std::string value);
    static void updPokazCode(int pokaz_id, std::string pokaz_code);
    static void Upd_PokazBaseId(int pokaz_id, int pokaz_base_id);
    static int Get_PokazBaseId(int pokaz_id);
    static int GetObjectIdByCellValueStr(std::string pokaz_name, std::string field_name, std::string cell_value);
    static int GetObjectIdByCellValueInt(std::string pokaz_name, std::string field_name, int cell_value);
    static TDictionary<std::string,std::string> GetCellValuesByObjectId(int object_id);
    static void CreateTmpPokaztlTBbyId(int CurrYear, int root_Pokaz_id);
    static int getLengthKBKforSubPokaz(int pokaz_owner_id);
};

#endif // DB_POKAZTL_H
