#include "dialogcreercolis.h"
#include "ui_dialogcreercolis.h"
#include "../ColisStandard.h"
#include "../ColisFragile.h"
#include "../ColisRefrigere.h"
#include "../ColisExpress.h"
#include <QMessageBox>

DialogCreerColis::DialogCreerColis(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCreerColis)
{
    ui->setupUi(this);
    
    setWindowTitle("Créer un Colis");
    
    // Connexions
    connect(ui->comboType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DialogCreerColis::onTypeColisChanged);
    connect(ui->btnValider, &QPushButton::clicked, this, &DialogCreerColis::onValider);
    connect(ui->btnAnnuler, &QPushButton::clicked, this, &DialogCreerColis::onAnnuler);
    
    // Initialiser l'affichage des champs spécifiques
    onTypeColisChanged(0);
}

DialogCreerColis::~DialogCreerColis()
{
    delete ui;
}

void DialogCreerColis::onTypeColisChanged(int index)
{
    // Masquer tous les champs spécifiques
    ui->labelFragilite->setVisible(false);
    ui->comboFragilite->setVisible(false);
    ui->labelTemperature->setVisible(false);
    ui->spinTemperature->setVisible(false);
    
    // Afficher selon le type sélectionné
    switch (index) {
        case 0:  // Standard
            // Rien de spécifique
            break;
        case 1:  // Fragile
            ui->labelFragilite->setVisible(true);
            ui->comboFragilite->setVisible(true);
            break;
        case 2:  // Réfrigéré
            ui->labelTemperature->setVisible(true);
            ui->spinTemperature->setVisible(true);
            break;
        case 3:  // Express
            // Rien de spécifique
            break;
    }
}

bool DialogCreerColis::validerChamps()
{
    // Vérifier la description
    if (ui->editDescription->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "La description ne peut pas être vide.");
        return false;
    }
    
    // Vérifier la masse
    if (ui->spinMasse->value() <= 0) {
        QMessageBox::warning(this, "Erreur", "La masse doit être supérieure à 0.");
        return false;
    }
    
    return true;
}

void DialogCreerColis::onValider()
{
    if (!validerChamps()) {
        return;
    }
    
    // Récupérer les valeurs communes
    QString description = ui->editDescription->text().trimmed();
    int masse = ui->spinMasse->value() * 1000;  // Conversion kg -> g
    
    // Créer le colis selon le type
    int typeIndex = ui->comboType->currentIndex();
    
    try {
        switch (typeIndex) {
            case 0: {  // Standard
                colis_ = std::make_unique<ColisStandard>(
                    description.toStdString(), 
                    masse
                );
                break;
            }
            case 1: {  // Fragile
                Fragilite fragilite;
                switch (ui->comboFragilite->currentIndex()) {
                    case 0:
                        fragilite = Fragilite::FAIBLE;
                        break;
                    case 1:
                        fragilite = Fragilite::MOYEN;
                        break;
                    case 2:
                        fragilite = Fragilite::ELEVE;
                        break;
                    default:
                        fragilite = Fragilite::MOYEN;
                }
                
                colis_ = std::make_unique<ColisFragile>(
                    description.toStdString(), 
                    masse,
                    fragilite
                );
                break;
            }
            case 2: {  // Réfrigéré
                float temperature = ui->spinTemperature->value();
                colis_ = std::make_unique<ColisRefrigere>(
                    description.toStdString(), 
                    masse,
                    temperature
                );
                break;
            }
            case 3: {  // Express
                colis_ = std::make_unique<ColisExpress>(
                    description.toStdString(), 
                    masse
                );
                break;
            }
            default:
                QMessageBox::critical(this, "Erreur", "Type de colis invalide.");
                return;
        }
        
        accept();  // Fermer le dialogue avec succès
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur", 
            QString("Erreur lors de la création du colis: %1").arg(e.what()));
    }
}

void DialogCreerColis::onAnnuler()
{
    reject();  // Fermer le dialogue sans créer de colis
}

std::unique_ptr<Colis> DialogCreerColis::getColis()
{
    return std::move(colis_);
}
