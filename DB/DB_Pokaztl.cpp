#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_PlanStr.h"
#include "DB_Cell.h"
#include "DB_DenegnPotok.h"
#include "DB_CellValue.h"
#include "DB_GurnProtokol.h"
#include "DB_KeyOfObjects.h"

void TDB_Pokaztl::ProcessPokazId(int pokaz_id, int plan_id) {
    // Шаг 1: Получить pokaz_owner_id
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("SELECT pokaz_owner_id FROM public.pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    int pokaz_owner_id = q->FieldByName("pokaz_owner_id")->AsInteger();
    q->Close();

    // Шаг 2: Получить cell_id с нужным cell_period для pokaz_id
    std::map<int, int> cell_map;
    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id, cell_period FROM public.cell WHERE pokaz_id = :pokaz_id AND cell_period IN (1, 2, 3)");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    while (!q->EoF()) {
        int cell_id = q->FieldByName("cell_id")->AsInteger();
        int cell_period = q->FieldByName("cell_period")->AsInteger();
        cell_map[cell_id] = cell_period;
        q->Next();
    }
    q->Close();

    if (cell_map.empty()) {
        std::cout << "Нет данных по pokaz_id = " << pokaz_id << std::endl;
        delete q;
        return;
    }

    struct CellValueData {
        int cell_id;
        int cell_period;
        int vid_otc;
        std::string value_date;
        double value;
    };

    std::map<std::string, std::vector<CellValueData>> cell_values;
    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id, value_date, vid_otc, value FROM public.cell_value "
                "WHERE plan_id = :plan_id AND cell_id IN (");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    size_t count = 0;
    for (const auto& [cell_id, _] : cell_map) {
        q->SQL->Add(std::to_string(cell_id) + (count++ < cell_map.size() - 1 ? "," : ""));
    }
    q->SQL->Add(") ORDER BY value_date");
    q->Open();

    while (!q->EoF()) {
        int cell_id = q->FieldByName("cell_id")->AsInteger();
        CellValueData data;
        data.cell_id = cell_id;
        data.cell_period = cell_map[cell_id];
        data.vid_otc = q->FieldByName("vid_otc")->AsInteger();
        data.value_date = q->FieldByName("value_date")->AsString();
        data.value = q->FieldByName("value")->AsDouble();

        cell_values[data.value_date].push_back(data);
        q->Next();
    }
    q->Close();

    // Шаг 4: Получить данные для pokaz_owner_id
    std::map<std::string, std::vector<CellValueData>> owner_cell_values;
    std::map<int, int> owner_cell_map;
    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id, cell_period FROM public.cell WHERE pokaz_id = :pokaz_owner_id AND cell_period IN (1, 2, 3)");
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->Open();

    while (!q->EoF()) {
        int cell_id = q->FieldByName("cell_id")->AsInteger();
        int cell_period = q->FieldByName("cell_period")->AsInteger();
        owner_cell_map[cell_id] = cell_period;
        q->Next();
    }
    q->Close();

    if (!owner_cell_map.empty()) {
        q->SQL->Clear();
        q->SQL->Add("SELECT cell_id, value_date, vid_otc, value FROM public.cell_value "
                    "WHERE plan_id = :plan_id AND cell_id IN (");
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        size_t count = 0;
        for (const auto& [cell_id, _] : owner_cell_map) {
            q->SQL->Add(std::to_string(cell_id) + (count++ < owner_cell_map.size() - 1 ? "," : ""));
        }
        q->SQL->Add(") ORDER BY value_date");
        q->Open();

        while (!q->EoF()) {
            int cell_id = q->FieldByName("cell_id")->AsInteger();
            CellValueData data;
            data.cell_id = cell_id;
            data.cell_period = owner_cell_map[cell_id];
            data.vid_otc = q->FieldByName("vid_otc")->AsInteger();
            data.value_date = q->FieldByName("value_date")->AsString();
            data.value = q->FieldByName("value")->AsDouble();

            owner_cell_values[data.value_date].push_back(data);
            q->Next();
        }
        q->Close();
    }

    // Шаг 5: Обнуление value у pokaz_id и вычитание у pokaz_owner_id
    for (const auto& entry : cell_values) {
        for (const auto& val : entry.second) {
            // Обнуление у pokaz_id
            q->SQL->Clear();
            q->SQL->Add("UPDATE public.cell_value SET value = 0 "
                        "WHERE plan_id = :plan_id AND cell_id = :cell_id "
                        "AND value_date = :value_date AND vid_otc = :vid_otc");
            q->ParamByName("plan_id")->AsInteger() = plan_id;
            q->ParamByName("cell_id")->AsInteger() = val.cell_id;
            q->ParamByName("value_date")->AsString() = val.value_date;
            q->ParamByName("vid_otc")->AsInteger() = val.vid_otc;
            q->ExecSQL();

            // Вычитание у pokaz_owner_id
            if (owner_cell_values.find(entry.first) != owner_cell_values.end()) {
                for (auto& owner_val : owner_cell_values.at(entry.first)) {
                    if (val.vid_otc == owner_val.vid_otc && val.cell_period == owner_val.cell_period) {
                        q->SQL->Clear();
                        q->SQL->Add("UPDATE public.cell_value SET value = value - :value "
                                    "WHERE plan_id = :plan_id AND cell_id = :cell_id "
                                    "AND value_date = :value_date AND vid_otc = :vid_otc");
                        q->ParamByName("plan_id")->AsInteger() = plan_id;
                        q->ParamByName("cell_id")->AsInteger() = owner_val.cell_id;
                        q->ParamByName("value_date")->AsString() = owner_val.value_date;
                        q->ParamByName("vid_otc")->AsInteger() = owner_val.vid_otc;
                        q->ParamByName("value")->AsFloat() = val.value;
                        q->ExecSQL();
                    }
                }
            }
        }
    }
    delete q;
}

//void TDB_Pokaztl::ProcessPokazId(int pokaz_id, int plan_id) {
//    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
//    q->SQL->Clear();
//    q->SQL->Add("SELECT process_pokaz_id(:pokaz_id, :plan_id)");
//    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
//    q->ParamByName("plan_id")->AsInteger() = plan_id;
//    q->Open();
//    delete q;
//}

std::string TDB_Pokaztl::getNameEntity(int pokaz_is_razd, int typeEntity) {
    switch (pokaz_is_razd) {
        case 1:
            switch (typeEntity) {
                case 0:
                case 100:
                    return "раздел";
                case 101:
                    return "цель";
                case 102:
                    return "задачу";
            }
            break;
        case 2:
        case 3:
        case 4:
            // Handle other cases if needed
            break;
    }
    return "";
}

std::string TDB_Pokaztl::getNameEntityRP(int pokaz_is_razd, int typeEntity) {
    switch (pokaz_is_razd) {
        case 1:
            switch (typeEntity) {
                case 0:
                case 100:
                    return "раздела";
                case 101:
                    return "цели";
                case 102:
                    return "задачи";
            }
            break;
        case 2:
        case 3:
        case 4:
            // Handle other cases if needed
            break;
    }
    return "";
}

int TDB_Pokaztl::getTypeEntity(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT typeEntity");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("typeEntity")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::updTypeEntity(int pokaz_id, int typeEntity) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET typeEntity=:typeEntity");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("typeEntity")->AsInteger() = typeEntity;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_Pokaztl::GetPokaz(TQueryUni *qIn, int pokaz_id) {
    bool result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl");
    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->Open();
    if (qIn->EoF()) {
        result = false;
    }
    return result;
}

