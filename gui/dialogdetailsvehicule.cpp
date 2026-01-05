#include "dialogdetailsvehicule.h"
#include "ui_dialogdetailsvehicule.h"
#include "../Drone.h"
#include "../CamionnetteElectrique.h"
#include "../RobotTrottoir.h"
#include "../ColisStandard.h"
#include "../ColisFragile.h"
#include "../ColisRefrigere.h"
#include "../ColisExpress.h"
#include <QHeaderView>

DialogDetailsVehicule::DialogDetailsVehicule(const Vehicule* vehicule, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDetailsVehicule)
    , vehicule_(vehicule)
{
    ui->setupUi(this);
    
    if (vehicule_) {
        afficherInfosVehicule();
        afficherColis();
    }
}

DialogDetailsVehicule::~DialogDetailsVehicule()
{
    delete ui;
}

void DialogDetailsVehicule::afficherInfosVehicule()
{
    // ID et nom
    ui->labelID->setText(QString::number(vehicule_->getId()));
    ui->labelNom->setText(QString::fromStdString(vehicule_->getNom()));
    
    // Type de véhicule
    QString type;
    QColor couleurType;
    if (dynamic_cast<const Drone*>(vehicule_)) {
        type = "Drone";
        couleurType = QColor("#2e7d32"); // Vert
    } else if (dynamic_cast<const CamionnetteElectrique*>(vehicule_)) {
        type = "Camionnette Électrique";
        couleurType = QColor("#0d47a1"); // Bleu
    } else if (dynamic_cast<const RobotTrottoir*>(vehicule_)) {
        type = "Robot Trottoir";
        couleurType = QColor("#757575"); // Gris
    } else {
        type = "Inconnu";
        couleurType = QColor("#424242");
    }
    
    ui->labelType->setText(type);
    ui->labelType->setStyleSheet(
        QString("color: %1; font-weight: bold;").arg(couleurType.name())
    );
    
    // État
    Etat etat = vehicule_->getEtat();
    QString texteEtat;
    QColor couleurEtat;
    
    switch (etat) {
        case Etat::EN_SERVICE:
            texteEtat = "En service";
            couleurEtat = QColor("#2e7d32"); // Vert
            break;
        case Etat::ATTENTE_LIVRAISON:
            texteEtat = "En attente de livraison";
            couleurEtat = QColor("#f57c00"); // Orange
            break;
        case Etat::HORS_SERVICE:
            texteEtat = "Hors service";
            couleurEtat = QColor("#c62828"); // Rouge
            break;
        default:
            texteEtat = "Inconnu";
            couleurEtat = QColor("#424242");
    }
    
    ui->labelEtat->setText(texteEtat);
    ui->labelEtat->setStyleSheet(
        QString("color: %1; font-weight: bold;").arg(couleurEtat.name())
    );
    
    // Capacité
    double capaciteMax = vehicule_->getCapaciteMax() / 1000.0; // grammes -> kg
    double capaciteUtilisee = vehicule_->getCapaciteUtilisee() / 1000.0;
    double capaciteDispo = capaciteMax - capaciteUtilisee;
    int pourcentage = (capaciteMax > 0) ? (int)((capaciteUtilisee / capaciteMax) * 100) : 0;
    
    ui->labelCapaciteMax->setText(QString("%1 kg").arg(capaciteMax, 0, 'f', 2));
    ui->labelCapaciteUtilisee->setText(QString("%1 kg").arg(capaciteUtilisee, 0, 'f', 2));
    ui->labelCapaciteDispo->setText(QString("%1 kg").arg(capaciteDispo, 0, 'f', 2));
    
    // Barre de progression
    ui->progressBarCapacite->setValue(pourcentage);
    
    // Couleur de la barre selon le remplissage
    QString couleurBarre;
    if (pourcentage < 50) {
        couleurBarre = "#2e7d32"; // Vert
    } else if (pourcentage < 90) {
        couleurBarre = "#f57c00"; // Orange
    } else {
        couleurBarre = "#c62828"; // Rouge
    }
    
    ui->progressBarCapacite->setStyleSheet(
        QString("QProgressBar {"
                "   border: 1px solid #3a3a3a;"
                "   border-radius: 3px;"
                "   text-align: center;"
                "   background-color: #1e1e1e;"
                "}"
                "QProgressBar::chunk {"
                "   background-color: %1;"
                "}").arg(couleurBarre)
    );
    
    // Nombre de colis
    ui->labelNombreColis->setText(QString::number(vehicule_->getNombreColis()));
}

void DialogDetailsVehicule::afficherColis()
{
    ui->tableColisVehicule->setRowCount(0);
    
    // Note: On ne peut pas directement accéder aux colis depuis l'extérieur
    // car ils sont privés dans Vehicule. On affiche juste le nombre.
    // Pour une vraie implémentation, il faudrait ajouter une méthode
    // getColis(size_t index) ou getListeColis() dans Vehicule.h
    
    // Pour l'instant, on affiche un message si le véhicule a des colis
    if (vehicule_->getNombreColis() == 0) {
        ui->tableColisVehicule->setRowCount(1);
        QTableWidgetItem* item = new QTableWidgetItem("Aucun colis à bord");
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QColor("#757575"));
        ui->tableColisVehicule->setItem(0, 0, item);
        ui->tableColisVehicule->setSpan(0, 0, 1, 4);
    } else {
        // Afficher un message indiquant qu'il y a des colis
        // (nécessiterait une méthode d'accès pour afficher les détails)
        ui->tableColisVehicule->setRowCount(1);
        QString message = QString("%1 colis en cours de transport").arg(vehicule_->getNombreColis());
        QTableWidgetItem* item = new QTableWidgetItem(message);
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QColor("#0d47a1"));
        ui->tableColisVehicule->setItem(0, 0, item);
        ui->tableColisVehicule->setSpan(0, 0, 1, 4);
    }
}
