#include "Entity.h"
#include "CommandCenter.h"

void resetEntity(Entity& e, const std::list<std::string>& args) {
    e.setX(0.0); e.setY(0.0);
}
void levelUp(Entity& e, const std::list<std::string>& args) {
    e.setNivel(e.getNivel() + 1);
}
void showStatus(Entity& e, const std::list<std::string>& args) {
    std::cout << e.getStatus() << std::endl;
}

class DamageFunctor {
    Entity& entity;
    int contadorAtaques = 0;
public:
    DamageFunctor(Entity& e) : entity(e) {}
    void operator()(const std::list<std::string>& args) {
        if (args.empty()) {
            return;
        }
        try {
            entity.setVida(entity.getVida() - std::stoi(args.front()));
            contadorAtaques++;
            std::cout << "Ataque #" << contadorAtaques << " realizado" << std::endl;
        } catch (...) {}
    }
};

class SpendFunctor {
    Entity& e;
    int totalGastado = 0;
public:
    SpendFunctor(Entity& _e) : e(_e) {}
    void operator()(const std::list<std::string>& args) {
        if (args.empty()) {
            return;
        }
        try {
            int costo = std::stoi(args.front());
            e.setRecursos(e.getRecursos() - costo);
            totalGastado += costo;
            std::cout << "Gasto: " << costo << ", total gastado: " << totalGastado << std::endl;
        } catch (...) {}
    }
};

class TeleportFunctor {
    Entity& e;
    int usosRestantes = 3;
public:
    TeleportFunctor(Entity& _e) : e(_e) {}
    void operator()(const std::list<std::string>& args) {
        if (usosRestantes <= 0 || args.size() < 2) {
            return;
        }
        try {
            auto it = args.begin();
            e.setX(std::stod(*it));
            e.setY(std::stod(*(++it)));
            usosRestantes--;
            std::cout << "TP realizado, usos restantes: " << usosRestantes << std::endl;
        } catch (...) {}
    }
};

int main() {
    Entity hero(0, 0, 100, 1, 50);
    CommandCenter center(hero);

    center.registerCommand("reset", [&hero](const auto& a) { resetEntity(hero, a); });
    center.registerCommand("lvlup", [&hero](const auto& a) { levelUp(hero, a); });
    center.registerCommand("status", [&hero](const auto& a) { showStatus(hero, a); });

    center.registerCommand("move", [&hero](const std::list<std::string>& args) {
        if (args.size() < 2) return;
        hero.setX(hero.getX() + std::stod(args.front()));
        hero.setY(hero.getY() + std::stod(args.back()));
    });

    center.registerCommand("heal", [&hero](const auto& args) {
        if (!args.empty()) hero.setVida(hero.getVida() + std::stoi(args.front()));
    });

    center.registerCommand("add_res", [&hero](const auto& args) {
        if (!args.empty()) hero.setRecursos(hero.getRecursos() + std::stoi(args.front()));
    });

    center.registerCommand("damage", DamageFunctor(hero));
    center.registerCommand("spend", SpendFunctor(hero));
    center.registerCommand("tp", TeleportFunctor(hero));

    center.registerMacro("recovery", { {"status", {}}, {"reset", {}}, {"status", {}} });
    center.registerMacro("skirmish", { {"move", {"10", "10"}}, {"damage", {"20"}} });
    center.registerMacro("emergency", { {"tp", {"0", "0"}}, {"heal", {"50"}}, {"status", {}} });

    std::cout << "--- Ejecutando Comandos ---" << std::endl;
    center.executeCommand("move", {"5.5", "3.2"});
    center.executeCommand("spend", {"10"});
    center.executeCommand("tp", {"100", "100"});

    std::cout << std::endl <<"--- Ejecutando Macros ---" << std::endl;
    center.executeMacro("emergency");
    center.executeMacro("skirmish");

    center.printHistorial();

    return 0;
}