bool TDB_Pokaztl::GetRootPokaztl(int aPokaz_id, int& wPokaz_id, std::string& wPokaz_name, int& wPokaz_is_razd, int& wPokaz_type, int& wPokaz_vert) {
    bool result = false;
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        qTmp.SQL->Clear();
        qTmp.SQL->Add("select * from GetRootPokaztl(:aPokaz_id)");
        qTmp.ParamByName("aPokaz_id")->AsInteger() = aPokaz_id;
        qTmp.Open();
        if (!qTmp.EoF()) {
            result = true;
            wPokaz_id = qTmp.FieldByName("pokaz_id2")->AsInteger();
            wPokaz_name = qTmp.FieldByName("apokaz_name")->AsString();
            wPokaz_is_razd = qTmp.FieldByName("apokaz_is_razd")->AsInteger();
            wPokaz_type = qTmp.FieldByName("apokaz_type")->AsInteger();
            wPokaz_vert = qTmp.FieldByName("apokaz_vert")->AsInteger();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

//bool TDB_Pokaztl::GetPokaz(TQueryUni *qIn, int pokaz_id) {
//    bool result = true;
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT *");
//    qIn->SQL->Add("FROM Pokaztl");
//    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id");
//    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
//    qIn->Open();
//    if (qIn->EoF()) {
//        result = false;
//    }
//    return result;
//}

bool TDB_Pokaztl::isExist(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (q->EoF()) {
        result = false;
    }
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Pokaztl::GetPokazByOgr(TQueryUni *qIn, int pokaz_id, TDateTime date_beg, TDateTime date_end, int plan_id) {
    bool result = true;
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl a, Plan_str b");
    qIn->SQL->Add("WHERE a.pokaz_id = :pokaz_id");
    qIn->SQL->Add("AND a.pokaz_beg_date < :date_end");
    qIn->SQL->Add("AND a.pokaz_end_date > :date_beg");
    qIn->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    qIn->SQL->Add("AND (b.plan_id = 1 OR b.plan_id = :plan_id)");
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("date_end")->AsDateTime() = date_end;
    qIn->ParamByName("date_beg")->AsDateTime() = date_beg;
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->Open();
    if (qIn->EoF()) {
        result = false;
    }
    return result;
}

bool TDB_Pokaztl::GetPokazForCell(TQueryUni *qIn, int cell_id) {
    bool result = true;
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT DISTINCT *");
    qIn->SQL->Add("FROM Cell, Pokaztl");
    qIn->SQL->Add("WHERE cell_id = :cell_id");
    qIn->SQL->Add("AND Pokaztl.pokaz_id = Cell.pokaz_id");
    qIn->ParamByName("cell_id")->AsInteger() = cell_id;
    qIn->Open();
    if (qIn->EoF()) {
        //throw Exception("Показатель по cell_id не найден");
        result = false;
    }
    return result;
}

std::string TDB_Pokaztl::GetCodeAU(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_code");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    std::string result = q->FieldByName("pokaz_code")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_Pokaztl::GetCodeBK(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_bk_code");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    std::string result = q->FieldByName("pokaz_bk_code")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_Pokaztl::GetCodeBK_rzdltl(int pokaz_id, int god) {
    std::string pokaz_bk_code = GetCodeBK(pokaz_id);
    return SetCodeBK_rzdltl(pokaz_bk_code, god);
}

int TDB_Pokaztl::GetTypePokaz(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_type");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    int result = q->FieldByName("pokaz_type")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetPokazIsRazd(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_is_razd");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    int result = q->FieldByName("pokaz_is_razd")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_Pokaztl::GetPokazCode(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_code");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    std::string result = q->FieldByName("pokaz_code")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_Pokaztl::GetPokazName(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "???";
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_name");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_name")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_Pokaztl::GetPokazName_EdIzm(int pokaz_id, int& edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "???";
    edizm_id = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_name, edizm_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_name")->AsString();
        edizm_id = q->FieldByName("edizm_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetPokazOwnerId(int pokaz_id) {
    if (pokaz_id == 0)
        return 0;
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_owner_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_owner_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

//void TDB_Pokaztl::GetRootPokaztl(int aPokaz_id, int& wPokaz_id, std::string& wPokaz_name, int& wPokaz_is_razd, int& wPokaz_type, int& wPokaz_vert) {
//    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn);
//    sp->storedProcName = "GetRootPokaztl";
//    switch (GetDBType()) {
//        case TDBType::MSSQL: {
//            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
//            sp->Params->CreateParam(ftInteger, "@Pokaz_id", ptOutput);
//            sp->Params->CreateParam(ftString, "@Pokaz_name", ptOutput);
//            sp->Params->CreateParam(ftInteger, "@Pokaz_is_razd", ptOutput);
//            sp->Params->CreateParam(ftInteger, "@Pokaz_type", ptOutput);
//            sp->Params->CreateParam(ftInteger, "@Pokaz_vert", ptOutput);
//            sp->Params->ParamByName("@aPokaz_id")->AsInteger() = aPokaz_id;
//            sp->ExecProc();
//            wPokaz_id = sp->ParamByName("@Pokaz_id")->AsInteger();
//            wPokaz_name = sp->ParamByName("@Pokaz_name")->AsString();
//            wPokaz_is_razd = sp->ParamByName("@Pokaz_is_razd")->AsInteger();
//            wPokaz_type = sp->ParamByName("@Pokaz_type")->AsInteger();
//            wPokaz_vert = sp->ParamByName("@Pokaz_vert")->AsInteger();
//            break;
//        }
//        case TDBType::Postgre: {
//            sp->Params->CreateParam(ftInteger, "apokaz_id", ptInput);
//            sp->Params->CreateParam(ftInteger, "pokaz_id2", ptOutput);
//            sp->Params->CreateParam(ftString, "apokaz_name", ptOutput);
//            sp->Params->CreateParam(ftInteger, "apokaz_is_razd", ptOutput);
//            sp->Params->CreateParam(ftInteger, "apokaz_type", ptOutput);
//            sp->Params->CreateParam(ftInteger, "apokaz_vert", ptOutput);
//            sp->Params->ParamByName("apokaz_id")->AsInteger() = aPokaz_id;
//            sp->ExecProc();
//            wPokaz_id = sp->ParamByName("pokaz_id2")->AsInteger();
//            wPokaz_name = sp->ParamByName("apokaz_name")->AsString();
//            wPokaz_is_razd = sp->ParamByName("apokaz_is_razd")->AsInteger();
//            wPokaz_type = sp->ParamByName("apokaz_type")->AsInteger();
//            wPokaz_vert = sp->ParamByName("apokaz_vert")->AsInteger();
//            break;
//        }
//    }
//    delete sp;
//}

int TDB_Pokaztl::GetEnumId(int pokaz_id) {
    if (pokaz_id == 0)
        return 0;
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT enum_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("enum_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetInCast(int pokaz_id) {
    if (pokaz_id == 0)
        return 0;
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT in_cast");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("in_cast")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetPokazVert(int pokaz_id) {
    if (pokaz_id == 0)
        return 0;
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_vert");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_vert")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::GetVert_Level(int class_id, int& pokaz_vert, std::string& vert_str, int& level_sign) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_vert, level_sign, vert_str");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = class_id;
    q->Open();
    if (!q->EoF()) {
        pokaz_vert = q->FieldByName("pokaz_vert")->AsInteger();
        vert_str = q->FieldByName("vert_str")->AsString();
        level_sign = q->FieldByName("level_sign")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::GetBegEndLevel(int pokaz_id, TDateTime& pokaz_beg_date, TDateTime& pokaz_end_date, int& level_sign) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_beg_date, pokaz_end_date, level_sign");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        pokaz_beg_date = q->FieldByName("pokaz_beg_date")->AsDateTime();
        pokaz_end_date = q->FieldByName("pokaz_end_date")->AsDateTime();
        level_sign = q->FieldByName("level_sign")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetBegEnd(int pokaz_id, TDateTime pokaz_beg_date, TDateTime pokaz_end_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_beg_date = :pokaz_beg_date, pokaz_end_date = :pokaz_end_date");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
    q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::GetIs_List(int pokaz_id, int& is_list) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    is_list = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT is_list");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        is_list = q->FieldByName("is_list")->AsInteger();
    }
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetIs_List(int pokaz_id, int is_list) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET is_list = :is_list");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("is_list")->AsInteger() = is_list;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::GetClassId(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT objekt_class_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("objekt_class_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::UpdClassId(int pokaz_id, int objekt_class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET objekt_class_id = :objekt_class_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("objekt_class_id")->AsInteger() = objekt_class_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::GetRootCastId(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT enum_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("enum_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetKBK_Id(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT enum_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("enum_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetPrevPokazId(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT b.pokaz_id as pokaz_id");
    q->SQL->Add("FROM Plan_str a, Plan_str b");
    q->SQL->Add("WHERE a.pokaz_id = :pokaz_id");
    q->SQL->Add("AND b.plan_str_owner_id = a.plan_str_owner_id");
    q->SQL->Add("AND (b.plan_id = a.plan_id OR b.plan_id = 1)");
    q->SQL->Add("AND b.plan_str_order < a.plan_str_order");
    q->SQL->Add("ORDER BY b.plan_str_order DESC");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int GetOwnerId(int aPokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_owner_id FROM Pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = aPokaz_id;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("pokaz_owner_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return Result;
}

int GetOwnerId1(int aPokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_owner_id, pokaz_is_razd FROM Pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = aPokaz_id;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("pokaz_owner_id")->AsInteger();
        if (q->FieldByName("pokaz_is_razd")->AsInteger() == 0) { // это составная часть
            Result = 0;
        }
    }
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_Pokaztl::GetPokazIdInCast(int aPokaz_id) {
    int pokaz_id = aPokaz_id;
    while (true) {
        int N = GetOwnerId(pokaz_id);
        if (N <= 0) break;
        pokaz_id = N;
    }
    return pokaz_id;
}

int TDB_Pokaztl::GetPokazIdInCast1(int aPokaz_id) {
    int pokaz_id = aPokaz_id;
    while (true) {
        int N = GetOwnerId1(pokaz_id);
        if (N <= 0) break;
        pokaz_id = N;
    }
    return pokaz_id;
}

std::string TDB_Pokaztl::GetCastName(int pokazId) {
    int wPokaz_id, wPokaz_is_razd, wPokaz_type, wPokaz_vert;
    std::string wPokaz_name;
    GetRootPokaztl(pokazId, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
    return wPokaz_name;
}



std::string TDB_Pokaztl::GetPrimecanie(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    if (pokaz_id == 0) return Result;
    q->SQL->Clear();
    q->SQL->Add("SELECT Primecanie FROM Pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("Primecanie")->AsString();
    }
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_Pokaztl::SetPrimecanie(int pokaz_id, std::string Primecanie) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl SET Primecanie=:Primecanie WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("Primecanie")->AsString() = Primecanie;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void SetTableColumnNames(int pokaz_id, std::string tableName, std::string columnName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl SET idsOwnerFieldItogo=:tableName, forMultiItogo=:columnName WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("tableName")->AsString() = tableName;
    q->ParamByName("columnName")->AsString() = columnName;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void UpdTypePokaz(int pokaz_id, int pokaz_type) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl SET pokaz_type=:pokaz_type WHERE pokaz_id=:pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_type")->AsInteger() = pokaz_type;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int GetIsSurgtName(int pokaz_id) {
    int Result = 0;
    if (pokaz_id == 0) return Result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT isSurgtName FROM Pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("isSurgtName")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return Result;
}

void UpdIsSurgtName(int pokaz_id, int isSurgtName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl SET isSurgtName=:isSurgtName WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("isSurgtName")->AsInteger() = isSurgtName;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::Set_level_text(int plan_id, int owner_id, int pokaz_id, int level_sign) {
    std::string s, level_text;
    char c;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    switch (level_sign) {
        case 3: level_text = " - "; break;
        case 4: level_text = ""; break;
    }

    switch (level_sign) {
        case 3:
        case 4: goto Upd;
    }

    q->SQL->Clear();
    q->SQL->Add("SELECT plan_str_order, level_text, level_sign");
    q->SQL->Add("FROM Pokaztl, Plan_str");
    q->SQL->Add("WHERE plan_str_owner_id = :owner_id");
    q->SQL->Add("AND (plan_id = 1 OR plan_id = :plan_id)");
    q->SQL->Add("AND Plan_str.pokaz_id <> :pokaz_id");
    q->SQL->Add("AND Pokaztl.pokaz_id = Plan_str.pokaz_id");
    q->SQL->Add("AND Pokaztl.level_sign = :level_sign");
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->Open();

    if (q->EoF()) {
        switch (q->FieldByName("level_sign")->AsInteger()) {
            case 1: level_text = "1."; break;
            case 2: level_text = "а)"; break;
        }
    }
    else {
        switch (q->FieldByName("level_sign")->AsInteger()) {
            case 1: {
                s = q->FieldByName("level_text")->AsString();
                s = s.substr(0, s.length() - 1);
                if (s == "") s = "0";
                level_text = std::to_string(std::stoi(s) + 1) + ".";
                break;
            }
            case 2: {
                s = q->FieldByName("level_text")->AsString();
                c = s[0];
                level_text = std::string(1, c + 1) + ")";
                break;
            }
        }
    }

    q->Close();

Upd:
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl ");
    q->SQL->Add("SET level_text = :level_text");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("level_text")->AsString() = level_text;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::GetSostCasts(TQueryUni *qIn, int VidCast) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl a, Plan_str b");
    qIn->SQL->Add("WHERE pokaz_owner_id = 0 AND pokaz_is_razd = 0");
    qIn->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    if (VidCast == 0) {
        qIn->SQL->Add("AND (a.pokaz_type = :VidCast OR a.pokaz_type IS NULL)");
    } else {
        qIn->SQL->Add("AND a.pokaz_type = :VidCast");
    }
    qIn->SQL->Add("ORDER BY plan_str_order ");
    qIn->ParamByName("VidCast")->AsInteger() = VidCast;
    qIn->Open();
}

void TDB_Pokaztl::GetAllSostCasts(TQueryUni *qIn, int VidCast) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl a, Plan_str b");
    qIn->SQL->Add("WHERE pokaz_is_razd = 0");
    qIn->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    if (VidCast == 0) {
        qIn->SQL->Add("AND (a.pokaz_type = :VidCast OR a.pokaz_type IS NULL)");
    } else {
        qIn->SQL->Add("AND a.pokaz_type = :VidCast");
    }
    qIn->SQL->Add("ORDER BY pokaz_owner_id, plan_str_order");
    qIn->ParamByName("VidCast")->AsInteger() = VidCast;
    qIn->Open();
}

int TDB_Pokaztl::AddSostCast(int VidCast, std::string pokazName, int pokaz_owner_id,
                             int pokaz_beg_date, int pokaz_end_date, std::string Primecanie) {
    int pokaz_id;
    bool bInTransaction;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 
    try {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Pokaztl");
        q->SQL->Add("(pokaz_name,pokaz_is_razd,pokaz_owner_id,pokaz_beg_date,pokaz_end_date, pokaz_type, create_date, pokaz_vert, Primecanie)");
        q->SQL->Add("VALUES (:pokazName,0,:pokaz_owner_id,:Begdate,:Enddate, :pokaz_type, :create_date, :pokaz_vert, :Primecanie)");
        q->ParamByName("pokazName")->AsString() = pokazName;
        q->ParamByName("Begdate")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
        q->ParamByName("Enddate")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
        q->ParamByName("create_date")->AsDateTime() = Date();
        q->ParamByName("pokaz_type")->AsInteger() = VidCast;
        q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
        if (pokaz_owner_id == 0) {
            q->ParamByName("pokaz_vert")->AsInteger() = 0;
        } else {
            q->ParamByName("pokaz_vert")->AsInteger() = 1;
        }
        q->ParamByName("Primecanie")->AsString() = Primecanie;
        q->ExecSQL();
       //MAK q->Free();
delete q;
        pokaz_id = TDB_Pokaztl::GetId();
      
    } catch (...) {
        ;
    }
    return pokaz_id;
}

int TDB_Pokaztl::AddSostCastBK(int VidCast, std::string pokazName, int pokaz_owner_id,
                               int pokaz_beg_date, int pokaz_end_date, int edizm_id, int pokaz_is_god, int pokaz_is_kv) {
    int pokaz_id;
    bool bInTransaction;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Pokaztl");
    q->SQL->Add("(pokaz_name,pokaz_is_razd,pokaz_owner_id,pokaz_beg_date,pokaz_end_date, pokaz_type, create_date, pokaz_vert, edizm_id, pokaz_is_god, pokaz_is_kv)");
    q->SQL->Add("VALUES (:pokazName,0,:pokaz_owner_id,:Begdate,:Enddate, :pokaz_type, :create_date, :pokaz_vert, :edizm_id, :pokaz_is_god, :pokaz_is_kv)");
    q->ParamByName("pokazName")->AsString() = pokazName;
    q->ParamByName("Begdate")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
    q->ParamByName("Enddate")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
    q->ParamByName("create_date")->AsDateTime() = Date();
    q->ParamByName("pokaz_type")->AsInteger() = VidCast;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    if (pokaz_owner_id == 0) {
        q->ParamByName("pokaz_vert")->AsInteger() = 0;
    } else {
        q->ParamByName("pokaz_vert")->AsInteger() = 1;
    }
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("pokaz_is_god")->AsInteger() = pokaz_is_god;
    q->ParamByName("pokaz_is_kv")->AsInteger() = pokaz_is_kv;
    q->ExecSQL();
   //MAK q->Free();
delete q;
    pokaz_id = TDB_Pokaztl::GetId();
 
    return pokaz_id;
}

int TDB_Pokaztl::AddCastClass(int VidCast, std::string pokazName, int pokaz_owner_id, int pokaz_beg_date,
                              int pokaz_end_date, int level_sign, int pokaz_vert, std::string vert_str, int is_list, int pokaz_is_god) {
    int pokaz_id;
    bool bInTransaction;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
 
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Pokaztl");
    q->SQL->Add("(pokaz_name,pokaz_is_razd,pokaz_owner_id,pokaz_beg_date,pokaz_end_date, pokaz_type, " );
    q->SQL->Add("create_date, level_sign, pokaz_vert, vert_str, is_list, pokaz_is_god)");
    q->SQL->Add("VALUES (:pokazName,0,:pokaz_owner_id,:Begdate,:Enddate, :pokaz_type, " );
    q->SQL->Add(":create_date, :level_sign, :pokaz_vert, :vert_str, :is_list, :pokaz_is_god)");
    q->ParamByName("pokazName")->AsString() = pokazName;
    q->ParamByName("Begdate")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
    q->ParamByName("Enddate")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
    q->ParamByName("create_date")->AsDateTime() = Date();
    q->ParamByName("pokaz_type")->AsInteger() = VidCast;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("vert_str")->AsString() = vert_str;
    q->ParamByName("is_list")->AsInteger() = is_list;
    q->ParamByName("pokaz_is_god")->AsInteger() = pokaz_is_god;
    q->ExecSQL();
   //MAK q->Free();
delete q;
    pokaz_id = TDB_Pokaztl::GetId();
 
    return pokaz_id;
}

int TDB_Pokaztl::AddCastSetOfStr(int VidCast, std::string pokazName, int pokaz_owner_id, int pokaz_beg_date,
                                 int pokaz_end_date, std::string ListTables, std::string PredicatSelect, std::string Primecanie) {
    int pokaz_id;
    bool bInTransaction;
    int Result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Pokaztl");
        q->SQL->Add("(pokaz_name,pokaz_is_razd,pokaz_owner_id,pokaz_beg_date,pokaz_end_date, pokaz_type, " );
        q->SQL->Add("create_date, idsOwnerFieldItogo, forMultiItogo, Primecanie)");
        q->SQL->Add("VALUES (:pokazName,0,:pokaz_owner_id,:Begdate,:Enddate, :pokaz_type, " );
        q->SQL->Add(":create_date, :idsOwnerFieldItogo, :forMultiItogo, :Primecanie)");
        q->ParamByName("pokazName")->AsString() = pokazName;
        q->ParamByName("Begdate")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
        q->ParamByName("Enddate")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
        q->ParamByName("create_date")->AsDateTime() = Date();
        q->ParamByName("pokaz_type")->AsInteger() = VidCast;
        q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
        q->ParamByName("idsOwnerFieldItogo")->AsString() = ListTables;
        q->ParamByName("forMultiItogo")->AsString() = PredicatSelect;
        q->ParamByName("Primecanie")-> AsString() = Primecanie;
        q->ExecSQL();
       //MAK q->Free();
delete q;
        pokaz_id = TDB_Pokaztl::GetId();
        Result = pokaz_id;
        TDB_PlanStr::AddElem(false, 1, pokaz_owner_id, pokaz_id);
 
    } catch (...) {
        
    }
    return Result;
}

void TDB_Pokaztl::AddParentId(int class_id, int parent_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_base_id = :parent_id");
    q->SQL->Add("WHERE pokaz_id = :class_id");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ParamByName("parent_id")->AsInteger() = parent_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::ReNameSostCast(int pokaz_id, std::string pokazName, int pokaz_beg_date, int pokaz_end_date, std::string Primecanie) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl ");
    q->SQL->Add("SET pokaz_name = :pokazName, pokaz_beg_date = :pokaz_beg_date, " );
    q->SQL->Add("pokaz_end_date = :pokaz_end_date, Primecanie = :Primecanie");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokazName")->AsString() = pokazName;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
    q->ParamByName("pokaz_end_date")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
    q->ParamByName("Primecanie")-> AsString() = Primecanie;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdCastClass(int pokaz_id, std::string pokazName, int pokaz_beg_date,
                               int pokaz_end_date, int level_sign, int pokaz_vert, std::string vert_str, int is_list, int pokaz_is_god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl ");
    q->SQL->Add("SET pokaz_name = :pokazName, pokaz_beg_date = :pokaz_beg_date, pokaz_end_date = :pokaz_end_date," );
    q->SQL->Add("level_sign = :level_sign, pokaz_vert = :pokaz_vert, vert_str = :vert_str, is_list = :is_list, pokaz_is_god = :pokaz_is_god");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokazName")->AsString() = pokazName;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
    q->ParamByName("pokaz_end_date")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("vert_str")->AsString() = vert_str;
    q->ParamByName("is_list")->AsInteger() = is_list;
    q->ParamByName("pokaz_is_god")->AsInteger() = pokaz_is_god;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdCastSetOfStr(int CastSetId, std::string pokazName, int pokaz_beg_date, int pokaz_end_date,
                                  std::string ListTables, std::string PredicatSelect, std::string Primecanie) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl ");
    q->SQL->Add("SET pokaz_name = :pokazName, pokaz_beg_date = :pokaz_beg_date, pokaz_end_date = :pokaz_end_date," );
    q->SQL->Add("idsOwnerFieldItogo = :ListTables, forMultiItogo = :PredicatSelect, Primecanie = :Primecanie");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokazName")->AsString() = pokazName;
    q->ParamByName("pokaz_id")->AsInteger() = CastSetId;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = EncodeDate(pokaz_beg_date, 1, 1);
    q->ParamByName("pokaz_end_date")->AsDateTime() = EncodeDate(pokaz_end_date, 12, 31);
    q->ParamByName("ListTables")->AsString() = ListTables;
    q->ParamByName("PredicatSelect")->AsString() = PredicatSelect;
    q->ParamByName("Primecanie")->AsString() = Primecanie;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_Pokaztl::IsSostCastEmpty(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl ");
    q->SQL->Add("WHERE pokaz_owner_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    bool isEmpty = q->EoF();
   //MAK q->Free();
delete q;
    return isEmpty;
}

bool TDB_Pokaztl::UnikName(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT a.pokaz_id");
    q->SQL->Add("FROM Pokaztl a, Plan_str b");
    q->SQL->Add("WHERE (plan_id = 1 OR plan_id = :plan_id)");
    q->SQL->Add("AND plan_str_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND pokaz_name = :pokaz_name");

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND DATEPART(YEAR, create_date) = :CurrGod");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', create_date) = :CurrGod");
            break;
    }

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("CurrGod")->AsInteger() = Year;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_Pokaztl::UnikName2(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name, int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT a.pokaz_id");
    q->SQL->Add("FROM Pokaztl a, Plan_str b");
    q->SQL->Add("WHERE (plan_id = 1 OR plan_id = :plan_id)");
    q->SQL->Add("AND plan_str_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND pokaz_name = :pokaz_name");
    q->SQL->Add("AND edizm_id = :edizm_id");

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND DATEPART(YEAR, create_date) = :CurrGod");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', create_date) = :CurrGod");
            break;
    }

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("CurrGod")->AsInteger() = Year;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_Pokaztl::UnikNameIzm(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name, int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT a.pokaz_id");
    q->SQL->Add("FROM Pokaztl a, Plan_str b");
    q->SQL->Add("WHERE (plan_id = 1 OR plan_id = :plan_id)");
    q->SQL->Add("AND plan_str_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND b.pokaz_id <> :pokaz_id");
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND pokaz_name = :pokaz_name");

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND DATEPART(YEAR, create_date) = :CurrGod");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', create_date) = :CurrGod");
            break;
    }

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("CurrGod")->AsInteger() = Year;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_Pokaztl::UnikNameIzm2(int Year, int plan_id, int pokaz_owner_id, std::string pokaz_name, int pokaz_id, int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT a.pokaz_id");
    q->SQL->Add("FROM Pokaztl a, Plan_str b");
    q->SQL->Add("WHERE (plan_id = 1 OR plan_id = :plan_id)");
    q->SQL->Add("AND plan_str_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND b.pokaz_id <> :pokaz_id");
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND pokaz_name = :pokaz_name");
    q->SQL->Add("AND edizm_id = :edizm_id");

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND DATEPART(YEAR, create_date) = :CurrGod");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', create_date) = :CurrGod");
            break;
    }

    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("CurrGod")->AsInteger() = Year;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->Open();
    if (!q->EoF()) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_Pokaztl::AddPart(int subject_id, std::string pokaz_name, int pokaz_owner_id, TDateTime pokaz_beg_date,
                          TDateTime pokaz_end_date, int level_sign, int in_cast) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int pokaz_id = -1;

    try {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Pokaztl");
        q->SQL->Add("(pokaz_name, pokaz_is_razd, pokaz_owner_id, pokaz_beg_date, pokaz_end_date, level_sign, create_date, in_cast)");
        q->SQL->Add("values (:pokaz_name,1,:pokaz_owner_id,:pokaz_beg_date,:pokaz_end_date, :level_sign, :create_date, :in_cast)");

        q->ParamByName("pokaz_name")->AsString() = pokaz_name;
        q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
        q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
        q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
        q->ParamByName("level_sign")->AsInteger() = level_sign;
        q->ParamByName("create_date")->AsDateTime() = Date();
        q->ParamByName("in_cast")->AsInteger() = in_cast;

        q->ExecSQL();
        
        pokaz_id = TDB_Pokaztl::GetId();
        TDB_PlanStr::AddElem(false, subject_id, pokaz_owner_id, pokaz_id);
        
 
    } catch (...) {
 
    }

   //MAK q->Free();
delete q;
    return pokaz_id;
}

void TDB_Pokaztl::UpdPart(int pokaz_id, std::string pokaz_name, TDateTime pokaz_beg_date, TDateTime pokaz_end_date,
                           int level_sign) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Pokaztl");
        q->SQL->Add("SET pokaz_name=:pokaz_name, pokaz_beg_date=:pokaz_beg_date," );
        q->SQL->Add("pokaz_end_date=:pokaz_end_date, level_sign=:level_sign");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id");

        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("pokaz_name")->AsString() = pokaz_name;
        q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
        q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
        q->ParamByName("level_sign")->AsInteger() = level_sign;

        q->ExecSQL();
    } catch (...) {
        // Handle exceptions
    }

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdPartBK(int pokaz_id, std::string pokaz_bk_code) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Pokaztl");
        q->SQL->Add("SET pokaz_bk_code=:pokaz_bk_code");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id");

        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;

        q->ExecSQL();
    } catch (...) {
        // Handle exceptions
    }

   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::AddParam(bool isItogo, int subject_id, std::string pokaz_name,
                           int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
                           std::string pokaz_code, string pokaz_bk_code, int pokaz_type,
                           int enum_id, int edizm_id, TDateTime pokaz_beg_date,
                           TDateTime pokaz_end_date, int level_sign, std::string vert_str,
                           int is_list, int in_cast) {
    int pokaz_id;
 
    try {
        pokaz_id = TDB_Pokaztl::AddParamOnlyPokaztl(pokaz_name, pokaz_is_razd, pokaz_owner_id,
                                                     pokaz_vert, pokaz_code, pokaz_bk_code,
                                                     pokaz_type, enum_id, edizm_id,
                                                     pokaz_beg_date, pokaz_end_date, level_sign,
                                                     vert_str, is_list, in_cast);
        // Add element to plan structure
        TDB_PlanStr::AddElem(isItogo, subject_id, pokaz_owner_id, pokaz_id);
 
    } catch (...) {
         
    }
    return pokaz_id;
}

int TDB_Pokaztl::AddParamOnlyPokaztl(std::string pokaz_name, int pokaz_is_razd, int pokaz_owner_id,
                                      int pokaz_vert, std::string pokaz_code, std::string pokaz_bk_code,
                                      int pokaz_type, int enum_id, int edizm_id,
                                      TDateTime pokaz_beg_date, TDateTime pokaz_end_date,
                                      int level_sign, std::string vert_str, int is_list, int in_cast) {
    int pokaz_id;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Pokaztl");
    q->SQL->Add("(pokaz_name, pokaz_is_razd, pokaz_owner_id, pokaz_vert,"
                "pokaz_code, pokaz_bk_code, pokaz_type, enum_id, edizm_id, pokaz_beg_date,"
                "pokaz_end_date, level_sign, vert_str, create_date, is_list, in_cast)");
    q->SQL->Add("VALUES (:pokaz_name, :pokaz_is_razd, :pokaz_owner_id, :pokaz_vert,"
                ":pokaz_code, :pokaz_bk_code, :pokaz_type, :enum_id, :edizm_id, :pokaz_beg_date,"
                ":pokaz_end_date, :level_sign, :vert_str, :create_date, :is_list, :in_cast)");
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("pokaz_is_razd")->AsInteger() = pokaz_is_razd;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("pokaz_code")->AsString() = pokaz_code;
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ParamByName("pokaz_type")->AsInteger() = pokaz_type;
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
    q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("vert_str")->AsString() = vert_str;
    q->ParamByName("create_date")->AsDateTime() = Date();
    q->ParamByName("is_list")->AsInteger() = is_list;
    q->ParamByName("in_cast")->AsInteger() = in_cast;
    q->ExecSQL();
    pokaz_id = TDB_Pokaztl::GetId();
    return pokaz_id;
   //MAK q->Free();
delete q;
}


void TDB_Pokaztl::UpdParam(int pokaz_id, std::string pokaz_name,
                           int pokaz_vert, std::string pokaz_code, std::string pokaz_bk_code, int enum_id, int edizm_id,
                           TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
                           std::string vert_str){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_name=:pokaz_name, pokaz_vert=:pokaz_vert, pokaz_code=:pokaz_code, pokaz_bk_code=:pokaz_bk_code,");
    q->SQL->Add("enum_id=:enum_id, edizm_id=:edizm_id, pokaz_beg_date=:pokaz_beg_date, pokaz_end_date=:pokaz_end_date,");
    q->SQL->Add("level_sign=:level_sign, vert_str=:vert_str");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("pokaz_code")->AsString() = pokaz_code;
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
    q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("vert_str")->AsString() = vert_str;
    
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::AddObjekt(bool isItogo, int subject_id, std::string pokaz_name,
                           int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
                           std::string pokaz_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
                           std::string vert_str, int objekt_class_id, int in_cast, std::string pokaz_bk_code) {
    int pokaz_id = 0;
 
    try {
        pokaz_id = TDB_Pokaztl::AddOnlyObjekt(pokaz_name, pokaz_is_razd, pokaz_owner_id, pokaz_vert,
                                               pokaz_code, pokaz_beg_date, pokaz_end_date, level_sign,
                                               vert_str, objekt_class_id, in_cast, pokaz_bk_code);

        TDB_PlanStr::AddElem(isItogo, subject_id, pokaz_owner_id, pokaz_id);
 
    } catch (...) {
 
        pokaz_id = 0;
    }
    return pokaz_id;
}

int TDB_Pokaztl::AddOnlyObjekt(std::string pokaz_name,
                               int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
                               std::string pokaz_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
                               std::string vert_str, int objekt_class_id, int in_cast, std::string pokaz_bk_code) {
    int pokaz_id = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Pokaztl");
    q->SQL->Add("(pokaz_name, pokaz_is_razd, pokaz_owner_id, pokaz_vert," );
    q->SQL->Add("pokaz_code, pokaz_beg_date, pokaz_end_date, level_sign," );
    q->SQL->Add("vert_str, create_date, objekt_class_id, in_cast, pokaz_bk_code)");
    q->SQL->Add("values (:pokaz_name,:pokaz_is_razd,:pokaz_owner_id,:pokaz_vert," );
    q->SQL->Add(":pokaz_code, :pokaz_beg_date, :pokaz_end_date, :level_sign," );
    q->SQL->Add(":vert_str, :create_date, :objekt_class_id, :in_cast, :pokaz_bk_code)");
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("pokaz_is_razd")->AsInteger() = pokaz_is_razd;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("pokaz_code")->AsString() = pokaz_code;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
    q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("vert_str")->AsString() = vert_str;
    q->ParamByName("create_date")->AsDateTime() = Date();
    q->ParamByName("objekt_class_id")->AsInteger() = objekt_class_id;
    q->ParamByName("in_cast")->AsInteger() = in_cast;
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ExecSQL();
    pokaz_id = TDB_Pokaztl::GetId();
    return pokaz_id;
   //MAK q->Free();
delete q;
    return pokaz_id;
}

int TDB_Pokaztl::AddObjekt2(bool isItogo, int subject_id, std::string pokaz_name,
                            int pokaz_is_razd, int pokaz_owner_id, int pokaz_vert,
                            std::string pokaz_bk_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
                            std::string vert_str, int objekt_class_id, int in_cast) {
    int pokaz_id = 0;
 
    try {
        pokaz_id = TDB_Pokaztl::AddOnlyObjekt(pokaz_name, pokaz_is_razd, pokaz_owner_id, pokaz_vert,
                                               "", pokaz_beg_date, pokaz_end_date, level_sign,
                                               vert_str, objekt_class_id, in_cast, pokaz_bk_code);
        TDB_PlanStr::AddElem(isItogo, subject_id, pokaz_owner_id, pokaz_id);
 
    } catch (...) {
 
    }
    return pokaz_id;
}

void TDB_Pokaztl::UpdObjekt(int pokaz_id, std::string pokaz_name,
                            int pokaz_vert, std::string pokaz_code, TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign,
                            std::string vert_str, std::string pokaz_bk_code) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_name=:pokaz_name, pokaz_vert = :pokaz_vert," );
    q->SQL->Add("pokaz_code = :pokaz_code, pokaz_beg_date=:pokaz_beg_date," );
    q->SQL->Add("pokaz_end_date=:pokaz_end_date," );
    q->SQL->Add("level_sign=:level_sign, vert_str = :vert_str, pokaz_bk_code = :pokaz_bk_code");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("pokaz_code")->AsString() = pokaz_code;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
    q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("vert_str")->AsString() = vert_str;
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdObjekt2(int pokaz_id, std::string pokaz_name, int pokaz_vert, std::string pokaz_bk_code,
                             TDateTime pokaz_beg_date, TDateTime pokaz_end_date, int level_sign, std::string vert_str) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_name=:pokaz_name, pokaz_vert = :pokaz_vert," );
    q->SQL->Add("pokaz_bk_code = :pokaz_bk_code, pokaz_beg_date=:pokaz_beg_date," );
    q->SQL->Add("pokaz_end_date=:pokaz_end_date," );
    q->SQL->Add("level_sign=:level_sign, vert_str = :vert_str");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ParamByName("pokaz_beg_date")->AsDateTime() = pokaz_beg_date;
    q->ParamByName("pokaz_end_date")->AsDateTime() = pokaz_end_date;
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("vert_str")->AsString() = vert_str;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdOwnerId(int plan_id, int pokaz_id, int pokaz_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

 
    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Pokaztl");
        q->SQL->Add("SET pokaz_owner_id = :pokaz_owner_id");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id");
        q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ExecSQL();
       //MAK q->Free();
delete q;

        TDB_PlanStr::ChangeOwnerId(plan_id, pokaz_id, pokaz_owner_id);

         
    } catch (...) {
         
    }
}

void TDB_Pokaztl::UpdLevelText(int pokaz_id, std::string level_text){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET level_text = :level_text");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("level_text")->AsString() = level_text;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::updNp_vid_id(int pokaz_id, int newNp_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET np_vid_id = :np_vid_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("np_vid_id")->AsInteger() = newNp_vid_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

std::string TDB_Pokaztl::GetLevelText(int pokaz_id) {
    std::string level_text;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT level_text");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        level_text = q->FieldByName("level_text")->AsString();
    }
   //MAK q->Free();
delete q;
    return level_text;
}

void TDB_Pokaztl::UpdlevelSign(int pokaz_id, int level_sign) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET level_sign = :level_sign");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("level_sign")->AsInteger() = level_sign;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdPokazName(int pokaz_id, std::string pokaz_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_name = :pokaz_name");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdcreateInUpd(int pokaz_id, int createInUpd) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET createInUpd = :createInUpd");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("createInUpd")->AsInteger() = createInUpd;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::GetCreateInUpd(int pokaz_id) {
    if (pokaz_id == 0) return 0;

    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT createInUpd");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("createInUpd")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::UpdCodeBK(int pokaz_id, const std::string pokaz_bk_code) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_bk_code = :pokaz_bk_code");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdPokazVertTo1(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_is_razd, pokaz_vert");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (q->FieldByName("pokaz_is_razd")->AsInteger() != 2) return;
    if (q->FieldByName("pokaz_vert")->AsInteger() > 0) return;

    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vert = 1");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
    TDB_Cell::Updcell_is_evaTo1(pokaz_id);
}

void TDB_Pokaztl::UpdPokazVertTo3(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_is_razd, pokaz_vert");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (q->FieldByName("pokaz_is_razd")->AsInteger() != 2) return;
    if (q->FieldByName("pokaz_vert")->AsInteger() == 3) return;

    q->Close();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vert = 3");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdPokazVert(int pokaz_id, int pokaz_vert) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vert = :pokaz_vert");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_vert")->AsInteger() = pokaz_vert;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdPokaz_is_kv(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_is_kv = 1");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
    TDB_Cell::Updcell_is_eval1(pokaz_id);
}

void TDB_Pokaztl::UpdPokaz_is_mes(int pokaz_id, int kv) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_is_mes = 1");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
    TDB_Cell::Updcell_is_eval2(pokaz_id, kv);
}

int TDB_Pokaztl::getPokaz_vsego_id(int pokaz_id) {
    if (pokaz_id == 0) return 0;

    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_vsego_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_vsego_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::UpdPokaz_vsego_id(int pokaz_id, int pokaz_vsego_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = :pokaz_vsego_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_vsego_id")->AsInteger() = pokaz_vsego_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdOrtIzm(int pokaz_id, int pokaz_is_mes, int pokaz_is_kv, int pokaz_is_god, 
                             int pokaz_is_ist_fin, int pokaz_is_din_izm, int pokaz_is_const) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_is_mes = :pokaz_is_mes, pokaz_is_kv = :pokaz_is_kv,");
    q->SQL->Add("pokaz_is_god = :pokaz_is_god, pokaz_is_ist_fin = :pokaz_is_ist_fin,");
    q->SQL->Add("pokaz_is_din_izm = :pokaz_is_din_izm, pokaz_is_const = :pokaz_is_const");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_is_mes")->AsInteger() = pokaz_is_mes;
    q->ParamByName("pokaz_is_kv")->AsInteger() = pokaz_is_kv;
    q->ParamByName("pokaz_is_god")->AsInteger() = pokaz_is_god;
    q->ParamByName("pokaz_is_ist_fin")->AsInteger() = pokaz_is_ist_fin;
    q->ParamByName("pokaz_is_din_izm")->AsInteger() = pokaz_is_din_izm;
    q->ParamByName("pokaz_is_const")->AsInteger() = pokaz_is_const;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::GetOrtIzm(int pokaz_id, int& pokaz_is_mes, int& pokaz_is_kv, int& pokaz_is_god, 
                             int& pokaz_is_ist_fin, int& pokaz_is_din_izm, int& pokaz_is_const) {
    int pokaz_is_razd = TDB_Pokaztl::GetPokazIsRazd(pokaz_id);
    if (pokaz_is_razd == 1) { // It's a section
        int new_pokaz_id = TDB_Pokaztl::getPokaz_vsego_id(pokaz_id);
        if (new_pokaz_id == 0) { // Total for the section does not exist yet
            new_pokaz_id = TDB_Pokaztl::GetPokazOwnerId(pokaz_id);
            new_pokaz_id = TDB_Pokaztl::getPokaz_vsego_id(new_pokaz_id);
        }
        pokaz_id = new_pokaz_id;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_is_mes, pokaz_is_kv, pokaz_is_god, pokaz_is_ist_fin, pokaz_is_din_izm, pokaz_is_const");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    
    pokaz_is_mes = q->FieldByName("pokaz_is_mes")->AsInteger();
    pokaz_is_kv = q->FieldByName("pokaz_is_kv")->AsInteger();
    pokaz_is_god = q->FieldByName("pokaz_is_god")->AsInteger();
    pokaz_is_ist_fin = q->FieldByName("pokaz_is_ist_fin")->AsInteger();
    pokaz_is_din_izm = q->FieldByName("pokaz_is_din_izm")->AsInteger();
    pokaz_is_const = q->FieldByName("pokaz_is_const")->AsInteger();

    q->Close();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdOrtIzm1(int pokaz_id, int pokaz_is_ist_fin) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_is_ist_fin = :pokaz_is_ist_fin");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_is_ist_fin")->AsInteger() = pokaz_is_ist_fin;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdEdIzm_PokazNP(int pokaz_base_id, int np_vid_id, int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET edizm_id = :edizm_id");
    q->SQL->Add("WHERE pokaz_base_id = :pokaz_base_id");
    q->SQL->Add("AND np_vid_id = :np_vid_id");
    q->ParamByName("pokaz_base_id")->AsInteger() = pokaz_base_id;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

std::string TDB_Pokaztl::GetforUnikIdentif(int pokaz_id) {
    std::string result = "";
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT forUnikIdentif");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("forUnikIdentif")->AsString();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::findByUnikIdentif(std::string forUnikIdentif) {
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE forUnikIdentif = :forUnikIdentif");
    q->ParamByName("forUnikIdentif")->AsString() = forUnikIdentif;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::findByUnikIdentif(std::string forUnikIdentif, int plan_id, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT a.pokaz_id");
    q->SQL->Add("FROM Pokaztl a, Plan_str b");
    q->SQL->Add("WHERE b.plan_id = :plan_id");
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND a.forUnikIdentif = :forUnikIdentif");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->ParamByName("forUnikIdentif")->AsString() = forUnikIdentif;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::findByUnikIdentifGod(std::string forUnikIdentif, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE forUnikIdentif = :forUnikIdentif");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->ParamByName("forUnikIdentif")->AsString() = forUnikIdentif;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::UpdforUnikIdentif(int pokaz_id, std::string forUnikIdentif) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET forUnikIdentif = :forUnikIdentif");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("forUnikIdentif")->AsString() = forUnikIdentif;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdforMultipartDgvr(int dgvr_id, int objectid1, int is_list) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl SET enum_id = :objectid1, is_list = :is_list WHERE pokaz_id = :dgvr_id");
    q->ParamByName("dgvr_id")->AsInteger() = dgvr_id;
    q->ParamByName("objectid1")->AsInteger() = objectid1;
    q->ParamByName("is_list")->AsInteger() = is_list;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdforMultiItogo(int pokaz_id, std::string forMultiItogo, int objekt_class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl SET forMultiItogo = :forMultiItogo, objekt_class_id = :objekt_class_id WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("forMultiItogo")->AsString() = forMultiItogo;
    q->ParamByName("objekt_class_id")->AsInteger() = objekt_class_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_Pokaztl::isInforMultiItogo(int Itogo_idMulti, int field_id) {
    bool Result = false;
    if (Itogo_idMulti == 0)
        return Result;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT forMultiItogo FROM Pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = Itogo_idMulti;
    q->Open();
    if (!q->EoF()) {
        std::string forMultiItogo = q->FieldByName("forMultiItogo")->AsString();
        std::string ws = std::to_string(field_id) + ";";
        if (Pos(ws, forMultiItogo) > 0)
            Result = true;
    }
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_Pokaztl::getIsListForDgvr(int objectid, int& enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (q != nullptr) {
        q->SQL->Clear();
        q->SQL->Add("SELECT is_list, enum_id");
        q->SQL->Add("FROM Pokaztl");
        q->SQL->Add("WHERE pokaz_id = :objectid");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->Open();
        if (!q->EoF()) {
            enum_id = q->FieldByName("enum_id")->AsInteger();
            result = q->FieldByName("is_list")->AsInteger();
        }
    }
       //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::AddNaturP(int pokaz_base_id, int pokaz_is_razd, int pokaz_type,
                            int enum_id, int edizm_id, int np_vid_id, int in_cast) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (q != nullptr) {
        try {
            q->SQL->Clear();
            q->SQL->Add("INSERT INTO Pokaztl");
            q->SQL->Add("(pokaz_base_id, pokaz_is_razd, pokaz_type," );
            q->SQL->Add("enum_id, edizm_id, np_vid_id, create_date, in_cast)");
            q->SQL->Add("values (:pokaz_base_id,:pokaz_is_razd, :pokaz_type," );
            q->SQL->Add(":enum_id, :edizm_id, :np_vid_id, :create_date, :in_cast)");
            q->ParamByName("pokaz_base_id")->AsInteger() = pokaz_base_id;
            q->ParamByName("pokaz_is_razd")->AsInteger() = pokaz_is_razd;
            q->ParamByName("pokaz_type")->AsInteger() = pokaz_type;
            q->ParamByName("enum_id")->AsInteger() = enum_id;
            q->ParamByName("edizm_id")->AsInteger() = edizm_id;
            q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
            q->ParamByName("create_date")->AsDateTime() = Date();
            q->ParamByName("in_cast")->AsInteger() = in_cast;
            q->ExecSQL();
            result = GetIdNP(pokaz_base_id, np_vid_id);
        }
        catch (...) {
           //MAK q->Free();
delete q;
        }
    }
    return result;
}

bool TDB_Pokaztl::DeletePokaztl(int pokazId, int pokazOwnerId, bool isMsg) {
    std::string spokazId = IntToStr(pokazId);
    std::string s, pokazName, nDO;
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    if (qW != nullptr) {
        try {
            qW->SQL->Clear();
            qW->SQL->Add("SELECT pokaz_name, pokaz_is_razd, objekt_class_id, pokaz_beg_date, pokaz_is_ist_fin FROM Pokaztl ");
            qW->SQL->Add("WHERE pokaz_id = " + spokazId);
            qW->Open();
            int PokazIsRazd = qW->FieldByName("pokaz_is_razd")->AsInteger();
            int objekt_class_id = qW->FieldByName("objekt_class_id")->AsInteger();
            int pokaz_is_ist_fin = qW->FieldByName("pokaz_is_ist_fin")->AsInteger();
            TDateTime pokaz_beg_date = qW->FieldByName("pokaz_beg_date")->AsDateTime();
            pokazName = qW->FieldByName("pokaz_name")->AsString();
            int Year, Month, Day;
            DecodeDate(pokaz_beg_date, Year, Month, Day);
            qW->Free();
            double summa;
            if (isMsg) {
                if (PokazIsRazd == 4) { // это объект
                    if (TDB_DenegnPotok::IsPlanGrDgvr(4, Year, pokazId, summa)) {
                        return result;
                    }
                    if (TDB_DenegnPotok::IsPlategkaDgvrGod(YearOf(Date()), pokazId)) {
                        return result;
                    }
                }
            }
            TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
            qW->SQL->Clear();
            qW->SQL->Add("SELECT out_cell_id FROM Cell, Influence ");
            qW->SQL->Add("WHERE pokaz_id = " + spokazId);
            switch (GetDBType()) {
                case TDBType::MSSQL:
                    qW->SQL->Add("AND cell_id = LEFT(in_cell_id, 10)");
                    break;
                case TDBType::Postgre:
                    qW->SQL->Add("AND CASE WHEN LEFT(in_cell_id, 10) <> '' THEN cell_id = cast(LEFT(in_cell_id, 10) as integer) ELSE false END");
                    break;
            }
            qW->Open();
            bool isN = !qW->EoF();
            int out_cell_id = qW->FieldByName("out_cell_id")->AsInteger();
            qW->Free();
            if (isN) {
                int wpokaz_id = TDB_Cell::GetPokaz_idbyCell_id(out_cell_id);
                return result;
            }
            if (isMsg) {
                if (PokazIsRazd == 2) { // это показатель
                    int vid_otc;
                    if (TDB_CellValue::IsSumma(vid_otc, Year, pokazId, summa)) {
                        switch (vid_otc) {
                            case 2:
                                s = "(заявка)";
                                break;
                            case 3:
                                s = "(план)";
                                break;
                            case 4:
                                s = "(уточ план)";
                                break;
                            case 5:
                                s = "(факт)";
                                break;
                        }
                    }
                }
            }
            if (PokazIsRazd == 2 || PokazIsRazd == 3 || PokazIsRazd == 4 || PokazIsRazd == 5) {
                TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
                qW->SQL->Clear();
                qW->SQL->Add("SELECT pokaz_id FROM Pokaztl ");
                qW->SQL->Add("WHERE pokaz_owner_id = " + spokazId);
                qW->Open();
                isN = !qW->EoF();
                qW->Free();
                if (isN) {
                    return result;
                }
                if (PokazIsRazd == 4 || PokazIsRazd == 5) { // это объект
                    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
                    if (GetisDocumConected()) {
                        qW->SQL->Clear();
                        qW->SQL->Add("SELECT docum_id FROM Cell_value");
                        qW->SQL->Add("WHERE objectid = " + spokazId);
                        qW->SQL->Add("AND docum_id IS NOT NULL");
                        qW->Open();
                        TDB_CellValue::DeleteDocums(qW);
                    }
                    qW->SQL->Clear();
                    qW->SQL->Add("DELETE FROM Cell_value");
                    qW->SQL->Add("WHERE objectid = " + spokazId);
                    qW->ExecSQL();
                    qW->Free();
                    if (PokazIsRazd == 4) { // это объект
                        if (TDB_Pokaztl::GetPokazName(objekt_class_id) == "Денежное обязательство") {
                            nDO = TDB_KeyOfObjects::GetKeyByIdObject(pokazId);
                            if (nDO != "") {
                                TDB_GurnProtokol::Protokol_AddItem("Удаление ДО", GetUserLogin(), GetUserId(),
                                                                    pokazId, nDO);
                            }
                        }
                    }
                    qW = new TQueryUni(DefDBconnect.Conn);
                    qW->SQL->Add("DELETE FROM KeyOfObjects");
                    qW->SQL->Add("WHERE objectid = " + spokazId);
                    qW->ExecSQL();
                    qW->Free();
                    qW = new TQueryUni(DefDBconnect.Conn);
                    qW->SQL->Add("DELETE FROM LinksObjects");
                    qW->SQL->Add("WHERE objectid = " + spokazId);
                    qW->ExecSQL();
                    qW->Free();
                    TDB_DenegnPotok::DeleteItemByObj(StrToInt(spokazId));
                    qW = new TQueryUni(DefDBconnect.Conn);
                    try {
                        switch (GetDBType()) {
                            case TDBType::MSSQL:
                                qW->SQL->Add("if OBJECT_ID('ObjectPlan') is not null");
                                qW->SQL->Add("begin");
                                qW->SQL->Add("DELETE FROM ObjectPlan");
                                qW->SQL->Add("WHERE objectid = " + spokazId);
                                qW->SQL->Add("end");
                                qW->ExecSQL();
                                qW->Close();
                                break;
                            case TDBType::Postgre:
                                qW->SQL->Add("DO $$ BEGIN IF EXISTS (SELECT 1 FROM information_schema.tables WHERE table_schema = 'public' AND table_name = 'ObjectPlan') THEN");
                                qW->SQL->Add("DELETE FROM ObjectPlan");
                                qW->SQL->Add("WHERE objectid = " + spokazId + ";");
                                qW->SQL->Add("END IF; END $$");
                                qW->ExecSQL();
                                qW->Close();
                                break;
                        }
                    }
                    catch (...) {
                        ;
                    }
                    qW->Free();
                }
                qW = new TQueryUni(DefDBconnect.Conn);
                qW->SQL->Clear();
                qW->SQL->Add("SELECT pokaz_id FROM Pokaztl ");
                qW->SQL->Add("WHERE pokaz_base_id = " + spokazId);
                qW->Open();
                while (!qW->EoF()) {
                    DelPokaztl(qW->FieldByName("pokaz_id")->AsInteger());
                    qW->Next();
                }
                qW->Free();
                DelPokaztl(pokazId);
                if (PokazIsRazd == 3 || PokazIsRazd == 5) { // показатель Итого по разделу
                    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
                    qW->SQL->Add("UPDATE Pokaztl ");
                    qW->SQL->Add("SET pokaz_vsego_id=0 ");
                    qW->SQL->Add("WHERE pokaz_id = " + IntToStr(pokazOwnerId));
                    qW->ExecSQL();
                    qW->Free();
                }
            }
            if (PokazIsRazd == 1) { // это раздел
                TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
                qW->SQL->Clear();
                qW->SQL->Add("SELECT pokaz_id FROM Pokaztl ");
                qW->SQL->Add("WHERE pokaz_owner_id = " + spokazId);
                qW->SQL->Add(" AND pokaz_is_razd <> 3"); // без 'Итого'
                qW->Open();
                isN = !qW->EoF();
                qW->Free();
                if (isN) {
                    return result;
                }
                qW = new TQueryUni(DefDBconnect.Conn);
                qW->SQL->Clear();
                qW->SQL->Add("SELECT pokaz_vsego_id FROM Pokaztl ");
                qW->SQL->Add("WHERE pokaz_id = " + spokazId);
                qW->Open();
                int PokazVsegoId = qW->FieldByName("pokaz_vsego_id")->AsInteger();
                qW->Free();
                if (PokazVsegoId > 0) {
                    DelPokaztl(PokazVsegoId);
                }
                DelPokaztl(pokazId);
            }
            TDB_GurnProtokol::Protokol_AddItem("Удаление показ-я/меропр-я/объекта/договора", GetUserLogin(),
                                                GetUserId(), pokazId, pokazName);
            result = true;
        } catch (...) {
            ;
        }
    }
    return result;
}

bool TDB_Pokaztl::IsklucitPokaztl(int plan_id, int pokazId, int pokazOwnerId) {
    std::string spokazId = IntToStr(pokazId);
    bool result = false;
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    int N, PokazIsRazd;
    if (qW != nullptr) {
        qW->SQL->Clear();
        qW->SQL->Add("SELECT plan_id FROM Plan_str");
        qW->SQL->Add("WHERE pokaz_id = :pokazId");
        qW->ParamByName("pokazId")->AsInteger() = pokazId;
        qW->Open();
        N = qW->RecordCount();
        qW->Free();
        if (N == 1) {
            return result;
        }
        qW->Free();
        TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Clear();
        qW->SQL->Add("SELECT pokaz_is_razd FROM Pokaztl ");
        qW->SQL->Add("WHERE pokaz_id = " + spokazId);
        qW->Open();
        PokazIsRazd = qW->FieldByName("pokaz_is_razd")->AsInteger();
        qW->Free();
        if (PokazIsRazd == 2 || PokazIsRazd == 3 || PokazIsRazd == 4) {
            TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
            qW->SQL->Clear();
            qW->SQL->Add("SELECT pokaz_id FROM Plan_str");
            qW->SQL->Add("WHERE plan_id = :plan_id");
            qW->SQL->Add("AND plan_str_owner_id = :pokazId");
            qW->ParamByName("plan_id")->AsInteger() = plan_id;
            qW->ParamByName("pokazId")->AsInteger() = pokazId;
            qW->Open();
            N = 0;
            if (!qW->EoF()) N = 1;
            qW->Free();
            if (N > 0) {
                return result;
            }
            IsklPokaztl(plan_id, pokazId);
        }
        result = true;
    }
    return result;
}

void TDB_Pokaztl::DelPokaztl(int aPokazId) {

    try {
        // Delete from Plan_str
        TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Plan_str ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->ExecSQL();
        qW->Free();

        // Delete from Plan_str1n
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Plan_str1n");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->ExecSQL();
        qW->Free();

        // Delete from Formula
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Formula ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->ExecSQL();
        qW->Free();

        // Delete from Influence
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Influence ");
        qW->SQL->Add("WHERE out_cell_id IN");
        qW->SQL->Add("( SELECT cell_id FROM Cell ");
        qW->SQL->Add("  WHERE pokaz_id = " + IntToStr(aPokazId) + ")");
        qW->ExecSQL();
        qW->Free();

        // Delete from CNP_value
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM CNP_value");
        qW->SQL->Add("WHERE pokaz_id = :pokaz_id");
        qW->ParamByName("pokaz_id")->AsInteger() = aPokazId;
        qW->ExecSQL();
        qW->Free();

        // Delete from Cell_value, Cell, Cell_value_bp, Utoc_plana
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("SELECT cell_id FROM Cell ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->Open();
        qW->First();
        TQueryUni *qq = new TQueryUni(DefDBconnect.Conn);
        while (!qW->EoF()) {
            if (GetisDocumConected()) {
                qq->SQL->Clear();
                qq->SQL->Add("SELECT docum_id FROM Cell_value");
                qq->SQL->Add("WHERE cell_id = " + qW->FieldByName("cell_id")->AsString());
                qq->SQL->Add("AND docum_id IS NOT NULL");
                qq->Open();
                TDB_CellValue::DeleteDocums(qq);
            }
            qq->SQL->Clear();
            qq->SQL->Add("DELETE FROM Cell_value ");
            qq->SQL->Add("WHERE cell_id = " + qW->FieldByName("cell_id")->AsString());
            qq->ExecSQL();

            // Delete from Cell_value_bp
            qq->SQL->Clear();
            qq->SQL->Add("DELETE FROM Cell_value_bp ");
            qq->SQL->Add("WHERE cell_id = " + qW->FieldByName("cell_id")->AsString());
            qq->ExecSQL();

            // Delete from Utoc_plana
            qq->SQL->Clear();
            qq->SQL->Add("DELETE FROM Utoc_plana ");
            qq->SQL->Add("WHERE cell_id_otkuda = " + qW->FieldByName("cell_id")->AsString());
            qq->SQL->Add("OR cell_id_kuda = " + qW->FieldByName("cell_id")->AsString());
            qq->ExecSQL();

            // Delete from Denegn_potok
            TDB_DenegnPotok::DeleteItemByCellId(qW->FieldByName("cell_id")->AsInteger());

            qW->Next();
        }
        qq->Free();
        qW->Free();

        // Delete from Cell
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Cell ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->ExecSQL();
        qW->Free();

        // Delete from Pokaz_din_izm
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Pokaz_din_izm ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->ExecSQL();
        qW->Free();

        // Delete from Pokaztl
        qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Pokaztl ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->ExecSQL();
        qW->Free();

        // Commit transaction on success
         
    } catch (...) {
        // Rollback transaction on failure
         
    }
}

void TDB_Pokaztl::IsklPokaztl(int aPlan_id, int aPokazId) {
    bool bInTransaction = false;

    
    try {
        // Delete from Plan_str
        TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("DELETE FROM Plan_str ");
        qW->SQL->Add("WHERE plan_id = :aPlan_id");
        qW->SQL->Add("AND pokaz_id = :aPokazId");
        qW->ParamByName("aPlan_id")->AsInteger() = aPlan_id;
        qW->ParamByName("aPokazId")->AsInteger() = aPokazId;
        qW->ExecSQL();
        qW->Free();

        // Delete from Cell_value
        qW = new TQueryUni(DefDBconnect.Conn);
        TQueryUni *qq = new TQueryUni(DefDBconnect.Conn);
        qW->SQL->Add("SELECT cell_id FROM Cell ");
        qW->SQL->Add("WHERE pokaz_id = " + IntToStr(aPokazId));
        qW->Open();
        qW->First();
        while (!qW->EoF()) {
            qq = new TQueryUni(DefDBconnect.Conn);
            if (GetisDocumConected()) {
                qq->SQL->Clear();
                qq->SQL->Add("SELECT docum_id FROM Cell_value");
                qq->SQL->Add("WHERE cell_id = " + qW->FieldByName("cell_id")->AsString());
                qq->SQL->Add("AND plan_id = :aPlan_id");
                qq->SQL->Add("AND docum_id IS NOT NULL");
                qq->ParamByName("aPlan_id")->AsInteger() = aPlan_id;
                qq->Open();
                TDB_CellValue::DeleteDocums(qq);
            }
            qq->SQL->Clear();
            qq->SQL->Add("DELETE FROM Cell_value ");
            qq->SQL->Add("WHERE cell_id = " + qW->FieldByName("cell_id")->AsString());
            qq->SQL->Add("AND plan_id = :aPlan_id");
            qq->ParamByName("aPlan_id")->AsInteger() = aPlan_id;
            qq->ExecSQL();

            // Delete from Utoc_plana
            qq->SQL->Clear();
            qq->SQL->Add("DELETE FROM Utoc_plana ");
            qq->SQL->Add("WHERE plan_id = :aPlan_id");
            qq->SQL->Add("AND (cell_id_otkuda = " + qW->FieldByName("cell_id")->AsString());
            qq->SQL->Add("OR cell_id_kuda = " + qW->FieldByName("cell_id")->AsString() + ")");
            qq->ParamByName("aPlan_id")->AsInteger() = aPlan_id;
            qq->ExecSQL();

            // Delete from Denegn_potok
            TDB_DenegnPotok::DeleteItemByPlanCellId(aPlan_id, qW->FieldByName("cell_id")->AsInteger());

            qW->Next();
        }
        delete qq;
        qW->Free();

        // Commit transaction on success
         
    } catch (...) {
        // Rollback transaction on failure
         
    }
}

void TDB_Pokaztl::GetNaturP(TQueryUni *qIn, int pokaz_base_id, int np_vid_id) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl a, Nat_pokaz_vid b ");
    qIn->SQL->Add("WHERE a.pokaz_base_id = :pokaz_base_id");
    qIn->SQL->Add("AND a.np_vid_id = :np_vid_id");
    qIn->SQL->Add("AND b.np_vid_id = a.np_vid_id");
    qIn->ParamByName("pokaz_base_id")->AsInteger() = pokaz_base_id;
    qIn->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    qIn->Open();
}

int TDB_Pokaztl::getNp_vid_id(int pokaz_id) {
    int result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT np_vid_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("np_vid_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetId() {
    int result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as pokaz_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Pokaztl', 'pokaz_id')) as pokaz_id");
            break;
    }
    q->Open();
    result = q->FieldByName("pokaz_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetIdbyName(int pokaz_owner_id, std::string pokaz_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND pokaz_name = :pokaz_name");
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->Open();

    if (q->EoF()) {
        throw std::runtime_error("Показатель по pokaz_owner_id = " + std::to_string(pokaz_owner_id) + " и pokaz_name = " + pokaz_name + " не найден");
        result = 0;
    } else {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

TDateTime TDB_Pokaztl::GetDateCreate(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDateTime result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT create_date");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("create_date")->AsDateTime();
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetIdbyNameYear(int pokaz_owner_id, std::string pokaz_name, int CurrYear) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND pokaz_name = :pokaz_name");
    
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :CurrYear BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :CurrYear BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->ParamByName("CurrYear")->AsInteger() = CurrYear;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetIdClass(std::string class_name, bool isMSG) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_name = :pokaz_name");
    q->SQL->Add("AND pokaz_type = 1");
    q->ParamByName("pokaz_name")->AsString() = class_name;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    } else {
        if (isMSG) {
            throw std::runtime_error("В метабазе отсутствует описание класса \"" + class_name + "\". Следует обратиться к Разработчику");
        }
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetIdNP(int pokaz_base_id, int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_base_id = :pokaz_base_id");
    q->SQL->Add("AND np_vid_id = :np_vid_id");
    q->ParamByName("pokaz_base_id")->AsInteger() = pokaz_base_id;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetIdNP_BP(int pokaz_base_id, int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_base_id = :pokaz_base_id");
    q->SQL->Add("AND np_vid_id = :np_vid_id");
    q->SQL->Add("AND pokaz_type = 4");
    q->SQL->Add("AND enum_id = 1");
    q->ParamByName("pokaz_base_id")->AsInteger() = pokaz_base_id;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Pokaztl::IsBaseClass(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_base_id = :pokaz_base_id");
    q->ParamByName("pokaz_base_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF()) {
        result = true;
    }

   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Pokaztl::IsSubItem(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND pokaz_is_razd <> 3");  // without 'Итого'
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF()) {
        result = true;
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetLinkClass(int class_id, PArrLinkClasses LinkClasses) {
    TQueryUni* q1 = new TQueryUni(DefDBconnect.Conn);
    try {
        q1->SQL->Add("SELECT * FROM Pokaztl WHERE pokaz_owner_id = :pokaz_owner_id");
        q1->ParamByName("pokaz_owner_id")->SetValue(class_id);
        q1->Open();

        int count = 0;
        while (!q1->EoF()) {
            int pokaz_is_razd = q1->FieldByName("pokaz_is_razd")->AsInteger();

            if (pokaz_is_razd == 1) { // раздел
                GetLinkClass1(q1->FieldByName("pokaz_id")->AsInteger(), count, LinkClasses);
            } else if (pokaz_is_razd == 2 && count < 50) { // показатель
                if (q1->FieldByName("pokaz_type")->AsInteger() == 7) { // связь
                    if (count >= 49) {
                        break;
                    }
                    count++;
                    (*LinkClasses)[count].pokaz_id = q1->FieldByName("pokaz_id")->AsInteger();
                    (*LinkClasses)[count].class_id = q1->FieldByName("enum_id")->AsInteger();
                    (*LinkClasses)[count].class_name = GetPokazName(q1->FieldByName("enum_id")->AsInteger());
                } else if (q1->FieldByName("pokaz_vert")->AsInteger() > 0) { // составной показатель
                    GetLinkClass1(q1->FieldByName("pokaz_id")->AsInteger(), count, LinkClasses);
                }
            }
            q1->Next();
        }
        delete q1;
        return count;
    } catch (...) {
        delete q1;
        throw;
    }
}

void TDB_Pokaztl::GetLinkClass1(int pokaz_id, int& N, PArrLinkClasses pLinkClasses) {
    TQueryUni* qW = new TQueryUni(DefDBconnect.Conn);
    try {
        qW->SQL->Add("SELECT * FROM Pokaztl WHERE pokaz_owner_id = :pokaz_owner_id");
        qW->ParamByName("pokaz_owner_id")->SetValue(pokaz_id);
        qW->Open();

        while (!qW->EoF() && N < 50) {
            int pokaz_is_razd = qW->FieldByName("pokaz_is_razd")->AsInteger();

            if (pokaz_is_razd == 1) { // раздел
                GetLinkClass1(qW->FieldByName("pokaz_id")->AsInteger(), N, pLinkClasses);
            } else if (pokaz_is_razd == 2) { // показатель
                if (qW->FieldByName("pokaz_type")->AsInteger() == 7) { // связь
                    if (N >= 49) {
                        break;
                    }
                    N++;
                    (*pLinkClasses)[N].pokaz_id = qW->FieldByName("pokaz_id")->AsInteger();
                    (*pLinkClasses)[N].class_id = qW->FieldByName("enum_id")->AsInteger();
                    (*pLinkClasses)[N].class_name = GetPokazName(qW->FieldByName("enum_id")->AsInteger());
                } else if (qW->FieldByName("pokaz_vert")->AsInteger() > 0) { // составной показатель
                    GetLinkClass1(qW->FieldByName("pokaz_id")->AsInteger(), N, pLinkClasses);
                }
            }
            qW->Next();
        }
        delete qW;
    } catch (...) {
        delete qW;
        throw;
    }
}

int TDB_Pokaztl::getPeriodicn(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_is_const, pokaz_is_god, pokaz_is_kv, pokaz_is_mes FROM Pokaztl WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    int result = 0;
    if (!q->EoF()) {
        if (q->FieldByName("pokaz_is_const")->AsInteger() == 1) {
            result = 0;
        }
        if (q->FieldByName("pokaz_is_god")->AsInteger() == 1) {
            result = 1;
        }
        if (q->FieldByName("pokaz_is_kv")->AsInteger() == 1) {
            result = 2;
        }
        if (q->FieldByName("pokaz_is_mes")->AsInteger() == 1) {
            result = 3;
        }
    }

    return result;
}


void TDB_Pokaztl::SetNewEndYear(int pokaz_id, int Year) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_end_date = :pokaz_end_date");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_end_date")->AsDateTime() = EncodeDate(Year, 12, 31);
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetKeyField(int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET objekt_class_id = 0");
    q->SQL->Add("WHERE pokaz_owner_id = :class_id");
    q->SQL->Add("AND pokaz_is_razd = 2");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET objekt_class_id = :class_id, isSurgtName = 0");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetKeyFieldAvtoNumber(int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET objekt_class_id = 0");
    q->SQL->Add("WHERE pokaz_owner_id = :class_id");
    q->SQL->Add("AND pokaz_is_razd = 2");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET objekt_class_id = :class_id, isSurgtName = 1");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UnSetKeyField(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET objekt_class_id = 0, isSurgtName = 0");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetFieldToItogo(int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = 0");
    q->SQL->Add("WHERE pokaz_owner_id = :class_id");
    q->SQL->Add("AND pokaz_is_razd = 2");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = 1");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UnSetFieldToItogo(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = 0");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetFieldToAgregator(int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = 4");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::SetFieldToAgregatorMrpr(int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = 8");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UnSetFieldToAgregator(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_vsego_id = 0");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::findFieldToItogo(int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_owner_id = :class_id");
    q->SQL->Add("AND pokaz_is_razd = 2");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->Open();
    if (!q->EoF())
        Result = q->FieldByName("pokaz_id")->AsInteger();
    
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_Pokaztl::GetPokazIdbyPokazBkCode(std::string pokaz_bk_code, int CurrYear, std::string aCastName) {
    if (pokaz_bk_code.empty())
        return 0;
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Pokaztl::CreateTmpPokaztlTB(CurrYear, aCastName);
    
    int Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("FROM #PokaztlTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("FROM PokaztlTB");
            break;
    }
    if (pokaz_bk_code[0] == '_')
        q->SQL->Add("WHERE pokaz_bk_code LIKE :pokaz_bk_code");
    else
        q->SQL->Add("WHERE pokaz_bk_code = :pokaz_bk_code");
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->Open();
    if (!q->EoF())
        Result = q->FieldByName("pokaz_id")->AsInteger();
    
   //MAK q->Free();
delete q;
    TDB_Pokaztl::DropTmpPokaztlTB();
    return Result;
}

void TDB_Pokaztl::CreateTmpPokaztlTB(int CurrYear, std::string aCastNameTB) {
    int root_Pokaz_id;
    TDateTime ToDate;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Pokaztl::DropTmpPokaztlTB();

    root_Pokaz_id = TDB_Pokaztl::GetIdbyNameYear(0, aCastNameTB, CurrYear);
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Clear();
            q->SQL->Add("CREATE TABLE #PokaztlTB (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL,");
            q->SQL->Add("pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code char(20) NULL, is_obrab tinyint NULL)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP1 ON #PokaztlTB (pokaz_id)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP2 ON #PokaztlTB (pokaz_bk_code)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            break;
        case TDBType::Postgre:
            q->SQL->Clear();
            q->SQL->Add("CREATE TEMP TABLE PokaztlTB (pokaz_id integer, pokaz_is_razd smallint NULL, pokaz_owner_id integer NULL,");
            q->SQL->Add("pokaz_vert smallint NULL, pokaz_type smallint NULL, pokaz_bk_code character varying(20) NULL, is_obrab smallint NULL);");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP1 ON PokaztlTB (pokaz_id);");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP2 ON PokaztlTB (pokaz_bk_code);");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            break;
    }
   //MAK q->Free();
    delete q;

    ToDate = EncodeDate(CurrYear, 12, 30);

    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn);
    sp->storedProcName = "CreateTmpPokaztlTB";
    switch (GetDBType()) {
        case TDBType::MSSQL:
            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aToDate", ptInput);
            sp->Params->ParamByName("@aPokaz_id")->AsInteger() = root_Pokaz_id;
            sp->Params->ParamByName("@aToDate")->AsDateTime() = ToDate;
            break;
        case TDBType::Postgre:
            sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "aToDate", ptInput);
            sp->Params->ParamByName("aPokaz_id")->AsInteger() = root_Pokaz_id;
            sp->Params->ParamByName("aToDate")->SetValue(ToDate);
            break;
    }
    sp->ExecProc();
    delete sp;
}

void TDB_Pokaztl::DropTmpPokaztlTB() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("if object_id('tempdb..#PokaztlTB') is not null");
            q->SQL->Add("DROP TABLE #PokaztlTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("DROP TABLE IF EXISTS PokaztlTB;");
            break;
    }
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::GetIdByKBKinTmpPokaztlTB(std::string KBK) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("FROM #PokaztlTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("FROM PokaztlTB");
            break;
    }
    q->SQL->Add("WHERE pokaz_bk_code = :pokaz_bk_code");
    q->ParamByName("pokaz_bk_code")->AsString() = KBK;
    q->Open();
    result = q->FieldByName("pokaz_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetIdItogoRazd(int pokaz_id_razd) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND pokaz_is_razd = 3");
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_id_razd;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_Pokaztl::isTBCastLimit(int aPokaz_id) {
    int wPokaz_id;
    std::string wPokaz_name;
    int wPokaz_is_razd;
    int wPokaz_type;
    int wPokaz_vert;

    GetRootPokaztl(aPokaz_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
    return (wPokaz_name.find("Лимиты") != std::string::npos);
}

int TDB_Pokaztl::isTBCast_Limit_OrNo(int aPokaz_id) {
    int wPokaz_id;
    std::string wPokaz_name;
    int wPokaz_is_razd;
    int wPokaz_type;
    int wPokaz_vert;
    int owner_id;

    GetRootPokaztl(aPokaz_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
    int Result = 0;

    if (wPokaz_name.find("Горбюджет:") != std::string::npos) {
        if (wPokaz_name.find("Лимиты") != std::string::npos) 
            Result = 1;
        else 
            Result = 2;
    } else {
        Result = 3;
        owner_id = aPokaz_id;
        while (true) {
            owner_id = GetPokazOwnerId(owner_id);
            if (owner_id == 0) 
                return Result;
            wPokaz_name = GetPokazName(owner_id);
            if (wPokaz_name.find(" Р") != std::string::npos) 
                Result = 1;
            else if (wPokaz_name.find(" П") != std::string::npos) 
                Result = 2;
            if (Result == 1 || Result == 2) 
                return Result;
        }
    }
    return Result;
}

bool TDB_Pokaztl::isTBCastLimit0(int cast_id) {
    std::string wPokaz_name = GetPokazName(cast_id);
    return (wPokaz_name.find("Лимиты") != std::string::npos);
}

bool TDB_Pokaztl::isTBCast(int aPokaz_id) {
    int wPokaz_id;
    std::string wPokaz_name;
    int wPokaz_is_razd;
    int wPokaz_type;
    int wPokaz_vert;

    GetRootPokaztl(aPokaz_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
    return (wPokaz_name.find("Горбюджет:") != std::string::npos);
}

bool TDB_Pokaztl::EstLiPokazBKCode(int pokaz_owner_id, std::string pokaz_bk_code, int god) {
    int wPokaz_id, cast_id;
    std::string wPokaz_name;
    int wPokaz_is_razd, wPokaz_type, wPokaz_vert;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_owner_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_bk_code = :pokaz_bk_code");
    // Depending on the database type, adjust the SQL query accordingly
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN Year(pokaz_beg_date) AND Year(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->ParamByName("pokaz_bk_code")->AsString() = pokaz_bk_code;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    if (q->EoF()) {
       //MAK q->Free();
delete q;
        return false;
    }
    GetRootPokaztl(pokaz_owner_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
    cast_id = wPokaz_id;
    while (!q->EoF()) {
        pokaz_owner_id = q->FieldByName("pokaz_owner_id")->AsInteger();
        if (pokaz_owner_id == 0) {
            if (cast_id == wPokaz_id) {
               //MAK q->Free();
delete q;
                return true;
            }
            break;
        }
        GetRootPokaztl(pokaz_owner_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
        if (cast_id == wPokaz_id) {
           //MAK q->Free();
delete q;
            return true;
        }
        q->Next();
    }
   //MAK q->Free();
delete q;
    return false;
}

int TDB_Pokaztl::getSvobodnijPokazId() {
    int pokaz_id, Pokaz_idPred;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    // Assuming q is connected to the database
    Pokaz_idPred = 999;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id > 999");
    q->SQL->Add("ORDER BY pokaz_id");
    q->Open();
    while (!q->EoF()) {
        pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        if (pokaz_id == (Pokaz_idPred + 1)) {
            Pokaz_idPred = pokaz_id;
            q->Next();
        } else {
           //MAK q->Free();
delete q;
            return Pokaz_idPred + 1;
        }
    }
   //MAK q->Free();
delete q;
    return 0; // Return 0 if no free pokaz_id is found
}

void TDB_Pokaztl::AddIdSvob(bool isItogo, int aPokaz_id, int apokaz_owner_id, int aVidCast, int apokaz_is_razd,
                            int pokaz_type, int is_list) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("create procedure proc_InsertPokaztl");
            q->SQL->Add("  @aPokaz_id int,");
            q->SQL->Add("  @aPokaz_owner_id int");
            q->SQL->Add("as");
            q->SQL->Add("SET IDENTITY_INSERT Pokaztl ON");
            q->SQL->Add("INSERT INTO Pokaztl");
            q->SQL->Add("(pokaz_id, pokaz_owner_id)");
            q->SQL->Add("values (@aPokaz_id, @aPokaz_owner_id)");
            q->SQL->Add("SET IDENTITY_INSERT Pokaztl OFF");
            q->ExecSQL();
            break;
        case TDBType::Postgre:
            q->SQL->Add("CREATE OR REPLACE FUNCTION public.proc_InsertPokaztl(");
            q->SQL->Add("  aPokaz_id integer,");
            q->SQL->Add("  aPokaz_owner_id integer");
            q->SQL->Add(") RETURNS void as $$");
            q->SQL->Add("begin");
            q->SQL->Add("    INSERT INTO Pokaztl");
            q->SQL->Add("    (pokaz_id, pokaz_owner_id)");
            q->SQL->Add("     values (aPokaz_id, aPokaz_owner_id);");
            q->SQL->Add("     SELECT setval(pg_get_serial_sequence('Pokaztl', 'pokaz_id'),");
            q->SQL->Add("         (select max(pokaz_id) from Pokaztl)");
            q->SQL->Add("      );");
            q->SQL->Add("     RETURN;");
            q->SQL->Add("end");
            q->SQL->Add("$$ LANGUAGE plpgsql;");
            break;
    }
    q->ExecSQL();

    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn);
    sp->storedProcName = "proc_InsertPokaztl";

    switch (GetDBType()) {
        case TDBType::MSSQL:
            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
            sp->Params->CreateParam(ftInteger, "@aPokaz_owner_id", ptInput);
            sp->Params->ParamByName("@aPokaz_id")->AsInteger() = aPokaz_id;
            sp->Params->ParamByName("@aPokaz_owner_id")->AsInteger() = apokaz_owner_id;
            break;
        case TDBType::Postgre:
            sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
            sp->Params->CreateParam(ftInteger, "aPokaz_owner_id", ptInput);
            sp->Params->ParamByName("aPokaz_id")->AsInteger() = aPokaz_id;
            sp->Params->ParamByName("aPokaz_owner_id")->AsInteger() = apokaz_owner_id;
            break;
    }
    sp->ExecProc();
    delete sp;

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("DROP PROCEDURE proc_InsertPokaztl");
            break;
        case TDBType::Postgre:
            q->SQL->Add("DROP FUNCTION IF EXISTS public.proc_InsertPokaztl(");
            q->SQL->Add("  aPokaz_id integer,");
            q->SQL->Add("  aPokaz_owner_id integer");
            q->SQL->Add(") CASCADE;");
            break;
    }
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl ");
    q->SQL->Add("SET pokaz_is_razd = :pokaz_is_razd, create_date = :create_date, in_cast = :in_cast");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = aPokaz_id;
    q->ParamByName("create_date")->AsDateTime() = Date();
    q->ParamByName("in_cast")->AsInteger() = aVidCast;
    q->ParamByName("pokaz_is_razd")->AsInteger() = apokaz_is_razd;
    q->ExecSQL();

    if (apokaz_is_razd == 2 || apokaz_is_razd == 0) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Pokaztl ");
        q->SQL->Add("SET pokaz_type = :pokaz_type, is_list = :is_list");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id");
        q->ParamByName("pokaz_id")->AsInteger() = aPokaz_id;
        q->ParamByName("pokaz_type")->AsInteger() = pokaz_type;
        q->ParamByName("is_list")->AsInteger() = is_list;
        q->ExecSQL();
    }
   //MAK q->Free();
delete q;

    TDB_PlanStr::AddElem(isItogo, 1, apokaz_owner_id, aPokaz_id);
}

int TDB_Pokaztl::GetFieldidToMeropr(int aClass_id) {
    int result = 0;
    if (FieldidToMeropr > 0) {
        result = FieldidToMeropr;
        return result;
    }

    TQueryUni *qFld = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetAllSubClassItem(qFld, aClass_id, Date());

    while (!qFld->EoF()) {
        if (qFld->FieldByName("pokaz_type")->AsInteger() == 8) {
            result = qFld->FieldByName("pokaz_id")->AsInteger();
            FieldidToMeropr = qFld->FieldByName("pokaz_id")->AsInteger();
            break;
        }
        qFld->Next();
    }

    if (result == 0) {
        throw std::runtime_error("Не найдено поле связи с ПСЭР в методе В классе Договор отсутствует поле GetFieldidToMeropr");
    }

    delete qFld;
    return result;
}

std::string TDB_Pokaztl::evalSetPolnijNomer(int plan_str_owner_id, int plan_id, int pokaz_id) {
    int predPokaz_id = TDB_PlanStr::getPredPokazId(plan_str_owner_id, plan_id, pokaz_id);

    std::string level_text, level_textOwner;
    int i, wN, Code;
    std::string wsBeg, wsEnd;

    if (predPokaz_id == 0) {
        level_textOwner = TDB_Pokaztl::GetLevelText(plan_str_owner_id);
        if (level_textOwner == "") {
            level_text = "1";
        } else {
            level_text = level_textOwner + "." + "1";
        }
    } else {
        level_text = TDB_Pokaztl::GetLevelText(predPokaz_id);
        wsBeg = ""; 
        wsEnd = "";
        for (i = level_text.length(); i >= 1; i--) {
            if (level_text[i] != '.') {
                wsEnd = level_text[i] + wsEnd;
            } else {
                wsBeg = level_text.substr(1, i);
                break;
            }
        }
        std::istringstream(wsEnd) >> wN;
        wN++;
        level_text = wsBeg + std::to_string(wN);
    }

    return level_text;
}

void TDB_Pokaztl::UpdidSlagaemoeForItogo(int pokaz_id, int field_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET idSlagaemoeForItogo = :field_id, pokaz_vsego_id = :w2");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ParamByName("w2")->AsInteger() = (field_id > 0) ? 2 : 0;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdIdsOwnerFieldItogo(int pokaz_id, std::string idsOwnerFieldItogo) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET idsOwnerFieldItogo = :idsOwnerFieldItogo, pokaz_vsego_id = :w2");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("idsOwnerFieldItogo")->AsString() = idsOwnerFieldItogo;
    q->ParamByName("w2")->AsInteger() = (!idsOwnerFieldItogo.empty()) ? 2 : 0;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::UpdidItogoForSlagaemoer(int pokaz_id, int field_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET idItogoForSlagaemoe = :field_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("field_id")->AsInteger() = field_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::getUpParentByClassId(int currObjectId, int class_id_UpParent) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int pokaz_id = currObjectId;
    while (true) {
        int N = GetOwnerId(pokaz_id);
        if (N <= 0) {
            pokaz_id = 0;
            break;
        }
        pokaz_id = N;
        if (TDB_Pokaztl::GetClassId(N) == class_id_UpParent)
            break;
    }

   //MAK q->Free();
delete q;
    return pokaz_id;
}

void TDB_Pokaztl::updSystFieldObj(std::string stmnt, int ObjectId, int pokaz_id, int systField_id, std::string value) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    switch (systField_id) {
        case 1: q->SQL->Add("SET pokaz_name = :value"); break;
        case 2: q->SQL->Add("SET pokaz_code = :value"); break;
    }
    q->SQL->Add("WHERE pokaz_id = :ObjectId");
    q->ParamByName("ObjectId")->AsInteger() = (ObjectId > 0) ? ObjectId : pokaz_id;
    q->ParamByName("value")->AsString() = value;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Pokaztl::updPokazCode(int pokaz_id, std::string pokaz_code) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_code = :pokaz_code");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_code")->AsString() = pokaz_code;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}


void TDB_Pokaztl::Upd_PokazBaseId(int pokaz_id, int pokaz_base_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_base_id = :pokaz_base_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("pokaz_base_id")->AsInteger() = pokaz_base_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

int TDB_Pokaztl::Get_PokazBaseId(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_base_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_base_id")->AsInteger();

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetObjectIdByCellValueStr(std::string pokaz_name, std::string field_name, std::string cell_value) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT objectid FROM Cell_value");
    q->SQL->Add("WHERE cell_id IN ");
    q->SQL->Add("  (SELECT cell_id FROM Cell WHERE pokaz_id IN ");
    q->SQL->Add("    (SELECT pokaz_id from Pokaztl where pokaz_name = :field_name AND pokaz_owner_id IN ");
    q->SQL->Add("      (SELECT pokaz_id FROM Pokaztl where pokaz_name = :pokaz_name)))");
    q->SQL->Add("AND str_value = :str_value");
    q->ParamByName("str_value")->AsString() = cell_value;
    q->ParamByName("field_name")->AsString() = field_name;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

int TDB_Pokaztl::GetObjectIdByCellValueInt(std::string pokaz_name, std::string field_name, int cell_value) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT objectid FROM Cell_value");
    q->SQL->Add("WHERE cell_id IN ");
    q->SQL->Add("  (SELECT cell_id FROM Cell WHERE pokaz_id IN ");
    q->SQL->Add("    (SELECT pokaz_id from Pokaztl where pokaz_name = :field_name AND pokaz_owner_id IN ");
    q->SQL->Add("      (SELECT pokaz_id FROM Pokaztl where pokaz_name = :pokaz_name)))");
    q->SQL->Add("AND value = :value");
    q->ParamByName("value")->AsInteger() = cell_value;
    q->ParamByName("field_name")->AsString() = field_name;
    q->ParamByName("pokaz_name")->AsString() = pokaz_name;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("objectid")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return result;
}

TDictionary<std::string,std::string> TDB_Pokaztl::GetCellValuesByObjectId(int object_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDictionary<std::string, std::string> result;
    q->SQL->Clear();
    q->SQL->Add("SELECT Cell_value.str_value, Cell_value.value, Cell_Value.value_date, Pokaztl.pokaz_name FROM Cell_value");
    q->SQL->Add("INNER JOIN Cell ON Cell.cell_id = Cell_value.cell_id");
    q->SQL->Add("INNER JOIN Pokaztl ON Cell.pokaz_id = Pokaztl.pokaz_id");
    q->SQL->Add("WHERE objectid = :object_id");
    q->ParamByName("object_id")->AsInteger() = object_id;
    q->Open();
    while (!q->EoF()) {
        std::string value;
        if (!q->FieldByName("str_value")->IsNull()) {
            value = q->FieldByName("str_value")->AsString();
        } else if (!q->FieldByName("value")->IsNull()) {
            value = FloatToStr(q->FieldByName("value")->AsFloat());
        } else if (!q->FieldByName("value_date")->IsNull()) {
            value = DateTimeToStr(q->FieldByName("value_date")->AsDateTime());
        }
        result.Add(q->FieldByName("pokaz_name")->AsString(), value);
        q->Next();
    }

   //MAK q->Free();
delete q;
    return result;
}

void TDB_Pokaztl::CreateTmpPokaztlTBbyId(int CurrYear, int root_Pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDB_Pokaztl::DropTmpPokaztlTB();
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("CREATE TABLE #PokaztlTB (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL,");
            q->SQL->Add(" pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code char(20) NULL, is_obrab tinyint NULL)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP1 ON #PokaztlTB (pokaz_id)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP2 ON #PokaztlTB (pokaz_bk_code)");
            break;
        }
        case TDBType::Postgre: {
            q->SQL->Add("CREATE TEMP TABLE PokaztlTB (pokaz_id integer, pokaz_is_razd smallint NULL, pokaz_owner_id integer NULL,");
            q->SQL->Add(" pokaz_vert smallint NULL, pokaz_type smallint NULL, pokaz_bk_code character varying(20) NULL, is_obrab smallint NULL);");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP1 ON PokaztlTB (pokaz_id);");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP2 ON PokaztlTB (pokaz_bk_code);");
            break;
        }
    }

    q->ExecSQL();
   //MAK q->Free();
delete q;

    TDateTime ToDate = EncodeDate(CurrYear, 12, 30);

    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn);
    sp->storedProcName = "CreateTmpPokaztlTB";

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aToDate", ptInput);
            sp->Params->ParamByName("@aPokaz_id")->AsInteger() = root_Pokaz_id;
            sp->Params->ParamByName("@aToDate")->AsDateTime() = ToDate;
            break;
        }
        case TDBType::Postgre: {
            sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "aToDate", ptInput);
            sp->Params->ParamByName("aPokaz_id")->AsInteger() = root_Pokaz_id;
            sp->Params->ParamByName("aToDate")->AsDateTime() = ToDate;
            break;
        }
    }
    sp->ExecProc();
    delete sp;
}

int TDB_Pokaztl::getLengthKBKforSubPokaz(int pokaz_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("SELECT TOP 1 pokaz_bk_code FROM Pokaztl");
            q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
            break;
        }
        case TDBType::Postgre: {
            q->SQL->Add("SELECT pokaz_bk_code FROM Pokaztl");
            q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
            q->SQL->Add("LIMIT 1");
            break;
        }
    }
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_bk_code")->AsString().length();
   //MAK q->Free();
delete q;
    return result;
}
