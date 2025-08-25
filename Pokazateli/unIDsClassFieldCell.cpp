#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/DB_IstFin.h"
#include "../DB/DB_ComonNPokaz.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../DB/DB_EdIzm.h"
#include "../DB/DB_Cell.h"
#include "unIDsClassFieldCell.h"

bool TIDsClassFieldCell::getClassId(std::string class_name, int pokaz_type, bool isMSG, int& class_id) {
//vernut     auto it = slIDsClassFieldCell.Find(class_name);
//    if (it != slIDsClassFieldCell.end()) {
//        class_id = it->second;
//        return true;
//    }

    // Create a new TQueryUni object
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_name = :pokaz_name");
    q->SQL->Add("AND pokaz_type = :pokaz_type");
    q->ParamByName("pokaz_name")->AsString() = class_name;
    q->ParamByName("pokaz_type")->AsInteger() = pokaz_type;
    q->Open();

    if (!q->EoF()) {
        //vernut  slIDsClassFieldCell[class_name] = q->FieldByName("pokaz_id").AsInteger();
        class_id = q->FieldByName("pokaz_id").AsInteger();
        return true;
    } else {
        if (isMSG) {
            throw std::runtime_error("В метабазе отсутствует описание класса \"" + class_name + "\". Следует обратиться к Разработчику");
        }
        return false;
    }
   //MAK q->Free();
delete q;
}

