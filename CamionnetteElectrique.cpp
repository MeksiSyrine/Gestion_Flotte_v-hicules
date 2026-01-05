#include "CamionnetteElectrique.h"
#include <iostream>
#include <iomanip>

// Capacité fixe pour les camionnettes : 500000g = 500kg
constexpr int CAPACITE_CAMIONNETTE = 500000;

CamionnetteElectrique::CamionnetteElectrique(const std::string& nom)
    : Vehicule(nom, CAPACITE_CAMIONNETTE) {
}

void CamionnetteElectrique::afficher() const {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "[CAMIONNETTE ELECTRIQUE] #" << getId() << " - " << getNom() << std::endl;
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
