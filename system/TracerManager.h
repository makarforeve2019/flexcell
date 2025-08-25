#ifndef TRACERMANAGER_H
#define TRACERMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include "json.hpp"

class TracerManager {
public:
    static TracerManager& Instance();
    ~TracerManager();

    // ���������� ��������� � ���������� �����������
    static void EnableTracing();
    static void DisableTracing();
    static bool IsTracingEnabled();

    static bool IsSlotsDBEnabled();
    static bool IsSlot1DBEnabled();
    static bool IsSlot1PerfFuncEnabled();
    static bool IsSlot1F1LibEnabled();

    // ��������� ����������� endpoint ��� ���� �����������
    static void SetEndpoint(const std::string &endpoint);
    static std::string GetEndpoint();

    // ������ ��� ���������� ������
    void StartServiceTrace(const std::string &service_name);
    void EndServiceTrace();
    void StartFunctionTrace(const std::string &function_name);
    void AddSpanDefaultTags();
    void EndFunctionTrace();
    void StartDBTrace(const std::string &query);
    void EndDBTrace();
    void StartF1LibTrace();
    void EndF1LibTrace();
    void SaveSystemTraceTime();

    void StartSpan(const std::string &span_name);
    void EndSpan(bool force);
    void EndAllSpans();
    void AddSpanTag(const std::string &key, const std::string &value);
    void AddSpanLog(const std::string &log_message,
                    const std::unordered_map<std::string, std::string> &log_tags = {},
                    const int &log_level = TracerManager::LOG_LEVEL_INFO);

    // ������ �������� ������������ �����������
    static void IsServiceEnabled(const std::string &service);
    static void CheckClassTrace(const std::string &class_name);
    static std::string GetTraceLevel();
    static bool IsPerfFuncEnabled();

    // ����� ������������� �����������
    static void InitializeTracing(const std::string &request_id);
    static void InitSlot1();
    static void InitSlots();

    // ������ ��� ���������� ���������� ���������� �����������
    static void SetGlobalActivateTrace(bool activate);
    static bool GetGlobalActivateTrace();

    // ��������� �������������� ������������
    static void setTraceUserId(const std::string &user_id);

    // ���������� URL � body �������
    static void RecordURL(const std::string &url, const std::string &body);

    static void setTraceProfileId(const int &profile_id);

    static void SendIndependentSpan(const std::string &span_name, const std::unordered_map<std::string, std::string> &tags);

    // ���������� ������������ �����������
    static nlohmann::json trace_config_json;

    static std::string GetHostname();

    // ��������� ��� ������� �����������
    static inline const int LOG_LEVEL_MANDATORYINFO = 0;
    static inline const int LOG_LEVEL_ERROR = 1;
    static inline const int LOG_LEVEL_WARNING = 2;
    static inline const int LOG_LEVEL_INFO = 3;

    static std::string LogLevelStr(const int &log_level);


private:
    TracerManager();

    // ����������� ����� ������
    static bool enabled_;
    static bool postsend_;   // Флаг отложенного сохранения. true = сохранение трейса в конце запроса, а не при EndSpan
    static int profile_id_;
    static std::string endpoint_;
    static std::string hostname_;
    static std::string x_request_id_;
    static bool globalActivateTrace_;
    static std::string request_name_;
    static std::string service_;
    static std::string x_context_id_;
    static int trace_level_int;
    static bool slot1_db_;
    static std::string slot1_x_context_id_;
    static bool slot1_perf_func;
    static bool slot1_f1_lib;

    static long start_time_f1_lib;
    static long end_time_f1_lib;
    static long full_time_f1_lib;
    static long full_time_db;

    // ��������� ��� �������� ������ �����
    struct Span {
        std::string trace_id;
        std::string span_id;
        std::string parent_span_id;
        std::string span_name;
        long start_time;
        long end_time;
        std::unordered_map<std::string, std::string> tags;
        std::vector<std::tuple<std::string, std::unordered_map<std::string, std::string>, std::string>> logs;
        std::unordered_map<std::string, std::string> resource_attributes;

        ~Span() {
            tags.clear();
            logs.clear();
            resource_attributes.clear();
        }
    };

    // ���� ��� ����������� ������
    std::stack<Span> span_stack_;
    std::stack<Span> ended_span_stack_;

    // ��������������� ������
    struct curl_slist* GetHeaders() const;
    std::string GenerateTraceId() const;
    std::string GenerateSpanId() const;
    void SetDefaultValues();
    std::string BuildTraceJson(const Span &span) const;

    void SendSpan(const Span &completed_span);
};

#endif // TRACERMANAGER_H
