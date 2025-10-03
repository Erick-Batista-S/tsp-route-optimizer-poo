#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include "design/Point.h"
#include "design/Graph.h"
#include "design/Route.h"
#include "design/TSPAlgorithm.h"
#include "design/TSPException.h"

class TSPDemo {
private:
    Graph graph;
    std::vector<std::unique_ptr<TSPAlgorithm>> algorithms;
    
public:
    TSPDemo() {
        // Inicializar algoritmos
        algorithms.push_back(std::make_unique<NearestNeighborTSP>());
        algorithms.push_back(std::make_unique<GeneticTSP>(50, 100, 0.01)); // População 50, 100 gerações, 1% mutação
        algorithms.push_back(std::make_unique<TwoOptTSP>(1000)); // Máximo 1000 iterações
    }
    
    void run() {
        printHeader();
        
        try {
            // Teste 1: Pequeno conjunto de cidades
            std::cout << "\n=== TESTE 1: Pequeno Conjunto (5 cidades) ===\n";
            testSmallProblem();
            
            // Teste 2: Conjunto médio
            std::cout << "\n=== TESTE 2: Conjunto Médio (8 cidades) ===\n";
            testMediumProblem();
            
            // Teste 3: Demonstração de funcionalidades
            std::cout << "\n=== TESTE 3: Demonstração de Funcionalidades ===\n";
            demonstrateFeatures();
            
            // Teste 4: Comparação de performance
            std::cout << "\n=== TESTE 4: Comparação de Performance ===\n";
            performanceComparison();
            
        } catch (const TSPException& e) {
            std::cerr << "Erro TSP: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << std::endl;
        }
        
        printFooter();
    }
    
private:
    void printHeader() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    TSP ROUTE OPTIMIZER                      ║\n";
        std::cout << "║                  Etapa 2 - CLI Funcional                    ║\n";
        std::cout << "║                                                              ║\n";
        std::cout << "║  Demonstração dos conceitos de POO:                         ║\n";
        std::cout << "║  ✓ Herança e Polimorfismo (TSPAlgorithm)                   ║\n";
        std::cout << "║  ✓ Composição (Graph possui Points)                        ║\n";
        std::cout << "║  ✓ Encapsulamento (classes com interfaces claras)          ║\n";
        std::cout << "║  ✓ Tratamento de Exceções (TSPException hierarchy)         ║\n";
        std::cout << "║  ✓ STL e Smart Pointers (vector, unique_ptr, map)          ║\n";
        std::cout << "║  ✓ Sobrecarga de Operadores (Point, Route)                 ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }
    
