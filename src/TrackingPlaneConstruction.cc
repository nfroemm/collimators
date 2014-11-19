#include "TrackingPlaneConstruction.hh"
#include "TrackingPlaneSD.hh"

#include "globals.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"


TrackingPlaneConstruction::TrackingPlaneConstruction(G4String& parallelWorldName)
: G4VUserParallelWorld(parallelWorldName),
  BuildTrackingPlanes(true),
  numTrackingPlanes(36),
  hx(50*cm),
  hy(50*cm),
  hz(0.5*nanometer),
  TrackingPlane_S(NULL),
  TrackingPlane_L(NULL),
  TrackingPlane_P(NULL)
{}


TrackingPlaneConstruction::~TrackingPlaneConstruction()
{}


void TrackingPlaneConstruction::Construct()
{
  if (!BuildTrackingPlanes) return;
  if (numTrackingPlanes<1)  return;

  // GhostWorld
  G4LogicalVolume* GhostWorld_L = GetWorld()->GetLogicalVolume();

  // In this GhostWorld, there are tracking planes
  TrackingPlane_S = new G4Box("TrackingPlane", hx, hy, hz);

  // The tracking-plane material will be "NULL", i.e. no material
  TrackingPlane_L = new G4LogicalVolume(TrackingPlane_S, NULL, "TrackingPlane", 0, 0, 0, true);

  // The azimuthal separation of the tracking planes depends on how many planes there are
  G4double dTheta = 360.*degree/double(numTrackingPlanes);

  // The first tracking plane is at 12 o'clock
  G4ThreeVector*    TrackingPlane_Pos = new G4ThreeVector(0.,7.112*m,0.);
  G4RotationMatrix* TrackingPlane_Rot = new G4RotationMatrix();
  TrackingPlane_Rot->rotateZ( 90*degree );
  TrackingPlane_Rot->rotateY( 90*degree );

  // Place the tracking-plane physical volumes in the ghost world
  for (G4int ntp=0; ntp!=numTrackingPlanes; ++ntp) {
    if (ntp>0) {
      TrackingPlane_Rot->rotateZ( -dTheta );
      TrackingPlane_Pos->rotateZ( -dTheta );
    }
    TrackingPlane_P = new G4PVPlacement( G4Transform3D(*TrackingPlane_Rot,*TrackingPlane_Pos), TrackingPlane_L, "TrackingPlane", GhostWorld_L, true, ntp, false );
  }

  // Register the tracking planes as sensitive detectors
  TrackingPlaneSD* SD = new TrackingPlaneSD("TrackingPlane","TrackingPlaneHitsCollection");
  SetSensitiveDetector ("TrackingPlane", SD, false);
}


