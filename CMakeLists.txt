cmake_minimum_required(VERSION 3.16)
project(MWSRestAPI)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск компонентов Boost (system, thread, filesystem)
find_package(Boost REQUIRED COMPONENTS system thread filesystem locale)

# Поиск библиотеки PostgreSQL
find_package(PostgreSQL REQUIRED)

# Поиск библиотеки OpenSSL
find_package(OpenSSL REQUIRED)

include_directories(nlohmann)
## Путь к заголовкам
#include_directories("/usr/local/include")



file(GLOB SOURCES
        include/*.h
        calc/*.h
        calc/*.cpp
        DB/*.h
        DB/*.cpp
        DelphiAdapt/*.h
        DelphiAdapt/*.cpp
        F1DevExpress/*.h
        F1DevExpress/*.cpp
        Form/*.h
        Form/*.cpp
        Form/FormDgvr/*.h
        Form/FormDgvr/*.cpp
        Form/FormDraw/*.h
        Form/FormDraw/*.cpp
        Form/FormFinans/*.h
        Form/FormFinans/*.cpp
        GlobalVars/*.h
        GlobalVars/*.cpp
        MWSComponents/*.h
        MWSComponents/*.cpp
        nlohmann/*.h
        nlohmann/*.cpp
        Pokazateli/*.h
        Pokazateli/*.cpp
        RS/*.h
        RS/*.cpp
        system/*.h
        system/*.cpp
        SystFuncs/*.h
        SystFuncs/*.cpp
        unMWSPageSetup/*.h
        unMWSPageSetup/*.cpp
        FedBdgtPeremesch/*.h
        FedBdgtPeremesch/*.cpp
)

# Добавление исполняемого файла проекта
add_executable(${PROJECT_NAME} main.cpp ${SOURCES}
        DB/unDefDBconnect.cpp
        DB/unDefDBconnect.h
        Form/TrcrdFM.h
        Form/FormDraw/Tbov67STRUCT.h
        DB/DB_SystSettings.cpp
        DB/DB_SystSettings.h
        Form/FormDraw/Tbov15STRUCT.h
        Form/FormDraw/TT_ValForPeriod.h
        unWebModuleUnit.cpp
        Form/FormDraw/FormGF_Logic.cpp
        RS/RS_Spreadsheet.cpp
        RS/RS_Spreadsheet.h
        unHttpRequestParser.cpp
        unHttpRequestParser.h
        unHttpRequestParser.h
        unHttpRequestParser.cpp
        RS/RS_Document.h
        RS/RS_Document.h
        RS/RS_Document.cpp
        RS/RS_Document.cpp
        DB/unAPIDocum.h
        DB/unAPIDocum.cpp
        StringTableSession.cpp
        StringTableSession.h
        StringTableSession.h
        StringTableSession.cpp
        unServerConst.h
        RS/RS_SQLCommand.h
        RS/RS_SQLCommand.cpp
        connectPQdb.cpp
        connectPQdb.cpp
        RS/RS_Auth.cpp
        RS/RS_Auth.h
        RS/RS_Auth.h
        Pokazateli/FunCell.cpp
        Pokazateli/FunCell.h
        RS/RS_N3Table.cpp
        RS/RS_N3Table.h
        DB/DB_Dinizm.cpp
        DB/DB_Dinizm.h
        DB/buildversion.cpp
        DB/buildversion.h
        FedBdgtPeremesch/UpdBudgPro2.cpp
        FedBdgtPeremesch/UpdBudgPro2.h
        RS/RS_Budget.cpp
        RS/RS_Budget.h
        DB/DB_Budget.h
        DB/DB_Budget.cpp
        SystFuncs/unODataStatements.h
        initialization.cpp
        initialization.h
        system/Base91Converter.cpp
        system/Base91Converter.h
        system/TracerManager.cpp
        DB/DB_Request_state.cpp
        DB/DB_Request_state.h
        DB/DB_TraceConfig.cpp
        DB/DB_TraceConfig.h
        OtherServices/Common.cpp
        OtherServices/Common.h
        OtherServices/UserManager.cpp
        OtherServices/UserManager.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/f1flexcel/linux/lib/libF1FlexCel.so ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/f1flexcel/linux/lib/libflexskia.so.5 ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE libzip.so ${CMAKE_DL_LIBS})

## Линковка библиотек OpenTelemetry
#target_link_libraries(${PROJECT_NAME} PRIVATE /usr/local/lib/libopentelemetry_trace.so /usr/local/lib/libopentelemetry_metrics.so /usr/local/lib/libopentelemetry_logs.so)


target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PUBLIC calc
        PUBLIC DB
        PUBLIC DelphiAdapt
        PUBLIC Form
        PUBLIC Form/FormDgvr
        PUBLIC Form/FormDraw
        PUBLIC Form/FormFinans
        PUBLIC MWSComponents
        PUBLIC nlohmann
        PUBLIC Pokazateli
        PUBLIC RS
        PUBLIC system
        PUBLIC SystFuncs
        PUBLIC unMWSPageSetup
        PUBLIC F1DevExpress
        PUBLIC GlobalVars
        PUBLIC FedBdgtPeremesch
)

# Включение директорий и линковка с Boost
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

# Линковка с PostgreSQL
target_include_directories(${PROJECT_NAME} PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES})

# Линковка с OpenSSL
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)


# Подключение библиотеки libcurl
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)

# Специфичные флаги для компиляторов MinGW
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Флаги для профайлера
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Проверка, установлены ли необходимые компоненты
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

if(NOT PostgreSQL_FOUND)
    message(FATAL_ERROR "PostgreSQL not found")
endif()

if(NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found")
endif()
