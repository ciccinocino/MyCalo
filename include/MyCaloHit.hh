#ifndef MyCaloHit_h
#define MyCaloHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class MyCaloHit : public G4VHit {
public:
MyCaloHit();
MyCaloHit(const MyCaloHit&);
virtual ~MyCaloHit();
// operators
const MyCaloHit& operator=(const MyCaloHit&);
G4int operator==(const MyCaloHit&) const;
inline void* operator new(size_t);
inline void operator delete(void*);
// methods from base class
virtual void Draw() {
}
virtual void Print();
// methods to handle data
void Add(G4double de, G4double dposX, G4double dposY, G4double dposZ);
void Add(G4double de);
// get methods
G4double GetEdep() const;
G4double GetPosX() const;
G4double GetPosY() const;
G4double GetPosZ() const;
void SetPos(G4ThreeVector);
private:
G4double fEdep;          // Energy deposit in the sensitive volume
G4double fPosX, fPosY, fPosZ;       // Hit position in the sensitive volume
};

using MyCaloHitsCollection = G4THitsCollection<MyCaloHit>;

extern G4ThreadLocal G4Allocator<MyCaloHit>* MyCaloHitAllocator;

inline void* MyCaloHit::operator new(size_t){
        if (!MyCaloHitAllocator) {
                MyCaloHitAllocator = new G4Allocator<MyCaloHit>;
        }
        void *hit;
        hit = (void *) MyCaloHitAllocator->MallocSingle();
        return hit;
}

inline void MyCaloHit::operator delete(void *hit){
        if (!MyCaloHitAllocator) {
                MyCaloHitAllocator = new G4Allocator<MyCaloHit>;
        }
        MyCaloHitAllocator->FreeSingle((MyCaloHit*) hit);
}

inline void MyCaloHit::Add(G4double de, G4double dposX, G4double dposY, G4double dposZ){
        fEdep += de;
        fPosX += dposX;//*de;
        fPosY += dposY;//*de;
        fPosZ += dposZ;//*de;
}

inline void MyCaloHit::Add(G4double de){
        fEdep += de;
}

inline G4double MyCaloHit::GetEdep() const {
        return fEdep;
}

inline G4double MyCaloHit::GetPosX() const {
        return fPosX;
}

inline G4double MyCaloHit::GetPosY() const {
        return fPosY;
}

inline G4double MyCaloHit::GetPosZ() const {
        return fPosZ;
}

#endif
