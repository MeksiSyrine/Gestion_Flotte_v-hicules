#include "RobotTrottoir.h"
#include <iostream>
#include <iomanip>

// Capacité fixe pour les robots de trottoir : 20000g = 20kg
constexpr int CAPACITE_ROBOT = 20000;

RobotTrottoir::RobotTrottoir(const std::string& nom)
    : Vehicule(nom, CAPACITE_ROBOT) {
}

void RobotTrottoir::afficher() const {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "[ROBOT DE TROTTOIR] #" << getId() << " - " << getNom() << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Etat            : " << etatToString(getEtat()) << std::endl;
    
    // Affichage de la capacité en kilogrammes
    double capacite_utilisee_kg = getCapaciteUtilisee() / 1000.0;
    double capacite_max_kg = getCapaciteMax() / 1000.0;
    
    std::cout << "Capacite        : " 
              << std::fixed << std::setprecision(2) 
              << capacite_utilisee_kg << " kg / "
              << capacite_max_kg << " kg" << std::endl;
    
    std::cout << "Colis transportes: " << getNombreColis() << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}
