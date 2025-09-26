cmake_minimum_required(VERSION 3.16)

# Projeto TSP Route Optimizer
project(tsp_route_optimizer 
    VERSION 1.0.0
    DESCRIPTION "Traveling Salesman Problem Route Optimizer - POO Project"
    LANGUAGES CXX
)

# Configurações do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configurações de build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Flags de compilação
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Encontrar Qt6 (preferencial) ou Qt5
find_package(Qt6 COMPONENTS Core Widgets Gui QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION 6)
else()
    find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
    message(STATUS "Using Qt5")
    set(QT_VERSION 5)
endif()

# Diretórios de include
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/design)

# Arquivos de cabeçalho
set(HEADERS
    design/Point.h
    design/Graph.h
    design/Route.h
    design/TSPAlgorithm.h
    design/TSPException.h
)

# ========================================
# ETAPA 1: Apenas teste de compilação dos headers
# ========================================

# Teste de compilação dos headers (Etapa 1)
add_custom_target(check_headers
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++17 -I${CMAKE_SOURCE_DIR}/design -c ${CMAKE_SOURCE_DIR}/design/Point.h -o /dev/null
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++17 -I${CMAKE_SOURCE_DIR}/design -c ${CMAKE_SOURCE_DIR}/design/Graph.h -o /dev/null  
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++17 -I${CMAKE_SOURCE_DIR}/design -c ${CMAKE_SOURCE_DIR}/design/Route.h -o /dev/null
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++17 -I${CMAKE_SOURCE_DIR}/design -c ${CMAKE_SOURCE_DIR}/design/TSPAlgorithm.h -o /dev/null
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++17 -I${CMAKE_SOURCE_DIR}/design -c ${CMAKE_SOURCE_DIR}/design/TSPException.h -o /dev/null
    COMMENT "Verificando se todos os headers compilam corretamente..."
)

# ========================================
# ETAPAS 2 e 3: Implementações futuras
# ========================================

# Arquivos de implementação (serão criados na Etapa 2)
set(FUTURE_SOURCES
    src/Point.cpp
    src/Graph.cpp  
    src/Route.cpp
    src/TSPAlgorithm.cpp
    src/TSPException.cpp
)

# CLI principal (Etapa 2)
set(CLI_SOURCES
    src/cli/main_cli.cpp
)

# Arquivos GUI (Etapa 3)
set(GUI_SOURCES
    src/gui/MainWindow.cpp
    src/gui/GraphView.cpp
    src/gui/RouteVisualizer.cpp
    src/gui/main_gui.cpp
)

set(GUI_HEADERS
    src/gui/MainWindow.h
    src/gui/GraphView.h  
    src/gui/RouteVisualizer.h
)

# Targets condicionais para etapas futuras
if(EXISTS ${CMAKE_SOURCE_DIR}/src/Point.cpp)
    message(STATUS "Etapa 2: Encontrados arquivos de implementação")
    
    # Biblioteca principal (core TSP)
    add_library(tsp_core STATIC ${FUTURE_SOURCES} ${HEADERS})
    target_include_directories(tsp_core PUBLIC design src)
    
    # Aplicação CLI
    if(EXISTS ${CMAKE_SOURCE_DIR}/src/cli/main_cli.cpp)
        add_executable(tsp_cli ${CLI_SOURCES})
        target_link_libraries(tsp_cli tsp_core)
    endif()
    
    # Aplicação GUI (Etapa 3)
    if(EXISTS ${CMAKE_SOURCE_DIR}/src/gui/MainWindow.cpp AND Qt${QT_VERSION}_FOUND)
        message(STATUS "Etapa 3: Configurando GUI")
        
        if(QT_VERSION EQUAL 6)
            qt6_wrap_cpp(GUI_MOC_SOURCES ${GUI_HEADERS})
            add_executable(tsp_gui ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_MOC_SOURCES})
            target_link_libraries(tsp_gui tsp_core Qt6::Core Qt6::Widgets Qt6::Gui)
        else()
            qt5_wrap_cpp(GUI_MOC_SOURCES ${GUI_HEADERS})  
            add_executable(tsp_gui ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_MOC_SOURCES})
            target_link_libraries(tsp_gui tsp_core Qt5::Core Qt5::Widgets Qt5::Gui)
        endif()
        
        target_compile_definitions(tsp_gui PRIVATE QT_WIDGETS_LIB)
    endif()
else()
    message(STATUS "Etapa 1: Apenas headers disponíveis - use 'make check_headers' para verificar")
endif()

# Testes (opcional)
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Configurações de instalação (condicionais)
if(TARGET tsp_cli)
    install(TARGETS tsp_cli DESTINATION bin)
endif()
if(TARGET tsp_gui)
    install(TARGETS tsp_gui DESTINATION bin)
endif()

# Documentação com Doxygen (opcional)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    if(EXISTS ${DOXYGEN_IN})
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    endif()
endif()

# Resumo da configuração
message(STATUS "")
message(STATUS "=== Configuração do Projeto TSP ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt Version: ${QT_VERSION}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "===================================")
message(STATUS "")