
#ifndef MWSRESTAPI_DB_COMMON_H
#define MWSRESTAPI_DB_COMMON_H


#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <optional>
#include <boost/date_time.hpp>
#include "../system/ClassesUtils.h"
#include "json.hpp"
#include "TracerManager.h"

using json = nlohmann::json;

class TField {
public:
    PGresult *result;
    int row;
    int col;

    TField(PGresult *res, int r, int c) : result(res), row(r), col(c) {}

    int AsInteger() const {
        if (IsNull()) {
            throw std::runtime_error("Field is NULL");
        }
        char *value = PQgetvalue(result, row, col);
        return std::stoi(value);
    }

    double AsFloat() const {
        if (IsNull()) {
            throw std::runtime_error("Field is NULL");
        }
        char *value = PQgetvalue(result, row, col);
        return std::stod(value);
    }

    bool IsNull() const {
        return PQgetisnull(result, row, col) != 0;
    }
};

class TFields {
public:
    std::vector<TField> fields;

    TFields(PGresult *res) {
        int nFields = PQnfields(res);
        for (int i = 0; i < nFields; ++i) {
            fields.emplace_back(res, 0, i); // Предполагаем, что строки всегда начинают с 0
        }
    }

    TField &operator[](int index) {
        if (index < 0 || index >= fields.size()) {
            throw std::out_of_range("Field index out of range");
        }
        return fields[index];
    }

    int size() const {
        return fields.size();
    }
};


class SQLManager {
public:
    void Clear() {
        if (!parts.empty()) {
            parts.clear();
        }
    }

    void Add(const std::string &part) {
        parts.push_back(part);
    }

    std::string GetQuery() const {
        std::string fullQuery;
        for (const auto &part: parts) {
            if (!fullQuery.empty()) fullQuery += " ";
            fullQuery += part;
        }
        return fullQuery;
    }

    // Получение части запроса по индексу с проверкой
    std::string GetPart(size_t index) const {
        if (index >= parts.size()) {
            throw std::out_of_range("Index out of range");
        }
        return parts[index];
    }

    // Установка части запроса по индексу
    void SetPart(size_t index, const std::string &part) {
        if (index >= parts.size()) {
            parts.resize(index + 1); // Обеспечиваем наличие места под индекс
        }
        parts[index] = part;
    }

    // Дополнительный метод для получения количества частей
    size_t GetPartsCount() const {
        return parts.size();
    }

private:
    std::vector<std::string> parts;
};


class TParamUni {
public:

    //определение прокси-класса для строки
    class StringProxy {
    private:
        TParamUni &parent;

    public:
        StringProxy(TParamUni &parent) : parent(parent) {}

        //перегрузка оператора присваивания для строки
        StringProxy &operator=(const std::string &value) {
            parent.SetValue(value);
            return *this;
        }
    };

    // Прокси-класс для int
    class IntegerProxy {
    private:
        TParamUni &parent;

    public:
        IntegerProxy(TParamUni &parent) : parent(parent) {}

        // Перегрузка оператора присваивания для int
        IntegerProxy &operator=(int value) {
            parent.SetValue(value);
            return *this;
        }

        // Метод для получения значения
        operator int() const {
            return parent.GetValueAsInt();
        }
    };

    class DateTimeProxy {
    private:
        TParamUni &parent;

    public:
        DateTimeProxy(TParamUni &parent) : parent(parent) {}

        DateTimeProxy &operator=(const TDateTime &value) {
            parent.SetValue(value.getDateTime());
            return *this;
        }
    };

    class NumericProxy {
    private:
        TParamUni &parent;

    public:
        NumericProxy(TParamUni &parent) : parent(parent) {}

        // Перегрузка оператора присваивания для int
        NumericProxy &operator=(int value) {
            parent.SetValue(value);
            return *this;
        }

        // Перегрузка оператора присваивания для double
        NumericProxy &operator=(double value) {
            parent.SetValue(value);
            return *this;
        }
    };


    std::string ParamName; //имя параметра
    std::string ParamType; //тип параметра
    std::variant<int, double, std::string, boost::posix_time::ptime> ParamValue; //значение параметра
    bool isClear = true;
    bool isCreate = false;

    TParamUni() = default;


    void SetValue(int value) {  //установка значения параметра типа int
        ParamValue = value;
        ParamType = "ftInteger";
        isClear = false;
    }

