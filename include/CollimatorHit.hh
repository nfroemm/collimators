#ifndef CollimatorHit_h
#define CollimatorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
//#include "tls.hh" // multithreading


class CollimatorHit : public G4VHit
{
  public:
    CollimatorHit();
    CollimatorHit(const CollimatorHit&);
    virtual ~CollimatorHit();

    // Methods from base class, "G4VHit"
    virtual void Draw();
    virtual void Print();

    // Operators
    const CollimatorHit& operator=(const CollimatorHit&);
    G4int operator==(const CollimatorHit&) const;
    inline void* operator new(size_t);
    inline void  operator delete(void*);

  private:
    // Generic hit information
    G4String particleName;
    G4int muonID;
    G4int trackID;
    G4int pdgID;
    G4int ncoll;
    G4int nturn;
    // When the muon ENTERS the collimator
    G4double      t0;
    G4ThreeVector x0;
    G4ThreeVector p0;
    G4double      p0mag;
    G4double      r0global;
    G4double      u0global;
    G4double      r0local;
    G4double      u0local;
    // When the muon EXITS the collimator
    G4double      t1;
    G4ThreeVector x1;
    G4ThreeVector p1;
    G4double      p1mag;
    G4double      r1global;
    G4double      u1global;
    G4double      r1local;
    G4double      u1local;

  public:
    // Basic hitID
    G4String      GetParticleName()      const { return particleName; }
    G4int         GetMuonID()            const { return muonID; }
    G4int         GetTrackID()           const { return trackID; }
    G4int         GetPDGID()             const { return pdgID; }
    G4int         GetCollNum()           const { return ncoll; }
    G4int         GetTurnNum()           const { return nturn; }
    // Entrance info
    G4double      GetTimeEnter()         const { return t0; }
    G4ThreeVector GetPosEnter()          const { return x0; }
    G4ThreeVector GetMomEnter()          const { return p0; }
    G4double      GetMomMagEnter()       const { return p0mag; }
    G4double      GetGlobalRadiusEnter() const { return r0global; }
    G4double      GetGlobalAngleEnter()  const { return u0global; }
    G4double      GetLocalRadiusEnter()  const { return r0local; }
    G4double      GetLocalAngleEnter()   const { return u0local; }
    // Exit info
    G4double      GetTimeExit()          const { return t1; }
    G4ThreeVector GetPosExit()           const { return x1; }
    G4ThreeVector GetMomExit()           const { return p1; }
    G4double      GetMomMagExit()        const { return p1mag; }
    G4double      GetGlobalRadiusExit()  const { return r1global; }
    G4double      GetGlobalAngleExit()   const { return u1global; }
    G4double      GetLocalRadiusExit()   const { return r1local; }
    G4double      GetLocalAngleExit()    const { return u1local; }

    // Set methods
    void SetParticleName      (G4String str)    { particleName = str; }
    void SetMuonID            (G4int i)         { muonID   = i; }
    void SetTrackID           (G4int i)         { trackID  = i; }
    void SetPDGID             (G4int i)         { pdgID    = i; }
    void SetCollNum           (G4int i)         { ncoll    = i; }
    void SetTurnNum           (G4int i)         { nturn    = i; }
    //
    void SetTimeEnter         (G4double d)      { t0       = d; }
    void SetPosEnter          (G4ThreeVector v) { x0       = v; }
    void SetMomEnter          (G4ThreeVector v) { p0       = v; }
    void SetMomMagEnter       (G4double d)      { p0mag    = d; }
    void SetGlobalRadiusEnter (G4double d)      { r0global = d; }
    void SetGlobalAngleEnter  (G4double d)      { u0global = d; }
    void SetLocalRadiusEnter  (G4double d)      { r0local  = d; }
    void SetLocalAngleEnter   (G4double d)      { u0local  = d; }
    //
    void SetTimeExit          (G4double d)      { t1       = d; }
    void SetPosExit           (G4ThreeVector v) { x1       = v; }
    void SetMomExit           (G4ThreeVector v) { p1       = v; }
    void SetMomMagExit        (G4double d)      { p1mag    = d; }
    void SetGlobalRadiusExit  (G4double d)      { r1global = d; }
    void SetGlobalAngleExit   (G4double d)      { u1global = d; }
    void SetLocalRadiusExit   (G4double d)      { r1local  = d; }
    void SetLocalAngleExit    (G4double d)      { u1local  = d; }
};


typedef G4THitsCollection<CollimatorHit> CollimatorHitsCollection;

extern G4ThreadLocal G4Allocator<CollimatorHit>* CollimatorHitAllocator;

inline void* CollimatorHit::operator new(size_t)
{
  if(!CollimatorHitAllocator) CollimatorHitAllocator = new G4Allocator<CollimatorHit>;
  return (void *) CollimatorHitAllocator->MallocSingle();
}


inline void CollimatorHit::operator delete(void *hit)
{
  CollimatorHitAllocator->FreeSingle((CollimatorHit*) hit);
}
#endif
