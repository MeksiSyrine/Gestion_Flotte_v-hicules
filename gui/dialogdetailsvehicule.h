#ifndef DIALOGDETAILSVEHICULE_H
#define DIALOGDETAILSVEHICULE_H

#include <QDialog>
#include "../Vehicule.h"

namespace Ui {
class DialogDetailsVehicule;
}

/**
 * Dialogue pour afficher les détails d'un véhicule et sa liste de colis
 */
class DialogDetailsVehicule : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDetailsVehicule(const Vehicule* vehicule, QWidget *parent = nullptr);
    ~DialogDetailsVehicule();

private:
    Ui::DialogDetailsVehicule *ui;
    const Vehicule* vehicule_;
    
    void afficherInfosVehicule();
    void afficherColis();
};

#endif // DIALOGDETAILSVEHICULE_H
