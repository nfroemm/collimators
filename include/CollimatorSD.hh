#ifndef CollimatorSD_h
#define CollimatorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "CollimatorHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non-zero 
/// energy deposit.

class CollimatorSD : public G4VSensitiveDetector
{
  public:
    CollimatorSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~CollimatorSD();
  
    // Methods from base class "G4VSensitiveDetector"
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

    void CreateHit(const G4Step*);
    void AddHitExitInfo(const G4Step*);

  private:
    CollimatorHitsCollection* fHitsCollection;
};

#endif
