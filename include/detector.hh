#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include "g4root_defs.hh"

class mySensitiveDetector : public G4VSensitiveDetector {
    public:
        mySensitiveDetector(G4String);
        ~mySensitiveDetector();

    private:
        virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
        
        G4PhysicsOrderedFreeVector *quEff;
};

#endif