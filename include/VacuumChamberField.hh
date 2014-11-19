#ifndef VacuumChamberField_hh
#define VacuumChamberField_hh 1

#include "globals.hh"
#include "G4ElectroMagneticField.hh"
#include "G4ThreeVector.hh"

#include <vector>

class FieldHelper;



class VacuumChamberField : public G4ElectroMagneticField
{
  public:
    VacuumChamberField();
    virtual ~VacuumChamberField();

    virtual G4bool DoesFieldChangeEnergy() const { return true; }
    virtual void GetFieldValue(const G4double Point[4], G4double* Bfield) const;

    G4bool spinTracking_;

  private:
    G4int        quadModel_;
    G4double     quadFieldIndex_;
    G4bool       quadScraping_;
    FieldHelper* fieldHelper_;
};



class FieldHelper
{
  public:
    FieldHelper(G4int,G4bool);
    virtual ~FieldHelper();

    // These functions will be called a lot. Use "inline" just to make sure....
    inline G4int WithinQuad(const G4ThreeVector);
    inline G4ThreeVector GetQuadMultipoleEfield2D(double x, double y, double quadFieldIndex, int qnum);
    inline G4ThreeVector GetQuadMultipoleEfield2DWithScraping(double x, double y, double t, double quadFieldIndex, int qnum);

    G4int  quadModel_;
    G4bool quadScraping_;

  private:
    std::vector<double> quadShortStartAngle, quadShortEndAngle;
    std::vector<double> quadLongStartAngle,  quadLongEndAngle;
};


// Using std::vectors to setup a tensor is a pain, so let's do something simpler instead
const int ncoeff = 15;
const double QMC[7][2][ncoeff] = {
  { // Q1 scraping (Tables 7 & 10).  Counter-clockwise from theta=0 (i.e. 3 o'clock), 
    // the voltages on the quad plates are {+24, -24, +24, -17} kV.
    {0.0, 0.0, 18706.0, 0.0, 375.0, 0.0, -41.0, 0.0, -76.0, 0.0, -364.0, 0.0, -3.0, 0.0, 49.0 }, // cos
    {0.0, -2155.0, 0.0, 837.0, 0.0, -117.0, 0.0, -57.5, 0.0, 52.0, 0.0, -8.0, 0.0, -5.0,  0.0 } // sin
  },

  { // Q2 scraping (Table 8, with the sign of n={1,3,5,..} cosine terms flipped, c.f. Table 10).  Counter-
    // clockwise from theta=0 (i.e. 3 o'clock), the voltages on the quad plates are {+24, -24, +17, -17} kV.
    {0.0,  2150.0, 17235.0, 840.0, 28.0, 121.0, -39.0, 57.0, -5.0, -53.0, -334.0, -9.0, -6.0, 5.0, 45.0 }, // cos
    {0.0, -2156.0, 0.0, 837.0, 0.0, -117.0, 0.0, -57.0, 0.0, 52.0, 0.0, -8.0, 0.0, -5.0, 0.0}, // sin
  },

  { // Q3 scraping (Tables 7 & 10).  Counter-clockwise from theta=0 (i.e. 3 o'clock), 
    // the voltages on the quad plates are {+24, -24, +24, -17} kV.
    {0.0, 0.0, 18706.0, 0.0, 375.0, 0.0, -41.0, 0.0, -76.0, 0.0, -364.0, 0.0, -3.0, 0.0, 49.0 }, // cos
    {0.0, -2155.0, 0.0, 837.0, 0.0, -117.0, 0.0, -57.5, 0.0, 52.0, 0.0, -8.0, 0.0, -5.0,  0.0 } // sin
  },

  { // Q4 scraping (Tables 8 & 10).  Counter-clockwise from theta=0 (i.e. 3 o'clock), 
    // the voltages on the quad plates are {+17, -24, +24, -17} kV.
    {0.0, -2150.0, 17235.0, -840.0, 28.0, -121.0, -39.0, -57.0, -5.0, 53.0, -334.0, 9.0, -6.0, -5.0, 45.0 }, // cos
    {0.0, -2156.0, 0.0, 837.0, 0.0, -117.0, 0.0, -57.0, 0.0, 52.0, 0.0, -8.0, 0.0, -5.0, 0.0 } // sin
  },

  { // Storage (Table 5).  Counter-clockwise from theta=0 (i.e. 3 o'clock), 
    // the voltages on the quad plates are {+24, -24, +24, -24} kV.
    {0.0, 0.0, 20177.8, 0.0, 33.0, 0.0,-45.9, 0.0,-5.5, 0.0,-391.3, 0.0,-6.5, 0.0, 52.3}, // cos
    {0.0, 0.0, 0.1, 0.0, 0.1, 0.0, 0.1, 0.0,-0.2, 0.0, 0.1, 0.0, 0.0, 0.0,-0.1} // sin
  },

  { // Storage, including scallops (Table 5).  Counter-clockwise from theta=0 (i.e. 3 o'clock),
    // the voltages on the quad plates are {+23.7, -24.2, +23.9, -24.1} kV.
    {0.0, -12.0, 20177.8, -28.0, -153.0, -10.0, -26.0, 0.0, -5.5, 0.0, -391.3, 0.0, 18.0, 0.0, 52.3}, // cos
    {0.0, 0.0, 0.1, 0.0, 0.1, -8.0, 0.1, 0.0,-0.2, 0.0, 0.1, 0.0, 0.0, 0.0, -0.1} // sin
  },

  { // "Worst possible" (Table 6).  Counter-clockwise from theta=0 (i.e. 3 o'clock), 
    // the voltages on the quad plates are {+23.8, -23.4, +23.0, -24.1} kV.
    {0.0, 405.0, 19875.0, 173.0, -190.0, -10.0, -35.0, -50.0, 20.0, -50.0, -391.3, -15.0, 20.0, 4.0, 50.0}, // cos
    {0.0, 345.0, -75.0, -120.0, 20.0, -8.0, 30.0, 35.0, 10.0, -30.0, 0.0, 10.0, 4.0, 2.0, -2.0} // sin
  }
};


#endif
