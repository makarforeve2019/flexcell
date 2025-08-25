
#include "DB_IstFin.h"
#include "DB_Pokaztl.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Cell.h"
#include "unDefDBconnect.h"

void TDB_Cell::AddCell(int pokaz_id, int ist_fin_id, std::string cell_din_izm,
                       int cell_period, int cell_is_eval)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Cell");
    q->SQL->Add("(pokaz_id, ist_fin_id, cell_din_izm, cell_period, cell_is_eval)");
    q->SQL->Add("VALUES (:pokaz_id, :ist_fin_id, :cell_din_izm, :cell_period, :cell_is_eval)");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ParamByName("cell_din_izm")->AsString() = cell_din_izm;
    q->ParamByName("cell_period")->AsInteger() = cell_period;
    q->ParamByName("cell_is_eval")->AsInteger() = cell_is_eval;
    q->ExecSQL();
    delete q;
}

void TDB_Cell::Updcell_is_evaTo1(int pokaz_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = 1 ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND cell_period IN (1, 2, 3)");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    delete q;
}

void TDB_Cell::Updcell_is_evalIskluc1(int pokaz_id)
{
    int ist_fin_id, pokaz_is_kv, pokaz_is_mes, cell_id;
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    ist_fin_id = TDB_IstFin::GetIdVsego();
    TDB_Pokaztl::GetPokaz(qW, pokaz_id);
    pokaz_is_kv = qW->FieldByName("pokaz_is_kv")->AsInteger();
    pokaz_is_mes = qW->FieldByName("pokaz_is_mes")->AsInteger();
    TDB_Cell::GetAllCell_id(qW, pokaz_id);

    while (!qW->EoF())
    {
        if (qW->FieldByName("ist_fin_id")->AsInteger() != ist_fin_id)
        {
            cell_id = qW->FieldByName("cell_id")->AsInteger();
            if (qW->FieldByName("cell_is_eval")->AsInteger() == 1)
            {
                switch (qW->FieldByName("cell_period")->AsInteger())
                {
                    case 1:
                        if (pokaz_is_kv == 0)
                            TDB_Cell::Updcell_is_evalTo0(cell_id);
                        break;
                    case 2:
                        if (pokaz_is_kv == 1 && pokaz_is_mes == 0)
                            TDB_Cell::Updcell_is_evalTo0(cell_id);
                        break;
                    case 3:
                        TDB_Cell::Updcell_is_evalTo0(cell_id);
                        break;
                }
            }
        }
        qW->Next();
    }

    delete qW;
}

void TDB_Cell::Updcell_is_eval1(int pokaz_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = 1 ");
    q->SQL->Add("WHERE cell_period = 1 AND pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    delete q;
}

void TDB_Cell::Updcell_is_eval2(int pokaz_id, int kv)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = 1 ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id AND (cell_period = 1 ");
    q->SQL->Add("OR (cell_period = 2 AND :kv = 1 ))");
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    delete q;
}

void TDB_Cell::Updcell_is_eval3(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = 1 ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND cell_is_eval <> 2");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

    q->SQL->Clear();
    // для натуральных
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = 1 ");
    q->SQL->Add("WHERE cell_is_eval <> 2");
    q->SQL->Add("AND pokaz_id IN ");
    q->SQL->Add("(SELECT pokaz_id ");
    q->SQL->Add("FROM Pokaztl ");
    q->SQL->Add("WHERE pokaz_base_id = :pokaz_id)");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

    delete q;
}

void TDB_Cell::Updcell_is_evalTo0(int cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = 0");
    q->SQL->Add("WHERE cell_id = :cell_id");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ExecSQL();

    delete q; 
}

void TDB_Cell::Updcell_is_evalTo(int cell_id, int cell_is_eval) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Cell ");
    q->SQL->Add("SET cell_is_eval = :cell_is_eval");
    q->SQL->Add("WHERE cell_id = :cell_id");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("cell_is_eval")->AsInteger() = cell_is_eval;
    q->ExecSQL();

    delete q;
}

void TDB_Cell::DelForPokazId(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Cell ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

    delete q;
}

int TDB_Cell::GetCellId(int pokaz_id, int cell_period, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id ");
    q->SQL->Add("FROM Cell ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id ");
    q->SQL->Add("AND cell_period = :cell_period ");
    q->SQL->Add("AND ist_fin_id = :ist_fin_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_period")->AsInteger() = cell_period;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("cell_id")->AsInteger();
    }

    delete q;
    return result;
}

int TDB_Cell::GetCellId1(int pokaz_id, int cell_period, int ist_fin_id, int& cell_is_eval) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id, cell_is_eval ");
    q->SQL->Add("FROM Cell ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id ");
    q->SQL->Add("AND cell_period = :cell_period ");
    q->SQL->Add("AND ist_fin_id = :ist_fin_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_period")->AsInteger() = cell_period;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    if (!q->EoF()) {
        cell_is_eval = q->FieldByName("cell_is_eval")->AsInteger();
        result = q->FieldByName("cell_id")->AsInteger();
    }

    delete q;
    return result;
}

