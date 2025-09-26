#pragma once

/**
 * @file TSPAlgorithm.h
 * @brief Interface abstrata para algoritmos de resolução do TSP
 * @author Erick Batista da Silva
 */

#include "Graph.h"
#include "Route.h"
#include <memory>
#include <string>
#include <chrono>

/**
 * @class TSPAlgorithm
 * @brief Classe abstrata base para algoritmos TSP
 * 
 * Demonstra: Herança, Polimorfismo, Abstração (métodos virtuais puros)
 */
class TSPAlgorithm {
public:
    /**
     * @brief Construtor virtual padrão
     */
    TSPAlgorithm() = default;
    
    /**
     * @brief Destrutor virtual
     */
    virtual ~TSPAlgorithm() = default;
    
    /**
     * @brief Método puro virtual para resolver TSP
     * @param graph Grafo com pontos a serem visitados
     * @return Melhor rota encontrada
     * @throws TSPException se grafo inválido
     */
    virtual Route solve(const Graph& graph) = 0;
    
    /**
     * @brief Obtém nome do algoritmo
     */
    virtual std::string getAlgorithmName() const = 0;
    
    /**
     * @brief Obtém descrição do algoritmo
     */
    virtual std::string getDescription() const = 0;
    
    /**
     * @brief Define parâmetros do algoritmo (opcional)
     */
    virtual void setParameters(const std::unordered_map<std::string, double>& params) {}
    
    /**
     * @brief Obtém complexidade estimada
     */
    virtual std::string getTimeComplexity() const = 0;
    
    /**
     * @brief Verifica se algoritmo é exato ou heurístico
     */
    virtual bool isExactAlgorithm() const = 0;

protected:
    /**
     * @brief Valida se grafo pode ser processado
     */
    virtual bool validateGraph(const Graph& graph) const;
    
    /**
     * @brief Hook para pré-processamento (template method pattern)
     */
    virtual void preProcess(const Graph& graph) {}
    
    /**
     * @brief Hook para pós-processamento
     */
    virtual void postProcess(Route& route, const Graph& graph) {}
};

/**
 * @class NearestNeighborTSP
 * @brief Implementação do algoritmo Nearest Neighbor
 * 
 * Demonstra: Herança concreta, implementação de métodos virtuais
 */
class NearestNeighborTSP : public TSPAlgorithm {
private:
    int start_point_id_;  ///< ID do ponto inicial

public:
    /**
     * @brief Construtor
     */
    explicit NearestNeighborTSP(int start_point_id = 0);
    
    /**
     * @brief Implementa algoritmo Nearest Neighbor
     */
    Route solve(const Graph& graph) override;
    
    std::string getAlgorithmName() const override;
    std::string getDescription() const override;
    std::string getTimeComplexity() const override;
    bool isExactAlgorithm() const override;
    
    /**
     * @brief Define ponto inicial
     */
    void setStartPoint(int point_id);
};

/**
 * @class GeneticTSP
 * @brief Implementação de Algoritmo Genético para TSP
 */
class GeneticTSP : public TSPAlgorithm {
private:
    int population_size_;
    int max_generations_;
    double mutation_rate_;
    double crossover_rate_;

public:
    /**
     * @brief Construtor com parâmetros padrão
     */
    GeneticTSP(int population_size = 100, int max_generations = 500);
    
    Route solve(const Graph& graph) override;
    std::string getAlgorithmName() const override;
    std::string getDescription() const override;
    std::string getTimeComplexity() const override;
    bool isExactAlgorithm() const override;
    
    void setParameters(const std::unordered_map<std::string, double>& params) override;

private:
    /**
     * @brief Cria população inicial
     */
    std::vector<Route> createInitialPopulation(const Graph& graph);
    
    /**
     * @brief Operador de crossover
     */
    Route crossover(const Route& parent1, const Route& parent2, const Graph& graph);
    
    /**
     * @brief Operador de mutação
     */
    void mutate(Route& route);
};

/**
 * @class TwoOptTSP
 * @brief Implementação de otimização local 2-Opt
 */
class TwoOptTSP : public TSPAlgorithm {
private:
    std::unique_ptr<TSPAlgorithm> initial_solution_; ///< Algoritmo para solução inicial
    int max_iterations_;

public:
    /**
     * @brief Construtor
     * @param initial_solver Algoritmo para gerar solução inicial
     */
    explicit TwoOptTSP(std::unique_ptr<TSPAlgorithm> initial_solver);
    
    Route solve(const Graph& graph) override;
    std::string getAlgorithmName() const override;
    std::string getDescription() const override;
    std::string getTimeComplexity() const override;
    bool isExactAlgorithm() const override;

private:
    /**
     * @brief Aplica melhoria 2-opt
     */
    bool improve2Opt(Route& route, const Graph& graph);
};