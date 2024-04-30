#include "generator.hh"

myPrimaryGenerator::myPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);
    
    G4ParticleTable *pTable = G4ParticleTable::GetParticleTable();
    G4String pName = "e-";
    G4ParticleDefinition *particle = pTable->FindParticle(pName);

    G4ThreeVector pos(0., 0., 0.);
    G4ThreeVector mom(0., 0., 1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(10.*MeV);
    fParticleGun->SetParticleDefinition(particle);

}

myPrimaryGenerator::~myPrimaryGenerator() {
    delete fParticleGun;
}   

void myPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();
    if(particle == G4Geantino::Geantino()) {
        G4int Z = 27;
        G4int A = 60;
        G4double charge = 0.*eplus;
        G4double energy = 0.*keV;

        G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);
    }
    fParticleGun->GeneratePrimaryVertex(anEvent);
}