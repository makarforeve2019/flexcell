#include "Form2find.h"



TStringList StmntList;
void TForm2find::createStmntList()
{
    //StmntList.sorted = true;
  //*********************************************  class 1
  StmntList.AddObject("ПРОИЗВСТОЛБ", 1);
  //*********************************************  class 4
  StmntList.AddObject("РАСЧСЧЕТ", 4);
  StmntList.AddObject("ИНН", 4);
  StmntList.AddObject("ЮрАдрес", 4);
  //*********************************************  class 5
  StmntList.AddObject("БДЖПОЛУЧТЛЬ", 5);
  StmntList.AddObject("БДЖПОЛУЧТЛЬ_нет()", 5);
  //*********************************************  class 6
  StmntList.AddObject("РАСЧСЧЕТ2", 6);
  StmntList.AddObject("ЛицевойСчетТБ", 6);
  //*********************************************  class 7
  StmntList.AddObject("НАИМПОКАЗ", 7);
  StmntList.AddObject("НАИМЦелСт", 7);
  StmntList.AddObject("КОДБК", 7);
  StmntList.AddObject("КОДБК1", 7);
  StmntList.AddObject("КОДБКРздл", 7);
  StmntList.AddObject("КОДБКРздл1", 7);
  StmntList.AddObject("КОДБКЦелСт", 7);
  StmntList.AddObject("КОДБКЦелСт1", 7);
  StmntList.AddObject("КОДБКВидРасх", 7);
  StmntList.AddObject("КОДБКЭкКл", 7);
  StmntList.AddObject("НомерМрпр", 7);
  StmntList.AddObject("КБК-Дгвр:Кол-во", 7);
  StmntList.AddObject("КБК-Дгвр:Кол-воБО", 7);
  StmntList.AddObject("Владелец", 7);
  StmntList.AddObject("forUnikIdentif", 7);
  StmntList.AddObject("ПолныйНомер", 7);
  StmntList.AddObject("ИмяИстФин", 7);
  StmntList.AddObject("ДатаСоздОбъекта", 7);
  StmntList.AddObject("NamePlanВстроке", 7);
  StmntList.AddObject("ГодВстроке", 7);
  StmntList.AddObject("OKTMOPlanВстроке", 7);
  StmntList.AddObject("ОткрытьФормуПодчинОб", 7);
  StmntList.AddObject("ОткрытьФормуТекущОб", 7);
  StmntList.AddObject("TreeImgShow/Hide1", 7);
  StmntList.AddObject("TreeImgShow/HideNext", 7);
  //*********************************************  class 8
  StmntList.AddObject("ЕДИЗМ", 8);
  //*********************************************  class 9
  StmntList.AddObject("ЕДИЗМНВ", 9);
  //*********************************************  class 10
  StmntList.AddObject("ЕДИЗМобщНП", 10);
  //*********************************************  class 11
  StmntList.AddObject("ФОРМУЛАF1", 11);
  //*********************************************  class 12
  StmntList.AddObject("ЗНАЧЕНИЕСВ", 12);
  StmntList.AddObject("НАИМПОКАЗальтерн", 12);
  StmntList.AddObject("ЗНАЧЕНИЕСВ_ДООПРЕД", 12);
  //--StmntList.AddObject("ЗНАЧЕНИЕНВ", 12);
  StmntList.AddObject("ИСТЗНАЧВВОДПК", 12);
  StmntList.AddObject("ХарактСВобъекта", 12);
  StmntList.AddObject("КонкатСтрокСВобъек", 12);
  //--StmntList.AddObject("ПРОЦЕНТСВ", 12);
  //--StmntList.AddObject("ПРОЦЕНТНВ", 12);
  //--StmntList.AddObject("ОТКЛОНЕНИЕСВ", 12);
  //--StmntList.AddObject("ОТКЛОНЕНИЕНВ", 12);
  StmntList.AddObject("ЗНАЧЕНИЕСВНИ", 12);
  StmntList.AddObject("ЗНАЧЕНИЕНВНИ", 12);
  StmntList.AddObject("ПРОЦЕНТСВНИ", 12);
  StmntList.AddObject("ПРОЦЕНТНВНИ", 12);
  StmntList.AddObject("КБК-Дгвр:Сумма№БО", 12); //{"ОТКЛОНЕНИЕСВНИ"}
  StmntList.AddObject("КБК-Дгвр:СуммаБО", 12); //{"ОТКЛОНЕНИЕНВНИ"}
  StmntList.AddObject("КБК-Дгвр:Сумма№БОоз", 12); //{"ЗНАЧЕНИЕНВИР"}
  StmntList.AddObject("КБК-Дгвр:СуммаКЗ", 12);
  StmntList.AddObject("ОТКЛОНЕНИЕСВНачКВ", 12);
  StmntList.AddObject("ОТКЛОНЕНИЕНВНачКВ", 12);
  StmntList.AddObject("ЗНАЧЕНИЕСВИР", 12);
  StmntList.AddObject("ЗНАЧЕНИЕНВИР", 12);
  StmntList.AddObject("УТОЧНЕНИЕСВ", 12);
  StmntList.AddObject("УТОЧНЕНИЕСВНИ", 12);
  StmntList.AddObject("УТОЧНЕНИЕСВПосле", 12);
  StmntList.AddObject("Мрпр-Дгвр:Сумма№БО", 12);     //{"УТОЧНЕНИЕНВ"}
  StmntList.AddObject("Мрпр-Дгвр:СуммаБО", 12);        //{"УТОЧНЕНИЕНВНИ"}
  StmntList.AddObject("ФактКвДату", 12);        //{"УТОЧНЕНИЕСВдф"}
  StmntList.AddObject("СуммаПлГрНИВМесяце", 12);        //{"УТОЧНЕНИЕСВНИдф"}
  StmntList.AddObject("ПлнОСТАТОК", 12);
  StmntList.AddObject("ФктОСТАТОК", 12);
  StmntList.AddObject("ПлнОСТАТОКНИ", 12);
  StmntList.AddObject("ФактВсегоВНачале", 12);
  StmntList.AddObject("ПоступлВсегоВНачале", 12);
  StmntList.AddObject("УточПланВклДату", 12);
  StmntList.AddObject("СуммаПриход", 12);
  StmntList.AddObject("СуммаВозврат", 12);
  StmntList.AddObject("СуммаПлГрНИВНачале", 12);
  StmntList.AddObject("СуммаПлГрНИГод", 12);
  StmntList.AddObject("ПДГ_УТ_КассПл", 12);
  StmntList.AddObject("УТВ_УТ_КассПл", 12);
  StmntList.AddObject("Экономия/Резерв", 12);
  StmntList.AddObject("отчетCO_RP_015", 12);
  StmntList.AddObject("СведенияОбЭкономии", 12);
  StmntList.AddObject("КонкатСтрок", 12); 
  StmntList.AddObject("СВОДпоСУБЪвАльб", 12);
  StmntList.AddObject("СВОДпоСУБЪвА/n", 12);

  //*********************************************  class 13
  //--StmntList.AddObject("ЗНАЧЕНИЕНВ", 13);
  StmntList.AddObject("ПРОЦЕНТНВ", 13);
  StmntList.AddObject("ЗНАЧЕНИЕНВНИ", 13);
  StmntList.AddObject("ПРОЦЕНТНВНИ", 13);
  //--StmntList.AddObject("ОТКЛОНЕНИЕНВНИ", 13);
  StmntList.AddObject("ОТКЛОНЕНИЕНВНачКВ", 13);
  StmntList.AddObject("ЗНАЧЕНИЕНВИР", 13);
  //*********************************************  class 14
  StmntList.AddObject("Примечание", 14);
  StmntList.AddObject("Ведомство", 14);
  //*********************************************  class 15
  StmntList.AddObject("ЗНАЧЕНИЕобщНП", 15);
  StmntList.AddObject("ЗНАЧЕНИЕобщНПИР", 15);
  StmntList.AddObject("СВОДобщНПпоСУБЪвАльб", 15);
  StmntList.AddObject("СВОДобщНПпоСУБЪвА/n", 15);
  //*********************************************  class 16
  StmntList.AddObject("СистХарОбъекта", 16);
  StmntList.AddObject("СистХарПоказтля", 16);
  //*********************************************  class 17
  StmntList.AddObject("ВводЗначения", 17);
  //*********************************************  class 18
  StmntList.AddObject("ОперПрисвоить", 18);
}

int TForm2find::findByStmnt(std::string aStmnt) {
    int Index;
    if (StmntList.Count() == 0)
        TForm2find::createStmntList();
    if (!StmntList.Find(aStmnt, Index)) {
        return 0; // Или другое значение, указывающее на ошибку
    }

    const auto& obj = StmntList.Objects.Get(Index);
    if (std::holds_alternative<std::shared_ptr<int>>(obj)) {
        auto ptr = std::get<std::shared_ptr<int>>(obj);
        if (ptr) {
            return *ptr; // Возвращаем значение, на которое указывает shared_ptr
        } else {
            std::cerr << "Pointer to int is null." << std::endl;
            return 0; // Или другое значение, указывающее на ошибку
        }
    } else {
        std::cerr << "Expected shared_ptr<int>, found another type." << std::endl;
        return 0; // Или другое значение, указывающее на ошибку
    }
}
