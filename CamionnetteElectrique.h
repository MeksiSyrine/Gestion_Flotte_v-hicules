#ifndef CAMIONNETTE_ELECTRIQUE_H
#define CAMIONNETTE_ELECTRIQUE_H

#include "Vehicule.h"

/**
 * CamionnetteElectrique : véhicule terrestre avec grande capacité (500kg)
 * pour les livraisons volumineuses
 */
class CamionnetteElectrique : public Vehicule {
public:
    // Constructeur (capacité fixée à 500000g = 500kg)
    CamionnetteElectrique(const std::string& nom);

    // Destructeur
    ~CamionnetteElectrique() override = default;

    // Implémentation de l'affichage polymorphe
    void afficher() const override;
};

#endif // CAMIONNETTE_ELECTRIQUE_H
