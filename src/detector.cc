#include "detector.hh"

mySensitiveDetector::mySensitiveDetector(G4String name) : G4VSensitiveDetector(name) {
    quEff = new G4PhysicsOrderedFreeVector();

    std::ifstream datafile;
    datafile.open("eff.dat");

    while(!datafile.eof()) {
        G4double wlen, qeff;
        
        datafile >> wlen >> qeff;
        G4cout << wlen << " " << qeff << G4endl;

        quEff->InsertValues(wlen, qeff/100.);
    }

    datafile.close();

    // Function does not exist for some reason
    // quEff->SetSpline(false);
}

mySensitiveDetector::~mySensitiveDetector() {

}

G4bool mySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {

    G4Track *track = aStep->GetTrack();

    track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();
    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
    //G4cout << "Photon position: " << posPhoton << G4endl;

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();
    //G4cout << "Copy Number: " << copyNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume();
    G4ThreeVector posDet = physVol->GetTranslation();
    //G4cout << "Detector position: " << posDet << G4endl;

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    // man->FillNtupleIColumn(0, 0, evt);
    // man->FillNtupleDColumn(0, 1, posPhoton[0]);
    // man->FillNtupleDColumn(0, 2, posPhoton[1]);
    // man->FillNtupleDColumn(0, 3, posPhoton[2]);
    // man->FillNtupleDColumn(0, 4, wlen);
    // man->AddNtupleRow(0);
    
    // if(G4UniformRand() < quEff->Value(wlen)) {
    //     man->FillNtupleIColumn(1, 0, evt);
    //     man->FillNtupleDColumn(1, 1, posDet[0]);
    //     man->FillNtupleDColumn(1, 2, posDet[1]);
    //     man->FillNtupleDColumn(1, 3, posDet[2]);
    //     man->AddNtupleRow(1);
    // }

}