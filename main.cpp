#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <boost/stacktrace/frame.hpp>
#include <boost/stacktrace.hpp>

#include <iostream>
#include <string>
#include <thread>
#include "unWebModuleUnit.h"
#include "DB/unDBread.h"
#include "connectPQdb.h"
#include "StringTableSession.h"
#include <json.hpp>
#include "initialization.h"
#include "TracerManager.h"
#include "DB_Request_state.h"
#include <cstdlib>
#include <csignal>
#include "OtherServices/Common.h"
#include "dmGlobalVars.h"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
using namespace boost::filesystem;
using namespace std;
using json = nlohmann::json;

std::string endpointTrace;
bool globalActivateTrace;

std::string endpointNg;
std::string authHeaderGlob;

std::string ServiceName;

std::string FromService = "mws-rest-api-default";


TStringTableSession* GlobalSessionManager = nullptr;

void signalHandler( int signum ) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    std::string stacktrace;
    boost::stacktrace::stacktrace st;
    for (boost::stacktrace::frame frame: st) {
        stacktrace +=  to_string(frame) + "\n";
    }
    std::cout << "Stack dump:" << stacktrace << std::endl;

    TracerManager::Instance().AddSpanLog("Signal", {{"Stack dump", stacktrace},
                                                    {"signum", std::to_string(static_cast<int>(signum))}},
                                         TracerManager::LOG_LEVEL_ERROR);
    TracerManager::Instance().EndAllSpans();

    exit(signum);
}

void signalHandlerInit(  ) {
    //signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGBUS, signalHandler);
    //signal(SIGILL, signalHandler);
    signal(SIGABRT, signalHandler);
    //signal(SIGFPE, signalHandler);
    signal(SIGSTOP, signalHandler);
    //signal(SIGTERM, signalHandler);
}


class Server {
    boost::asio::io_context ioc;
    tcp::acceptor acceptor_;
    tcp::socket socket_;

public:
    Server(short port) : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {}
    std::string versionBac = "v.0.5.27 NEWFLEXCELL";
    void start() {
        std::string errorMessage;
        bool isConnected = TDBread::TestDBConnection(
                admUserName, admUserPassword, aDatabaseName, aServerName, aPort,
                aUserName_docum, aUserPassword_docum, aDatabaseName_docum, aServerName_docum, aPort_docum,
                aIsAnotherServer_docum, isDBpostgre_docum, errorMessage
        );

        // Устанавливаем глобальный endpoint и включаем трассировку
        TracerManager::SetEndpoint(endpointTrace);
        TracerManager::SetGlobalActivateTrace(globalActivateTrace);
        TracerManager::EnableTracing();

        if (ConnectDBAtStartProc==1){
            TDBread::ConnectDB(admUserName, admUserPassword, aDatabaseName, aServerName, aPort,
                               aUserName_docum, aUserPassword_docum, aDatabaseName_docum, aServerName_docum, aPort_docum, aIsAnotherServer_docum, isDBpostgre_docum);
        }

        // Инициализируем трассировку при старте сервера

        cout << endl << "   - Server is starting on port "<< getGlobalPort() << " " << /*SERVER_VERSION*/ versionBac << endl;
        GlobalSessionManager = new TStringTableSession();
        GlobalSessionManager->MaxInactiveHours = 3;
        listenAndServe();
    }

    void printSocketQueue(tcp::acceptor& acceptor_) {
        int listenFd = acceptor_.native_handle(); // Получаем файловый дескриптор сокета

        // Опция для получения информации о TCP-соединении
        struct tcp_info tcpInfo;
        socklen_t tcpInfoLen = sizeof(tcpInfo);

        if (getsockopt(listenFd, SOL_TCP, TCP_INFO, &tcpInfo, &tcpInfoLen) == 0) {
            if (TracerManager::Instance().GetTraceLevel() == "INFO") {
                // Формируем строку с информацией
                std::string queueInfo =
                        "Unacknowledged packets: " + std::to_string(tcpInfo.tcpi_unacked) +
                        ", Pending packets: " + std::to_string(tcpInfo.tcpi_sacked);

                // Отправляем один спан
                TracerManager::Instance().SendIndependentSpan(
                        "TCP Queue Info",   // Имя спана
                        {{"QueueInfo", queueInfo}} // Один тег с объединённой информацией
                );
            }
        } else {
            // Логирование ошибки, если нужно
//        std::cerr << "Error retrieving TCP queue information." << std::endl;
        }
    }