    void SetValue(double value) { //установка значения параметра типа double
        ParamValue = value;
        ParamType = "ftFloat";
        isClear = false;
    }

    void SetValue(const std::string &value) { //установка значения параметра типа строка
        ParamValue = value;
        ParamType = "ftString";
        isClear = false;
    }

    void SetValue(const TDateTime &value) {
        std::ostringstream ss;
        // Установка формата для даты и времени
        boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
        ss.imbue(std::locale(std::locale::classic(), facet)); // Применяем формат
        ss << value.getDateTime();  // Форматируем дату и время

        // Правильно добавляем одинарные кавычки вокруг сформированной строки даты и времени
        ParamValue = ss.str(); // Добавляем одинарные кавычки
        ParamType = "ftDateTime";
        isClear = false;
    }

    // Метод для получения значения параметра как int
    int GetValueAsInt() const {
        if (std::holds_alternative<int>(ParamValue)) {
            return std::get<int>(ParamValue);
        } else {
            throw std::runtime_error("Параметр не является типом int");
        }
    }


    //получение прокси для установки значения типа int
    IntegerProxy AsInteger() {
        return IntegerProxy(*this);
    }

    //получение прокси для установки значения типа строка
    StringProxy AsString() {
        return StringProxy(*this);
    }

    DateTimeProxy AsDateTime() {
        return DateTimeProxy(*this);
    }

    NumericProxy AsFloat() {
        return NumericProxy(*this);
    }
};


class TQueryUni {
public:
    int current_row = 0; //текущая строка
    int Tag = 0;
    PGresult *current_result = nullptr; //текущий результат запроса
    SQLManager *SQL;
    std::vector<TField> Fields;
    bool active = false;
    std::string db_name;  // переменная для хранения имени базы данных
    TQueryUni(PGconn *conn) : db_conn(conn) {
        SQL = new SQLManager();
//        // Получаем имя базы данных при создании объекта
//        PGresult *res = PQexec(db_conn, "SELECT current_database();");
//        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
//            db_name = PQgetvalue(res, 0, 0);  // сохраняем имя базы данных
//            //std::cout << "Connected to database: " << db_name << std::endl;
//        } else {
//            std::cerr << "Error getting database name: " << PQerrorMessage(db_conn) << std::endl;
//            TracerManager::Instance().AddSpanLog("DB_Common. db_conn initialize", {{"errordesr", "Error getting database name "}},
//                                                 TracerManager::LOG_LEVEL_ERROR);
//        }
//        PQclear(res);
    }

    ~TQueryUni() {
        if (current_result != nullptr) {
            PQclear(current_result);
            current_result = nullptr;
        }
        if (SQL != nullptr) {
            delete SQL;
            SQL = nullptr;
        }
    }

    void Free() {
        if (current_result != nullptr) {
            PQclear(current_result); // освобождаем память, выделенную для результата
            current_result = nullptr;
        }
    }

    void Close() {
        active = false;
//        Free();

    }

    bool Active() const {
        return active;
    }

    void Active(bool value) {
        if (value != active) {
            if (value) {
                Open();
            } else {
                Close();
            }
        }
    }

    class FieldProxy {
    private:
        PGresult *result;
        int row;
        std::string fieldName;


    public:
        FieldProxy(PGresult *result, int row, const std::string &fieldName)
                : result(result), row(row), fieldName(fieldName) {}

        FieldProxy *operator->() {
            return this;
        }

        char *AsBytea() const {
            int column_index = PQfnumber(result, fieldName.c_str());
            if (column_index == -1) {
                throw std::runtime_error("Поле не найдено: " + fieldName);
            }
            if (IsNull()) {
                return nullptr;  // Возвращаем пустую строку для NULL
            }
            char *value = PQgetvalue(result, row, column_index);
            return value;
        }

        int GetByteaSize() const {
            int column_index = PQfnumber(result, fieldName.c_str());
            if (column_index == -1) {
                throw std::runtime_error("Поле не найдено: " + fieldName);
            }
            if (IsNull()) {
                return 0;  // Возвращаем пустую строку для NULL
            }
            int value = PQgetlength(result, row, column_index);
            return value;
        }


        std::string AsString() const {
            int column_index = PQfnumber(result, fieldName.c_str());
            if (column_index == -1) {
                throw std::runtime_error("Поле не найдено: " + fieldName);
            }
            if (IsNull()) {
                return "";  // Возвращаем пустую строку для NULL
            }
            char *value = PQgetvalue(result, row, column_index);
            std::string ws = std::string(value);
            return ws;
        }


