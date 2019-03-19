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
        // print event statistics
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

        //Get position from absorber and gap
        /*  auto absoPosZ = absoTotalHit->GetPosZ();
            auto gapPosZ = gapTotalHit->GetPosZ();
            auto Z = absoPosZ + gapPosZ; */

        // fill histograms
        analysisManager->FillH1(0, absoTotalHit->GetEdep());
        analysisManager->FillH1(1, gapTotalHit->GetEdep());
        //  analysisManager->FillH1(2, gapTotalHit->GetPos().getZ());
        //  analysisManager->FillH1(2, absoTotalHit->GetPos().getZ());

        // for(auto gapHit : gapHC) {
        for(int i=0; i<gapHC->entries()-1; ++i) {
                auto gapHit = (*gapHC)[i];
                analysisManager->FillH1(2, gapHit->GetPosX());
                analysisManager->FillH1(3, gapHit->GetPosY());
                analysisManager->FillH1(4, gapHit->GetPosZ());
        }
        // for(auto absoHit : absoHC) {
        for(int i=0; i<absoHC->entries()-1; ++i) {
                auto absoHit = (*absoHC)[i];
                analysisManager->FillH1(2, absoHit->GetPosX());
                analysisManager->FillH1(3, absoHit->GetPosY());
                analysisManager->FillH1(4, absoHit->GetPosZ());
        }

        // fill ntuple
        analysisManager->FillNtupleDColumn(0, absoTotalHit->GetEdep());
        analysisManager->FillNtupleDColumn(1, gapTotalHit->GetEdep());
        // for(auto gapHit : gapHC) {
        for(int i=0; i<gapHC->entries()-1; ++i) {
                auto gapHit = (*gapHC)[i];
                analysisManager->FillNtupleDColumn(2, gapHit->GetPosX());
                analysisManager->FillNtupleDColumn(3, gapHit->GetPosY());
                analysisManager->FillNtupleDColumn(4, gapHit->GetPosZ());
                analysisManager->AddNtupleRow();
        }
        // for(auto absoHit : absoHC) {
        for(int i=0; i<absoHC->entries()-1; ++i) {
                auto absoHit = (*absoHC)[i];
                analysisManager->FillNtupleDColumn(2, absoHit->GetPosX());
                analysisManager->FillNtupleDColumn(3, absoHit->GetPosY());
                analysisManager->FillNtupleDColumn(4, absoHit->GetPosZ());
                analysisManager->AddNtupleRow();
        }
}
