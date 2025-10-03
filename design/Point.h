#pragma once

/**
 * @file Point.h
 * @brief Representa um ponto geográfico no problema TSP
 * @author Erick Batista da Silva
 */

#include <string>
#include <iostream>

/**
 * @class Point
 * @brief Classe que representa um ponto/cidade no grafo TSP
 * 
 * Demonstra: Encapsulamento (dados privados), Abstração (interface clara)
 */
class Point {
private:
    double x_;          ///< Coordenada X (longitude)
    double y_;          ///< Coordenada Y (latitude)  
    std::string name_;  ///< Nome da cidade/ponto
    int id_;           ///< Identificador único

public:
    /**
     * @brief Construtor com validação de coordenadas
     * @param x Coordenada X
     * @param y Coordenada Y
     * @param name Nome do ponto
     * @param id Identificador único
     */
    Point(double x, double y, const std::string& name = "", int id = 0);
    
    /**
     * @brief Construtor de cópia
     */
    Point(const Point& other);
    
    /**
     * @brief Operador de atribuição
     */
    Point& operator=(const Point& other);
    
    /**
     * @brief Destrutor
     */
    ~Point() = default;

    // Getters com validação
    double getX() const { return x_; }
    double getY() const { return y_; }
    const std::string& getName() const { return name_; }
    int getId() const { return id_; }
    
    // Setters com validação
    void setX(double newX);
    void setY(double newY);
    void setName(const std::string& name);
    
    /**
     * @brief Calcula distância euclidiana até outro ponto
     * @param other Ponto de destino
     * @return Distância entre os pontos
     */
    double distanceTo(const Point& other) const;
    
    /**
     * @brief Sobrecarga do operador == para comparação
     */
    bool operator==(const Point& other) const;
    
    /**
     * @brief Sobrecarga do operador < para ordenação
     */
    bool operator<(const Point& other) const;
    
    /**
     * @brief Sobrecarga do operador << para impressão
     */
    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};