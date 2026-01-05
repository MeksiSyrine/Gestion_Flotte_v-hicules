# Makefile pour le projet de gestion de flotte
# Compilation C++17 avec warnings

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = flotte

# Fichiers sources
SRCS = main.cpp \
       Flotte.cpp \
       Vehicule.cpp \
       Drone.cpp \
       CamionnetteElectrique.cpp \
       RobotTrottoir.cpp \
       Colis.cpp \
       ColisStandard.cpp \
       ColisFragile.cpp \
       ColisRefrigere.cpp \
       ColisExpress.cpp

# Fichiers objets (générés automatiquement)
OBJS = $(SRCS:.cpp=.o)

# Règle par défaut : compilation du programme
all: $(TARGET)

# Compilation du programme principal
$(TARGET): $(OBJS)
	@echo "Liaison de $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@echo "✓ Compilation terminée : $(TARGET).exe"

# Règle générique pour compiler les fichiers .cpp en .o
%.o: %.cpp
	@echo "Compilation de $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Tests individuels (optionnel)
test_colis: test_colis.cpp Colis.o ColisStandard.o ColisFragile.o ColisRefrigere.o ColisExpress.o
	$(CXX) $(CXXFLAGS) test_colis.cpp Colis.o ColisStandard.o ColisFragile.o ColisRefrigere.o ColisExpress.o -o test_colis
	@echo "✓ Test colis compilé : test_colis.exe"

test_vehicules: test_vehicules.cpp Vehicule.o Drone.o CamionnetteElectrique.o RobotTrottoir.o Colis.o ColisStandard.o ColisFragile.o ColisRefrigere.o ColisExpress.o
	$(CXX) $(CXXFLAGS) test_vehicules.cpp Vehicule.o Drone.o CamionnetteElectrique.o RobotTrottoir.o Colis.o ColisStandard.o ColisFragile.o ColisRefrigere.o ColisExpress.o -o test_vehicules
	@echo "✓ Test véhicules compilé : test_vehicules.exe"

test_flotte: test_flotte.cpp Flotte.o Vehicule.o Drone.o CamionnetteElectrique.o RobotTrottoir.o Colis.o ColisStandard.o ColisFragile.o ColisRefrigere.o ColisExpress.o
	$(CXX) $(CXXFLAGS) test_flotte.cpp Flotte.o Vehicule.o Drone.o CamionnetteElectrique.o RobotTrottoir.o Colis.o ColisStandard.o ColisFragile.o ColisRefrigere.o ColisExpress.o -o test_flotte
	@echo "✓ Test flotte compilé : test_flotte.exe"

# Nettoyage des fichiers générés
clean:
	@echo "Nettoyage des fichiers objets et exécutables..."
	rm -f $(OBJS) $(TARGET).exe test_colis.exe test_vehicules.exe test_flotte.exe
	@echo "✓ Nettoyage terminé"

# Recompilation complète
rebuild: clean all

# Afficher l'aide
help:
	@echo "Makefile - Gestion de flotte de livraison autonome"
	@echo ""
	@echo "Cibles disponibles :"
	@echo "  make            - Compile le programme principal (flotte.exe)"
	@echo "  make all        - Identique à make"
	@echo "  make test_colis - Compile le test de la hiérarchie des colis"
	@echo "  make test_vehicules - Compile le test de la hiérarchie des véhicules"
	@echo "  make test_flotte - Compile le test de la classe Flotte"
	@echo "  make clean      - Supprime tous les fichiers générés"
	@echo "  make rebuild    - Nettoie puis recompile tout"
	@echo "  make help       - Affiche cette aide"

.PHONY: all clean rebuild help test_colis test_vehicules test_flotte
