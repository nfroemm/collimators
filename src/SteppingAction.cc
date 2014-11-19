#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"


SteppingAction::SteppingAction(void)
{}


SteppingAction::~SteppingAction()
{}


void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (aStep->GetTrack()->GetTrackID()==4) {
    // Kill decay electrons if they're hanging around too long
    G4int nturns_max = 500;
    G4double tmax = nturns_max*( 149.2*ns );
    if (aStep->GetTrack()->GetLocalTime()>tmax) aStep->GetTrack()->SetTrackStatus( fKillTrackAndSecondaries );
  }

#if 0
  G4double      time   = aStep->GetTrack()->GetGlobalTime();
  G4ThreeVector pos    = aStep->GetTrack()->GetPosition();
  G4ThreeVector pol    = aStep->GetTrack()->GetPolarization();
  G4ThreeVector mom    = aStep->GetTrack()->GetMomentum();
  G4ThreeVector momDir = aStep->GetTrack()->GetMomentumDirection();
  G4cout << time/ns << "\t" << pol.dot(momDir) << G4endl;

  G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  if (processName=="DecayWithSpin" ) {
    aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
  }



//if (aStep->GetPostStepPoint()->GetTime()>10.*ns) 

  G4String processName = aStep->GetPostStepPoint()->
                               GetProcessDefinedStep()->GetProcessName();

  if(processName != "DecayWithSpin" ){

     G4Track* aTrack= aStep->GetTrack();

     G4String particleName = aStep->GetTrack()->
                                    GetDefinition()->GetParticleName();

     G4ThreeVector polDir  = aTrack->GetPolarization();
     G4ThreeVector momDir  = aTrack->GetMomentumDirection();

     if (particleName == "mu+") {
        if (momDir * polDir < (1.-1.E-7)) {

           G4double cos_theta = momDir * polDir;
           G4double gTime     = aTrack->GetGlobalTime();

           G4cout << " *** ERROR - WARNING *** " << G4endl;
           G4cout << "processName: " << processName << G4endl;
           G4cout << "particleName " << particleName << G4endl;
           G4cout << "Global Time: " << gTime/ns << "nsec" << G4endl;
           G4cout << "Angle between spin and momentum:" << cos_theta << G4endl;
           G4Exception("SteppingAction::UserSteppingAction","Error",
                       FatalException,
                       "Angle between spin and momentum too large");
        }
     }
  }
#endif

}
