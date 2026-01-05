#include "Vehicule.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// Initialisation du compteur statique (commence à 1)
int Vehicule::compteur_id_ = 1;

Vehicule::Vehicule(const std::string& nom, int capacite_max)
    : id_(compteur_id_++), nom_(nom), capacite_max_(capacite_max), 
      etat_(Etat::EN_SERVICE) {
    // Validation : capacité positive
    if (capacite_max <= 0) {
        throw std::invalid_argument("La capacite maximale doit etre positive");
    }
}

bool Vehicule::ajouterColis(std::unique_ptr<Colis> colis) {
    if (!colis) {
        std::cout << "Erreur : colis invalide (nullptr)" << std::endl;
        return false;
    }

    // Vérification de la capacité disponible
    int masse_colis = colis->getMasse();
    int capacite_utilisee = getCapaciteUtilisee();

    if (capacite_utilisee + masse_colis > capacite_max_) {
        std::cout << "ERREUR : Capacite insuffisante !" << std::endl;
        std::cout << "  Capacite utilisee : " << capacite_utilisee << "g" << std::endl;
        std::cout << "  Masse du colis    : " << masse_colis << "g" << std::endl;
        std::cout << "  Capacite max      : " << capacite_max_ << "g" << std::endl;
        std::cout << "  Manquant          : " 
                  << (capacite_utilisee + masse_colis - capacite_max_) << "g" << std::endl;
        return false;
    }

    // Ajout du colis (transfert de propriété avec move)
    colis_.push_back(std::move(colis));
    std::cout << "Colis ajoute avec succes au vehicule #" << id_ << std::endl;
    return true;
}

void Vehicule::retirerDernierColis() {
    if (colis_.empty()) {
        std::cout << "Aucun colis a retirer (vehicule vide)" << std::endl;
        return;
    }

    std::cout << "Retrait du dernier colis du vehicule #" << id_ << std::endl;
    colis_.pop_back();  // Destruction automatique du unique_ptr
}

void Vehicule::livrer() {
    size_t nb_colis = colis_.size();
    
    // Vider tous les colis (destruction automatique)
    colis_.clear();
    
    // Changer l'état
    etat_ = Etat::ATTENTE_LIVRAISON;
    
    std::cout << "=== LIVRAISON EFFECTUEE ===" << std::endl;
    std::cout << "Vehicule #" << id_ << " (" << nom_ << ")" << std::endl;
    std::cout << nb_colis << " colis livre(s) avec succes" << std::endl;
    std::cout << "Nouvel etat : " << etatToString(etat_) << std::endl;
}

void Vehicule::afficherColis() const {
    if (colis_.empty()) {
        std::cout << "  [Aucun colis]" << std::endl;
        return;
    }

    std::cout << "  Liste des " << colis_.size() << " colis :" << std::endl;
    std::cout << "  " << std::string(40, '-') << std::endl;
    
    // Affichage polymorphe : chaque colis affiche ses propres détails
    for (size_t i = 0; i < colis_.size(); ++i) {
        std::cout << "  Colis #" << (i + 1) << ":" << std::endl;
        std::cout << "  ";
        colis_[i]->afficher();  // Appel polymorphe
        if (i < colis_.size() - 1) {
            std::cout << std::endl;
        }
    }
}

void Vehicule::changerEtat(Etat nouvel_etat) {
    etat_ = nouvel_etat;
}

Etat Vehicule::getEtat() const {
    return etat_;
}

int Vehicule::getId() const {
    return id_;
}

const std::string& Vehicule::getNom() const {
    return nom_;
}

int Vehicule::getCapaciteMax() const {
    return capacite_max_;
}

int Vehicule::getCapaciteUtilisee() const {
    int total = 0;
    for (const auto& colis : colis_) {
        total += colis->getMasse();
    }
    return total;
}

size_t Vehicule::getNombreColis() const {
    return colis_.size();
}

std::string Vehicule::etatToString(Etat e) const {
    switch (e) {
        case Etat::EN_SERVICE:          return "En service";
        case Etat::HORS_SERVICE:        return "Hors service";
        case Etat::ATTENTE_LIVRAISON:   return "Attente de livraison";
        default:                        return "Inconnu";
    }
}
