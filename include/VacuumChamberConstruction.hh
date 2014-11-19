#ifndef VacuumChamberConstruction_hh
#define VacuumChamberConstruction_hh 1

#include "globals.hh"

class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;


class VacuumChamberConstruction
{
  public: 
    VacuumChamberConstruction();
    virtual ~VacuumChamberConstruction();

  public:
    void BuildVacCmbr( G4LogicalVolume* Mother_L );

  public:
    G4double hx;
    G4double hy;
    G4Tubs*            RingVacuum_S;
    G4LogicalVolume*   RingVacuum_L;
    G4VPhysicalVolume* RingVacuum_P;
};

#endif
