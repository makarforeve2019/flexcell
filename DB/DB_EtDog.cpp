#include "DB_EtDog.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Pokaztl.h"
#include "DB_PlanStr.h"
#include "unDefDBconnect.h"

int class_EtDog_id = 0;
int EtDog_nacalo_id = 0;
int EtDog_okoncanie_id = 0;

int TDB_EtDog::GetIdClassEtDog() {
    if (class_EtDog_id == 0)
        class_EtDog_id = TDB_Pokaztl::GetIdClass("Этап договора", true);
    return class_EtDog_id;
}

int TDB_EtDog::GetField_id(const std::string& field_name) {
    int result = 0;
    TQueryUni *qFld = new TQueryUni(DefDBconnect.Conn);
    TDB_EtDog::GetIdClassEtDog();
    if (class_EtDog_id == 0)
        return result;

    TDB_PlanStr::GetAllSubClassItem(qFld, class_EtDog_id, Date()); // Assuming Date is a defined variable
    while (!qFld->EoF()) {
        if (qFld->FieldByName("pokaz_name").AsString() == field_name) {
            result = qFld->FieldByName("pokaz_id").AsInteger();
            break;
        }
        qFld->Next();
    }
    delete qFld;
    return result;
}

int TDB_EtDog::GetEtDogNacaloId() {
    if (EtDog_nacalo_id == 0) {
        EtDog_nacalo_id = GetField_id("Начало");
    }
    if (EtDog_nacalo_id == 0) {
        //throw std::runtime_error("В классе Этап договора отсутствует поле Начало");
    }
    return EtDog_nacalo_id;
}

int TDB_EtDog::GetEtDogOkoncanieId() {
    if (EtDog_okoncanie_id == 0)
        EtDog_okoncanie_id = GetField_id("Окончание");
    if (EtDog_okoncanie_id == 0){
        //throw Exception("В классе Этап договора отсутствует поле Окончание");
    }
    return EtDog_okoncanie_id;
}



