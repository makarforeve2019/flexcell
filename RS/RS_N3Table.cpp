
#include "RS_N3Table.h"
#include "DB_Album.h"

std::string N3Table::n3tableload(std::string TblName, int Year) {
    try {
        nlohmann::json AllPlansJsonArr;
        bool result = N3Table::GetAllPlansJson(AllPlansJsonArr);
        if (!result) return "failed";
        int formIDfromTableName = N3Table::getFormIdFromn3tablename(TblName);
        nlohmann::json AllColumnDataJson = N3Table::GetAlln3ColumnData(formIDfromTableName);
        std::string ws = AllColumnDataJson.dump(); // Преобразуем JSON в строку
        nlohmann::json AllFormIdJsonArr = N3Table::getAllFormIdFromn3tablename(formIDfromTableName);


        int album_vid_func;
        int album_isbp;
        int readOnly;
        std::string album_obozn;
        int plan_id_fiks;
        int vid_deistv;
        int album_typ;
        int isYearQuartMonth;
        int YearFixed;
        std::string JSONFiltr;

        for (const auto& formId : AllFormIdJsonArr) {
            int FormID = formId.get<int>(); // Преобразуем JSON элемент в int
            int albumID =  getAlbumIDFromFormID(FormID);
            int god_ravno;
            TDB_Album::getGod_ravno(albumID, god_ravno);
            if (god_ravno != Year) continue;

            TDB_Album::GetAlbumInfoYQM(albumID, album_vid_func, album_isbp, readOnly,
                    album_obozn, plan_id_fiks, vid_deistv, album_typ,
                    isYearQuartMonth, YearFixed, JSONFiltr);
    /////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////ЗАПОЛНЯЕМ InfoAlbumLogic///////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
            TInfoAlbumLogic InfoAlbumLogic;
            InfoAlbumLogic.CurrAlbumName = TDB_Album::getAlbumName(albumID);
            InfoAlbumLogic.accessalbum_idSelBlockSubj = 0;
            InfoAlbumLogic.CanEdit = false;
            InfoAlbumLogic.CurrVidOtch = album_vid_func;
            InfoAlbumLogic.CurrYear = Year;
            InfoAlbumLogic.CurrQuarter = 0;
            InfoAlbumLogic.CurrMonth = 0;
            InfoAlbumLogic.aShablonKBK = "";
            InfoAlbumLogic.aId_Owner_link1n = 0;
    ////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////ЗАПОЛНЯЕМ InfoFormLogic///////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
            TInfoFormLogic InfoFormLogic;
            InfoFormLogic.isISTZNACHVVODPK = false;
            InfoFormLogic.modeNumber = 0;
            double selectedDate = 0;
            InfoFormLogic.selectedDate = UnixTimestampToDateTime(selectedDate);

            for (const auto& plan_id : AllPlansJsonArr) {
                int PlanID = plan_id.get<int>();
                InfoAlbumLogic.CurrDate = Now();
                InfoAlbumLogic.CurrPlanId = PlanID;
                InfoAlbumLogic.CurrPlanName = ""; //НУЖНО ЛИ ИМЯ???

                bool isCurForm = true;

                TFormLogicF1 *formLogic = nullptr;
                formLogic = new TFormLogicF1();
//                TQueryUni *qTmp =
                formLogic->quForms = new TQueryUni(DefDBconnect.Conn);
                formLogic->OpenForm(isCurForm, albumID, InfoAlbumLogic, InfoFormLogic, 0, 0, "", FormID, 1, 0);
                N3Table::CreateContent(formLogic, FormID, PlanID, Year, TblName, AllColumnDataJson);
                delete formLogic;
                formLogic = nullptr;
                int wi=1;
            }
            TQueryUni *qLog = new TQueryUni(DefDBconnect.MWSModulsConn);
            qLog->SQL->Clear();
            qLog->SQL->Add("INSERT INTO datamartlog(");
            qLog->SQL->Add("tbl_name, reporting_year, createddate)");
            qLog->SQL->Add("VALUES (:tbl_name, :reporting_year, :createddate)");
            qLog->ParamByName("tbl_name")->AsString() = TblName;
            qLog->ParamByName("reporting_year")->AsInteger() = Year;
            qLog->ParamByName("createddate")->AsDateTime() = Now();
            qLog->ExecSQL();
            delete qLog;
        }
        return "success";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "failed";
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return "failed";
    }
}

