#include "../design/TSPAlgorithm.h"
#include "../design/TSPException.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>

// ==================== NEAREST NEIGHBOR ALGORITHM ====================

NearestNeighborTSP::NearestNeighborTSP() {}

NearestNeighborTSP::~NearestNeighborTSP() {}

Route NearestNeighborTSP::solve(const Graph& graph) {
    if (graph.isEmpty()) {
        throw EmptyGraphException("Cannot solve TSP on empty graph");
    }
    
    if (graph.size() < 2) {
        throw InvalidInputException("Need at least 2 points for TSP");
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    Route bestRoute;
    double bestDistance = std::numeric_limits<double>::max();
    
    // Tentar começar de cada ponto (para melhorar a solução)
    for (size_t startIdx = 0; startIdx < graph.size(); ++startIdx) {
        Route currentRoute = solveFromPoint(graph, startIdx);
        if (currentRoute.getTotalDistance() < bestDistance) {
            bestDistance = currentRoute.getTotalDistance();
            bestRoute = currentRoute;
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    lastExecutionTime = duration.count();
    
    return bestRoute;
}

std::string NearestNeighborTSP::getName() const {
    return "Nearest Neighbor";
}

std::string NearestNeighborTSP::getDescription() const {
    return "Greedy algorithm that always chooses the nearest unvisited city";
}

Route NearestNeighborTSP::solveFromPoint(const Graph& graph, size_t startIndex) {
    Route route;
    std::vector<bool> visited(graph.size(), false);
    
    // Começar do ponto especificado
    Point currentPoint = graph.getPoint(startIndex);
    route.addPoint(currentPoint);
    visited[startIndex] = true;
    
    // Visitar todos os outros pontos
    while (route.size() < graph.size()) {
        double minDistance = std::numeric_limits<double>::max();
        size_t nextIndex = 0;
        
        // Encontrar o ponto mais próximo não visitado
        for (size_t i = 0; i < graph.size(); ++i) {
            if (!visited[i]) {
                double distance = graph.getDistance(currentPoint, graph.getPoint(i));
                if (distance < minDistance) {
                    minDistance = distance;
                    nextIndex = i;
                }
            }
        }
        
        // Mover para o próximo ponto
        currentPoint = graph.getPoint(nextIndex);
        route.addPoint(currentPoint);
        visited[nextIndex] = true;
    }
    
    return route;
}

// ==================== GENETIC ALGORITHM ====================

GeneticTSP::GeneticTSP(int populationSize, int maxGenerations, double mutationRate)
    : populationSize(populationSize), maxGenerations(maxGenerations), 
      mutationRate(mutationRate), generator(std::random_device{}()) {}

GeneticTSP::~GeneticTSP() {}

Route GeneticTSP::solve(const Graph& graph) {
    if (graph.isEmpty()) {
        throw EmptyGraphException("Cannot solve TSP on empty graph");
    }
    
    if (graph.size() < 2) {
        throw InvalidInputException("Need at least 2 points for TSP");
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Inicializar população
    std::vector<Route> population = initializePopulation(graph);
    
    for (int generation = 0; generation < maxGenerations; ++generation) {
        // Avaliação (já feita no construtor da Route)
        
        // Seleção e reprodução
        std::vector<Route> newPopulation;
        
        // Manter os melhores (elitismo)
        std::sort(population.begin(), population.end());
        int eliteCount = populationSize / 10; // 10% de elite
        for (int i = 0; i < eliteCount; ++i) {
            newPopulation.push_back(population[i]);
        }
        
        // Gerar resto da população por cruzamento
        while (newPopulation.size() < populationSize) {
            Route parent1 = selectParent(population);
            Route parent2 = selectParent(population);
            Route child = crossover(parent1, parent2, graph);
            
            if (shouldMutate()) {
                mutate(child);
            }
            
            newPopulation.push_back(child);
        }
        
        population = newPopulation;
    }
    
    // Retornar a melhor solução
    std::sort(population.begin(), population.end());
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    lastExecutionTime = duration.count();
    
    return population[0];
}

std::string GeneticTSP::getName() const {
    return "Genetic Algorithm";
}

std::string GeneticTSP::getDescription() const {
    return "Evolutionary algorithm using selection, crossover and mutation";
}

std::vector<Route> GeneticTSP::initializePopulation(const Graph& graph) {
    std::vector<Route> population;
    std::vector<Point> points = graph.getAllPoints();
    
    for (int i = 0; i < populationSize; ++i) {
        std::shuffle(points.begin(), points.end(), generator);
        Route route(points);
        population.push_back(route);
    }
    
    return population;
}

Route GeneticTSP::selectParent(const std::vector<Route>& population) {
    // Seleção por torneio
    const int tournamentSize = 3;
    std::uniform_int_distribution<> dist(0, population.size() - 1);
    
    Route best = population[dist(generator)];
    for (int i = 1; i < tournamentSize; ++i) {
        Route candidate = population[dist(generator)];
        if (candidate < best) {
            best = candidate;
        }
    }
    
    return best;
}

Route GeneticTSP::crossover(const Route& parent1, const Route& parent2, const Graph& graph) {
    // Order Crossover (OX)
    std::vector<Point> child(parent1.size());
    std::vector<bool> used(parent1.size(), false);
    
    // Selecionar segmento aleatório do parent1
    std::uniform_int_distribution<> dist(0, parent1.size() - 1);
    int start = dist(generator);
    int end = dist(generator);
    if (start > end) std::swap(start, end);
    
    // Copiar segmento do parent1
    for (int i = start; i <= end; ++i) {
        child[i] = parent1.getPoint(i);
        used[i] = true;
    }
    
    // Preencher resto com parent2
    int childIdx = (end + 1) % parent1.size();
    for (size_t i = 0; i < parent2.size(); ++i) {
        Point point = parent2.getPoint((end + 1 + i) % parent2.size());
        
        // Verificar se já foi usado
        bool alreadyUsed = false;
        for (size_t j = start; j <= end; ++j) {
            if (child[j] == point) {
                alreadyUsed = true;
                break;
            }
        }
        
        if (!alreadyUsed) {
            while (used[childIdx]) {
                childIdx = (childIdx + 1) % parent1.size();
            }
            child[childIdx] = point;
            used[childIdx] = true;
            childIdx = (childIdx + 1) % parent1.size();
        }
    }
    
    return Route(child);
}

void GeneticTSP::mutate(Route& route) {
    // Swap mutation
    std::uniform_int_distribution<> dist(0, route.size() - 1);
    size_t idx1 = dist(generator);
    size_t idx2 = dist(generator);
    
    if (idx1 != idx2) {
        std::vector<Point> points = route.getPoints();
        std::swap(points[idx1], points[idx2]);
        route = Route(points);
    }
}

bool GeneticTSP::shouldMutate() {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    return dist(generator) < mutationRate;
}

// ==================== 2-OPT ALGORITHM ====================

TwoOptTSP::TwoOptTSP(int maxIterations) : maxIterations(maxIterations) {}

TwoOptTSP::~TwoOptTSP() {}

Route TwoOptTSP::solve(const Graph& graph) {
    if (graph.isEmpty()) {
        throw EmptyGraphException("Cannot solve TSP on empty graph");
    }
    
    if (graph.size() < 2) {
        throw InvalidInputException("Need at least 2 points for TSP");
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Começar com solução do Nearest Neighbor
    NearestNeighborTSP nn;
    Route currentRoute = nn.solve(graph);
    
    // Aplicar 2-opt
    bool improved = true;
    int iterations = 0;
    
    while (improved && iterations < maxIterations) {
        improved = false;
        iterations++;
        
        for (size_t i = 1; i < currentRoute.size() - 1; ++i) {
            for (size_t j = i + 1; j < currentRoute.size(); ++j) {
                Route newRoute = twoOptSwap(currentRoute, i, j);
                if (newRoute < currentRoute) {
                    currentRoute = newRoute;
                    improved = true;
                }
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    lastExecutionTime = duration.count();
    
    return currentRoute;
}

std::string TwoOptTSP::getName() const {
    return "2-Opt";
}

std::string TwoOptTSP::getDescription() const {
    return "Local search algorithm that iteratively improves the route by swapping edges";
}

Route TwoOptTSP::twoOptSwap(const Route& route, size_t i, size_t j) {
    std::vector<Point> newRoute;
    
    // Adicionar pontos antes de i
    for (size_t idx = 0; idx < i; ++idx) {
        newRoute.push_back(route.getPoint(idx));
    }
    
    // Adicionar pontos de j até i em ordem reversa
    for (int idx = j; idx >= static_cast<int>(i); --idx) {
        newRoute.push_back(route.getPoint(idx));
    }
    
    // Adicionar pontos depois de j
    for (size_t idx = j + 1; idx < route.size(); ++idx) {
        newRoute.push_back(route.getPoint(idx));
    }
    
    return Route(newRoute);
}