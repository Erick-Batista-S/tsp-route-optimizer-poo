#pragma once

/**
 * @file Route.h
 * @brief Representa uma rota/solução do problema TSP
 * @author Erick Batista da Silva
 */

#include "Point.h"
#include <vector>
#include <iostream>
#include <chrono>

/**
 * @class Route
 * @brief Classe que representa uma rota (sequência de pontos visitados)
 * 
 * Demonstra: Composição (contém sequência de Points), Sobrecarga de operadores
 */
class Route {
private:
    std::vector<int> point_sequence_;  ///< Sequência de IDs dos pontos
    double total_distance_;            ///< Distância total da rota
    std::chrono::milliseconds calculation_time_;  ///< Tempo de cálculo
    bool distance_calculated_;         ///< Flag se distância foi calculada

public:
    /**
     * @brief Construtor padrão
     */
    Route();
    
    /**
     * @brief Construtor com sequência inicial
     * @param sequence Sequência inicial de IDs
     */
    explicit Route(const std::vector<int>& sequence);
    
    /**
     * @brief Construtor de cópia
     */
    Route(const Route& other);
    
    /**
     * @brief Operador de atribuição
     */
    Route& operator=(const Route& other);
    
    /**
     * @brief Destrutor
     */
    ~Route() = default;
    
    /**
     * @brief Adiciona ponto no final da rota
     * @param point_id ID do ponto a adicionar
     */
    void addPoint(int point_id);
    
    /**
     * @brief Insere ponto em posição específica
     * @param position Posição onde inserir
     * @param point_id ID do ponto
     */
    void insertPoint(size_t position, int point_id);
    
    /**
     * @brief Remove ponto da rota
     * @param position Posição do ponto a remover
     */
    void removePoint(size_t position);
    
    /**
     * @brief Obtém sequência de pontos
     */
    const std::vector<int>& getSequence() const { return point_sequence_; }
    
    /**
     * @brief Define nova sequência
     */
    void setSequence(const std::vector<int>& sequence);
    
    /**
     * @brief Obtém tamanho da rota
     */
    size_t size() const { return point_sequence_.size(); }
    
    /**
     * @brief Verifica se rota está vazia
     */
    bool empty() const { return point_sequence_.empty(); }
    
    /**
     * @brief Obtém distância total (calcula se necessário)
     * @param graph Grafo para calcular distâncias
     * @return Distância total da rota
     */
    double getTotalDistance(const class Graph& graph);
    
    /**
     * @brief Força recálculo da distância
     */
    void invalidateDistance();
    
    /**
     * @brief Define tempo de cálculo
     */
    void setCalculationTime(std::chrono::milliseconds time);
    
    /**
     * @brief Obtém tempo de cálculo
     */
    std::chrono::milliseconds getCalculationTime() const;
    
    /**
     * @brief Verifica se é uma rota válida (fechada)
     */
    bool isValidTSPRoute() const;
    
    /**
     * @brief Fecha a rota (adiciona ponto inicial no final)
     */
    void closeTSPRoute();
    
    /**
     * @brief Operação 2-opt para otimização local
     * @param i Primeira posição
     * @param k Segunda posição
     */
    void twoOptSwap(size_t i, size_t k);
    
    // Sobrecarga de operadores
    
    /**
     * @brief Operador < para comparação por distância
     */
    bool operator<(const Route& other) const;
    
    /**
     * @brief Operador == para comparação de rotas
     */
    bool operator==(const Route& other) const;
    
    /**
     * @brief Operador + para concatenar rotas
     */
    Route operator+(const Route& other) const;
    
    /**
     * @brief Operador += para adicionar rota
     */
    Route& operator+=(const Route& other);
    
    /**
     * @brief Operador [] para acessar ponto por índice
     */
    int& operator[](size_t index);
    const int& operator[](size_t index) const;
    
    /**
     * @brief Operador << para impressão
     */
    friend std::ostream& operator<<(std::ostream& os, const Route& route);
    
    /**
     * @brief Salva rota em arquivo
     */
    void saveToFile(const std::string& filename) const;
    
    /**
     * @brief Carrega rota de arquivo
     */
    void loadFromFile(const std::string& filename);
};