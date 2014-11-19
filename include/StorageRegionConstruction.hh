#ifndef StorageRegionConstruction_h
#define StorageRegionConstruction_h 1

#include "G4VUserParallelWorld.hh"

class G4Torus;
class G4LogicalVolume;
class G4VPhysicalVolume;


class StorageRegionConstruction : public G4VUserParallelWorld
{
  public:
    StorageRegionConstruction(G4String& parallelWorldName);
    virtual ~StorageRegionConstruction();

  public:
    virtual void Construct();

  private:
    G4bool BuildStorageRegionTorus;
    G4double Rmin;
    G4double Rmax;
    G4double Rtor;
    G4double SPhi;
    G4double DPhi;
    G4Torus*           StorageRegionTorus_S;
    G4LogicalVolume*   StorageRegionTorus_L;
    G4VPhysicalVolume* StorageRegionTorus_P;
};

#endif
