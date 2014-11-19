#include"PhaseSpace.hh"

#include"globals.hh"
#include"G4SystemOfUnits.hh"
#include"Randomize.hh"

#include <iostream>
using namespace std;

#include <Eigen/Dense>
using namespace Eigen;


double ran_fnalw (double center, double full_width)
{
  // The data below are the (normalized) integral, or "cumulative distri-
  // bution function" (CDF), of the so-called "Fermilab W" shown in CDR 
  // Fig. 7.5, lower-left pane.  These data are used to generate the 
  // longitudinal beam profile if the user has selected "t_distr = 989".
  static const int nbins = 100;
  static const double fnalw_cdf[nbins] = {
  0.000000000, 0.000825196, 0.004630266, 0.009031312, 0.018887819,\
  0.030623940, 0.043643699, 0.056617613, 0.068124513, 0.078118553,\
  0.085866227, 0.092697016, 0.099023518, 0.105029111, 0.110897171,\
  0.116627699, 0.122266538, 0.127767845, 0.133177463, 0.138403704,\
  0.143446569, 0.148260212, 0.152936322, 0.157474900, 0.161875946,\
  0.166231147, 0.170632192, 0.175124926, 0.179709348, 0.184431302,\
  0.189244946, 0.194150277, 0.199376519, 0.205382112, 0.212212901,\
  0.220877458, 0.231375785, 0.243295283, 0.256315041, 0.271260258,\
  0.287580800, 0.305551735, 0.324852152, 0.346032183, 0.368358318,\
  0.391463806, 0.415256957, 0.439325173, 0.463530922, 0.487828359,\
  0.512171641, 0.536469078, 0.560674827, 0.584743043, 0.608536194,\
  0.631641682, 0.653967817, 0.675147848, 0.694448265, 0.712419200,\
  0.728739742, 0.743684959, 0.756704717, 0.768624215, 0.779122542,\
  0.787787099, 0.794617888, 0.800623481, 0.805849723, 0.810755054,\
  0.815568698, 0.820290652, 0.824875074, 0.829367808, 0.833768853,\
  0.838124054, 0.842525100, 0.847063678, 0.851739788, 0.856553431,\
  0.861596296, 0.866822537, 0.872232155, 0.877733462, 0.883372301,\
  0.889102829, 0.894970889, 0.900976482, 0.907302984, 0.914133773,\
  0.921881447, 0.931875487, 0.943382387, 0.956356301, 0.969376060,\
  0.981112181, 0.990968688, 0.995369734, 0.999174804, 1.000000000};

  // Generate a random number
  double rand = G4RandFlat::shoot();
  // Find the random number in the above integral table
  for (int i=0; i!=(nbins-1); ++i) {
    if ( (fnalw_cdf[i]<rand) && (rand<fnalw_cdf[i+1]) ) {
      // Map the random number to the user's inputs
      double d_cdf = fnalw_cdf[i+1] - fnalw_cdf[i];
      double frac_high = ( fnalw_cdf[i+1]-rand )/d_cdf;
      double frac_low  = ( rand-fnalw_cdf[ i ] )/d_cdf;
      double ix_interp = frac_low*i + frac_high*(i+1);
      double fnalw = center + ( ix_interp/nbins - 0.5 )*full_width;
      return fnalw;
    }
  }
  return 0.;
}


