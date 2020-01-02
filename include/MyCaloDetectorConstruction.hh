#ifndef MyCaloDetectorConstruction_h
#define MyCaloDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class MyCaloDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyCaloDetectorConstruction();
  virtual ~MyCaloDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

private:
  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();
  G4bool fCheckOverlaps;
  G4int fNofLayers;
};

#endif
