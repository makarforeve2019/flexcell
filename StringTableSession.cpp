//
// Created by Alex on 10.05.2024.
//

#include "StringTableSession.h"

TStringTableSession::TStringTableSession() {
    slTableSessions = new TStringList();
    //slTableSessions->Sorted = true;
}

TStringTableSession::~TStringTableSession() {
    delete slTableSessions;
}

void TStringTableSession::PrintAll() {
    for (int i = 0; i < slTableSessions->Count(); i++) {
        std::string sessionInfo = slTableSessions->Strings[i];
//        std::cout << sessionInfo << std::endl;
    }
}


void TStringTableSession::AddSessionData(const string& tblsessionId, TFormLogicF1* formLogicF1) {
    slTableSessions->AddObject(tblsessionId, formLogicF1); // ��������� ��� ��������� ������
}

// ��������� ������ ������
bool TStringTableSession::GetSessionData(const string& tblsessionId, TFormLogicF1*& formLogicF1) {
    int index;
    bool found = slTableSessions->Find(tblsessionId, index);
    if (found) {
//        formLogicF1 = std::get<TFormLogicF1*>(slTableSessions->Objects[index]); // �������� ������ ������
        formLogicF1 = slTableSessions->getObjectByKey<TFormLogicF1*>(tblsessionId);
        return true;
    }
    return false;
}

// �������� ������
void TStringTableSession::DeleteSession(const string& tblsessionId) {
    int index;
    bool found = slTableSessions->Find(tblsessionId, index);
    if (found) {
//        delete std::get<TFormLogicF1*>(slTableSessions->Objects[index]); // ������� ������ ������ �����
        delete slTableSessions->getObjectByKey<TFormLogicF1*>(tblsessionId);
        slTableSessions->Delete(index); // ������� ������ �� ������
    }
}

// ������� ���������� ������
void TStringTableSession::CleanupSessions() {
    TDateTime currentTime = Now();
    for (int i = slTableSessions->Count() - 1; i >= 0; --i) {
        TFormLogicF1* session = std::get<TFormLogicF1*>(slTableSessions->Objects[i]);
        double hoursSinceLastUse = HoursBetween(currentTime, session->dataLastInstanceUsed);
        if (hoursSinceLastUse > MaxInactiveHours) {
            delete session; // ������� ������ ������
            slTableSessions->Delete(i); // ������� ������ �� ������
        }
    }
}

int TStringTableSession::GetCount() {
    return slTableSessions->Count();
}

bool TStringTableSession::UpdateSessionData(const std::string& tblsessionId, TFormLogicF1* newFormLogicF1) {
    int index;
    if (slTableSessions->Find(tblsessionId, index)) {
        // Удаляем старый объект, если нужно
        delete slTableSessions->getObjectByKey<TFormLogicF1*>(tblsessionId);
        // Обновляем ссылку
        slTableSessions->Objects[index] = newFormLogicF1;
        return true;
    }
    // Если идентификатор не найден
    return false;
}