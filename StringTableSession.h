//
// Created by Alex on 10.05.2024.
//

#ifndef MWSRESTAPI_STRINGTABLESESSION_H
#define MWSRESTAPI_STRINGTABLESESSION_H


#include "ClassesUtils.h"
#include "unFormLogicF1.h"

class TStringTableSession  {
public:
    TStringList* slTableSessions;

    int MaxInactiveHours = 1;


    TStringTableSession();

    virtual ~TStringTableSession();


    void PrintAll();


    void AddSessionData(const std::string& tblsessionId, TFormLogicF1* formLogicF1);



    bool GetSessionData(const std::string& tblsessionId, TFormLogicF1*& formLogicF1);


    void DeleteSession(const std::string& tblsessionId);


    void CleanupSessions();

    int GetCount();

    bool UpdateSessionData(const std::string& tblsessionId, TFormLogicF1* newFormLogicF1);
};


#endif //MWSRESTAPI_STRINGTABLESESSION_H
