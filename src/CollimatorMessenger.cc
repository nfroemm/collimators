#include "CollimatorMessenger.hh"
#include "CollimatorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"


CollimatorMessenger::CollimatorMessenger(CollimatorConstruction* collConstruction)
: G4UImessenger(),
  collConstruction_(collConstruction)
{
  // Setup a new director in the user interface
  collDir_ = new G4UIdirectory("/collimator/");
  collDir_->SetGuidance("UI commands for setting collimator geometry/material.");

  // Command for changing the collimator material
  collMaterialCmd_ = new G4UIcmdWithAString("/collimator/setMaterial",this);
  collMaterialCmd_->SetGuidance("Set collimator material.");
  collMaterialCmd_->SetParameterName("choice",false);
  collMaterialCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Command for changing the collimator thickness
  collThicknessCmd_ = new G4UIcmdWithADoubleAndUnit("/collimator/setThickness",this);
  collThicknessCmd_->SetGuidance("Set collimator thickness.");
  collThicknessCmd_->SetParameterName("thickness",false);
  collThicknessCmd_->SetUnitCategory("Length");
  collThicknessCmd_->AvailableForStates(G4State_Idle);
}


CollimatorMessenger::~CollimatorMessenger()
{
  delete collDir_;
  delete collMaterialCmd_;
  delete collThicknessCmd_;
}


void CollimatorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  // Set collimator material
  if (command==collMaterialCmd_) {
    collConstruction_->SetMaterial(newValue);
    return;
  }
  // Set collimator thickness
  if (command==collThicknessCmd_) {
    collConstruction_->SetThickness( collThicknessCmd_->GetNewDoubleValue(newValue) );
    return;
  }
}


