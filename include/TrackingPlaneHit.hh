#ifndef TrackingPlaneHit_h
#define TrackingPlaneHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
//#include "tls.hh" // multithreading


class TrackingPlaneHit : public G4VHit
{
  public:
    TrackingPlaneHit();
    TrackingPlaneHit(const TrackingPlaneHit&);
    virtual ~TrackingPlaneHit();

    // Methods inherited from base class, "G4VHit"
    virtual void Draw();
    virtual void Print();

    // Operators
    const TrackingPlaneHit& operator=(const TrackingPlaneHit&);
    G4int operator==(const TrackingPlaneHit&) const;
    inline void* operator new(size_t);
    inline void  operator delete(void*);

  private:
    // Definition of hit info
    G4String      particleName;
    G4int         muonID;
    G4int         trackID;
    G4int         pdgID;
    G4int         nplane;
    G4double      nturn;
    G4double      rglobal;
    G4double      uglobal; // "theta" = "u"
    G4double      rlocal;
    G4double      ulocal;
    G4double      time;
    G4ThreeVector pos;
    G4ThreeVector mom;
    G4ThreeVector pol;

  public:
    // Get methods
    G4String       GetParticleName()      const { return particleName; }
    G4int          GetMuonID()            const { return muonID;  }
    G4int          GetTrackID()           const { return trackID; }
    G4int          GetPDGID()             const { return pdgID;   }
    G4int          GetTrackingPlaneNum()  const { return nplane;  }
    G4double       GetTurnNum()           const { return nturn;   }
    G4double       GetGlobalRadius()      const { return rglobal; }
    G4double       GetGlobalAngle()       const { return uglobal; }
    G4double       GetLocalRadius()       const { return rlocal;  }
    G4double       GetLocalAngle()        const { return ulocal;  }
    G4double       GetTime()              const { return time;    }
    G4ThreeVector  GetPosition()          const { return pos;     }
    G4ThreeVector  GetMomentum()          const { return mom;     }
    G4ThreeVector  GetPolarization()      const { return pol;     }

    // Set methods
    void SetParticleName      (G4String str)     { particleName = str; }
    void SetMuonID            (G4int i)          { muonID  = i; }
    void SetTrackID           (G4int i)          { trackID = i; }
    void SetPDGID             (G4int i)          { pdgID   = i; }
    void SetTrackingPlaneNum  (G4int i)          { nplane  = i; }
    void SetTurnNum           (G4double d)       { nturn   = d; }
    void SetGlobalRadius      (G4double d)       { rglobal = d; }
    void SetGlobalAngle       (G4double d)       { uglobal = d; }
    void SetLocalRadius       (G4double d)       { rlocal  = d; }
    void SetLocalAngle        (G4double d)       { ulocal  = d; }
    void SetTime              (G4double d)       { time    = d; }
    void SetPosition          (G4ThreeVector v)  { pos     = v; }
    void SetMomentum          (G4ThreeVector v)  { mom     = v; }
    void SetPolarization      (G4ThreeVector v)  { pol     = v; }
};


typedef G4THitsCollection<TrackingPlaneHit> TrackingPlaneHitsCollection;

extern G4ThreadLocal G4Allocator<TrackingPlaneHit>* TrackingPlaneHitAllocator;

inline void* TrackingPlaneHit::operator new(size_t)
{
  if(!TrackingPlaneHitAllocator) TrackingPlaneHitAllocator = new G4Allocator<TrackingPlaneHit>;
  return (void *) TrackingPlaneHitAllocator->MallocSingle();
}

inline void TrackingPlaneHit::operator delete(void *hit)
{
  TrackingPlaneHitAllocator->FreeSingle((TrackingPlaneHit*) hit);
}
#endif
