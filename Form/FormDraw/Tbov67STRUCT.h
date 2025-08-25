

#ifndef MWSRESTAPI_TBOV67STRUCT_H
#define MWSRESTAPI_TBOV67STRUCT_H

#include <cstdint>

struct Tbov67 {
    int32_t plan_id;
    int32_t pokaz_id;
    double f1_1;
    double f1_2_1;
    double f1_2_2;
    double f1_2_3;
    double f1_3;
    double f1_4;
    double snig_ek_byMer;
    double snig_ek_other;
    double fact_zarezerv;
    double ostatokLBO;
    double Itogorez;
    double ostatokLBOgrbs;
};

//Tbov67* Pbov67;

#endif //MWSRESTAPI_TBOV67STRUCT_H