int TDB_Cell::GetCellId2(int pokaz_id, int cell_period, int ist_fin_id, std::string cell_din_izm) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id ");
    q->SQL->Add("FROM Cell ");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id ");
    q->SQL->Add("AND cell_period = :cell_period ");
    q->SQL->Add("AND ist_fin_id = :ist_fin_id ");
    q->SQL->Add("AND cell_din_izm = :cell_din_izm");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_period")->AsInteger() = cell_period;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ParamByName("cell_din_izm")->AsString() = cell_din_izm;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("cell_id")->AsInteger();
    }

    delete q;
    return result;
}

void TDB_Cell::GetCell_idIstFin(TQueryUni *qIn, int pokaz_id, int cell_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Cell, Ist_fin ");
    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id ");
    qIn->SQL->Add("AND cell_id = :cell_id ");
    qIn->SQL->Add("AND Cell.ist_fin_id = Ist_fin.ist_fin_id");
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("cell_id")->AsInteger() = cell_id;
    qIn->Open();
}

void TDB_Cell::GetCell_id(TQueryUni *qIn, int Pokaz_id, int Cell_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Cell ");
    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id ");
    qIn->SQL->Add("AND cell_id = :cell_id ");
    qIn->ParamByName("pokaz_id")->AsInteger() = Pokaz_id;
    qIn->ParamByName("cell_id")->AsInteger() = Cell_id;
    qIn->Open();
}

void TDB_Cell::GetAllCell_id(TQueryUni *qIn, int Pokaz_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Cell ");
    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id ");
    qIn->ParamByName("pokaz_id")->AsInteger() = Pokaz_id;
    qIn->Open();
}

void TDB_Cell::GetInfCell_id(TQueryUni *qIn, int Cell_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Cell ");
    qIn->SQL->Add("WHERE cell_id = :cell_id ");
    qIn->ParamByName("cell_id")->AsInteger() = Cell_id;
    qIn->Open();
}

void TDB_Cell::eval2_3by1(int Cell_Id, int& cell_id2, int& cell_id3, int& cell_id4) {
    int pokaz_id, cell_period;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Cell::GetInfCell_id(q, Cell_Id);
    pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
    cell_period = q->FieldByName("cell_period")->AsInteger();
    q->Close();
    cell_id2 = TDB_Cell::GetCellId(pokaz_id, cell_period, ist_fin_id_Budg2);
    cell_id3 = TDB_Cell::GetCellId(pokaz_id, cell_period, ist_fin_id_TrBudg);
    cell_id4 = TDB_Cell::GetCellId(pokaz_id, cell_period, ist_fin_id_CelSubsid);
    delete q;
}

int TDB_Cell::GetIstFinbyCell_id(int Cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int ist_fin_id = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_id FROM Cell ");
    q->SQL->Add("WHERE cell_id = :cell_id ");
    q->ParamByName("cell_id")->AsInteger() = Cell_id;
    q->Open();
    if (!q->EoF()) {
        ist_fin_id = q->FieldByName("ist_fin_id")->AsInteger();
    }
    delete q;
    return ist_fin_id;
}

int TDB_Cell::GetPokaz_idbyCell_id(int Cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int pokaz_id = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Cell ");
    q->SQL->Add("WHERE cell_id = :cell_id ");
    q->ParamByName("cell_id")->AsInteger() = Cell_id;
    q->Open();
    if (!q->EoF()) {
        pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
    }
    delete q;
    return pokaz_id;
}

void TDB_Cell::GetInfobyCell_id(int Cell_id, int& ist_fin_id, int& cell_period, int& pokaz_id, int& cell_is_eval) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Cell ");
    q->SQL->Add("WHERE cell_id = :cell_id ");
    q->ParamByName("cell_id")->AsInteger() = Cell_id;
    q->Open();
    if (!q->EoF()) {
        ist_fin_id = q->FieldByName("ist_fin_id")->AsInteger();
        cell_period = q->FieldByName("cell_period")->AsInteger();
        pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        cell_is_eval = q->FieldByName("cell_is_eval")->AsInteger();
    } else {
        ist_fin_id = 0;
        cell_period = 0;
        pokaz_id = 0;
        cell_is_eval = 0;
    }
    delete q;
}

int TDB_Cell::getSvobodnijCellId() {
    int cell_id = 0;
    int cell_idPred = 999;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id FROM Cell ");
    q->SQL->Add("WHERE cell_id > 999 ");
    q->SQL->Add("ORDER BY cell_id ");
    q->Open();
    while (!q->EoF()) {
        cell_id = q->FieldByName("cell_id")->AsInteger();
        if (cell_id == (cell_idPred + 1)) {
            cell_idPred = cell_id;
            q->Next();
        } else {
            delete q;
            return cell_idPred + 1;
        }
    }
    delete q;
    return cell_idPred + 1;
}

