
#ifndef DB_ACCESSALBUM_H
#define DB_ACCESSALBUM_H

#include <iostream> 
#include "DB_Common.h"

class TDB_AccessAlbum {
public:
    // Получить accessalbum_id для указанных album_id, plan_id, god
    static int GetIdAccessAlbum(int album_id, int plan_id);
    // Прочитать записи для альбома
    static void GetAccessAlbum(TQueryUni* qIn, int album_id);
    static void GetAccessAlbumById(TQueryUni* qIn, int accessalbum_id);
    // Прочитать записи для альбома с правами текущего пользователя
    static void GetAccessAlbumForUser(TQueryUni* qIn, int album_id, int user_id);
    static void GetAccessBlockAlbumForUser(TQueryUni* qIn, int album_id);
    // Добавить запись
    static int AddAccessAlbum(int album_id, int plan_id, int god, TDateTime date_create);
    // Удалить запись
    static void DelAccessAlbum(int accessalbum_id);
    // Удалить записи для альбома
    static void DelAccessAlbumAll(int album_id);
};

#endif // DB_ACCESSALBUM_H
