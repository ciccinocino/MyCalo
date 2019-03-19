#ifndef MyCaloRunAction_h
#define MyCaloRunAction_h 1
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class MyCaloRunAction : public G4UserRunAction
{
public:
MyCaloRunAction();
virtual ~MyCaloRunAction();
virtual void BeginOfRunAction(const G4Run*);
virtual void   EndOfRunAction(const G4Run*);
};

#endif
