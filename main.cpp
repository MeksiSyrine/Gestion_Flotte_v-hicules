#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <iomanip>

#include "Flotte.h"
#include "Drone.h"
#include "CamionnetteElectrique.h"
#include "RobotTrottoir.h"
#include "ColisStandard.h"
#include "ColisFragile.h"
#include "ColisRefrigere.h"
#include "ColisExpress.h"

/**
 * Menu interactif pour gérer une flotte de véhicules autonomes
 * 
 * Fonctionnalités :
 * - Création/suppression de véhicules
 * - Réception de colis
 * - Assignation et livraison
 * - Gestion robuste des erreurs
 */

// ============================================================================
// PROTOTYPES DES FONCTIONS
// ============================================================================

void afficherMenu();
void creerVehicule(Flotte& flotte);
void supprimerVehicule(Flotte& flotte);
void recevoirColis(Flotte& flotte);
void afficherColisEnAttente(Flotte& flotte);
void assignerColisAVehicule(Flotte& flotte);
void retirerDernierColis(Flotte& flotte);
void livrerVehicule(Flotte& flotte);
void afficherListeVehicules(Flotte& flotte);
void afficherDetailsVehicule(Flotte& flotte);

// ============================================================================
// FONCTION AFFICHAGE DU MENU
// ============================================================================

void afficherMenu() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "      GESTION DE FLOTTE DE LIVRAISON AUTONOME\n";
    std::cout << "============================================================\n";
    std::cout << "1. Creer un vehicule (Drone / Camionnette / Robot)\n";
    std::cout << "2. Supprimer un vehicule\n";
    std::cout << "3. Reception d'un colis\n";
    std::cout << "4. Liste des colis en attente de livraison\n";
    std::cout << "5. Ajouter un colis (en attente) a un vehicule\n";
    std::cout << "6. Retirer le dernier colis d'un vehicule\n";
    std::cout << "7. Livrer les colis d'un vehicule\n";
    std::cout << "8. Afficher la liste de tous les vehicules\n";
    std::cout << "9. Afficher les details d'un vehicule\n";
    std::cout << "0. Quitter\n";
    std::cout << "============================================================\n";
    std::cout << "Votre choix : ";
}

// ============================================================================
// OPTION 1 : CRÉER UN VÉHICULE
// ============================================================================

