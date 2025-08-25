#include "FunCell.h"
#include <memory>
#include <stdexcept>
#include "unDefDBconnect.h"
#include "DB_Common.h"
#include "DB_Pokaztl.h"
#include "DB_IstFin.h"
#include "DB_Dinizm.h"
#include "SystFuncsLogic.h"
#include "unDBType.h"

// Определение переменной Cell
TCell Cell;

// Вставляет строку в #TmpCell
void AddTmpCell(int pokaz_id, int ist_fin_id, const std::string& cell_din_izm, int cell_period, int cell_is_eval) {
    TQueryUni *qCell = new TQueryUni(DefDBconnect.Conn);
    qCell->SQL->Clear();

    switch (GetDBType()) {
        case TDBType::MSSQL:
            qCell->SQL->Add("INSERT INTO #TmpCell (pokaz_id, ist_fin_id, cell_din_izm, cell_period, cell_is_eval) "
                            "VALUES (:pokaz_id, :ist_fin_id, :cell_din_izm, :cell_period, :cell_is_eval)");
            break;
        case TDBType::Postgre:
            qCell->SQL->Add("INSERT INTO TmpCell (pokaz_id, ist_fin_id, cell_din_izm, cell_period, cell_is_eval) "
                            "VALUES (:pokaz_id, :ist_fin_id, :cell_din_izm, :cell_period, :cell_is_eval)");
            break;
    }
    qCell->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qCell->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    qCell->ParamByName("cell_din_izm")->AsString() = cell_din_izm;
    qCell->ParamByName("cell_period")->AsInteger() = cell_period;
    qCell->ParamByName("cell_is_eval")->AsInteger() = cell_is_eval;
    qCell->ExecSQL();
    delete qCell;
}

void FillCell(int pokaz_id, int ist_fin_idOnly) {
    std::vector<int> IstFinList;
    std::vector<int> PeriodList;
    int countLast, PokazBaseId;
    int pokaz_is_const, pokaz_is_mes, pokaz_is_kv, pokaz_is_ist_fin, pokaz_is_razd, pokaz_vert, pokaz_type, pokaz_base_id;
    int CellIsEvalGl = 0;

    TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
    TDB_Pokaztl::GetPokaz(qPokaz, pokaz_id);
    pokaz_is_const = qPokaz->FieldByName("pokaz_is_const")->AsInteger();
    pokaz_is_mes = qPokaz->FieldByName("pokaz_is_mes")->AsInteger();
    pokaz_is_kv = qPokaz->FieldByName("pokaz_is_kv")->AsInteger();
    pokaz_is_ist_fin = qPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger();
    pokaz_is_razd = qPokaz->FieldByName("pokaz_is_razd")->AsInteger();
    pokaz_vert = qPokaz->FieldByName("pokaz_vert")->AsInteger();
    pokaz_base_id = qPokaz->FieldByName("pokaz_base_id")->AsInteger();
    pokaz_type = qPokaz->FieldByName("pokaz_type")->AsInteger();
    PokazBaseId = pokaz_base_id;
    delete qPokaz;

    TQueryUni *qPDinIzm = new TQueryUni(DefDBconnect.Conn);
    TDB_Dinizm::GetPokazDinizm(qPDinIzm, pokaz_id);
    countLast = 2;
    while (!qPDinIzm->EoF()) {
        countLast++;
        qPDinIzm->Next();
    }
    qPDinIzm->First();

    if (pokaz_is_ist_fin == 1) {
        TQueryUni *qIstFin = new TQueryUni(DefDBconnect.Conn);
        TDB_IstFin::GetIstFin_All(qIstFin);
        while (!qIstFin->EoF()) {
            if ((ist_fin_idOnly > 0 && ist_fin_idOnly != qIstFin->FieldByName("ist_fin_id")->AsInteger()) ||
                !qIstFin->FieldByName("pokaz_id")->IsNull()) {
                qIstFin->Next();
                continue;
            }
            IstFinList.push_back(qIstFin->FieldByName("ist_fin_id")->AsInteger());
            qIstFin->Next();
        }
        delete qIstFin;
    } else {
        IstFinList.push_back(0);
    }

    PeriodList.push_back(0);
    if (pokaz_is_const != 1) {
        PeriodList.push_back(1);
        PeriodList.push_back(2);
        PeriodList.push_back(3);
    }

    Cell.ist_fin_id = 0;
    Cell.cell_din_izm = "";
    Cell.period = 0;

    if (pokaz_is_razd == 3 || pokaz_vert == 1) {
        CellIsEvalGl = 1;
    }
    if (PokazBaseId > 0) {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        TDB_Pokaztl::GetPokaz(q, PokazBaseId);
        if (q->FieldByName("pokaz_vert")->AsInteger() == 1 || q->FieldByName("pokaz_is_razd")->AsInteger() == 3) {
            CellIsEvalGl = 1;
        }
        delete q;
    }

    std::string S = "";

    TQueryUni *q2 = new TQueryUni(DefDBconnect.Conn);
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q2->SQL->Add("CREATE TABLE #TmpCell (pokaz_id int NULL, cell_period tinyint NULL, ist_fin_id int NULL,");
            q2->SQL->Add(" cell_din_izm char(50) NULL, cell_is_eval tinyint NULL)");
            break;
        case TDBType::Postgre:
            q2->SQL->Add("CREATE TEMP TABLE TmpCell (pokaz_id int NULL, cell_period smallint NULL, ist_fin_id int NULL,");
            q2->SQL->Add(" cell_din_izm character(50) NULL, cell_is_eval smallint NULL)");
            break;
    }
    q2->ExecSQL();

    for (const auto& ist_fin_id : IstFinList) {
        for (const auto& period : PeriodList) {
            AddTmpCell(pokaz_id, ist_fin_id, S, period, CellIsEvalGl);
        }
    }

    q2->SQL->Clear();
    q2->SQL->Add("INSERT INTO Cell (pokaz_id, cell_period, ist_fin_id, cell_din_izm, cell_is_eval)");
    q2->SQL->Add("  SELECT pokaz_id, cell_period, ist_fin_id, cell_din_izm, cell_is_eval");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q2->SQL->Add("  FROM #TmpCell");
            break;
        case TDBType::Postgre:
            q2->SQL->Add("  FROM TmpCell");
            break;
    }
    q2->ExecSQL();

    q2->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q2->SQL->Add("DROP TABLE #TmpCell");
            break;
        case TDBType::Postgre:
            q2->SQL->Add("DROP TABLE IF EXISTS TmpCell");
            break;
    }
    q2->ExecSQL();
    delete q2;
}
