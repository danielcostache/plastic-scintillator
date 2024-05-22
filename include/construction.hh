#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4GenericMessenger.hh"

#include "detector.hh"

class myDetectorConstruction : public G4VUserDetectorConstruction {
    public: 
        myDetectorConstruction();
        ~myDetectorConstruction();

        G4LogicalVolume *GetScoringVolume() const;

        virtual G4VPhysicalVolume *Construct();   

    private:
        G4LogicalVolume *fScoringVolume;

        G4GenericMessenger *fMessenger;
        
        G4double xWorld, yWorld, zWorld;
        
        G4Tubs *solidScintillator;
        G4Box *solidWorld, *solidAtmosphere;
        G4LogicalVolume *logicWorld, *logicScintillator, *logicAtmosphere[10];
        G4VPhysicalVolume *physWorld, *physScintillator, *physAtmosphere[10];

        G4Material *worldMat, *pilotU, *Al, *Air[10];
        G4Element *C, *H, *N, *O;

        void DefineMaterial();
        void ConstructScintillator();
        void ConstructAtmosphere();

        G4bool isScintillator, isAtmosphere;
};

#endif