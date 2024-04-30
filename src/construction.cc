#include "construction.hh"

myDetectorConstruction::myDetectorConstruction() {
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    
    xWorld = 0.5*m;
    yWorld = 0.5*m;
    zWorld = 0.5*m;

    DefineMaterial();
}

myDetectorConstruction::~myDetectorConstruction() {

}

G4LogicalVolume *myDetectorConstruction::GetScoringVolume() const {
    return fScoringVolume;
}

void myDetectorConstruction::DefineMaterial() {
    G4NistManager *nist = G4NistManager::Instance();
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};

    // Defining the world material
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    G4double rindexAir[2] = {1.0, 1.0};
    mptWorld->AddProperty("RINDEX", energy, rindexAir, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    // Defining EJ-228 / Pilot U / BC-428 scintillator
    pilotU = new G4Material("PilotU", 1.023*g/cm3, 2);

    pilotU->AddElement(nist->FindOrBuildElement("C"), 47.46*perCent);
    pilotU->AddElement(nist->FindOrBuildElement("H"), 52.54*perCent);

    std::vector<G4double> scintEnergyPilotU = {1.239841939*eV/0.396, 1.239841939*eV/0.391, 1.239841939*eV/0.386};
    std::vector<G4double> rindexpilotU = {1.59, 1.58, 1.57};
    std::vector<G4double> scintFracPilotU = {0.1, 1.0, 0.1};

    G4MaterialPropertiesTable *mptpilotU = new G4MaterialPropertiesTable();
    mptpilotU->AddProperty("RINDEX", scintEnergyPilotU, rindexpilotU);
    mptpilotU->AddProperty("SCINTILLATIONCOMPONENT1", scintEnergyPilotU, scintFracPilotU);
    mptpilotU->AddConstProperty("SCINTILLATIONYIELD", 10.2/keV);
    mptpilotU->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptpilotU->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1.4*ns);
    mptpilotU->AddConstProperty("SCINTILLATIONYIELD1", 1.);

    pilotU->SetMaterialPropertiesTable(mptpilotU);

    Al = nist->FindOrBuildMaterial("G4_Al");

}

void myDetectorConstruction::ConstructScintillator() {
    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, 7.5*cm, 2.*cm, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, pilotU, "logicalScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.4*m), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    auto                *solidCase1 = new G4Tubs("solidCase1", 7.5*cm, 7.6*cm, 2.*cm, 0*deg, 360*deg);
    G4LogicalVolume     *logicCase1 = new G4LogicalVolume(solidCase1, Al, "logicCase1");
    G4VPhysicalVolume   *physCase1  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.4*m), logicCase1, "physCase1", logicWorld, false, 0, true);
    auto                *solidCase2 = new G4Tubs("solidCase2", 0.*cm, 7.6*cm, 0.01*cm, 0*deg, 360*deg);
    G4LogicalVolume     *logicCase2 = new G4LogicalVolume(solidCase2, Al, "logicCase2");
    G4VPhysicalVolume   *physCase2  = new G4PVPlacement(0, G4ThreeVector(0., 0., 37.99*cm), logicCase2, "physCase2", logicWorld, false, 0, true);
    auto                *solidCase3 = new G4Tubs("solidCase3", 0.*cm, 7.6*cm, 0.01*cm, 0*deg, 360*deg);
    G4LogicalVolume     *logicCase3 = new G4LogicalVolume(solidCase3, Al, "logicCase3");
    G4VPhysicalVolume   *physCase3  = new G4PVPlacement(0, G4ThreeVector(0., 0., 42.01*cm), logicCase3, "physCase3", logicWorld, false, 0, true);


    fScoringVolume = logicScintillator;
}

G4VPhysicalVolume *myDetectorConstruction::Construct() {
    // Defining the world
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    ConstructScintillator();

    return physWorld;
}
