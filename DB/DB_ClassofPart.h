#ifndef DB_CLASSOFPART_H
#define DB_CLASSOFPART_H


#include "DB_Common.h"

class TDB_ClassofPart {
public:
    // получает все контейнеры для класса class_id
    static void GetClassConteiners(TQueryUni* qIn, int class_id);
    // получить первый контейнер для класса class_id
    static int GetClassConteiner1(int class_id);
    // получить контейнер для класса class_id, но не класс, а часть
    static int GetCastConteiner1(int class_id);
    // получает все элементы из таблицы Class_ofpart
    static void GetAllItems(TQueryUni* qIn);
    // получает все элементы для контейнера conteiner_id
    static void GetSubElems(TQueryUni* qIn, int conteiner_id);
    // получает количество элементов для контейнера conteiner_id и имя первого элемента
    static int GetSubElems1(int conteiner_id, std::string& name1); // overload
    // получает первый элемент для контейнера conteiner_id
    static int GetSubElems1(int conteiner_id); // overload
    // получает все возможные контейнеры
    static void GetConteinerAll(TQueryUni* qIn);
    // вставляет новый элемент
    static void AddElem(int conteiner_id, int class_id);
    // проверяет, есть ли такой элемент
    static bool IsElem(int conteiner_id, int class_id);
    // проверяет, используется ли где-то класс class_id
    static bool IsClass(int class_id);
    // удаляет все элементы для класса class_id
    static void DelClassElems(int class_id);
    // удаляет один элемент
    static void DelClassElem(int conteiner_id, int class_id);
    // читает контейнер класса, колторый сам класс
    static bool getConteinerClassName(int CastId, std::string& aConteinerClassName, int& aConteinerClassId);
};

#endif // DB_CLASSOFPART_H
