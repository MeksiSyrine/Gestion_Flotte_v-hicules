#include "ColisStandard.h"
#include <iostream>

ColisStandard::ColisStandard(const std::string& description, int masse)
    : Colis(description, masse) {
}

void ColisStandard::afficher() const {
    std::cout << "[COLIS STANDARD]" << std::endl;
    std::cout << "  Description : " << getDescription() << std::endl;
    std::cout << "  Masse       : " << getMasse() << "g" << std::endl;
}
