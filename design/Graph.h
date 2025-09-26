#pragma once

/**
 * @file Graph.h
 * @brief Representa o grafo completo para o problema TSP
 * @author Erick Batista da Silva
 */

#include "Point.h"
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <functional>

/**
 * @brief Hash personalizado para std::pair<int,int>
 */
struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

/**
 * @class Graph
 * @brief Classe que representa o grafo completo de pontos/cidades
 * 
 * Demonstra: Composição (contém Points), Templates (std::vector), STL
 */
class Graph {
private:
    std::vector<std::unique_ptr<Point>> points_;           ///< Pontos do grafo
    std::map<std::pair<int,int>, double> distances_;       ///< Cache de distâncias (usando map por simplicidade)
    bool distances_calculated_;  ///< Flag se distâncias foram calculadas

public:
    /**
     * @brief Construtor padrão
     */
    Graph();
    
    /**
     * @brief Construtor de cópia (deep copy dos pontos)
     */
    Graph(const Graph& other);
    
    /**
     * @brief Operador de atribuição
     */
    Graph& operator=(const Graph& other);
    
    /**
     * @brief Destrutor
     */
    ~Graph() = default;
    
    /**
     * @brief Adiciona um novo ponto ao grafo
     * @param point Ponto a ser adicionado (transfere propriedade)
     * @throws std::invalid_argument se ID já existe
     */
    void addPoint(std::unique_ptr<Point> point);
    
    /**
     * @brief Remove ponto por ID
     * @param id ID do ponto a remover
     * @return true se removido com sucesso
     */
    bool removePoint(int id);
    
    /**
     * @brief Obtém ponto por ID
     * @param id ID do ponto
     * @return Referência para o ponto ou nullopt se não encontrado
     */
    std::optional<std::reference_wrapper<const Point>> getPoint(int id) const;
    
    /**
     * @brief Obtém todos os pontos
     * @return Vector com referências constantes para os pontos
     */
    std::vector<std::reference_wrapper<const Point>> getAllPoints() const;
    
    /**
     * @brief Obtém quantidade de pontos
     */
    size_t size() const { return points_.size(); }
    
    /**
     * @brief Verifica se grafo está vazio
     */
    bool empty() const { return points_.empty(); }
    
    /**
     * @brief Calcula distância entre dois pontos
     * @param id1 ID do primeiro ponto
     * @param id2 ID do segundo ponto
     * @return Distância entre os pontos
     * @throws std::invalid_argument se IDs não existem
     */
    double getDistance(int id1, int id2) const;
    
    /**
     * @brief Pré-calcula todas as distâncias (otimização)
     */
    void calculateAllDistances();
    
    /**
     * @brief Verifica se grafo é válido para TSP (pelo menos 2 pontos)
     */
    bool isValidForTSP() const;
    
    /**
     * @brief Limpa todos os pontos
     */
    void clear();
    
    /**
     * @brief Carrega pontos de arquivo
     * @param filename Caminho do arquivo
     * @throws std::runtime_error se arquivo não pode ser lido
     */
    void loadFromFile(const std::string& filename);
    
    /**
     * @brief Salva pontos em arquivo
     * @param filename Caminho do arquivo
     * @throws std::runtime_error se arquivo não pode ser escrito
     */
    void saveToFile(const std::string& filename) const;
};