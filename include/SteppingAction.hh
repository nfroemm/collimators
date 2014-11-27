#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"


class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(void);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    G4double tMaxMuon_;
    G4double tMaxElectron_;
};

#endif
