#include "KickerConstruction.hh"

#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"


KickerConstruction::KickerConstruction()
 : 
  electrodeLength(1.7*m),
  gapLength(0.1*m),
  electrodeThickness(0.25*mm),
  G4MaterialName("G4_Al")
{}


KickerConstruction::~KickerConstruction()
{}


void KickerConstruction::BuildKickers(G4LogicalVolume* Mother_L)
{
  //
  // KICKERS
  //
  G4Tubs*            Kicker_S;
  G4LogicalVolume*   Kicker_L;
  G4VPhysicalVolume* Kicker_P;
  G4RotationMatrix*  Kicker_Rot;

  // Positioning in ring
  G4double kicker_rot[3];
  kicker_rot[0] =  (electrodeLength+gapLength)/(7.112*m);
  kicker_rot[1] =  0;
  kicker_rot[2] = -kicker_rot[0];

  // Kicker electrode spanning angle
  G4double kicker_dphi = electrodeLength/(7.112*m);

  // Build the kickers
  Kicker_S = new G4Tubs( "Kicker", (7.112-0.1)*m, (7.112+0.1)*m, 7*cm, -kicker_dphi/2., kicker_dphi );
  Kicker_L = new G4LogicalVolume( Kicker_S, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), "Kicker", 0, 0, 0, true );
  for (G4int knum=0; knum!=3; ++knum) {
    Kicker_Rot = new G4RotationMatrix();
    Kicker_Rot->rotateZ(kicker_rot[knum]);
    Kicker_P   = new G4PVPlacement( G4Transform3D(*Kicker_Rot,G4ThreeVector()), Kicker_L, "Kicker", Mother_L, true, knum, true );
  }

#if 0
  G4Torus*           KickerElectrode_S;
  G4LogicalVolume*   KickerElectrode_L;
  G4VPhysicalVolume* KickerElectrode_P;
  KickerElectrode_S = new G4Torus("KickerElectrode", 47*mm, (47*mm+electrodeThickness), 7.112*m, -kicker_dphi/2., kicker_dphi );
  KickerElectrode_L = new G4LogicalVolume( KickerElectrode_S, G4NistManager::Instance()->FindOrBuildMaterial(G4MaterialName), "KickerElectrode", 0, 0, 0, true );
  KickerElectrode_P = new G4PVPlacement( 0, G4ThreeVector(), KickerElectrode_L, "KickerElectrode", Kicker_L, false, 0, true);
#endif
}
