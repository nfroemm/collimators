#include "WorldConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "G4RunManager.hh"

#include "G4UImanager.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "TrackingPlaneConstruction.hh"
#include "StorageRegionConstruction.hh"
#include "G4ParallelWorldPhysics.hh"
#include "Randomize.hh"



int main (int argc, char** argv)
{
  // Set the random number generator and seed
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(123);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Define some physics
  G4VModularPhysicsList* physicsList = new PhysicsList();

  // Create the tracking/normal world
  WorldConstruction* world = new WorldConstruction();

#if 1
  // Create the tracking-plane parallel world
  G4String parallelWorld01Name = "TrackingPlaneWorld";
  TrackingPlaneConstruction* parallelWorld01 = new TrackingPlaneConstruction(parallelWorld01Name);
  world->RegisterParallelWorld(parallelWorld01);
  physicsList->RegisterPhysics(new G4ParallelWorldPhysics(parallelWorld01Name,true));
#endif

#if 1
  // Create the storage-region parallel world
  G4String parallelWorld02Name = "StorageRegionWorld";
  StorageRegionConstruction* parallelWorld02 = new StorageRegionConstruction(parallelWorld02Name);
  world->RegisterParallelWorld(parallelWorld02);
  physicsList->RegisterPhysics(new G4ParallelWorldPhysics(parallelWorld02Name,true));
#endif

  // Register world(s) with the run manager
  runManager->SetUserInitialization(world);

  // Register physics with the run manager
  runManager->SetUserInitialization(physicsList);

  // Set other mandatory initialization classes
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize G4 kernel
  runManager->Initialize();

  // Get pointer to user-interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive("warnings");
  visManager->Initialize();
#endif

  if (argc!=1) {
    // batch
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  } else {
    // interactive
    #ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    #else
    UImanager->ApplyCommand("/control/execute init.mac");
    #endif
    ui->SessionStart();
    delete ui;
    #endif
  }

  // Job termination
  #ifdef G4VIS_USE
  delete visManager;
  #endif
  delete runManager;
  return 0;
}

