#ifndef FormEndH
#define FormEndH

#include <json.hpp>
#include "unF1toDE.h"
#include "FormDopInp.h"
#include "unFormLogicF1.h"
#include "FormTypes.h"

using namespace std;
using json = nlohmann::json;

class TFormEnd {
public:
    // переносим наименование в другую ячейку
    // а также суррогатное видимое имя заменяется на ... (троеточие)
    static void SetNameNewCol(int FormId, TFormLogicF1 * FormLogicF1, int BegRow, int EndRow);
    // делает столбец с наименованием невидимым
    static void SetNameHidden(TFormLogicF1 * FormLogicF1, int BegRow);
    // делаем невидимыми строки, не соответствующие второму фильтру
    // так же обрабатывается оператор ПОЛОЖИТЬ
    // так же обрабатывается окраска Locked ячеек
    static void SetHiddenByFilter2(TFormLogicF1 * FormLogicF1, int BegRow, int EndRow);
    // обработка условного форматирования
    static void doCondFormatir(TFormLogicF1 * FormLogicF1, int aRow, int aCol, bool isEdit);
    // объединяем ячейки
    static void SetAlignmentObjedinit(int form_id, TFormLogicF1* FormLogicF1, int BegRow, int EndRow);
    // вычислить итоги по разделу за месяц и в том числе
    static void doItogoZaMesjc_VTomCisle(TFormLogicF1* FormLogicF1);
    // подсчет итого по разделу за месяц
    static void ItogoZaMesjc(TFormLogicF1* FormLogicF1, int nCol);
    // подсчет в том числе за месяц
    static void VTomCisle(TFormLogicF1* FormLogicF1, int nRow, int nCol);
    // дублирует итого по разделу в Разделе
    static void ExecIfDublirItogo(TFormLogicF1* FormLogicF1);
    // Осуществляет обработку оператора OdinakovZnac
    // задает: как обработать соседние ячейки с одинаковыми значениями
    static void execOdinakovZnac(TFormLogicF1* FormLogicF1);
    // обработка строкового оператора ::УдалитьСкрытые2 в конце формы
    static void DeleteHiddenRows2(TFormLogicF1* FormLogicF1, int aRowOper, bool isHiddenOnlyFiltr = false);
    // обработка строкового оператора ::УдалитьСкрытыеФильтром в конце формы
    static void DeleteHiddenRowsByFiltr(TFormLogicF1* FormLogicF1, int aRowOper);
    // обработка строкового оператора ::УдалитьСкрытые в конце формы
    static void DeleteHiddenRows(TFormLogicF1* FormLogicF1, int aRowOper);
    // раскрасить шарку формы в серый цвет
    static void doShapkaColor(TFormLogicF1* FormLogicF1);
    // чередовать раскрасить строк таблицы в разные цвета
    static void doLinesColor(TFormLogicF1* FormLogicF1);
};
#endif