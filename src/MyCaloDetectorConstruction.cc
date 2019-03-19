#include "MyCaloDetectorConstruction.hh"
#include "MyCaloSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
using namespace std;

MyCaloDetectorConstruction::MyCaloDetectorConstruction():G4VUserDetectorConstruction(),fCheckOverlaps(true),fNofLayers(-1){}

MyCaloDetectorConstruction::~MyCaloDetectorConstruction(){}  

G4VPhysicalVolume* MyCaloDetectorConstruction::Construct(){
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

void MyCaloDetectorConstruction::DefineMaterials(){ 
  // Lead material defined using NIST Manager
  auto nist = G4NistManager::Instance();
  nist->FindOrBuildMaterial("G4_AIR");
  nist->FindOrBuildMaterial("G4_Pb");
  nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* MyCaloDetectorConstruction::DefineVolumes(){
  //Ottieni materiali
  //Materiale del mondo
  auto worldMaterial = G4Material::GetMaterial("G4_AIR");
  //Materiale assorbitore
  auto absorberMaterial = G4Material::GetMaterial("G4_Pb");
  //  auto absorberDensity = absorberMaterial->GetDensity();
  //Materiale attivo
  auto gapMaterial = G4Material::GetMaterial("G4_SODIUM_IODIDE");
  //  auto gapDensity = gapMaterial->GetDensity();
  if (!worldMaterial || !absorberMaterial || !gapMaterial){
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("B4DetectorConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);

  }  

  //Parametri geometrici
  //Spessore di uno strato di materiale attivo
  G4double gapThickness =  0.5*cm;
  //Spessore di uno strato di materiale assorbitore
  G4double absoThickness = 5*cm;//ratio*gapThickness;
  //Larghezza che contiene il 99% dell'energia di uno sciame da 150 GeV (Fernow)
  G4double caloSizeXY  = (-17.3+14.3*log(150))*cm;
  //Lughezza di interazione materiale attivo
  G4double gapNucLength = gapMaterial->GetNuclearInterLength();
  cout << "Lunghezza interazione gap: " << gapNucLength/cm << " cm" << endl;
  //Lunghezza di interazione assorbitore
  G4double absoNucLength = absorberMaterial->GetNuclearInterLength();
  cout << "Lunghezza interazione assorbitore: " << absoNucLength/cm << " cm" << endl;
  //Lunghezza di interazione media
  G4double layerNucLength = (gapThickness/gapNucLength + absoThickness/absoNucLength);///(gapThickness + absoThickness);
  G4cout << "Ogni strato vale " << layerNucLength << " lunghezze di interazione" << G4endl;
  //Lunghezza che contiene il 90% dell'energia di uno sciame da 150 GeV (Fernow)
  G4double totalNucLength = (-1.26 + 1.74*log(150));
  G4cout << "Lunghezze di radiazione totali: " << totalNucLength << G4endl;

  //Spessore di uno strato assorbitore + attivo
  G4double layerThickness = absoThickness + gapThickness;
  //Numero di strati totali
  fNofLayers = ceil(totalNucLength/layerNucLength);
  //Spessore totale del calorimetro
  G4double caloThickness = fNofLayers * layerThickness;  
  //Dimensioni trasversali del mondo
  auto worldSizeXY = 1.1 * caloSizeXY;               
  //Spessore del mondo
  auto worldSizeZ  = 1.1 * caloThickness;            
  
  //World
  //Solid
  auto worldS = new G4Box("World", worldSizeXY/2, worldSizeXY/2, worldSizeZ/2);
  //Logical Volume
  auto worldLV = new G4LogicalVolume(worldS, worldMaterial, "World");
  //Physical Volume
  auto worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlaps);
  
  //Calorimeter
  //Solid
  auto calorimeterS = new G4Box("Calorimeter", caloSizeXY/2, caloSizeXY/2, caloThickness/2);
  //Logical Volume
  auto caloLV = new G4LogicalVolume(calorimeterS, worldMaterial, "Calorimeter");
  //Physical Volume
  new G4PVPlacement(0, G4ThreeVector(), caloLV, "Calorimeter", worldLV, false, 0, fCheckOverlaps);
  
  //Layer
  //Solid
  auto layerS = new G4Box("Layer", caloSizeXY/2, caloSizeXY/2, layerThickness/2);
  //Logical Volume
  auto layerLV = new G4LogicalVolume(layerS, worldMaterial, "Layer");
  //Physical Volume
  new G4PVReplica("Layer", layerLV, caloLV, kZAxis, fNofLayers, layerThickness);
  
  //Absorber
  //Solid
  auto absorberS = new G4Box("Absorber", caloSizeXY/2, caloSizeXY/2, absoThickness/2);
  //Logical Volume
  auto absorberLV = new G4LogicalVolume(absorberS, absorberMaterial, "AbsoLV");
  //Physical Volume
  new G4PVPlacement(0, G4ThreeVector(0., 0., absoThickness/2), absorberLV, "Absorber", layerLV, false, 0, fCheckOverlaps);

  //Active Material
  //Solid
  auto gapS = new G4Box("Gap", caloSizeXY/2, caloSizeXY/2, gapThickness/2);
  //Logical Volume
  auto gapLV = new G4LogicalVolume(gapS, gapMaterial, "GapLV");
  //Physical Volume
  new G4PVPlacement(0, G4ThreeVector(0., 0., -gapThickness/2), gapLV, "Gap", layerLV, false, 0, fCheckOverlaps);
  
  //print parameters
  G4cout
    << G4endl 
    << "------------------------------------------------------------" << G4endl
    << "---> The calorimeter is " << fNofLayers << " layers of: [ "
    << absoThickness/mm << "mm of " << absorberMaterial->GetName() 
    << " + "
    << gapThickness/mm << "mm of " << gapMaterial->GetName() << " ] " << G4endl
    << "Total length: " << fNofLayers*(absoThickness/mm + gapThickness/mm) << " mm" << G4endl
    << "Half width: " << caloSizeXY/mm << " mm" << G4endl
    << "------------------------------------------------------------" << G4endl;
  
  // Visualization attributes
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  caloLV->SetVisAttributes(simpleBoxVisAtt);

  // Always return the physical World
  return worldPV;
}

void MyCaloDetectorConstruction::ConstructSDandField(){
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // Sensitive detectors
  auto absoSD = new MyCaloSD("AbsorberSD", "AbsorberHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  SetSensitiveDetector("AbsoLV",absoSD);
  
  auto gapSD = new MyCaloSD("GapSD", "GapHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(gapSD);
  SetSensitiveDetector("GapLV",gapSD);
}