int N3Table::getFormIdFromn3tablename(std::string TblName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = -1;
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Forms WHERE n3tablename = :n3tablename");
    q->ParamByName("n3tablename")->AsString() = TblName;
    q->Open();

    if (!q->EoF()) {
        Result = q->FieldByName("form_id")->AsInteger();
    }
    delete q;
    return Result;
}

nlohmann::json N3Table::getAllFormIdFromn3tablename(int FormID) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json AllFormId = nlohmann::json::array();
    AllFormId.push_back(FormID);
    q->SQL->Clear();
    q->SQL->Add("SELECT form_id");
    q->SQL->Add("FROM Forms");
    q->SQL->Add("WHERE formid_exporttbl = :formid_exporttbl");
    q->ParamByName("formid_exporttbl")->AsInteger() = FormID;
    q->Open();
    while (!q->EoF()) {
        AllFormId.push_back(q->FieldByName("form_id")->AsInteger());
        q->Next();
    }
    delete q;
    return AllFormId;
}


bool N3Table::GetAllPlansJson(nlohmann::json &AllPlansJsonArr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id");
    q->SQL->Add("FROM plans");
    q->SQL->Add("WHERE plan_is_rajon = 9");
    q->Open();
    while (!q->EoF()) {
        AllPlansJsonArr.push_back(q->FieldByName("plan_id")->AsInteger());
        q->Next();
        result = true;
    }
    delete q;
    return result;
}

int N3Table::getAlbumIDFromFormID(int FormID) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int albumID = -1;
    q->SQL->Clear();
    q->SQL->Add("SELECT album_id");
    q->SQL->Add("FROM album_form");
    q->SQL->Add("WHERE form_id = :form_id");
    q->ParamByName("form_id")->AsInteger() = FormID;
    q->Open();
    while (!q->EoF()) {
        albumID = q->FieldByName("album_id")->AsInteger();
        q->Next();
    }
    delete q;
    return albumID;
}

nlohmann::json N3Table::GetAlln3ColumnData(int FormID) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json AllColumnDataJson = nlohmann::json::object(); // Создаем JSON объект
    q->SQL->Clear();
    q->SQL->Add("SELECT attr_1, n3columnname, n3columntype, grafa_isexport");
    q->SQL->Add("FROM form_str");
    q->SQL->Add("WHERE form_id = :form_id");
    q->SQL->Add("AND n3columnname != ''");
    q->ParamByName("form_id")->AsInteger() = FormID;
    q->Open();
    while (!q->EoF()) {
        int attr_1 = q->FieldByName("attr_1")->AsInteger();
        std::string n3columnname = q->FieldByName("n3columnname")->AsString();
        std::string n3columntype = q->FieldByName("n3columntype")->AsString();
        int grafa_isexport = q->FieldByName("grafa_isexport")->AsInteger();

        nlohmann::json columnData = {
                {"n3columnname", n3columnname},
                {"n3columntype", n3columntype},
                {"grafa_isexport", grafa_isexport}
        };

        // Проверяем, существует ли уже ключ attr_1 в AllColumnDataJson
        if (AllColumnDataJson.contains(std::to_string(attr_1))) {
            AllColumnDataJson[std::to_string(attr_1)].push_back(columnData);
        } else {
            AllColumnDataJson[std::to_string(attr_1)] = nlohmann::json::array();
            AllColumnDataJson[std::to_string(attr_1)].push_back(columnData);
        }

        q->Next();
    }
    delete q;
    return AllColumnDataJson;
}


