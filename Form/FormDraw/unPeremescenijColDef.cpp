#include "unPeremescenijColDef.h"
#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Plan.h"

const int L_kolLines = 10;
std::string L_arrLines[L_kolLines] = {
    "Объем перемещений для бюджетополучателя",
    "Объем перемещений для бюджетополучателя (85) - Снято (со знаком +)",
    "Объем перемещений для бюджетополучателя (85) - Положено (со знаком +)",
    "Объем перемещений для ВСЕХ",
    "Объем перемещений для ВСЕХ (85) - Снято (со знаком +)",
    "Объем перемещений для ВСЕХ (85) - Положено (со знаком +)",
    "Объем перемещений для бюджетополучателя (26 - распределение) - Снято (со знаком +)",
    "Объем перемещений для бюджетополучателя (26 - распределение) - Положено (со знаком +)",
    "Объем перемещений для ВСЕХ (26 - распределение) - Снято (со знаком +)",
    "Объем перемещений для ВСЕХ (26 - распределение) - Положено (со знаком +)"
};

int TPeremescenijColDef::kolLines() {
    return L_kolLines;
}

std::string TPeremescenijColDef::nextLine(int aN) {
    if (aN > L_kolLines || aN < 1)
        return "";
    else
        return L_arrLines[aN - 1]; // Adjust for 0-based indexing in arrays
}