    void listenAndServe() {
        while (true) {
            try {
                // Печатаем информацию о состоянии очереди
                printSocketQueue(acceptor_);

                acceptor_.accept(socket_);

                // Установка параметра keep-alive для сокета
                boost::asio::socket_base::keep_alive option(true);
                socket_.set_option(option);

                // Чтение запроса с ограничением на размер тела
                boost::beast::flat_buffer buffer;
                http::request_parser<http::string_body> parser;
                parser.body_limit(1024 * 1024 * 1024); // Устанавливаем лимит на 1024MB

                http::request<http::string_body> request;
                http::read(socket_, buffer, parser);
                request = parser.release();

                // Остальная обработка запроса
                std::string authHeader;
                std::string url = std::string(request.target());
                std::string body = request.body();
                std::string adminPermissionHeader;

                if (request.base().find(http::field::authorization) != request.base().end()) {
                    authHeader = std::string(request[http::field::authorization]);
                    authHeaderGlob = std::string(request[http::field::authorization]);
                }

                for (auto const& field : request.base()) {
                    if (field.name_string() == "Admin-Permission") {
                        adminPermissionHeader = std::string(field.value());
                        break;
                    }
                }

                for (auto const& field : request.base()) {
                    if (field.name_string() == "From-Service") {
                        FromService = std::string(field.value());
                        break;
                    }
                }

                std::string xRequestId;
                for (auto const& field : request.base()) {
                    std::string headerName(field.name_string().data(), field.name_string().size());
                    std::transform(headerName.begin(), headerName.end(), headerName.begin(), ::tolower);

                    if (headerName == "x-request-id") {
                        xRequestId = std::string(field.value()); // Сохраняем значение как строку
                        break;
                    }
                }

                std::string xContextId;
                for (auto const& field : request.base()) {
                    std::string headerName(field.name_string().data(), field.name_string().size());
                    std::transform(headerName.begin(), headerName.end(), headerName.begin(), ::tolower);

                    if (headerName == "x-context-id") {
                        xContextId = std::string(field.value()); // Сохраняем значение как строку
                        break;
                    }
                }

                // Изначально создаем response
                http::response<http::string_body> response;

                if (ConnectDBAtStartProc == 0) {
                    TDBread::ConnectDB(admUserName, admUserPassword, aDatabaseName, aServerName, aPort,
                                       aUserName_docum, aUserPassword_docum, aDatabaseName_docum, aServerName_docum,
                                       aPort_docum, aIsAnotherServer_docum, isDBpostgre_docum);
                }

                // Если URL содержит "/Spreadsheet/OpenForm", отправляем 202 (Accepted)
                if (url.find("/Spreadsheet/OpenForm") != std::string::npos) {
                    int responseStatus = 202;

                    std::string basePath = "/MWSRestAPI/UBPRosleshoz";
                    Common common(endpointNg);

                    std::string responseBody = common.RequestStateOpen(basePath, xRequestId, responseStatus);

                    if (responseStatus == 200) {
                        //std::cout << "RequestStateOpen succeeded: " << responseBody << std::endl;
                    } else {
                        std::cout << "RequestStateOpen failed with status: " << responseStatus << std::endl;
                    }

                    boost::beast::http::status status = boost::beast::http::status::accepted;
                    response.result(status);
                    response.version(request.version());
                    response.prepare_payload();
                    http::write(socket_, response);
                }

                // Установите начальный статус
                boost::beast::http::status status = http::status::ok;

                // Вызов handle_request
                std::string responseContent = handle_request(body, url, authHeader, status, response, adminPermissionHeader, xRequestId, xContextId);

                if (ConnectDBAtStartProc == 0) {
                    TDBread::DisconnectDB();
                }

                if (url.find("/Spreadsheet/OpenForm") != std::string::npos) {
                    // Завершаем обработку и переходим к следующей итерации цикла
                    if (socket_.is_open()) {
                        try {
                            //std::cout << "Attempting to shutdown and close socket" << std::endl;
                            socket_.shutdown(tcp::socket::shutdown_send);
                            socket_.close();
                            //std::cout << "Socket closed successfully" << std::endl;
                        } catch (const boost::system::system_error& e) {
                            std::cerr << "Socket shutdown error: " << e.what() << std::endl;
                        }
                    } else {
                        std::cerr << "Socket is already closed" << std::endl;
                    }
                    continue; // Переход к следующей итерации цикла
                }

                // После того, как status был обновлен, устанавливаем его в response
                response.result(status);
                response.version(request.version());
                response.set(http::field::content_type, "application/json");
                response.body() = responseContent;
                response.prepare_payload();
                // Отправка финального ответа
                http::write(socket_, response);
            } catch (const std::exception& e) {
                http::response<http::string_body> response{http::status::internal_server_error, 11};
                std::cout << e.what() << std::endl;
                response.body() = e.what();
                response.prepare_payload();
                http::write(socket_, response);
            } catch (...) {
                std::cerr << "Неизвестная ошибка" << std::endl;
                http::response<http::string_body> response{http::status::internal_server_error, 11};
                response.body() = "Unknown error";
                response.prepare_payload();
                http::write(socket_, response);
            }

            // Закрытие сокета
            if (socket_.is_open()) {
                try {
                    //std::cout << "Attempting to shutdown socket..." << std::endl;
                    socket_.shutdown(tcp::socket::shutdown_send);
                    //std::cout << "Socket shutdown successfully." << std::endl;
                } catch (const boost::system::system_error& e) {
                    if (e.code() == boost::asio::error::not_connected) {
                        std::cerr << "Socket was already closed by the other side." << std::endl;
                    } else {
                        std::cerr << "Socket shutdown error: " << e.what() << std::endl;
                    }
                }

                try {
                    socket_.close();
                    //std::cout << "Socket closed successfully." << std::endl;
                } catch (const boost::system::system_error& e) {
                    std::cerr << "Socket close error: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Socket is already closed. No action required." << std::endl;
            }
        }
    }

    void stop() {
        cout << "   - Server stopped." << endl;
        // Завершение трассировки при остановке сервера
        delete GlobalSessionManager;
        GlobalSessionManager = nullptr;
        ioc.stop();
    }

    void loadConfig(const boost::filesystem::path& configPath) {
        if (!boost::filesystem::exists(configPath)) {
            std::cerr << "Configuration file does not exist: " << configPath << std::endl;
            throw std::runtime_error("Configuration file not found");
        }

        std::ifstream file(configPath);
        json configJson;
        file >> configJson;

        // Загрузка настроек базы данных
//        admUserName = configJson["Database"]["admUserName"];
//        admUserPassword = configJson["Database"]["admUserPassword"];

        admUserName = std::getenv("DB_USER") ? std::getenv("DB_USER") : "";
        admUserPassword = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "";



        aDatabaseName = configJson["Database"]["aDatabaseName"];
        aServerName = configJson["Database"]["aServerName"];
        aPort = configJson["Database"]["aPort"];

//        aUserName_docum = configJson["Database"]["aUserName_docum"];DOCUM_DB_USER и DOCUM_DB_PASSWORD
//        aUserPassword_docum = configJson["Database"]["aUserPassword_docum"];

        aUserName_docum = std::getenv("DOCUM_DB_USER") ? std::getenv("DOCUM_DB_USER") : admUserName;
        aUserPassword_docum = std::getenv("DOCUM_DB_PASSWORD") ? std::getenv("DOCUM_DB_PASSWORD") : admUserPassword;

        aDatabaseName_docum = configJson["Database"]["aDatabaseName_docum"];
        aServerName_docum = configJson["Database"]["aServerName_docum"];
        aPort_docum = configJson["Database"]["aPort_docum"];
        aIsAnotherServer_docum = configJson["Database"]["aIsAnotherServer_docum"];
        isDBpostgre_docum = configJson["Database"]["isDBpostgre_docum"];

        if (configJson["Database"].contains("ConnectDBAtStartProc") && !configJson["Database"]["ConnectDBAtStartProc"].is_null()) {
            ConnectDBAtStartProc = configJson["Database"]["ConnectDBAtStartProc"].get<int>();
        } else {
            std::cerr << "ConnectDBAtStartProc is missing or not a number, using default value 0" << std::endl;
            ConnectDBAtStartProc = 0;
        }

        if (configJson["Database"].contains("ConnectDBTimeout") && !configJson["Database"]["ConnectDBTimeout"].is_null()) {
            ConnectDBTimeout = configJson["Database"]["ConnectDBTimeout"].get<int>();
        } else {
            std::cerr << "ConnectDBTimeout is missing or not a number, using default value 10" << std::endl;
            ConnectDBTimeout = 10;
        }

        if (configJson["Database"].contains("ConnectDBAttempts") && !configJson["Database"]["ConnectDBAttempts"].is_null()) {
            ConnectDBAttempts = configJson["Database"]["ConnectDBAttempts"].get<int>();
        } else {
            std::cerr << "ConnectDBAttempts is missing or not a number, using default value 3" << std::endl;
            ConnectDBAttempts = 2;
        }

        // Загрузка настроек авторизации
        AuthOption = configJson["Authorization"]["AuthOption"];
        AuthExternalType = configJson["Authorization"]["AuthExternalType"];
        AuthExternalServer = configJson["Authorization"]["AuthExternalServer"];
        DefaultContext = configJson["Authorization"]["DefaultContext"];
        UpdateUserPermissions = configJson["Authorization"]["UpdateUserPermissions"];
        CreateUser = configJson["Authorization"]["CreateUser"];

        endpointTrace = configJson["TraceService"]["endpointTrace"];
        globalActivateTrace = configJson["TraceService"]["globalActivateTrace"];

        endpointNg = configJson["NgService"]["endpointNg"];

        ServiceName = std::getenv("FROM_SERVICE") ? std::getenv("FROM_SERVICE") : "mws-rest-api-default";

        // Выводим сообщение о успешной загрузке конфигурации
        std::cout << "Configuration successfully loaded from " << configPath << std::endl << std::endl;
    }
};

int main(int argc, char* argv[]) {
    signalHandlerInit();
    try {

        // Установить глобальную локаль
        boost::locale::generator gen;
        std::locale::global(gen("ru_RU.UTF-8"));
        std::wcout.imbue(std::locale());
        std::locale::global(std::locale("C"));

        // Инициализация программы
        initialization::InitializeProgram();

        const std::vector<std::string_view> args(argv + 1, argv + argc);

        std::string command;
        Server* server = nullptr;
        boost::filesystem::path defaultConfig = boost::filesystem::current_path() / "mwsconfigsvr.json";

        if (!args.empty()) {
            int port = 0;
            std::string configPath = defaultConfig.string();  // Используем путь по умолчанию

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == "-port" && i + 1 < args.size()) {
                    port = std::stoi(std::string(args[i + 1]));
                    ++i; // Пропускаем следующий аргумент
                } else if (args[i] == "-pf" && i + 1 < args.size()) {
                    configPath = std::string(args[i + 1]);
                    ++i; // Пропускаем следующий аргумент
                }
            }

            if (port > 0) {
                server = new Server(port);
                try {
                    server->loadConfig(configPath);
                    setGlobalPort(port);
                    server->start();
                } catch (const std::exception& e) {
                    std::cerr << "Failed to start server: " << e.what() << std::endl;
                    delete server;
                    return 1;
                }
                delete server;
                return 0;
            } else {
                std::cerr << "Port must be specified with -port option." << std::endl;
                return 1;
            }
        }

