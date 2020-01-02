#include "MyCaloSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

MyCaloSD::MyCaloSD(const G4String& name, const G4String& hitsCollectionName, G4int nofCells) :
  G4VSensitiveDetector(name),fHitsCollection(nullptr),fNofCells(nofCells){
  collectionName.insert(hitsCollectionName);
}

MyCaloSD::~MyCaloSD(){
}

void MyCaloSD::Initialize(G4HCofThisEvent* hce){
  // Create hits collection
  fHitsCollection = new MyCaloHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Create hits
  // fNofCells for cells + one more for total sums
  for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new MyCaloHit());
  }
}

G4bool MyCaloSD::ProcessHits(G4Step* step, G4TouchableHistory*){
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();

  // Get hit position
  auto pos = step->GetPostStepPoint()->GetPosition();

  if (edep==0.)
    return false;

  auto touchable = step->GetPreStepPoint()->GetTouchable();

  // Get calorimeter cell id
  auto layerNumber = touchable->GetReplicaNumber(1);

  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if (!hit) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber;
    G4Exception("MyCaloSD::ProcessHits()","MyCode0004", FatalException, msg);
  }

  //  hit->SetPos(pos);
	
  // Get hit for total accounting
  auto hitTotal = (*fHitsCollection)[fHitsCollection->entries()-1];

  // Add values
  hit->Add(edep, pos.getX(), pos.getY(), pos.getZ());
  hitTotal->Add(edep, pos.getX(), pos.getY(), pos.getZ());

  return true;
}

void MyCaloSD::EndOfEvent(G4HCofThisEvent*){
  if (verboseLevel>1) {
    auto nofHits = fHitsCollection->entries();
    G4cout
      << G4endl
      << "-------->Hits Collection: in this event there are " << nofHits << " hits in the tracker chambers: " << G4endl;
    for (G4int i=0; i<nofHits; i++)
      (*fHitsCollection)[i]->Print();
  }
}