        int AsInteger() const {
            if (IsNull()) {
                return 0;  // Возвращаем 0 для NULL
            }
            try {
                std::string valueStr = AsString();
                return std::stoi(valueStr);
            } catch (const std::exception &e) {
                throw std::runtime_error("Ошибка преобразования к int: " + std::string(e.what()));
            }
        }


        double AsDouble() const {
            if (IsNull()) {
                return 0.0;  // Возвращаем 0.0 для NULL
            }
            try {
                std::string valueStr = AsString();
                return std::stod(valueStr);
            } catch (const std::exception &e) {
                throw std::runtime_error("Ошибка преобразования к double: " + std::string(e.what()));
            }
        }

        double AsFloat() const {
            if (IsNull()) {
                return 0.0;  // Возвращаем 0.0 для NULL
            }
            try {
                std::string valueStr = AsString();
                return StrToFloat(valueStr);
            } catch (const std::exception &e) {
                throw std::runtime_error("Ошибка преобразования к double: " + std::string(e.what()));
            }
        }

        TDateTime AsDateTime() const {
            if (IsNull()) {
//                throw std::runtime_error("Попытка прочитать NULL как дату/время: " + fieldName);
                return 0;
            }
            try {
                std::string valueStr = AsString();
                return TDateTime(valueStr);  // предполагается, что TDateTime может быть инициализирован из строки
            } catch (const std::exception &e) {
                throw std::runtime_error("Ошибка преобразования даты/времени: " + std::string(e.what()));
            }
        }


//        bool IsNull() const {
//            int column_index = PQfnumber(result, fieldName.c_str());
//            if (column_index == -1) {
//                throw std::runtime_error("Поле не найдено: " + fieldName);
//            }
//            return PQgetisnull(result, row, column_index) != 0;
//        }
        bool IsNull() const {
            int num_rows = PQntuples(result);

            // Если результат пустой или row выходит за пределы диапазона, считаем, что значение NULL
            if (num_rows == 0 || row >= num_rows) {
                return true; // Возвращаем true, так как результат пустой, что означает NULL
            }

            int column_index = PQfnumber(result, fieldName.c_str());
            if (column_index == -1) {
                throw std::runtime_error("Field not found: " + fieldName);
            }

            // Проверяем, является ли значение в ячейке NULL
            return PQgetisnull(result, row, column_index) != 0;
        }

    };


    FieldProxy FieldByName(const std::string &fieldName) const {
        if (!current_result) {
            throw std::runtime_error("Результирующий набор не был инициализирован.");
        }
        return FieldProxy(current_result, current_row, fieldName);
    }

    bool EoF() const { //проверка конца результата
        if (!current_result) {
            std::cerr << "Результирующий набор не был инициализирован." << std::endl;
            return true;
        }
        int wi = PQntuples(current_result);
        return current_row >= PQntuples(current_result); // исправлено сравнение
    }

    bool BoF() const { // проверка, находимся ли мы перед первой записью
        if (!current_result) {
            std::cerr << "Результирующий набор не был инициализирован." << std::endl;
            return true; // Если нет результата, считаем, что находимся в начале
        }
        return current_row <= 0; // Возвращаем true, если текущая строка меньше или равна 0
    }

    bool Next() {  // переход к следующей строке
        if (current_row < PQntuples(current_result)) {
            ++current_row;
            return true;
        }
        return false;
    }


    bool Prior() {  //переход к предыдущей строке
        if (current_row > 0) {
            --current_row;
            return true;
        }
        return false;
    }


    bool First() { //переход к первой строке
        if (PQntuples(current_result) > 0) {
            current_row = 0;
            return true;
        }
        return false;
    }

    bool Last() { //переход к последней строке
        int num_rows = PQntuples(current_result);
        if (num_rows > 0) {
            current_row = num_rows - 1;
            return true;
        }
        return false;
    }

    bool IsEmpty() const {
        if (current_result == nullptr) {
            return true; // Если результат не был инициализирован, считаем его пустым
        }
        return PQntuples(current_result) == 0; // Проверяем, есть ли строки в результате
    }

    int RecordCount() const {
        if (current_result == nullptr) {
            return 0; // Если результат не был инициализирован, возвращаем 0
        }
        return PQntuples(current_result); // Возвращаем количество строк в результате
    }


