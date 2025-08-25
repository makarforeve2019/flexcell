#include "TracerManager.h"
#include <chrono>
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <unistd.h>
#include <algorithm>

// Инициализация статических членов
nlohmann::json TracerManager::trace_config_json = nlohmann::json::object();
bool TracerManager::enabled_ = false;
bool TracerManager::postsend_ = false;
std::string TracerManager::endpoint_;
std::string TracerManager::hostname_;
std::string TracerManager::x_request_id_;
bool TracerManager::globalActivateTrace_ = false;
std::string TracerManager::request_name_;
std::string TracerManager::service_;
std::string TracerManager::x_context_id_;
int TracerManager::trace_level_int = 0;
bool TracerManager::slot1_db_ = false;
std::string TracerManager::slot1_x_context_id_;
bool TracerManager::slot1_perf_func = false;
bool TracerManager::slot1_f1_lib = false;
long TracerManager::end_time_f1_lib = 0;
long TracerManager::start_time_f1_lib = 0;
long TracerManager::full_time_f1_lib = 0;
long TracerManager::full_time_db = 0;
int TracerManager::profile_id_ = 0;

// Конструктор и получение единственного экземпляра
TracerManager& TracerManager::Instance() {
    static TracerManager instance;
    return instance;
}

// Конструктор
TracerManager::TracerManager() {
    curl_global_init(CURL_GLOBAL_ALL);
    SetDefaultValues(); // Инициализация значений по умолчанию
}

// Деструктор
TracerManager::~TracerManager() {
    curl_global_cleanup();
}

// Установка глобального endpoint
void TracerManager::SetEndpoint(const std::string &endpoint) {
    endpoint_ = "http://"+endpoint;
}

// Получение глобального endpoint
std::string TracerManager::GetEndpoint() {
    return endpoint_;
}

// Включение трассировки
void TracerManager::EnableTracing() {
    enabled_ = true;
}

// Отключение трассировки
void TracerManager::DisableTracing() {
    enabled_ = false;
}

// Проверка статуса трассировки
bool TracerManager::IsTracingEnabled() {
    return enabled_;
}

// Установка глобальной активации трассировки
void TracerManager::SetGlobalActivateTrace(bool activate) {
    globalActivateTrace_ = activate;
}

// Получение состояния глобальной активации трассировки
bool TracerManager::GetGlobalActivateTrace() {
    return globalActivateTrace_;
}

// Установка идентификатора пользователя
void TracerManager::setTraceUserId(const std::string &user_id) {
    x_context_id_ = user_id;
}

// Добавление обязательных тегов в спан
void TracerManager::AddSpanDefaultTags() {
    if (!enabled_ || span_stack_.empty()) return;
    AddSpanTag("hostname", hostname_);
    AddSpanTag("xrequestid", x_request_id_);
    AddSpanTag("xcontextid", x_context_id_);
}

// Начало трассировки сервиса
void TracerManager::StartServiceTrace(const std::string &service_name) {
    //IsServiceEnabled(service_name);
    if (!enabled_) return;

    request_name_ = service_name;
    StartSpan(service_name);
    AddSpanDefaultTags();
    AddSpanTag("requestname", service_name);

//    AddSpanTag("hostname", hostname_);
//    AddSpanTag("xrequestid", x_request_id_);
//    AddSpanTag("xcontextid", x_context_id_);
//    AddSpanTag("service.name", service_);
}

// Завершение трассировки сервиса
void TracerManager::EndServiceTrace() {
    EndSpan(false);
}


// Начало трассировки функции
void TracerManager::StartFunctionTrace(const std::string &function_name) {
    if (!enabled_ || !IsPerfFuncEnabled()) return;

    StartSpan(function_name);
    AddSpanDefaultTags();
    //AddSpanTag("requestname", request_name_);
}

// Завершение трассировки функции
void TracerManager::EndFunctionTrace() {
    if (!enabled_ || !IsPerfFuncEnabled()) return;
    EndSpan(false);
}

// Начало трассировки БД
void TracerManager::StartDBTrace(const std::string &query) {
    if (!enabled_) return;
    if (!IsSlotsDBEnabled()) return;

    StartSpan(query);
    AddSpanDefaultTags();
    AddSpanTag("db", "1");
}

// Завершение трассировки БД
void TracerManager::EndDBTrace() {
    if (!enabled_) return;
    if (!IsSlotsDBEnabled()) return;

    EndSpan(false);
    // Расчет полного времени выполнения БД
    if (postsend_) {
        Span completed_span = ended_span_stack_.top();
        full_time_db = full_time_db + (completed_span.end_time - completed_span.start_time);
    } else {
        Span completed_span = span_stack_.top();
        full_time_db = full_time_db + (completed_span.end_time - completed_span.start_time);
    }
}

