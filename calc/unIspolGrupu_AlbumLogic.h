#ifndef UNISPOLGRUPU_ALBUMLOGIC_H
#define UNISPOLGRUPU_ALBUMLOGIC_H

#include "../DB/DB_Common.h"
#include "../Form/FormTypes.h"

class TIspolGrupu_AlbumLogic {
public:
    static void evalPlanIds(int aAlbum_id, int& countPlanIds, std::vector<int>& planIds);
};


#endif // UNISPOLGRUPU_ALBUMLOGIC_H
