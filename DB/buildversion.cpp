//#include "buildversion.h"
//#include "DB_Common.h"
//#include "unDefDBconnect.h"
//
//void buildversion::CheckAndCreateVersionTable() {
//    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
//
//    // �������� ������� �������
//    q->SQL->Clear();
//    q->SQL->Add("SELECT to_regclass('public.buildversion');");
//    q->Open();
//    bool tableExists = !q->FieldByName("to_regclass")->IsNull();
//    q->Close();
//
//    // ���� ������� �� ����������, ������� ��
//    if (!tableExists) {
//        q->SQL->Clear();
//        q->SQL->Add(
//                "CREATE TABLE buildversion ("
//                "id SERIAL PRIMARY KEY, "
//                "version INT NOT NULL, "
//                "dateversion TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
//                ");"
//        );
//        q->ExecSQL();
//    }
//
//    delete q;
//}
//
//// ������� ��� ��������� ������� �������� ������������ �����
//std::time_t GetExecutableCreationTime(const std::string &path) {
//    struct stat fileInfo;
//    if (stat(path.c_str(), &fileInfo) == 0) {
//        return fileInfo.st_mtime; // ���������� ����� ���������� ��������� �����
//    } else {
//        throw std::runtime_error("Unable to get file info for: " + path);
//    }
//}
//
//void buildversion::UpdateVersionIfNeeded(const string &executablePath) {
//    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
//
//    // �������� ����� �������� ������������ �����
//    TDateTime exeCreationTime = TBuildversion::GetExecutableCreationTime(executablePath);
//
//    // �������� ����� ���������� ���������� ������ �� ���� ������
//    q->SQL->Clear();
//    q->SQL->Add("SELECT dateversion FROM buildversion ORDER BY id DESC LIMIT 1;");
//    q->Open();
//    TDateTime lastUpdateTime = 0;
//    if (!q->EoF()) {
//        lastUpdateTime = q->FieldByName("dateversion")->AsDateTime();
//    }
//    q->Close();
//
//    // ���������� �������
//    if (exeCreationTime != lastUpdateTime) {
//        // �������� ������� ������
//        q->SQL->Clear();
//        q->SQL->Add("SELECT version FROM buildversion ORDER BY id DESC LIMIT 1;");
//        q->Open();
//        int currentVersion = 0;
//        if (!q->EoF()) {
//            currentVersion = q->FieldByName("version")->AsInteger();
//        }
//        q->Close();
//
//        // ����������� ������
//        int newVersion = currentVersion + 1;
//
//        // ��������� ������ � ���� ������
//        q->SQL->Clear();
//        q->SQL->Add("INSERT INTO buildversion (version, dateversion) VALUES (:new_version, to_timestamp(:exe_time));");
//        q->ParamByName("new_version")->AsInteger() = newVersion;
//        q->ParamByName("exe_time")->AsDateTime() = exeCreationTime;
//        q->ExecSQL();
//    }
//
//    delete q;
//}
//
//int buildversion::GetCurrentVersion() {
//    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
//
//    // ��������� ������� ������
//    q->SQL->Clear();
//    q->SQL->Add("SELECT version FROM buildversion ORDER BY id DESC LIMIT 1;");
//    q->Open();
//    int currentVersion = 0;
//    if (!q->EoF()) {
//        currentVersion = q->FieldByName("version")->AsInteger();
//    }
//    q->Close();
//
//    delete q;
//    return currentVersion;
//}
