#include "Skunkworks.hh"
#include "WorldConstruction.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

//#include "G4PhysicalVolume.hh"


Skunkworks::Skunkworks()
 : G4MaterialName("G4_Galactic")
{}


Skunkworks::~Skunkworks()
{}


void Skunkworks::BuildSkunkworks(G4LogicalVolume* Mother_L)
{
  G4double hx;
  G4double hy;
  G4double hz;
  G4Box*             Skunkworks_S;
  G4LogicalVolume*   Skunkworks_L;
  G4VPhysicalVolume* Skunkworks_P;

  hx = 0.5*cm;
  hy = 0.5*m;
  hz = 0.5*m;
  Skunkworks_S = new G4Box("Skunkworks", hx, hy, hz);
  Skunkworks_L = new G4LogicalVolume(Skunkworks_S, G4NistManager::Instance()->FindOrBuildMaterial(G4MaterialName), "Skunkworks", 0, 0, 0, true);
  Skunkworks_P = new G4PVPlacement(0, G4ThreeVector(-5*m,7.112*m,0), Skunkworks_L, "Skunkworks", Mother_L, false, 0, true);
}


#if 0
  // Source: http://en.wikipedia.org/wiki/Web_colors
  G4Colour color;
  G4ThreeVector rgb;

  rgb = G4ThreeVector(139,69,19);
  G4Colour::AddToMap("SaddleBrown",rgb/255); // copper

  rgb = G4ThreeVector(245,245,245);
  G4Colour::AddToMap("WhiteSmoke",rgb/255);

  // Gray/Black colors (good for metals)
  rgb = G4ThreeVector(220,220,220);
  G4Colour::AddToMap("Gainsboro",rgb/255);
  //
  rgb = G4ThreeVector(211,211,211);
  G4Colour::AddToMap("LightGray",rgb/255);
  //
  rgb = G4ThreeVector(192,192,192);
  G4Colour::AddToMap("Silver",rgb/255);
  //
  rgb = G4ThreeVector(169,169,169);
  G4Colour::AddToMap("DarkGray",rgb/255);
  //
  rgb = G4ThreeVector(128,128,128);
  G4Colour::AddToMap("Gray",rgb/255);
  //
  rgb = G4ThreeVector(105,105,105);
  G4Colour::AddToMap("DimGray",rgb/255);
  //
  rgb = G4ThreeVector(119,136,153);
  G4Colour::AddToMap("LightSlateGray",rgb/255);
  //
  rgb = G4ThreeVector(112,128,144);
  G4Colour::AddToMap("SlateGray",rgb/255);
  //
  rgb = G4ThreeVector(47,79,79);
  G4Colour::AddToMap("DarkSlateGray",rgb/255);
  //
  rgb = G4ThreeVector(0,0,0);
  G4Colour::AddToMap("Black",rgb/255);
#endif


#if 0
  //
  // MATERIALS
  //
  G4Material*     Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4Material*     Cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4Material*     Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
#endif