// Проверка что в слотах установлена трассировка БД
bool TracerManager::IsSlotsDBEnabled() {
    return (IsSlot1DBEnabled());
}

// Проверка что в слоте1 установлена трассировка БД
bool TracerManager::IsSlot1DBEnabled() {
    return ((slot1_db_) && (slot1_x_context_id_ == x_context_id_));
}

// Проверка что в слоте1 установлена трассировка Функций
bool TracerManager::IsSlot1PerfFuncEnabled() {
    return ((slot1_perf_func) && (slot1_x_context_id_ == x_context_id_));
}


// Метод для инициализации трассировки
void TracerManager::InitializeTracing(const std::string &request_id) {
    std::string trace_level = GetTraceLevel();
//    if (trace_level.empty() || !trace_config_json.contains("services") || !trace_config_json.contains("classes")) {
//        enabled_ = false;
//        return;
//    }
    if (trace_level.empty()) {
        enabled_ = false;
        return;
    }
    enabled_ = true;

    full_time_f1_lib = 0;
    full_time_db = 0;
    postsend_ = false;
    if (trace_config_json.contains("postsend") && !trace_config_json["postsend"].is_null()) {
        postsend_ = trace_config_json["postsend"].get<bool>();  // Инициация флага отложенного сохранения
    }

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    hostname_ = hostname;
    x_request_id_ = request_id;

    trace_level_int = (GetTraceLevel() == "ERROR") ? LOG_LEVEL_ERROR :
                      (GetTraceLevel() == "WARNING") ? LOG_LEVEL_WARNING : LOG_LEVEL_INFO;

}

// Проверка доступности сервиса
void TracerManager::IsServiceEnabled(const std::string &service) {
    if (!GetGlobalActivateTrace()) {
        enabled_ = false;
        return;
    }

    if (trace_config_json.contains("services")) {
        std::string service_lower = service;
        std::transform(service_lower.begin(), service_lower.end(), service_lower.begin(), ::tolower);

        for (const auto &s : trace_config_json["services"]) {
            std::string s_lower = s;
            std::transform(s_lower.begin(), s_lower.end(), s_lower.begin(), ::tolower);

            if (s_lower == service_lower || s_lower == "*") {
                enabled_ = true;
                return;
            }
        }
    }

    enabled_ = false;
}

// Проверка доступности класса для трассировки
void TracerManager::CheckClassTrace(const std::string &class_name) {
//    if (!GetGlobalActivateTrace()) {
//        enabled_ = false;
//        return;
//    }
//
//    std::string class_name_lower = class_name;
//    std::transform(class_name_lower.begin(), class_name_lower.end(), class_name_lower.begin(), ::tolower);
//
//    if (trace_config_json.contains("classes")) {
//        for (const auto &c : trace_config_json["classes"]) {
//            std::string c_lower = c;
//            std::transform(c_lower.begin(), c_lower.end(), c_lower.begin(), ::tolower);
//
//            if (c_lower == class_name_lower || c_lower == "*") {
//                enabled_ = true;
//                return;
//            }
//        }
//    }
//    enabled_ = false;
}

void TracerManager::AddSpanTag(const std::string &key, const std::string &value) {
    // Проверяем, включена ли трассировка и есть ли активный спан
    if (!enabled_ || span_stack_.empty()) return;

    // Добавляем тег в текущий спан (верхний в стеке)
    span_stack_.top().tags[key] = value;
}


// Добавление лог-события в спан
void TracerManager::AddSpanLog(const std::string &log_message,
                               const std::unordered_map<std::string, std::string> &log_tags,
                               const int &log_level) {
    if (!enabled_ || span_stack_.empty()) return;

    if (log_level > trace_level_int) return;

    std::unordered_map<std::string, std::string> enriched_log_tags = log_tags;
    enriched_log_tags["hostname"] = hostname_;
    enriched_log_tags["xrequestid"] = x_request_id_;
    enriched_log_tags["xcontextid"] = x_context_id_;
    enriched_log_tags["requestname"] = request_name_  ;
//    enriched_log_tags["service.name"] = service_name_  ;

    if (profile_id_ != 0) {
        enriched_log_tags["profileid"] = std::to_string(profile_id_);
    }

    if (log_level == LOG_LEVEL_ERROR) {
        enriched_log_tags["error"] = "true";
        AddSpanTag("error", "true");
    }

    span_stack_.top().logs.emplace_back(log_message, enriched_log_tags, LogLevelStr(log_level));
}

