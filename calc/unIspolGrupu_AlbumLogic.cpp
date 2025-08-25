#include "unIspolGrupu_AlbumLogic.h"
#include "SystFuncsLogic.h"
//#include "dmGlobalVars.h"
#include "unDefDBconnect.h"
#include "DB_AccessAlbum.h"
#include "DB_Album.h"
#include "DB_Plan.h"

void TIspolGrupu_AlbumLogic::evalPlanIds(int aAlbum_id, int& countPlanIds, std::vector<int>& planIds) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    countPlanIds = 0;
    TDB_AccessAlbum::GetAccessAlbum(q, aAlbum_id);
    if (q->EoF())
        return;

    if (q->FieldByName("plan_id")->AsInteger() < 0) {
        aAlbum_id = -q->FieldByName("plan_id")->AsInteger();
        TDB_AccessAlbum::GetAccessAlbum(q, aAlbum_id);
    }

    while (!q->EoF()) {
        countPlanIds++;
        q->Next();
    }

    planIds.resize(countPlanIds); // Изменяем размер вектора

    q->First();
    int n = 0;
    while (!q->EoF()) {
        planIds[n] = q->FieldByName("plan_id")->AsInteger();
        n++;
        q->Next();
    }

    delete q;
}