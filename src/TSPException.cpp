#include "../design/TSPException.h"

// Implementação da classe base TSPException
TSPException::TSPException(const std::string& message) : message(message) {}

TSPException::~TSPException() noexcept {}

const char* TSPException::what() const noexcept {
    return message.c_str();
}

const std::string& TSPException::getMessage() const {
    return message;
}

// Implementação das exceções específicas
InvalidInputException::InvalidInputException(const std::string& message)
    : TSPException("Invalid Input: " + message) {}

EmptyGraphException::EmptyGraphException(const std::string& message)
    : TSPException("Empty Graph: " + message) {}

PointNotFoundException::PointNotFoundException(const std::string& message)
    : TSPException("Point Not Found: " + message) {}

DuplicatePointException::DuplicatePointException(const std::string& message)
    : TSPException("Duplicate Point: " + message) {}

InvalidIndexException::InvalidIndexException(const std::string& message)
    : TSPException("Invalid Index: " + message) {}

AlgorithmException::AlgorithmException(const std::string& message)
    : TSPException("Algorithm Error: " + message) {}

FileException::FileException(const std::string& message)
    : TSPException("File Error: " + message) {}