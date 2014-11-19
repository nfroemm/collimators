#include "QuadConstruction.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"


QuadConstruction::QuadConstruction()
: G4MaterialName("G4_Galactic"),
  QuadShort_S(0),
  QuadShort_L(0),
  QuadShort_P(0),
  QuadLong_S(0),
  QuadLong_L(0),
  QuadLong_P(0)
{}


QuadConstruction::~QuadConstruction()
{}


void QuadConstruction::BuildQuads(G4LogicalVolume* Mother_L)
{
  // Short 13-degree sections
  G4RotationMatrix*  QuadShort_Rot;
  QuadShort_S   = new G4Tubs("QuadShort", (7.112-0.1)*m, (7.112+0.1)*m, 7*cm, (90-13)*degree, 13*degree);
  QuadShort_L   = new G4LogicalVolume( QuadShort_S, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), "QuadShort", 0, 0, 0, true );
  QuadShort_Rot = new G4RotationMatrix();
  QuadShort_Rot->rotateZ(-21.9*degree);
  for (G4int qnum=0; qnum!=4; ++qnum) {
    if (qnum>0) QuadShort_Rot->rotateZ(-90*degree);
    QuadShort_P = new G4PVPlacement(G4Transform3D(*QuadShort_Rot,G4ThreeVector()), QuadShort_L, "QuadShort", Mother_L, true, qnum, true);
  }

  // Long 26-degree sections
  G4RotationMatrix*  QuadLong_Rot;
  QuadLong_S   = new G4Tubs("QuadLong", (7.112-0.1)*m, (7.112+0.1)*m, 7*cm, (90-26)*degree, 26*degree);
  QuadLong_L   = new G4LogicalVolume( QuadLong_S, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), "QuadLong", 0, 0, 0, true );
  QuadLong_Rot = new G4RotationMatrix();
  QuadLong_Rot->rotateZ(-(21.9+4+13)*degree);
  for (G4int qnum=0; qnum!=4; ++qnum) {
    if (qnum>0) QuadLong_Rot->rotateZ(-90*degree);
    QuadLong_P = new G4PVPlacement(G4Transform3D(*QuadLong_Rot,G4ThreeVector()), QuadLong_L, "QuadLong", Mother_L, true, qnum, true);
  }
}