double ran_g4bl_dpp()
{
  // The data below are the (normalized) integral, or "cumulative distri-
  // bution function" (CDF), of muon dp/p from V. Tishchenko's g4beamline
  // simulation (16 Apr 2014).  The g4beamline results only had 10,613 mu+ 
  // total, so the distribution is a little "choppy".
  static const int nbins = 200;
  static const double g4bl_dpp_cdf[nbins] = {
  // Source: http://gm2-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=1811 (accessed 16 Apr 2014)
  0.0000942240,  0.0000942240,  0.0000942240,  0.0000942240,  0.0000942240,\
  0.0001507590,  0.0002072930,  0.0002638270,  0.0003203620,  0.0003768960,\
  0.0006784130,  0.0009799300,  0.0012814470,  0.0015829640,  0.0018844810,\
  0.0026948080,  0.0035051350,  0.0043154620,  0.0051257890,  0.0059361160,\
  0.0078394420,  0.0097427680,  0.0116460940,  0.0135494210,  0.0154527470,\
  0.0181852450,  0.0209177420,  0.0236502400,  0.0263827380,  0.0291152360,\
  0.0328841990,  0.0366531610,  0.0404221240,  0.0441910860,  0.0479600490,\
  0.0528597000,  0.0577593520,  0.0626590030,  0.0675586540,  0.0724583060,\
  0.0783190430,  0.0841797800,  0.0900405160,  0.0959012530,  0.1017619900,\
  0.1079053990,  0.1140488080,  0.1201922170,  0.1263356260,  0.1324790350,\
  0.1393197020,  0.1461603690,  0.1530010360,  0.1598417040,  0.1666823710,\
  0.1745971920,  0.1825120140,  0.1904268350,  0.1983416560,  0.2062564780,\
  0.2140770750,  0.2218976730,  0.2297182700,  0.2375388670,  0.2453594650,\
  0.2549326300,  0.2645057950,  0.2740789600,  0.2836521250,  0.2932252900,\
  0.3022708000,  0.3113163100,  0.3203618200,  0.3294073310,  0.3384528410,\
  0.3484217470,  0.3583906530,  0.3683595590,  0.3783284650,  0.3882973710,\
  0.3963064170,  0.4043154620,  0.4123245080,  0.4203335530,  0.4283425990,\
  0.4374634880,  0.4465843780,  0.4557052670,  0.4648261570,  0.4739470460,\
  0.4825402810,  0.4911335150,  0.4997267500,  0.5083199850,  0.5169132200,\
  0.5253933850,  0.5338735510,  0.5423537170,  0.5508338830,  0.5593140490,\
  0.5690002830,  0.5786865170,  0.5883727500,  0.5980589840,  0.6077452180,\
  0.6168661080,  0.6259869970,  0.6351078870,  0.6442287760,  0.6533496660,\
  0.6621501930,  0.6709507210,  0.6797512480,  0.6885517760,  0.6973523040,\
  0.7063601240,  0.7153679450,  0.7243757660,  0.7333835860,  0.7423914070,\
  0.7497031940,  0.7570149820,  0.7643267690,  0.7716385560,  0.7789503440,\
  0.7868840100,  0.7948176760,  0.8027513430,  0.8106850090,  0.8186186750,\
  0.8261377560,  0.8336568360,  0.8411759160,  0.8486949970,  0.8562140770,\
  0.8630358990,  0.8698577220,  0.8766795440,  0.8835013660,  0.8903231890,\
  0.8963912180,  0.9024592480,  0.9085272780,  0.9145953080,  0.9206633370,\
  0.9261283330,  0.9315933290,  0.9370583250,  0.9425233200,  0.9479883160,\
  0.9512296240,  0.9544709320,  0.9577122400,  0.9609535480,  0.9641948550,\
  0.9673042500,  0.9704136440,  0.9735230380,  0.9766324320,  0.9797418260,\
  0.9817016870,  0.9836615470,  0.9856214080,  0.9875812680,  0.9895411290,\
  0.9910110240,  0.9924809200,  0.9939508150,  0.9954207100,  0.9968906060,\
  0.9973428810,  0.9977951570,  0.9982474320,  0.9986997080,  0.9991519830,\
  0.9993215870,  0.9994911900,  0.9996607930,  0.9998303970,  1.0000000000,\
  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,\
  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,\
  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,\
  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000,  1.0000000000};
  // Here is the range of dp/p in the above distribution: 
  static const double g4bl_dpp_min = -3.125e-2;
  static const double g4bl_dpp_max =  3.125e-2;

  // Generate a random number
  double rand = G4RandFlat::shoot();
  // Find the random number in the above integral table
  for (int i=0; i!=(nbins-1); ++i) {
    if ( (g4bl_dpp_cdf[i]<rand) && (rand<g4bl_dpp_cdf[i+1]) ) {
      // Map the random number to a muon dp/p
      double d_cdf = g4bl_dpp_cdf[i+1] - g4bl_dpp_cdf[i];
      double frac_high = ( g4bl_dpp_cdf[i+1]-rand )/d_cdf;
      double frac_low  = ( rand-g4bl_dpp_cdf[ i ] )/d_cdf;
      double ix_interp = frac_low*i + frac_high*(i+1);
      double g4bl_dpp = (ix_interp/nbins - 0.5) * (g4bl_dpp_max - g4bl_dpp_min);
      return g4bl_dpp;
    }
  }
  return 0.;
}


