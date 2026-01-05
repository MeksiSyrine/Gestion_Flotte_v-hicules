#ifndef DIALOGCREERCOLIS_H
#define DIALOGCREERCOLIS_H

#include <QDialog>
#include <memory>
#include "../Colis.h"

namespace Ui {
class DialogCreerColis;
}

class DialogCreerColis : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreerColis(QWidget *parent = nullptr);
    ~DialogCreerColis();

    // Récupérer le colis créé
    std::unique_ptr<Colis> getColis();

private slots:
    void onTypeColisChanged(int index);
    void onValider();
    void onAnnuler();

private:
    Ui::DialogCreerColis *ui;
    std::unique_ptr<Colis> colis_;
    
    bool validerChamps();
};

#endif // DIALOGCREERCOLIS_H
