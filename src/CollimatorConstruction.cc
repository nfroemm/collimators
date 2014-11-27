#include "CollimatorConstruction.hh"
#include "CollimatorSD.hh"
#include "CollimatorMessenger.hh"

#include "globals.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include <vector>


CollimatorConstruction* CollimatorConstruction::instance=0;
CollimatorConstruction* CollimatorConstruction::Instance()
{
  if (instance==0) {
    static CollimatorConstruction CollimatorConstruction;
    instance = &CollimatorConstruction;
  }
  return instance;
}


CollimatorConstruction::CollimatorConstruction() :

//G4MaterialName("G4_GRAPHITE"),
//dz(10.*mm),

//G4MaterialName("G4_Al"),
//dz( 9.*mm),

//G4MaterialName("G4_Cu"),
//dz( 3.*mm),

//G4MaterialName("G4_GRAPHITE"),
//dz(20.000*mm),

  G4MaterialName("G4_Al"),
  dz( 9.210*mm),

//G4MaterialName("G4_Cu"),
//dz( 1.487*mm),

  rmin(45.*mm),
  rmax(48.*mm),
  Collimator_L(0),
  collRingAngle(0),
  collMaterial_(0),
  constructed_(false),
  collMsgr_(0)
{
  collMsgr_ = new CollimatorMessenger(this);

  G4int expNum = 1000;
  if (expNum==989) {
    collType[0] = NONE;
    collType[1] = FULL;
    collType[2] = FULL;
    collType[3] = NONE;
    collType[4] = FULL; collType[5] = FULL;
    collType[6] = FULL;
    collType[7] = NONE;
  } else if (expNum==821) {
    // E821
    G4MaterialName = "G4_Cu";
    rmin = 45*mm;
    rmax = 55*mm;
    dz   =  3*mm;
    collType[0] = FULL;
    collType[1] = HALF_OUTER;
    collType[2] = HALF_OUTER;
    collType[3] = HALF_OUTER;
    collType[4] = HALF_OUTER;
    collType[5] = FULL;
    collType[6] = FULL;
    collType[7] = HALF_INNER;
  } else {
    // Custom
    collType[0] = NONE;
    collType[1] = FULL;
    collType[2] = NONE;
    collType[3] = NONE;
    collType[4] = NONE;
    collType[5] = NONE;
    collType[6] = NONE;
    collType[7] = NONE;
  }

}


CollimatorConstruction::~CollimatorConstruction()
{}


void CollimatorConstruction::BuildCollimators(G4LogicalVolume* Mother_L)
{
  //
  // COLLIMATORS
  //
  G4Tubs*            Collimator_S;
  G4VPhysicalVolume* Collimator_P;
  G4RotationMatrix*  Collimator_Rot;
  G4ThreeVector*     Collimator_Pos;
  G4VisAttributes*   Collimator_VisAtt;

  // Collimator locations in ring (degrees downstream from 12 o'clock).
  // NB: Some of these won't be used in E989 (hence the "<>_temp").
  G4double collRingAngle_temp[] = { 6, 126, 156, 186, 216, 246, 276, 306 };

  // Loop over the collimators
  for (G4int ncol=0; ncol!=8; ++ncol) 
  {
    // Some of the collimators are removed in E989
    if (collType[ncol]==NONE) continue;

    // The collimators' construction depends on the type of collimator
    G4double sphi = 0;
    G4double dphi = 0;
    if (collType[ncol]==FULL) {
      sphi =   0*degree;
      dphi = 360*degree;
    } else if (collType[ncol]==HALF_OUTER) {
      sphi = -90*degree;
      dphi = 180*degree;
    } else if (collType[ncol]==HALF_INNER) {
      sphi =  90*degree;
      dphi = 180*degree;
    }

    // Set the default material
    collMaterial_ = G4NistManager::Instance()->FindOrBuildMaterial( G4MaterialName );

    // Build this collimator
    Collimator_S   = new G4Tubs("Collimator", rmin, rmax, dz/2., sphi, dphi);
    Collimator_L   = new G4LogicalVolume( Collimator_S, collMaterial_, "Collimator", 0, 0, 0, true );
    Collimator_Rot = new G4RotationMatrix;
    Collimator_Pos = new G4ThreeVector(0,7.112*m,0);
    Collimator_Rot->rotateZ(90*degree);
    Collimator_Rot->rotateY(90*degree);
    Collimator_Rot->rotateZ(-collRingAngle_temp[ncol]*degree);
    Collimator_Pos->rotateZ(-collRingAngle_temp[ncol]*degree);
    Collimator_P   = new G4PVPlacement(G4Transform3D(*Collimator_Rot,*Collimator_Pos), Collimator_L, "Collimator", Mother_L, false, ncol, true);

    // Keep track of the constructed collimator angles (for CollimatorGun)
    collRingAngle.push_back( collRingAngle_temp[ncol] );
  }

  // Set visualization attributes
  Collimator_VisAtt = new G4VisAttributes;
  Collimator_VisAtt->SetForceSolid(true);
  Collimator_VisAtt->SetVisibility(true);
  G4ThreeVector rgb;
  // Pretty colors
  if (Collimator_L->GetMaterial()->GetName()=="G4_GRAPHITE") {
    rgb=G4ThreeVector(100,100,100); // graphite
  } else if (Collimator_L->GetMaterial()->GetName()=="G4_Al") {
    rgb=G4ThreeVector(119,136,153); // aluminum
  } else { 
    rgb=G4ThreeVector(139, 69, 19); // copper
  }
  Collimator_VisAtt->SetColor( rgb/255. );
  Collimator_L->SetVisAttributes(Collimator_VisAtt);

  // The collimators are now constructed
  constructed_ = true;
}


void CollimatorConstruction::SetMaterial( G4String materialName )
{
  // Get a pointer to the new material
  G4Material* newMaterial = G4NistManager::Instance()->FindOrBuildMaterial( materialName );

  // Check that the requested new material is actually a valid material
  if (newMaterial) {
    // Check that the new material isn't equal to the old material
    if (collMaterial_!=newMaterial) {
      // Set the collimator material to the new material
      collMaterial_ = newMaterial;
      if (Collimator_L) Collimator_L->SetMaterial( collMaterial_ );
      G4cout << "Collimator material is now " << Collimator_L->GetMaterial()->GetName() << G4endl;
    } else {
      // Otherwise, the user is being silly
      G4cout << "Collimator material already set to " << materialName << "!\n";
    }
  } else {
    // Otherwise, the user is smokin' crack
    G4cout << "WARNING from CollimatorConstruction::SetMaterial()\n";
    G4cout << "Material " << materialName << " not found!\n";
  }
}


void CollimatorConstruction::SetThickness(G4double thickness)
{
  G4double max_thickness = 2.*cm;
  if (thickness>max_thickness) {
    G4cout << "ERROR: Collimator thickness can be at most " << max_thickness/cm << " cm" << G4endl;
    throw;
  }
#if 1
  G4Tubs* collLV = NULL;
  if (Collimator_L) collLV=dynamic_cast<G4Tubs*>(Collimator_L->GetSolid());
  G4cout << "Present Thickness: " << 2.*collLV->GetZHalfLength()/mm << " mm" << G4endl;
  collLV->SetZHalfLength(thickness/2.);
  G4cout << "  NEW   Thickness: " << 2.*collLV->GetZHalfLength()/mm << " mm" << G4endl;
#endif
}
