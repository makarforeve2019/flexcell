
#ifndef DB_CellH
#define DB_CellH



#include "DB_Common.h"


class TDB_Cell
{
public:
    static void AddCell(int pokaz_id, int ist_fin_id, std::string cell_din_izm,
                        int cell_period, int cell_is_eval);
    static void Updcell_is_evaTo1(int pokaz_id);
    static void Updcell_is_evalIskluc1(int pokaz_id);
    static void Updcell_is_eval1(int pokaz_id);
    static void Updcell_is_eval2(int pokaz_id, int kv);
    static void Updcell_is_eval3(int pokaz_id);
    static void Updcell_is_evalTo0(int cell_id);
    static void Updcell_is_evalTo(int cell_id, int cell_is_eval);
    static void DelForPokazId(int pokaz_id);
    static int GetCellId(int pokaz_id, int cell_period, int ist_fin_id);
    static int GetCellId1(int pokaz_id, int cell_period, int ist_fin_id, int &cell_is_eval);
    static int GetCellId2(int pokaz_id, int cell_period, int ist_fin_id, std::string cell_din_izm);
    static void GetCell_idIstFin(TQueryUni *qIn, int Pokaz_id, int Cell_id);
    static void GetCell_id(TQueryUni *qIn, int Pokaz_id, int Cell_id);
    static void GetAllCell_id(TQueryUni *qIn, int Pokaz_id);
    static void GetInfCell_id(TQueryUni *qIn, int Cell_id);
    static void eval2_3by1(int Cell_Id, int &cell_id2, int &cell_id3, int &cell_id4);
    static int GetIstFinbyCell_id(int Cell_id);
    static int GetPokaz_idbyCell_id(int Cell_id);
    static void GetInfobyCell_id(int Cell_id, int &ist_fin_id, int &cell_period, int &pokaz_id, int &cell_is_eval);
    static int getSvobodnijCellId();
    static void AddIdSvobod(int aCell_id, int aPokaz_id, int acell_period);
};

#endif
