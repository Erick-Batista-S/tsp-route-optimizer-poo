# TSP Route Optimizer - C++ POO Project

**Projeto**: Otimização de Rotas (Problema do Caixeiro Viajante)  
**Disciplina**: Programação Orientada a Objetos (C++)  
**Universidade**: UFPB - Centro de Informática  
**Autor**: Erick Batista da Silva  
**Matrícula**: 20240010967

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
- [x] Diagrama UML de classes
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
