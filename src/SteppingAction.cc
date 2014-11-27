#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"


SteppingAction::SteppingAction(void):
  tMaxMuon_(30.*microsecond),
  tMaxElectron_(1.*microsecond)
{}


SteppingAction::~SteppingAction()
{}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Get the particle identification
  G4int pdgID = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();

  if (abs(pdgID)==13) {
    // Kill muons if they're hanging around too long
    G4double tGlobal = step->GetTrack()->GetGlobalTime();
    if (tGlobal>tMaxMuon_) step->GetTrack()->SetTrackStatus(fStopAndKill);

  } else if (abs(pdgID)==11) {
    // Kill electrons if they're hanging around too long
    G4double tLocal = step->GetTrack()->GetLocalTime();
    if (tLocal>tMaxElectron_) step->GetTrack()->SetTrackStatus(fStopAndKill);
  }

#if 0
  else {
  }

  G4double      time   = step->GetTrack()->GetGlobalTime();
  G4ThreeVector pos    = step->GetTrack()->GetPosition();
  G4ThreeVector pol    = step->GetTrack()->GetPolarization();
  G4ThreeVector mom    = step->GetTrack()->GetMomentum();
  G4ThreeVector momDir = step->GetTrack()->GetMomentumDirection();
  G4cout << time/ns << "\t" << pol.dot(momDir) << G4endl;

  G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  if (processName=="DecayWithSpin" ) {
    step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
  }
#endif

}
