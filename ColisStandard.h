#ifndef COLIS_STANDARD_H
#define COLIS_STANDARD_H

#include "Colis.h"

/**
 * Colis standard : type de base sans caractéristiques particulières
 */
class ColisStandard : public Colis {
public:
    // Constructeur
    ColisStandard(const std::string& description, int masse);

    // Destructeur
    ~ColisStandard() override = default;

    // Implémentation de l'affichage polymorphe
    void afficher() const override;
};

#endif // COLIS_STANDARD_H
