//
// Created by Alex on 01.05.2024.
//

#ifndef MWSRESTAPI_TBOV15STRUCT_H
#define MWSRESTAPI_TBOV15STRUCT_H
#include <string>

struct Tbov15 {
    std::string KBK;
    std::string unc;
    double dBO15;
    double dKassRasxBO15;
    double summaNaGod15;
    double ostatokFree15;
    double oplacenoAll;
};

typedef Tbov15* Pbov15;
#endif //MWSRESTAPI_TBOV15STRUCT_H
