#include "MyCaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <iomanip>

G4ThreadLocal G4Allocator<MyCaloHit>* MyCaloHitAllocator = 0;

MyCaloHit::MyCaloHit(): G4VHit(), fEdep(0.), fPosX(0.), fPosY(0.), fPosZ(0.){}

MyCaloHit::~MyCaloHit(){}

MyCaloHit::MyCaloHit(const MyCaloHit& right): G4VHit(){
  fEdep = right.fEdep;
  fPosX = right.fPosX;
  fPosY = right.fPosY;
  fPosZ = right.fPosZ;
}

const MyCaloHit& MyCaloHit::operator=(const MyCaloHit& right){
  fEdep = right.fEdep;
  fPosX = right.fPosX;
  fPosY = right.fPosY;
  fPosZ = right.fPosZ;
  return *this;
}

G4int MyCaloHit::operator==(const MyCaloHit& right) const{
  return (this == &right) ? 1 : 0;
}

void MyCaloHit::Print(){
  G4cout
    << "Edep: " << std::setw(7) << G4BestUnit(fEdep,"Energy")
    << " hit position: " << std::setw(7) << G4BestUnit(fPosZ, "Length")
    << G4endl;
}

void MyCaloHit::SetPos(G4ThreeVector vec){
  fPosX = vec.getX();
  fPosY = vec.getY();
  fPosZ = vec.getZ();
}
