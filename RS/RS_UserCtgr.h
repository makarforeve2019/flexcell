//
// Created by Alex on 15.04.2024.
//

#ifndef MWSRESTAPI_RS_USERCTGR_H
#define MWSRESTAPI_RS_USERCTGR_H
#include <string>

class Userctgr {
public:
    // Чтение всех категорий для пользователя user_id - сортировано по наименованию категории
    static std::string GetUserctgrAll(int context_id);
    //  Получение содержания пользовательской категории
    static std::string GetUserctgrContent(int userctgr_id, int year);
    //  Получение всех категорий пользователя, сортированных по окончанию года жизни end_date
    static std::string GetUserctgrUserAllEndDate(int user_id);
    // Получение свойства только ReadOnly
    static std::string GetUserUserctgrRO(int context_id, int userctgr_id);
    //Состояние чтения/редактирования при формировании проекта ПСЭР
    static std::string ReadOnlyGrpsCtgrs(int context_id, int userctgr_id, int year);
};


#endif //MWSRESTAPI_RS_USERCTGR_H
