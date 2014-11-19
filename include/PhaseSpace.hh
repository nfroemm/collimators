#ifndef PhaseSpace_hh
#define PhaseSpace_hh

#include<vector>
using namespace std;

#include<Eigen/Dense>
using namespace Eigen;


class Twiss
{
  public:
    double beta, alpha, gamma, eta, etap, phi;
    double emit, rms_emit;
    int emit_distr; // 0=delta, 1=flat/uniform, 2=gaussian, 821, 989

    // Methods
    void Print();
};


class PhaseSpace
{
  public: 
    // Transverse Twiss parameters and dispersion/derivative
    Twiss twissx;
    Twiss twissy;
    // Longitudinal distribution and values
    int    p_distr, t_distr;
    double p_width, t_width;
    // Muon phase-space coordinates
    MatrixXd coords; // {t, x, xp, y, yp, z, dp/p, sx, sy, sz}

    // Ensemble beam properties
    VectorXd avg;  // averages of all phase-space coordinates
    VectorXd rms;  // rms's of all phase-space coordinates
    MatrixXd cov;  // covariance matrix
    MatrixXd covb; // betatron-only part of covariance matrix
    MatrixXd cor;  // correlation matrix

    // Default constructor/destructor
    PhaseSpace();
   ~PhaseSpace();

  private:
};


// Helpers
double ran_fnalw (double center, double full_width);
double ran_g4bl_dpp();

void CreatePhaseSpace (int nmuons, const Twiss &twissx, const Twiss &twissy, PhaseSpace &phsp);
void WritePhaseSpace (const PhaseSpace &phsp, const string filename);

void CalculateBeamParameters (PhaseSpace &phsp, int verbosity);
MatrixXd TransferMatrixFromTwiss (const Twiss &twissx0, const Twiss &twissy0, const Twiss &twissx1, const Twiss &twissy1, MatrixXd &mat);



#endif
