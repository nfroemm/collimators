#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "ParticleGun.hh"

#include "RootManager.hh"


ActionInitialization::ActionInitialization()
: G4VUserActionInitialization(),
  rootMgr_(0)
{
  rootMgr_ = new RootManager();
}


ActionInitialization::~ActionInitialization()
{
  delete rootMgr_;
}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction( new RunAction(rootMgr_) );
}


void ActionInitialization::Build() const
{
  SetUserAction( new ParticleGun()         );
  SetUserAction( new RunAction(rootMgr_)   );
  SetUserAction( new EventAction(rootMgr_) );
  SetUserAction( new SteppingAction()      );
  SetUserAction( new StackingAction()      );
}  


