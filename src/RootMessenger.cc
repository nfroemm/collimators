#include "RootMessenger.hh"
#include "RootManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


RootMessenger::RootMessenger(RootManager* rootMgr)
: G4UImessenger(),
  rootMgr_(rootMgr),
  rootUIDir_(0),
  rootOutputFileCmd_(0)
{
  // Make a new user-interface directory
  rootUIDir_ = new G4UIdirectory("/ROOT/");
  rootUIDir_->SetGuidance("UI commands for controlling ROOT output.");

  // Create a command for setting file directory
  rootOutputFileCmd_ = new G4UIcmdWithAString("/ROOT/setOutputFile",this);
  rootOutputFileCmd_->SetGuidance("Set output file.");
  rootOutputFileCmd_->SetParameterName("choice",false);
  rootOutputFileCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);
}


RootMessenger::~RootMessenger()
{
  delete rootUIDir_;
  delete rootOutputFileCmd_;
}


void RootMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  if (cmd==rootOutputFileCmd_) {
    rootMgr_->SetOutputFile(newValue);
    return;
  }
}

