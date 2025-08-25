
#ifndef DB_BKKBK_H
#define DB_BKKBK_H

#include <iostream>
#include "DB_Common.h"

class TDB_bkKBK {
public:
    // Получает всю информацию про все элементы, подчиненные kbk_id
    static void GetSubAll(TQueryUni* qIn, int kbk_id);
    // Получает всю информацию про все элементы, подчиненные kbk_id, но подходящие по времени
    static void GetSubAllByGod(TQueryUni* qIn, int kbk_id, int god);
    // Получает всю информацию про все элементы в диапазоне kbk_id0, kbk_id1
    static void GetAllByIds(TQueryUni* qIn, int kbk_id0, int kbk_id1);
    // Добавляет корневой элемент, если его нет
    static void AddKBK0(int kbk_id, int orderN, std::string nameKBK, std::string codeKBK);
    // Добавляет элемент
    static int AddKBK(int kbk_owner_id, int vid, std::string nameKBK, std::string codeKBK, int beg_God, int end_God);
    // Дать новый kbk_id по vid
    static int GetKBK_id(int vid);
    // Дать новый orderN по kbk_owner_id
    static int GetOrderN(int vid, int kbk_owner_id);
    // Дать следующий элемент kbk_owner_id и kbk_id
    static int GetNextKBKid(int kbk_owner_id, int kbk_id);
    // Дать информацию про kbk_id
    static void GetKBKInfo(TQueryUni* qIn, int kbk_id);
    // Дать codeKBK пр kbk_id
    static std::string GetCodeKBK(int kbk_id);
    // Дать название пр kbk_id
    static std::string GetName(int kbk_id);
    // Дать kbk_owner_id по kbk_id
    static int GetOwnerId(int kbk_id);
    // Есть ли подчиненные у kbk_owner_id
    static bool isOwnerId(int kbk_owner_id);
    // Дать название по codeKBK
    static std::string GetNameByCodeKBK(std::string codeKBK);
    // Дать kbk_id по codeKBK
    static int GetKBKidByCodeKBK(std::string codeKBK, int god);
    // Дать kbk_id для КОСГУ
    static int GetKBKidByKOSGU(std::string codeKBK, int god);
    // Модифицирует элемент
    static void ModKBK(int kbk_id, std::string nameKBK, std::string codeKBK, int beg_God, int end_God);
    // Удаляет элемент
    static bool DelKBK(int kbk_id);
    // Поменять номерами элементы
    static void UpdOrderN(int kbk_id_1, int kbk_id_2);
};

#endif // DB_BKKBK_H