void creerVehicule(Flotte& flotte) {
    std::cout << "\n--- CREATION D'UN VEHICULE ---\n";
    std::cout << "Type de vehicule :\n";
    std::cout << "  1. Drone (capacite: 5 kg)\n";
    std::cout << "  2. Camionnette electrique (capacite: 500 kg)\n";
    std::cout << "  3. Robot de trottoir (capacite: 20 kg)\n";
    std::cout << "Votre choix : ";
    
    int type;
    std::cin >> type;
    
    // Vérifier erreur de saisie
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Nom du vehicule : ";
    std::string nom;
    std::getline(std::cin, nom);
    
    if (nom.empty()) {
        std::cout << "ERREUR : Le nom ne peut pas etre vide.\n";
        return;
    }
    
    try {
        switch(type) {
            case 1: {
                std::cout << "Autonomie (km) : ";
                int autonomie;
                std::cin >> autonomie;
                
                if (std::cin.fail() || autonomie <= 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "ERREUR : Autonomie invalide.\n";
                    return;
                }
                
                flotte.ajouterVehicule(std::make_unique<Drone>(nom, autonomie));
                break;
            }
            case 2: {
                flotte.ajouterVehicule(std::make_unique<CamionnetteElectrique>(nom));
                break;
            }
            case 3: {
                flotte.ajouterVehicule(std::make_unique<RobotTrottoir>(nom));
                break;
            }
            default:
                std::cout << "ERREUR : Type de vehicule invalide.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "ERREUR : " << e.what() << "\n";
    }
}

// ============================================================================
// OPTION 2 : SUPPRIMER UN VÉHICULE
// ============================================================================

void supprimerVehicule(Flotte& flotte) {
    std::cout << "\n--- SUPPRESSION D'UN VEHICULE ---\n";
    
    if (flotte.getNombreVehicules() == 0) {
        std::cout << "Aucun vehicule dans la flotte.\n";
        return;
    }
    
    flotte.afficherListeVehicules();
    
    std::cout << "\nIndex du vehicule a supprimer : ";
    size_t index;
    std::cin >> index;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    flotte.supprimerVehicule(index);
}

// ============================================================================
// OPTION 3 : RÉCEPTION D'UN COLIS
// ============================================================================

void recevoirColis(Flotte& flotte) {
    std::cout << "\n--- RECEPTION D'UN COLIS ---\n";
    std::cout << "Type de colis :\n";
    std::cout << "  1. Standard\n";
    std::cout << "  2. Fragile\n";
    std::cout << "  3. Refrigere\n";
    std::cout << "  4. Express\n";
    std::cout << "Votre choix : ";
    
    int type;
    std::cin >> type;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Description : ";
    std::string description;
    std::getline(std::cin, description);
    
    if (description.empty()) {
        std::cout << "ERREUR : La description ne peut pas etre vide.\n";
        return;
    }
    
    std::cout << "Masse (grammes) : ";
    int masse;
    std::cin >> masse;
    
    if (std::cin.fail() || masse <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Masse invalide.\n";
        return;
    }
    
    try {
        switch(type) {
            case 1: {
                flotte.recevoirColis(std::make_unique<ColisStandard>(description, masse));
                break;
            }
            case 2: {
                std::cout << "Niveau de fragilite :\n";
                std::cout << "  1. Faible\n";
                std::cout << "  2. Moyen\n";
                std::cout << "  3. Eleve\n";
                std::cout << "Votre choix : ";
                int frag;
                std::cin >> frag;
                
                if (std::cin.fail() || frag < 1 || frag > 3) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "ERREUR : Fragilite invalide.\n";
                    return;
                }
                
                Fragilite fragilite;
                switch(frag) {
                    case 1: fragilite = Fragilite::FAIBLE; break;
                    case 2: fragilite = Fragilite::MOYEN; break;
                    case 3: fragilite = Fragilite::ELEVE; break;
                    default: fragilite = Fragilite::FAIBLE;
                }
                
                flotte.recevoirColis(std::make_unique<ColisFragile>(description, masse, fragilite));
                break;
            }
            case 3: {
                std::cout << "Temperature cible (C) : ";
                float temp;
                std::cin >> temp;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "ERREUR : Temperature invalide.\n";
                    return;
                }
                
                flotte.recevoirColis(std::make_unique<ColisRefrigere>(description, masse, temp));
                break;
            }
            case 4: {
                flotte.recevoirColis(std::make_unique<ColisExpress>(description, masse));
                break;
            }
            default:
                std::cout << "ERREUR : Type de colis invalide.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "ERREUR : " << e.what() << "\n";
    }
}

// ============================================================================
// OPTION 4 : LISTE DES COLIS EN ATTENTE
// ============================================================================

void afficherColisEnAttente(Flotte& flotte) {
    std::cout << "\n";
    flotte.afficherColisEnAttente();
}

// ============================================================================
// OPTION 5 : AJOUTER UN COLIS À UN VÉHICULE
// ============================================================================

void assignerColisAVehicule(Flotte& flotte) {
    std::cout << "\n--- ASSIGNATION D'UN COLIS A UN VEHICULE ---\n";
    
    if (flotte.getNombreColisEnAttente() == 0) {
        std::cout << "Aucun colis en attente.\n";
        return;
    }
    
    if (flotte.getNombreVehicules() == 0) {
        std::cout << "Aucun vehicule dans la flotte.\n";
        return;
    }
    
    // Afficher colis en attente
    flotte.afficherColisEnAttente();
    
    std::cout << "\nIndex du colis : ";
    size_t index_colis;
    std::cin >> index_colis;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    // Afficher véhicules
    std::cout << "\n";
    flotte.afficherListeVehicules();
    
    std::cout << "\nIndex du vehicule : ";
    size_t index_vehicule;
    std::cin >> index_vehicule;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    flotte.assignerColis(index_colis, index_vehicule);
}

// ============================================================================
// OPTION 6 : RETIRER LE DERNIER COLIS D'UN VÉHICULE
// ============================================================================

