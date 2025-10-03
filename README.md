# 🚀 TSP Route Optimizer - Etapa 2 CLI

**Projeto**: Otimização de Rotas (Problema do Caixeiro Viajante)  
**Disciplina**: Programação Orientada a Objetos (C++)  
**Universidade**: UFPB - Centro de Informática  
**Autor**: Erick Batista da Silva  
**Matrícula**: 20240010967

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

## 📁 Estrutura do Projeto

```
tsp-route-optimizer-poo/
├── design/           # Headers das classes (Etapa 1)
├── src/             # Implementações completas
│   └── main_final.cpp  # CLI funcional (Etapa 2)
├── docs/            # Documentação e UML
├── tests/           # Testes unitários
└── CMakeLists.txt   # Configuração de build
```

## 🏆 Requisitos POO Atendidos

Todos os 11 conceitos requeridos foram implementados e demonstrados na Etapa 2:
- ✅ Classes e objetos
- ✅ Encapsulamento  
- ✅ Herança e polimorfismo
- ✅ Composição
- ✅ STL e templates
- ✅ Smart pointers e RAII
- ✅ Sobrecarga de operadores
- ✅ Tratamento de exceções

## 📄 Licença

MIT License - Consulte `LICENSE` para detalhes.
