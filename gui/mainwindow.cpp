#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogcreervehicule.h"
#include "dialogcreercolis.h"
#include "dialogdetailsvehicule.h"
#include <QProgressBar>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include "../Drone.h"
#include "../CamionnetteElectrique.h"
#include "../RobotTrottoir.h"
#include "../ColisFragile.h"
#include "../ColisRefrigere.h"
#include "../ColisExpress.h"
#include "../ColisStandard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Configuration de la fenêtre
    setWindowTitle("Gestion de Flotte de Livraison Autonome");
    resize(1200, 700);
    
    // Connexions des signaux/slots
    setupConnections();
    
    // Message de bienvenue
    QMessageBox::information(this, "Bienvenue", 
        "Bienvenue dans le système de gestion de flotte!\n\n"
        "Version Qt - Interface Graphique\n"
        "Gérez vos drones, camionnettes et robots autonomes.");
    
    // Rafraîchir l'interface
    rafraichirVehicules();
    rafraichirColis();
    rafraichirStatistiques();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // Connexions du menu
    connect(ui->actionAPropos, &QAction::triggered, this, &MainWindow::onAPropos);
    
    // Connexions de la toolbar
    connect(ui->btnCreerVehicule, &QPushButton::clicked, this, &MainWindow::onCreerVehicule);
    connect(ui->btnCreerColis, &QPushButton::clicked, this, &MainWindow::onCreerColis);
    connect(ui->btnAssigner, &QPushButton::clicked, this, &MainWindow::onAssignerColis);
    connect(ui->btnLivrer, &QPushButton::clicked, this, &MainWindow::onLivrerVehicule);
    connect(ui->btnSupprimer, &QPushButton::clicked, this, &MainWindow::onSupprimerVehicule);
    connect(ui->btnDetails, &QPushButton::clicked, this, &MainWindow::onAfficherDetails);
}

