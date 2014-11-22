#ifndef CollimatorConstruction_hh
#define CollimatorConstruction_hh 1

#include "globals.hh"
#include <vector>

class G4LogicalVolume;
class CollimatorMessenger;
class G4Material;


class CollimatorConstruction
{
  public:
    CollimatorConstruction();
    virtual ~CollimatorConstruction();

  public:
    static CollimatorConstruction* Instance();
    void BuildCollimators( G4LogicalVolume* Mother_L );

    void SetMaterial( G4String );
    void SetThickness( G4double );

  public:
    G4String             G4MaterialName;
    G4double             rmin;
    G4double             rmax;
    G4double             dz;
    G4LogicalVolume*     Collimator_L;
    std::vector<double>  collRingAngle;
    G4Material*          collMaterial_;
    G4bool               constructed_;
    CollimatorMessenger* collMsgr_;

  private:
    static CollimatorConstruction* instance;
    enum collimatorTypes { NONE, FULL, HALF_OUTER, HALF_INNER } collType[8];
};

#endif
