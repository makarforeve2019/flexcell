
#ifndef MWSRESTAPI_RS_DOCUMENT_H
#define MWSRESTAPI_RS_DOCUMENT_H

#include <string>
#include <../nlohmann/json.hpp>
#include "../DB/unDefDBconnect.h"
#include <boost/beast/http.hpp>
namespace http = boost::beast::http;

class Document {
public:
    // Чтение ID документа и файлов для показателя
    static std::string DocumentPokaz(
            int PokazId, int comnp_id, int cell_period, int God, int Kvrtl, int Mesjc, int Den, int PlanId, int VidOtch, boost::beast::http::status& status
    );
    static bool GetDocumidPokaz(int PokazId, int comnp_id, int cell_period,int God,int Kvrtl,int Mesjc,int Den,int PlanId,int VidOtch, std::string& JSONresult);
    // Чтение документа по Id
    static std::string DocumentById( std::string documentIdHex, boost::beast::http::status& status );
    // Чтение контента документа. Контент в формате Zip и Hex. Т.е. контент нужно перевести из Hex, затем UnZip
    static std::string DocumentContent( long documentId, int fileId, boost::beast::http::status& status );
    // Создание документа. Контент в формате Zip и Hex
    static std::string DocumentCreate( bool InSpreadsheet, std::string JsonBody, boost::beast::http::status& status );
    // Изменение документа. Контент в формате Zip и Hex
    static std::string DocumentUpdate( bool InSpreadsheet, std::string JsonBody, boost::beast::http::status& status );
};
#endif //MWSRESTAPI_RS_DOCUMENT_H