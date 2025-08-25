#ifndef IDsClassFieldCellH
#define IDsClassFieldCellH


#include "../DB/DB_Common.h"

class TIDsClassFieldCell {
public:
    static bool getClassId(std::string class_name, int pokaz_type, bool isMSG, int& class_id);
    static bool getClassIdByCode(std::string pokaz_code, int pokaz_type, bool isMSG, int class_id);
    static bool getClassFieldId(int class_id, std::string field_name, TDateTime CurDate, bool isMSG, int& field_id);
    static bool getClassFieldIdByCode(int class_id, std::string pokaz_code, TDateTime CurDate, bool isMSG, int& field_id);
    static int getPokazCellId(int pokaz_id, int cell_period, int ist_fin_id);
    static int getIstFinId(std::string ist_fin_name);
    static int getCNPedizm(int comnp_id);
    static int getCNPtype(int comnp_id);
    static int getEdIzmCoef(int edizm_id);
    static std::string getNameEdIzm(int edizm_id);
    static void saveEdIzmInfo();
    static int getPlanIdBySinonim(std::string sinonim);
    static std::string getCodeCelSubsByIstFinId(int ist_fin_id);
    static bool findSubsidPokazCellId(std::string codeCelSubs, int& SubsidPokazId, int& SubsidCellIdKV, int& SubsidCellIdGod);
    static int GetIstFinIdBySubs(int pokaz_idSubsid);
    static bool findSummaDgvrSubsidCellId(int class_idZakupki, int SummaDgvrPokazId, int ist_fin_id, std::string codeCelSubs, int SubsidPokazId, int& SummaDgvrCellIdKV, int& SummaDgvrCellIdGod);
};
#endif