void Twiss::Print()
{
  printf ("%12s  %i %s\n",    "emit_distr:",         emit_distr, "(0=delta-function, 1=flat/uniform, 2=gaussian, 821, 989)");
  printf ("%12s% 12.4e  %s\n",      "emit:",       emit/mm/mrad, "mm-mrad");
  printf ("%12s% 12.4e  %s\n",  "rms_emit:",   rms_emit/mm/mrad, "mm-mrad");
  printf ("%12s% 12.4e  %s\n",      "beta:",             beta/m, "m"      );
  printf ("%12s% 12.4e  %s\n",     "alpha:",              alpha, "-"      );
  printf ("%12s% 12.4e  %s\n",     "gamma:",        gamma/(1/m), "1/m"    );
  printf ("%12s% 12.4e  %s\n",       "eta:",              eta/m, "m"      );
  printf ("%12s% 12.4e  %s\n",      "etap:",             etap/m, "-"      );
  printf ("%12s% 12.4e  %s\n",       "phi:",            phi/rad, "rad"    );
}


PhaseSpace::PhaseSpace()
{}


PhaseSpace::~PhaseSpace()
{}


MatrixXd TransferMatrixFromTwiss (const Twiss &twissx0, const Twiss &twissy0, const Twiss &twissx1, const Twiss &twissy1)
{
  // Given an initial set of Twiss parameters, this subroutine calculates the linear transfer map 

  // 2x2 helper matrices, from which the 6x6 matrix is built
  Matrix2d Ax, Bx, Cx, Dx, Rx; // horizontal
  Matrix2d Ay, By, Cy, Dy, Ry; // vertical

  //================================================
  // HORIZONTAL BETATRON ---------------------------
  //================================================
  // Initial/Starting
  if (twissx0.beta!=0) {
    // Floquet transformation to action-angle variables
    Ax(0,0) = 1/sqrt( twissx0.beta );
    Ax(1,0) = 0;
    Ax(0,1) = twissx0.alpha/sqrt( twissx0.beta );
    Ax(1,1) = sqrt( twissx0.beta );
  } else {
    Ax = Matrix2d::Identity();
    Ax = Matrix2d::Zero();
    G4cout << "ERROR: twissx0.beta=0" << G4endl;
    throw;
  }
  // Target/Desired
  if (twissx1.beta!=0) {
    // Inverse Floquet transformation to desired Twiss parameters
    Bx(0,0) =  sqrt( twissx1.beta );
    Bx(0,1) =  0;
    Bx(1,0) = -twissx1.alpha/sqrt( twissx1.beta );
    Bx(1,1) =  1/sqrt( twissx1.beta );
  } else {
    G4cout << "ERROR: twissx1.beta=0" << G4endl;
    throw;
  }
  // Phase advance
  Rx(0,0) =  cos(twissx1.phi-twissx0.phi);
  Rx(0,1) =  sin(twissx1.phi-twissx0.phi);
  Rx(1,0) = -sin(twissx1.phi-twissx0.phi);
  Rx(1,1) =  cos(twissx1.phi-twissx0.phi);
  // Total horizontal betatron part of 6x6 transformation
  Bx = Bx*(Rx*Ax);

  //================================================
  // VERTICAL BETATRON -----------------------------
  //================================================
  // Initial/Starting
  if (twissy0.beta!=0) {
    // Floquet transformation to action-angle variables
    Ay(0,0) = 1/sqrt( twissy0.beta );
    Ay(1,0) = 0;
    Ay(0,1) = twissy0.alpha/sqrt( twissy0.beta );
    Ay(1,1) = sqrt( twissy0.beta );
  } else {
    throw;
  }
  // Target/Desired
  if (twissy1.beta!=0) {
    // Inverse Floquet transformation to desired Twiss
    By(0,0) =  sqrt( twissy1.beta );
    By(0,1) =  0;
    By(1,0) = -twissy1.alpha/sqrt( twissy1.beta );
    By(1,1) =  1/sqrt( twissy1.beta );
  } else {
    throw;
  }
  // Phase advance
  Ry(0,0) =  cos(twissy1.phi-twissy0.phi);
  Ry(0,1) =  sin(twissy1.phi-twissy0.phi);
  Ry(1,0) = -sin(twissy1.phi-twissy0.phi);
  Ry(1,1) =  cos(twissy1.phi-twissy0.phi);
  // Total vertical betatron part of 6x6 transformation
  By = By*(Ry*Ay);

  //================================================
  // HORIZONTAL DISPERSION -------------------------
  //================================================
  // Initial dispersion/derivative
  Cx(0,0) = 0;
  Cx(1,0) = 0;
  Cx(0,1) = twissx0.eta;
  Cx(1,1) = twissx0.etap;
  // Final dispersion/derivative
  Dx(0,0) = 0;
  Dx(1,0) = 0;
  Dx(0,1) = twissx1.eta;
  Dx(1,1) = twissx1.etap;
  // Total horizontal dispersion part of 6x6 transformation
  Dx = Dx - Bx*Cx;

  //================================================
  // VERTICAL DISPERSION ---------------------------
  //================================================
  // Initial dispersion/derivative
  Cy(0,0) = 0;
  Cy(1,0) = 0;
  Cy(0,1) = twissy0.eta;
  Cy(1,1) = twissy0.etap;
  // Final dispersion/derivative
  Dy(0,0) = 0;
  Dy(1,0) = 0;
  Dy(0,1) = twissy1.eta;
  Dy(1,1) = twissy1.etap;
  // Total horizontal dispersion part of 6x6 transformation
  Dy = Dy - By*Cy;

  //================================================
  // TOTAL 6x6 TRANSFORMATION MATRIX ---------------
  //================================================
  // Built from above 2x2 sub-blocks
  int ncoord = 10;
  MatrixXd TT = MatrixXd::Identity(ncoord,ncoord);
  // Horizontal betatron
  TT(1,1) = Bx(0,0);
  TT(1,2) = Bx(0,1);
  TT(2,1) = Bx(1,0);
  TT(2,2) = Bx(1,1);
  // Horizontal dispersion/derivative
  TT(1,5) = Dx(0,0);
  TT(1,6) = Dx(0,1);
  TT(2,5) = Dx(1,0);
  TT(2,6) = Dx(1,1);
  // Vertical betatron
  TT(3,3) = By(0,0);
  TT(3,4) = By(0,1);
  TT(4,3) = By(1,0);
  TT(4,4) = By(1,1);
  // Vertical dispersion/derivative
  TT(3,5) = Dy(0,0);
  TT(3,6) = Dy(0,1);
  TT(4,5) = Dy(1,0);
  TT(4,6) = Dy(1,1);

  return TT;
}


