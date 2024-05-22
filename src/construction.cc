#include "construction.hh"

myDetectorConstruction::myDetectorConstruction() {
    xWorld = 0.5*m;
    yWorld = 0.5*m;
    zWorld = 0.5*m;

    // xWorld = 40.*km;
    // yWorld = 40.*km;
    // zWorld = 20.*km;

    isAtmosphere = false;
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


    // Defining the atmospheric layers
    G4double density0 = 1.29*kg/m3;
    G4double aN = 14.01*g/mole;
    G4double aO = 16.00*g/mole;

    N = new G4Element("Nitrogen", "N", 7, aN);
    O = new G4Element("Oxygen", "O", 8, aO);

    G4double f = 3;
    G4double R = 8.3144626181532;
    G4double g0 = 9.81;
    G4double kappa = (f+2)/f;
    G4double T = 293.15;
    G4double M = (0.3*aO + 0.7*aN)/1000.;

    for(G4int i = 0; i < 10; i++) {
        std::stringstream stri; 
        stri << i;
        G4double h = 40e3/10.*i;
        G4double density = density0*pow((1-kappa)/kappa*M*g0*h/(R*T), (1/(kappa-1)));
        Air[i] = new G4Material("G4_AIR"+stri.str(), density, 2);
        Air[i]->AddElement(N, 0.7);
        Air[i]->AddElement(O, 0.3);
    }
}

void myDetectorConstruction::ConstructScintillator() {
    solidScintillator = new G4Tubs("solidScintillator", 0.*cm, 7.5*cm, 2.*cm, 0.*deg, 360.*deg);
    logicScintillator = new G4LogicalVolume(solidScintillator, pilotU, "logicalScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 7.*cm), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    // auto                *solidCase1 = new G4Tubs("solidCase1", 7.5*cm, 7.6*cm, 2.*cm, 0*deg, 360*deg);
    // G4LogicalVolume     *logicCase1 = new G4LogicalVolume(solidCase1, Al, "logicCase1");
    // G4VPhysicalVolume   *physCase1  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.4*m), logicCase1, "physCase1", logicWorld, false, 0, true);
    // auto                *solidCase2 = new G4Tubs("solidCase2", 0.*cm, 7.6*cm, 0.01*cm, 0*deg, 360*deg);
    // G4LogicalVolume     *logicCase2 = new G4LogicalVolume(solidCase2, Al, "logicCase2");
    // G4VPhysicalVolume   *physCase2  = new G4PVPlacement(0, G4ThreeVector(0., 0., 37.99*cm), logicCase2, "physCase2", logicWorld, false, 0, true);
    // auto                *solidCase3 = new G4Tubs("solidCase3", 0.*cm, 7.6*cm, 0.01*cm, 0*deg, 360*deg);
    // G4LogicalVolume     *logicCase3 = new G4LogicalVolume(solidCase3, Al, "logicCase3");
    // G4VPhysicalVolume   *physCase3  = new G4PVPlacement(0, G4ThreeVector(0., 0., 42.01*cm), logicCase3, "physCase3", logicWorld, false, 0, true);


    fScoringVolume = logicScintillator;
}

void myDetectorConstruction::ConstructAtmosphere() {
    solidAtmosphere = new G4Box("solidAir", xWorld, yWorld, zWorld/10);
    for(G4int i = 0; i < 10; i++) {
        logicAtmosphere[i] = new G4LogicalVolume(solidAtmosphere, Air[i], "logicAtmosphere");
        physAtmosphere[i] = new G4PVPlacement(0, G4ThreeVector(0., 0., zWorld/10.*2.*i - zWorld + zWorld/10.), logicAtmosphere[i], "physAtmosphere", logicWorld, false, i, true);
    }
}

G4VPhysicalVolume *myDetectorConstruction::Construct() {
    // Defining the world
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    if(isScintillator) {
        ConstructScintillator();
    }
    else if(isAtmosphere) {
        ConstructAtmosphere();
    }

    return physWorld;
}
