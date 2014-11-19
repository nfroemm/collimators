#include "StorageRegionSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AffineTransform.hh"

#include <cmath>


StorageRegionSD::StorageRegionSD(const G4String& name, const G4String& hitsCollectionName) 
: G4VSensitiveDetector(name),
  verbosity(0),
  killTracksLeaving(true),
  rkill(4.75*cm),
  fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}


StorageRegionSD::~StorageRegionSD() 
{}


void StorageRegionSD::Initialize(G4HCofThisEvent* hce)
{
  // Create the hits collection
  fHitsCollection = new StorageRegionHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}


StorageRegionHit* StorageRegionSD::MakeHit(const G4Step* step, const G4StepPoint* pt)
{
  // Define some helper variables
  G4String       particleName;
  G4int          muonID, trackID, pdgID, nturn;
  G4double       time, rglobal, uglobal, rlocal, ulocal;
  G4ThreeVector  pos, mom, pol;

  // Get basic hit information
  particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  muonID  = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  trackID = step->GetTrack()->GetTrackID();
  pdgID   = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  nturn   = 0;
  time    = pt->GetGlobalTime();
  pos     = pt->GetPosition();
  mom     = pt->GetMomentum();
  pol     = pt->GetPolarization();
  rglobal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  uglobal = atan2( pos.x(), pos.y() ); if (uglobal<0) uglobal+=2.*M_PI;

  // Position
  G4double x = rglobal - 7.112*m;
  G4double y = pos.z();
  G4double z = ( 7.112*m )*uglobal;
  pos = G4ThreeVector(x,y,z);
  rlocal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  ulocal = atan2( pos.y(), pos.x() ); if (ulocal<0) ulocal+=2.*M_PI;

  // Momentum
  mom.rotateZ(uglobal);
  G4double px = mom.y();
  G4double py = mom.z();
  G4double pz = mom.x();
  mom = G4ThreeVector(px,py,pz);

  // If the track is a muon, transform momentum to beam-optical
  if (trackID==1) {
    G4double xp = mom.x()/(3094.349530048523*MeV);
    G4double yp = mom.y()/(3094.349530048523*MeV);
    G4double dpp = (mom.mag()-3094.349530048523*MeV)/(3094.349530048523*MeV);
    mom = G4ThreeVector(xp,yp,dpp);
  }

  // Polarization
  pol.rotateZ(uglobal);

  // Create the new hit
  StorageRegionHit* newHit = new StorageRegionHit();

  // Add hit info
  newHit->SetParticleName( particleName );
  newHit->SetMuonID( muonID );
  newHit->SetTrackID( trackID );
  newHit->SetPDGID( pdgID );
  newHit->SetTurnNum( nturn );
  newHit->SetGlobalRadius( rglobal );
  newHit->SetGlobalAngle( uglobal );
  newHit->SetLocalRadius( rlocal );
  newHit->SetLocalAngle( ulocal );
  newHit->SetTime( time );
  newHit->SetPosition( pos );
  newHit->SetMomentum( mom );
  newHit->SetPolarization( pol );

  // Kill the track if it is leaving the storage region
  if (killTracksLeaving) {
    if (pt==step->GetPostStepPoint()) {
      if (rlocal>rkill) {
        G4ThreeVector rhat = G4ThreeVector(cos(ulocal),sin(ulocal),0);
        if (mom.dot(rhat)>0) {
          step->GetTrack()->SetTrackStatus(fStopAndKill);
          step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
        }
      }
    }
  }

  // Return the hit
  return newHit;
}


G4bool StorageRegionSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4int trackID = step->GetTrack()->GetTrackID();
  // Only process mu+/mu- hits
  if (trackID!=1) return false;
//if ( (trackID!=1) || (trackID!=4) )return false;

  // Get the pre- and post-step point
  G4StepPoint* pt0 = step->GetPreStepPoint();
  G4StepPoint* pt1 = step->GetPostStepPoint();

  // The if/else logic below is [a crude attempt] to avoid double counting!
  if (pt1->GetStepStatus()==fGeomBoundary)
  {
    // Create the new hit
    StorageRegionHit* newHit = MakeHit(step,pt1);

    // Add the hit to the collection
    fHitsCollection->insert(newHit);
  } 
  else if (pt0->GetStepStatus()==fGeomBoundary)
  {
#if 0
    // Create the new hit
    StorageRegionHit* newHit = MakeHit(step,pt0);

    // Add the hit to the collection
    fHitsCollection->insert(newHit);
#endif
  }

  // Process hits
  return true;
}


void StorageRegionSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int nhits = fHitsCollection->entries();
  if (verbosity>0) for (G4int i=0; i<nhits; ++i) (*fHitsCollection)[i]->Print();
  G4cout << "STORAGE-REGION HITS: " << nhits << " hits\n\n";
}

