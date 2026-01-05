#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "../Flotte.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * Fenêtre principale de l'application Qt
 * Gère l'interface graphique pour la flotte de livraison
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots pour les actions du menu
    void onAPropos();
    
    // Slots pour les boutons de la toolbar
    void onCreerVehicule();
    void onCreerColis();
    void onAssignerColis();
    void onLivrerVehicule();
    void onSupprimerVehicule();
    void onAfficherDetails();

private:
    Ui::MainWindow *ui;
    Flotte flotte_;  // Instance de la flotte (logique métier)

    // Méthodes de rafraîchissement de l'interface
    void rafraichirVehicules();
    void rafraichirColis();
    void rafraichirStatistiques();
    
    // Méthode d'initialisation des connexions
    void setupConnections();
};

#endif // MAINWINDOW_H