void CalculateBeamParameters( PhaseSpace &phsp, int verbosity=0 )
{
  // Helper variables
  int nmuons = phsp.coords.rows();
  int ncoord = phsp.coords.cols();
  double emitx, betax, alphax, gammax, etax, etapx, phix;
  double emity, betay, alphay, gammay, etay, etapy, phiy;

  // Reinitialize phase-space variables
  phsp.avg.resize(ncoord);         phsp.avg.setZero();
  phsp.rms.resize(ncoord);         phsp.rms.setZero();
  phsp.cov.resize( ncoord,ncoord); phsp.cov.setZero();
  phsp.cor.resize( ncoord,ncoord); phsp.cor.setZero();
  phsp.covb.resize(ncoord,ncoord); phsp.covb.setZero();

  // Compute averages { <t>, <x>, <xp>, <y>, <yp>, <z>, <dp/p>, <sx>, <sy>, <sz> }
  for (int n=0; n!=nmuons; ++n) {
    for (int i=0; i!=ncoord; ++i) {
      phsp.avg(i) += phsp.coords(n,i);
    }
  }
  phsp.avg /= nmuons;

  // Compute covariance matrix
  for (int n=0; n!=nmuons; ++n) {
    for (int i=0; i!=ncoord; ++i) {
      for (int j=i; j!=ncoord; ++j) {
        phsp.cov(i,j) += ( phsp.coords(n,i)-phsp.avg(i) )*( phsp.coords(n,j)-phsp.avg(j) );
      }
    }
  }
  for (int i=0; i!=ncoord; ++i) {
    for (int j=i; j!=ncoord; ++j) {
      phsp.cov(i,j) /= nmuons;
      if (j!=i) phsp.cov(j,i) = phsp.cov(i,j);
    }
  }

  // Compute coordinate RMS's
  for (int i=0; i!=ncoord; ++i) {
    phsp.rms(i) = sqrt( phsp.cov(i,i) );
  }

  // Compute correlation matrix
  for (int i=0; i!=ncoord; ++i) {
    for (int j=i; j!=ncoord; ++j) {
      if ( phsp.rms(i)!=0 && phsp.rms(j)!=0 ) {
        phsp.cor(i,j) = phsp.cov(i,j)/( phsp.rms(i)*phsp.rms(j) );
      } else {
        phsp.cor(i,j) = 0;
      }
      if (j!=i) phsp.cor(j,i) = phsp.cor(i,j);
    }
  }

  // Compute betatron-only part of covariance matrix
  if (phsp.cov(6,6)==0) {
    phsp.covb = phsp.cov;
  } else {
    for (int i=1; i!=7; ++i) {
      for (int j=1; j!=7; ++j) {
        // Subtract away the dispersion component
        phsp.covb(i,j) = phsp.cov(i,j) - phsp.cov(i,6)*phsp.cov(j,6)/phsp.cov(6,6);
      }
    }
  }

  // Phase advance
  phix = 0;
  phiy = 0;

  // Compute emittances
  emitx = sqrt( phsp.covb(1,1)*phsp.covb(2,2) - pow(phsp.cov(1,2),2) );
  emity = sqrt( phsp.covb(3,3)*phsp.covb(4,4) - pow(phsp.cov(3,4),2) );

  // Compute horizontal Twiss parameters
  if (emitx!=0) {
    betax  =  phsp.covb(1,1)/emitx;
    alphax = -phsp.covb(1,2)/emitx;
    gammax =  phsp.covb(2,2)/emitx;
  } else {
    betax  = 0;
    alphax = 0;
    gammax = 0;
  }
  // Compute vertical Twiss parameters
  if (emity!=0) {
    betay  =  phsp.covb(3,3)/emity;
    alphay = -phsp.covb(3,4)/emity;
    gammay =  phsp.covb(4,4)/emity;
  } else {
    betay  = 0;
    alphay = 0;
    gammay = 0;
  }
  // Compute dispersion/derivative
  if (phsp.cov(6,6)!=0) {
    etax  = phsp.cov(1,6)/phsp.cov(6,6);
    etapx = phsp.cov(2,6)/phsp.cov(6,6);
    etay  = phsp.cov(3,6)/phsp.cov(6,6);
    etapy = phsp.cov(4,6)/phsp.cov(6,6);
  } else {
    etax  = 0;
    etapx = 0;
    etay  = 0;
    etapy = 0;
  }

  // Assign calculated parameters
  phsp.twissx.beta  = betax;
  phsp.twissx.alpha = alphax;
  phsp.twissx.gamma = gammax;
  phsp.twissx.eta   = etax;
  phsp.twissx.etap  = etapx;
  phsp.twissx.phi   = phix;

  phsp.twissy.beta  = betay;
  phsp.twissy.alpha = alphay;
  phsp.twissy.gamma = gammay;
  phsp.twissy.eta   = etay;
  phsp.twissy.etap  = etapy;
  phsp.twissy.phi   = phiy;

  phsp.twissx.rms_emit = emitx;
  phsp.twissy.rms_emit = emity;
  phsp.twissx.emit = emitx;
  phsp.twissy.emit = emity;
  if (phsp.twissx.emit_distr==1) phsp.twissx.emit=4*emitx;
  if (phsp.twissy.emit_distr==1) phsp.twissy.emit=4*emitx;

  // Print calculated beam parameters
  if (verbosity>0) {
    string str; str.insert(0,51,'=');
    cout << str << endl;
    printf("%s%5.0e%s","Transverse phase-space information for ", double(nmuons)," muons:\n");
    cout << str << endl;
    cout << "~~~~~~~~~~~\n";
    cout << "HORIZONTAL:\n";
    cout << "~~~~~~~~~~~\n";
    phsp.twissx.Print();
    cout << "~~~~~~~~~\n";
    cout << "VERTICAL:\n";
    cout << "~~~~~~~~~\n";
    phsp.twissy.Print();
    if (verbosity>1) {
      cout << endl;
      cout << "AVG"  << endl << phsp.avg.transpose() << endl << endl;
      cout << "RMS"  << endl << phsp.rms.transpose() << endl << endl;
      cout << "COR"  << endl << phsp.cor  << endl << endl;
      cout << "COV"  << endl << phsp.cov  << endl << endl;
      cout << "COVB" << endl << phsp.covb << endl << endl;
    }
    cout << "\n\n";
  }
}


