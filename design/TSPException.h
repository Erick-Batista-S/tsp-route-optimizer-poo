#pragma once

/**
 * @file TSPException.h
 * @brief Exceções customizadas para o sistema TSP
 * @author Erick Batista da Silva
 */

#include <stdexcept>
#include <string>

/**
 * @class TSPException
 * @brief Classe base para exceções do sistema TSP
 * 
 * Demonstra: Herança de std::exception, tratamento de exceções
 */
class TSPException : public std::exception {
private:
    std::string message_;

public:
    /**
     * @brief Construtor com mensagem
     */
    explicit TSPException(const std::string& message);
    
    /**
     * @brief Obtém mensagem de erro
     */
    const char* what() const noexcept override;
};

/**
 * @class InvalidGraphException
 * @brief Exceção para grafos inválidos
 */
class InvalidGraphException : public TSPException {
public:
    explicit InvalidGraphException(const std::string& details);
};

/**
 * @class FileIOException
 * @brief Exceção para erros de arquivo
 */
class FileIOException : public TSPException {
public:
    explicit FileIOException(const std::string& filename, const std::string& operation);
};

/**
 * @class AlgorithmException
 * @brief Exceção para erros durante execução de algoritmos
 */
class AlgorithmException : public TSPException {
public:
    explicit AlgorithmException(const std::string& algorithm_name, const std::string& details);
};