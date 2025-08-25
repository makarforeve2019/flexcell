#ifndef DB_ENUM_H
#define DB_ENUM_H

#include "DB_Common.h"


class TDB_Enum {
public:
    // получает всю информацию про элемент
    static bool GetEnum(TQueryUni *qIn, int enum_id);
    
    // получает всю информацию про элемент по имени
    static bool GetEnumByName(TQueryUni *qIn, std::string enum_name);
    
    // получает наименование элемента
    static std::string GetEnumName(int enum_id);
    
    // получает порядковый номер элемента
    static int GetOrder(int enum_id);
    
    // получает номер п/п элемента, нумерация с 0.
    static int GetNPP(int enum_id);
    
    // получает enum_id элемента
    static int GetEnumId(std::string enum_name);
    
    // получает enum_id градации
    static int GetEnumGradId(int enum_id, std::string enumGrad_name);
    
    // получает всю информацию о всех перечислениях
    static void GetEnumAll(TQueryUni *qIn);
    
    // получает всю информацию о всех градациях перечисления
    static void GetEnumGradAll(TQueryUni *qIn, int enum_owner_id);
    
    // получает всю информацию о всех градациях перечисления в виде одной строки
    static std::string GetTextGradAll(int enum_owner_id);
    
    // добавляет перечисление
    static int AddEnum(std::string enum_name);
    
    // добавляет градацию перечисления
    static int AddEnumGrad(std::string enum_name, int enum_owner_id);
    
    // изменяем наименование
    static void UpdNameEnum(std::string enum_name, int enum_id);
    
    // удаляем элемент
    static void DelEnum(int enum_id);
    
    // меняет местами два элемента
    static void ChangeOrders(int enum_id_1, int enum_order_1, int enum_id_2, int enum_order_2);
    
    // получает enum_id градации по владельцу enum_id
    static int GetEnumIdGrad(int enum_id, int nP_P);
};

#endif // DB_ENUM_H
