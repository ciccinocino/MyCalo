#include "MyCaloRunAction.hh"
#include "MyCaloAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

MyCaloRunAction::MyCaloRunAction(): G4UserRunAction(){ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in MyCaloAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  
  // Creating histograms
  G4double energy = 150.0*GeV;  // Energia del fascio (in GeV)
  G4double res = 0.01*GeV;     // Sensibilità in energia (in GeV)
  G4int bin  = energy/res; // Numero di bin
  G4double lenXY = 55.*cm;
  G4double lenZ = 150.*cm;
  G4double lenRes = 1.*cm;
  G4int lenBinXY = 2*lenXY/lenRes;
  G4int lenBinZ = 2*lenZ/lenRes;
  analysisManager->CreateH1("Eabs","Edep in absorber", bin, 0.*GeV, energy,"GeV");
  analysisManager->SetH1XAxisTitle(0,"E [GeV]");
  analysisManager->CreateH1("Egap","Edep in gap", bin, 0.*GeV, energy,"GeV");
  analysisManager->SetH1XAxisTitle(1,"E [GeV]");
  analysisManager->CreateH1("X","X in detector", lenBinXY,-lenXY,lenXY,"cm");
  analysisManager->SetH1XAxisTitle(2,"X [cm]");
  analysisManager->CreateH1("Y","Y in detector", lenBinXY,-lenXY,lenXY,"cm");
  analysisManager->SetH1XAxisTitle(3,"Y [cm]");
  analysisManager->CreateH1("Z","Z in detector", lenBinZ,-lenZ,lenZ,"cm");
  analysisManager->SetH1XAxisTitle(4,"Z [cm]");

  // Creating ntuple
  analysisManager->CreateNtuple("MyCalo", "Edep and Position");
  analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->CreateNtupleDColumn("Egap");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->FinishNtuple();
}

MyCaloRunAction::~MyCaloRunAction(){
  delete G4AnalysisManager::Instance();  
}

void MyCaloRunAction::BeginOfRunAction(const G4Run* /*run*/){ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  G4String fileName = "MyCalo";
  analysisManager->OpenFile(fileName);
}

void MyCaloRunAction::EndOfRunAction(const G4Run* /*run*/){
  // print histogram statistics
  auto analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->GetH1(1)){
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster){
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else{
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    G4cout 
      << " EAbs : mean = " << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
      << " rms = "         << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") 
      << G4endl;
    G4cout 
      << " EGap : mean = " << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
      << " rms = "         << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") 
      << G4endl;
  }
  // save histograms & ntuple
  analysisManager->Write();
  analysisManager->CloseFile();
}
