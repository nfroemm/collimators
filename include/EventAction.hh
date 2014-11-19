#ifndef EventAction_hh
#define EventAction_hh 1

#include "G4UserEventAction.hh"

#include "TrackingPlaneHit.hh"
#include "StorageRegionHit.hh"
#include "CollimatorHit.hh"

class RunAction;
class RootManager;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RootManager*);
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event*);
    virtual void    EndOfEventAction(const G4Event*);

  private:
    RootManager* fRootManager;
    TrackingPlaneHitsCollection* tphc;
    StorageRegionHitsCollection* srhc;
    CollimatorHitsCollection*     chc;

    void FillTrackingPlaneTree(const G4Event*);
    void FillStorageRegionTree(const G4Event*);
    void FillCollimatorTree(const G4Event*);
};

#endif
