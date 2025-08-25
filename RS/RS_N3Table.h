#ifndef MWSRESTAPI_RS_N3TABLE_H
#define MWSRESTAPI_RS_N3TABLE_H


#include <string>
#include "json.hpp"
#include "unFormLogicF1.h"

class N3Table {
public:
    static std::string n3tableload(std::string TblName, int Year);
    static int getFormIdFromn3tablename(std::string TblName);
    static nlohmann::json getAllFormIdFromn3tablename(int FormID);
    static bool GetAllPlansJson(nlohmann::json &AllPlansJsonArr);
    static int getAlbumIDFromFormID(int FormID);
    static nlohmann::json GetAlln3ColumnData(int FormID);
    static void CreateContent(TFormLogicF1* formLogic, int formid, int planid, int year, std::string tablename, nlohmann::json AllColumnDataJson);
    static int InsertEmptyRowInTable(std::string tablename);
    static void UpdateLastRowInTable(std::string updateQuery);

};


#endif //MWSRESTAPI_RS_N3TABLE_H
