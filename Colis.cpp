#include "Colis.h"
#include <stdexcept>

Colis::Colis(const std::string& description, int masse)
    : description_(description), masse_(masse) {
    // Validation : la masse doit être positive
    if (masse <= 0) {
        throw std::invalid_argument("La masse doit être positive (en grammes)");
    }
}

const std::string& Colis::getDescription() const {
    return description_;
}

int Colis::getMasse() const {
    return masse_;
}
