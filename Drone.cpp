#include "Drone.h"
#include <iostream>
#include <iomanip>

// Capacité fixe pour les drones : 5000g = 5kg
constexpr int CAPACITE_DRONE = 5000;

Drone::Drone(const std::string& nom, int autonomie_km)
    : Vehicule(nom, CAPACITE_DRONE), autonomie_km_(autonomie_km) {
    if (autonomie_km <= 0) {
        throw std::invalid_argument("L'autonomie doit etre positive");
    }
}

int Drone::getAutonomie() const {
    return autonomie_km_;
}

void Drone::afficher() const {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "[DRONE] #" << getId() << " - " << getNom() << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Autonomie       : " << autonomie_km_ << " km" << std::endl;
    std::cout << "Etat            : " << etatToString(getEtat()) << std::endl;
    
    // Affichage de la capacité en kilogrammes (conversion grammes -> kg)
    double capacite_utilisee_kg = getCapaciteUtilisee() / 1000.0;
    double capacite_max_kg = getCapaciteMax() / 1000.0;
    
    std::cout << "Capacite        : " 
              << std::fixed << std::setprecision(2) 
              << capacite_utilisee_kg << " kg / "
              << capacite_max_kg << " kg" << std::endl;
    
    std::cout << "Colis transportes: " << getNombreColis() << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}
