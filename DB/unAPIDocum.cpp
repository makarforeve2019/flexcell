#include "unAPIDocum.h"
#include "unDefDBconnect.h"
#include "DB_Common.h"
#include "ClassesUtils.h"
#include "unDocumentValueLogic.h"
#include <zip.h>  //Библиотека ziplib
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

char* TAPIDocum::getTmpFileName() {
//    return "test.zip";
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string struuid = "tmpdoc" + boost::uuids::to_string(uuid) + ".zip";
    return struuid.data();
}

bool TAPIDocum::ReadDocum(std::string Docum_id, TStringList& slFileName, TStringList& slFileNote, std::string& DocumNote) {
    bool result = false;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("SELECT D.note as D_note, DF.note as DF_note, DF.filename, DF.ordern");
        qIn->SQL->Add("FROM docums  D, docum_files  DF");
        qIn->SQL->Add("WHERE D.docum_id = :docum_id");
        qIn->SQL->Add("AND DF.docum_id = D.docum_id");
        long iDocum_id = std::stoull(Docum_id, nullptr, 16);
        qIn->ParamByName("docum_id")->AsInteger() = iDocum_id;  // Нужен As лонг!
        qIn->Open();

        result = !(qIn->EoF());
        while (!qIn->EoF()) {
            DocumNote = qIn->FieldByName("D_note").AsString();
            std::string FileNote = qIn->FieldByName("DF_note").AsString();
            int OrderN = qIn->FieldByName("ordern").AsInteger();
            slFileNote.AddObject(FileNote, OrderN);
            std::string Filename = qIn->FieldByName("filename").AsString();
            slFileName.AddObject(Filename, OrderN);
            qIn->Next();
        }
        delete qIn;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

/*
bool TAPIDocum::DocumentContent( long documentId, int fileId, std::string& fileName, std::string& fileContent ) {
    bool result = false;
    int formatType = 1;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        if (formatType == 2)   // контент в base64
            qIn->SQL->Add("SELECT encode(vlogenie, 'base64')  as vlogenie, filename FROM docum_files");
        else if (formatType == 3)  // контент в hex
            qIn->SQL->Add("SELECT encode(vlogenie, 'hex')  as vlogenie, filename FROM docum_files");
        else if (formatType == 4)   // контент в тексте
            qIn->SQL->Add("SELECT encode(vlogenie, 'escape')  as vlogenie, filename FROM docum_files");
        else
            qIn->SQL->Add("SELECT vlogenie, filename FROM docum_files");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->SQL->Add("AND ordern =:ordern");
        qIn->ParamByName("docum_id")->AsInteger() = documentId;  // Нужен As лонг!
        qIn->ParamByName("ordern")->AsInteger() = fileId;
        qIn->OpenBlob();
        if (!qIn->EoF()) {
            int file_number = 0;   //номер файла в архиве

            int bsize = qIn->FieldByName("vlogenie").GetByteaSize();
            char* source_char = qIn->FieldByName("vlogenie").AsBytea();
            fileName =  qIn->FieldByName("filename").AsString();

//            ofstream myFile ("zzz.zip", ios::out | ios::binary );
//            myFile.write (source_char, bsize);
//            //myFile.write (source_char2, bsize-2);
//            myFile.close();

            zip_error_t ze;
            zip_error_init(&ze);

            zip_source_t* zip_source;
            if  ((zip_source = zip_source_buffer_create(source_char, bsize, 0, &ze))  == NULL) {
                // Ошибка
                throw std::runtime_error("error in zip_source_buffer_create");
            };
            zip_t* zip;
            if ((zip = zip_open_from_source(zip_source, ZIP_RDONLY, &ze)) == NULL) {
                // Ошибка
                throw std::runtime_error("error in zip_open_from_source");
            };

            int  files_total = zip_get_num_files(zip); // количество файлов в архиве
            if (files_total < 1) {
                // Ошибка
                zip_source_close(zip_source);
                throw std::runtime_error("error in zip files below 1");
            }
            struct zip_file *file_in_zip; // дексриптор файла внутри архива
            file_in_zip = zip_fopen_index(zip, file_number, 0);

            int r;
            char buffer[10000];
            if (file_in_zip) {
                // читаем в цикле содержимое файла и выводим
                while ( (r = zip_fread(file_in_zip, buffer, sizeof(buffer))) > 0) {
                    fileContent.append(buffer,r);
//                    std::cout << "bytes readed:" << r << std::endl;
//                    std::cout << "fileContent length:" << fileContent.length() << std::endl;
//                    std::cout << "fileContent size:" << fileContent.size() << std::endl;
                };
//                ofstream myFile ("zzz.pdf", ios::out | ios::binary );
//                myFile.write(fileContent.data(), fileContent.length());
//                myFile.close();
                // закрываем файл внутри архива
                zip_fclose(file_in_zip);
            } else {
                // Ошибка
                throw std::runtime_error("error in zip_fopen_index");
            };
            zip_source_close(zip_source);
            zip_source_free(zip_source);

            result = true;
        }
        delete qIn;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}
*/


bool TAPIDocum::DocumentContent( long documentId, int fileId, std::string& fileName, std::string& fileContent ) {
    bool result = false;
    int formatType = 1;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        if (formatType == 2)   // контент в base64
            qIn->SQL->Add("SELECT encode(vlogenie, 'base64')  as vlogenie, filename FROM docum_files");
        else if (formatType == 3)  // контент в hex
            qIn->SQL->Add("SELECT encode(vlogenie, 'hex')  as vlogenie, filename FROM docum_files");
        else if (formatType == 4)   // контент в тексте
            qIn->SQL->Add("SELECT encode(vlogenie, 'escape')  as vlogenie, filename FROM docum_files");
        else
            qIn->SQL->Add("SELECT vlogenie, filename FROM docum_files");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->SQL->Add("AND ordern =:ordern");
        qIn->ParamByName("docum_id")->AsInteger() = documentId;  // Нужен As лонг!
        qIn->ParamByName("ordern")->AsInteger() = fileId;
        qIn->OpenBlob();
        if (!qIn->EoF()) {
            int file_number = 0;   //номер файла в архиве

            int bsize = qIn->FieldByName("vlogenie").GetByteaSize();
            char* source_char = qIn->FieldByName("vlogenie").AsBytea();
            fileName =  qIn->FieldByName("filename").AsString();

            zip_error_t ze;
            zip_error_init(&ze);

            zip_source_t* zip_source;
            if  ((zip_source = zip_source_buffer_create(source_char, bsize, 0, &ze))  == NULL) {
                // Ошибка
                throw std::runtime_error("error in zip_source_buffer_create");
            };
            zip_t* zip;
            if ((zip = zip_open_from_source(zip_source, ZIP_RDONLY, &ze)) == NULL) {
                TracerManager::Instance().AddSpanLog("Ошибка TAPIDocum::DocumentContent", {{"errordescr", "(zip = zip_open_from_source(zip_source, ZIP_RDONLY, &ze)) == NULL"}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                // Ошибка
                zip_error_fini(&ze);
                zip_source_close(zip_source);
                //zip_source_free(zip_source);
                throw std::runtime_error("error in zip_open_from_source");
            };

            int  files_total = zip_get_num_files(zip); // количество файлов в архиве
            if (files_total < 1) {
                // Ошибка
                TracerManager::Instance().AddSpanLog("Ошибка TAPIDocum::DocumentContent", {{"errordescr", "(files_total < 1)"}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                zip_close(zip);
                zip_error_fini(&ze);
                zip_source_close(zip_source);
                //zip_source_free(zip_source);
                throw std::runtime_error("error in zip files below 1");
            }
            struct zip_file *file_in_zip; // дексриптор файла внутри архива
            file_in_zip = zip_fopen_index(zip, file_number, 0);

            int r;
            char buffer[10000];
            if (file_in_zip) {
                // читаем в цикле содержимое файла и выводим
                while ( (r = zip_fread(file_in_zip, buffer, sizeof(buffer))) > 0) {
                    fileContent.append(buffer,r);
                };
                // закрываем файл внутри архива
                zip_fclose(file_in_zip);

                //int tmp_len = fileContent.size();  // for check only
                //std::cout << tmp_len << std::endl; // for check only
            } else {
                // Ошибка
                TracerManager::Instance().AddSpanLog("Ошибка TAPIDocum::DocumentContent", {{"errordescr", "(file_in_zip) == false"}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                zip_close(zip);
                zip_source_close(zip_source);
                zip_error_fini(&ze);
                //zip_source_free(zip_source);
                throw std::runtime_error("error in zip_fopen_index");
            };

            zip_close(zip);
            zip_error_fini(&ze);
            zip_source_close(zip_source);
            //zip_source_free(zip_source);

            result = true;
        }
        delete qIn;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::DocumentContent", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
    }
    return result;
}

long TAPIDocum::AddDocumOnly(std::string note) {
    long documentId = 0;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("INSERT INTO docums");
        qIn->SQL->Add("(note) VALUES ( '' ); ");
        qIn->SQL->Add("SELECT currval(pg_get_serial_sequence('docums', 'docum_id')) as docum_id");
        //qIn->ParamByName("docum_id")->AsString() = note;
        qIn->Open();
        if (!qIn->EoF()) {
            documentId = qIn->FieldByName("docum_id").AsInteger();
        }
        delete qIn;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return documentId;
}

bool TAPIDocum::newsaveDocument( long documentId, int PlanId, int VidOtch,
                                 int PokazId, int comnp_id, int cell_period, int God, int Kvrtl, int Mesjc, int Den,
                                 std::string fileName) {
    bool result = true;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.Conn);
        qIn->SQL->Clear();
        qIn->SQL->Add("INSERT INTO cnp_value");
        qIn->SQL->Add("(str_value, pokaz_id, comnp_id, vid_period, plan_id, vid_otc, god, kvrtl, mesjc, den, docum_id)");
        qIn->SQL->Add("VALUES (:fileName, :pokaz_id,:comnp_id,:vid_period,:plan_id,:vid_otc,:god,:kvrtl,:mesjc,:den,:docum_id)");
        qIn->ParamByName("pokaz_id")->AsInteger()  = PokazId;
        qIn->ParamByName("comnp_id")->AsInteger() = comnp_id;
        qIn->ParamByName("vid_period")->AsInteger() = cell_period;
        qIn->ParamByName("plan_id")->AsInteger() = PlanId;
        qIn->ParamByName("vid_otc")->AsInteger() = VidOtch;
        qIn->ParamByName("god")->AsInteger() = God;
        qIn->ParamByName("kvrtl")->AsInteger() = Kvrtl;
        qIn->ParamByName("mesjc")->AsInteger() = Mesjc;
        qIn->ParamByName("den")->AsInteger() = Den;
        qIn->ParamByName("docum_id")->AsString() = LongToHex(documentId);
        qIn->ParamByName("fileName")->AsString() = fileName;
        if (!qIn->ExecSQL()) {
            result = false;
        };
        delete qIn;
    }
    catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Ошибка TAPIDocum::newsaveDocument", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::newsaveDocument( long documentId, int ObjectId, int CellId,
                                 int PlanId, int VidOtch, TDateTime ValueDate,
                                 std::string fileName) {
    bool result = true;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("INSERT INTO Cell_value");
        qIn->SQL->Add("(str_value, objectid, cell_id, plan_id, vid_otc, value_date, docum_id)");
        qIn->SQL->Add("VALUES (:fileName, :objectid, :cell_id, :plan_id, :vid_otc,:value_date,:docum_id)");
        qIn->ParamByName("objectid")->AsInteger()  = ObjectId;
        qIn->ParamByName("cell_id")->AsInteger()  = CellId;
        qIn->ParamByName("plan_id")->AsInteger()  = PlanId;
        qIn->ParamByName("vid_otc")->AsInteger()  = VidOtch;
        qIn->ParamByName("value_date")->AsDateTime()  = ValueDate;
        qIn->ParamByName("docum_id")->AsString() = LongToHex(documentId);
        qIn->ParamByName("fileName")->AsString() = fileName;
        if (!qIn->ExecSQL()) {
            result = false;
        };
        delete qIn;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

/*
// Создание и сохранение в БД через промежуточный файл
bool TAPIDocum::updateFileContent( long documentId, int fileId, std::string fileName, std::string& fileContent) {
    bool result = true;

    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string struuid = "tmpdoc" + boost::uuids::to_string(uuid) + ".zip";
    char *filetmpzip = struuid.data();
    //char *filetmpzip = getTmpFileName();
    bool filetmpcreated = false;
    try {
        // Создание zip файла
        int err = 0;
        zip_t *zip = zip_open(filetmpzip, ZIP_CREATE, &err);
        auto src = zip_source_buffer_create(fileContent.c_str(), fileContent.length(), 0, 0);
        zip_file_add(zip, fileName.c_str(), src, ZIP_FL_ENC_UTF_8);
        zip_close(zip);
        filetmpcreated = true;

        // Скачивание zip файла в память
        FILE* file = fopen(filetmpzip, "r+b");
        if (file == NULL) return false;
        fseek(file, 0, SEEK_END);
        long int fileSize = ftell(file);
        rewind(file);
        unsigned char* buffRead = (unsigned char*)malloc(fileSize);
        size_t bytes_read = fread(buffRead, 1, fileSize, file);
        if (bytes_read != fileSize) return false;
        fclose(file);

        //Изменение БД из скачанного файла
        const char* paramValues[3];
        paramValues[0] = std::to_string(documentId).c_str();
        paramValues[1] = std::to_string(fileId).c_str();
        paramValues[2] = reinterpret_cast<const char*>(buffRead);

        const int paramFormats[3]{ 0, 0, 1 };
        const int paramLenghts[3]{ sizeof(paramValues[0]), sizeof(paramValues[1]), (int)fileSize};

        PGresult *res = PQexecParams(DefDBconnect.DocumDBConn,
                                     "UPDATE docum_files SET vlogenie = $3::bytea WHERE docum_id = $1::int4 and ordern = $2::int4 ",
//                                     "INSERT INTO docum_files (docum_id, ordern, vlogenie) VALUES($1::int4, $2::int4, $3::bytea) ",
                                     3,
                                     NULL,
                                     paramValues,
                                     paramLenghts,
                                     paramFormats,
                                     1);
        if ((res && PQresultStatus(res) == PGRES_COMMAND_OK)) {
            result = true;
        }
        else {
            result = false;
        }
        PQfreemem(res);

        remove(filetmpzip);
        filetmpcreated = false;
        return result;

//        std::ifstream infile(filetmpzip, std::ios_base::binary);
//        const int length = 100;
//        unsigned char bytes[length];
//        if (infile.read(reinterpret_cast<char*>(bytes), length)) {
//            size_t numRead = infile.gcount();
//            std::ostringstream oss;
//            for(size_t i = 0; i < numRead; ++i) {
//                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned short>(bytes[i]);
//            }
//            zipstr = oss.str();
//        }

//        std::ifstream fileziptmptoread(filetmpzip, ios::binary);
//        if (fileziptmptoread) {
//            ostringstream ss;
//            ostringstream sshex;
//            ss << fileziptmptoread.rdbuf();
//
//            int len = ss.str().length();
//            for (string::size_type i = 0; i < ss.str().length(); ++i)
//                sshex << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned short>(ss.str()[i]);
//            zipstr = sshex.str();
//        }


    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    if (filetmpcreated) remove(filetmpzip);
    return result;
}
*/

bool TAPIDocum::updateFileContent( long documentId, int fileId, std::string fileName, std::string& fileContent) {
    bool result = true;

    try {
        zip_error_t ze;
        zip_error_init(&ze);

        zip_source_t* zip_source_mem;
        if  ((zip_source_mem = zip_source_buffer_create(0, 0, 0, &ze))  == NULL) {
            TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::updateFileContent", {{"errordesr", "(zip_source_mem = zip_source_buffer_create(0, 0, 0, &ze))  == NULL"}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            throw std::runtime_error("UpdateContent: error in zip_source_buffer_create");
        };
        zip_source_keep(zip_source_mem);
        zip_t* zip_mem;
        if ((zip_mem = zip_open_from_source(zip_source_mem, ZIP_TRUNCATE, &ze)) == NULL) {
            TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::updateFileContent", {{"errordesr", "zip_mem = zip_open_from_source(zip_source_mem, ZIP_TRUNCATE, &ze)) == NULL"}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            zip_error_fini(&ze);
            zip_source_close(zip_source_mem);
            zip_source_free(zip_source_mem);
            throw std::runtime_error("UpdateContent:error in zip_open_from_source");
        };

        int err = 0;
        auto src = zip_source_buffer_create(fileContent.c_str(), fileContent.length(), 0, 0);
        zip_file_add(zip_mem, fileName.c_str(), src, ZIP_FL_ENC_UTF_8);
        zip_close(zip_mem);

        zip_source_open(zip_source_mem);
        zip_source_seek(zip_source_mem, 0, SEEK_END);
        zip_int64_t sz = zip_source_tell(zip_source_mem);
        zip_source_seek(zip_source_mem, 0, SEEK_SET);
        unsigned char* buffRead  = (unsigned char*)malloc(sz);
        zip_source_read(zip_source_mem, buffRead, sz);

        zip_error_fini(&ze);
        zip_source_close(zip_source_mem);
        zip_source_free(zip_source_mem);

        long int fileSize = sz;

        //Изменение БД из скачанного файла
        const char* paramValues[3] = {std::to_string(documentId).c_str() ,
                                      std::to_string(fileId).c_str(),
                                      reinterpret_cast<const char*>(buffRead)};

        const int paramFormats[3]{ 0, 0, 1 };
        const int paramLenghts[3]{ sizeof(paramValues[0]), sizeof(paramValues[1]), (int)fileSize};

        PGresult *res = PQexecParams(DefDBconnect.DocumDBConn,
                                     "UPDATE docum_files SET vlogenie = $3::bytea WHERE docum_id = $1::int4 and ordern = $2::int4 ",
                                     3,              /* params */
                                     NULL,           /* let the backend deduce param type */
                                     paramValues,
                                     paramLenghts,   /* don't need param lengths since text */
                                     paramFormats,   /* default to all text params */
                                     1);
        if ((res && PQresultStatus(res) == PGRES_COMMAND_OK)) {
            result = true;
        }
        else {
            TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::updateFileContent", {{"errordesr", "SQL result unsuccessful"}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            result = false;
        }
        PQfreemem(res);
        free(buffRead);
   }
   catch (const std::exception& e) {
       TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::updateFileContent", {{"errordesr", e.what()}},
                                            TracerManager::LOG_LEVEL_ERROR);
       std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
       result = false;
   }
   return result;
}

bool TAPIDocum::addFile( long documentId, int fileId, std::string fileNote, std::string fileName, std::string& fileContent ) {
    bool result = true;
//    result = TAPIDocum::updateFileContent( documentId, fileId, fileName, fileContent);
//    return result;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("INSERT INTO docum_files");
        qIn->SQL->Add("(docum_id, ordern, note, filename)");
        qIn->SQL->Add("VALUES (:docum_id, :ordern, :note, :filename)");
        qIn->ParamByName("docum_id")->AsInteger()  = documentId;  // Нужен As лонг!
        qIn->ParamByName("ordern")->AsInteger()  = fileId;
        qIn->ParamByName("note")->AsString() = fileNote;
        qIn->ParamByName("filename")->AsString() = fileName;
        if (!qIn->ExecSQL()) {
            result = false;
        }
        else {
            result = TAPIDocum::updateFileContent( documentId, fileId, fileName, fileContent);
        }
        delete qIn;
    }
    catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::addFile", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::updateFile(long documentId, int fileId, std::string fileNote, std::string fileName, std::string& fileContent) {
    bool result = true;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("UPDATE docum_files");
        qIn->SQL->Add("SET filename=:filename, note=:note");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->SQL->Add("AND ordern =:ordern");
        qIn->ParamByName("docum_id")->AsInteger()  = documentId;  // Нужен As лонг!
        qIn->ParamByName("ordern")->AsInteger()  = fileId;
        qIn->ParamByName("note")->AsString() = fileNote;
        qIn->ParamByName("filename")->AsString() = fileName;
        if (!qIn->ExecSQL()) {
            result = false;
        }
        else {
            result = TAPIDocum::updateFileContent( documentId, fileId, fileName, fileContent);
        }
        delete qIn;
    }
    catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::updateFile", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::deleteFile(long documentId, int fileId) {
    bool result = true;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("DELETE FROM docum_files");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->SQL->Add("AND ordern =:ordern");
        qIn->ParamByName("docum_id")->AsInteger()  = documentId;   // Нужен As лонг!
        qIn->ParamByName("ordern")->AsInteger()  = fileId;
        if (!qIn->ExecSQL()) {
            result = false;
        };
        delete qIn;
    }
    catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::deleteFile", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::getCountFiles(long documentId, int& countFiles) {
    bool result = true;
    countFiles = -1;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("SELECT COUNT(*) as countfiles FROM docum_files");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->ParamByName("docum_id")->AsInteger()  = documentId;   // Нужен As лонг!
        qIn->Open();
        if (!qIn->EoF()) {
            countFiles = qIn->FieldByName("countfiles").AsInteger();
        };
        delete qIn;
    }
    catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::getCountFiles", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::deleteDocument(long documentId) {
    bool result = true;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("DELETE FROM docums");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->ParamByName("docum_id")->AsInteger()  = documentId;   // Нужен As лонг!
        if (!qIn->ExecSQL()) {
            result = false;
        };
        delete qIn;
    }
    catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TAPIDocum::deleteDocument", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::deleteDocumentRelation(long documentId) {
    bool result = true;
    try {
        int ObjectId, CellId, PlanId, VidOtch,
                PokazId, ComnpId, VidPeriod, God, Kvrtl, Mesjc, Den;
        TDateTime ValueDate;
        if (TDocumentValueLogic::GetDocumRelation(LongToHex(documentId),
                                     ObjectId, CellId, PlanId, VidOtch, ValueDate,
                                     PokazId, ComnpId, VidPeriod, God, Kvrtl, Mesjc, Den)) {
            TQueryUni *qIn = new TQueryUni(DefDBconnect.Conn);
            qIn->SQL->Clear();
            if (PokazId) {
                qIn->SQL->Add("DELETE FROM  cnp_value");
                qIn->SQL->Add("WHERE docum_id = :docum_id");
            }
            else if (ObjectId) {
                qIn->SQL->Add("DELETE FROM  cell_value");
                qIn->SQL->Add("WHERE docum_id = :docum_id");
            }
            qIn->ParamByName("docum_id")->AsString() = LongToHex(documentId);
            if (!qIn->ExecSQL()) {
                result = false;
            };
            delete qIn;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

bool TAPIDocum::getMaxFileId(long documentId, int& maxFileId) {
    bool result = true;
    maxFileId = 0;
    try {
        TQueryUni *qIn = new TQueryUni(DefDBconnect.DocumDBConn);
        qIn->SQL->Clear();
        qIn->SQL->Add("SELECT MAX(ordern) as maxFileId FROM docum_files");
        qIn->SQL->Add("WHERE docum_id =:docum_id");
        qIn->ParamByName("docum_id")->AsInteger()  = documentId;  // Нужен As лонг!
        qIn->Open();
        if (!qIn->EoF()) {
            maxFileId = qIn->FieldByName("maxFileId").AsInteger();
        };
        delete qIn;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
        result = false;
    }
    return result;
}

