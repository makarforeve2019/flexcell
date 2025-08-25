//
// Created by Alex on 17.04.2024.
//

#ifndef MWSRESTAPI_RS_PLAN_H
#define MWSRESTAPI_RS_PLAN_H

#include <string>

class Plan {
public:
// Выбор всех субъектов планирования для пользователя
static std::string GetAllPlanForUser(int user_id);
// Полномочия текущего пользователя по всем plan_id для пользователя
static std::string LoadPlanPermForCurUser(int user_id);
};


#endif //MWSRESTAPI_RS_PLAN_H