bool TIDsClassFieldCell::getClassIdByCode(std::string pokaz_code, int pokaz_type, bool isMSG, int class_id) {
    int Index;
    bool Result = false;

    if (GlobalVars->slIDsClassFieldCell->Find(pokaz_code, Index)) {
        class_id = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        Result = true;
        return Result;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_code = :pokaz_code");
    q->SQL->Add("AND pokaz_type = :pokaz_type");
    q->ParamByName("pokaz_code")->AsString() = pokaz_code;
    q->ParamByName("pokaz_type")->AsInteger() = pokaz_type;
    q->Open();

    if (!q->EoF()) {
        GlobalVars->slIDsClassFieldCell->AddObject(pokaz_code,  q->FieldByName("pokaz_id")->AsInteger());
        class_id = q->FieldByName("pokaz_id")->AsInteger();
        Result = true;
       //MAK q->Free();
delete q;
        return Result;
    } else {
        if (isMSG) {
            // SystFuncs->MessageBoxExclamation(("В метабазе отсутствует описание класса \"" +
            //                                  pokaz_code + "\". Следует обратиться к Разработчику").c_str(),
            //                                 MsgVnim);
            //throw std::exception(("В метабазе отсутствует описание класса \"" +
            //                pokaz_code + "\". Следует обратиться к Разработчику").c_str());
        }
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TIDsClassFieldCell::getClassFieldId(int class_id, std::string field_name,
                                          TDateTime CurDate, bool isMSG, int &field_id) {
    int Index;
    std::string Key;
    int parent_class_id;
    std::string ws;
 
    bool Result = false;

    if (class_id == 0)
        return Result;

    Key = std::to_string(class_id) + "_" + field_name;
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        field_id = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        Result = true;
        return Result;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    ws = std::to_string(class_id) + ")";
    TDB_Pokaztl::GetPokaz(q, class_id);
    parent_class_id = q->FieldByName("pokaz_base_id")->AsInteger();
    while (parent_class_id != 0) {
        ws = std::to_string(parent_class_id) + "," + ws;
        TDB_Pokaztl::GetPokaz(q, parent_class_id);
        parent_class_id = q->FieldByName("pokaz_base_id")->AsInteger();
    }
    ws = "(" + ws;

    q->SQL->Clear();
 
    q->SQL->Add("SELECT b.pokaz_name as pokaz_name, b.pokaz_id as pokaz_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_id = :plan_id ");
    q->SQL->Add("AND a.plan_str_owner_id IN " + ws);
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND :CurDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    q->SQL->Add("ORDER BY plan_str_owner_id , plan_str_order");
    q->ParamByName("CurDate")->AsDateTime() = CurDate;
    q->ParamByName("plan_id")->AsInteger() = 1;
    q->Open();
    while (!q->EoF()) {
        Key = std::to_string(class_id) + "_" + q->FieldByName("pokaz_name")->AsString();
        GlobalVars->slIDsClassFieldCell->AddObject(Key, q->FieldByName("pokaz_id")->AsInteger());
        q->Next();
    }

   //MAK q->Free();
delete q;

    Key = std::to_string(class_id) + "_" + field_name;
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        field_id = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        Result = true;
        return Result;
    } else {
        if (isMSG) {
            // SystFuncs->MessageBoxExclamation(("В метабазе отсутствует описание поля класса \"" +
            //                                  field_name + "\". Следует обратиться к Разработчику").c_str(),
            //                                 MsgVnim);
//            throw std::exception(("В метабазе отсутствует описание поля класса \"" +
//                            field_name + "\". Следует обратиться к Разработчику").c_str());
        }
    }

    return Result;
}

bool TIDsClassFieldCell::getClassFieldIdByCode(int class_id, std::string pokaz_code,
                                                TDateTime CurDate, bool isMSG, int &field_id) {
    int Index;
    std::string Key;
    int parent_class_id;
    std::string ws;

    bool Result = false;

    if (class_id == 0)
        return Result;

    Key = std::to_string(class_id) + "_" + pokaz_code;
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        field_id = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        Result = true;
        return Result;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    ws = std::to_string(class_id) + ")";
    TDB_Pokaztl::GetPokaz(q, class_id);
    parent_class_id = q->FieldByName("pokaz_base_id")->AsInteger();
    while (parent_class_id != 0) {
        ws = std::to_string(parent_class_id) + "," + ws;
        TDB_Pokaztl::GetPokaz(q, parent_class_id);
        parent_class_id = q->FieldByName("pokaz_base_id")->AsInteger();
    }
    ws = "(" + ws;

    q->SQL->Clear();
 
    q->SQL->Add("SELECT b.pokaz_code as pokaz_code, b.pokaz_id as pokaz_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_id = :plan_id ");
    q->SQL->Add("AND a.plan_str_owner_id IN " + ws);
    q->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND :CurDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    q->SQL->Add("ORDER BY plan_str_owner_id , plan_str_order");
    q->ParamByName("CurDate")->AsDateTime() = CurDate;
    q->ParamByName("plan_id")->AsInteger() = 1;
    q->Open();
    while (!q->EoF()) {
        Key = std::to_string(class_id) + "_" + q->FieldByName("pokaz_code")->AsString();
        GlobalVars->slIDsClassFieldCell->AddObject(Key, q->FieldByName("pokaz_id")->AsInteger());
        q->Next();
    }

   //MAK q->Free();
delete q;

    Key = std::to_string(class_id) + "_" + pokaz_code;
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        field_id = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        Result = true;
        return Result;
    } else {
        if (isMSG) {
            // SystFuncs->MessageBoxExclamation(("В метабазе отсутствует описание поля класса \"" +
            //                                  pokaz_code + "\". Следует обратиться к Разработчику").c_str(),
            //                                 MsgVnim);
//            throw std::exception(("В метабазе отсутствует описание поля класса \"" +
//                            pokaz_code + "\". Следует обратиться к Разработчику").c_str());
        }
    }

    return Result;
}

int TIDsClassFieldCell::getPokazCellId(int pokaz_id, int cell_period, int ist_fin_id) {
    int Index;
    std::string Key;
    int Result = 0;

    if (pokaz_id == 0)
        return Result;

    Key = std::to_string(pokaz_id) + "_" + std::to_string(cell_period) + "_" + std::to_string(ist_fin_id);
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        Result = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        return Result;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT cell_id");
    q->SQL->Add("FROM Cell");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND cell_period = :cell_period");
    q->SQL->Add("AND ist_fin_id = :ist_fin_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_period")->AsInteger() = cell_period;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    
    if (!q->EoF()) {
        GlobalVars->slIDsClassFieldCell->AddObject(Key, q->FieldByName("cell_id")->AsInteger());
        Result = q->FieldByName("cell_id")->AsInteger();
    }

   //MAK q->Free();
delete q;
    return Result;
}

int getIstFinId(const std::string& ist_fin_name) {
    std::string key = "IstFinId_" + ist_fin_name;
    int index = 0;

    if (GlobalVars->slIDsClassFieldCell->Find(key, index)) {
        auto result = GlobalVars->slIDsClassFieldCell->GetObject(index);
    }

    int id = TDB_IstFin::UnikName(ist_fin_name);  // Предполагается, что UnikName возвращает уникальный идентификатор для имени
    if (id == 0) {
        return 0;  // Если ID не найден, возвращаем 0
    }

    GlobalVars->slIDsClassFieldCell->AddObject(key, std::make_shared<int>(id));  // Сохраняем ID в словарь
    return id;  // Возвращаем полученный идентификатор
}


int TIDsClassFieldCell::getCNPedizm(int comnp_id) {
    int Index, id;
    std::string Key;
    int Result = 0;
    Key = "CNPedizm_" + std::to_string(comnp_id);
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        Result = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->GetObject(Index));
        return Result;
    }
    id = TDB_ComonNPokaz::GetEdizm_id(comnp_id);
    GlobalVars->slIDsClassFieldCell->AddObject(Key,  id);
    Result = id;
    return Result;
}