#if 0
  //
  // BACKLEG HOLE
  //
  G4Polycone *BacklegHole_S;
  G4ThreeVector *BacklegHole_Pos;
  G4RotationMatrix *BacklegHole_Rot;
  //
  G4double BacklegHole_z[] = {-1*m, 0*m, 0.001*mm, 2*m };
  G4double BacklegHole_rInner[] = { 0, 0, 0, 0 };
  G4double BacklegHole_rOuter[] = { (4.00/2)*2.54*cm, (4.00/2)*2.54*cm, (5.25/2)*2.54*cm, (5.25/2)*2.54*cm };
  BacklegHole_S = new G4Polycone("BacklegHole", 0*degree, 360*degree, 4, BacklegHole_z, BacklegHole_rInner, BacklegHole_rOuter);
  BacklegHole_Pos = new G4ThreeVector;
  BacklegHole_Rot = new G4RotationMatrix;
  BacklegHole_Pos->set( -3.26798*m, 7.16867*m, 0);
  BacklegHole_Rot->rotateY( 90*degree );
  BacklegHole_Rot->rotateZ( 1.25*degree );
  //
  // SPACER PLATE (Source: E821 bluprints "d19-m-3184-4", "d19-m-3195-5")
  //
  G4Tubs* SpacerPlate_S;
  SpacerPlate_S = new G4Tubs("SpacerPlate", 302.44*2.54*cm, 323.84*2.54*cm, (24.5/1)*cm, 0*degree, 360*degree);
  //
  // UPPER SPACER PLATE (WITH BACKLEG HOLE)
  //
  G4SubtractionSolid *UpperSpacerPlateWithHole_S;
  G4LogicalVolume    *UpperSpacerPlateWithHole_L;
  G4VPhysicalVolume  *UpperSpacerPlateWithHole_P;
  G4RotationMatrix   *UpperSpacerPlate_Rot;
  G4ThreeVector      *UpperSpacerPlate_Pos;
  //
  UpperSpacerPlate_Rot = new G4RotationMatrix;
  UpperSpacerPlate_Pos = new G4ThreeVector( 0.0*m, 0.0*m, SpacerPlate_S->GetZHalfLength() );
  UpperSpacerPlateWithHole_S = new G4SubtractionSolid("UpperSpacerPlateWithHole", SpacerPlate_S, BacklegHole_S, G4Transform3D(*BacklegHole_Rot,G4ThreeVector(BacklegHole_Pos->x(),BacklegHole_Pos->y(),-UpperSpacerPlate_Pos->z())) );
  UpperSpacerPlateWithHole_L = new G4LogicalVolume( UpperSpacerPlateWithHole_S, Fe, "UpperSpacerPlateWithHole", 0, 0, 0, true );
  UpperSpacerPlateWithHole_P = new G4PVPlacement( G4Transform3D(*UpperSpacerPlate_Rot,*UpperSpacerPlate_Pos), UpperSpacerPlateWithHole_L, "UpperSpacerPlateWithHole", World_L, false, 0, true);
  //
  // LOWER SPACER PLATE (WITH BACKLEG HOLE)
  //
  G4SubtractionSolid *LowerSpacerPlateWithHole_S;
  G4LogicalVolume    *LowerSpacerPlateWithHole_L;
  G4VPhysicalVolume  *LowerSpacerPlateWithHole_P;
  G4RotationMatrix   *LowerSpacerPlate_Rot;
  G4ThreeVector      *LowerSpacerPlate_Pos;
  //
  LowerSpacerPlate_Rot = new G4RotationMatrix;
  LowerSpacerPlate_Pos = new G4ThreeVector( 0.0*m, 0.0*m,-SpacerPlate_S->GetZHalfLength() );
  LowerSpacerPlateWithHole_S = new G4SubtractionSolid("LowerSpacerPlateWithHole", SpacerPlate_S, BacklegHole_S, G4Transform3D(*BacklegHole_Rot,G4ThreeVector(BacklegHole_Pos->x(),BacklegHole_Pos->y(),-LowerSpacerPlate_Pos->z())) );
  LowerSpacerPlateWithHole_L = new G4LogicalVolume( LowerSpacerPlateWithHole_S, Fe, "LowerSpacerPlateWithHole", 0, 0, 0, true );
  LowerSpacerPlateWithHole_P = new G4PVPlacement( G4Transform3D(*LowerSpacerPlate_Rot,*LowerSpacerPlate_Pos), LowerSpacerPlateWithHole_L, "LowerSpacerPlateWithHole", World_L, false, 0, true);
  //
  G4VisAttributes *SpacerPlate_VisAtt = new G4VisAttributes;
  G4Color::GetColour("LightSlateGray",color);
  SpacerPlate_VisAtt->SetColor(color);
  SpacerPlate_VisAtt->SetForceSolid(true);
  SpacerPlate_VisAtt->SetVisibility(true);
  LowerSpacerPlateWithHole_L->SetVisAttributes(SpacerPlate_VisAtt);
