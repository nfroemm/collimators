#ifndef KickerConstruction_hh
#define KickerConstruction_hh 1

#include "globals.hh"
#include "G4LogicalVolume.hh"

class KickerConstruction
{
  public:
    KickerConstruction();
    virtual ~KickerConstruction();

  private:
    G4double electrodeLength;
    G4double gapLength;
    G4double electrodeThickness;
    G4String G4MaterialName;

  public: 
    void BuildKickers(G4LogicalVolume* Mother_L);
};

#endif
