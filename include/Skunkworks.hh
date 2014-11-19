#ifndef Skunkworks_hh
#define Skunkworks_hh 1

#include "globals.hh"
#include "G4LogicalVolume.hh"


class Skunkworks
{
  public:
    Skunkworks();
    virtual ~Skunkworks();

  private:
    G4String G4MaterialName;

  public:
    void BuildSkunkworks(G4LogicalVolume* Mother_L);
};

#endif
