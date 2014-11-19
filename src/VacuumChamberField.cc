#include "VacuumChamberField.hh"
#include "G4ElectroMagneticField.hh"

#include "G4SystemOfUnits.hh"
#include <vector>


#include "G4Field.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_SpinEqRhs.hh"

#include "G4EqEMFieldWithSpin.hh"

//#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"

#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"

#include "G4TransportationManager.hh"


#include <cmath>


VacuumChamberField::VacuumChamberField()
: G4ElectroMagneticField(),
  spinTracking_  (true),
  quadModel_     (  2  ),
  quadFieldIndex_(0.182),
  quadScraping_  (false),
  fieldHelper_(0)
{
  // If the quads are "continuous", we need to rescale the E-field strength
  if (quadModel_==1) quadFieldIndex_*=( 4.*(13.*degree+26.*degree)/(360.*degree) );

  // Define a helper to take care of EM-field calculations
  fieldHelper_ = new FieldHelper(quadModel_, quadScraping_);
}


VacuumChamberField::~VacuumChamberField()
{
  delete fieldHelper_;
}


void VacuumChamberField::GetFieldValue( const G4double Point[4], G4double* Bfield ) const
{
  // Global position
  G4ThreeVector pos(Point[0],Point[1],Point[2]);

  // Calculate the E-field
  G4ThreeVector Efield(0.,0.,0.);
  if (quadModel_==1) {
    // Continuous quads
    G4int qnum = fieldHelper_->WithinQuad(pos);
    if (qnum==4) {
      G4double xlocal = hypot(pos.x(),pos.y()) - 7.112*m;
      G4double ylocal = pos.z();
      // This gives the E-field "at 12 o'clock in global coordinates" so to speak
      Efield = fieldHelper_->GetQuadMultipoleEfield2D( xlocal, ylocal, quadFieldIndex_, qnum );
    }
    // Rotate the E-field from 12 o'clock down (clockwise) to the location of the particle
    G4double uglobal = atan2(pos.x(),pos.y());
    Efield.rotateZ(90.*degree);
    Efield.rotateY(90.*degree);
    Efield.rotateZ(-uglobal);

  } else if (quadModel_==2) {
    // 2D-discrete quads
    G4int qnum = fieldHelper_->WithinQuad(pos);
    if (qnum!=-1) {
      G4double xlocal = hypot(pos.x(),pos.y()) - 7.112*m;
      G4double ylocal = pos.z();
      if (fieldHelper_->quadScraping_) {
        G4double time = Point[4];
        Efield = fieldHelper_->GetQuadMultipoleEfield2DWithScraping( xlocal, ylocal, time, quadFieldIndex_, qnum );
      } else {
        Efield = fieldHelper_->GetQuadMultipoleEfield2D( xlocal, ylocal, quadFieldIndex_, 4 );
      }
    }
    // Convert E-field to global coordinates
    G4double uglobal = atan2(pos.x(),pos.y());
    Efield.rotateZ(90.*degree);
    Efield.rotateY(90.*degree);
    Efield.rotateZ(-uglobal);
  }

  // Assign the EM field
  Bfield[0] = 0.;
  Bfield[1] = 0.;
  Bfield[2] = 1.451299135941164*tesla;
  Bfield[3] = Efield.x();
  Bfield[4] = Efield.y();
  Bfield[5] = Efield.z();

  return;
}


//------------------------------------------------
//------------------------------------------------


FieldHelper::FieldHelper(G4int quadModel, G4bool quadScraping)
: quadModel_(quadModel),
  quadScraping_(quadScraping)
{
  quadShortStartAngle.resize(4);
    quadShortStartAngle[0] = 21.9*degree; 
    quadShortStartAngle[1] = quadShortStartAngle[0] + 90*degree;
    quadShortStartAngle[2] = quadShortStartAngle[1] + 90*degree;
    quadShortStartAngle[3] = quadShortStartAngle[2] + 90*degree;

  quadShortEndAngle.resize(4);
    quadShortEndAngle[0] = quadShortStartAngle[0] + 13*degree;
    quadShortEndAngle[1] = quadShortStartAngle[1] + 13*degree;
    quadShortEndAngle[2] = quadShortStartAngle[2] + 13*degree;
    quadShortEndAngle[3] = quadShortStartAngle[3] + 13*degree;

  quadLongStartAngle.resize(4);
    quadLongStartAngle[0] = quadShortEndAngle[0] + 4*degree;
    quadLongStartAngle[1] = quadShortEndAngle[1] + 4*degree;
    quadLongStartAngle[2] = quadShortEndAngle[2] + 4*degree;
    quadLongStartAngle[3] = quadShortEndAngle[3] + 4*degree;

  quadLongEndAngle.resize(4);
    quadLongEndAngle[0] = quadLongStartAngle[0] + 26*degree;
    quadLongEndAngle[1] = quadLongStartAngle[1] + 26*degree;
    quadLongEndAngle[2] = quadLongStartAngle[2] + 26*degree;
    quadLongEndAngle[3] = quadLongStartAngle[3] + 26*degree;
}


