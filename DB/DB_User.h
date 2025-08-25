#ifndef DB_UserH
#define DB_UserH

#include "DB_Common.h"
#include <string>
#include <vector>

const int tables_num = 73;

inline std::vector<std::string> tables_all = {
    "AccessAlbum", "Album", "Album_form", "bkMetaDef", "bkKBK", "Budget_poluc", "Budget_poluc_rajon",
    "Chart", "Class_ofpart", "Cell", "Cell_value", "Cell_value_bp", "DateUtvPlans", "Denegn_potok",
    "Din_izm", "Docums", "Edizm", "Enum", "Form_str", "Forms", "Formula", "Import_pokaz",
    "Influence", "Ist_fin", "KeyOfObjects", "KollectPrdlg", "KollectPrdlgMmbr", "LinksObjects", "Nat_pokaz_vid",
    "NeOdnoznPlategka", "Payers", "Plan_str", "Plans", "Podpisi", "Pokaz_din_izm", "Pokaztl", "PrdlgMmbr", "PrdlgUP", "Rukovs",
    "Series", "SeriesProp", "Uii", "User_plan", "User_userctgr", "Userctgr", "Users", "Utoc_plana",
    "GrpsCtgrs", "GurnUserSesii", "ProtokolUserWorks", "VidProdukcii", "GrnlUtocKP", "UtocKP", "StrPodrsd", "UnikNmbrDgvr",
    "Vedomstvo", "HelpSource", "HelpSource_stroki", "ComonNPokaz", "CNP_value", "OKDP", "OKVED", "VidIzmSBR",
    "proc_SelCells","proc_SetValue","CreateTmpTblPokaztl", "DelCopyValuesByRoot",
    "GetCountValuesByRoot", "GetRootPokaztl", "GetValueItogoMesjc", "CreateTmpPokaztlTB", "CreateTmpPokaztlDoxodCBF", ""
};

class TDB_User {
public:

    static bool RegistryUserADByPassword(int UserId, std::string aPassWord, int &LastYear, int &Active, bool isAD, TDateTime &ChangePassDate);
    static void ResetLockTime(int user_id);
    static void ResetLoginAttempts(int user_id);
    static bool GetUserInfo(TQueryUni *qIn, std::string user_name, std::string user_password);
    static std::string GetUserPlanName(std::string AUserName);
    static int GetId();
    static void GetAllUser(TQueryUni *qIn);
    static void GetUser(TQueryUni *qIn, int user_id);
    static std::string GetPasswordSysadm(std::string &user_name);
    static std::string GetspisokRazd(int user_id);
    static int GetUserAdm(int user_id);
    static std::string GetUserPassword(int user_id);
    static std::string GetUserFIOshort(int user_id);
    static bool GetUserFamImjOtc(int user_id, std::string &Fam, std::string &Imj, std::string &Otc);
    static void GetUserTlfns(int user_id, std::string &TELmestn, std::string &TELgorod, std::string &TELmobil, std::string &FIOshort, std::string &dolgnost);
    static bool IsUserLocked(int user_id);
    static void LockUser(int user_id, int LockDurationMinutes);
    static int GetUserId(std::string user_name);
    static std::string GetUserName(int user_id);
    static int GetIsECP(int user_id);
    static TDateTime GetLockTime(int user_id);
    static int GetLoginAttempts(int user_id);
    static int AddUser(std::string user_name, std::string user_password, std::string user_comm, int user_adm, std::string spisokRazd, std::string FIOshort, std::string TELmestn, std::string TELgorod, std::string TELmobil, std::string signal_codes, std::string email);
    static void UpdUser1(int user_id, std::string user_comm, std::string spisokRazd, std::string FIOshort, std::string TELmestn, std::string TELgorod, std::string TELmobil, std::string signal_codes, std::string email);
    static void UpdUser2(int user_id, int user_adm);
    static void UpdUser3(int user_id, std::string user_password);
    static void UpdUser4(int user_id, int LastYear);
    static void UpdIsECP(int user_id, int isECP);
    static void UpdIsGRBSDefaultOwner(int user_id, int isGRBSDefaultOwner);
    static void UpdPassword(int user_id, std::string Password);
    static void UpdateLoginAttempts(int user_id, int LoginAttempts);
    static void UpdChangePassDate(int user_id, TDateTime value_date);
    static void UpdDolgnost(int user_id, std::string dolgnost);
    static void DelUser(int user_id);
    static void UserGrantall(std::string aLogin);
    static int GetUserByProfileID(int profileid);
    static bool SaveUserToken(int user_id, std::string accessToken, TDateTime expDate);
    static bool GetUserTokenByProfileID(int profileid, std::string& accessToken, TDateTime& expDate);
    static bool GetUserProfileIDByToken(std::string accessToken, int& user_id, std::string& profileid, TDateTime& expDate);

    };

#endif
