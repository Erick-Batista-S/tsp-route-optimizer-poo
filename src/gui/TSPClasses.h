#ifndef TSPCLASSES_H
#define TSPCLASSES_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <random>

/**
 * @brief Classe que representa um ponto/cidade no problema TSP
 * 
 * Demonstra conceitos POO:
 * - Encapsulamento: dados privados com interface pública
 * - Sobrecarga de operadores
 * - Métodos const
 */
class Point {
private:
    double m_x, m_y;
    std::string m_name;

public:
    Point(double x = 0, double y = 0, const std::string& name = "") 
        : m_x(x), m_y(y), m_name(name) {}
    
    // Getters
    double getX() const { return m_x; }
    double getY() const { return m_y; }
    const std::string& getName() const { return m_name; }
    
    // Setters
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setName(const std::string& name) { m_name = name; }
    
    // Operações
    double distanceTo(const Point& other) const {
        double dx = m_x - other.m_x;
        double dy = m_y - other.m_y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    // Sobrecarga de operadores
    bool operator==(const Point& other) const {
        return std::abs(m_x - other.m_x) < 5.0 && 
               std::abs(m_y - other.m_y) < 5.0;
    }
    
    bool operator<(const Point& other) const {
        if (std::abs(m_x - other.m_x) > 1e-9) return m_x < other.m_x;
        return m_y < other.m_y;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "Point(" << p.m_x << ", " << p.m_y << ", \"" << p.m_name << "\")";
    }
};

/**
 * @brief Classe de exceção personalizada para operações TSP
 */
class TSPException : public std::exception {
private:
    std::string m_message;
public:
    explicit TSPException(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override { return m_message.c_str(); }
};

/**
 * @brief Classe que representa um grafo completo de pontos
 * 
 * Demonstra conceitos POO:
 * - Composição: Graph contém Points
 * - STL: uso de std::vector
 * - Tratamento de exceções
 */
class Graph {
private:
    std::vector<Point> m_points;

public:
    void addPoint(const Point& point) {
        for (const auto& existing : m_points) {
            if (existing == point) {
                throw TSPException("Point already exists");
            }
        }
        m_points.push_back(point);
    }
    
    const Point& getPoint(size_t index) const {
        if (index >= m_points.size()) {
            throw TSPException("Index out of bounds");
        }
        return m_points[index];
    }
    
    size_t getSize() const { return m_points.size(); }
    const std::vector<Point>& getPoints() const { return m_points; }
    
    void clear() { m_points.clear(); }
    
    double getDistance(size_t from, size_t to) const {
        return getPoint(from).distanceTo(getPoint(to));
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
        return os << "Graph[" << g.m_points.size() << " points]";
    }
};

/**
 * @brief Classe que representa uma rota/solução TSP
 * 
 * Demonstra conceitos POO:
 * - Composição: Route contém Points organizados
 * - Sobrecarga de operadores
 * - Cálculos automáticos
 */
class Route {
private:
    std::vector<Point> m_points;
    double m_totalDistance;

public:
    Route() : m_totalDistance(0.0) {}
    
    void addPoint(const Point& point) {
        m_points.push_back(point);
        recalculateDistance();
    }
    
    void clear() {
        m_points.clear();
        m_totalDistance = 0.0;
    }
    
    double getTotalDistance() const { return m_totalDistance; }
    size_t getSize() const { return m_points.size(); }
    const std::vector<Point>& getPoints() const { return m_points; }
    
    void recalculateDistance() {
        m_totalDistance = 0.0;
        if (m_points.size() < 2) return;
        
        for (size_t i = 0; i < m_points.size() - 1; ++i) {
            m_totalDistance += m_points[i].distanceTo(m_points[i + 1]);
        }
        if (m_points.size() > 2) {
            m_totalDistance += m_points.back().distanceTo(m_points.front());
        }
    }
    
    bool operator<(const Route& other) const {
        return m_totalDistance < other.m_totalDistance;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Route& r) {
        os << "Route[" << r.m_points.size() << " points, distance=" 
           << std::fixed << std::setprecision(1) << r.m_totalDistance << "]";
        return os;
    }
};

/**
 * @brief Classe base abstrata para algoritmos TSP
 * 
 * Demonstra conceitos POO:
 * - Herança e polimorfismo
 * - Métodos virtuais puros
 * - Padrão Strategy
 */
class TSPAlgorithm {
public:
    virtual ~TSPAlgorithm() = default;
    virtual Route solve(const Graph& graph) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
};

/**
 * @brief Algoritmo Nearest Neighbor para TSP
 * 
 * Implementação gulosa que sempre escolhe a cidade mais próxima
 */
class NearestNeighborTSP : public TSPAlgorithm {
public:
    Route solve(const Graph& graph) override {
        Route route;
        if (graph.getSize() == 0) return route;
        
        std::vector<bool> visited(graph.getSize(), false);
        size_t current = 0;
        route.addPoint(graph.getPoint(current));
        visited[current] = true;
        
        for (size_t step = 1; step < graph.getSize(); ++step) {
            double minDistance = std::numeric_limits<double>::max();
            size_t nearest = 0;
            
            for (size_t i = 0; i < graph.getSize(); ++i) {
                if (!visited[i]) {
                    double distance = graph.getDistance(current, i);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearest = i;
                    }
                }
            }
            
            current = nearest;
            visited[current] = true;
            route.addPoint(graph.getPoint(current));
        }
        
        return route;
    }
    
    std::string getName() const override { return "Nearest Neighbor"; }
    std::string getDescription() const override { 
        return "Greedy algorithm that selects nearest unvisited city"; 
    }
};

/**
 * @brief Algoritmo Brute Force para TSP
 * 
 * Busca exaustiva por todas as permutações possíveis
 */
class BruteForceTSP : public TSPAlgorithm {
public:
    Route solve(const Graph& graph) override {
        Route bestRoute;
        if (graph.getSize() == 0) return bestRoute;
        
        std::vector<size_t> indices;
        for (size_t i = 0; i < graph.getSize(); ++i) {
            indices.push_back(i);
        }
        
        double bestDistance = std::numeric_limits<double>::max();
        
        do {
            Route currentRoute;
            for (size_t idx : indices) {
                currentRoute.addPoint(graph.getPoint(idx));
            }
            
            if (currentRoute.getTotalDistance() < bestDistance) {
                bestDistance = currentRoute.getTotalDistance();
                bestRoute = currentRoute;
            }
        } while (std::next_permutation(indices.begin(), indices.end()));
        
        return bestRoute;
    }
    
    std::string getName() const override { return "Brute Force"; }
    std::string getDescription() const override { 
        return "Exhaustive search through all permutations"; 
    }
};

#endif // TSPCLASSES_H