void N3Table::CreateContent(TFormLogicF1* formLogic, int formid, int planid, int year, std::string tablename, nlohmann::json AllColumnDataJson) {

    // Удаляем записи, соответствующие заданным year и planid
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
    q->SQL->Clear();
    std::string deleteQuery = "DELETE FROM " + tablename + " WHERE reporting_year = " + std::to_string(year) + " AND region_mnemonic_code = '" + std::to_string(planid) + "'";
    q->SQL->Add(deleteQuery);
    if (!q->ExecSQL()) {
        //std::cerr << "Ошибка выполнения запроса на удаление записей." << std::endl;
    } else {
        //std::cout << "Записи за указанный год и planid успешно удалены." << std::endl;
    }

    delete q;


//    // Опусташаем таблицу каскадно
//    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
//    q->SQL->Clear();
//    std::string clearQuery = "TRUNCATE TABLE " + tablename + " RESTART IDENTITY";
//    q->SQL->Add(clearQuery);
//    if (!q->ExecSQL()) {
//        std::cerr << "Ошибка выполнения запроса на очистку таблицы." << std::endl;
//    } else {
//        std::cout << "Таблица успешно очищена и последовательность сброшена." << std::endl;
//    }
//    delete q;

    TwwStringList* p = formLogic->FormsValuesArray->GetItem(0);
    for (int row = p->form_begBodyRow; row <= p->form_endBodyRow; ++row) {
        PAtrRow AtrRow = ((*p->ListRows)[row]);
        if (AtrRow->isRowHidden > 0) continue;
        int newRecordID = N3Table::InsertEmptyRowInTable(tablename);
        std::string updateQuery = "UPDATE " + tablename + " SET ";

        bool firstColumn = true;
        std::string wsAllColumnDataJson = to_string(AllColumnDataJson);
        // Проходим по всем колонкам в JSON
        for (auto it = AllColumnDataJson.begin(); it != AllColumnDataJson.end(); ++it) {
            std::string colStr = it.key();
            int col = std::stoi(colStr);
            nlohmann::json columnArray = it.value();

            for (const auto& columnData : columnArray) {

                std::string n3columnname = columnData.at("n3columnname").get<std::string>();
                if (n3columnname == "reporting_year" || n3columnname == "region_mnemonic_code") continue;
                std::string n3columntype = columnData.at("n3columntype").get<std::string>();

                if (!firstColumn) {
                    updateQuery += ", ";
                }


                if (n3columntype == "numeric" || n3columntype == "NUMERIC") {
                    double value = formLogic->f1Forms->NumberRC_Get(row, col);
                    std::ostringstream oss;
                    oss.imbue(std::locale::classic());  // Устанавливаем классическую локаль, чтобы использовать точку как десятичный разделитель
                    oss << std::fixed << std::setprecision(6) << value;  // Форматируем число с 6 знаками после запятой
                    updateQuery += n3columnname + " = " + oss.str();
                    firstColumn = false;
                } else if (n3columntype == "varchar"|| n3columntype == "VARCHAR") {
                    std::string value = char16ToUTF8(formLogic->f1Forms->TextRC_Get(row, col));
                    updateQuery += n3columnname + " = '" + value + "'";
                    firstColumn = false;
                }
            }
        }
        updateQuery += ", region_mnemonic_code = '" + IntToStr(planid) + "'";
        updateQuery += ", reporting_year = " + IntToStr(year);
        updateQuery += " WHERE "+tablename+"_id = " + std::to_string(newRecordID);

        // Выполняем обновление строки
        TQueryUni *updateQueryObj = new TQueryUni(DefDBconnect.MWSModulsConn);
        updateQueryObj->SQL->Clear();
        updateQueryObj->SQL->Add(updateQuery);
        if (!updateQueryObj->ExecSQL()) {
            std::cerr << "Ошибка выполнения запроса на обновление строки." << std::endl;
        }
        delete updateQueryObj;
    }
}

int N3Table::InsertEmptyRowInTable(std::string tablename) {
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
    q->SQL->Clear();
    // Создаем запрос для вставки пустой записи
    std::string insertQuery = "INSERT INTO " + tablename + " DEFAULT VALUES RETURNING " + tablename + "_id";
    q->SQL->Add(insertQuery);
    PGresult* res = q->Open(); // Выполняем запрос
    if (res == nullptr) {
        std::cerr << "Ошибка выполнения запроса на вставку." << std::endl;
        delete q;
        return -1;
    }

    // Получаем id вставленной записи
    int newRecordID = 0;
    if (!q->EoF()) {
        newRecordID = q->FieldByName(tablename + "_id")->AsInteger();
    } else {
        std::cerr << "Не удалось получить ID вставленной записи." << std::endl;
    }

//    PQclear(res); // Освобождаем PGresult
    delete q; // Освобождаем объект запроса
    return newRecordID;
}


void N3Table::UpdateLastRowInTable(std::string updateQuery) {

}


