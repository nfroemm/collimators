#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4DecayWithSpin.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
#include "G4PionDecayMakeSpin.hh"

#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessTable.hh"
#include "G4DecayTable.hh"
#include "G4ProcessManager.hh"

// See physics list in /Users/nfroemm/Code/geant/x_old/geant4.9.6.p03/examples/extended/runAndEvent/RE04/
//#include "G4EmStandardPhysics_option4.hh"


PhysicsList::PhysicsList() : G4VModularPhysicsList() 
{
  G4int verbosity = 1;
  RegisterPhysics(new G4EmStandardPhysics(verbosity));
  RegisterPhysics(new G4EmExtraPhysics(verbosity));
  RegisterPhysics(new G4DecayPhysics(verbosity));
}


PhysicsList::~PhysicsList() {}


void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
  G4GenericIon::GenericIonDefinition();

  G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
  MuonPlusDecayTable->Insert(new G4MuonDecayChannelWithSpin("mu+",0.986));
  MuonPlusDecayTable->Insert(new G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
  G4MuonPlus::MuonPlusDefinition()->SetDecayTable(MuonPlusDecayTable);

  G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
  MuonMinusDecayTable->Insert(new G4MuonDecayChannelWithSpin("mu-",0.986));
  MuonMinusDecayTable->Insert(new G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
  G4MuonMinus::MuonMinusDefinition()->SetDecayTable(MuonMinusDecayTable);
}


void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
  G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();
  G4ProcessTable*   processTable = G4ProcessTable::GetProcessTable();

  G4VProcess* decay;
  decay = processTable->FindProcess("Decay",G4MuonPlus::MuonPlus());

  G4ProcessManager* fManager;
  fManager = G4MuonPlus::MuonPlus()->GetProcessManager();

  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(decayWithSpin);
    // Set ordering for PostStepDoIt and AtRestDoIt
    fManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
    fManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
  }

  decay = processTable->FindProcess("Decay",G4MuonMinus::MuonMinus());

  fManager = G4MuonMinus::MuonMinus()->GetProcessManager();

  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(decayWithSpin);
    // Set ordering for PostStepDoIt and AtRestDoIt
    fManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
    fManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
  }

  G4PionDecayMakeSpin* poldecay = new G4PionDecayMakeSpin();

  decay = processTable->FindProcess("Decay",G4PionPlus::PionPlus());

  fManager = G4PionPlus::PionPlus()->GetProcessManager();

  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(poldecay);
    // Set ordering for PostStepDoIt and AtRestDoIt
    fManager ->SetProcessOrdering(poldecay, idxPostStep);
    fManager ->SetProcessOrdering(poldecay, idxAtRest);
  }

  decay = processTable->FindProcess("Decay",G4PionMinus::PionMinus());

  fManager = G4PionMinus::PionMinus()->GetProcessManager();

  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(poldecay);
    // Set ordering for PostStepDoIt and AtRestDoIt
    fManager ->SetProcessOrdering(poldecay, idxPostStep);
    fManager ->SetProcessOrdering(poldecay, idxAtRest);
  }
}


void PhysicsList::SetCuts()
{
  SetCutsWithDefault();
}