// Начало спана
void TracerManager::StartSpan(const std::string &span_name) {
    if (!enabled_) return;

    Span new_span;
    new_span.trace_id = span_stack_.empty() ? GenerateTraceId() : span_stack_.top().trace_id;
    new_span.span_id = GenerateSpanId();
    new_span.parent_span_id = span_stack_.empty() ? "" : span_stack_.top().span_id;
    new_span.span_name = span_name;
    new_span.start_time = std::chrono::system_clock::now().time_since_epoch().count();
    new_span.end_time = 0;

    span_stack_.push(new_span);
}

size_t IgnoreResponse(void* contents, size_t size, size_t nmemb, void* userp) {
    // Возвращаем размер данных, чтобы CURL считал их обработанными
    return size * nmemb;
}

void TracerManager::EndSpan(bool force=false) {
    if (span_stack_.empty()) return;
    if (!force) {
        if (!enabled_) return;
    }

    Span completed_span = span_stack_.top();
    span_stack_.pop();

    if (completed_span.end_time == 0) {
        completed_span.end_time = std::chrono::system_clock::now().time_since_epoch().count();
    }

    if ((!force) && (postsend_)) {   // если не завершающий EndSpan и отложенное сохранение
        ended_span_stack_.push(completed_span);   // сохраняем в стек завершенных спанов
        return;
    }

    SendSpan(completed_span);
}

void TracerManager::SendSpan(const Span &completed_span) {
    int still_running = 0;
    CURL *curl = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();

    if (curl && multi_handle) {
        std::string trace_json = BuildTraceJson(completed_span);

        // Вывод JSON в консоль
        //std::cout << "Отправляемый JSON: " << trace_json << std::endl;

        struct curl_slist *trace_curl_headers = nullptr;
        trace_curl_headers = curl_slist_append(trace_curl_headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, endpoint_.c_str());
        //std::cout << "Отправка запроса на URL: " << endpoint_ << std::endl;
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, trace_json.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, trace_json.size());
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, GetHeaders());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, trace_curl_headers);

        // Устанавливаем функцию для игнорирования вывода
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, IgnoreResponse);

        curl_multi_add_handle(multi_handle, curl);

        int attempt_count = 0;
        do {
            CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
            if(still_running)
                mc = curl_multi_poll(multi_handle, NULL, 0, 10, NULL);
            else
                break;

            if(mc)
                break;
            if(attempt_count > 100) {
                DisableTracing(); // Traceoff because jaeger not available
                std::cerr << "Traceoff because jaeger server not available" << std::endl;
                break;
            }
            attempt_count++;
        } while(still_running);

        curl_slist_free_all(trace_curl_headers);
        curl_multi_cleanup(multi_handle);

        curl_easy_cleanup(curl);
    }
}


// Запись URL и body запроса
void TracerManager::RecordURL(const std::string &url, const std::string &body) {
    // Получаем ссылку на текущий объект через Instance()
    TracerManager& instance = TracerManager::Instance();

    // Проверяем, включена ли трассировка и есть ли активный спан
    if (!instance.enabled_ || instance.span_stack_.empty()) return;

    // Добавляем теги в текущий спан
    instance.AddSpanTag("url", url);
    instance.AddSpanTag("body", body);
}

// Генерация traceId
std::string TracerManager::GenerateTraceId() const {
    std::stringstream ss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 15);
    for (int i = 0; i < 32; ++i) {
        ss << std::hex << dist(gen);
    }
    return ss.str();
}

// Генерация spanId
std::string TracerManager::GenerateSpanId() const {
    std::stringstream ss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 15);
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << dist(gen);
    }
    return ss.str();
}

// Получение заголовков для HTTP-запроса
struct curl_slist* TracerManager::GetHeaders() const {
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    return headers;
}

