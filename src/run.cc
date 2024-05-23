#include "run.hh"

myRunAction::myRunAction() {
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("fEdep");
    man->FinishNtuple(0);
}

myRunAction::~myRunAction() {

}

void myRunAction::BeginOfRunAction(const G4Run* run) {
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    G4int runNumber = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runNumber;
    man->OpenFile("output"+strRunID.str()+".root");

}   

void myRunAction::EndOfRunAction(const G4Run*) {
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}