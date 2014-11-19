#include "RunAction.hh"

#include "G4Run.hh"
#include "RootManager.hh"

#include "G4RunManager.hh"
#include "ParticleGun.hh"
#include "WorldConstruction.hh"



RunAction::RunAction(RootManager* rootMan)
 : G4UserRunAction(),
 rootMgr_(rootMan)
{}


RunAction::~RunAction()
{}


G4Run* RunAction::GenerateRun()
{
  return 0;
}


void RunAction::BeginOfRunAction(const G4Run*)
{
  rootMgr_->Book();
}


void RunAction::EndOfRunAction(const G4Run*)
{
  rootMgr_->Save();
}


