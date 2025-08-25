#ifndef DB_USERPLAN_H
#define DB_USERPLAN_H

#include "DB_Common.h" // Включение необходимых заголовочных файлов

#include "DB_Common.h"

class TDB_UserPlan {
public:
    // Чтение полномочий текущего пользователя для заданного plan_id
    static int GetUserPlan_Perm(int plan_id);
    // Чтение полномочий пользователя с user_id для заданного plan_id
    static int GetUserPlan_Perm(int user_id, int plan_id);
    // Чтение полномочий текущего пользователя для plan_id Префектуры
    static int GetUserPref_Perm();
    // Загрузить полномочия текущего пользователя для всех plan_id
    static void LoadPlanPermForCurUser();
    static void LoadPlanPermForCurUser(TQueryUni *q, int user_id);
    // Чтение полномочий пользователя
    static void GetUserPerm(TQueryUni *qIn, int user_id);
    // Удаление всех планов для пользователя
    static void DelPlansForUser(int user_id);
    // Проверка наличия у пользователя полномочий для plan_id
    static bool isUserPlan(int user_id, int plan_id);
    // Удаление у пользователя полномочий для plan_id
    static void DelUserPlan(int user_id, int plan_id);
    // Добавление полномочий для пользователя
    static void AddUserPerm(int user_id, int plan_id, int user_perm);
    // Обновление полномочий для пользователя
    static void UpdUserPerm(int user_id, int plan_id, int user_perm);
};

#endif // DB_USERPLAN_H
