#include "../design/Point.h"
#include <cmath>
#include <sstream>

Point::Point() : x(0.0), y(0.0), name("") {}

Point::Point(double x, double y, const std::string& name, int id) 
    : x(x), y(y), name(name) {}

Point::Point(const Point& other)
    : x(other.x), y(other.y), name(other.name) {}

Point& Point::operator=(const Point& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        name = other.name;
    }
    return *this;
}

Point::~Point() {}

double Point::getX() const { return x; }
double Point::getY() const { return y; }
std::string Point::getName() const { return name; }

void Point::setX(double newX) { x = newX; }
void Point::setY(double newY) { y = newY; }
void Point::setName(const std::string& newName) { name = newName; }

double Point::distanceTo(const Point& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Point::operator==(const Point& other) const {
    const double EPSILON = 1e-9;
    return std::abs(x - other.x) < EPSILON && 
           std::abs(y - other.y) < EPSILON;
}

bool Point::operator<(const Point& other) const {
    if (std::abs(x - other.x) < 1e-9) {
        return y < other.y;
    }
    return x < other.x;
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y, 
                name.empty() ? other.name : name + "+" + other.name);
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "Point(" << point.x << ", " << point.y;
    if (!point.name.empty()) {
        os << ", \"" << point.name << "\"";
    }
    os << ")";
    return os;
}

std::string Point::toString() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}