#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <chrono>

// ================= CLASSES BASE =================

class Point {
private:
    double x, y;
    std::string name;

public:
    Point(double x = 0, double y = 0, const std::string& name = "") 
        : x(x), y(y), name(name) {}
    
    double getX() const { return x; }
    double getY() const { return y; }
    std::string getName() const { return name; }
    
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    void setName(const std::string& newName) { name = newName; }
    
    double distanceTo(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    bool operator==(const Point& other) const {
        const double EPSILON = 1e-9;
        return std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON;
    }
    
    bool operator<(const Point& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "Point(" << p.x << ", " << p.y << ", \"" << p.name << "\")";
        return os;
    }
};

class TSPException : public std::exception {
private:
    std::string message;
public:
    TSPException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class Route {
private:
    std::vector<Point> points;
    double totalDistance;
    
    void calculateDistance() {
        totalDistance = 0.0;
        if (points.size() < 2) return;
        
        for (size_t i = 0; i < points.size() - 1; ++i) {
            totalDistance += points[i].distanceTo(points[i + 1]);
        }
        // Retorno ao início para fechar o ciclo
        if (points.size() >= 3) {
            totalDistance += points.back().distanceTo(points.front());
        }
    }

public:
    Route() : totalDistance(0.0) {}
    
    Route(const std::vector<Point>& pts) : points(pts) {
        calculateDistance();
    }
    
    void addPoint(const Point& point) {
        points.push_back(point);
        calculateDistance();
    }
    
    size_t size() const { return points.size(); }
    Point getPoint(size_t index) const { 
        if (index >= points.size()) throw TSPException("Index out of bounds");
        return points[index]; 
    }
    double getTotalDistance() const { return totalDistance; }
    std::vector<Point> getPoints() const { return points; }
    
    bool operator<(const Route& other) const {
        return totalDistance < other.totalDistance;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Route& route) {
        os << "Route[" << route.points.size() << " points, distance=" 
           << route.totalDistance << "]: ";
        for (size_t i = 0; i < route.points.size(); ++i) {
            if (i > 0) os << " -> ";
            os << route.points[i].getName();
        }
        if (route.points.size() >= 3) {
            os << " -> " << route.points[0].getName();
        }
        return os;
    }
};

class Graph {
private:
    std::vector<Point> points;

public:
    void addPoint(const Point& point) {
        // Verificar duplicatas
        for (const auto& p : points) {
            if (p == point) {
                throw TSPException("Point already exists");
            }
        }
        points.push_back(point);
    }
    
    Point getPoint(size_t index) const {
        if (index >= points.size()) throw TSPException("Index out of bounds");
        return points[index];
    }
    
    size_t size() const { return points.size(); }
    std::vector<Point> getAllPoints() const { return points; }
    
    double getDistance(size_t from, size_t to) const {
        return getPoint(from).distanceTo(getPoint(to));
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph) {
        os << "Graph[" << graph.points.size() << " points]";
        return os;
    }
};

// ================= ALGORITMOS TSP =================

class TSPAlgorithm {
protected:
    long lastExecutionTime;

public:
    TSPAlgorithm() : lastExecutionTime(0) {}
    virtual ~TSPAlgorithm() {}
    
