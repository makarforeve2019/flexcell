#ifndef MWSRESTAPI_RS_SPREADSHEET_H
#define MWSRESTAPI_RS_SPREADSHEET_H

#include <string>

#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/detail/base64.hpp>

namespace http = boost::beast::http;

class Spreadsheet {
public:
    static std::string OpenForm(int port, std::string JsonStringws, http::response<http::string_body>& res, std::string xRequestId);
    static std::string GetJsonForm(std::string TblSessionId);
    static std::string CellValueAtr(std::string TblSessionId, int nSheet, int nRow, int nCol);
//    static std::string AllCellValueAtr(std::string TblSessionId, int nSheet);
    static std::string CellUpdate(std::string JsonStringws, boost::beast::http::status& status);
    static std::string CellUpdateBatch(std::string JsonStringws);
    static std::string CloseForm(std::string JsonStringws);
    static std::string Close(std::string JsonStringws);
    static std::string GetTww(std::string context_id, std::string TblSessionId, int nSheet);
    static std::string GetAttRow(std::string context_id, std::string TblSessionId, int nSheet, int nRow);
    static std::string GetNumberActiveSessions();
    static std::string ButtonClick(std::string JsonStringws);
    static std::string insertPokaztl(int port, std::string JsonString);
    static std::string deletePokaztl(int port, std::string JsonString);
    static std::string changePokaztl(int port, std::string JsonString);
    static std::string getPokaztl(std::string context_id, std::string TblSessionId, int nSheet, int nRow);
    static std::string BudgetChange(std::string JsonString);
    static std::string ExportInPDF(std::string TblSessionId, int nSheet);
    static std::string ExportInXLS(std::string TblSessionId, int nSheet);
};

class SpreadsheetAttr {
public:
    static std::string AllCellValueAtr(std::string JsonStringws);
//    static std::string OpenForm(int port, std::string JsonStringws, http::response<http::string_body>& res);
//    static std::string CellValueAtr(std::string TblSessionId, int nSheet, int nRow, int nCol);
//    static std::string AllCellValueAtr(std::string TblSessionId, int nSheet);
//    static std::string CellUpdate(std::string JsonStringws);
//    static std::string CellUpdateBatch(std::string JsonStringws);
//    static std::string CloseForm(std::string JsonStringws);
//    static std::string Close(std::string JsonStringws);
//    static std::string GetTww(std::string context_id, std::string TblSessionId, int nSheet);
//    static std::string GetAttRow(std::string context_id, std::string TblSessionId, int nSheet, int nRow);
//    static std::string GetNumberActiveSessions();
//    static std::string ButtonClick(std::string JsonStringws);
//    static std::string insertPokaztl(int port, std::string JsonString);
//    static std::string deletePokaztl(int port, std::string JsonString);
//    static std::string changePokaztl(int port, std::string JsonString);
//    static std::string getPokaztl(std::string context_id, std::string TblSessionId, int nSheet, int nRow);
//    static std::string BudgetChange(std::string JsonString);
//    static std::string ExportInPDF(std::string TblSessionId, int nSheet);
//    static std::string ExportInXLS(std::string TblSessionId, int nSheet);
};


#endif //MWSRESTAPI_RS_SPREADSHEET_H
