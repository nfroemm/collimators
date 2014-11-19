#include "TrackingPlaneSD.hh"
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


TrackingPlaneSD::TrackingPlaneSD(const G4String& name, const G4String& hitsCollectionName) 
: G4VSensitiveDetector(name),
  verbosity(0),
  fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}


TrackingPlaneSD::~TrackingPlaneSD() 
{}


void TrackingPlaneSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new TrackingPlaneHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}


G4bool TrackingPlaneSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // Only process mu+/mu- hits
  G4int trackID = step->GetTrack()->GetTrackID();
  if (trackID!=1) return false;

  // Define some helper variables
  G4String      particleName;
  G4int         muonID, pdgID, nplane;
  G4double      nturn, time, rglobal, uglobal, rlocal, ulocal;
  G4ThreeVector pos, mom, pol;

  // Get the pre- and post-step point
  G4StepPoint *pt0 = step->GetPreStepPoint();
  G4StepPoint *pt1 = step->GetPostStepPoint();

  // Get hit information
  particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  muonID  = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  trackID = step->GetTrack()->GetTrackID();
  pdgID   = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  nplane  = pt0->GetTouchableHandle()->GetCopyNumber(); // yes, pt0
  nturn   = 0;
  time    = pt0->GetGlobalTime();
  pos     = pt1->GetPosition();
  mom     = pt0->GetMomentum();
  pol     = pt1->GetPolarization();
  rglobal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  uglobal = atan2( pos.x(), pos.y() ); if (uglobal<0) uglobal+=2.*M_PI;

#if 0
  // Calculate the turn number
  G4int nentries = fHitsCollection->entries();
  if (nentries==0) nturn=0.;
  else {
    G4double nturn0   = (*fHitsCollection)[nentries-1]->GetTurnNum();
    G4double uglobal0 = (*fHitsCollection)[nentries-1]->GetGlobalAngle();
    nturn = nturn0 + (uglobal-uglobal0)/(2*M_PI);
  }

  const G4TouchableHandle &pH0 = pt0->GetTouchableHandle();
  const G4TouchableHandle &pH1 = pt1->GetTouchableHandle();
  G4cout << " PreStepPoint: " << pH0->GetVolume(0)->GetName() << G4endl;
  G4cout << "PostStepPoint: " << pH1->GetVolume(0)->GetName() << G4endl;
#endif

  // Transform to local coordinates
  pos    = pt0->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(pos);
  rlocal = sqrt( pow(pos.x(),2) + pow(pos.y(),2) );
  ulocal = atan2( pos.y(), pos.x() ); if (ulocal<0) ulocal+=2.*M_PI;
  pos.setZ( (7.112*m)*uglobal );
  mom *= pt0->GetTouchableHandle()->GetHistory()->GetTopTransform().NetRotation().inverse();
  pol *= pt1->GetTouchableHandle()->GetHistory()->GetTopTransform().NetRotation().inverse();

  // If the track is a muon, transform momentum to beam-optical
  if (trackID==1) {
    G4double xp = mom.x()/(3094.349530048523*MeV);
    G4double yp = mom.y()/(3094.349530048523*MeV);
    G4double dpp = (mom.mag()-3094.349530048523*MeV)/(3094.349530048523*MeV);
    mom = G4ThreeVector(xp,yp,dpp);
  }

  // Create the hit
  TrackingPlaneHit* newHit = new TrackingPlaneHit();

  // Fill hit info
  newHit->SetParticleName( particleName );
  newHit->SetMuonID( muonID );
  newHit->SetTrackID( trackID );
  newHit->SetPDGID( pdgID );
  newHit->SetTrackingPlaneNum( nplane );
  newHit->SetTurnNum( nturn );
  newHit->SetGlobalRadius( rglobal );
  newHit->SetGlobalAngle( uglobal );
  newHit->SetLocalRadius( rlocal );
  newHit->SetLocalAngle( ulocal );
  newHit->SetTime( time );
  newHit->SetPosition( pos );
  newHit->SetMomentum( mom );
  newHit->SetPolarization( pol );

  // Insert the hit into the hits collection
  fHitsCollection->insert( newHit );

  return true;
}


void TrackingPlaneSD::EndOfEvent(G4HCofThisEvent*)
{
  if (!fHitsCollection) return;

  // Get the number of hits in the collection
  G4int nhits = fHitsCollection->entries();

  // Print hits if the user has instructed
  if (verbosity>0) {
    if (verbosity>1) {
      // Print all hits
      for (G4int i=0; i!=nhits; ++i) {
        G4cout << i+1 << G4endl;
        (*fHitsCollection)[i]->Print();
      }
    } else {
      // Print only the first few hits
      G4int printmax = 10;
      for (G4int i=0; i!=std::min(nhits,printmax); ++i) {
        G4cout << i+1 << G4endl;
        (*fHitsCollection)[i]->Print();
      }
      if (nhits>printmax) G4cout << "<--- SNIP --->\n\n";
    }
  }

  G4cout << "TRACKING-PLANE HITS: " << nhits << " hits\n\n";
}


