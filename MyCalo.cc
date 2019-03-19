#include "MyCaloDetectorConstruction.hh"
#include "MyCaloActionInitialization.hh"
#include "G4RunManager.hh"
#include "G4PhysListFactory.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4UIExecutive.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

int main(int argc, char** argv){

#ifdef G4VIS_USE
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  G4RunManager *runManager = new G4RunManager;

  // Set mandatory initialization classes

  // Physics List
  MyCaloDetectorConstruction *detConstruction = new MyCaloDetectorConstruction();
  runManager->SetUserInitialization(detConstruction);
  G4PhysListFactory *physListFactory = new G4PhysListFactory();
  G4VModularPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BERT");
  runManager->SetUserInitialization(physicsList);

  //Action Inizialization
  MyCaloActionInitialization *actionInitialization = new MyCaloActionInitialization();
  runManager->SetUserInitialization(actionInitialization);
  // Initialize G4 kernel
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  G4UIExecutive* ui = 0;
  if (argc!=1){
    // batch mode
    G4String command = "/control/execute ";
    G4String macro = argv[1];
    UImanager->ApplyCommand(command+macro);
  }
  else{
    ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if(ui->IsGUI()){
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  return 0;
}

