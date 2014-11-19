#ifndef StorageRegionHit_h
#define StorageRegionHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
//#include "tls.hh" // multithreading


class StorageRegionHit : public G4VHit
{
  public:
    StorageRegionHit();
    StorageRegionHit(const StorageRegionHit&);
    virtual ~StorageRegionHit();

    // Methods from base class "G4VHit"
    virtual void Draw();
    virtual void Print();

    // Operators
    const StorageRegionHit& operator=(const StorageRegionHit&);
    G4int operator==(const StorageRegionHit&) const;
    inline void* operator new(size_t);
    inline void  operator delete(void*);

  private:
    G4String      particleName;
    G4int         muonID;
    G4int         trackID;
    G4int         pdgID;
    G4int         nturn;
    G4double      rglobal;
    G4double      uglobal;
    G4double      rlocal;
    G4double      ulocal;
    G4double      time;
    G4ThreeVector pos;
    G4ThreeVector mom;
    G4ThreeVector pol;

  public: 
    // Get methods
    G4String      GetParticleName() const { return particleName; }
    G4int         GetMuonID()       const { return muonID;  }
    G4int         GetTrackID()      const { return trackID; }
    G4int         GetPDGID()        const { return pdgID;   }
    G4int         GetTurnNum()      const { return nturn;   }
    G4double      GetGlobalRadius() const { return rglobal; }
    G4double      GetGlobalAngle()  const { return uglobal; }
    G4double      GetLocalRadius()  const { return rlocal;  }
    G4double      GetLocalAngle()   const { return ulocal;  }
    G4double      GetTime()         const { return time;    }
    G4ThreeVector GetPosition()     const { return pos;     }
    G4ThreeVector GetMomentum()     const { return mom;     }
    G4ThreeVector GetPolarization() const { return pol;     }

    // Set methods
    void SetParticleName (G4String str)    { particleName = str; }
    void SetMuonID       (G4int i)         {  muonID = i; }
    void SetTrackID      (G4int i)         { trackID = i; }
    void SetPDGID        (G4int i)         {   pdgID = i; }
    void SetTurnNum      (G4int i)         {   nturn = i; }
    void SetGlobalRadius (G4double d)      { rglobal = d; }
    void SetGlobalAngle  (G4double d)      { uglobal = d; }
    void SetLocalRadius  (G4double d)      { rlocal  = d; }
    void SetLocalAngle   (G4double d)      { ulocal  = d; }
    void SetTime         (G4double d)      {    time = d; }
    void SetPosition     (G4ThreeVector v) {     pos = v; }
    void SetMomentum     (G4ThreeVector v) {     mom = v; }
    void SetPolarization (G4ThreeVector v) {     pol = v; }
};


typedef G4THitsCollection<StorageRegionHit> StorageRegionHitsCollection;

extern G4ThreadLocal G4Allocator<StorageRegionHit>* StorageRegionHitAllocator;

inline void* StorageRegionHit::operator new(size_t)
{
  if (!StorageRegionHitAllocator) StorageRegionHitAllocator = new G4Allocator<StorageRegionHit>;
  return (void *) StorageRegionHitAllocator->MallocSingle();
}

inline void StorageRegionHit::operator delete(void *hit)
{
  StorageRegionHitAllocator->FreeSingle((StorageRegionHit*) hit);
}

#endif
