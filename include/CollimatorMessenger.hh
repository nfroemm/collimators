#ifndef CollimatorMessenger_h
#define CollimatorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class CollimatorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;


class CollimatorMessenger: public G4UImessenger
{
  public:
    CollimatorMessenger( CollimatorConstruction* );
    virtual ~CollimatorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    CollimatorConstruction*    collConstruction_;
    G4UIdirectory*             collDir_;
    G4UIcmdWithAString*        collMaterialCmd_;
    G4UIcmdWithADoubleAndUnit* collThicknessCmd_;
};

#endif
