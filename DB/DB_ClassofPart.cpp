#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "DB_ClassofPart.h"

void TDB_ClassofPart::GetClassConteiners(TQueryUni* qIn, int class_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Class_ofpart, Pokaztl");
    qIn->SQL->Add("WHERE class_id = :class_id");
    qIn->SQL->Add("AND pokaz_id = conteiner_id");
    qIn->ParamByName("class_id")->AsInteger() = class_id;
    qIn->Open();
}

void TDB_ClassofPart::GetSubElems(TQueryUni *qIn, int conteiner_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Class_ofpart, Pokaztl");
    qIn->SQL->Add("WHERE conteiner_id = :conteiner_id");
    qIn->SQL->Add("AND pokaz_id = class_id");
    qIn->ParamByName("conteiner_id")->AsInteger() = conteiner_id;
    qIn->Open();
//    qIn->Last();
//    qIn->First();
}


int TDB_ClassofPart::GetClassConteiner1(int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    GetClassConteiners(q, class_id);
    if (!q->EoF()) {
        result = q->FieldByName("conteiner_id")->AsInteger();
    }
    delete q;
    return result;
}

int TDB_ClassofPart::GetCastConteiner1(int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = class_id;
    GetClassConteiners(q, class_id);
    while (!q->EoF()) {
        int wId = q->FieldByName("conteiner_id")->AsInteger();
        if (TDB_Pokaztl::GetTypePokaz(wId) != 1) {
            result = wId;
            break;
        }
        q->Next();
    }
    delete q;
    return result;
}

void TDB_ClassofPart::GetAllItems(TQueryUni *qIn) {
    qIn->SQL->Clear();
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    qIn->SQL->Add("SELECT conteiner_id, class_id, a.pokaz_name as name1, b.pokaz_name as name2");
    qIn->SQL->Add("FROM Class_ofpart, Pokaztl a, Pokaztl b");
    qIn->SQL->Add("WHERE a.pokaz_id = conteiner_id");
    qIn->SQL->Add("AND b.pokaz_id = class_id");
    qIn->Open();
}

void TDB_ClassofPart::GetConteinerAll(TQueryUni *qIn) {
    qIn->SQL->Clear();
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl");
    qIn->SQL->Add("WHERE pokaz_owner_id = 0");
    qIn->SQL->Add("AND pokaz_type > 30");
    qIn->Open();
}

void TDB_ClassofPart::AddElem(int conteiner_id, int class_id) {
    if (IsElem(conteiner_id, class_id))
        return;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Class_ofpart");
    q->SQL->Add("(conteiner_id, class_id)");
    q->SQL->Add("values (:conteiner_id, :class_id)");
    q->ParamByName("conteiner_id")->AsInteger() = conteiner_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    delete q;
}

bool TDB_ClassofPart::IsElem(int conteiner_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Class_ofpart");
    q->SQL->Add("WHERE conteiner_id = :conteiner_id");
    q->SQL->Add("AND class_id = :class_id");
    q->ParamByName("conteiner_id")->AsInteger() = conteiner_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->Open();
    if (!q->EoF())
        Result = true;
    delete q;
    return Result;
}

bool TDB_ClassofPart::IsClass(int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Class_ofpart");
    q->SQL->Add("WHERE (conteiner_id = :class_id OR class_id = :class_id)");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->Open();
    if (!q->EoF())
        Result = true;
    delete q;
    return Result;
}

void TDB_ClassofPart::DelClassElems(int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Class_ofpart");
    q->SQL->Add("WHERE class_id = :class_id");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    delete q;
}

void TDB_ClassofPart::DelClassElem(int conteiner_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Class_ofpart");
    q->SQL->Add("WHERE conteiner_id = :conteiner_id");
    q->SQL->Add("AND class_id = :class_id");
    q->ParamByName("conteiner_id")->AsInteger() = conteiner_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    delete q;
}

//void TDB_ClassofPart::GetSubElems(TQueryUni *qIn, int conteiner_id) {
//    qIn->SQL->Clear();
//    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
//    qIn->SQL->Add("SELECT *");
//    qIn->SQL->Add("FROM Class_ofpart, Pokaztl");
//    qIn->SQL->Add("WHERE conteiner_id = :conteiner_id");
//    qIn->SQL->Add("AND pokaz_id = class_id");
//    qIn->ParamByName("conteiner_id")->AsInteger() = conteiner_id;
//    qIn->Open();
//    qIn->Last();
//    qIn->First();
//}

int TDB_ClassofPart::GetSubElems1(int conteiner_id, std::string &name1) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetSubElems(q, conteiner_id);
    int n = 0;
    while (!q->EoF()) {
        n++;
        if (n == 1) name1 = q->FieldByName("pokaz_name")->AsString();
        q->Next();
    }
    delete q;
    return n;
}

int TDB_ClassofPart::GetSubElems1(int conteiner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetSubElems(q, conteiner_id);
    int result = 0;
    if (!q->EoF())
        result = q->FieldByName("class_id")->AsInteger();
    delete q;
    return result;
}

bool TDB_ClassofPart::getConteinerClassName(int CastId, std::string &aConteinerClassName, int &aConteinerClassId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    GetClassConteiners(q, CastId);
    while (!q->EoF()) {
        if (q->FieldByName("pokaz_type")->AsInteger() % 10 == 1) {
            aConteinerClassName = q->FieldByName("pokaz_name")->AsString();
            aConteinerClassId = q->FieldByName("pokaz_id")->AsInteger();
            result = true;
            break;
        }
        q->Next();
    }
    delete q;
    return result;
}
