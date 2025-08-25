#ifndef DB_GRPSCTGRS_H
#define DB_GRPSCTGRS_H

#include <iostream>

#include "DB_Common.h"
#include "unDefDBconnect.h"



class TDB_GrpsCtgrs {
public:
    static int getIsReadOnly_GrpsCtgrs(int user_id, int userctgr_id, int god); // вернуть состояние чтения/редактирования при формировании проекта ПСЭР
    static int GetId(); // возвращает grct_id добавленной записи
    static bool IsPlanirGod(int God); // Проверить, нет ли такого планируемого года
    static int AddPlanirGod(int god); // добавить новый планируемый год
    static int AddRazrabotcik(int god, int user_id, int userctgr_id); // добавить нового разработчика
    static void GetAll1(TQueryUni* qIn); // чтение всех строк для вида 1
    static void GetForGrctId(TQueryUni* qIn, int grct_id); // чтение для grct_id - это заголоаок, здесь user_id = 0
    static void GetForGod(TQueryUni* qIn, int god); // чтение для god
    static void DelItem(int grct_id); // удаление строки
    static void UpdReadOnly(int grct_id, int isReadOnly); // изменить признак редактирования
    static void UpdDocimageId(int grct_id, int docimage_id); // изменить docimage_id
};

#endif // DB_GRPSCTGRS_H
