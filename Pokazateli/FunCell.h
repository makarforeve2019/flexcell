#ifndef MWSRESTAPI_FUNCELL_H
#define MWSRESTAPI_FUNCELL_H

#include <iostream>
#include <string>
#include <vector>
#include "unDefDBconnect.h"

// ��������� TCell
struct TCell {
    int ist_fin_id;
    std::string cell_din_izm;
    int period;
};

// ���������� ���������� Cell
extern TCell Cell;

// ���������� ������� FillCell
void FillCell(int pokaz_id, int ist_fin_idOnly = 0);

// ���������� ��������������� ������� AddTmpCell
void AddTmpCell(int pokaz_id, int ist_fin_id, const std::string& cell_din_izm, int cell_period, int cell_is_eval);

#endif // MWSRESTAPI_FUNCELL_H
