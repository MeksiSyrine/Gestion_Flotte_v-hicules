#include "Flotte.h"
#include <iostream>
#include <iomanip>

// ========================================
// GESTION DES VÉHICULES
// ========================================

void Flotte::ajouterVehicule(std::unique_ptr<Vehicule> vehicule) {
    if (!vehicule) {
        std::cout << "ERREUR : Vehicule invalide (nullptr)" << std::endl;
        return;
    }

    int id = vehicule->getId();
    std::string nom = vehicule->getNom();
    
    vehicules_.push_back(std::move(vehicule));
    
    std::cout << "✓ Vehicule #" << id << " (" << nom 
              << ") ajoute a la flotte" << std::endl;
}

bool Flotte::supprimerVehicule(size_t index) {
    if (index >= vehicules_.size()) {
        std::cout << "ERREUR : Index de vehicule invalide (" 
                  << index << " >= " << vehicules_.size() << ")" << std::endl;
        return false;
    }

    std::cout << "✓ Vehicule #" << vehicules_[index]->getId() 
              << " (" << vehicules_[index]->getNom() 
              << ") supprime de la flotte" << std::endl;
    
    vehicules_.erase(vehicules_.begin() + index);
    return true;
}

size_t Flotte::getNombreVehicules() const {
    return vehicules_.size();
}