// Сборка JSON для отправки на сервер
std::string TracerManager::BuildTraceJson(const Span &span) const {
    //auto end_time = std::chrono::system_clock::now().time_since_epoch().count();
    auto end_time = span.end_time;

    // Начинаем формировать JSON
    nlohmann::json resourceSpans = {
            {"resource", {
                                 {"attributes", nlohmann::json::array()}
                         }},
            {"scopeSpans", nlohmann::json::array()}
    };

    // Добавляем атрибуты ресурса
    for (const auto &attr : span.resource_attributes) {
        resourceSpans["resource"]["attributes"].push_back({
                                                                  {"key", attr.first},
                                                                  {"value", {{"stringValue", attr.second}}}
                                                          });
    }

    // Гарантированно добавляем service.name
    resourceSpans["resource"]["attributes"].push_back({
                                                              {"key", "service.name"},
                                                              {"value", {{"stringValue", service_}}}
                                                      });

//    std::string span_name;
//    if ( span.span_name.length() > 250 ) {
//        span_name = span.span_name.substr(0, 250) + "...";
//    }
//    else {
//        span_name = span.span_name;
//    }

    // Формируем данные спана
    nlohmann::json spanJson = {
            {"traceId", span.trace_id},
            {"spanId", span.span_id},
            {"parentSpanId", span.parent_span_id},
            {"name", ( span.span_name.length() > 250 ) ? span.span_name.substr(0, 250) + "..." : span.span_name },
            //{"name", span.span_name},
            //{"name", span_name},
            {"kind", "SPAN_KIND_INTERNAL"},
            {"startTimeUnixNano", span.start_time},
            {"endTimeUnixNano", end_time},
            {"attributes", nlohmann::json::array()},
            {"events", nlohmann::json::array()}
    };

    // Добавляем теги (атрибуты спана)
    for (const auto &tag : span.tags) {
        spanJson["attributes"].push_back({
                                                 {"key", tag.first},
                                                 {"value", {{"stringValue", tag.second}}}
                                         });
    }

    // Добавляем события (логи)
    for (const auto &log : span.logs) {
        const auto& [message, tags, level] = log;
        nlohmann::json event = {
                {"name", message},
                {"timeUnixNano", span.start_time},
                {"attributes", nlohmann::json::array()}
        };

        // Добавляем лог-уровень
        event["attributes"].push_back({
                                              {"key", "log.level"},
                                              {"value", {{"stringValue", level}}}
                                      });

        // Добавляем дополнительные теги для события
        for (const auto &tag : tags) {
            event["attributes"].push_back({
                                                  {"key", tag.first},
                                                  {"value", {{"stringValue", tag.second}}}
                                          });
        }

        // Добавляем событие в список
        spanJson["events"].push_back(event);
    }

    // Формируем ScopeSpans
    nlohmann::json scopeSpan = {
            {"scope", {
                              {"name", "TracerManager"},
                              {"version", "1.0.0"}
                      }},
            {"spans", nlohmann::json::array({spanJson})}
    };

    resourceSpans["scopeSpans"].push_back(scopeSpan);

    // Финальное формирование JSON
    nlohmann::json traceJson = {
            {"resourceSpans", nlohmann::json::array({resourceSpans})}
    };

    // Возвращаем строку JSON
    //std::cout<< traceJson.dump() << std::endl;
    try {
        return traceJson.dump();
    } catch (const nlohmann::json::exception& e) {
        return "{}";  // Возвращаем пустой JSON
    }
}


// Получение уровня трассировки
std::string TracerManager::GetTraceLevel() {
    if (trace_config_json.contains("tracelevel") && !trace_config_json["tracelevel"].is_null()) {
        return trace_config_json["tracelevel"];
    }
    return "";
}

// Инициализация слота1 трассировки пользователя
void TracerManager::InitSlot1() {
    slot1_db_ = false;
    slot1_x_context_id_.clear();
    slot1_perf_func = false;
    slot1_f1_lib = true;
    if (trace_config_json.contains("slot1_db") && !trace_config_json["slot1_db"].is_null()) {
        slot1_db_ = trace_config_json["slot1_db"].get<bool>();
    }
    if (trace_config_json.contains("slot1_userid") && !trace_config_json["slot1_userid"].is_null()) {
        slot1_x_context_id_ = trace_config_json["slot1_userid"];
    }
    if (trace_config_json.contains("slot1_perf_func") && !trace_config_json["slot1_perf_func"].is_null()) {
        slot1_perf_func = trace_config_json["slot1_perf_func"].get<bool>();;
    }

}

// Инициализация слотов трассировки пользователя
void TracerManager::InitSlots() {
    InitSlot1();
}


// Проверка включенности функциональной трассировки
bool TracerManager::IsPerfFuncEnabled() {
//    if (trace_config_json.contains("perf_func") && !trace_config_json["perf_func"].is_null()) {
//        return trace_config_json["perf_func"].get<bool>();
//    }
    return IsSlot1PerfFuncEnabled();
}

// Установка значений по умолчанию для атрибутов ресурса
void TracerManager::SetDefaultValues() {
    request_name_ = "default-request";
    service_ = "MWSRestAPI";
}

// Получение имени хоста
std::string TracerManager::GetHostname() {
    return hostname_;
}

