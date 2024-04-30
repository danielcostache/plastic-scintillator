#include <iostream>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4MTRunManager.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv) {
    
    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
    #else
        G4RunManager *runManager = new G4RunManager();
    #endif

    runManager->SetUserInitialization(new myDetectorConstruction());
    runManager->SetUserInitialization(new myPhysicsList());
    runManager->SetUserInitialization(new myActionInitialization());

    G4UIExecutive *uiExec = 0;
    
    if(argc == 1) {
        uiExec = new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialise();

    G4UImanager *uiManager = G4UImanager::GetUIpointer();
    if(uiExec) {
        uiManager->ApplyCommand("/control/execute vis.mac");
        uiExec->SessionStart();
    }
    else {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command+fileName);
    }

    return 0;
}