void MainWindow::rafraichirVehicules()
{
    // Désactiver le tri pendant la mise à jour
    ui->tableVehicules->setSortingEnabled(false);
    
    // Vider le tableau
    ui->tableVehicules->setRowCount(0);
    
    // Parcourir tous les véhicules de la flotte
    int nbVehicules = static_cast<int>(flotte_.getNombreVehicules());
    
    for (int i = 0; i < nbVehicules; ++i) {
        const Vehicule* v = flotte_.getVehicule(static_cast<size_t>(i));
        if (!v) continue;
        
        // Ajouter une nouvelle ligne
        int row = ui->tableVehicules->rowCount();
        ui->tableVehicules->insertRow(row);
        
        // Déterminer le type de véhicule avec dynamic_cast
        QString type;
        QColor couleurType;
        
        if (dynamic_cast<const Drone*>(v)) {
            type = "Drone";
            couleurType = QColor("#2e7d32");  // Vert
        } else if (dynamic_cast<const CamionnetteElectrique*>(v)) {
            type = "Camionnette";
            couleurType = QColor("#0d47a1");  // Bleu
        } else if (dynamic_cast<const RobotTrottoir*>(v)) {
            type = "Robot";
            couleurType = QColor("#424242");  // Gris
        } else {
            type = "Inconnu";
            couleurType = QColor("#757575");
        }
        
        // Colonne 0: ID
        QTableWidgetItem* itemId = new QTableWidgetItem(QString::number(v->getId()));
        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tableVehicules->setItem(row, 0, itemId);
        
        // Colonne 1: Nom
        QTableWidgetItem* itemNom = new QTableWidgetItem(QString::fromStdString(v->getNom()));
        ui->tableVehicules->setItem(row, 1, itemNom);
        
        // Colonne 2: Type (coloré selon le type)
        QTableWidgetItem* itemType = new QTableWidgetItem(type);
        itemType->setForeground(QBrush(couleurType));
        itemType->setTextAlignment(Qt::AlignCenter);
        ui->tableVehicules->setItem(row, 2, itemType);
        
        // Colonne 3: État
        QString etatStr = QString::fromStdString(v->etatToString(v->getEtat()));
        QTableWidgetItem* itemEtat = new QTableWidgetItem(etatStr);
        itemEtat->setTextAlignment(Qt::AlignCenter);
        
        // Colorier selon l'état
        if (v->getEtat() == Etat::EN_SERVICE) {
            itemEtat->setForeground(QBrush(QColor("#2e7d32")));  // Vert
        } else if (v->getEtat() == Etat::HORS_SERVICE) {
            itemEtat->setForeground(QBrush(QColor("#c62828")));  // Rouge
        } else {
            itemEtat->setForeground(QBrush(QColor("#0d47a1")));  // Bleu
        }
        
        ui->tableVehicules->setItem(row, 3, itemEtat);
        
        // Colonne 4: Capacité (format: X.XX / Y.YY kg)
        double capaciteUtilisee = v->getCapaciteUtilisee() / 1000.0;  // Conversion g -> kg
        double capaciteMax = v->getCapaciteMax() / 1000.0;
        QString capacite = QString("%1 / %2 kg")
                              .arg(capaciteUtilisee, 0, 'f', 2)
                              .arg(capaciteMax, 0, 'f', 2);
        
        QTableWidgetItem* itemCapacite = new QTableWidgetItem(capacite);
        itemCapacite->setTextAlignment(Qt::AlignCenter);
        
        // Tooltip détaillé
        double capaciteDispo = capaciteMax - capaciteUtilisee;
        QString tooltip = QString("Capacité maximale: %1 kg\nUtilisée: %2 kg\nDisponible: %3 kg")
                             .arg(capaciteMax, 0, 'f', 2)
                             .arg(capaciteUtilisee, 0, 'f', 2)
                             .arg(capaciteDispo, 0, 'f', 2);
        itemCapacite->setToolTip(tooltip);
        
        ui->tableVehicules->setItem(row, 4, itemCapacite);
        
        // Colonne 5: Barre de progression de charge
        QWidget* widgetBarre = new QWidget();
        QHBoxLayout* layoutBarre = new QHBoxLayout(widgetBarre);
        layoutBarre->setContentsMargins(4, 2, 4, 2);
        
        QProgressBar* progressBar = new QProgressBar();
        int pourcentage = (capaciteMax > 0) ? (int)((capaciteUtilisee / capaciteMax) * 100) : 0;
        progressBar->setValue(pourcentage);
        progressBar->setFormat(QString("%1%").arg(pourcentage));
        progressBar->setTextVisible(true);
        progressBar->setMaximumHeight(20);
        
        // Couleur selon le taux de remplissage
        QString couleurBarre;
        if (pourcentage < 50) {
            couleurBarre = "#2e7d32";  // Vert
        } else if (pourcentage < 80) {
            couleurBarre = "#f57c00";  // Orange
        } else if (pourcentage < 95) {
            couleurBarre = "#ff6f00";  // Orange foncé
        } else {
            couleurBarre = "#c62828";  // Rouge
        }
        
        progressBar->setStyleSheet(
            QString("QProgressBar {"
                    "   border: 1px solid #3a3a3a;"
                    "   border-radius: 3px;"
                    "   text-align: center;"
                    "   background-color: #1e1e1e;"
                    "   color: #e0e0e0;"
                    "   font-size: 10px;"
                    "   font-weight: bold;"
                    "}"
                    "QProgressBar::chunk {"
                    "   background-color: %1;"
                    "   border-radius: 2px;"
                    "}").arg(couleurBarre)
        );
        
        progressBar->setToolTip(tooltip);
        layoutBarre->addWidget(progressBar);
        
        ui->tableVehicules->setCellWidget(row, 5, widgetBarre);
        
        // Colonne 6: Nombre de colis
        QTableWidgetItem* itemNbColis = new QTableWidgetItem(
            QString::number(v->getNombreColis())
        );
        itemNbColis->setTextAlignment(Qt::AlignCenter);
        itemNbColis->setToolTip(QString("%1 colis à bord").arg(v->getNombreColis()));
        ui->tableVehicules->setItem(row, 6, itemNbColis);
    }
    
    // Réactiver le tri
    ui->tableVehicules->setSortingEnabled(true);
    
    // Ajuster les tailles de colonnes
    ui->tableVehicules->resizeColumnToContents(0);  // ID
    ui->tableVehicules->resizeColumnToContents(2);  // Type
    ui->tableVehicules->resizeColumnToContents(3);  // État
    ui->tableVehicules->resizeColumnToContents(4);  // Capacité
    ui->tableVehicules->setColumnWidth(5, 120);     // Barre de progression
    ui->tableVehicules->resizeColumnToContents(6);  // Nb Colis
}

