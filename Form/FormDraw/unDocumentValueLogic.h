#ifndef DocumentValueLogicH
#define DocumentValueLogicH

#include "FormTypes.h"


class TDocumentValueLogic {
public:
    static bool GetDocumid(int ObjectId, int CellId, int PlanId, int VidOtch, const TDateTime ValueDate, std::string& Docum_id);
    static bool GetDocumid(std::string& Docum_id, int PokazId, int ComnpId, int VidPeriod, int God, int Kvrtl, int Mesjc, int Den, int PlanId, int VidOtch);
    static bool GetDocumRelation(std::string Docum_id,
                                 int& ObjectId, int& CellId, int& PlanId, int& VidOtch, TDateTime& ValueDate,
                                 int& PokazId, int& ComnpId, int& VidPeriod, int& God, int& Kvrtl, int& Mesjc, int& Den);
};
#endif