void CreatePhaseSpace( int nmuons, const Twiss &twissx, const Twiss &twissy, PhaseSpace &phsp )
{
  // Copy distribution information
  phsp.twissx.emit_distr = twissx.emit_distr;
  phsp.twissy.emit_distr = twissy.emit_distr;

  int ncoord = 10; // { t, x, x', y, y', z, dp/p, sx, sy, sz }
  phsp.coords.resize(nmuons,ncoord);
  phsp.coords.setZero();

  //================================================
  //  TRANSVERSE DISTRIBUTION (HORIZONTAL)
  //================================================
  if ( (twissx.emit_distr==0) || (nmuons==1) ) {
    // Delta-function
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,1) = 0;
      phsp.coords(n,2) = 0;
    }
  } else if (twissx.emit_distr==1) {
    // Flat/Uniform
    for (int n=0; n!=nmuons; ++n) {
      bool within_ellipse = false;
      do {
        phsp.coords(n,1) = 2*( G4RandFlat::shoot()-0.5 ) * sqrt(twissx.emit*twissx.beta);
        phsp.coords(n,2) = 2*( G4RandFlat::shoot()-0.5 ) * sqrt(twissx.emit/twissx.beta);
        if ( (pow(phsp.coords(n,1),2)/(twissx.emit*twissx.beta) + pow(phsp.coords(n,2),2)/(twissx.emit/twissx.beta))<1 ) within_ellipse=true;
      } while (within_ellipse==false);
    }
  } else if (twissx.emit_distr==2) {
    // Gaussian
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,1) = G4RandGauss::shoot() * sqrt( twissx.emit*twissx.beta );
      phsp.coords(n,2) = G4RandGauss::shoot() * sqrt( twissx.emit/twissx.beta );
    }
  }

  //================================================
  //  TRANSVERSE DISTRIBUTION (VERTICAL)
  //================================================
  if ( (twissy.emit_distr==0) || (nmuons==1) ) {
    // Delta-function
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,3) = 0;
      phsp.coords(n,4) = 0;
    }
  } else if (twissy.emit_distr==1) {
    // Flat/Uniform
    for (int n=0; n!=nmuons; ++n) {
      bool within_ellipse = false;
      do {
        phsp.coords(n,3) = 2*( G4RandFlat::shoot()-0.5 )*sqrt(twissy.emit*twissy.beta);
        phsp.coords(n,4) = 2*( G4RandFlat::shoot()-0.5 )*sqrt(twissy.emit/twissy.beta);
        if ( (pow(phsp.coords(n,3),2)/(twissy.emit*twissy.beta) + pow(phsp.coords(n,4),2)/(twissy.emit/twissy.beta))<1 ) within_ellipse=true;
      } while (within_ellipse==false);
    }
  } else if (twissy.emit_distr==2) {
    // Gaussian
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,3) = G4RandGauss::shoot() * sqrt( twissy.emit*twissy.beta );
      phsp.coords(n,4) = G4RandGauss::shoot() * sqrt( twissy.emit/twissy.beta );
    }
  }

  //================================================
  //  MOMENTUM DISTRIBUTION
  //================================================
  if ( (phsp.p_distr==0) || (nmuons==1) ){
    // Delta-function
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,6) = 0;
    }
  } else if (phsp.p_distr==1) {
    // Flat
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,6) = ( G4RandFlat::shoot()-0.5 )*phsp.p_width;
    }
  } else if (phsp.p_distr==2) {
    // Gaussian
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,6) = G4RandGauss::shoot()*phsp.p_width;
    }
  } else if (phsp.p_distr==989) {
    // E989
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,6) = ran_g4bl_dpp();
    }
  } else {
    // Throw an error
    throw;
  }

  //================================================
  //  TIME DISTRIBUTION
  //================================================
  if ( (phsp.t_distr==0) || (nmuons==1) ) {
    // Delta-function
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,0) = 0;
    }
  } else if (phsp.t_distr==1) {
    // Flat
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,0) = ( G4RandFlat::shoot()-0.5 ) * phsp.t_width;
    }
  } else if (phsp.t_distr==2) {
    // Gaussian
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,0) = G4RandGauss::shoot() * phsp.t_width;
    }
  } else if (phsp.t_distr==821) {
    // E821
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,0) = G4RandGauss::shoot() * 25*ns;
    }
  } else if (phsp.t_distr==989) {
    // E989
    for (int n=0; n!=nmuons; ++n) {
      phsp.coords(n,0) = ran_fnalw(0*ns,120*ns);
    }
  }
  CalculateBeamParameters(phsp);

  // Transform transverse phase-space coordinates to non-zero alpha and dispersion/derivative
  MatrixXd TT = TransferMatrixFromTwiss( phsp.twissx, phsp.twissy, twissx, twissy );
  phsp.coords = (TT*phsp.coords.transpose()).transpose();
  CalculateBeamParameters(phsp,1);

  WritePhaseSpace(phsp,"phsp.dat");

  throw;

