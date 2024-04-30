#include "event.hh"

myEventAction::myEventAction(myRunAction*) {
    fEdep = 0.;
}

myEventAction::~myEventAction() {

}

void myEventAction::BeginOfEventAction(const G4Event*) {
    fEdep = 0.;
}

void myEventAction::EndOfEventAction(const G4Event*) {
    // G4cout << "Energy deposition: " << fEdep << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, 0, fEdep);
    man->AddNtupleRow(0);
}

void myEventAction::AddEdep(G4double edep) {
    fEdep += edep;
}