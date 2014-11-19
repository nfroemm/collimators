#include "StorageRegionConstruction.hh"
#include "StorageRegionSD.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"    


StorageRegionConstruction::StorageRegionConstruction(G4String& parallelWorldName)
: G4VUserParallelWorld(parallelWorldName),
  BuildStorageRegionTorus(true),
  Rmin(   0*mm),
  Rmax(  50*mm),
  Rtor(7112*mm),
  SPhi(0),
  DPhi(360*degree),
  StorageRegionTorus_S(NULL),
  StorageRegionTorus_L(NULL),
  StorageRegionTorus_P(NULL)
{}


StorageRegionConstruction::~StorageRegionConstruction()
{}


void StorageRegionConstruction::Construct()
{
  // GhostWorld
  if (!BuildStorageRegionTorus) return;
  G4LogicalVolume* GhostWorld_L = GetWorld()->GetLogicalVolume();

  // In this GhostWorld, there is a single G4Torus representing the storage region
  StorageRegionTorus_S = new G4Torus("StorageRegion", Rmin, Rmax, Rtor, SPhi, DPhi);

  // The storage-region material will be "NULL", i.e. no material
  StorageRegionTorus_L = new G4LogicalVolume(StorageRegionTorus_S, NULL, "StorageRegion", 0, 0, 0, true);

  // Place the storage region in its containing box
  StorageRegionTorus_P = new G4PVPlacement(0, G4ThreeVector(), StorageRegionTorus_L, "StorageRegion", GhostWorld_L, false, 0, true);

  // Register the storage region as a sensitive detector
  StorageRegionSD *SensitiveDetector = new StorageRegionSD("StorageRegion","StorageRegionHitsCollection");
  SetSensitiveDetector("StorageRegion", SensitiveDetector, false);
}