//UpperSpacerPlateWithHole_L->SetVisAttributes(SpacerPlate_VisAtt);



  //
  // OUTER COIL
  //
  G4Tubs *OuterCoil_S;
  OuterCoil_S = new G4Tubs("OuterCoil", (711.2+28+1.53)*cm, (711.2+57-0.64)*cm, (24.5-0.44)*cm, SpacerPlate_S->GetStartPhiAngle(), SpacerPlate_S->GetDeltaPhiAngle() );
  //
  // OUTER COIL HOLE
  //
  G4Polycone *OuterCoilHole_S;
  G4double OuterCoilHole_z[] = { 4*2.54*cm, 1.40094*m, 1.62938*m, 2.0*m }; // The first measurement comes from the pictures I took at MC1 (29 Aug 2014), the rest is geometry
  G4double OuterCoilHole_rInner[] = { 0, 0, 0, 0 };
  G4double OuterCoilHole_rOuter[] = { (4.00/2)*2.54*cm, (4.00/2)*2.54*cm, (2.625-0.25)*2.54*cm, (2.625-0.25)*2.54*cm };
  OuterCoilHole_S = new G4Polycone( "OuterCoilHole", 0*degree, 360*degree, 4, OuterCoilHole_z, OuterCoilHole_rInner, OuterCoilHole_rOuter );
  //
  // OUTER COIL BEAM PIPE
  //
  G4Polycone *OuterCoilBeamPipe_S;
  G4double OuterCoilBeamPipe_z[] = { 4*2.54*cm, 1.250*m, 1.251*m };
  G4double OuterCoilBeamPipe_rInner[] = { (4.00/2)*2.54*cm, (4.00/2)*2.54*cm, (4.00/2)*2.54*cm }; // 4-inch
  G4double OuterCoilBeamPipe_rOuter[] = { (4.00/2+0.375)*2.54*cm, (4.00/2+0.375)*2.54*cm, (4.00/2+0.375)*2.54*cm }; // 4-inch + 3/8-inch
  OuterCoilBeamPipe_S = new G4Polycone( "OuterCoilBeamPipe", 0*degree, 360*degree, 3, OuterCoilBeamPipe_z, OuterCoilBeamPipe_rInner, OuterCoilBeamPipe_rOuter );
  //
  // SUBTRACT HOLE FROM OUTER COIL
  //
  G4SubtractionSolid *OuterCoilWithHole_S = new G4SubtractionSolid( "OuterCoilWithHole", OuterCoil_S, OuterCoilHole_S, G4Transform3D(*BacklegHole_Rot,*BacklegHole_Pos) );
  //
  // UNION THE BEAM PIPE AND THE OUTER COIL (WITH SUBTRACTED HOLE)
  //
  G4UnionSolid       *OuterCoilWithBeamPipe_S;
  G4LogicalVolume    *OuterCoilWithBeamPipe_L;
  G4VPhysicalVolume  *OuterCoilWithBeamPipe_P;
  OuterCoilWithBeamPipe_S = new G4UnionSolid( "OuterCoilWithBeamPipe", OuterCoilWithHole_S, OuterCoilBeamPipe_S, G4Transform3D(*BacklegHole_Rot,*BacklegHole_Pos) );
  OuterCoilWithBeamPipe_L = new G4LogicalVolume( OuterCoilWithBeamPipe_S, Al, "OuterCoilWithBeamPipe", 0, 0, 0, true );
  OuterCoilWithBeamPipe_P = new G4PVPlacement( 0, G4ThreeVector(), OuterCoilWithBeamPipe_L, "OuterCoilWithBeamPipe", World_L, false, 0, true);

  G4VisAttributes *RingCoil_VisAtt = new G4VisAttributes;
  G4Color::GetColour("WhiteSmoke",color);
  RingCoil_VisAtt->SetColor(color);
  RingCoil_VisAtt->SetForceSolid(true);
  RingCoil_VisAtt->SetVisibility(true);
  OuterCoilWithBeamPipe_L->SetVisAttributes(RingCoil_VisAtt);


  //
  // INNER COILS
  //
  G4Tubs *InnerCoilUpper_S;
  G4LogicalVolume *InnerCoilUpper_L;
  G4VPhysicalVolume *InnerCoilUpper_P;
  InnerCoilUpper_S = new G4Tubs( "InnerCoil", (652.2)*cm, (683.2-0.58)*cm, 15.85*cm, SpacerPlate_S->GetStartPhiAngle(), SpacerPlate_S->GetDeltaPhiAngle() );
  InnerCoilUpper_L = new G4LogicalVolume( InnerCoilUpper_S, Al, "InnerCoil", 0, 0, 0, true );
  InnerCoilUpper_P = new G4PVPlacement( G4Transform3D(*UpperSpacerPlate_Rot,G4ThreeVector(0,0, 40.35*cm)), InnerCoilUpper_L, "InnerCoilUpper", World_L, false, 0, true );

  G4Tubs *InnerCoilLower_S;
  G4LogicalVolume *InnerCoilLower_L;
  G4VPhysicalVolume *InnerCoilLower_P;
  InnerCoilLower_S = new G4Tubs( "InnerCoil", (652.2)*cm, (683.2-0.58)*cm, 15.85*cm, SpacerPlate_S->GetStartPhiAngle(), SpacerPlate_S->GetDeltaPhiAngle() );
  InnerCoilLower_L = new G4LogicalVolume( InnerCoilLower_S, Al, "InnerCoil", 0, 0, 0, true );
  InnerCoilLower_P = new G4PVPlacement( G4Transform3D(*LowerSpacerPlate_Rot,G4ThreeVector(0,0,-40.35*cm)), InnerCoilLower_L, "InnerCoilLower", World_L, false, 0, true );
  InnerCoilLower_L->SetVisAttributes(RingCoil_VisAtt);

#endif

