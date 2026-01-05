#include "ColisRefrigere.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

ColisRefrigere::ColisRefrigere(const std::string& description, int masse, float temperature_cible)
    : Colis(description, masse), temperature_cible_(temperature_cible) {
    //température raisonnable pour un colis réfrigéré (-50°C à +10°C)
    if (temperature_cible < -50.0f || temperature_cible > 10.0f) {
        throw std::invalid_argument("Température cible aberrante pour un colis réfrigéré");
    }
}

float ColisRefrigere::getTemperatureCible() const {
    return temperature_cible_;
}

void ColisRefrigere::afficher() const {
    std::cout << "[COLIS RÉFRIGÉRÉ]" << std::endl;
    std::cout << "  Description : " << getDescription() << std::endl;
    std::cout << "  Masse       : " << getMasse() << "g" << std::endl;
    std::cout << "  Température : " << std::fixed << std::setprecision(1) 
              << temperature_cible_ << "°C" << std::endl;
}
