#ifndef COLIS_EXPRESS_H
#define COLIS_EXPRESS_H

#include "Colis.h"

/**
 * Colis express : livraison prioritaire et urgente
 */
class ColisExpress : public Colis {
public:
    // Constructeur
    ColisExpress(const std::string& description, int masse);

    // Destructeur
    ~ColisExpress() override = default;

    // Implémentation de l'affichage polymorphe avec mention URGENT
    void afficher() const override;
};

#endif // COLIS_EXPRESS_H
