//
// Created by Alex on 15.04.2024.
//

#ifndef MWSRESTAPI_RS_ALBUM_H
#define MWSRESTAPI_RS_ALBUM_H
#include <string>
#include <boost/date_time/posix_time/ptime.hpp>

class Album {
public:
    //  Получение параметров альбом. Необходимо для окна “Выбор периода и субъекта планирования”
    static std::string GetAlbumInfoYQM(int album_id);
    //  Получение списка форм по альбому
    static std::string GetAlbForms(int album_id);

    static std::string GetAlbumZaPeriod(int album_id, int byDate);

    static std::string GetAccessAlbumForUser(int context_id, int album_id);

    static std::string GetAccessBlockAlbumForUser(int context_id, int album_id);
};


#endif //MWSRESTAPI_RS_ALBUM_H
