#include "MyCaloPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


MyCaloPrimaryGeneratorAction::MyCaloPrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr){
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("kaon+");
  fParticleGun->SetParticleDefinition(particleDefinition);
  //fParticleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,-120.0*mm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(100.*GeV);
}


MyCaloPrimaryGeneratorAction::~MyCaloPrimaryGeneratorAction(){
  delete fParticleGun;
}


void MyCaloPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume 
  // from G4LogicalVolumeStore
  G4double worldZHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (worldLV){
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  }
  if (worldBox){
    worldZHalfLength = worldBox->GetZHalfLength();  
  }
  else{
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("MyCaloPrimaryGeneratorAction::GeneratePrimaries()","MyCode0002", JustWarning, msg);
  } 
  // Set gun position
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
