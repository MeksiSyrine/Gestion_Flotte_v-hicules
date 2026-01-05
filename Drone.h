#ifndef DRONE_H
#define DRONE_H

#include "Vehicule.h"

/**
 * Drone : véhicule aérien autonome avec capacité limitée (5kg)
 * et autonomie de vol en kilomètres
 */
class Drone : public Vehicule {
private:
    int autonomie_km_;

public:
    // Constructeur (capacité fixée à 5000g)
    Drone(const std::string& nom, int autonomie_km);

    // Destructeur
    ~Drone() override = default;

    // Getter
    int getAutonomie() const;

    // Implémentation de l'affichage polymorphe
    void afficher() const override;
};

#endif // DRONE_H
