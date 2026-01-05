#ifndef COLIS_REFRIGERE_H
#define COLIS_REFRIGERE_H

#include "Colis.h"

/**
 * Colis réfrigéré : nécessite le maintien d'une température spécifique
 */
class ColisRefrigere : public Colis {
private:
    float temperature_cible_;  // en degrés Celsius

public:
    // Constructeur
    ColisRefrigere(const std::string& description, int masse, float temperature_cible);

    // Destructeur
    ~ColisRefrigere() override = default;

    // Getter
    float getTemperatureCible() const;

    // Implémentation de l'affichage
    void afficher() const override;
};

#endif // COLIS_REFRIGERE_H
