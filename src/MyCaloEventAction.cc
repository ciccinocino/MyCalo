#include "MyCaloEventAction.hh"
#include "MyCaloSD.hh"
#include "MyCaloHit.hh"
#include "MyCaloAnalysis.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>
using namespace CLHEP;

MyCaloEventAction::MyCaloEventAction() : G4UserEventAction(), fAbsHCID(-1), fGapHCID(-1){
}

MyCaloEventAction::~MyCaloEventAction(){
}

MyCaloHitsCollection* MyCaloEventAction::GetHitsCollection(G4int hcID, const G4Event* event) const {
  auto hitsCollection = static_cast<MyCaloHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("MyCaloEventAction::GetHitsCollection()","MyCode0003", FatalException, msg);
  }
  return hitsCollection;
}

void MyCaloEventAction::PrintEventStatistics(G4double absoEdep, G4double absoZPos, G4double gapEdep, G4double gapZPos) const {
  // pPrint event statistics
  G4cout
    << "Absorber: total energy: " << std::setw(7) << G4BestUnit(absoEdep, "Energy") << G4endl
    << "                 depth: "              << std::setw(7) << G4BestUnit(absoZPos, "Length")
    << G4endl
    << "Gap: total energy: " << std::setw(7) << G4BestUnit(gapEdep, "Energy") << G4endl
    << "            depth: "         << std::setw(7) << G4BestUnit(gapZPos, "Length")
    << G4endl;
}

void MyCaloEventAction::BeginOfEventAction(const G4Event* /*event*/){
}

void MyCaloEventAction::EndOfEventAction(const G4Event* event){
  // Get hits collections IDs (only once)
  if (fAbsHCID == -1) {
    fAbsHCID = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");
    fGapHCID = G4SDManager::GetSDMpointer()->GetCollectionID("GapHitsCollection");
  }

  // Get hits collections
  auto absoHC = GetHitsCollection(fAbsHCID, event);
  auto gapHC = GetHitsCollection(fGapHCID, event);

  // Get hit with total values
  auto absoTotalHit = (*absoHC)[absoHC->entries()-1];
  auto gapTotalHit = (*gapHC)[gapHC->entries()-1];

  // Print per event (modulo n)
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ((printModulo>0) && (eventID%printModulo==0)) {
    G4cout << "---> End of event: " << eventID << G4endl;
    PrintEventStatistics(absoTotalHit->GetEdep(),absoTotalHit->GetPosZ(),gapTotalHit->GetEdep(),gapTotalHit->GetPosZ());
  }

  G4cout << "Total number of hits: absorber " << absoHC->entries() << G4endl
	 << "                           gap " <<  gapHC->entries() << G4endl;
  // Fill histograms, ntuple
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  G4double absoEdep = absoTotalHit->GetEdep();
  G4double gapEdep = gapTotalHit->GetEdep();
  G4double gapX = gapTotalHit->GetPosX();
  G4double gapY = gapTotalHit->GetPosY();
  G4double gapZ = gapTotalHit->GetPosZ();
  analysisManager->FillH1(0, absoEdep);
  analysisManager->FillH1(1, gapEdep);
  analysisManager->FillH1(2, gapX/gapEdep);
  analysisManager->FillH1(3, gapY/gapEdep);
  analysisManager->FillH1(4, gapZ/gapEdep);
  
  // fill ntuple
  // dividing by multiples of 10 to get the correct units (GeV and cm)
  analysisManager->FillNtupleDColumn(0, absoEdep/1000);
  analysisManager->FillNtupleDColumn(1, gapEdep/1000);
  analysisManager->FillNtupleDColumn(2, gapX/gapEdep/10);
  analysisManager->FillNtupleDColumn(3, gapY/gapEdep/10);
  analysisManager->FillNtupleDColumn(4, gapZ/gapEdep/10);
  analysisManager->AddNtupleRow();
}
