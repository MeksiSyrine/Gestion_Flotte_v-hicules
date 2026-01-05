#include "ColisFragile.h"
#include <iostream>

ColisFragile::ColisFragile(const std::string& description, int masse, Fragilite fragilite)
    : Colis(description, masse), fragilite_(fragilite) {
}

Fragilite ColisFragile::getFragilite() const {
    return fragilite_;
}

void ColisFragile::afficher() const {
    std::cout << "[COLIS FRAGILE] " << std::endl;
    std::cout << "  Description : " << getDescription() << std::endl;
    std::cout << "  Masse       : " << getMasse() << "g" << std::endl;
    std::cout << "  Fragilité   : " << fragiliteToString() << std::endl;
}

std::string ColisFragile::fragiliteToString() const {
    switch (fragilite_) {
        case Fragilite::FAIBLE: return "FAIBLE";
        case Fragilite::MOYEN:  return "MOYEN";
        case Fragilite::ELEVE:  return "ÉLEVÉ";
        default:                return "INCONNU";
    }
}
