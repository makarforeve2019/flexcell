//
// Created by Alex on 29.04.2024.
//

#ifndef MWSRESTAPI_TRCRDFM_H
#define MWSRESTAPI_TRCRDFM_H



struct TrcrdFM {
    int cell_id;
    int cell_period;
    int cell_is_eval;
    int ist_fin_id;
    int plan_id;
    int objectid;
    int pokaz_id;
    int vid_otc;
    int god;
    int nMesjc;
    int day;
    double valueUtCB;
    double valueUtTB;
    double valueUtCS;
    double valueFcCB;
    double valueFcTB;
    double valueFcCS;
};

using PrcrdFM = TrcrdFM*;

#endif //MWSRESTAPI_TRCRDFM_H