    void printFooter() {
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        TESTES CONCLUÍDOS                    ║\n";
        std::cout << "║                                                              ║\n";
        std::cout << "║  Todos os conceitos POO foram demonstrados com sucesso!     ║\n";
        std::cout << "║  O sistema está pronto para a interface gráfica (Etapa 3)   ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }
    
    void testSmallProblem() {
        // Criar problema pequeno (5 cidades brasileiras)
        Graph testGraph;
        testGraph.addPoint(Point(0, 0, "São Paulo"));
        testGraph.addPoint(Point(3, 4, "Rio de Janeiro"));
        testGraph.addPoint(Point(-2, 3, "Belo Horizonte"));
        testGraph.addPoint(Point(2, -3, "Curitiba"));
        testGraph.addPoint(Point(-1, -2, "Porto Alegre"));
        
        std::cout << "Grafo criado: " << testGraph << std::endl;
        std::cout << "Número de cidades: " << testGraph.size() << std::endl;
        
        runAllAlgorithms(testGraph);
    }
    
    void testMediumProblem() {
        // Criar problema médio (8 cidades)
        Graph testGraph;
        testGraph.addPoint(Point(0, 0, "São Paulo"));
        testGraph.addPoint(Point(3, 4, "Rio de Janeiro"));
        testGraph.addPoint(Point(-2, 3, "Belo Horizonte"));
        testGraph.addPoint(Point(2, -3, "Curitiba"));
        testGraph.addPoint(Point(-1, -2, "Porto Alegre"));
        testGraph.addPoint(Point(5, 1, "Salvador"));
        testGraph.addPoint(Point(-3, 6, "Brasília"));
        testGraph.addPoint(Point(1, 7, "Recife"));
        
        std::cout << "Grafo criado: " << testGraph << std::endl;
        std::cout << "Número de cidades: " << testGraph.size() << std::endl;
        
        runAllAlgorithms(testGraph);
    }
    
    void demonstrateFeatures() {
        std::cout << "\n--- Demonstração de Funcionalidades POO ---\n";
        
        // 1. Encapsulamento e Getters/Setters
        std::cout << "\n1. Encapsulamento - Criação e manipulação de pontos:\n";
        Point p1(10, 20, "Teste1");
        std::cout << "   Ponto criado: " << p1 << std::endl;
        std::cout << "   Coordenadas: (" << p1.getX() << ", " << p1.getY() << ")" << std::endl;
        
        p1.setX(15);
        p1.setY(25);
        std::cout << "   Após modificação: " << p1 << std::endl;
        
        // 2. Sobrecarga de operadores
        std::cout << "\n2. Sobrecarga de Operadores:\n";
        Point p2(5, 5, "Teste2");
        Point p3 = p1 + p2;
        std::cout << "   p1 + p2 = " << p3 << std::endl;
        std::cout << "   p1 == p2? " << (p1 == p2 ? "Sim" : "Não") << std::endl;
        std::cout << "   p1 < p2? " << (p1 < p2 ? "Sim" : "Não") << std::endl;
        std::cout << "   Distância entre p1 e p2: " << p1.distanceTo(p2) << std::endl;
        
        // 3. Composição (Graph possui Points)
        std::cout << "\n3. Composição - Graph gerencia Points com smart pointers:\n";
        Graph testGraph;
        testGraph.addPoint(p1);
        testGraph.addPoint(p2);
        std::cout << "   " << testGraph << std::endl;
        
        // 4. Tratamento de exceções
        std::cout << "\n4. Tratamento de Exceções:\n";
        try {
            testGraph.addPoint(p1); // Tentar adicionar ponto duplicado
        } catch (const DuplicatePointException& e) {
            std::cout << "   Exceção capturada: " << e.what() << std::endl;
        }
        
        try {
            testGraph.getPoint(999); // Índice inválido
        } catch (const InvalidIndexException& e) {
            std::cout << "   Exceção capturada: " << e.what() << std::endl;
        }
        
        // 5. Polimorfismo dinâmico
        std::cout << "\n5. Polimorfismo - Algoritmos intercambiáveis:\n";
        for (const auto& algorithm : algorithms) {
            std::cout << "   Algoritmo: " << algorithm->getName() 
                      << " - " << algorithm->getDescription() << std::endl;
        }
    }
    
    void performanceComparison() {
        // Criar grafo maior para comparação
        Graph largeGraph;
        std::vector<std::string> cities = {
            "São Paulo", "Rio de Janeiro", "Belo Horizonte", "Salvador", 
            "Brasília", "Fortaleza", "Manaus", "Curitiba", "Recife", 
            "Porto Alegre", "Belém", "Goiânia"
        };
        
        // Gerar coordenadas pseudo-aleatórias mas determinísticas
        for (size_t i = 0; i < cities.size(); ++i) {
            double x = (i * 17 + 23) % 100 - 50; // -50 a 49
            double y = (i * 13 + 37) % 100 - 50; // -50 a 49
            largeGraph.addPoint(Point(x, y, cities[i]));
        }
        
        std::cout << "Testando performance com " << largeGraph.size() << " cidades:\n";
        std::cout << "Cidades: ";
        for (size_t i = 0; i < largeGraph.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << largeGraph.getPoint(i).getName();
        }
        std::cout << std::endl;
        
        runAllAlgorithms(largeGraph);
        
        // Comparação final
        std::cout << "\n--- RESUMO DE PERFORMANCE ---\n";
        std::cout << std::setw(20) << "Algoritmo" 
                  << std::setw(15) << "Tempo (ms)" 
                  << std::setw(15) << "Qualidade" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
    }
    
    void runAllAlgorithms(const Graph& graph) {
        std::cout << "\n--- Executando todos os algoritmos ---\n";
        
        std::vector<std::pair<std::string, Route>> results;
        
        for (const auto& algorithm : algorithms) {
            try {
                std::cout << "\n🔄 Executando: " << algorithm->getName() << std::endl;
                std::cout << "   Descrição: " << algorithm->getDescription() << std::endl;
                
                Route result = algorithm->solve(graph);
                results.emplace_back(algorithm->getName(), result);
                
                std::cout << "   ✅ Resultado: " << result << std::endl;
                std::cout << "   ⏱️  Tempo de execução: " << algorithm->getLastExecutionTime() << "ms" << std::endl;
                
            } catch (const TSPException& e) {
                std::cout << "   ❌ Erro: " << e.what() << std::endl;
            }
        }
        
        // Comparar resultados
        if (!results.empty()) {
            std::cout << "\n--- Comparação de Resultados ---\n";
            
            // Encontrar melhor resultado
            auto best = std::min_element(results.begin(), results.end(),
                [](const auto& a, const auto& b) {
                    return a.second.getTotalDistance() < b.second.getTotalDistance();
                });
            
            std::cout << std::setw(20) << "Algoritmo" 
                      << std::setw(15) << "Distância" 
                      << std::setw(10) << "Relativo" << std::endl;
            std::cout << std::string(45, '-') << std::endl;
            
            for (const auto& [name, route] : results) {
                double relative = (route.getTotalDistance() / best->second.getTotalDistance() - 1.0) * 100;
                std::cout << std::setw(20) << name
                          << std::setw(15) << std::fixed << std::setprecision(2) << route.getTotalDistance()
                          << std::setw(9) << std::fixed << std::setprecision(1) << relative << "%"
                          << std::endl;
            }
            
            std::cout << "\n🏆 Melhor resultado: " << best->first 
                      << " (distância: " << best->second.getTotalDistance() << ")" << std::endl;
        }
    }
};

int main() {
    std::cout << "TSP Route Optimizer - Etapa 2 CLI\n";
    std::cout << "Desenvolvido por: Erick Batista da Silva\n";
    std::cout << "Disciplina: Programação Orientada a Objetos (C++)\n\n";
    
    TSPDemo demo;
    demo.run();
    
    return 0;
}