#include "ColisExpress.h"
#include <iostream>

ColisExpress::ColisExpress(const std::string& description, int masse)
    : Colis(description, masse) {
}

void ColisExpress::afficher() const {
    std::cout << "[COLIS EXPRESS]  *** URGENT ***" << std::endl;
    std::cout << "  Description : " << getDescription() << std::endl;
    std::cout << "  Masse       : " << getMasse() << "g" << std::endl;
    std::cout << "  Priorité    : MAXIMALE" << std::endl;
}
