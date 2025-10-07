cmake_minimum_required(VERSION 3.16)

# Projeto TSP Route Optimizer
project(tsp_route_optimizer 
    VERSION 3.0.0
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
    find_package(Qt5 COMPONENTS Core Widgets Gui QUIET)
    if(Qt5_FOUND)
        message(STATUS "Using Qt5")
        set(QT_VERSION 5)
    else()
        message(STATUS "Qt not found - CLI only")
    endif()
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

# Arquivos fonte da implementação (removidos - usando implementações inline)

# ========================================
# ETAPA 2: Executável CLI funcional  
# ========================================

# Executável CLI principal
add_executable(tsp_cli 
    src/main_final.cpp
)

target_include_directories(tsp_cli PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/design
    ${CMAKE_SOURCE_DIR}/src
)

# Configurar propriedades do executável
set_target_properties(tsp_cli PROPERTIES
    OUTPUT_NAME "tsp_optimizer"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Target para executar testes CLI
add_custom_target(run_tests
    COMMAND ${CMAKE_BINARY_DIR}/bin/tsp_optimizer
    DEPENDS tsp_cli
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Executando demonstração CLI do TSP"
)

# ========================================
# ETAPA 3: GUI com Qt6
# ========================================

if(Qt6_FOUND)
    message(STATUS "Configurando GUI com Qt6...")
    
    # Habilitar MOC (Meta-Object Compiler)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    # Arquivos da GUI
    set(GUI_HEADERS
        src/gui/MainWindow.h
        src/gui/GraphView.h
        src/gui/RouteVisualizer.h
    )
    
    set(GUI_SOURCES
        src/gui/MainWindow.cpp
        src/gui/GraphView.cpp
        src/gui/RouteVisualizer.cpp
        src/gui/main_gui.cpp
    )
    
    # Executável GUI
    add_executable(tsp_gui 
        ${GUI_SOURCES}
        ${GUI_HEADERS}
    )
    
    # Linkar com Qt6
    target_link_libraries(tsp_gui 
        Qt6::Core 
        Qt6::Widgets 
        Qt6::Gui
    )
    
    target_include_directories(tsp_gui PRIVATE 
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/design
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/gui
    )
    
    # Configurar propriedades do executável GUI
    set_target_properties(tsp_gui PROPERTIES
        OUTPUT_NAME "tsp_gui"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    
    # Target para executar GUI
    add_custom_target(run_gui
        COMMAND ${CMAKE_BINARY_DIR}/bin/tsp_gui
        DEPENDS tsp_gui
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Executando aplicação GUI do TSP"
    )
    
    message(STATUS "GUI configurada com sucesso!")
    
elseif(Qt5_FOUND)
    message(STATUS "Qt5 encontrado mas Qt6 preferido - implementar se necessário")
else()
    message(WARNING "Qt não encontrado - apenas CLI disponível")
endif()

# Instalação
install(TARGETS tsp_cli DESTINATION bin)
if(Qt6_FOUND)
    install(TARGETS tsp_gui DESTINATION bin)
endif()

# Resumo da configuração
message(STATUS "")
message(STATUS "=== Configuração do Projeto TSP ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
if(Qt6_FOUND OR Qt5_FOUND)
    message(STATUS "Qt Version: ${QT_VERSION}")
endif()
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "===================================")
message(STATUS "")