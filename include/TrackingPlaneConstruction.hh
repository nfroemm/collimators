#ifndef TrackingPlaneConstruction_h
#define TrackingPlaneConstruction_h 1

#include "G4VUserParallelWorld.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;


class TrackingPlaneConstruction : public G4VUserParallelWorld
{
  public:
    TrackingPlaneConstruction(G4String& parallelWorldName);
    virtual ~TrackingPlaneConstruction();

  public:
    virtual void Construct();

  private:
    G4bool BuildTrackingPlanes;
    G4int numTrackingPlanes;
    G4double  hx;
    G4double  hy;
    G4double  hz;
    G4Box*              TrackingPlane_S;
    G4LogicalVolume*    TrackingPlane_L;
    G4VPhysicalVolume*  TrackingPlane_P;
};

#endif
