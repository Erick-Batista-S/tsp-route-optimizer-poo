#include "../design/Graph.h"
#include "../design/TSPException.h"
#include <algorithm>
#include <limits>

Graph::Graph() {}

Graph::Graph(const Graph& other) {
    for (const auto& point : other.points) {
        addPoint(*point);
    }
}

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        clear();
        for (const auto& point : other.points) {
            addPoint(*point);
        }
    }
    return *this;
}

Graph::~Graph() {
    clear();
}

void Graph::addPoint(const Point& point) {
    // Verificar se o ponto já existe
    for (const auto& existingPoint : points) {
        if (*existingPoint == point) {
            throw DuplicatePointException("Point already exists: " + point.toString());
        }
    }
    
    points.push_back(std::make_unique<Point>(point));
    updateDistanceMatrix();
}

void Graph::removePoint(const Point& point) {
    auto it = std::find_if(points.begin(), points.end(),
        [&point](const std::unique_ptr<Point>& p) {
            return *p == point;
        });
    
    if (it == points.end()) {
        throw PointNotFoundException("Point not found: " + point.toString());
    }
    
    points.erase(it);
    updateDistanceMatrix();
}

Point Graph::getPoint(size_t index) const {
    if (index >= points.size()) {
        throw InvalidIndexException("Index out of bounds: " + std::to_string(index));
    }
    return *points[index];
}

std::vector<Point> Graph::getAllPoints() const {
    std::vector<Point> result;
    result.reserve(points.size());
    for (const auto& point : points) {
        result.push_back(*point);
    }
    return result;
}

size_t Graph::size() const {
    return points.size();
}

bool Graph::isEmpty() const {
    return points.empty();
}

void Graph::clear() {
    points.clear();
    distanceMatrix.clear();
}

double Graph::getDistance(size_t fromIndex, size_t toIndex) const {
    if (fromIndex >= points.size() || toIndex >= points.size()) {
        throw InvalidIndexException("Index out of bounds");
    }
    
    auto key = std::make_pair(std::min(fromIndex, toIndex), 
                             std::max(fromIndex, toIndex));
    auto it = distanceMatrix.find(key);
    
    if (it != distanceMatrix.end()) {
        return it->second;
    }
    
    // Calcular distância se não estiver no cache
    double distance = points[fromIndex]->distanceTo(*points[toIndex]);
    distanceMatrix[key] = distance;
    return distance;
}

double Graph::getDistance(const Point& from, const Point& to) const {
    size_t fromIndex = findPointIndex(from);
    size_t toIndex = findPointIndex(to);
    return getDistance(fromIndex, toIndex);
}

std::vector<Point> Graph::getNearestNeighbors(const Point& point, size_t count) const {
    size_t pointIndex = findPointIndex(point);
    
    if (count >= points.size()) {
        count = points.size() - 1; // Excluir o próprio ponto
    }
    
    std::vector<std::pair<double, size_t>> distances;
    for (size_t i = 0; i < points.size(); ++i) {
        if (i != pointIndex) {
            distances.emplace_back(getDistance(pointIndex, i), i);
        }
    }
    
    // Ordenar por distância
    std::partial_sort(distances.begin(), 
                     distances.begin() + count, 
                     distances.end());
    
    std::vector<Point> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back(*points[distances[i].second]);
    }
    
    return result;
}

Point Graph::findNearestPoint(const Point& point) const {
    if (points.empty()) {
        throw EmptyGraphException("Cannot find nearest point in empty graph");
    }
    
    size_t pointIndex = findPointIndex(point);
    double minDistance = std::numeric_limits<double>::max();
    size_t nearestIndex = 0;
    
    for (size_t i = 0; i < points.size(); ++i) {
        if (i != pointIndex) {
            double distance = getDistance(pointIndex, i);
            if (distance < minDistance) {
                minDistance = distance;
                nearestIndex = i;
            }
        }
    }
    
    return *points[nearestIndex];
}

void Graph::updateDistanceMatrix() {
    // Limpar matriz existente
    distanceMatrix.clear();
    
    // Pré-calcular todas as distâncias
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double distance = points[i]->distanceTo(*points[j]);
            auto key = std::make_pair(i, j);
            distanceMatrix[key] = distance;
        }
    }
}

size_t Graph::findPointIndex(const Point& point) const {
    for (size_t i = 0; i < points.size(); ++i) {
        if (*points[i] == point) {
            return i;
        }
    }
    throw PointNotFoundException("Point not found: " + point.toString());
}

bool Graph::hasPoint(const Point& point) const {
    return std::any_of(points.begin(), points.end(),
        [&point](const std::unique_ptr<Point>& p) {
            return *p == point;
        });
}

std::optional<Point> Graph::findPointByName(const std::string& name) const {
    auto it = std::find_if(points.begin(), points.end(),
        [&name](const std::unique_ptr<Point>& p) {
            return p->getName() == name;
        });
    
    if (it != points.end()) {
        return **it;
    }
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    os << "Graph[" << graph.points.size() << " points]: ";
    for (size_t i = 0; i < graph.points.size(); ++i) {
        if (i > 0) os << ", ";
        os << graph.points[i]->getName();
    }
    return os;
}