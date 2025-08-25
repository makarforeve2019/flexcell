
#include "RS_Document.h"
#include "unAPIDocum.h"
#include "json.hpp"
#include "unDocumentValueLogic.h"
#include "TracerManager.h"

#include <boost/beast/core.hpp>
#include <boost/beast/core/detail/base64.hpp>
namespace base64 = boost::beast::detail::base64;
using namespace base64;

using Json = nlohmann::json;

std::string base64_decode(const std::string& input) {
    std::string base64 = input;
    std::replace(base64.begin(), base64.end(), '-', '+');
    std::replace(base64.begin(), base64.end(), '_', '/');
    while (base64.size() % 4) {
        base64 += '=';
    }

    std::vector<unsigned char> decoded(boost::beast::detail::base64::decoded_size(base64.size()));
    auto result = boost::beast::detail::base64::decode(decoded.data(), base64.data(), base64.size());
    decoded.resize(result.first);

    return std::string(decoded.begin(), decoded.end());
}

std::string Document::DocumentPokaz(
        int PokazId, int comnp_id, int cell_period, int God, int Kvrtl, int Mesjc, int Den, int PlanId, int VidOtch, boost::beast::http::status& status
) {
    status = http::status::ok;
    std::string JSONresult;

    bool documResult = Document::GetDocumidPokaz(PokazId, comnp_id, cell_period, God,
                                                      Kvrtl, Mesjc, Den, PlanId, VidOtch, JSONresult);
    if (!documResult) {
        TracerManager::Instance().AddSpanLog("Document::DocumentPokaz. Document::GetDocumidPokaz returned false", {},
                                             TracerManager::LOG_LEVEL_ERROR);
        status = http::status::bad_request;
        JSONresult = "";
    }
    return JSONresult;
}

