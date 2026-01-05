#include "dialogcreervehicule.h"
#include "ui_dialogcreervehicule.h"
#include "../Drone.h"
#include "../CamionnetteElectrique.h"
#include "../RobotTrottoir.h"
#include <QMessageBox>

DialogCreerVehicule::DialogCreerVehicule(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCreerVehicule)
{
    ui->setupUi(this);
    
    setWindowTitle("Créer un Véhicule");
    
    // Connexions
    connect(ui->comboType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogCreerVehicule::onTypeVehiculeChanged);
    connect(ui->btnValider, &QPushButton::clicked, this, &DialogCreerVehicule::onValider);
    connect(ui->btnAnnuler, &QPushButton::clicked, this, &DialogCreerVehicule::onAnnuler);
    
    // Initialiser l'affichage des champs spécifiques
    onTypeVehiculeChanged(0);
}

DialogCreerVehicule::~DialogCreerVehicule()
{
    delete ui;
}

void DialogCreerVehicule::onTypeVehiculeChanged(int index)
{
    // Masquer tous les champs spécifiques
    ui->labelAltitudeMax->setVisible(false);
    ui->spinAltitudeMax->setVisible(false);
    ui->labelCapaciteInfo->setVisible(true);
    
    // Afficher les informations selon le type sélectionné
    switch (index) {
        case 0:  // Drone
            ui->labelAltitudeMax->setText("Autonomie (km):");
            ui->labelAltitudeMax->setVisible(true);
            ui->spinAltitudeMax->setVisible(true);
            ui->spinAltitudeMax->setValue(100);
            ui->labelCapaciteInfo->setText("Capacité: 5 kg (fixe)");
            break;
        case 1:  // Camionnette
            ui->labelCapaciteInfo->setText("Capacité: 500 kg (fixe)");
            break;
        case 2:  // Robot
            ui->labelCapaciteInfo->setText("Capacité: 20 kg (fixe)");
            break;
    }
}

bool DialogCreerVehicule::validerChamps()
{
    // Vérifier le nom
    if (ui->editNom->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide.");
        return false;
    }
    
    return true;
}

void DialogCreerVehicule::onValider()
{
    if (!validerChamps()) {
        return;
    }
    
    // Récupérer les valeurs communes
    QString nom = ui->editNom->text().trimmed();
    
    // Créer le véhicule selon le type
    int typeIndex = ui->comboType->currentIndex();
    
    try {
        switch (typeIndex) {
            case 0: {  // Drone
                int autonomieKm = ui->spinAltitudeMax->value();  // On utilise ce champ pour l'autonomie
                vehicule_ = std::make_unique<Drone>(
                    nom.toStdString(), 
                    autonomieKm
                );
                break;
            }
            case 1: {  // Camionnette
                vehicule_ = std::make_unique<CamionnetteElectrique>(
                    nom.toStdString()
                );
                break;
            }
            case 2: {  // Robot
                vehicule_ = std::make_unique<RobotTrottoir>(
                    nom.toStdString()
                );
                break;
            }
            default:
                QMessageBox::critical(this, "Erreur", "Type de véhicule invalide.");
                return;
        }
        
        accept();  // Fermer le dialogue avec succès
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", 
            QString("Erreur lors de la création du véhicule: %1").arg(e.what()));
    }
}

void DialogCreerVehicule::onAnnuler()
{
    reject();  // Fermer le dialogue sans créer de véhicule
}

std::unique_ptr<Vehicule> DialogCreerVehicule::getVehicule()
{
    return std::move(vehicule_);
}
