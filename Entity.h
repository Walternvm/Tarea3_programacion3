#ifndef ENTITY_H
#define ENTITY_H
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

class Entity{
private:
    double x;
    double y;
    int vida;
    int nivel;
    int recursos;
public:
    Entity(double _x, double _y, int _vida, int _nivel, int _recursos) : x(_x), y(_y), vida(_vida), nivel(_nivel), recursos(_recursos){};
    void setX(double _x) {
        this->x = _x;
    }
    double getX() {
        return this->x;
    }
    void setY(double _y) {
        this->y = _y;
    }
    double getY() {
        return this->y;
    }
    void setVida(int _vida) {
        this->vida = _vida;
    }
    int getVida() {
        return this->vida;
    }
    void setNivel(int _nivel) {
        this->nivel = _nivel;
    }
    int getNivel() {
        return this->nivel;
    }
    void setRecursos(int _recursos) {
        this->recursos = _recursos;
    }
    int getRecursos() {
        return this->recursos;
    }
    std::string getStatus() {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Vida: " << vida
            << ", Nivel: " << nivel
            << ", Recursos: " << recursos
            << ", Pos: (" << x << ", " << y << ")";
        return oss.str();
    }
};

#endif //ENTITY_H