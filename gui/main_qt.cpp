#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

/**
 * Point d'entrée de l'application Qt
 * Version graphique du système de gestion de flotte
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Configuration de l'application
    app.setApplicationName("Flotte de Livraison");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Projet Livraison");
    
    // Style moderne (Fusion sur Windows)
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Créer et afficher la fenêtre principale
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
