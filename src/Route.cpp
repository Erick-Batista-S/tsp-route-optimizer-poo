#include "../design/Route.h"
#include "../design/TSPException.h"
#include <algorithm>
#include <numeric>
#include <sstream>

Route::Route() : totalDistance(0.0) {}

Route::Route(const std::vector<Point>& points) : points(points) {
    calculateTotalDistance();
}

Route::Route(const Route& other) 
    : points(other.points), totalDistance(other.totalDistance) {}

Route& Route::operator=(const Route& other) {
    if (this != &other) {
        points = other.points;
        totalDistance = other.totalDistance;
    }
    return *this;
}

Route::~Route() {}

void Route::addPoint(const Point& point) {
    points.push_back(point);
    calculateTotalDistance();
}

void Route::removePoint(size_t index) {
    if (index >= points.size()) {
        throw InvalidIndexException("Index out of bounds: " + std::to_string(index));
    }
    points.erase(points.begin() + index);
    calculateTotalDistance();
}

void Route::insertPoint(size_t index, const Point& point) {
    if (index > points.size()) {
        throw InvalidIndexException("Index out of bounds: " + std::to_string(index));
    }
    points.insert(points.begin() + index, point);
    calculateTotalDistance();
}

Point Route::getPoint(size_t index) const {
    if (index >= points.size()) {
        throw InvalidIndexException("Index out of bounds: " + std::to_string(index));
    }
    return points[index];
}

std::vector<Point> Route::getPoints() const {
    return points;
}

size_t Route::size() const {
    return points.size();
}

bool Route::isEmpty() const {
    return points.empty();
}

void Route::clear() {
    points.clear();
    totalDistance = 0.0;
}

double Route::getTotalDistance() const {
    return totalDistance;
}

void Route::calculateTotalDistance() {
    totalDistance = 0.0;
    if (points.size() < 2) return;
    
    // Calcular distância entre pontos consecutivos
    for (size_t i = 0; i < points.size() - 1; ++i) {
        totalDistance += points[i].distanceTo(points[i + 1]);
    }
    
    // Adicionar distância de volta ao primeiro ponto (ciclo TSP)
    if (points.size() >= 3) {
        totalDistance += points.back().distanceTo(points.front());
    }
}

void Route::optimizeTwoOpt() {
    if (points.size() < 4) return;
    
    bool improved = true;
    while (improved) {
        improved = false;
        for (size_t i = 1; i < points.size() - 2; ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                if (j - i == 1) continue; // Skip adjacent edges
                
                double currentDistance = points[i-1].distanceTo(points[i]) + 
                                       points[j].distanceTo(points[(j+1) % points.size()]);
                double newDistance = points[i-1].distanceTo(points[j]) + 
                                   points[i].distanceTo(points[(j+1) % points.size()]);
                
                if (newDistance < currentDistance) {
                    // Reverter a subsequência entre i e j
                    std::reverse(points.begin() + i, points.begin() + j + 1);
                    improved = true;
                    calculateTotalDistance();
                }
            }
        }
    }
}

bool Route::operator<(const Route& other) const {
    return totalDistance < other.totalDistance;
}

bool Route::operator==(const Route& other) const {
    const double EPSILON = 1e-9;
    return std::abs(totalDistance - other.totalDistance) < EPSILON &&
           points == other.points;
}

Route Route::operator+(const Route& other) const {
    Route result = *this;
    for (const Point& point : other.points) {
        // Evitar pontos duplicados
        auto it = std::find(result.points.begin(), result.points.end(), point);
        if (it == result.points.end()) {
            result.addPoint(point);
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Route& route) {
    os << "Route[" << route.points.size() << " points, distance=" 
       << route.totalDistance << "]: ";
    
    for (size_t i = 0; i < route.points.size(); ++i) {
        if (i > 0) os << " -> ";
        os << route.points[i].getName();
    }
    
    if (route.points.size() >= 3) {
        os << " -> " << route.points[0].getName(); // Volta ao início
    }
    
    return os;
}

std::string Route::toString() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}