    TParamUni *ParamByName(const std::string &name) {  //получение параметра по имени
        for (auto &param: params) {
            if (param.ParamName == name) return &param;
        }
        params.emplace_back();
        params.back().ParamName = name;
        return &params.back();
    }

    PGresult* Open() {
        auto query = SQL->GetQuery();

        // Замена плейсхолдеров на значения параметров
        current_row = 0;
        for (const auto& param : params) {
            std::string placeholder = ":" + param.ParamName;
            std::string value = std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, double>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, std::string>) {
                    std::vector<char> buffer(2 * arg.length() + 1);
                    size_t escapedLength = PQescapeString(buffer.data(), arg.c_str(), arg.length());
                    std::string ws2(buffer.data(), escapedLength);
                    return "'" + ws2 + "'";
                } else if constexpr (std::is_same_v<T, boost::posix_time::ptime>)
                    return boost::posix_time::to_iso_extended_string(arg);
                else
                    return "";
            }, param.ParamValue);

            size_t start_pos = 0;
            while ((start_pos = query.find(placeholder, start_pos)) != std::string::npos) {
                query.replace(start_pos, placeholder.length(), value);
                start_pos += value.length();
            }
        }

        // Очистка предыдущего результата
        if (current_result != nullptr) {
            PQclear(current_result);
            current_result = nullptr;
        }

        PGresult* lastResult = nullptr;
        PGresult* tempResult = nullptr;

        try {
            // Выполнение основного запроса
            //auto begin = std::chrono::steady_clock::now();

            TracerManager::Instance().StartDBTrace(query);
            lastResult = PQexec(db_conn, query.c_str());
            TracerManager::Instance().EndDBTrace();
            //auto end = std::chrono::steady_clock::now();
            //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
            //std::cout << "Время(мс): " << elapsed_ms.count() <<  ";Запрос: " << query << std::endl;


            // Проверяем основной результат
            if (PQresultStatus(lastResult) != PGRES_TUPLES_OK && PQresultStatus(lastResult) != PGRES_COMMAND_OK) {
                // Логируем ошибку через TracerManager
                TracerManager::Instance().AddSpanLog(
                        "DB_Common. Open. Ошибка выполнения запроса",
                        {{"Ошибка:", PQerrorMessage(db_conn)},
                         {"Статус ошибки: ", PQresStatus(PQresultStatus(lastResult))},
                         {"Сообщение о статусе: ", PQresultErrorMessage(lastResult)},
                         {"  SQLSTATE: ", PQresultErrorField(lastResult, PG_DIAG_SQLSTATE)},
                         {"  Сообщение об ошибке: ", PQresultErrorField(lastResult, PG_DIAG_MESSAGE_PRIMARY)},
                         {"  Детали ошибки: ", PQresultErrorField(lastResult, PG_DIAG_MESSAGE_DETAIL)},
                         {"  Подсказка: ", PQresultErrorField(lastResult, PG_DIAG_MESSAGE_HINT)},
                         {"  Позиция ошибки: ", PQresultErrorField(lastResult, PG_DIAG_STATEMENT_POSITION)}},
                        TracerManager::LOG_LEVEL_ERROR);

                // Выводим ошибку в консоль
                std::cerr << "Ошибка выполнения SQL запроса:\n";
                std::cerr << "Ошибка: " << PQerrorMessage(db_conn) << '\n';
                std::cerr << "Статус ошибки: " << PQresStatus(PQresultStatus(lastResult)) << '\n';
                std::cerr << "Сообщение о статусе: " << PQresultErrorMessage(lastResult) << '\n';
                std::cerr << "SQLSTATE: " << PQresultErrorField(lastResult, PG_DIAG_SQLSTATE) << '\n';
                std::cerr << "Сообщение об ошибке: " << PQresultErrorField(lastResult, PG_DIAG_MESSAGE_PRIMARY) << '\n';
                std::cerr << "Детали ошибки: " << PQresultErrorField(lastResult, PG_DIAG_MESSAGE_DETAIL) << '\n';
                std::cerr << "Подсказка: " << PQresultErrorField(lastResult, PG_DIAG_MESSAGE_HINT) << '\n';
                std::cerr << "Позиция ошибки: " << PQresultErrorField(lastResult, PG_DIAG_STATEMENT_POSITION) << '\n';

                // Очищаем основной результат
                PQclear(lastResult);
                throw std::runtime_error("Ошибка выполнения SQL запроса");
            }

            // Обрабатываем все промежуточные результаты
            while ((tempResult = PQgetResult(db_conn)) != nullptr) {
                if (tempResult != lastResult) {
                    PQclear(tempResult);
                }
            }

            // Сохраняем последний результат
            current_result = lastResult;

            // Обрабатываем столбцы
            int nFields = PQnfields(current_result);
            Fields.clear();
            Fields.reserve(nFields);
            for (int i = 0; i < nFields; ++i) {
                Fields.emplace_back(current_result, 0, i);
            }

            active = true;
            return current_result;

        } catch (...) {
            // Очищаем все результаты в случае исключения
            if (lastResult) {
                PQclear(lastResult);
            }
            throw; // Пробрасываем исключение дальше
        }
    }







    PGresult *OpenBlob() { //выполнение запроса с параметрами
        auto query = SQL->GetQuery();
        //замена плейсхолдеров на значения параметров
        current_row = 0;
        for (const auto &param: params) {
            std::string placeholder = ":" + param.ParamName; //плейсхолдер в SQL запросе
            std::string value; //значение параметра в виде строки

            //получение строки из variant значения параметра
            value = std::visit([](auto &&arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>)
                    return std::to_string(arg); // Для int
                else if constexpr (std::is_same_v<T, double>)
                    return std::to_string(arg); // Для double
                else if constexpr (std::is_same_v<T, std::string>)
                    return "'" + arg + "'";
                else if constexpr (std::is_same_v<T, boost::posix_time::ptime>) // Для boost::posix_time::ptime
                    return boost::posix_time::to_iso_extended_string(arg); // Преобразование в строку
                else
                    return ""; //в случае неизвестного типа
            }, param.ParamValue);

            //поиск плейсхолдера в запросе и его замена
            size_t start_pos = 0;
            while ((start_pos = query.find(placeholder, start_pos)) != std::string::npos) {
                query.replace(start_pos, placeholder.length(), value);
                start_pos += value.length(); //продолжаем поиск со следующей позиции
            }
        }

        PGresult *res = PQexecParams(db_conn, query.c_str(), 0, NULL, NULL, NULL, NULL, 1);
        if (current_result != nullptr) {
            PQclear(current_result);
        }
        current_result = res;

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "Ошибка выполнения запроса: " << PQerrorMessage(db_conn) << std::endl;
            PQclear(res); //освобождаем память при ошибке
            current_result = nullptr;

            return nullptr; //возвращаем nullptr в случае ошибки
        }

        int nFields = PQnfields(current_result);
        Fields.reserve(nFields);
        for (int i = 0; i < nFields; ++i) {
            Fields.emplace_back(current_result, 0, i); // Заполняем поля
        }
        active = true;
        return res; //возвращаем результат выполнения запроса
    }

    bool ExecSQL() { // выполнение запроса без параметров
        auto query = SQL->GetQuery();

        // Сортируем параметры по длине имени в порядке убывания, чтобы сначала обрабатывать более длинные
        std::sort(params.begin(), params.end(), [](const TParamUni& a, const TParamUni& b) {
            return a.ParamName.size() > b.ParamName.size();
        });

        // Замена плейсхолдеров на значения параметров
        for (const auto& param : params) {
            std::string placeholder = ":" + param.ParamName;
            std::string value = std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    return std::to_string(arg); // Для int, без кавычек
                } else if constexpr (std::is_same_v<T, double>) {
                    return formatDouble(arg); // Для double, без кавычек
                } else if constexpr (std::is_same_v<T, std::string>) {
                    // Используем PQescapeString для экранирования строки
                    size_t length = arg.length();
                    char* buffer = new char[2 * length + 1];

                    size_t escapedLength = PQescapeString(buffer, arg.c_str(), length);
                    std::string escapedArg(buffer, escapedLength);
                    delete[] buffer;

                    return "'" + escapedArg + "'"; // Оборачиваем экранированную строку в одинарные кавычки
                } else {
                    return "";
                }
            }, param.ParamValue);

            size_t start_pos = 0;
            while ((start_pos = query.find(placeholder, start_pos)) != std::string::npos) {
                query.replace(start_pos, placeholder.length(), value);
                start_pos += value.length();
            }
        }

        // Замер времени выполнения запроса
