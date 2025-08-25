#ifndef FORMGF_LOGIC_H
#define FORMGF_LOGIC_H

#include "unF1toDE.h"
#include "unFormLogicF1.h"
#include "../FormTypes.h"
#include "../DB/DB_Common.h"

const std::string splan = "план";
const std::string sfact = "факт";

class TFormGF_Logic {
public:
     // проверка наличия столбца с мероприятием для формы с объектами
    static bool IsColMeropr(TFormLogicF1* FormLogicF1);
    // достраивает форму для 10, 13, 14 типа
    static void DoForm10_13_14(TInfoFormLogic* InfoFormLogic, TFormLogicF1* FormLogicF1, int aWork_with_gf);
     // достраивает форму для 11 типа   
    static void DoForm11(TFormLogicF1* FormLogicF1);
    // достраивает форму для 12 типа    
    static void DoForm12(TFormLogicF1* FormLogicF1);
     // расширяем строки заголовка и подвала   
    static void strRashirit(TFormLogicF1* FormLogicF1);
    // проверка наличия столбца - месячный, факт, число    
    static int IsMesFact(TFormLogicF1* FormLogicF1);
        // считывает и кладет исходные данные - только уточненно плановые в план-график финансирования
    static void GetSetItems(TFormLogicF1* FormLogicF1, int InitKolCol, int NColMes);
        // считывает и кладет исходные данные - только новые уточнения к отправленному плану-графику финансирования
    static void GetSetItems13(TInfoFormLogic* InfoFormLogic, TFormLogicF1* FormLogicF1, int InitKolCol, int NColMes);
    // считывает и кладет суммируя все данные плана-графика финансирования    
    static void GetSetItems14(TFormLogicF1* FormLogicF1, int InitKolCol, int NColMes);
     // считываем исходные данные кладем в форму и формируем ValueData   
    static void GetSetItems11(TFormLogicF1* FormLogicF1, int InitKolCol, int NColMes);
        // считываем исходные данные кладем в форму и формируем ValueData
    static void GetSetItems12(TFormLogicF1* FormLogicF1, int InitKolCol, int NColMes);
    // перенуморавка в списке-увеличение на 1    
    static void Perenumerovati(TFormLogicF1* FormLogicF1, int DblRow, int nMaxRow, int nBegRow, int toCol);
        // сформировать список PlanId
    static std::string GetPlanIdSets(TFormLogicF1* FormLogicF1, int NColMes);
        // пересчитать за диапазон дней
    static void execSummaDiapazon(TFormLogicF1* FormLogicF1);
};



#endif // FORMGF_LOGIC_H
