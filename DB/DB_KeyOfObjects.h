#ifndef DB_KEYOFOBJECTS_H
#define DB_KEYOFOBJECTS_H

#include <string>

#include "FormTypes.h"
#include "DB_Common.h"

class TDB_KeyOfObjects {
public:
    // установить ключ объекта
    static void SetKeyObject(int objectid, int class_id_ForKey, const std::string& str_value, int god);
    // проверить, нет ли другого объекта с таким же ключом
    static bool IsKeyObject(int class_id_ForKey, const std::string& str_value, int god, int objectid);
    // найти объект по его ключу
    static bool GetIdObjectByKey(int class_id, std::string value_key, int& Objectid, int god);
    // выдать ключ по объекту
    static std::string GetKeyByIdObject(int Objectid);
    // удалить ключ по объекту и году
    static void DelKeyByIdObjectGod(int Objectid, int god);
    // установить и вернуть ключ объекта, вычислив его автоматически для АвтоНомера
    static std::string SetKeyObjectAvtoNumber(int objectid, int class_id_ForKey, int god);
};

#endif // DB_KEYOFOBJECTS_H
