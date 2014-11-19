#include "QuadField.hh"

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"

#include <vector>
#include <cmath>
using namespace std;


QuadField::QuadField() 
: G4ElectroMagneticField(),
  quad_field_index_(0.182),
  doScraping_(true)
{}


QuadField::~QuadField()
{}


void QuadField::GetFieldValue( const G4double Point[4], G4double* Bfield ) const
{
  // Convert from global to local coordinates
  G4double x = hypot(Point[0],Point[1]) - 7.112*m;
  G4double y = Point[2];
/*

  // Calculate the Efield
  G4ThreeVector Efield;
  if (doScraping_) {
    G4double t = Point[3];
    Efield = GetQuadMultipoleEfield2DWithScraping(x, y, t, quad_field_index_, 4);
  } else {
    Efield = GetQuadMultipoleEfield2D(x, y, quad_field_index_, 4);
  }

  // Convert local E-field back to global coordinates
  G4double theta = atan2(Point[0],Point[1]);
  Efield.rotateZ(90*degree);
  Efield.rotateY(90*degree);
  Efield.rotateZ(-theta);

  // Assign the EM field
  Bfield[0] = 0.;
  Bfield[1] = 0.;
  Bfield[2] = 1.4513*tesla;
  Bfield[3] = Efield.x();
  Bfield[4] = Efield.y();
  Bfield[5] = Efield.z();
*/
  Bfield[2] = 1.4513*tesla;
  return;
}


#if 0
G4ThreeVector GetQuadMultipoleEfield2D(
  double x, double y, double quad_field_index=0.182/*E989*/, int qnum=4/*storage*/
){
  if ( (abs(x)>5*cm) || (abs(y)>5*cm) ) {
    // TODO: Get the E-field from the map
    return G4ThreeVector();
  }

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

  // Rescale the E-field to the desired quad_field_index (mu+)
  Er *= (-quad_field_index/0.141243)*volt;
  Et *= (-quad_field_index/0.141243)*volt;

  // Transform E-field to Cartesian coordinates
  double Ex = cos(theta)*Er - sin(theta)*Et;
  double Ey = sin(theta)*Er + cos(theta)*Et;

  // printf("% 12.4f% 12.4f% 12.4e% 12.4e% 12.4f\n",x,y,Ex/volt*m,Ey/volt*m,quad_field_index);

  // Return E-field vector
  G4ThreeVector quadMultipoleEfield2D(Ex,Ey,0);
  return quadMultipoleEfield2D;
}


G4ThreeVector GetQuadMultipoleEfield2DWithScraping(
  double x, double y, double t, double quad_field_index=0.182/*E989*/, int qnum=4/*storage*/
){
  G4double tScrapeEnd = 7.00*microsecond;
  G4double tauScrape  = 4.75*microsecond;


  G4ThreeVector Efield;
  if (t>tScrapeEnd) {
    // Get the scraping and storage E-fields
    G4ThreeVector Escrape = GetQuadMultipoleEfield2D( x, y, quad_field_index, qnum );
    G4ThreeVector Estore  = GetQuadMultipoleEfield2D( x, y, quad_field_index, 4 );
    // Interpolate given the RC time structure of scraping
    G4double fracScrape = exp( -(t-tScrapeEnd)/tauScrape );
    Efield = fracScrape*Escrape + (1-fracScrape)*Estore;
  } else {
    // Get the scraping E-field
    Efield = GetQuadMultipoleEfield2D( x, y, quad_field_index, qnum );
  }

//G4double Ex = fracScrape*Escrape.x() + (1-fracScrape)*Estore.x();
//G4double Ey = fracScrape*Escrape.y() + (1-fracScrape)*Estore.y();
//G4ThreeVector Efield(Ex,Ey,0);

//printf("% 12.4f% 12.4f% 12.4f% 12.4e% 12.4e% 12.4f\n",x,y,t,Efield.x()/volt*m,Efield.y()/volt*m,quad_field_index);

  return Efield;
}
#endif