void MainWindow::rafraichirColis()
{
    // Désactiver le tri pendant la mise à jour
    ui->tableColisAttente->setSortingEnabled(false);
    
    // Vider le tableau
    ui->tableColisAttente->setRowCount(0);
    
    // Récupérer le nombre de colis en attente
    size_t nbColis = flotte_.getNombreColisEnAttente();
    
    // Ajouter chaque colis
    for (size_t i = 0; i < nbColis; ++i) {
        const Colis* c = flotte_.getColis(i);
        if (!c) continue;
        
        int row = ui->tableColisAttente->rowCount();
        ui->tableColisAttente->insertRow(row);
        
        // Déterminer le type de colis avec dynamic_cast
        QString type = "Standard";
        QString details = "-";
        
        if (auto* fragile = dynamic_cast<const ColisFragile*>(c)) {
            type = "Fragile";
            // Obtenir le niveau de fragilité
            switch (fragile->getFragilite()) {
                case Fragilite::FAIBLE:
                    details = "Fragilité: Faible";
                    break;
                case Fragilite::MOYEN:
                    details = "Fragilité: Moyen";
                    break;
                case Fragilite::ELEVE:
                    details = "Fragilité: Élevé";
                    break;
            }
        } else if (auto* refrigere = dynamic_cast<const ColisRefrigere*>(c)) {
            type = "Réfrigéré";
            details = QString("Temp: %1°C").arg(refrigere->getTemperatureCible(), 0, 'f', 1);
        } else if (dynamic_cast<const ColisExpress*>(c)) {
            type = "Express";
            details = "URGENT";
        }
        
        // Colonne 0: Index
        QTableWidgetItem* itemIndex = new QTableWidgetItem(QString::number(i));
        itemIndex->setTextAlignment(Qt::AlignCenter);
        ui->tableColisAttente->setItem(row, 0, itemIndex);
        
        // Colonne 1: Description
        QTableWidgetItem* itemDesc = new QTableWidgetItem(
            QString::fromStdString(c->getDescription())
        );
        QString tooltipDesc = QString("Description: %1\nMasse: %2 kg\nType: %3")
                                 .arg(QString::fromStdString(c->getDescription()))
                                 .arg(c->getMasse() / 1000.0, 0, 'f', 2)
                                 .arg(type);
        if (details != "-") {
            tooltipDesc += "\n" + details;
        }
        itemDesc->setToolTip(tooltipDesc);
        ui->tableColisAttente->setItem(row, 1, itemDesc);
        
        // Colonne 2: Masse (en kg)
        double masseKg = c->getMasse() / 1000.0;
        QTableWidgetItem* itemMasse = new QTableWidgetItem(
            QString("%1 kg").arg(masseKg, 0, 'f', 2)
        );
        itemMasse->setTextAlignment(Qt::AlignCenter);
        itemMasse->setToolTip(QString("Masse: %1 kg (%2 g)")
                                .arg(masseKg, 0, 'f', 2)
                                .arg(c->getMasse()));
        ui->tableColisAttente->setItem(row, 2, itemMasse);
        
        // Colonne 3: Type (colorié selon le type)
        QTableWidgetItem* itemType = new QTableWidgetItem(type);
        itemType->setTextAlignment(Qt::AlignCenter);
        itemType->setToolTip(QString("Type de colis: %1").arg(type));
        
        // Colorier selon le type
        if (type == "Fragile") {
            itemType->setForeground(QBrush(QColor("#c62828")));  // Rouge
        } else if (type == "Réfrigéré") {
            itemType->setForeground(QBrush(QColor("#0d47a1")));  // Bleu
        } else if (type == "Express") {
            itemType->setForeground(QBrush(QColor("#ff6f00")));  // Orange
        } else {
            itemType->setForeground(QBrush(QColor("#757575")));  // Gris
        }
        
        ui->tableColisAttente->setItem(row, 3, itemType);
        
        // Colonne 4: Détails
        QTableWidgetItem* itemDetails = new QTableWidgetItem(details);
        itemDetails->setTextAlignment(Qt::AlignCenter);
        if (details != "-") {
            itemDetails->setToolTip(details);
        }
        ui->tableColisAttente->setItem(row, 4, itemDetails);
    }
    
    // Réactiver le tri
    ui->tableColisAttente->setSortingEnabled(true);
    
    // Ajuster la largeur des colonnes
    ui->tableColisAttente->resizeColumnsToContents();
}

