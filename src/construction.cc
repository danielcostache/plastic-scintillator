#include "construction.hh"

myDetectorConstruction::myDetectorConstruction() {
    xWorld = 0.5*m;
    yWorld = 0.5*m;
    zWorld = 0.5*m;

    isScintillator = true;

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


    // Defining BC-408 scintillator
    BC408 = new G4Material("BC408", 1.032*g/cm3, 2);

    BC408->AddElement(nist->FindOrBuildElement("C"), 47.62*perCent);
    BC408->AddElement(nist->FindOrBuildElement("H"), 52.38*perCent);

    std::vector<G4double> scintEnergyBC408 = {1.239841939*eV/0.9, 1.239841939*eV/0.425, 1.239841939*eV/0.2};
    std::vector<G4double> rindexBC408 = {1.58, 1.58, 1.58};
    std::vector<G4double> scintFracBC408 = {0.01, 1.0, 0.01};

    G4MaterialPropertiesTable *mptBC408 = new G4MaterialPropertiesTable();
    mptBC408->AddProperty("RINDEX", scintEnergyBC408, rindexBC408);
    mptBC408->AddProperty("SCINTILLATIONCOMPONENT1", scintEnergyBC408, scintFracBC408);
    mptBC408->AddConstProperty("SCINTILLATIONYIELD", 10.2/keV);
    mptBC408->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptBC408->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1*ns);
    mptBC408->AddConstProperty("SCINTILLATIONYIELD1", 1.);

    BC408->SetMaterialPropertiesTable(mptBC408);

    Al = nist->FindOrBuildMaterial("G4_Al");
    Fe = nist->FindOrBuildMaterial("G4_Fe");

}

void myDetectorConstruction::ConstructScintillator() {
    // Defining the Scintillator

    G4double radDet=7.5*cm, widthDet=2.*mm, widthCase=0.1*mm, zDet=7*cm;

    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, radDet, widthDet, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, BC408, "logicalScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., zDet), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    auto                *solidCase1 = new G4Tubs("solidCase1", radDet, radDet+1*mm, widthDet, 0*deg, 360*deg);
    G4LogicalVolume     *logicCase1 = new G4LogicalVolume(solidCase1, worldMat, "logicCase1");
    G4VPhysicalVolume   *physCase1  = new G4PVPlacement(0, G4ThreeVector(0., 0., zDet), logicCase1, "physCase1", logicWorld, false, 0, true);
    auto                *solidCase2 = new G4Tubs("solidCase2", 0.*cm, radDet+1*mm, 1*mm, 0*deg, 360*deg);
    G4LogicalVolume     *logicCase2 = new G4LogicalVolume(solidCase2, Al, "logicCase2");
    G4VPhysicalVolume   *physCase2  = new G4PVPlacement(0, G4ThreeVector(0., 0., zDet-widthDet/2-1/2*mm), logicCase2, "physCase2", logicWorld, false, 0, true);
    auto                *solidCase3 = new G4Tubs("solidCase3", 0.*cm, radDet+1*mm, widthCase, 0*deg, 360*deg);
    G4LogicalVolume     *logicCase3 = new G4LogicalVolume(solidCase3, worldMat, "logicCase3");
    G4VPhysicalVolume   *physCase3  = new G4PVPlacement(0, G4ThreeVector(0., 0., zDet+widthDet/2+widthCase/2), logicCase3, "physCase3", logicWorld, false, 0, true);


    fScoringVolume = logicScintillator;
}

G4VPhysicalVolume *myDetectorConstruction::Construct() {
    // Defining the world
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    if(isScintillator) {
        ConstructScintillator();
    }

    return physWorld;
}
