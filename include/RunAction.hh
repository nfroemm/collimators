#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class RootManager;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(RootManager* rootMan);
    virtual ~RunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);

  private:
    RootManager* rootMgr_;
};

#endif
