# 🚀 TSP Route Optimizer - Etapa 2 CLI

**Traveling Salesman Problem Route Optimizer**  
**Disciplina:** Programação Orientada a Objetos (C++)  
**Desenvolvedor:** Erick Batista da Silva  
**Instituição:** UFPB - Centro de Informática  

## 📋 Status do Projeto

✅ **Etapa 1** - Design das Classes (Concluída)  
✅ **Etapa 2** - CLI Funcional (Concluída)  
🔄 **Etapa 3** - Interface Gráfica (Próxima)  

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

**🎉 Etapa 2 concluída com sucesso!**  
**Todos os requisitos POO foram implementados e demonstrados.**

## 📝 Descrição

Aplicação completa que resolve variações do Problema do Caixeiro Viajante (TSP) usando conceitos avançados de Programação Orientada a Objetos em C++. O projeto implementa múltiplos algoritmos de otimização com interface gráfica interativa.

## 🎯 Objetivos

- Demonstrar aplicação prática dos principais conceitos de POO
- Implementar hierarquia de algoritmos usando polimorfismo
- Utilizar boas práticas modernas de C++ (smart pointers, STL, exceções)
- Criar interface gráfica funcional com Qt

## 🏗️ Arquitetura

### Classes Principais

- **`Point`**: Representa pontos/cidades geográficas
- **`Graph`**: Grafo completo com pontos e distâncias
- **`Route`**: Sequência de pontos visitados (solução)
- **`TSPAlgorithm`**: Interface abstrata para algoritmos
- **Algoritmos**: NearestNeighbor, Genetic, TwoOpt

### Conceitos POO Demonstrados

✅ **Abstração & Encapsulamento**: Interfaces claras, dados privados  
✅ **Herança & Polimorfismo**: Hierarquia de algoritmos  
✅ **Composição**: Graph contém Points, Route contém sequência  
✅ **Templates & STL**: Containers modernos, algoritmos genéricos  
✅ **Smart Pointers**: Gerenciamento automático de memória  
✅ **Exceções**: Tratamento robusto de erros  
✅ **Sobrecarga**: Operadores para Route (<, +, ==, <<)

## 📋 Etapas de Desenvolvimento

### ✅ Etapa 1 - Design & Headers (25/09/2025)
- [x] Headers das classes principais
- [x] Diagrama UML de classes (SVG + PlantUML)
- [x] CMakeLists.txt funcional
- [x] Relatório de arquitetura

### 🔄 Etapa 2 - CLI Funcional (02/10/2025)
- [ ] Implementação das classes core
- [ ] Interface CLI para testes
- [ ] Algoritmos básicos funcionais

### 🔄 Etapa 3 - GUI Completa (06/10/2025)
- [ ] Interface gráfica Qt
- [ ] Visualização interativa
- [ ] Relatório final + vídeo

## 🛠️ Como Compilar

### Pré-requisitos
```bash
# Ubuntu/Debian
sudo apt install cmake qt6-base-dev qtcreator

# Fedora
sudo dnf install cmake qt6-qtbase-devel

# Arch Linux
sudo pacman -S cmake qt6-base
```

### Compilação
```bash
mkdir build && cd build
cmake ..
make

# Executar CLI
./tsp_cli --help

# Executar GUI (Etapa 3)
./tsp_gui
```

## 📊 Funcionalidades

### CLI (Etapa 2)
- Carregamento de pontos de arquivo
- Execução de diferentes algoritmos
- Comparação de performance
- Exportação de resultados

### GUI (Etapa 3)
- Visualização interativa de mapas
- Adição de pontos por clique
- Seleção de algoritmos
- Animação da otimização
- Métricas em tempo real

## 📁 Estrutura do Projeto

```
tsp-route-optimizer-poo/
├── design/           # Headers das classes (Etapa 1)
├── src/             # Implementações
├── docs/            # Documentação e UML
├── tests/           # Testes unitários
├── data/            # Arquivos de exemplo
└── CMakeLists.txt   # Configuração de build
```

## 🏆 Requisitos POO Atendidos

Consulte `docs/requirements_mapping.md` para mapeamento detalhado de cada conceito implementado.

## 📄 Licença

MIT License - Consulte `LICENSE` para detalhes.
