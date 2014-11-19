#ifndef QuadConstruction_hh
#define QuadConstruction_hh 1

#include "globals.hh"

class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;

class QuadConstruction
{
  public:
    QuadConstruction();
    virtual ~QuadConstruction();

  public:
    G4String G4MaterialName;
    G4Tubs*            QuadShort_S;
    G4LogicalVolume*   QuadShort_L;
    G4VPhysicalVolume* QuadShort_P;
    G4Tubs*            QuadLong_S;
    G4LogicalVolume*   QuadLong_L;
    G4VPhysicalVolume* QuadLong_P;

  public: 
    void BuildQuads(G4LogicalVolume* Mother_L);
};

#endif
