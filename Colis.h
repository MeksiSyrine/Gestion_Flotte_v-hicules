#ifndef COLIS_H
#define COLIS_H

#include <string>

/**
 * Classe abstraite de base pour tous les types de colis.
 * Utilise le polymorphisme pour permettre différents types de colis
 * avec des comportements d'affichage spécifiques.
 */
class Colis {
private:
    std::string description_;
    int masse_;  // en grammes

public:
    // Constructeur
    Colis(const std::string& description, int masse);

    // Destructeur
    virtual ~Colis() = default;

    // Interdiction de la copie (les colis sont des ressources uniques)
    Colis(const Colis&) = delete;
    Colis& operator=(const Colis&) = delete;

    // Getters + const
    const std::string& getDescription() const;
    int getMasse() const;

    // Méthode virtuelle pure pour l'affichage
    virtual void afficher() const = 0;
};

#endif // COLIS_H