void TDB_Cell::AddIdSvobod(int aCell_id, int aPokaz_id, int acell_period) {
//vernut    TStoredProcUni *sp;
//    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
//
//    try {
//        q->SQL->Clear();
//        switch (GetDBType()) {
//            case TDBType::MSSQL:
//                q->SQL->Add("create procedure proc_InsertCell ");
//                q->SQL->Add("  @acell_id int, ");
//                q->SQL->Add("  @apokaz_id int, ");
//                q->SQL->Add("  @acell_period  int, ");
//                q->SQL->Add("  @acell_din_izm varchar(50) ");
//                q->SQL->Add("as ");
//                q->SQL->Add("SET IDENTITY_INSERT Cell ON ");
//                q->SQL->Add("INSERT INTO Cell ");
//                q->SQL->Add("(cell_id, pokaz_id, ist_fin_id, cell_period, cell_is_eval, cell_din_izm) ");
//                q->SQL->Add("values (@acell_id, @apokaz_id, 0, @acell_period, 0, @acell_din_izm) ");
//                q->SQL->Add("SET IDENTITY_INSERT Cell OFF ");
//                break;
//            case TDBType::Postgre:
//                q->SQL->Add("CREATE OR REPLACE FUNCTION public.proc_InsertCell( ");
//                q->SQL->Add("  acell_id integer, ");
//                q->SQL->Add("  apokaz_id integer, ");
//                q->SQL->Add("  acell_period integer, ");
//                q->SQL->Add("  acell_din_izm character varying(50) ");
//                q->SQL->Add(") RETURNS void as $$ ");
//                q->SQL->Add("begin ");
//                q->SQL->Add("    INSERT INTO Cell ");
//                q->SQL->Add("    (cell_id, pokaz_id, ist_fin_id, cell_period, cell_is_eval, cell_din_izm) ");
//                q->SQL->Add("     values (acell_id, apokaz_id, 0, acell_period, 0, acell_din_izm); ");
//                q->SQL->Add("     SELECT setval(pg_get_serial_sequence('Cell', 'cell_id'), ");
//                q->SQL->Add("         (select max(cell_id) from Cell) ");
//                q->SQL->Add("      ); ");
//                q->SQL->Add("     RETURN; ");
//                q->SQL->Add("end ");
//                q->SQL->Add("$$ LANGUAGE plpgsql; ");
//                break;
//        }
//        q->ExecSQL();
//    } catch (...) {
//
//    }
//
//    sp = TStoredProcFactory::Create(nullptr);
//    sp->DatabaseName = "EcDB";
//    sp->connection = DefDBconnect->Conn;
//    sp->StoredProcName = "proc_InsertCell";
//
//    switch (GetDBType()) {
//        case TDBType::MSSQL:
//            sp->CreateParam(ftInteger, "@acell_id", ptInput);
//            sp->CreateParam(ftInteger, "@aPokaz_id", ptInput);
//            sp->CreateParam(ftInteger, "@acell_period", ptInput);
//            sp->CreateParam(ftString, "@acell_din_izm", ptInput);
//            break;
//        case TDBType::Postgre:
//            sp->CreateParam(ftInteger, "acell_id", ptInput);
//            sp->CreateParam(ftInteger, "aPokaz_id", ptInput);
//            sp->CreateParam(ftInteger, "acell_period", ptInput);
//            sp->CreateParam(ftString, "acell_din_izm", ptInput);
//            break;
//    }
//
//    sp->ParamByName("@acell_id")->AsInteger() = aCell_id;
//    sp->ParamByName("@aPokaz_id")->AsInteger() = aPokaz_id;
//    sp->ParamByName("@acell_period")->AsInteger() = acell_period;
//    sp->ParamByName("@acell_din_izm")->AsString() = DupeString(' ', 50);
//    sp->ExecProc();
//    delete sp;
//
//    q->SQL->Clear();
//    switch (GetDBType()) {
//        case TDBType::MSSQL:
//            q->SQL->Add("DROP PROCEDURE proc_InsertCell");
//            break;
//        case TDBType::Postgre:
//            q->SQL->Add("DROP FUNCTION IF EXISTS public.proc_InsertCell( ");
//            q->SQL->Add("  acell_id integer, ");
//            q->SQL->Add("  apokaz_id integer, ");
//            q->SQL->Add("  acell_period integer, ");
//            q->SQL->Add("  acell_din_izm character varying(50) ");
//            q->SQL->Add(") CASCADE; ");
//            break;
//    }
//    q->ExecSQL();
//    delete q;
}
