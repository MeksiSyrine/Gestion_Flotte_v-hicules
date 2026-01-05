#ifndef VEHICULE_H
#define VEHICULE_H

#include <string>
#include <vector>
#include <memory>
#include "Colis.h"

/**
 * Énumération pour l'état du véhicule
 */
enum class Etat {
    EN_SERVICE,
    HORS_SERVICE,
    ATTENTE_LIVRAISON
};

/**
 * Classe abstraite de base pour tous les types de véhicules.
 * Gère une collection de colis avec unique_ptr et utilise le polymorphisme
 * pour permettre différents types de véhicules avec des capacités variées.
 */
class Vehicule {
private:
    static int compteur_id_;  // Compteur statique pour générer des ID uniques
    int id_;
    std::string nom_;
    int capacite_max_;  // en grammes
    Etat etat_;
    std::vector<std::unique_ptr<Colis>> colis_;  // Collection polymorphe de colis

public:
    // Constructeur
    Vehicule(const std::string& nom, int capacite_max);

    // Destructeur virtuel (ESSENTIEL pour le polymorphisme)
    virtual ~Vehicule() = default;

    // Interdiction de la copie (les véhicules sont des ressources uniques)
    Vehicule(const Vehicule&) = delete;
    Vehicule& operator=(const Vehicule&) = delete;

    // Gestion des colis
    bool ajouterColis(std::unique_ptr<Colis> colis);
    void retirerDernierColis();
    void livrer();
    void afficherColis() const;

    // Gestion de l'état
    void changerEtat(Etat nouvel_etat);
    Etat getEtat() const;

    // Getters const
    int getId() const;
    const std::string& getNom() const;
    int getCapaciteMax() const;
    int getCapaciteUtilisee() const;
    size_t getNombreColis() const;

    // Méthode helper pour conversion Etat -> string
    std::string etatToString(Etat e) const;

    // Méthode virtuelle pure pour affichage polymorphe
    virtual void afficher() const = 0;
};

#endif // VEHICULE_H