//        auto start_time = std::chrono::high_resolution_clock::now();

        // Выполнение SQL-запроса
        //auto begin = std::chrono::steady_clock::now();
        TracerManager::Instance().StartDBTrace(query);
        PGresult* res = PQexec(db_conn, query.c_str());
        TracerManager::Instance().EndDBTrace();
        //auto end = std::chrono::steady_clock::now();
        //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        //std::cout << "Время(мс): " << elapsed_ms.count() <<  ";Запрос: " << query << std::endl;

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Ошибка выполнения запроса: " << PQerrorMessage(db_conn) << std::endl;
            PQclear(res); // освобождение памяти
            return false;
        }

        PQclear(res); // освобождение памяти, выделенной для результатов запроса
        return true; // возвращаем true, если запрос выполнен успешно
    }


    bool Locate(const std::string &columnName, int value) {
        if (!current_result) {
            std::cerr << "Результирующий набор не был инициализирован." << std::endl;
            return false;
        }

        int num_rows = PQntuples(current_result);
        int column_index = PQfnumber(current_result, columnName.c_str());
        if (column_index == -1) {
            std::cerr << "Столбец не найден: " << columnName << std::endl;
            return false;
        }

        for (int i = 0; i < num_rows; ++i) {
            const char *cell_value = PQgetvalue(current_result, i, column_index);
            if (cell_value && std::stoi(cell_value) == value) {
                current_row = i;
                return true;
            }
        }

        return false;
    }


    std::string getFieldType(Oid oid) {
        switch (oid) {
            case 23:   // INTEGER
                return "ftInteger";
            case 1043: // VARCHAR
                return "ftWideString";
            case 1114: // TIMESTAMP
                return "ftDateTime";
            case 21:   // SMALLINT
                return "ftSmallint";
            case 16:   // BOOLEAN
                return "ftBoolean";
            case 1082: // DATE
                return "ftDate";
            case 700:  // FLOAT4
                return "ftFloat";
            case 701:  // FLOAT8
                return "ftDouble";
            case 20:   // BIGINT
                return "ftLargeint";
            case 1700: // NUMERIC
                return "ftNumeric";
            case 1184: // TIMESTAMPTZ (timestamp with time zone)
                return "ftTimeStamp";
            case 19:   // NAME
                return "ftName"; // добавление нового типа данных "name"
            case 25:   // TEXT
                return "ftWideMemo"; // добавление нового типа данных "text"
            case 1042: // CHARACTER
                return "ftString"; // обработка типа данных CHARACTER
            default:
                return "Unknown";
        }
    }

    nlohmann::json openQueryGetJson(const std::string &query) {
        PGresult *res = PQexec(db_conn, query.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "SQL query failed: " << PQerrorMessage(db_conn) << std::endl;
            PQclear(res);
            return {};
        }

        nlohmann::json j;
        nlohmann::json fields_json = nlohmann::json::array();
        int nFields = PQnfields(res);

        for (int i = 0; i < nFields; i++) {
            fields_json.push_back({
                                          {"fieldName", PQfname(res, i)},
                                          {"fieldType", getFieldType(PQftype(res, i))}
                                  });
        }
        j["defFilds"] = fields_json;

        nlohmann::json records_json = nlohmann::json::array();
        int nRows = PQntuples(res);
        for (int i = 0; i < nRows; i++) {
            std::vector<nlohmann::json> record;
            for (int j = 0; j < nFields; j++) {
                if (PQgetisnull(res, i, j)) {
                    // Поле NULL, подставляем специальное значение в зависимости от типа поля
                    std::string fieldType = getFieldType(PQftype(res, j));
                    if (fieldType == "ftInteger") {
                        record.push_back(IntegerIsNull);
                    } else if (fieldType == "ftWideString") {
                        record.push_back(StringIsNull);
                    } else if (fieldType == "ftDateTime" || fieldType == "ftTimeStamp") {
                        record.push_back("30-12-1899 00:00:00.000");
                    } else if (fieldType == "ftFloat") {
                        record.push_back(NumberIsNull);
                    } else if (fieldType == "ftDouble") {
                        record.push_back(NumberIsNull);
                    } else if (fieldType == "ftLargeint") {
                        record.push_back(LargeintIsNull);
                    } else if (fieldType == "ftSmallint") {
                        record.push_back(IntegerIsNull);
                    } else if (fieldType == "ftDate") {
                        record.push_back("1899-12-30");
                    } else if (fieldType == "ftName") {
                        record.push_back("1899-12-30");
                    } else {
                        record.push_back(nullptr);
                    }

                } else {
                    // Поле не NULL, используем значение из базы
                    std::string fieldType = getFieldType(PQftype(res, j));
                    std::string value = PQgetvalue(res, i, j);
                    if (fieldType == "ftBoolean") {
                        // Сохраняем булевое значение как true/false
                        record.push_back(value == "t");
                    } else {
                        record.push_back(value);
                    }
                }
            }
            records_json.push_back(record);
        }
        j["records"] = records_json;

        PQclear(res);
        return j;
    }

    // Константы для значений NULL
    const double NumberIsNull = 1.1754943508222875080e-38;
    const std::string StringIsNull = "&#$&#$&#$";
    const int IntegerIsNull = -2147483648;
    const long long LargeintIsNull = -9223372036854775807LL - 1;

    std::vector<TParamUni> params;
