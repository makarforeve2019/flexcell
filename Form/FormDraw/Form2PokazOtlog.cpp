


#include "Form2PokazOtlog.h"

void TForm2PokazOtlog::doForm2PokazOtlog(bool aIs_sort, int aOwnerNomRow, int newCurrPlanId, int saveCurrPlanId, 
                                        std::string strName, std::string note2, std::string shablon, std::string oldshablon,
                                        int level_sign, bool Is_open, bool readonly, bool is_hidden, bool is_filter2, int ColOnFilter2, bool noPrint, bool is_filter, int nStolbec,
                                        std::string font, unsigned char h, unsigned char is_bold,
                                        unsigned char Align_vert, unsigned char Align_gor, PDopAtrCol aDopAtrCol,
                                        TQueryUni *q) {
    std::vector<int> planIds;
    int countPlanIds = 0;
    TIspolGrupu_AlbumLogic::evalPlanIds(FormLogicF1->CurrAlbumId, countPlanIds, planIds);


    for (int i = 0; i < countPlanIds; i++) {
        int wplan_id = planIds[i];
        aDopAtrCol->is_planId = true;
        aDopAtrCol->plan_id = wplan_id;

        CurrPokazId = InfoFormLogic->OtlogenPokazId;
        CurrObject.pokaz_id = InfoFormLogic->OtlogenPokazId;
        CurrObject.pokaz_is_razd = TDB_Pokaztl::GetPokazIsRazd(CurrObject.pokaz_id);
        TForm2PokazKorteg(this).doForm2PokazKorteg(aIs_sort, aOwnerNomRow, newCurrPlanId, saveCurrPlanId,
                                                   strName, note2, shablon, oldshablon,
                                                   level_sign, Is_open, readonly, is_hidden, is_filter2, ColOnFilter2, noPrint, is_filter, nStolbec,
                                                   font, h, is_bold, Align_vert, Align_gor, aDopAtrCol, q);
    }
}
