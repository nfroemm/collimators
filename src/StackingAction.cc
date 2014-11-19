#include "StackingAction.hh"

#include "G4Track.hh"


StackingAction::StackingAction()
{}


StackingAction::~StackingAction()
{}


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
  // Get the trackID
  G4int trackID = track->GetTrackID();

  if (trackID==1) {
    // Muons (and all primary particles) are most important
    return fUrgent;

  } else if (trackID==4) {
    // Postpone tracking of decay electrons until after the urgent stack is empty (see above)
    return fKill;
    return fWaiting;

  } else if ( (trackID==2) || (trackID==3) ){
    // Kill secondary neutrinos
    return fKill;

  } else {
    // Kill everything else
    return fKill;
  }
}


