#ifndef MyCaloEventAction_h
#define MyCaloEventAction_h 1

#include "G4UserEventAction.hh"
#include "MyCaloHit.hh"
#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Absober and Gap layers 
/// stored in the hits collections.

class MyCaloEventAction : public G4UserEventAction
{
public:
  MyCaloEventAction();
  virtual ~MyCaloEventAction();
  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);
  
private:
  // methods
  MyCaloHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
  void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength, G4double gapEdep, G4double gapTrackLength) const;
  // data members                   
  G4int  fAbsHCID;
  G4int  fGapHCID;
};

#endif

    
