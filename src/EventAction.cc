#include "EventAction.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"

#include "RootManager.hh"
#include "TrackingPlaneSD.hh"
#include "StorageRegionSD.hh"
#include "CollimatorSD.hh"

#include <iomanip>
#include <cmath>


EventAction::EventAction(RootManager* rootMgr)
: G4UserEventAction(),
  fRootManager(rootMgr),
  tphc(0),
  srhc(0),
  chc(0)
{}


EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event* event)
{
  G4int eventID = event->GetEventID();
//if (eventID%100==0) {
    G4cout << G4endl;
    G4cout << ">>> Event " << eventID << G4endl;
    G4cout << G4endl;
//}
}


void EventAction::EndOfEventAction(const G4Event* event)
{
  FillTrackingPlaneTree(event);
  FillStorageRegionTree(event);
  FillCollimatorTree(event);
}


void EventAction::FillTrackingPlaneTree(const G4Event* event)
{
  // Get the ID number of the tracking-plane hits collection
  G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID("TrackingPlaneHitsCollection");
  if (hcid<0) return;

  // Get the tracking-plane hits collection
  tphc = static_cast<TrackingPlaneHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcid));
  if (!tphc) return;

  // Loop over the hits
  for (G4int i=0; i!=tphc->entries(); ++i)
  {
    // Get the hit
    TrackingPlaneHit* tph = (*tphc)[i];

    // Define some helper varibles
    G4int muonID, trackID, pdgID, nplane;
    G4double nturn, time, rglobal, uglobal, rlocal, ulocal;
    G4ThreeVector pos, mom, pol;

    // Get hit info
    muonID  = tph->GetMuonID();
    trackID = tph->GetTrackID();
    pdgID   = tph->GetPDGID();
    nplane  = tph->GetTrackingPlaneNum();
    nturn   = tph->GetTurnNum();
    rglobal = tph->GetGlobalRadius();
    uglobal = tph->GetGlobalAngle();
    rlocal  = tph->GetLocalRadius();
    ulocal  = tph->GetLocalAngle();
    time    = tph->GetTime();
    pos     = tph->GetPosition();
    mom     = tph->GetMomentum();
    pol     = tph->GetPolarization();

    // Add data to the TTree
    fRootManager->FillTrackingPlaneTree(
      muonID,  trackID, pdgID,  nplane, nturn,
      rglobal, uglobal, rlocal, ulocal,
      time, pos, mom, pol
    );
  }
}


void EventAction::FillStorageRegionTree(const G4Event* event)
{
  // Get the ID number of the tracking-plane hits collection
  G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID("StorageRegionHitsCollection");
  if (hcid<0) return;

  // Get the tracking-plane hits collection
  srhc = static_cast<StorageRegionHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcid));
  if (!srhc) return;

  // Loop over the hits
  for (G4int i=0; i!=srhc->entries(); ++i)
  {
    // Get the hit
    StorageRegionHit* srh = (*srhc)[i];

    // Define some helper varibles
    G4int muonID, trackID, pdgID, nturn;
    G4double time, rglobal, uglobal, rlocal, ulocal;
    G4ThreeVector pos, mom, pol;

    // Get hit info
    muonID  = srh->GetMuonID();
    trackID = srh->GetTrackID();
    pdgID   = srh->GetPDGID();
    nturn   = srh->GetTurnNum();
    rglobal = srh->GetGlobalRadius();
    uglobal = srh->GetGlobalAngle();
    rlocal  = srh->GetLocalRadius();
    ulocal  = srh->GetLocalAngle();
    time    = srh->GetTime();
    pos     = srh->GetPosition();
    mom     = srh->GetMomentum();
    pol     = srh->GetPolarization();

    // Add data to the TTree
    fRootManager->FillStorageRegionTree(
      muonID,  trackID, pdgID,  nturn,
      rglobal, uglobal, rlocal, ulocal,
      time, pos, mom, pol
    );
  }
}


void EventAction::FillCollimatorTree(const G4Event* event)
{
  // Get the ID number of the tracking-plane hits collection
  G4int hcid = G4SDManager::GetSDMpointer()->GetCollectionID("CollimatorHitsCollection");
  if (hcid<0) return;

  // Get the tracking-plane hits collection
  chc = static_cast<CollimatorHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcid));
  if (!chc) return;

  // Loop over the hits
  for (G4int i=0; i!=chc->entries(); ++i)
  {
    // Get the collimator hit from the hits collection
    CollimatorHit* ch = (*chc)[i];

    // Define some helper variables
    G4int muonID, trackID, pdgID, ncol, nturn;
    G4double r0g, u0g, r0l, u0l, t0;
    G4ThreeVector pos0, mom0;
    G4double p0mag;
    G4double r1g, u1g, r1l, u1l, t1;
    G4ThreeVector pos1, mom1;
    G4double p1mag;

    // Get hit info
    muonID  = ch->GetMuonID();
    trackID = ch->GetTrackID();
    pdgID   = ch->GetPDGID();
    ncol    = ch->GetCollNum();
    nturn   = ch->GetTurnNum();

    r0g   = ch->GetGlobalRadiusEnter();
    u0g   = ch->GetGlobalAngleEnter()/degree;
    r0l   = ch->GetLocalRadiusEnter();
    u0l   = ch->GetLocalAngleEnter()/degree;
    t0    = ch->GetTimeEnter();
    pos0  = ch->GetPosEnter();
    mom0  = ch->GetMomEnter();
    p0mag = ch->GetMomMagEnter();

    r1g   = ch->GetGlobalRadiusExit();
    u1g   = ch->GetGlobalAngleExit()/degree;
    r1l   = ch->GetLocalRadiusExit();
    u1l   = ch->GetLocalAngleExit()/degree;
    t1    = ch->GetTimeExit();
    pos1  = ch->GetPosExit();
    mom1  = ch->GetMomExit();
    p1mag = ch->GetMomMagExit();

    // Fill
    fRootManager->FillCollimatorTree(
      muonID, trackID, pdgID, ncol, nturn,
      r0g, u0g, r0l, u0l, t0, pos0, mom0, p0mag,
      r1g, u1g, r1l, u1l, t1, pos1, mom1, p1mag
    );
  }
}


