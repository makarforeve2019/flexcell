#ifndef DB_UtocKP_H
#define DB_UtocKP_H


#include "FormDopInp.h"
#include "DB_Common.h"
#include "../Form/FormTypes.h"
#include "unFormLogicF1.h"


class TDB_UtocKP {
public:
    static int GetIdGrnlUtocKP();
    static int GetIdUtocKP();
    static int addZagUtocKP(int god, int cast_id, std::string nomerDok, TDateTime date_dok);
    static void updZagUtocKP(int grnlutkp_id, std::string nomerDok, TDateTime date_dok, std::string obosnov, TDateTime fromData, TDateTime byData, int prdlg_id);
    static void UpdDateUpdDB(int grnlutkp_id, TDateTime date_UpdDB);
    static void UpdObosnov(int grnlutkp_id, std::string obosnov);
    static void UpdId_FromPref(int grnlutkp_id, int id_FromPref);
    static int GetId_FromPref(int grnlutkp_id);
    static void GetZagsUtvUtocKP(TQueryUni *qIn, int god);
    static void GetAllUtocKP(TQueryUni *qIn, int god);
    static void GetAllUtocKPDesc(TQueryUni *qIn, int god);
    static void GetAllUtocKPDescSPO(TQueryUni *qIn, TDateTime s, TDateTime po);
    static bool UnikNomDok(int grnlutkp_id, std::string nomerDok);
    static void DelUtocKP(int grnlutkp_id);
    static void DelUtocKpPOF(int prdlg_id);
    static void DelZagUtocKP(int grnlutkp_id);
    static void DelAllDetStr(int grnlutkp_id);
    static int getGrnlutkpIdForPOF(int prdlg_id);
    static void GetZagUtocbyID(TQueryUni *qIn, int grnlutkp_id);
    static void GetNDokDateDok(int grnlutkp_id, std::string &nomerDok, TDateTime &date_dok);
    static std::string GetObosnov(int grnlutkp_id);
    static void GetAllDetStr(TQueryUni *qIn, int grnlutkp_id);
    static void CopyAllDetStr(int grnlutkp_id, int grnlutkp_idNew);
    static int GetUtocIDbyNomDok(int god, std::string nomerDok);
    static int GetSostUtoc(int grnlutkp_id);
    static int GetPrdlgId_IsMetodSosdanij(int grnlutkp_id, int &isMetodSosdanij, TDateTime &fromData, TDateTime &byData);
    static void SetSostUtoc(int grnlutkp_id, int sost, TDateTime DateSost);
    static void UpduserName(int grnlutkp_id, std::string userName);
    static double GetUtKassPl_pdg(PCellValueAtr vd);
    static double GetUtKassPl_utv(TFormLogicF1 *FormLogicF1, PCellValueAtr vd);
    static int GetId_pdg(int god);
    static int FindByPrdlgId(int prdlg_id, int &sost, std::string &nomerDok, TDateTime &date_dok);
    static void updIsMetodSosdanij(int grnlutkp_id, int isMetodSosdanij);
    static int addUtocKP(int grnlutkp_id, int plan_id, int god, int kv, int pokaz_id, int cell_id, int vid_otc, double summa);
    static void updGrnlutkpId_InUtoc(int grnlutkp_idFrom, int grnlutkp_idTo);
    static void updUtocKP_bysumma(int utockp_id, double summa);
    static void saveUtValue(PCellValueAtr vd, int grnlutkp_id, double value);
    static int addShablonFormUKP(int god, int Form_id_shablon);
    static void updShablonFormUKP(int grnlutkp_id, int Form_id_shablon);
    static int selShablonFormUKP(int god, int &grnlutkp_id);
    static void Set_1_Bit(int grnlutkp_id);
    static void ReSet_1_Bit(int grnlutkp_id);
    static bool Get_1_Bit(int grnlutkp_id);
    static void getUtocKpByPrdlgId(TQueryUni *qIn, int prdlg_id);
};

#endif // DB_UtocKP_H