#if 0
  cout << phsp.coords << endl;
  throw;

  cout << nmuons << endl;
  cout << phsp.co.size() << endl;
  cout << endl;

  cout << twissx.emit  << endl;
  cout << twissx.beta  << endl;
  cout << twissx.alpha << endl;
  cout << twissx.gamma << endl;
  cout << twissx.eta   << endl;
  cout << twissx.etap  << endl;
  cout << twissx.phi   << endl;
  cout << endl;

  cout << twissy.emit  << endl;
  cout << twissy.beta  << endl;
  cout << twissy.alpha << endl;
  cout << twissy.gamma << endl;
  cout << twissy.eta   << endl;
  cout << twissy.etap  << endl;
  cout << twissy.phi   << endl;
  cout << endl;

  cout << sqrt( twissx.emit*twissx.beta ) << endl;
  cout << sqrt( twissx.emit/twissx.beta ) << endl;
  cout << sqrt( twissy.emit*twissy.beta ) << endl;
  cout << sqrt( twissy.emit/twissy.beta ) << endl;
  cout << endl;
#endif

} // END "CreatePhaseSpace()"


void WritePhaseSpace( const PhaseSpace &phsp, const string filename)
{
  // Internal helpers
  int nmuons = phsp.coords.rows();
  double t, x, xp, y, yp, z, dpp, sx, sy, sz;
  // Open output file
  FILE * outfile = fopen( filename.c_str(), "w" );
  // Write header
  fprintf(outfile,"%s%11s%12s%12s%12s%12s%12s%12s%12s%12s%12s\n","#","x(m)","x'(rad)","y(m)","y'(rad)","s(m)","dp/p","t(ns)","sx","sy","sz");
  // Write phase-space data
  for (int n=0; n!=nmuons; ++n) {
    t  = phsp.coords(n,0);
    x  = phsp.coords(n,1);
    xp = phsp.coords(n,2);
    y  = phsp.coords(n,3);
    yp = phsp.coords(n,4);
    z  = phsp.coords(n,5);
    dpp= phsp.coords(n,6);
    sx = phsp.coords(n,7);
    sy = phsp.coords(n,8);
    sz = phsp.coords(n,9);
    fprintf(outfile,"% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e\n",x,xp,y,yp,z,dpp,t,sx,sy,sz);
  }
  // Close file
  fclose(outfile);
  // Print message for user
  cout << endl << "Phase space for " << nmuons << " muons written to \"" << filename << "\"" << endl << endl;
}