void MainWindow::rafraichirStatistiques()
{
    // Mise à jour de la barre de statut
    int nbVehicules = flotte_.getNombreVehicules();
    int nbColis = flotte_.getNombreColisEnAttente();
    
    QString stats = QString("Véhicules: %1 | Colis en attente: %2")
                        .arg(nbVehicules)
                        .arg(nbColis);
    
    statusBar()->showMessage(stats);
}

// ========== SLOTS DU MENU ==========

void MainWindow::onAPropos()
{
    QMessageBox::about(this, "À propos",
        "<h2>Gestion de Flotte de Livraison Autonome</h2>"
        "<p>Version 1.0 - Interface Qt</p>"
        "<p>Système de gestion pour véhicules autonomes:</p>"
        "<ul>"
        "<li>🚁 Drones (5 kg max)</li>"
        "<li>🚚 Camionnettes électriques (500 kg max)</li>"
        "<li>🤖 Robots de trottoir (20 kg max)</li>"
        "</ul>"
        "<p>Développé avec C++17 et Qt Widgets</p>");
}

// ========== SLOTS DE LA TOOLBAR ==========

void MainWindow::onCreerVehicule()
{
    DialogCreerVehicule dialog(this);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Récupérer le véhicule créé
        auto vehicule = dialog.getVehicule();
        
        if (vehicule) {
            // Ajouter à la flotte
            flotte_.ajouterVehicule(std::move(vehicule));
            
            // Rafraîchir l'affichage
            rafraichirVehicules();
            rafraichirStatistiques();
            
            statusBar()->showMessage("Véhicule créé avec succès", 3000);
        }
    }
}

void MainWindow::onCreerColis()
{
    DialogCreerColis dialog(this);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Récupérer le colis créé
        auto colis = dialog.getColis();
        
        if (colis) {
            // Ajouter à la flotte (file d'attente)
            flotte_.recevoirColis(std::move(colis));
            
            // Rafraîchir l'affichage
            rafraichirColis();
            rafraichirStatistiques();
            
            statusBar()->showMessage("Colis ajouté en attente d'assignation", 3000);
        }
    }
}

void MainWindow::onAssignerColis()
{
    // Vérifier qu'un colis est sélectionné
    QList<QTableWidgetItem*> selectedColis = ui->tableColisAttente->selectedItems();
    if (selectedColis.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", 
            "Veuillez sélectionner un colis dans la liste d'attente.");
        return;
    }
    
    // Vérifier qu'un véhicule est sélectionné
    QList<QTableWidgetItem*> selectedVehicules = ui->tableVehicules->selectedItems();
    if (selectedVehicules.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", 
            "Veuillez sélectionner un véhicule dans la flotte.");
        return;
    }
    
    // Récupérer l'index du colis (colonne 0)
    int rowColis = ui->tableColisAttente->currentRow();
    if (rowColis < 0) return;
    
    int indexColis = ui->tableColisAttente->item(rowColis, 0)->text().toInt();
    
    // Récupérer l'index du véhicule (ligne sélectionnée)
    int indexVehicule = ui->tableVehicules->currentRow();
    if (indexVehicule < 0) return;
    
    // Tenter l'assignation
    if (flotte_.assignerColis(indexColis, indexVehicule)) {
        // Succès
        rafraichirVehicules();
        rafraichirColis();
        rafraichirStatistiques();
        
        statusBar()->showMessage("Colis assigné avec succès", 3000);
    } else {
        // Échec
        QMessageBox::warning(this, "Assignation impossible", 
            "Le colis n'a pas pu être assigné au véhicule.\n"
            "Vérifiez que le véhicule a suffisamment de capacité disponible.");
    }
}

