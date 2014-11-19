// The world is a G4Box
#include "WorldConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

// Inside the world, there is a single vacuum chamber with EM-field
#include "VacuumChamberConstruction.hh"
#include "VacuumChamberField.hh"
#include "G4MagneticField.hh"
#include "G4ElectroMagneticField.hh"
#include "G4EqMagElectricField.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

// Lastly, there are beam collimators
#include "CollimatorConstruction.hh"
#include "CollimatorSD.hh"
#include "G4VisAttributes.hh"
using namespace std;


WorldConstruction::WorldConstruction()
: G4VUserDetectorConstruction(),
  World_S(0),
  World_L(0),
  World_P(0),
  vacCmbr_(0),
  collimators_(0)
{
  vacCmbr_ = new VacuumChamberConstruction();
  collimators_ = CollimatorConstruction::Instance();
}


WorldConstruction::~WorldConstruction()
{
  delete vacCmbr_;
//delete collimators_;
}


G4VPhysicalVolume* WorldConstruction::Construct()
{
  // Build the World
  World_S = new G4Box( "World", 10*m, 10*m, 10*cm );
  World_L = new G4LogicalVolume( World_S, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), "World", 0, 0, 0, true );
  World_P = new G4PVPlacement( 0, G4ThreeVector(), World_L, "World", 0, false, 0, true );

  // Vacuum chamber + collimators
  vacCmbr_->BuildVacCmbr( World_L );
  collimators_->BuildCollimators( vacCmbr_->RingVacuum_L );
  return World_P;
}


void WorldConstruction::ConstructSDandField()
{
  // Vacuum chamber EM-fields
  VacuumChamberField* vacCmbrField = new VacuumChamberField();
  G4ClassicalRK4*     vacCmbrStepper;
  G4ChordFinder*      vacCmbrChordFinder;
  G4FieldManager*     vacCmbrFieldManager;
  G4bool              vacCmbrFieldToDaughters = true;

  // Setup EM-field (with or without spin tracking)
  if (vacCmbrField->spinTracking_) {
    G4EqEMFieldWithSpin* vacCmbrEOM = new G4EqEMFieldWithSpin( vacCmbrField );
    vacCmbrStepper = new G4ClassicalRK4( vacCmbrEOM, 12 );
    vacCmbrChordFinder = new G4ChordFinder( (G4MagneticField*)vacCmbrField, 0.01*mm, vacCmbrStepper );
    vacCmbrFieldManager = new G4FieldManager( vacCmbrField, vacCmbrChordFinder, vacCmbrFieldToDaughters );
    vacCmbr_->RingVacuum_L->SetFieldManager( vacCmbrFieldManager, vacCmbrFieldToDaughters );
    printf("\namu = %18.14e\n\n",vacCmbrEOM->GetAnomaly());
  } else {
    G4EqMagElectricField* vacCmbrEOM = new G4EqMagElectricField( vacCmbrField );
    vacCmbrStepper = new G4ClassicalRK4( vacCmbrEOM, 8 );
    vacCmbrChordFinder = new G4ChordFinder( (G4MagneticField*)vacCmbrField, 0.01*mm, vacCmbrStepper );
    vacCmbrFieldManager = new G4FieldManager( vacCmbrField, vacCmbrChordFinder, vacCmbrFieldToDaughters );
    vacCmbr_->RingVacuum_L->SetFieldManager( vacCmbrFieldManager, vacCmbrFieldToDaughters );
  }

  // Collimator sensitive detector
  if (collimators_->constructed_) {
    CollimatorSD* SD = new CollimatorSD("Collimator","CollimatorHitsCollection");
    SetSensitiveDetector("Collimator", SD, true);
  }
}