private:
    PGconn *db_conn; //подключение к базе данных
    //список параметров
};

////////////////////////////////////////////////////////////////////////////
///////////////////////////////TFDStoredProc////////////////////////////////
////////////////////////////////////////////////////////////////////////////

enum FieldType {
    ftInteger, ftDateTime, ftString, ftFloat, ftDouble
};
enum ParamType {
    ptInput, ptOutput
};

class TFDStoredProc;

class TFDParam {
public:
    std::string name;
    FieldType type;
    ParamType paramType;
    std::variant<int, std::string, TDateTime, float, double> value;

    TFDParam(const std::string &paramName, FieldType paramType, ParamType ptType)
            : name(paramName), type(paramType), paramType(ptType) {}


    // Устанавливаем значение для int
    void SetValue(int val) {
        if (type != ftInteger) throw std::logic_error("Parameter type mismatch.");
        value = val;
    }

    void SetValue(const std::string &val) {
        if (type != ftString) throw std::logic_error("Parameter type mismatch.");
        value = val;
    }

    // Устанавливаем значение для float
    void SetValue(float val) {
        if (type != ftFloat) throw std::logic_error("Parameter type mismatch.");
        value = val;
    }

    // Устанавливаем значение для ftDouble
    void SetValue(double val) {
        if (type != ftDouble) throw std::logic_error("Parameter type mismatch.");
        value = val;
    }