int TIDsClassFieldCell::getCNPtype(int comnp_id) {
    int Index, id;
    std::string Key;
    int Result = 0;
    Key = "CNPtype_" + std::to_string(comnp_id);
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        Result = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->GetObject(Index));
        return Result;
    }
    id = TDB_ComonNPokaz::GetComnp_type(comnp_id);
    GlobalVars->slIDsClassFieldCell->AddObject(Key,  id);
    Result = id;
    return Result;
}

std::string TIDsClassFieldCell::getCodeCelSubsByIstFinId(int ist_fin_id) {
    int Index;
    std::string Key, code, Result;
    Key = "CodeCelSubs_" + std::to_string(ist_fin_id);
    if (GlobalVars->slNamesClassFieldCell->TryGetValue(Key, code)) {
        Result = code;
        return Result;
    }
    code = TDB_IstFin::getCodeCelSubsByIstFinId(ist_fin_id);
    GlobalVars->slNamesClassFieldCell->Add(Key, code);
    Result = code;
    return Result;
}

int TIDsClassFieldCell::getEdIzmCoef(int edizm_id) {
    int Index, Result = 0;
    std::string Key;
    if (GlobalVars->slIDsClassFieldCell->Count() == 0)
        TIDsClassFieldCell::saveEdIzmInfo();
    Key = "EdIzmCoef_" + std::to_string(edizm_id);
    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        Result = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->GetObject(Index));
        return Result;
    }
    return Result;
}

std::string TIDsClassFieldCell::getNameEdIzm(int edizm_id) {
    int Index;
    std::string Key, edizm_name, Result;
    if (GlobalVars->slNamesClassFieldCell->Count() == 0)
        TIDsClassFieldCell::saveEdIzmInfo();
    Key = "NameEdIzm_" + std::to_string(edizm_id);
    if (GlobalVars->slNamesClassFieldCell->TryGetValue(Key, edizm_name)) {
        Result = edizm_name;
        return Result;
    }
    Result = "???";
    return Result;
}

void TIDsClassFieldCell::saveEdIzmInfo() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Edizm");
    q->Open();
    
    while (!q->EoF()) {
        int edizm_id = q->FieldByName("edizm_id")->AsInteger();
        std::string edizm_name = q->FieldByName("edizm_name")->AsString();
        int edizm_coef = q->FieldByName("edizm_coef")->AsInteger();
        
        std::string key = "EdIzmCoef_" + std::to_string(edizm_id);
        GlobalVars->slIDsClassFieldCell->AddObject(key, edizm_coef);
        
        key = "NameEdIzm_" + std::to_string(edizm_id);
        GlobalVars->slNamesClassFieldCell->Add(key, edizm_name);
        
        q->Next();
    }

   //MAK q->Free();
delete q;
}

