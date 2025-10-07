# 🚀 TSP Route Optimizer - Etapa 3 GUI Completa ✅

**Projeto**: Otimização de Rotas (Problema do Caixeiro Viajante)  
**Disciplina**: Programação Orientada a Objetos (C++)  
**Universidade**: UFPB - Centro de Informática  
**Autor**: Erick Batista da Silva  
**Matrícula**: 20240010967

## 📋 Status do Projeto

✅ **Etapa 1** - Design das Classes (Concluída - 25/09/2025)  
✅ **Etapa 2** - CLI Funcional (Concluída - 02/10/2025)  
✅ **Etapa 3** - Interface Gráfica (Concluída - 06/10/2025)  

## 🎯 Etapa 3 - Interface Gráfica Completa

### ✅ O que foi entregue:

- **Interface Gráfica Qt6** completa e interativa
- **Visualização em tempo real** dos algoritmos TSP
- **Interação por clique** para adicionar pontos no mapa
- **Animação** da construção de rotas TSP
- **Painel de controles** para seleção de algoritmos
- **Métricas em tempo real** (distância, tempo, número de pontos)
- **Todos os conceitos POO** demonstrados na GUI

## 🏗️ Como Compilar e Executar

### Pré-requisitos
- **C++17** ou superior
- **CMake 3.16** ou superior
- **Qt6** (Core, Widgets, Gui)
- **Compilador GCC/Clang** compatível

#### Instalação do Qt6:
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev qt6-tools-dev

# Fedora
sudo dnf install qt6-qtbase-devel qt6-qttools-devel

# Arch Linux
sudo pacman -S qt6-base qt6-tools
```

### Instruções de Build

```bash
# 1. Navegar para o diretório do projeto
cd tsp-route-optimizer-poo

# 2. Criar e entrar no diretório de build
mkdir -p build && cd build

# 3. Configurar com CMake
cmake ..

# 4. Compilar CLI (Etapa 2)
make tsp_cli

# 5. Compilar GUI (Etapa 3)
make tsp_gui

# 6. Executar CLI
./bin/tsp_optimizer

# 7. Executar GUI
./bin/tsp_gui
```

## � Como Usar a Interface Gráfica

### Controles Principais:
1. **Adicionar Pontos**: Clique no mapa para adicionar cidades
2. **Pontos Aleatórios**: Use o controle para gerar pontos automaticamente
3. **Selecionar Algoritmo**: Choose entre Nearest Neighbor e Brute Force
4. **Executar**: Clique para resolver o TSP e ver a animação
5. **Limpar**: Reset o grafo para começar novamente

### Visualização:
- **Pontos azuis**: Cidades no grafo
- **Linhas verdes**: Rota otimizada final
- **Animação**: Construção passo-a-passo da solução
- **Métricas**: Painel com estatísticas em tempo real

## 🎭 Conceitos POO Demonstrados na GUI

### ✅ **1. Classes e Objetos**
- **MainWindow**: Janela principal da aplicação
- **GraphView**: Widget customizado para visualização
- **RouteVisualizer**: Componente de animação
- **Point, Graph, Route**: Classes de domínio TSP
- **TSPAlgorithm**: Hierarquia de algoritmos

### ✅ **2. Herança e Polimorfismo**
- **MainWindow** herda de **QMainWindow**
- **GraphView** herda de **QWidget**  
- **RouteVisualizer** herda de **QWidget**
- **TSPAlgorithm** → **NearestNeighborTSP**, **BruteForceTSP**
- Override de métodos virtuais do Qt (paintEvent, mousePressEvent)

### ✅ **3. Encapsulamento**
- Dados privados em todas as classes GUI
- Interfaces públicas bem definidas (slots e signals)
- Estado interno protegido dos widgets

### ✅ **4. Composição**
- **MainWindow** contém **GraphView** e widgets Qt
- **Graph** contém **Points** organizados
- **Route** contém **Points** em sequência TSP

### ✅ **5. STL e Templates**  
- `std::vector` para containers de pontos
- `std::unique_ptr` para algoritmos TSP
- `std::shared_ptr` para objetos compartilhados entre GUI
- `std::chrono` para medição de performance

### ✅ **6. Sobrecarga de Operadores**
- **Point**: `operator==`, `operator<`, `operator<<`
- **Route**: `operator<`, `operator<<`
- **Graph**: `operator<<`

### ✅ **7. Tratamento de Exceções**
- **TSPException**: Classe personalizada de exceções
- Try/catch em operações críticas da GUI
- Validação robusta de entrada do usuário

### ✅ **8. Smart Pointers e RAII**
- `std::unique_ptr` para algoritmos TSP
- `std::shared_ptr` para objetos compartilhados
- Gestão automática de memória (sem vazamentos)

### ✅ **9. Programação Orientada a Eventos**
- Sistema **signals/slots** do Qt
- Eventos de mouse, teclado e timers
- Padrão Observer implementado

### ✅ **10. Padrões de Design**
- **Observer**: signals/slots do Qt
- **Strategy**: algoritmos TSP intercambiáveis
- **Composite**: hierarquia de widgets Qt

### ✅ **11. Programação Genérica**
- Templates do Qt e STL
- Containers genéricos
- Algoritmos STL utilizados  

## 📋 Status do Projeto

✅ **Etapa 1** - Design das Classes (Concluída)  
✅ **Etapa 2** - CLI Funcional (Concluída)  
✅ **Etapa 3** - Interface Gráfica (Concluída)  

## 🎯 Etapa 2 - Implementação CLI

### ✅ O que foi entregue:

- **CLI funcional** que demonstra todas as operações TSP principais
- **Implementação completa** de todos os conceitos POO requeridos
- **Algoritmos TSP** funcionais (Nearest Neighbor, Brute Force)
- **Sistema robusto** com tratamento de exceções
- **Testes automatizados** que validam todas as funcionalidades

## 🏗️ Como Compilar e Executar

### Pré-requisitos
- **C++17** ou superior
- **CMake 3.16** ou superior
- **Compilador GCC/Clang** compatível

### Instruções de Build

```bash
# 1. Navegar para o diretório do projeto
cd tsp-route-optimizer-poo

