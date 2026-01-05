#ifndef FLOTTE_H
#define FLOTTE_H

#include <vector>
#include <memory>
#include <string>
#include "Vehicule.h"
#include "Drone.h"
#include "CamionnetteElectrique.h"
#include "RobotTrottoir.h"
#include "Colis.h"

/**
 * Classe Flotte : Gestionnaire central de tous les véhicules et colis
 * 
 * Orchestre :
 * - Collection polymorphe de véhicules autonomes
 * - File d'attente de colis en attente d'assignation
 * - Opérations d'assignation, livraison, etc.
 */
class Flotte {
private:
    std::vector<std::unique_ptr<Vehicule>> vehicules_;
    std::vector<std::unique_ptr<Colis>> colis_en_attente_;

public:
    // Constructeur par défaut
    Flotte() = default;

    // Destructeur
    ~Flotte() = default;

    // Interdiction de la copie
    Flotte(const Flotte&) = delete;
    Flotte& operator=(const Flotte&) = delete;

    // === GESTION DES VÉHICULES ===
    void ajouterVehicule(std::unique_ptr<Vehicule> vehicule);
    bool supprimerVehicule(size_t index);
    size_t getNombreVehicules() const;
    void afficherListeVehicules() const;
    bool afficherDetailsVehicule(size_t index) const;
    const Vehicule* getVehicule(size_t index) const;  // Accès lecture seule pour le GUI

    // === GESTION DES COLIS EN ATTENTE ===
    void recevoirColis(std::unique_ptr<Colis> colis);
    size_t getNombreColisEnAttente() const;
    void afficherColisEnAttente() const;
    const Colis* getColis(size_t index) const;  // Accès lecture seule pour le GUI

    // === OPÉRATIONS SUR LES VÉHICULES ===
    bool assignerColis(size_t index_colis, size_t index_vehicule);
    bool livrerVehicule(size_t index);
    bool retirerDernierColisVehicule(size_t index);
    bool changerEtatVehicule(size_t index, Etat nouvel_etat);

private:
    // Méthode helper pour déterminer le type de véhicule
    std::string getTypeVehicule(const Vehicule* vehicule) const;
};

#endif // FLOTTE_H