bool TIDsClassFieldCell::findSubsidPokazCellId(std::string codeCelSubs, int& SubsidPokazId, int& SubsidCellIdKV, int& SubsidCellIdGod) {
    std::string Key = codeCelSubs;
    std::string KeyKV = codeCelSubs + "_KV";
    std::string KeyGod = codeCelSubs + "_God";
    int Index;

    if (GlobalVars->slIDsClassFieldCell->Find(Key, Index)) {
        SubsidPokazId = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        GlobalVars->slIDsClassFieldCell->Find(KeyKV, Index);
        SubsidCellIdKV = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        GlobalVars->slIDsClassFieldCell->Find(KeyGod, Index);
        SubsidCellIdGod = *std::get<std::shared_ptr<int>>(GlobalVars->slIDsClassFieldCell->Objects[Index]);
        return true;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    std::string nameSostCast = "Горбюджет: Субсидии";
    int root_Pokaz_id = TDB_Pokaztl::GetIdbyName(0, nameSostCast);
    q->SQL->Add("SELECT pokaz_id FROM Pokaztl");
    q->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
    q->SQL->Add("AND pokaz_code = :pokaz_code");
    q->ParamByName("pokaz_owner_id")->AsInteger() = root_Pokaz_id;
    q->ParamByName("pokaz_code")->AsString() = codeCelSubs;
    q->Open();

    if (!q->EoF()) {
        SubsidPokazId = q->FieldByName("pokaz_id")->AsInteger();
        GlobalVars->slIDsClassFieldCell->AddObject(Key, SubsidPokazId );
        SubsidCellIdKV = TDB_Cell::GetCellId(SubsidPokazId, 2, 0); // Assuming 2 is the constant for "кв"
        GlobalVars->slIDsClassFieldCell->AddObject(KeyKV, SubsidCellIdKV);
        SubsidCellIdGod = TDB_Cell::GetCellId(SubsidPokazId, 1, 0); // Assuming 1 is the constant for "год"
        GlobalVars->slIDsClassFieldCell->AddObject(KeyGod, SubsidCellIdGod);
       //MAK q->Free();
delete q;
        return true;
    }

   //MAK q->Free();
delete q;
    return false;
}

int TIDsClassFieldCell::GetIstFinIdBySubs(int pokaz_idSubsid) {
    std::string key = "IstFin_" + std::to_string(pokaz_idSubsid);
    int index;
    int ist_fin_id = 0; // Начальное значение, если идентификатор не найден

    if (GlobalVars->slIDsClassFieldCell->Find(key, index)) {
        auto result = GlobalVars->slIDsClassFieldCell->GetObject(index);
    }

    ist_fin_id = TDB_IstFin::GetIstFinIdBySubs(pokaz_idSubsid);
    GlobalVars->slIDsClassFieldCell->AddObject(key, std::make_shared<int>(ist_fin_id));
    return ist_fin_id;
}

bool TIDsClassFieldCell::findSummaDgvrSubsidCellId(int class_idZakupki, int SummaDgvrPokazId, int ist_fin_id, std::string codeCelSubs, int SubsidPokazId, int& SummaDgvrCellIdKV, int& SummaDgvrCellIdGod) {
    std::string KeyKV = "summaDgvr_" + codeCelSubs + "_KV";
    std::string KeyGod = "summaDgvr_" + codeCelSubs + "_God";
    int index;
    SummaDgvrCellIdKV = 0;
    SummaDgvrCellIdGod = 0;

    // Проверка наличия ключа KV
    if (GlobalVars->slIDsClassFieldCell->Find(KeyKV, index)) {
        auto result = GlobalVars->slIDsClassFieldCell->GetObject(index);
    }

    // Проверка наличия ключа God
    if (GlobalVars->slIDsClassFieldCell->Find(KeyGod, index)) {
        auto result = GlobalVars->slIDsClassFieldCell->GetObject(index);
    }

    if (SummaDgvrCellIdKV > 0) {
        return true;
    }

    // Получаем идентификаторы для новых ключей
    SummaDgvrCellIdGod = TDB_Cell::GetCellId(SummaDgvrPokazId, 1, ist_fin_id);
    GlobalVars->slIDsClassFieldCell->AddObject(KeyGod, std::make_shared<int>(SummaDgvrCellIdGod));

    SummaDgvrCellIdKV = TDB_Cell::GetCellId(SummaDgvrPokazId, 2, ist_fin_id);
    GlobalVars->slIDsClassFieldCell->AddObject(KeyKV, std::make_shared<int>(SummaDgvrCellIdKV));

    if (SummaDgvrCellIdKV == 0) {
        //throw std::runtime_error("Для характеристики Сумма договора не создан cell_id для кода целевых средств " + codeCelSubs + ". Обработка прекращается");
    }

    return true;
}

int getPlanIdBySinonim(const std::string& sinonim) {
    std::string key = "planName";
    int index = 0;

    auto loadSinonims = [&key]() {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        q->SQL->Add("SELECT sinonims, plan_id FROM Plans");
        q->Open();
        while (!q->EoF()) {
            int plan_id = q->FieldByName("plan_id")->AsInteger();
            std::string sinonims = q->FieldByName("sinonims")->AsString();
            if (!sinonims.empty()) {
                std::istringstream wsl(sinonims);
                std::string token;
                while (getline(wsl, token, ';')) {
                    GlobalVars->slIDsClassFieldCell->AddObject(key + '_' + token, std::make_shared<int>(plan_id));
                }
            }
            q->Next();
        }
       //MAK q->Free();
delete q;
    };

    if (!GlobalVars->slIDsClassFieldCell->Find(key, index)) {
        loadSinonims();
    }

    if (GlobalVars->slIDsClassFieldCell->Find(key + '_' + sinonim, index)) {
        auto result = GlobalVars->slIDsClassFieldCell->GetObject(index);

    }
    return 0;
}