    virtual Route solve(const Graph& graph) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    
    long getLastExecutionTime() const { return lastExecutionTime; }
};

class NearestNeighborTSP : public TSPAlgorithm {
public:
    Route solve(const Graph& graph) override {
        auto start = std::chrono::high_resolution_clock::now();
        
        if (graph.size() < 2) throw TSPException("Need at least 2 points");
        
        Route route;
        std::vector<bool> visited(graph.size(), false);
        
        // Começar do primeiro ponto
        size_t current = 0;
        route.addPoint(graph.getPoint(current));
        visited[current] = true;
        
        // Visitar pontos mais próximos
        while (route.size() < graph.size()) {
            double minDist = 1e9;
            size_t nextPoint = 0;
            
            for (size_t i = 0; i < graph.size(); ++i) {
                if (!visited[i]) {
                    double dist = graph.getDistance(current, i);
                    if (dist < minDist) {
                        minDist = dist;
                        nextPoint = i;
                    }
                }
            }
            
            current = nextPoint;
            route.addPoint(graph.getPoint(current));
            visited[current] = true;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        return route;
    }
    
    std::string getName() const override { return "Nearest Neighbor"; }
    std::string getDescription() const override { 
        return "Greedy algorithm that selects nearest unvisited city"; 
    }
};

class BruteForceTSP : public TSPAlgorithm {
public:
    Route solve(const Graph& graph) override {
        auto start = std::chrono::high_resolution_clock::now();
        
        if (graph.size() < 2) throw TSPException("Need at least 2 points");
        if (graph.size() > 8) throw TSPException("Brute force only for small graphs");
        
        std::vector<Point> points = graph.getAllPoints();
        Route bestRoute(points);
        
        // Gerar todas as permutações
        std::sort(points.begin(), points.end());
        do {
            Route current(points);
            if (current < bestRoute) {
                bestRoute = current;
            }
        } while (std::next_permutation(points.begin(), points.end()));
        
        auto end = std::chrono::high_resolution_clock::now();
        lastExecutionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        return bestRoute;
    }
    
    std::string getName() const override { return "Brute Force"; }
    std::string getDescription() const override { 
        return "Exhaustive search through all permutations"; 
    }
};

// ================= DEMO PRINCIPAL =================

class TSPDemo {
private:
    std::vector<std::unique_ptr<TSPAlgorithm>> algorithms;
    
public:
    TSPDemo() {
        algorithms.push_back(std::make_unique<NearestNeighborTSP>());
        algorithms.push_back(std::make_unique<BruteForceTSP>());
    }
    
    void run() {
        printHeader();
        
        try {
            // Teste 1: Problema pequeno
            std::cout << "\n=== TESTE 1: Conjunto pequeno (4 cidades) ===\n";
            testSmallProblem();
            
            // Teste 2: Demonstração POO
            std::cout << "\n=== TESTE 2: Demonstração POO ===\n";
            demonstratePOO();
            
            // Teste 3: Comparação algoritmos
            std::cout << "\n=== TESTE 3: Comparação de Algoritmos ===\n";
            compareAlgorithms();
            
        } catch (const TSPException& e) {
            std::cerr << "❌ Erro TSP: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "❌ Erro: " << e.what() << std::endl;
        }
        
        printFooter();
    }
    
private:
    void printHeader() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    TSP ROUTE OPTIMIZER                      ║\n";
        std::cout << "║                  Etapa 2 - CLI Funcional                    ║\n";
        std::cout << "║                                                              ║\n";
        std::cout << "║  Demonstração completa dos conceitos POO:                   ║\n";
        std::cout << "║  ✓ Classes e Objetos (Point, Graph, Route, TSPAlgorithm)   ║\n";
        std::cout << "║  ✓ Encapsulamento (dados privados, getters/setters)        ║\n";
        std::cout << "║  ✓ Herança (TSPAlgorithm → NearestNeighbor, BruteForce)    ║\n";
        std::cout << "║  ✓ Polimorfismo (métodos virtuais, dispatch dinâmico)      ║\n";
        std::cout << "║  ✓ Composição (Graph contém Points, Route contém Points)   ║\n";
        std::cout << "║  ✓ STL (vector, unique_ptr, chrono)                        ║\n";
        std::cout << "║  ✓ Sobrecarga de Operadores (<<, ==, <)                    ║\n";
        std::cout << "║  ✓ Tratamento de Exceções (TSPException)                   ║\n";
        std::cout << "║  ✓ Smart Pointers (unique_ptr para algoritmos)             ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }
    
    void testSmallProblem() {
        Graph graph;
        graph.addPoint(Point(0, 0, "São Paulo"));
        graph.addPoint(Point(3, 4, "Rio de Janeiro"));
        graph.addPoint(Point(2, 1, "Curitiba"));
        graph.addPoint(Point(1, 3, "Belo Horizonte"));
        
        std::cout << "Grafo: " << graph << std::endl;
        std::cout << "Pontos:\n";
        for (size_t i = 0; i < graph.size(); ++i) {
            std::cout << "  " << graph.getPoint(i) << std::endl;
        }
    }
    
