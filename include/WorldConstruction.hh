#ifndef WorldConstruction_h
#define WorldConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Box;

class VacuumChamberConstruction;
class CollimatorConstruction;


class WorldConstruction : public G4VUserDetectorConstruction
{
  public:
    WorldConstruction();
    virtual ~WorldConstruction();

  public: 
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private: 
    G4Box*             World_S;
    G4LogicalVolume*   World_L;
    G4VPhysicalVolume* World_P;
    VacuumChamberConstruction* vacCmbr_;
    CollimatorConstruction*    collimators_;
};

#endif