        while (true) {
            std::getline(std::cin, command);
            std::stringstream ss(command);
            std::string cmd;
            int port;
            ss >> cmd;

            if (cmd == "start") {
                if (ss >> port) {
                    boost::filesystem::path configPath;
                    if (!(ss >> configPath)) {
                        configPath = defaultConfig;
                    } else {
                        try {
                            // Пытаемся преобразовать путь в канонический вид
                            configPath = boost::filesystem::canonical(configPath);
                        } catch (const boost::filesystem::filesystem_error& e) {
                            // Если указан некорректный путь, выводим ошибку и продолжаем цикл
                            std::cerr << "Error: " << e.what() << ". Please specify a correct path to the configuration file." << std::endl;
                            continue;
                        }
                    }
                    if (server) {
                        server->stop();
                        delete server;
                    }
                    server = new Server(port);
                    try {
                        server->loadConfig(configPath);
                        setGlobalPort(port);
                        std::thread serverThread([&] { server->start(); });
                        serverThread.detach();
                    } catch (const std::exception& e) {
                        std::cerr << "Failed to start server: " << e.what() << std::endl;
                        delete server;
                        server = nullptr;
                    }
                } else {
                    std::cout << "Please specify a port number after 'start' command and optionally a path to the configuration file." << std::endl;
                }
            } else if (cmd == "stop") {
                if (server) {
                    server->stop();
                    delete server;
                    server = nullptr;
                }
                break;
            } else {
                std::cout << "Unknown command. Try 'start <port> [path_to_config]' or 'stop'." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
