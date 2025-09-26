# Relatório de Arquitetura - TSP Route Optimizer

## Justificativa da Arquitetura

### 1. Visão Geral da Arquitetura

O TSP Route Optimizer foi projetado seguindo princípios sólidos de Programação Orientada a Objetos, priorizando **composição sobre herança**, **baixo acoplamento** e **alta coesão**. A arquitetura modular permite extensibilidade para novos algoritmos e fácil manutenção.

### 2. Decisões Arquiteturais Principais

#### 2.1 Strategy Pattern para Algoritmos TSP

**Decisão**: Implementar hierarquia de classes com `TSPAlgorithm` como base abstrata.

**Justificativa**:
- **Polimorfismo**: Permite trocar algoritmos em tempo de execução
- **Extensibilidade**: Novos algoritmos podem ser adicionados sem modificar código existente
- **Testabilidade**: Cada algoritmo pode ser testado isoladamente
- **Principio Aberto/Fechado**: Aberto para extensão, fechado para modificação

```cpp
// Exemplo de uso polimórfico
std::unique_ptr<TSPAlgorithm> algorithm = 
    std::make_unique<GeneticTSP>(100, 500); // 100 indivíduos, 500 gerações
Route best_route = algorithm->solve(graph);
```

#### 2.2 Composição em Graph e Route

**Decisão**: `Graph` contém `Point` objects, `Route` referencia pontos por ID.

**Justificativa**:
- **Ownership Claro**: Graph é dono dos Points (RAII com unique_ptr)
- **Performance**: Route mantém apenas IDs (lightweight)
- **Consistência**: Mudanças em Point se refletem automaticamente
- **Memória Segura**: unique_ptr evita vazamentos e double-free

```cpp
class Graph {
    std::vector<std::unique_ptr<Point>> points_; // Ownership forte
    std::map<std::pair<int,int>, double> distances_; // Cache otimizado
};

class Route {
    std::vector<int> point_sequence_; // Apenas referencias por ID
};
```

#### 2.3 Cache de Distâncias

**Decisão**: Usar `std::map<std::pair<int,int>, double>` para cache de distâncias.

**Justificativa**:
- **Performance**: Evita recalcular distâncias euclidianas repetidamente
- **Complexidade**: O(log n) para busca vs O(1) cálculo trigonométrico
- **Memória**: Trade-off aceitável para datasets típicos de TSP
- **Simetria**: Automaticamente trata distância(A,B) = distância(B,A)

#### 2.4 Hierarquia de Exceções

**Decisão**: Criar hierarquia específica herdando de `std::exception`.

**Justificativa**:
- **Granularidade**: Diferentes tipos de erro podem ser tratados especificamente
- **Debugging**: Mensagens de erro mais informativas
- **Robustez**: Evita crashes por estados inválidos
- **Padrão C++**: Segue convenções da STL

### 3. Conceitos POO Aplicados

#### 3.1 Encapsulamento
- **Point**: Coordenadas privadas com validação nos setters
- **Graph**: Detalhes de implementação do cache ocultos
- **Route**: Sequência interna protegida, acesso via métodos públicos

#### 3.2 Herança e Polimorfismo
- **TSPAlgorithm**: Hierarquia bem definida com interface comum
- **Exceções**: Especialização por tipo de erro
- **Virtual Dispatch**: Algoritmo correto escolhido em tempo de execução

#### 3.3 Composição e Agregação
- **Graph-Point**: Composição (ownership forte)
- **Route-Graph**: Agregação (usa para calcular distâncias)
- **Algorithm-Graph**: Dependência (recebe como parâmetro)

#### 3.4 Templates e STL
- **Smart Pointers**: `std::unique_ptr` para gestão automática
- **Containers**: `std::vector`, `std::map` para estruturas eficientes
- **Algorithms**: STL algorithms quando apropriado (sort, min_element)

### 4. Padrões de Design Implementados

#### 4.1 Strategy Pattern
Permite escolher algoritmo TSP dinamicamente, facilitando comparação de performance e experimentação.

#### 4.2 Template Method Pattern
Base TSPAlgorithm pode implementar passos comuns (validação, logging) deixando apenas o core algoritmo para subclasses.

#### 4.3 RAII (Resource Acquisition Is Initialization)
Smart pointers garantem liberação automática de recursos, evitando vazamentos de memória.

### 5. Benefícios da Arquitetura

#### 5.1 Manutenibilidade
- **Separação clara de responsabilidades**
- **Baixo acoplamento entre classes**
- **Interface bem definida para cada componente**

#### 5.2 Extensibilidade  
- **Novos algoritmos**: Apenas herdar de TSPAlgorithm
- **Novos tipos de ponto**: Estender Point ou criar interface
- **Novas optimizações**: Adicionar à Route sem afetar Graph

#### 5.3 Testabilidade
- **Unidades isoladas**: Cada classe pode ser testada independentemente
- **Mocks fáceis**: Interfaces permitem substituição para testes
- **Casos específicos**: Hierarquia de exceções facilita teste de erros

#### 5.4 Performance
- **Cache inteligente**: Evita cálculos desnecessários
- **Memory layout**: Containers STL otimizados
- **Smart pointers**: Overhead mínimo comparado a raw pointers

### 6. Considerações para Próximas Etapas

#### Etapa 2 (CLI)
- **Parser**: Adicionar classe para ler diferentes formatos (CSV, TSP)
- **Output**: Formatação flexível de resultados
- **Config**: Sistema de configuração para parâmetros de algoritmos

#### Etapa 3 (GUI)  
- **MVC Pattern**: Separar lógica de apresentação
- **Qt Integration**: Adapter pattern para conectar com widgets
- **Visualização**: Classes específicas para renderização gráfica

### 7. Conclusão

A arquitetura proposta atende plenamente aos requisitos de Programação Orientada a Objetos, demonstrando todos os conceitos fundamentais de forma prática e bem fundamentada. A escolha por **composição sobre herança**, **smart pointers** e **padrões de design** estabelecidos resulta em código robusto, extensível e maintível.

O design modular facilita o desenvolvimento incremental nas próximas etapas, permitindo adicionar funcionalidades sem comprometer a estrutura existente. A hierarquia de algoritmos em particular oferece excelente base para experimentação e comparação de diferentes abordagens ao problema TSP.

---

**Data**: Setembro 2024  
**Autor**: Erick Batista da Silva  
**Disciplina**: Programação Orientada a Objetos (POO)  
**Professor**: Bidu (bidu@ci.ufpb.br)