void Flotte::afficherListeVehicules() const {
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "LISTE DES VEHICULES DE LA FLOTTE (" 
              << vehicules_.size() << " vehicule(s))" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    if (vehicules_.empty()) {
        std::cout << "  [Aucun vehicule dans la flotte]" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        return;
    }

    for (size_t i = 0; i < vehicules_.size(); ++i) {
        const auto& v = vehicules_[i];
        
        // Format : [Index] #ID - Nom | Type | État | Capacité | Nb colis
        std::cout << "[" << i << "] #" << v->getId() << " - " << v->getNom()
                  << " | " << getTypeVehicule(v.get())
                  << " | " << v->etatToString(v->getEtat())
                  << " | Capacite : " << std::fixed << std::setprecision(2)
                  << (v->getCapaciteUtilisee() / 1000.0) << "/"
                  << (v->getCapaciteMax() / 1000.0) << " kg"
                  << " | " << v->getNombreColis() << " colis" << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

bool Flotte::afficherDetailsVehicule(size_t index) const {
    if (index >= vehicules_.size()) {
        std::cout << "ERREUR : Index de vehicule invalide (" 
                  << index << " >= " << vehicules_.size() << ")" << std::endl;
        return false;
    }

    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "DETAILS DU VEHICULE [" << index << "]" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    vehicules_[index]->afficher();
    
    std::cout << "\nColis transportes :" << std::endl;
    vehicules_[index]->afficherColis();
    
    std::cout << std::string(60, '=') << std::endl;
    
    return true;
}

// ========================================
// GESTION DES COLIS EN ATTENTE
// ========================================

void Flotte::recevoirColis(std::unique_ptr<Colis> colis) {
    if (!colis) {
        std::cout << "ERREUR : Colis invalide (nullptr)" << std::endl;
        return;
    }

    std::cout << "✓ Colis recu : " << colis->getDescription() 
              << " (" << colis->getMasse() << "g) - En attente d'assignation" 
              << std::endl;
    
    colis_en_attente_.push_back(std::move(colis));
}

size_t Flotte::getNombreColisEnAttente() const {
    return colis_en_attente_.size();
}

void Flotte::afficherColisEnAttente() const {
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "COLIS EN ATTENTE DE LIVRAISON (" 
              << colis_en_attente_.size() << " colis)" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    if (colis_en_attente_.empty()) {
        std::cout << "  [Aucun colis en attente]" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        return;
    }

    for (size_t i = 0; i < colis_en_attente_.size(); ++i) {
        const auto& c = colis_en_attente_[i];
        std::cout << "[" << i << "] " << c->getDescription() 
                  << " - " << c->getMasse() << "g" << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

// ========================================
// OPÉRATIONS D'ASSIGNATION ET LIVRAISON
// ========================================

bool Flotte::assignerColis(size_t index_colis, size_t index_vehicule) {
    // Vérification des index
    if (index_colis >= colis_en_attente_.size()) {
        std::cout << "ERREUR : Index de colis invalide (" 
                  << index_colis << " >= " << colis_en_attente_.size() << ")" 
                  << std::endl;
        return false;
    }

    if (index_vehicule >= vehicules_.size()) {
        std::cout << "ERREUR : Index de vehicule invalide (" 
                  << index_vehicule << " >= " << vehicules_.size() << ")" 
                  << std::endl;
        return false;
    }

    std::cout << "\n--- Tentative d'assignation ---" << std::endl;
    std::cout << "Colis   : " << colis_en_attente_[index_colis]->getDescription() 
              << " (" << colis_en_attente_[index_colis]->getMasse() << "g)" 
              << std::endl;
    std::cout << "Vehicule: #" << vehicules_[index_vehicule]->getId() 
              << " - " << vehicules_[index_vehicule]->getNom() << std::endl;

    // Extraire le colis (transfert temporaire)
    auto colis = std::move(colis_en_attente_[index_colis]);
    
    // Essayer de l'ajouter au véhicule
    if (vehicules_[index_vehicule]->ajouterColis(std::move(colis))) {
        // Succès : retirer le slot vide de la file d'attente
        colis_en_attente_.erase(colis_en_attente_.begin() + index_colis);
        std::cout << "✓ Assignation reussie !" << std::endl;
        return true;
    } else {
        // Échec : remettre le colis en attente
        colis_en_attente_[index_colis] = std::move(colis);
        std::cout << "✗ Assignation echouee (capacite insuffisante)" << std::endl;
        return false;
    }
}

bool Flotte::livrerVehicule(size_t index) {
    if (index >= vehicules_.size()) {
        std::cout << "ERREUR : Index de vehicule invalide (" 
                  << index << " >= " << vehicules_.size() << ")" << std::endl;
        return false;
    }

    vehicules_[index]->livrer();
    return true;
}

bool Flotte::retirerDernierColisVehicule(size_t index) {
    if (index >= vehicules_.size()) {
        std::cout << "ERREUR : Index de vehicule invalide (" 
                  << index << " >= " << vehicules_.size() << ")" << std::endl;
        return false;
    }

    vehicules_[index]->retirerDernierColis();
    return true;
}

bool Flotte::changerEtatVehicule(size_t index, Etat nouvel_etat) {
    if (index >= vehicules_.size()) {
        std::cout << "ERREUR : Index de vehicule invalide (" 
                  << index << " >= " << vehicules_.size() << ")" << std::endl;
        return false;
    }

    vehicules_[index]->changerEtat(nouvel_etat);
    
    std::cout << "✓ Etat du vehicule #" << vehicules_[index]->getId() 
              << " change a : " << vehicules_[index]->etatToString(nouvel_etat) 
              << std::endl;
    
    return true;
}

// ========================================
// MÉTHODES HELPER PRIVÉES
// ========================================

std::string Flotte::getTypeVehicule(const Vehicule* vehicule) const {
    if (dynamic_cast<const Drone*>(vehicule)) {
        return "DRONE";
    }
    if (dynamic_cast<const CamionnetteElectrique*>(vehicule)) {
        return "CAMIONNETTE";
    }
    if (dynamic_cast<const RobotTrottoir*>(vehicule)) {
        return "ROBOT";
    }
    return "INCONNU";
}

const Vehicule* Flotte::getVehicule(size_t index) const {
    if (index >= vehicules_.size()) {
        return nullptr;
    }
    return vehicules_[index].get();
}

const Colis* Flotte::getColis(size_t index) const {
    if (index >= colis_en_attente_.size()) {
        return nullptr;
    }
    return colis_en_attente_[index].get();
}
