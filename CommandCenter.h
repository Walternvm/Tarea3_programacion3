#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H
#include <functional>
#include <list>
#include <map>
#include "Entity.h"

using Command = std::function<void(const std::list<std::string>&)>;

class CommandCenter {
private:
    Entity& entity;
    std::map<std::string, Command> comandos;
    std::list<std::string> historial;
    using macroSteps = std::list<std::pair<std::string, std::list<std::string>>>;
    std::map<std::string, macroSteps> macros;
public:
    CommandCenter(Entity& _entity) : entity(_entity) {}

    void registerCommand(const std::string& n, Command cmd) {
        comandos[n] = cmd;
    }

    void removeCommand(const std::string& n) {
        std::map<std::string, Command>::iterator it = comandos.find(n);
        if (it != comandos.end()) {
            comandos.erase(it);
            std::cout << "Comando " << n << " eliminado" << std::endl;
        }
        else {
            std::cout << "Comando " << n << " no encontrado" << std::endl;
        }
    }

    void executeCommand(const std::string& n, const std::list<std::string>& args) {
        std::map<std::string, Command>::iterator it = comandos.find(n);
        if (it != comandos.end()) {
            std::string b = entity.getStatus();
            it->second(args);
            std::string a = entity.getStatus();
            historial.push_back("Comando: " + n + "\n  Antes:   " + b + "\n  Despues: " + a + "\n");
        } else {
            std::cout << "Comando " << n << " no encontrado" << std::endl;
        }
    }

    void registerMacro(const std::string& n, const macroSteps& steps) {
        macros[n] = steps;
    }

    void executeMacro(const std::string& n) {
        auto itMacro = macros.find(n);
        if (itMacro == macros.end()) {
            std::cout << "Macro " << n << " no encontrado" << std::endl;
            return;
        }
        macroSteps::iterator itStep;
        for (itStep = itMacro->second.begin(); itStep != itMacro->second.end(); itStep++) {
            std::string cmdname = itStep->first;
            std::list<std::string> args = itStep->second;
            if (comandos.find(cmdname) == comandos.end()) {
                std::cout << "Comando " << cmdname << " no encontrado, macro detenido" << std::endl;
                break;
            }
            executeCommand(cmdname, args);
        }
    }

    void printHistorial() {
        std::cout << std::endl << "--- Historial de Comandos ---" << std::endl;
        std::list<std::string>::iterator it;
        for (it = historial.begin(); it != historial.end(); it++) {
            std::cout << *it << std::endl;
        }
        std::cout << "-------------------------------------" << std::endl << std::endl;
    }
};
#endif // COMMANDCENTER_H