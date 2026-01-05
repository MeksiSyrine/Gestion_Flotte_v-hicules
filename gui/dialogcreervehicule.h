#ifndef DIALOGCREERVEHICULE_H
#define DIALOGCREERVEHICULE_H

#include <QDialog>
#include <memory>
#include "../Vehicule.h"

namespace Ui {
class DialogCreerVehicule;
}

class DialogCreerVehicule : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreerVehicule(QWidget *parent = nullptr);
    ~DialogCreerVehicule();

    // Récupérer le véhicule créé
    std::unique_ptr<Vehicule> getVehicule();

private slots:
    void onTypeVehiculeChanged(int index);
    void onValider();
    void onAnnuler();

private:
    Ui::DialogCreerVehicule *ui;
    std::unique_ptr<Vehicule> vehicule_;
    
    bool validerChamps();
};

#endif // DIALOGCREERVEHICULE_H