void MainWindow::onLivrerVehicule()
{
    // Vérifier qu'un véhicule est sélectionné
    QList<QTableWidgetItem*> selectedVehicules = ui->tableVehicules->selectedItems();
    if (selectedVehicules.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", 
            "Veuillez sélectionner un véhicule dans la flotte.");
        return;
    }
    
    // Récupérer l'index du véhicule
    int indexVehicule = ui->tableVehicules->currentRow();
    if (indexVehicule < 0) return;
    
    // Vérifier que le véhicule a des colis
    const Vehicule* v = flotte_.getVehicule(indexVehicule);
    if (!v) return;
    
    if (v->getNombreColis() == 0) {
        QMessageBox::information(this, "Aucun colis", 
            "Ce véhicule ne transporte aucun colis.");
        return;
    }
    
    // Demander confirmation
    QString nomVehicule = QString::fromStdString(v->getNom());
    int nbColis = v->getNombreColis();
    
    QMessageBox::StandardButton reponse = QMessageBox::question(this, 
        "Confirmer la livraison",
        QString("Voulez-vous livrer les %1 colis du véhicule '%2' ?").arg(nbColis).arg(nomVehicule),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reponse == QMessageBox::Yes) {
        // Effectuer la livraison
        flotte_.livrerVehicule(indexVehicule);
        
        // Rafraîchir
        rafraichirVehicules();
        rafraichirStatistiques();
        
        statusBar()->showMessage(
            QString("Livraison effectuée : %1 colis livrés").arg(nbColis), 
            3000
        );
    }
}

void MainWindow::onSupprimerVehicule()
{
    // Vérifier qu'un véhicule est sélectionné
    QList<QTableWidgetItem*> selectedVehicules = ui->tableVehicules->selectedItems();
    if (selectedVehicules.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", 
            "Veuillez sélectionner un véhicule dans la flotte.");
        return;
    }
    
    // Récupérer l'index du véhicule
    int indexVehicule = ui->tableVehicules->currentRow();
    if (indexVehicule < 0) return;
    
    // Récupérer les infos du véhicule
    const Vehicule* v = flotte_.getVehicule(indexVehicule);
    if (!v) return;
    
    QString nomVehicule = QString::fromStdString(v->getNom());
    int nbColis = v->getNombreColis();
    
    // Vérifier si le véhicule a des colis
    if (nbColis > 0) {
        QMessageBox::warning(this, "Suppression impossible", 
            QString("Le véhicule '%1' transporte encore %2 colis.\n"
                   "Livrez ou retirez les colis avant de supprimer le véhicule.")
                   .arg(nomVehicule).arg(nbColis));
        return;
    }
    
    // Demander confirmation
    QMessageBox::StandardButton reponse = QMessageBox::question(this, 
        "Confirmer la suppression",
        QString("Voulez-vous vraiment supprimer le véhicule '%1' ?").arg(nomVehicule),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reponse == QMessageBox::Yes) {
        // Supprimer le véhicule
        if (flotte_.supprimerVehicule(indexVehicule)) {
            // Rafraîchir
            rafraichirVehicules();
            rafraichirStatistiques();
            
            statusBar()->showMessage(
                QString("Véhicule '%1' supprimé de la flotte").arg(nomVehicule), 
                3000
            );
        }
    }
}

void MainWindow::onAfficherDetails()
{
    // Vérifier qu'un véhicule est sélectionné
    QList<QTableWidgetItem*> selectedVehicules = ui->tableVehicules->selectedItems();
    if (selectedVehicules.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", 
            "Veuillez sélectionner un véhicule dans la flotte.");
        return;
    }
    
    // Récupérer l'index du véhicule
    int indexVehicule = ui->tableVehicules->currentRow();
    if (indexVehicule < 0) return;
    
    // Récupérer le véhicule
    const Vehicule* v = flotte_.getVehicule(indexVehicule);
    if (!v) return;
    
    // Ouvrir le dialogue des détails
    DialogDetailsVehicule dialogue(v, this);
    dialogue.exec();
}
