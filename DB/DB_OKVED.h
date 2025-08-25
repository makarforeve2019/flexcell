
#ifndef DB_OKVED_H
#define DB_OKVED_H

#include "DB_Common.h"
#include <string>

class TDB_OKVED {
public:
    // получает okved_id добавленного элемента
    static int GetId();

    // добавляет элемент
    static int AddItem(int ownerId, std::string okved_kod, std::string okved_name, int orderN, std::string Primecanie);

    // получить всю информацию по владельцу
    static void GetAllItemsOwner(TQueryUni* qIn, int ownerId);

    // вернуть max orderN по владельцу
    static int GetMaxOrderNbyOwner(int ownerId);

    // выборка элементов по подстроке
    static void GetItemsBySubstr(TQueryUni* qIn, std::string substr);

    // выборка элемента по коду
    static void GetItemByCode(TQueryUni* qIn, std::string okved_kod);

    // выборка элементов по подстроке в пределах узла
    static void GetItemsBySubstrInUzel(TQueryUni* qIn, std::string substr, std::string inUzel);

    // Изменить Primecanie по okved_id
    static void UpdPrimecanie(int okved_id, std::string Primecanie);

    // есть ли подчиненные элементы у владельца
    static bool isItemsOwner(int ownerId);

    // получает всю информацию про элемент по okved_id
    static bool GetOKVED(TQueryUni* qIn, int okved_id);

    // получает наименование элемента по okved_id
    static std::string GetOKVEDName(int okved_id);

    // получает код элемента по okved_id
    static std::string GetOKVEDCode(int okved_id);

    // получает Primecanie по okved_id
    static std::string GetPrimecanie(int okved_id);

    // получает okved_id по коду элемента
    static int GetOkvedIdByCode(std::string okved_kod);
};
#endif