    void demonstratePOO() {
        // 1. Encapsulamento
        std::cout << "\n1. Encapsulamento - Manipulação segura de dados:\n";
        Point p1(10, 20, "TestPoint");
        std::cout << "   Ponto criado: " << p1 << std::endl;
        std::cout << "   Coordenadas: (" << p1.getX() << ", " << p1.getY() << ")" << std::endl;
        
        p1.setX(15);
        p1.setY(25);
        std::cout << "   Após modificação: " << p1 << std::endl;
        
        // 2. Sobrecarga de operadores
        std::cout << "\n2. Sobrecarga de Operadores:\n";
        Point p2(15, 25, "TestPoint2");
        std::cout << "   p1 == p2? " << (p1 == p2 ? "Sim" : "Não") << std::endl;
        std::cout << "   Distância p1 → p2: " << p1.distanceTo(p2) << std::endl;
        
        // 3. Composição
        std::cout << "\n3. Composição - Graph possui Points:\n";
        Graph testGraph;
        testGraph.addPoint(p1);
        testGraph.addPoint(Point(5, 5, "OutroPonto"));
        std::cout << "   " << testGraph << std::endl;
        
        // 4. Exceções
        std::cout << "\n4. Tratamento de Exceções:\n";
        try {
            testGraph.addPoint(p1); // Duplicado
        } catch (const TSPException& e) {
            std::cout << "   Exceção capturada: " << e.what() << std::endl;
        }
        
        try {
            testGraph.getPoint(999); // Índice inválido
        } catch (const TSPException& e) {
            std::cout << "   Exceção capturada: " << e.what() << std::endl;
        }
        
        // 5. Polimorfismo
        std::cout << "\n5. Polimorfismo - Algoritmos intercambiáveis:\n";
        for (const auto& algo : algorithms) {
            std::cout << "   " << algo->getName() << ": " << algo->getDescription() << std::endl;
        }
    }
    
    void compareAlgorithms() {
        Graph graph;
        graph.addPoint(Point(0, 0, "A"));
        graph.addPoint(Point(1, 0, "B"));
        graph.addPoint(Point(1, 1, "C"));
        graph.addPoint(Point(0, 1, "D"));
        
        std::cout << "Testando " << graph.size() << " pontos:\n";
        std::vector<std::pair<std::string, Route>> results;
        
        for (const auto& algo : algorithms) {
            std::cout << "\n🔄 Executando: " << algo->getName() << std::endl;
            
            Route result = algo->solve(graph);
            results.emplace_back(algo->getName(), result);
            
            std::cout << "   ✅ " << result << std::endl;
            std::cout << "   ⏱️  Tempo: " << algo->getLastExecutionTime() << "ms" << std::endl;
        }
        
        // Comparação
        std::cout << "\n--- Comparação de Resultados ---\n";
        auto best = std::min_element(results.begin(), results.end(),
            [](const auto& a, const auto& b) {
                return a.second.getTotalDistance() < b.second.getTotalDistance();
            });
        
        for (const auto& [name, route] : results) {
            double diff = route.getTotalDistance() - best->second.getTotalDistance();
            std::cout << "   " << name << ": " << route.getTotalDistance() 
                      << " (+" << diff << ")" << std::endl;
        }
        
        std::cout << "\n🏆 Melhor: " << best->first << std::endl;
    }
    
    void printFooter() {
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                       TESTE CONCLUÍDO                       ║\n";
        std::cout << "║                                                              ║\n";
        std::cout << "║  ✅ Todos os conceitos POO foram demonstrados!              ║\n";
        std::cout << "║  ✅ CLI funcional implementado com sucesso!                 ║\n";
        std::cout << "║  ✅ Sistema pronto para interface gráfica (Etapa 3)         ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
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