    // Устанавливаем значение для TDateTime
    void SetValue(const TDateTime &val) {
        if (type != ftDateTime) throw std::logic_error("Parameter type mismatch.");
        value = val;
    }

    std::string &AsString() {
        try {
            return std::get<std::string>(value);
        } catch (const std::bad_variant_access &) {
            static std::string tmp;
            std::ostringstream oss;
            if (std::holds_alternative<int>(value)) {
                oss << std::get<int>(value);
            } else if (std::holds_alternative<float>(value)) {
                oss << std::get<float>(value);
            } else if (std::holds_alternative<double>(value)) {
                oss << std::get<double>(value);
            } else if (std::holds_alternative<TDateTime>(value)) {
                //
            } else {
                //throw std::runtime_error("Unsupported type for AsString");
            }
            tmp = oss.str();
            return tmp;
        }
    }

    // Получаем значение int
    int &AsInteger() {
        if (type != ftInteger || !std::holds_alternative<int>(value))
            throw std::logic_error("Parameter type mismatch or not set.");
        return std::get<int>(value);
    }

    // Получаем значение float
    float &AsFloat() {
        if (type != ftFloat || !std::holds_alternative<float>(value))
            throw std::logic_error("Parameter type mismatch or not set.");
        return std::get<float>(value);
    }

    // Получаем значение double
    double &AsDouble() {
        if (type != ftDouble || !std::holds_alternative<double>(value))
            throw std::logic_error("Parameter type mismatch or not set.");
        return std::get<double>(value);
    }


    // Получаем значение TDateTime
    TDateTime &AsDateTime() {
        if (type != ftDateTime || !std::holds_alternative<TDateTime>(value))
            throw std::logic_error("Parameter type mismatch or not set.");
        return std::get<TDateTime>(value);
    }
};


class TFDParams {
private:
    TFDStoredProc *owner;


public:
    std::vector<TFDParam> params;

    TFDParams(TFDStoredProc *ownerProc) : owner(ownerProc) {}

