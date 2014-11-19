#include "CollimatorMessenger.hh"
#include "CollimatorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


CollimatorMessenger::CollimatorMessenger(CollimatorConstruction* collConstruction)
 : G4UImessenger(),
   collConstruction_(collConstruction)
{
  // Setup a new director in the user interface
  collDir_ = new G4UIdirectory("/collimator/");
  collDir_->SetGuidance("UI commands for setting collimator geometry/material.");

  // Create a command for changing the collimator material
  collMaterialCmd_ = new G4UIcmdWithAString("/collimator/setMaterial",this);
  collMaterialCmd_->SetGuidance("Set collimator material.");
  collMaterialCmd_->SetParameterName("choice",false);
  collMaterialCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);
}


CollimatorMessenger::~CollimatorMessenger()
{
  delete collDir_;
  delete collMaterialCmd_;
}


void CollimatorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command==collMaterialCmd_) 
    collConstruction_->SetMaterial(newValue); return;
}