void retirerDernierColis(Flotte& flotte) {
    std::cout << "\n--- RETRAIT DU DERNIER COLIS ---\n";
    
    if (flotte.getNombreVehicules() == 0) {
        std::cout << "Aucun vehicule dans la flotte.\n";
        return;
    }
    
    flotte.afficherListeVehicules();
    
    std::cout << "\nIndex du vehicule : ";
    size_t index;
    std::cin >> index;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    flotte.retirerDernierColisVehicule(index);
}

// ============================================================================
// OPTION 7 : LIVRER LES COLIS D'UN VÉHICULE
// ============================================================================

void livrerVehicule(Flotte& flotte) {
    std::cout << "\n--- LIVRAISON ---\n";
    
    if (flotte.getNombreVehicules() == 0) {
        std::cout << "Aucun vehicule dans la flotte.\n";
        return;
    }
    
    flotte.afficherListeVehicules();
    
    std::cout << "\nIndex du vehicule a livrer : ";
    size_t index;
    std::cin >> index;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    flotte.livrerVehicule(index);
}

// ============================================================================
// OPTION 8 : AFFICHER LA LISTE DE TOUS LES VÉHICULES
// ============================================================================

void afficherListeVehicules(Flotte& flotte) {
    std::cout << "\n";
    flotte.afficherListeVehicules();
}

// ============================================================================
// OPTION 9 : AFFICHER LES DÉTAILS D'UN VÉHICULE
// ============================================================================

void afficherDetailsVehicule(Flotte& flotte) {
    std::cout << "\n--- DETAILS D'UN VEHICULE ---\n";
    
    if (flotte.getNombreVehicules() == 0) {
        std::cout << "Aucun vehicule dans la flotte.\n";
        return;
    }
    
    flotte.afficherListeVehicules();
    
    std::cout << "\nIndex du vehicule : ";
    size_t index;
    std::cin >> index;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERREUR : Saisie invalide.\n";
        return;
    }
    
    std::cout << "\n";
    flotte.afficherDetailsVehicule(index);
}

// ============================================================================
// FONCTION PRINCIPALE
// ============================================================================

int main() {
    Flotte flotte;
    int choix = -1;
    
    // En-tête de bienvenue
    std::cout << "\n";
    std::cout << "================================================================\n";
    std::cout << "||  BIENVENUE DANS LE SYSTEME DE GESTION DE FLOTTE           ||\n";
    std::cout << "||  Livraison Autonome - Gestion Intelligente                ||\n";
    std::cout << "================================================================\n";
    std::cout << "\nVersion 1.0 - C++17\n";
    std::cout << "Gestion de drones, camionnettes et robots autonomes\n";
    
    // Boucle principale
    while (choix != 0) {
        afficherMenu();
        std::cin >> choix;
        
        // Gestion des erreurs de saisie
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n ERREUR : Veuillez entrer un nombre valide.\n";
            std::cout << "\nAppuyez sur Entree pour continuer...";
            std::cin.get();
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Exécution de l'action choisie
        switch(choix) {
            case 1:
                creerVehicule(flotte);
                break;
            case 2:
                supprimerVehicule(flotte);
                break;
            case 3:
                recevoirColis(flotte);
                break;
            case 4:
                afficherColisEnAttente(flotte);
                break;
            case 5:
                assignerColisAVehicule(flotte);
                break;
            case 6:
                retirerDernierColis(flotte);
                break;
            case 7:
                livrerVehicule(flotte);
                break;
            case 8:
                afficherListeVehicules(flotte);
                break;
            case 9:
                afficherDetailsVehicule(flotte);
                break;
            case 0:
                std::cout << "\n============================================================\n";
                std::cout << "  Merci d'avoir utilise le systeme de gestion de flotte.\n";
                std::cout << "  Au revoir !\n";
                std::cout << "============================================================\n";
                break;
            default:
                std::cout << "\n ERREUR : Choix invalide. Veuillez choisir entre 0 et 9.\n";
        }
        
        // Pause avant de réafficher le menu (sauf si on quitte)
        if (choix != 0) {
            std::cout << "\nAppuyez sur Entree pour continuer...";
            std::cin.get();
        }
    }
    
    return 0;
}
