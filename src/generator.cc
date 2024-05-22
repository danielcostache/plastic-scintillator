#include "generator.hh"

myPrimaryGenerator::myPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);
    

    G4ParticleTable *pTable = G4ParticleTable::GetParticleTable();
    G4String pName = "geantino";
    G4ParticleDefinition *particle = pTable->FindParticle(pName);

    double r, theta;
    srand(time(0));
    r = 1.5 * ((double) rand() / RAND_MAX);
    theta = 2*3.14* ((double) rand() / RAND_MAX);
    

    G4ThreeVector pos(r*sin(theta)*cm, r*cos(theta)*cm, 0.);
    G4ThreeVector mom(0., 0., 1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleMomentum(0.*GeV);
    fParticleGun->SetParticleDefinition(particle);

}

myPrimaryGenerator::~myPrimaryGenerator() {
    delete fParticleGun;
}   

void myPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();
    if(particle == G4Geantino::Geantino()) {
        G4int Z = 38;
        G4int A = 90;
        G4double charge = 0.*eplus;
        G4double energy = 0.*keV;

        G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);
    }
    fParticleGun->GeneratePrimaryVertex(anEvent);
}