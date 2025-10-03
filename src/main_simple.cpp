#include <iostream>
#include <vector>
#include <memory>

#include "design/Point.h"
#include "design/Graph.h"
#include "design/Route.h"
#include "design/TSPAlgorithm.h"
#include "design/TSPException.h"

int main() {
    std::cout << "=== TSP Route Optimizer - Etapa 2 CLI ===\n";
    std::cout << "Desenvolvido por: Erick Batista da Silva\n\n";
    
    try {
        // Teste básico de criação de pontos
        std::cout << "1. Testando criação de pontos:\n";
        Point p1(0, 0, "São Paulo");
        Point p2(3, 4, "Rio de Janeiro");
        std::cout << "   " << p1 << std::endl;
        std::cout << "   " << p2 << std::endl;
        std::cout << "   Distância: " << p1.distanceTo(p2) << std::endl;
        
        // Teste de grafo
        std::cout << "\n2. Testando criação de grafo:\n";
        Graph graph;
        graph.addPoint(p1);
        graph.addPoint(p2);
        graph.addPoint(Point(-2, 3, "Belo Horizonte"));
        
        std::cout << "   Grafo criado com " << graph.size() << " pontos\n";
        for (size_t i = 0; i < graph.size(); ++i) {
            std::cout << "   " << graph.getPoint(i) << std::endl;
        }
        
        // Teste de rota
        std::cout << "\n3. Testando criação de rota:\n";
        std::vector<Point> routePoints = {p1, p2, Point(-2, 3, "Belo Horizonte")};
        Route route(routePoints);
        std::cout << "   " << route << std::endl;
        
        // Teste de algoritmo Nearest Neighbor
        std::cout << "\n4. Testando algoritmo Nearest Neighbor:\n";
        NearestNeighborTSP nn;
        std::cout << "   Algoritmo: " << nn.getName() << std::endl;
        std::cout << "   Descrição: " << nn.getDescription() << std::endl;
        
        Route solution = nn.solve(graph);
        std::cout << "   Solução: " << solution << std::endl;
        std::cout << "   Tempo: " << nn.getLastExecutionTime() << "ms\n";
        
        std::cout << "\n✅ Todos os testes passaram com sucesso!\n";
        std::cout << "✅ Conceitos POO demonstrados:\n";
        std::cout << "   - Encapsulamento (Point, Graph, Route)\n";
        std::cout << "   - Herança (TSPAlgorithm → NearestNeighborTSP)\n";
        std::cout << "   - Polimorfismo (virtual methods)\n";
        std::cout << "   - Composição (Graph contém Points)\n";
        std::cout << "   - STL containers (vector, map, unique_ptr)\n";
        std::cout << "   - Sobrecarga de operadores (<<, ==, <)\n";
        std::cout << "   - Tratamento de exceções\n";
        
    } catch (const TSPException& e) {
        std::cerr << "❌ Erro TSP: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "❌ Erro: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}