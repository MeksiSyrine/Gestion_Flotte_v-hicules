#ifndef ROBOT_TROTTOIR_H
#define ROBOT_TROTTOIR_H

#include "Vehicule.h"

/**
 * RobotTrottoir : petit robot autonome circulant sur les trottoirs
 * avec capacité moyenne (20kg) pour livraisons locales
 */
class RobotTrottoir : public Vehicule {
public:
    // Constructeur (capacité fixée à 20000g = 20kg)
    RobotTrottoir(const std::string& nom);

    // Destructeur
    ~RobotTrottoir() override = default;

    // Implémentation de l'affichage polymorphe
    void afficher() const override;
};

#endif // ROBOT_TROTTOIR_H
