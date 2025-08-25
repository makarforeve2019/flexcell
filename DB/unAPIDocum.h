#ifndef MWSRESTAPI_UNAPIDOCUM_H
#define MWSRESTAPI_UNAPIDOCUM_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "ClassesUtils.h"

class TAPIDocum {
private:
    static char* getTmpFileName();
public:
    // true если у документа есть файлы
    static bool ReadDocum(std::string Docum_id, TStringList& slFileName, TStringList& slFileNote, std::string& DocumNote);
    // true если у документа и файла есть контент
    static bool DocumentContent( long documentId, int fileId, std::string& fileName, std::string& fileContent );

    static long AddDocumOnly(std::string note);

    static bool newsaveDocument( long documentId, int PlanId, int VidOtch,
                                 int PokazId, int comnp_id, int cell_period, int God, int Kvrtl, int Mesjc, int Den,
                                 std::string fileName);
    static bool newsaveDocument( long documentId, int ObjectId, int CellId,
                                 int PlanId, int VidOtch, TDateTime ValueDate,
                                 std::string fileName);
    static bool updateFileContent( long documentId, int fileId, std::string fileName, std::string& fileContent);

    static bool addFile( long documentId, int fileId, std::string fileNote, std::string fileName, std::string& fileContent );

    static bool updateFile(long documentId, int fileId, std::string fileNote, std::string fileName, std::string& fileContent);

    static bool deleteFile(long documentId, int fileId);

    static bool getCountFiles(long documentId, int& countFiles);

    static bool deleteDocument(long documentId);

    static bool deleteDocumentRelation(long documentId);

    static bool getMaxFileId(long documentId, int& maxFileId);
};
;
#endif //MWSRESTAPI_UNAPIDOCUM_H
