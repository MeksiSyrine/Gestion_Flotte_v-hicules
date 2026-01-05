#ifndef COLIS_FRAGILE_H
#define COLIS_FRAGILE_H

#include "Colis.h"

/**
 * Énumération pour le niveau de fragilité
 */
enum class Fragilite {
    FAIBLE,
    MOYEN,
    ELEVE
};

/**
 * Colis fragile : nécessite une attention particulière lors du transport
 */
class ColisFragile : public Colis {
private:
    Fragilite fragilite_;

public:
    // Constructeur
    ColisFragile(const std::string& description, int masse, Fragilite fragilite);

    // Destructeur
    ~ColisFragile() override = default;

    // Getter
    Fragilite getFragilite() const;

    // Implémentation de l'affichage polymorphe
    void afficher() const override;

private:
    // Méthode utilitaire pour convertir enum en string
    std::string fragiliteToString() const;
};

#endif // COLIS_FRAGILE_H