bool Document::GetDocumidPokaz(int PokazId, int comnp_id, int cell_period,int God,int Kvrtl,int Mesjc,int Den,int PlanId,int VidOtch, std::string& JSONresult) {
    std::string Docum_id;
    // Docum_id в Hex
    if (!TDocumentValueLogic::GetDocumid(Docum_id, PokazId, comnp_id, cell_period, God, Kvrtl, Mesjc, Den, PlanId,
                                         VidOtch)) {
        TracerManager::Instance().AddSpanLog("Document::GetDocumidPokaz. TDocumentValueLogic::GetDocumid returned false", {},
                                             TracerManager::LOG_LEVEL_ERROR);
        return false;
    }
    else {
        try {
            json JSONdocum;
            json JSONfiles = json::array();

            JSONdocum["documentId"] = HexToLong(Docum_id);

            std:string DocumNote, fileName;

            //Читаем информацию о нужном документе, чтобы потом найти нужный порядковый номер
            TStringList slFileName;
            TStringList slFileNote;
            TAPIDocum::ReadDocum(Docum_id, slFileName, slFileNote, DocumNote);

            for (int i = 0; i < slFileName.Count(); i++) {
                std::string fileName = slFileName[i];
                int ordern = *std::get<std::shared_ptr<int>>(slFileName.GetObject(i));

                json userfile;
                userfile["id"] = ordern;
                userfile["name"] = fileName;
                JSONfiles.push_back(userfile);
            }

            JSONdocum["files"] = JSONfiles;
            JSONresult = JSONdocum.dump();
        }
        catch (const std::exception& e) {
            TracerManager::Instance().AddSpanLog("Exception.Document::GetDocumidPokaz", {{"errordesr", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            return false;
        }
    }
    return true;
}

std::string Document::DocumentContent( long documentId, int fileId, boost::beast::http::status& status  ) {
    status = http::status::ok;
    std::string fileContent;
    std::string fileName;
    json JSONdocum;

    if (TAPIDocum::DocumentContent( documentId, fileId, fileName, fileContent )) {
        try {
            JSONdocum["documentId"] = documentId;
            JSONdocum["fileId"] = fileId;
            JSONdocum["fileName"] = fileName;

            std::size_t len_encoded = encoded_size(fileContent.size());

            std::string encodedFileContent;
            encodedFileContent.resize(len_encoded);
            encode(&encodedFileContent[0], fileContent.c_str(), fileContent.size());
            JSONdocum["content"] = encodedFileContent;
        }
        catch (const std::exception& e) {
            TracerManager::Instance().AddSpanLog("Exception.Document::DocumentContent", {{"errordesr", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            status = http::status::bad_request;
            return "";
        }
    }
    return JSONdocum.dump();
}

std::string Document::DocumentCreate( bool InSpreadsheet, std::string JsonBody, boost::beast::http::status& status  ) {
    status = http::status::ok;
    json JSONresult;
    nlohmann::json json;
    nlohmann::json resultArray;
    try {
        json = nlohmann::json::parse(JsonBody);
    } catch (const nlohmann::json::parse_error& e) {
        TracerManager::Instance().AddSpanLog("Ошибка разбора. Document::DocumentCreate", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка разбора: " << e.what() << std::endl;
        std::cerr << "Строка, вызвавшая ошибку: " << JsonBody << std::endl;
        JSONresult["error"] = "parsing error";
        status = http::status::bad_request;
        return to_string(JSONresult);
    }

    try {
        string sessionId;
        int nSheet, nRow, nCol;
        int ObjectId, CellId, PlanId, VidOtch;
        int PokazId, comnp_id, cell_period, God, Kvrtl, Mesjc, Den;
        TDateTime ValueDate;
        bool documentCreated;
        long documentId;

        if (InSpreadsheet) {
            sessionId = json["TblSessionId"];
            nSheet = json["nSheet"];
            nRow = json["nRow"];
            nCol = json["nCol"];

            ObjectId = json["ObjectId"].get<int>();
            if (ObjectId) {
                CellId = json["CellId"].get<int>();
                string ValueDatews = json["ValueDate"].get<std::string>();
                double ValueDateStr = StrToFloat(ValueDatews);
                ValueDate = UnixTimestampToDateTime(ValueDateStr);
            }
            PlanId = json["PlanId"].get<int>();
            VidOtch = json["VidOtch"].get<int>();

            PokazId = json["PokazId"].get<int>();
            if (PokazId) {
                comnp_id = json["comnp_id"].get<int>();
                cell_period = json["cell_period"].get<int>();
                God = json["God"].get<int>();
                Kvrtl = json["Kvrtl"].get<int>();
                Mesjc = json["Mesjc"].get<int>();
                Den = json["Den"].get<int>();
            }

            if ((!PokazId) && (!ObjectId)) {
                TracerManager::Instance().AddSpanLog("Ошибка Document::DocumentCreate", {{"errordesr", "Необходимо указать ObjectId или PokazId"}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                JSONresult["error"] = "Необходимо указать ObjectId или PokazId";
                status = http::status::bad_request;
                return to_string(JSONresult);
            }
        }

        documentId = TAPIDocum::AddDocumOnly("");
        if (documentId == 0) {
            TracerManager::Instance().AddSpanLog("Ошибка Document::DocumentCreate", {{"errordesr", "documentId is not defined or zero"}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            JSONresult["error"] = "documentId is not defined or zero";
            status = http::status::bad_request;
            return to_string(JSONresult);
        }

        //Чтение первого файла
        std::string fileName = "";
        nlohmann::json filesArrayForName  = json["files"];
        for (nlohmann::json &formItem: filesArrayForName) {
            fileName = formItem["name"].get<std::string>();
        }
        if (fileName == "") {
            status = http::status::bad_request;
            return to_string(JSONresult);
        }

        if (InSpreadsheet) {
            //Привязка документа к основной базе
            if (PokazId) {
                documentCreated = TAPIDocum::newsaveDocument(documentId, PlanId, VidOtch, PokazId, comnp_id,
                                                             cell_period, God, Kvrtl, Mesjc, Den, fileName);
            } else {
                documentCreated = TAPIDocum::newsaveDocument(documentId, ObjectId, CellId, PlanId, VidOtch, ValueDate,
                                                             fileName);
            }
            if (!documentCreated) {
                TracerManager::Instance().AddSpanLog("Ошибка Document::DocumentCreate", {{"errordesr", "new document is not created"}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                JSONresult["error"] = "new document is not created";
                status = http::status::bad_request;
                return to_string(JSONresult);
            }
        }

        nlohmann::json filesArray = json["files"];

        std::string fileIdStr, fileContent;
        int fileId;
        int nextfileId = 1;
        for (nlohmann::json &formItem: filesArray) {
            try {
                if ( formItem["id"].is_number() ) {
                    fileId = formItem["id"].get<int>();
                }
                else {
                    fileIdStr = formItem["id"].get<std::string>();
                    if (fileIdStr == "")
                        fileId = 0;
                    else
                        fileId = std::stoi(fileIdStr);
                    if (fileId < nextfileId) {
                        fileId = nextfileId;
                    }
                }
                nextfileId++;
            }
            catch(std::exception const & ex) {
                fileId = nextfileId++;
            }
            fileName = formItem["name"].get<std::string>();
            fileContent = formItem["content"].get<std::string>();

            std::string decodedFileContent;
            decodedFileContent = base64_decode(fileContent);

            TAPIDocum::addFile( documentId, fileId, "", fileName, decodedFileContent ); //{'dAdd':'','dUpd':'','edoId':''}
        }

        if (InSpreadsheet) {
            JSONresult["nSheet"] = nSheet;
            JSONresult["Row"] = json::array({nRow});
            JSONresult["Col"] = json::array({nCol});
            JSONresult["Values"] = json::array({"Документ"});
            //JSONresult["Styles"] = json::array({""});
        }
        else {
            JSONresult["documentId"] = documentId;
        }
    }
    catch(std::exception const & ex) {
        TracerManager::Instance().AddSpanLog("Exception.Ошибка Document::DocumentCreate", {{"errordesr", ex.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        // Обработка ошибки парсинга
        std::cerr << "Ошибка разбора: " << ex.what() << std::endl;
        status = http::status::bad_request;
        JSONresult["error"] = ex.what();
    }

    return to_string(JSONresult);
}

std::string Document::DocumentUpdate( bool InSpreadsheet, std::string JsonBody, boost::beast::http::status& status ) {
    status = http::status::ok;
    bool result = true;
    bool deleteDoc = false;
    json JSONresult;
    nlohmann::json json;
    nlohmann::json resultArray;
    try {
        json = nlohmann::json::parse(JsonBody);
    } catch (const nlohmann::json::parse_error& e) {
        TracerManager::Instance().AddSpanLog("Ошибка разбора.Document::DocumentUpdate", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка разбора: " << e.what() << std::endl;
        std::cerr << "Строка, вызвавшая ошибку: " << JsonBody << std::endl;
        JSONresult["error"] = "parsing error";
        status = http::status::bad_request;
        return to_string(JSONresult);
    }

    try {
        string sessionId;
        int nSheet, nRow, nCol;
        long documentId;

        if (InSpreadsheet) {
            sessionId = json["TblSessionId"];
            nSheet = json["nSheet"];
            nRow = json["nRow"];
            nCol = json["nCol"];
        }

        documentId = json["documentId"].get<long>();
        if (documentId == 0) {
            TracerManager::Instance().AddSpanLog("Ошибка.Document::DocumentUpdate", {{"errordesr", "documentId is not defined or zero"}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            JSONresult["error"] = "documentId is not defined or zero";
            status = http::status::bad_request;
            return to_string(JSONresult);
        }

        nlohmann::json filesArray = json["files"];

        std::string fileIdStr, fileName, fileContent, fileAction;
        int fileId;

        int maxFileId = 0;
        TAPIDocum::getMaxFileId(documentId, maxFileId);
        maxFileId++;
        for (nlohmann::json &formItem: filesArray) {
            try {
                if ( formItem["id"].is_number() )
                    fileId = formItem["id"].get<int>();
                else {
                    fileIdStr = formItem["id"].get<std::string>();
                    if (fileIdStr == "")
                        fileId = 0;
                    else
                        fileId = std::stoi(fileIdStr);
                }
            }
            catch(std::exception const & ex) {
                fileId = 0;
            }
            if ( !formItem["action"].is_null() ) {
                fileAction = formItem["action"].get<std::string>();
                if ((fileId == 0) && (fileAction == "save"))
                    fileAction = "create";
                if (fileAction != "delete") {
                    fileName = formItem["name"].get<std::string>();
                    fileContent = formItem["content"].get<std::string>();
                }
            }
            else
                continue;

            std::string decodedFileContent;
            decodedFileContent = base64_decode(fileContent);

            if (fileAction == "create")
                result = TAPIDocum::addFile( documentId, maxFileId++, "", fileName, decodedFileContent ); //{'dAdd':'','dUpd':'','edoId':''}
            else if (fileAction == "save")
                result = TAPIDocum::updateFile( documentId, fileId, "", fileName, decodedFileContent ); //{'dAdd':'','dUpd':'','edoId':''}
            else if (fileAction == "delete") {
                result = TAPIDocum::deleteFile(documentId, fileId);
                int countFiles;
                if (result&&TAPIDocum::getCountFiles(documentId, countFiles)) {
                    if (countFiles == 0) {
                        //Удаление документа
                        result = TAPIDocum::deleteDocument(documentId);
                        if (result) {
                            result = TAPIDocum::deleteDocumentRelation(documentId);
                            deleteDoc = true;
                        }
                    }
                }
            }
            else {
                result = false;
            }
            if (!result) break;
        }

        if (InSpreadsheet) {
            JSONresult["nSheet"] = nSheet;
            JSONresult["Row"] = json::array({nRow});
            JSONresult["Col"] = json::array({nCol});
            if (deleteDoc)
                JSONresult["Values"] = json::array({""});
            else
                JSONresult["Values"] = json::array({"Документ"});
        }
    }
    catch(std::exception const & ex) {
        // Обработка ошибки парсинга
        TracerManager::Instance().AddSpanLog("Exception Ошибка разбора.Document::DocumentUpdate", {{"errordesr", ex.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка разбора: " << ex.what() << std::endl;
        status = http::status::bad_request;
        JSONresult["error"] = ex.what();
    }

    return to_string(JSONresult);
}

std::string Document::DocumentById( std::string documentIdHex, boost::beast::http::status& status ) {
    bool result = true;
    status = http::status::ok;
    json JSONdocum;
    std::string Docum_id = documentIdHex;

    try {
            //Docum_id = LongToHex(documentId);
            json JSONfiles = json::array();
            JSONdocum["documentId"] = HexToLong(Docum_id);

            std:string DocumNote, fileName;

            //Читаем информацию о нужном документе, чтобы потом найти нужный порядковый номер
            TStringList slFileName;
            TStringList slFileNote;
            if (!TAPIDocum::ReadDocum(Docum_id, slFileName, slFileNote, DocumNote))
                result = false;
            if (result) {
                for (int i = 0; i < slFileName.Count(); i++) {
                    std::string fileName = slFileName[i];
                    int ordern = *std::get<std::shared_ptr<int>>(slFileName.GetObject(i));

                    json userfile;
                    userfile["id"] = ordern;
                    userfile["name"] = fileName;
                    JSONfiles.push_back(userfile);
                }

                JSONdocum["files"] = JSONfiles;
            }
    }
    catch (const std::exception& e) {
       TracerManager::Instance().AddSpanLog("Exception.Document::DocumentById", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
       result = false;
    }

    if (!result)
       status = http::status::bad_request;

    return JSONdocum.dump();
}

