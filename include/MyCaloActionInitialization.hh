#ifndef MyCaloActionInitialization_h
#define MyCaloActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class MyCaloActionInitialization : public G4VUserActionInitialization
{
public:
  MyCaloActionInitialization();
  virtual ~MyCaloActionInitialization();  
  virtual void BuildForMaster() const;
  virtual void Build() const;
};

#endif

    
