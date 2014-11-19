#include "VacuumChamberConstruction.hh"

#include "globals.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"


VacuumChamberConstruction::VacuumChamberConstruction()
: hx(10.2*cm),
  hy( 7.0*cm),
  RingVacuum_S(0),
  RingVacuum_L(0),
  RingVacuum_P(0)
{}


VacuumChamberConstruction::~VacuumChamberConstruction()
{}


void VacuumChamberConstruction::BuildVacCmbr( G4LogicalVolume* Mother_L )
{
  RingVacuum_S = new G4Tubs("RingVacuum", (7.112*m-hx), (7.112*m+hx), hy, 0*degree, 360*degree);
  RingVacuum_L = new G4LogicalVolume(RingVacuum_S, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), "RingVacuum", 0, 0, 0, true);
  RingVacuum_P = new G4PVPlacement(0, G4ThreeVector(), RingVacuum_L, "RingVacuum", Mother_L, false, 0, true);
}