FieldHelper::~FieldHelper()
{}


G4int FieldHelper::WithinQuad(G4ThreeVector pos)
{
  // Calculate the local horizontal position (beam-optical coordinates)
  G4double xlocal = hypot(pos.x(),pos.y()) - 7.112*m;
  if (abs(xlocal)>5*cm) return -1;

  // Calculate the local vertical position (beam-optical coordinates)
  G4double ylocal = pos.z();
  if (abs(ylocal)>5*cm) return -1;

  // If we've made it this far, and the quad model is "continuous", return the qnum for storage multipole expansion
  if (quadModel_==1) return 4;

  // Otherwise, we need to calculate the global angle
  G4double theta = atan2(pos.x(),pos.y()); if (theta<0) theta+=2.*M_PI; // global
  // Loop over the quads
  for (G4int qnum=0; qnum!=4; ++qnum) {
    // Check if the particle is within a short section
    if ( (theta>quadShortStartAngle[qnum]) && (theta<quadShortEndAngle[qnum]) ) return qnum;
    // Check if the particle is within a long section
    if ( (theta>quadLongStartAngle[qnum] ) && (theta<quadLongEndAngle[qnum] ) ) return qnum;
  }

  // Safety
  return -1;
}


G4ThreeVector FieldHelper::GetQuadMultipoleEfield2D(double x, double y, double quadFieldIndex, int qnum)
{
  if ( (abs(x)>5*cm) || (abs(y)>5*cm) ) 
    return G4ThreeVector(0,0,0);

  // Convert from local xy-coordinates to the circle
  double r = hypot(x,y);
  double theta = atan2(y,x);

  // Define some helper variabels
  double r0 = 45.*mm;  // E821 expansion: r0 = 45mm
  double Er =  0.;     // radial component of E-field
  double Et =  0.;     // theta  compontne of E-field

  // Calculate the E-field from the E821 multipole expansion
  for (int n=1; n!=ncoeff; ++n) {
    Er += ( (-n/r0) * pow((r/r0),(n-1)) ) * ( QMC[qnum][0][n]*cos(n*theta) + QMC[qnum][1][n]*sin(n*theta) ); // radial
    Et += ( (-n/r0) * pow((r/r0),(n-1)) ) * (-QMC[qnum][0][n]*sin(n*theta) + QMC[qnum][1][n]*cos(n*theta) ); // theta
  }

  // Rescale the E-field to the desired quadFieldIndex (mu+)
  Er *= (-quadFieldIndex/0.141243)*volt;
  Et *= (-quadFieldIndex/0.141243)*volt;

  // Transform E-field to Cartesian coordinates
  double Ex = cos(theta)*Er - sin(theta)*Et;
  double Ey = sin(theta)*Er + cos(theta)*Et;

  // Return E-field vector
  G4ThreeVector quadMultipoleEfield2D(Ex,Ey,0);
  return quadMultipoleEfield2D;
  // printf("% 12.4f% 12.4f% 12.4e% 12.4e% 12.4f\n",x,y,Ex/volt*m,Ey/volt*m,quadFieldIndex);
}


G4ThreeVector FieldHelper::GetQuadMultipoleEfield2DWithScraping(double x, double y, double t, double quadFieldIndex, int qnum)
{
  G4double tScrapeEnd = 7.00*microsecond;
  G4double tauScrape  = 5.00*microsecond;

  // Calculate the E-field
  G4ThreeVector Efield;
  if (t>tScrapeEnd) {
    // Get the scraping and storage E-fields
    G4ThreeVector Escrape = GetQuadMultipoleEfield2D( x, y, quadFieldIndex, qnum );
    G4ThreeVector Estore  = GetQuadMultipoleEfield2D( x, y, quadFieldIndex, 4 );
    // Interpolate given the RC time structure of scraping
    G4double fracScrape = exp( -(t-tScrapeEnd)/tauScrape );
    Efield = fracScrape*Escrape + (1-fracScrape)*Estore;
  } else {
    // Get the scraping E-field
    Efield = GetQuadMultipoleEfield2D( x, y, quadFieldIndex, qnum );
  }

  return Efield;
  // printf("% 12.4f% 12.4f% 12.4f% 12.4e% 12.4e% 12.4f\n",x,y,t,Efield.x()/volt*m,Efield.y()/volt*m,quadFieldIndex);
}


