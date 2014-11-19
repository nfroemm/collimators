#include "CollimatorSD.hh"

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


CollimatorSD::CollimatorSD(const G4String& name, const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert( hitsCollectionName );
}


CollimatorSD::~CollimatorSD() 
{}


void CollimatorSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new CollimatorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}


void CollimatorSD::CreateHit(const G4Step* step)
{
  // Get the pre-step point
  G4StepPoint *pt = step->GetPreStepPoint();

  // Define some helper variables
  G4String      particleName;
  G4int         muonID, pdgID, trackID, ncoll, nturn;
  G4double      time, pmag, rglobal, uglobal, rlocal, ulocal;
  G4ThreeVector pos, mom;

  // Get hit information
  particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  muonID  = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  trackID = step->GetTrack()->GetTrackID();
  pdgID   = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  ncoll   = pt->GetTouchableHandle()->GetCopyNumber();
  nturn   = 0;
  time    = pt->GetGlobalTime();
  pos     = pt->GetPosition();
  mom     = pt->GetMomentum();
  pmag    = mom.mag();
  rglobal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  uglobal = atan2( pos.x(), pos.y() ); if (uglobal<0) uglobal+=2.*M_PI;

  // Transform to local
  G4double x = rglobal - 7.112*m;
  G4double y = pos.z();
  G4double z = ( 7.112*m )*uglobal;
  pos = G4ThreeVector(x,y,z);
  mom.rotateZ( uglobal ); // unrotate

  // Calculate local radius and angle
  rlocal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  ulocal = atan2( pos.y(), pos.x() ); if (ulocal<0) ulocal+=2.*M_PI;

  // If the track is a muon, transform momentum to beam-optical
  trackID = step->GetTrack()->GetTrackID();
  if (trackID==1) {
    G4double px = mom.y();
    G4double py = mom.z();
    G4double xp = px/(3094.349530048523*MeV);
    G4double yp = py/(3094.349530048523*MeV);
    G4double dpp = (mom.mag()-3094.349530048523*MeV)/(3094.349530048523*MeV);
    mom = G4ThreeVector(xp,yp,dpp);
  }

  // Create the hit
  CollimatorHit* newHit = new CollimatorHit();
  // Add hit info
  newHit->SetParticleName( particleName );
  newHit->SetMuonID( muonID );
  newHit->SetTrackID( trackID );
  newHit->SetPDGID( pdgID );
  newHit->SetCollNum( ncoll );
  newHit->SetTurnNum( nturn );
  newHit->SetTimeEnter( time );
  newHit->SetPosEnter( pos );
  newHit->SetMomEnter( mom );
  newHit->SetMomMagEnter( pmag );
  newHit->SetGlobalRadiusEnter( rglobal );
  newHit->SetGlobalAngleEnter( uglobal );
  newHit->SetLocalRadiusEnter( rlocal );
  newHit->SetLocalAngleEnter( ulocal );
  // Insert the hit into the hits collection
  fHitsCollection->insert( newHit );
}


void CollimatorSD::AddHitExitInfo(const G4Step* step)
{
  // Get the post-step point
  G4StepPoint* pt = step->GetPostStepPoint();

  // Define some helper variables
  G4int trackID;
  G4double time, pmag, rglobal, uglobal, rlocal, ulocal;
  G4ThreeVector pos, mom;

  // Get hit information
  trackID = step->GetTrack()->GetTrackID();
  time    = pt->GetGlobalTime();
  pos     = pt->GetPosition();
  mom     = pt->GetMomentum();
  pmag    = mom.mag();
  rglobal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  uglobal = atan2( pos.x(), pos.y() ); if (uglobal<0) uglobal+=2.*M_PI;

  // Transform to local
  G4double x = rglobal - 7.112*m;
  G4double y = pos.z();
  G4double z = ( 7.112*m )*uglobal;
  pos = G4ThreeVector(x,y,z);
  mom.rotateZ( uglobal );

  // Calculate local radius and angle
  rlocal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  ulocal = atan2( pos.y(), pos.x() ); if (ulocal<0) ulocal+=2.*M_PI;

  // If the track is a muon, transform momentum to beam-optical
  trackID = step->GetTrack()->GetTrackID();
  if (trackID==1) {
    G4double px = mom.y();
    G4double py = mom.z();
    G4double xp = px/(3094.349530048523*MeV);
    G4double yp = py/(3094.349530048523*MeV);
    G4double dpp = (mom.mag()-3094.349530048523*MeV)/(3094.349530048523*MeV);
    mom = G4ThreeVector(xp,yp,dpp);
  }

  // Add the hit exit info
  CollimatorHit* hit = (*fHitsCollection)[fHitsCollection->entries()-1];
  if (!hit) throw;
  hit->SetTimeExit( time );
  hit->SetPosExit( pos );
  hit->SetMomExit( mom );
  hit->SetMomMagExit( pmag );
  hit->SetGlobalRadiusExit( rglobal );
  hit->SetGlobalAngleExit( uglobal );
  hit->SetLocalRadiusExit( rlocal );
  hit->SetLocalAngleExit( ulocal );
}


G4bool CollimatorSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // Only process mu+/mu- and decay e+/e- hits
  G4int trackID = step->GetTrack()->GetTrackID();
  if ( (trackID!=1) && (trackID!=4) ) return false;

  // Get the pre- and post-step point
  G4StepPoint* pt0 = step->GetPreStepPoint();
  G4StepPoint* pt1 = step->GetPostStepPoint();
  G4bool processHits = false;

  // Track entering collimator?
  if (pt0->GetStepStatus()==fGeomBoundary) {
    CreateHit(step);
    processHits = true;
  }
  // Track exiting collimator?
  if (pt1->GetStepStatus()==fGeomBoundary) {
    AddHitExitInfo(step);
    processHits = true;
  }
  return processHits;
}


void CollimatorSD::EndOfEvent(G4HCofThisEvent*)
{
//G4int nhits = fHitsCollection->entries();
//for (G4int i=0; i!=nhits; ++i) (*fHitsCollection)[i]->Print();
//G4cout << "COLLIMATOR HITS: " << nhits << " hits\n\n";
}