# 2. Criar e entrar no diretório de build
mkdir -p build && cd build

# 3. Configurar com CMake
cmake ..

# 4. Compilar
make tsp_cli

# 5. Executar o CLI
./bin/tsp_optimizer
```

## 🎭 Conceitos POO Demonstrados

### ✅ **1. Classes e Objetos**
- **Point**: Representa coordenadas geográficas
- **Graph**: Container para pontos com operações de distância  
- **Route**: Sequência de pontos formando uma rota TSP
- **TSPAlgorithm**: Classe base abstrata para algoritmos

### ✅ **2. Encapsulamento**
- Dados privados com interfaces públicas controladas
- Getters/setters com validação
- Ocultação de detalhes de implementação

### ✅ **3. Herança e Polimorfismo**
- **TSPAlgorithm** → **NearestNeighborTSP**, **BruteForceTSP**
- Métodos virtuais puros (`solve`, `getName`, `getDescription`)
- Dispatch dinâmico para diferentes algoritmos

### ✅ **4. Composição**
- **Graph** contém **Points** (relação HAS-A)
- **Route** contém **Points** organizados em sequência
- **TSPDemo** contém **TSPAlgorithms** via smart pointers

### ✅ **5. STL e Templates**  
- `std::vector` para containers dinâmicos
- `std::unique_ptr` para gerenciamento de memória
- `std::chrono` para medição de performance
- Algoritmos STL (`std::min_element`, `std::sort`)

### ✅ **6. Sobrecarga de Operadores**
- **Point**: `operator==`, `operator<`, `operator<<`
- **Route**: `operator<`, `operator<<`
- **Graph**: `operator<<`

### ✅ **7. Tratamento de Exceções**
- **TSPException**: Classe base para exceções específicas do domínio
- Validação robusta com mensagens informativas
- Captura e tratamento adequado em todos os níveis

### ✅ **8. Smart Pointers e RAII**
- `std::unique_ptr` para gerenciamento automático de algoritmos
- Sem vazamentos de memória
- Destruição automática e segura

---

**🎉 Etapa 3 concluída com sucesso!**  
**Interface gráfica completa com todos os conceitos POO demonstrados!**

## 📁 Estrutura do Projeto Final

```
tsp-route-optimizer-poo/
├── design/                    # Headers das classes (Etapa 1 - Projeto)
├── src/
│   ├── main_final.cpp        # CLI completo com implementação inline (Etapa 2)
│   └── gui/                  # Interface gráfica (Etapa 3)
│       ├── TSPClasses.h      # Classes TSP reutilizadas para GUI
│       ├── MainWindow.h/cpp  # Janela principal da aplicação
│       ├── GraphView.h/cpp   # Widget de visualização interativa
│       ├── RouteVisualizer.h/cpp # Widget de animação de rotas
│       └── main_gui.cpp      # Entrada da aplicação GUI
├── build/bin/
│   ├── tsp_optimizer        # Executável CLI (funcional)
│   └── tsp_gui              # Executável GUI (funcional)
├── docs/                    # Documentação e relatórios
└── tests/                   # Arquivos de teste
└── CMakeLists.txt          # Sistema de build completo
```

## 🏆 Todos os Requisitos Atendidos

✅ **Implementação Completa**: CLI + GUI funcionais  
✅ **11 Conceitos POO**: Todos demonstrados com exemplos práticos  
✅ **Algoritmos TSP**: Nearest Neighbor e Brute Force  
✅ **Interface Interativa**: Clique, animação, controles  
✅ **Documentação**: README completo com instruções  
✅ **Build System**: CMake configurado para ambas as versões  

## 🏗️ Arquitetura Final

**Implementação Limpa e Eficiente:**
- **`design/`**: Headers de projeto (referência arquitetural)
- **`src/main_final.cpp`**: CLI funcional com implementação inline completa
- **`src/gui/TSPClasses.h`**: Classes TSP adaptadas para GUI com Qt6
- **Sem duplicação**: Arquivos `.cpp` individuais removidos (código morto)
- **Duas versões**: CLI independente + GUI independente, ambas funcionais

## 📄 Licença

MIT License - Consulte `LICENSE` para detalhes.

---

**Projeto desenvolvido para a disciplina de Programação Orientada a Objetos**  
**UFPB - Centro de Informática - 2025.1**