std::string TracerManager::LogLevelStr(const int &log_level) {
    switch ( log_level )
    {
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_WARNING:
            return "WARNING";
        case LOG_LEVEL_INFO:
            return "INFO";
        default:
            return "";
    }
}

void TracerManager::SaveSystemTraceTime() {
    if (IsSlot1F1LibEnabled()) {
        full_time_f1_lib = full_time_f1_lib / 1000000;  // nanoseconds to milisecounds
        TracerManager::Instance().AddSpanLog("F1Spreadsheet library time", {
                                                               {"F1 full time(ms)", std::to_string(static_cast<long>(full_time_f1_lib))}},
                                             TracerManager::LOG_LEVEL_MANDATORYINFO);
    }
    if (IsSlotsDBEnabled()) {
        full_time_db = full_time_db / 1000000;  // nanoseconds to milisecounds
        TracerManager::Instance().AddSpanLog("DB time", {
                                                     {"DB full time(ms)", std::to_string(static_cast<long>(full_time_db))}},
                                             TracerManager::LOG_LEVEL_MANDATORYINFO);
    }
}

void TracerManager::EndAllSpans() {
    // Проверяем, включена ли трассировка
    //if (!enabled_) return;

    SaveSystemTraceTime();

    // Завершаем все спаны в стеках
    while (!ended_span_stack_.empty()) {
        Span ended_span = ended_span_stack_.top();
        ended_span_stack_.pop();
        span_stack_.push(ended_span);
    }

    while (!span_stack_.empty()) {
        EndSpan(true);
    }
}

void TracerManager::SendIndependentSpan(const std::string &span_name, const std::unordered_map<std::string, std::string> &tags) {
    if (!enabled_) return;

    // Получаем экземпляр TracerManager
    TracerManager &instance = TracerManager::Instance();

    // Создаем новый независимый спан
    Span independent_span;
    independent_span.trace_id = instance.GenerateTraceId(); // Генерация trace_id
    independent_span.span_id = instance.GenerateSpanId();   // Генерация span_id
    independent_span.parent_span_id = "";                   // Независимый спан не имеет родителя
    independent_span.span_name = span_name;
    independent_span.start_time = std::chrono::system_clock::now().time_since_epoch().count();
    independent_span.end_time = independent_span.start_time++;

    // Добавляем теги
    for (const auto &tag : tags) {
        independent_span.tags[tag.first] = tag.second;
    }

    // Генерация JSON для спана
    std::string trace_json = instance.BuildTraceJson(independent_span);

    // Отправка данных через CURL
    CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, endpoint_.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, trace_json.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, IgnoreResponse);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to send independent span: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void TracerManager::StartF1LibTrace() {
    if (IsSlot1F1LibEnabled()) {
        start_time_f1_lib = std::chrono::system_clock::now().time_since_epoch().count();
    }
}

void TracerManager::EndF1LibTrace() {
    if (IsSlot1F1LibEnabled()) {
        end_time_f1_lib = std::chrono::system_clock::now().time_since_epoch().count();
        full_time_f1_lib = full_time_f1_lib + ( end_time_f1_lib - start_time_f1_lib );
    }
}

bool TracerManager::IsSlot1F1LibEnabled() {
    return ((slot1_f1_lib) && (slot1_x_context_id_ == x_context_id_));
}

void TracerManager::setTraceProfileId(const int &profile_id) {
    profile_id_ = profile_id;
}


/*
void TracerManager::EndSpan() {
    if (!enabled_ || span_stack_.empty()) return;

    Span completed_span = span_stack_.top();
    span_stack_.pop();

    int still_running = 0;
    CURL *curl = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();
    int myfd;

    if (curl && multi_handle) {
        std::string trace_json = BuildTraceJson(completed_span);

        // Вывод JSON в консоль
        //std::cout << "Отправляемый JSON: " << trace_json << std::endl;

        curl_easy_setopt(curl, CURLOPT_URL, endpoint_.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, trace_json.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, trace_json.size());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, GetHeaders());

        // Устанавливаем функцию для игнорирования вывода
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, IgnoreResponse);

        curl_multi_add_handle(multi_handle, curl);

        int attempt_count = 0;
        do {
            CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
            struct curl_waitfd myown;
            myown.fd = myfd;
            myown.events = CURL_WAIT_POLLOUT;
            myown.revents = 0;

            if(still_running)
                mc = curl_multi_poll(multi_handle, &myown, 1, 0, NULL);

            if(mc || (attempt_count > 10))
                break;
            attempt_count++;
        } while(still_running);

        curl_multi_cleanup(multi_handle);

        curl_easy_cleanup(curl);
    }
}
*/