#include "MyCaloActionInitialization.hh"
#include "MyCaloPrimaryGeneratorAction.hh"
#include "MyCaloRunAction.hh"
#include "MyCaloEventAction.hh"

MyCaloActionInitialization::MyCaloActionInitialization() : G4VUserActionInitialization(){
}

MyCaloActionInitialization::~MyCaloActionInitialization(){
}

void MyCaloActionInitialization::BuildForMaster() const {
  SetUserAction(new MyCaloRunAction);
}

void MyCaloActionInitialization::Build() const {
  SetUserAction(new MyCaloPrimaryGeneratorAction);
  SetUserAction(new MyCaloRunAction);
  SetUserAction(new MyCaloEventAction);
}
