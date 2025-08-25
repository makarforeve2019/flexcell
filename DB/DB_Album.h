
#ifndef DB_AlbumPAS_H
#define DB_AlbumPAS_H

#include "DB_Common.h"

class TDB_Album {
public:
    // получает всю информацию про альбом album_id
    static void GetAlbumInfo(int album_id, int& album_vid_func, int& album_isbp,
        int& readOnly, std::string& album_obozn, int& plan_id_fiks, int& vid_deistv,
        int& album_typ, int& vid_deistv_Versij, int& YearFixed);
    static void GetAlbumInfo2(int album_id, int& isUpdDateSave);
    static std::string getAlbumName(int album_id);
    static std::string GetDinIzmIds(int album_id);
    static int GetVid_deistv(int album_id);
    // получает всю информацию про альбом album_id и периодичность подчиненных форм
    static void GetAlbumInfoYQM(int album_id, int& album_vid_func, int& album_isbp, int& readOnly,
        std::string& album_obozn, int& plan_id_fiks, int& vid_deistv, int& album_typ,
        int& isYearQuartMonth, int& YearFixed, std::string& JSONFiltr);
    // получает информацию про актуальные по времени формы альбом - следует ли задавать дату за период
    static bool GetAlbumZaPeriod(int album_id, TDateTime byDate);
    static bool GetAlbumZaPeriod(TQueryUni *qIn, int album_id, boost::posix_time::ptime& byDate);
    // возвращает id последней добавленной записи
    static int GetId();
    // добавить разделитель
    static int AddRasdel(int userctgr_id, int owner_id, int album_vid_func, unsigned char album_isbp);
    // удалить разделитель
    static void DelRasdel(int album_id);
    // добавить группу
    static int AddGrp(std::string album_obozn, int userctgr_id, int owner_id, int album_vid_func, unsigned char album_isbp,
        int begGod, int endGod);
    // добавить блок действий
    static int AddBlockDeistv(std::string album_obozn, int userctgr_id, int owner_id, int album_vid_func, unsigned char album_isbp,
        int begGod, int endGod);
    // добавить действие
    static int AddDeistv(std::string album_obozn, int userctgr_id, int owner_id,
        int vid_deistv, int vid_deistv_Versij, int album_vid_func, unsigned char album_isbp,
        int begGod, int endGod, std::string subNm);
    // добавить действие с параметром
    static int AddDeistvParam(std::string album_obozn, int userctgr_id, int owner_id,
        int vid_deistv, int vid_deistv_Versij, int album_vid_func, unsigned char album_isbp,
        int begGod, int endGod, std::string subNm, int userctgr_idParam, int album_idParam);
    // удалить группу
    static bool DelGrp(int album_id);
    // добавить альбом
    static int AddAlbum(std::string album_obozn, int userctgr_id, int owner_id,
        int album_vid_func, unsigned char album_isbp, int plan_id, int readonly, int begGod,
        int endGod, int plan_id_fiks, int vid_deistv);
    // изменить альбом
    static void UpdAlbum(int album_id, std::string album_obozn, int album_vid_func, unsigned char album_isbp, int readonly,
        int begGod, int endGod, int plan_id_fiks, int vid_deistv);
    // изменить действе с параметром
    static void UpdDeistvParam(int album_id, std::string album_obozn, int vid_deistv,
        int begGod, int endGod, int userctgr_idParam, int album_idParam, int vid_deistv_Versij);
    // изменить обозначение альбома или группы
    static void UpdItem(int album_id, std::string album_obozn, int begGod, int endGod);
    // исключение форму из альбома
    static void ExclForm(int album_id, int form_id);
    // исключение все формы из альбома
    static void ExclFormAll(int album_id);
    // изменить действие
    static int UpdDeistv(int album_id, std::string album_obozn, int vid_deistv, int vid_deistv_Versij,
        int begGod, int endGod, std::string subNm);
    // изменить владельца альбома или группы
    static void UpdOwnerId(int userctgr_id, int album_id, int owner_id);
    // изменить пояснение к альбому
    static void UpdPojsnenie(int album_id, std::string Pojsnenie);
    // прочитать пояснение к альбому
    static std::string GetPojsnenie(int album_id);
    // поменять порядковыми номерами
    static void UpdOrders(int album_id_1, int album_id_2);
    // сделать или отменить главную форму
    static void MainForm(int album_id, int form_id);
    // данную форму в альбоме сделать вспомогательной или отменить
    static void VspomogForm(int album_id, int form_id);
    // данную форму в альбоме сделать ReadOnly или отменить
    static void ReadOnlyForm(int album_id, int form_id);
    // удалить альбом
    static bool DelAlbum(int album_id);
    // удалить все альбомы польз категории userctgr_id
    static void DelAlbumAll(int userctgr_id);
    // получает максимальный порядковый номер
    static int GetMaxOrder (int owner_id, int userctgr_id);
    // получает всю информацию про элемент по album_id
    static bool GetItem (TQueryUni* qIn, int album_id);
    //функция проверки уникальности имени при добавлении
    static bool UnikName(std::string album_obozn, int owner_id, int album_vid_func, int userctgr_id);
    //функция проверки уникальности имени при добавлении
    static bool UnikName0(std::string album_obozn, int owner_id, int userctgr_id);
    //функция проверки уникальности имени при изменении
    static bool UnikName_Izm(std::string album_obozn, int owner_id, int album_vid_func, int userctgr_id, int album_id);
    //функция проверки уникальности имени при изменении
    static bool UnikName_Izm0(std::string album_obozn, int owner_id, int userctgr_id, int album_id);
    // выбрать все подчиненные элементы
    static void GetSubItemAll(TQueryUni* qIn, int userctgr_id, int owner_id);
    // выбрать все подчиненные элементы
    static void GetSubItemAll_1(TQueryUni* qIn, int userctgr_id, int owner_id);
    // выбрать все формы альбома
    static void GetAlbForms(TQueryUni* qIn, int album_id);
    // выбрать конкретную форму альбома
    static void GetAlbForm(TQueryUni* qIn, int album_id, int form_id);
    // получить form_order формы альбома
    static int GetAlbFormOrder(int album_id, int form_id);
    // изменить nModulOfMetods
    static void Upd_nModulOfMetods(int album_id, int form_id, int nModulOfMetods);
    // добавить форму к альбому
    static void AddAlbForm(int album_id, int form_id);
    // поменять номерами формы
    static void UpdOrderForms(int album_id, int form_id_1, int form_id_2);
    // поменять категорию альбома и его форм
    static void UpdUserCtgr(int album_id, int userctgr_id, bool UpdTypNr);
    // преобразование id при новом подходе
    static bool NewToOld(int new_id, int& old_id);
    // есть ли в формах альбома (в описании столбца) ссылка на Выбор принятой редакции Плана
    static bool isCol_SelectPrinjtPlans(int album_id);
    static void UpdUstGod(int album_id, int is_set_god, int god_plus, int god_ravno);
    static void UpdPlanId(int album_id, int plan_id);
    // изменить порядковый номер
    static void UpdOrderN(int album_id, int order_nr);
    // изменить isUpdDateSave
    static void UpdIsUpdDateSave(int album_id, int isUpdDateSave);
    //Получить информацию о фильтре
    static std::string getOpenBeginSost(int album_id);
    //Обновить информацию о фильтре
    static void updOpenBeginSost(int album_id, std::string JSONString);
    //Получить информацию о фильтре и id связанного альбома
    static bool getOpenbeginsostAll(int album_id, std::string& JSONString, int& toAlbum_id);
    //Получить фиксированный год, если такой задан
    static bool getGod_ravno(int album_id, int& god_ravno);
    // найти в указанной пользовательской категории альбом ГГГГ: План мероприятий
    static bool findAlbumPlMerpopr(int userctgr_id, std::string albumName,
      int& album_idPlMerpopr);
};

#endif
