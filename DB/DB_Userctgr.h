#ifndef DB_USERCTGR_H
#define DB_USERCTGR_H

#include <string> // For std::string
#include "DB_Common.h" // Include DB_Common header file
#include "../system/ClassesUtils.h"

class TDB_Userctgr {
public:
    // вернуть информацию категории по user_id и userctgr_obozn
    static int GetUserctgrUserAndObozn(const std::string& userctgr_obozn, int user_id, std::string& dostupn_funcii, int& isNoNasledMenu, int& MenuVariant);
        // вернуть информацию о Первой категории для пользователя user_id
    static int GetUserctgrUserN1(int user_id, std::string& dostupn_funcii, std::string& userctgr_obozn, int& isNoNasledMenu, int& MenuVariant);
    // вернуть информацию о пользовательской пользователя user_id
    static void GetUserCtgrInfo(int userctgr_id, std::string& dostupn_funcii, std::string& userctgr_obozn, int& isNoNasledMenu, int& MenuVariant);
        //Вернуть обозначение польз. катиегории
    static std::string GetUserCtgrObozn(int userctgr_id);
        // заполнить TStrings - все категорий для пользователя user_id - сортировано по наименованию категории
    static int LoadUserctgrUser_All_sort(TStringList* Items, int user_id);
    // чтение всех категорий для пользователя user_id - сортировано по наименованию категории
    static void GetUserctgrUser_All_sort(TQueryUni* qIn, int user_id);
    // чтение всех категорий для пользователя user_id - сортировано по году end_date
    static void GetUserctgrUser_All_sort_end_date(TQueryUni* qIn, int user_id);
    // чтение всех альбомов, использующих категорию userctgr_id, но с ограничением на год
    static void GetAlbumUserctgr_All_byGog(TQueryUni* qIn, int userctgr_id, int God);
    // чтение всех альбомов, использующих категорию userctgr_id
    static void GetAlbumUserctgr_All(TQueryUni* qIn, int userctgr_id);
    // возвращает id последней добавленной записи
    static int GetId();
    // чтение всех пользовательских категорий
    static void GetUserctgr_All(TQueryUni* qIn);

    static void GetUserctgr_Allabstr(TQueryUni* qIn, int abstract_ctgr);
     // чтение всех категорий для всех пользователей - сортировано по категории или пользователю
    // в зависимости от параметра sortByCtgr
    static void GetUserCtgrsForUsers(TQueryUni* qIn, bool sortByCtgr, int abstractCtgr);
    // чтение всех категорий для пользователя user_id
    static void GetUserctgrUser_All(TQueryUni* qIn, int user_id);
    // чтение пользовательского категория по userctgr_id
    static bool GetUserctgr(TQueryUni* qIn, int userctgr_id);
    // добавление пользовательского категория возвращает id новой записи
    static int AddUserctgr(const std::string userctgr_obozn, const std::string comment, const std::string dostupn_funcii, int isNoNasledMenu, int MenuVariant, int beg_date, int end_date, int abstract_ctgr);
    // изменение записи для userctgr_id
    static void UpdUserctgr(int userctgr_id, const std::string userctgr_obozn, const std::string comment, const std::string dostupn_funcii, int isNoNasledMenu, int MenuVariant, int beg_date, int end_date, int abstract_ctgr);
    // изменение только ReadOnly для user_id + userctgr_id
    static void UpdUserUserctgrRO(int user_id, int userctgr_id, int ReadOnly);
    // чтение свойства только ReadOnly user_id + userctgr_id
    static int GetUserUserctgrRO(int user_id, int userctgr_id);
    // чтение свойства isNoNasledMenu из категории userctgr_id
    static int GetIsNoNasledMenu(int userctgr_id);
    // чтение свойства dostupn_funcii из категории userctgr_id
    static std::string GetDostupn_funcii(int userctgr_id, int& isNoNasledMenu);
    //функция проверки уникальности имени при добавлении
    static bool UnikName(const std::string userctgr_obozn);
    //функция проверки уникальности имени при изменении
    static bool UnikName_Izm(const std::string userctgr_obozn, int userctgr_id);
    // чтение всех пользователей, использующих категорию userctgr_id
    static void GetUserUserctgr_All(TQueryUni* qIn, int userctgr_id);
    // удаление пользовательской категорий
    static void DelUserctgr(int userctgr_id);
    // удаление всех категорий для пользователя user_id
    static void DelUserctgrUser_All(int user_id);
    // удаление категории userctgr_id для всех пользователя
    static void DelUserctgrUser_AllUsers(int userctgr_id);
    // добавление пользовательского категория для пользователя user_id
    static void AddUserctgrUser(int user_id, int userctgr_id, int order_nr);
    // изменение только ReadOnly для user_id + userctgr_id
    static void UpdOrder_nr(int user_id, int userctgr_id, int order_nr);
    // чтение свойства только ReadOnly user_id + userctgr_id
    static void UpdUserUserctgrIsBlock(int user_id, int userctgr_id, int is_block);
    // изменение только projected для user_id + userctgr_id
    static int GetUserUserctgrIsBlock(int user_id, int userctgr_id);
    // чтение свойства только projected user_id + userctgr_id
    static void UpdUserUserctgrProjected(int user_id, int userctgr_id, int Projected);
    // чтение свойства только projected user_id + userctgr_id
    static int GetUserUserctgrProjected(int user_id, int userctgr_id);
    // выделить N-й байт справа (нумерация с 1)
    static int getUserCtgrId_N(int isNoNasledMenu, int N);
    // положить info в N-й байт справа и вернуть (нумерация с 1)
    static int setUserCtgrId_N(int info, int isNoNasledMenu, int N);
};


#endif // DB_USERCTGR_H