    TFDParam *CreateParam(FieldType type, const std::string &name, ParamType pType) {
        params.emplace_back(name, type, pType);
        return &params.back();
    }

    TFDParam *ParamByName(const std::string &paramName) {
        for (auto &param: params) {
            if (param.name == paramName) {
                return &param;
            }
        }
        throw std::runtime_error("Parameter not found: " + paramName);
    }

    size_t GetParamCount() const {
        return params.size();
    }

    const TFDParam &GetParam(size_t index) const {
        if (index >= params.size()) {
            throw std::out_of_range("Index out of range");
        }
        return params[index];
    }
};

class TFDStoredProc {
private:
    PGconn *conn;

public:
    std::unique_ptr<TFDParams> Params;

    TFDStoredProc(PGconn *dbConn) : conn(dbConn), Params(nullptr) {
        Params = std::make_unique<TFDParams>(this);
    }


    void SetStoredProcName(const std::string &name) {
        storedProcName = name;
    }

    TFDParam *ParamByName(const std::string &paramName) {
        return Params->ParamByName(paramName);  // Делегирование вызова метода Params
    }

    std::string getResultAsString(PGresult *result) {
        std::string data;
        if (PQresultStatus(result) == PGRES_TUPLES_OK) {
            int nrows = PQntuples(result);
            int ncols = PQnfields(result);

            for (int i = 0; i < nrows; ++i) {
                for (int j = 0; j < ncols; ++j) {
                    data += PQgetvalue(result, i, j);
                    if (j != ncols - 1) {
                        data += ", ";
                    }
                }
                data += "\n";
            }
        }
        return data;
    }

    void ExecProc() {
        std::string query = "SELECT " + storedProcName + "(";

        for (size_t i = 0; i < Params->GetParamCount(); ++i) {
            const TFDParam &param = Params->GetParam(i);

            if (param.paramType == ptInput) { // Обработка только входных параметров
                const TFDParam &param = Params->GetParam(i);
                if (i > 0) query += ", ";

                switch (param.type) {
                    case ftInteger:
                        query += std::to_string(std::get<int>(param.value));
                        break;
                    case ftFloat:
                        query += formatDouble(std::get<float>(param.value));
                        break;
                    case ftDouble:
                        query += formatDouble(std::get<double>(param.value));
                        break;
                    case ftString:
                        query += "'" + std::get<std::string>(param.value) + "'";
                        break;
                    case ftDateTime:
                        // Предполагается, что TDateTime::ToString() возвращает строку в формате ISO 8601
                        query += "'" + DateTimeToStr(std::get<TDateTime>(param.value)) + "'";
                        break;
                }
            }
        }

        query += ");";

        //auto begin = std::chrono::steady_clock::now();
        TracerManager::Instance().StartDBTrace(query);
        PGresult *result = PQexec(conn, query.c_str());
        TracerManager::Instance().EndDBTrace();
        //auto end = std::chrono::steady_clock::now();
        //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        //std::cout << "Время(мс): " << elapsed_ms.count() <<  ";Запрос: " << query << std::endl;

        std::string resultStr = getResultAsString(result);
        std::vector<std::string> resultVec = extractValues(resultStr);
        int outIndex = 0;
        for (size_t i = 0, outIndex = 0; i < Params->GetParamCount(); ++i) {
            TFDParam &param = Params->params[i];
            if (param.paramType == ptOutput) {
                // Присвоение значения в зависимости от типа параметра
                switch (param.type) {
                    case ftInteger:
                        param.SetValue(std::stoi(resultVec[outIndex]));
                        break;
                    case ftFloat:
                        param.SetValue(std::stof(resultVec[outIndex]));
                        break;
                    case ftDouble:
                        param.SetValue(std::stod(resultVec[outIndex]));
                        break;
                    case ftString:
                        param.SetValue(resultVec[outIndex]);
                        break;
                    case ftDateTime:
                        param.SetValue(StrToDateTime(resultVec[outIndex]));
                        break;
                }
                outIndex++;
            }
        }
//        int nrows = PQntuples(result);
//        for (int i = 0; i < nrows; i++) {
//            char* value = PQgetvalue(result, i, 0); // Получение значения первого столбца
//            std::cout << "Returned value: " << value << std::endl;
//        }
        PQclear(result);
    }

    void Free() {
        // Освобождение ресурсов, если это необходимо
    }

    std::string storedProcName;
};


#endif //MWSRESTAPI_DB_COMMON_H
