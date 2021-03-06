/**
 * Gamma Combination
 * Author: Matthew Kenzie matthew.kenzie@cern.ch
 * Date: August 2015
 *
 **/

#include "PDF_GLWADS_DKDpi_K3pi_newVars.h"

PDF_GLWADS_DKDpi_K3pi_newVars::PDF_GLWADS_DKDpi_K3pi_newVars(config cObs, config cErr, config cCor,
  ParametersAbs* pars, TString d)
: PDF_Abs(9)
{
	if ( pars ) p = pars;
	else p = new ParametersGammaCombo();
  name = "glwads-dkdpi-k3pi-newvars";
	initParameters();
	initRelations();
	initObservables();
	setObservables(cObs);
	setUncertainties(cErr);
	setCorrelations(cCor);
	buildCov();
	buildPdf();
}


PDF_GLWADS_DKDpi_K3pi_newVars::~PDF_GLWADS_DKDpi_K3pi_newVars(){}


void PDF_GLWADS_DKDpi_K3pi_newVars::initParameters()
{
	parameters = new RooArgList("parameters");
	parameters->add(*(p->get("r_dk")));
	parameters->add(*(p->get("d_dk")));
	parameters->add(*(p->get("r_dpi")));
	parameters->add(*(p->get("d_dpi")));
	parameters->add(*(p->get("g")));
	parameters->add(*(p->get("rD_k3pi")));
	parameters->add(*(p->get("dD_k3pi")));
	parameters->add(*(p->get("kD_k3pi")));
	parameters->add(*(p->get("F_pipipipi")));
	parameters->add(*(p->get("RBRdkdpi")));
}


void PDF_GLWADS_DKDpi_K3pi_newVars::initRelations()
{
  RooArgSet *p = (RooArgSet*)parameters;
  theory = new RooArgList("theory"); ///< the order of this list must match that of the COR matrix!
  theory->add(*(new RooFormulaVar("aads_dk_k3pi_th",   "aads_dk_k3pi_th",  " ( 2*kD_k3pi*r_dk*rD_k3pi*sin(d_dk+dD_k3pi)*sin(g) ) / ( r_dk^2 + rD_k3pi^2 + 2*kD_k3pi*r_dk*rD_k3pi*cos(d_dk + dD_k3pi)*cos(g) ) ", *p)));
  theory->add(*(new RooFormulaVar("aads_dpi_k3pi_th",  "aads_dpi_k3pi_th", " ( 2*kD_k3pi*r_dpi*rD_k3pi*sin(d_dpi+dD_k3pi)*sin(g) ) / ( r_dpi^2 + rD_k3pi^2 + 2*kD_k3pi*r_dpi*rD_k3pi*cos(d_dpi + dD_k3pi)*cos(g) ) ", *p)));
	theory->add(*(new RooFormulaVar("acp_dk_4pi_th",     "acp_dk_4pi_th",    "2*(2*F_pipipipi-1)*r_dk *sin(d_dk) *sin(g) / (1 + r_dk^2  + 2*(2*F_pipipipi-1)*r_dk *cos(d_dk) *cos(g))", *p)));
	theory->add(*(new RooFormulaVar("acp_dpi_4pi_th",    "acp_dpi_4pi_th",    "2*(2*F_pipipipi-1)*r_dpi *sin(d_dpi) *sin(g) / (1 + r_dpi^2  + 2*(2*F_pipipipi-1)*r_dpi *cos(d_dpi) *cos(g))", *p)));
	theory->add(*(new RooFormulaVar("afav_dk_k3pi_th",   "afav_dk_k3pi_th",  "2*r_dk *rD_k3pi*kD_k3pi*sin(g)*sin(d_dk -dD_k3pi) / (1 + r_dk^2  * rD_k3pi^2 + 2*r_dk *rD_k3pi*kD_k3pi*cos(g)*cos(d_dk -dD_k3pi))", *p)));
  theory->add(*(new RooFormulaVar("rads_dk_k3pi_th",   "rads_dk_k3pi_th",  " ( r_dk^2 + rD_k3pi^2 + 2*kD_k3pi*r_dk*rD_k3pi*cos(d_dk + dD_k3pi)*cos(g) ) / ( 1 + (r_dk*rD_k3pi)^2 + 2*kD_k3pi*r_dk*rD_k3pi*cos(d_dk-dD_k3pi)*cos(g) )", *p)));
  theory->add(*(new RooFormulaVar("rads_dpi_k3pi_th",  "rads_dpi_k3pi_th", " ( r_dpi^2 + rD_k3pi^2 + 2*kD_k3pi*r_dpi*rD_k3pi*cos(d_dpi + dD_k3pi)*cos(g) ) / ( 1 + (r_dpi*rD_k3pi)^2 + 2*kD_k3pi*r_dpi*rD_k3pi*cos(d_dpi-dD_k3pi)*cos(g) )", *p)));
	theory->add(*(new RooFormulaVar("rcp_4pi_th",       "rcp_4pi_th",       "(1 + r_dk^2 + 2*(2*F_pipipipi-1)*r_dk*cos(d_dk)*cos(g))", *p)));
	theory->add(*(new RooFormulaVar("rkp_k3pi_th",      "rkp_k3pi_th",      "RBRdkdpi*(1 + r_dk^2 * rD_k3pi^2 + 2*r_dk*rD_k3pi*kD_k3pi*cos(g)*cos(d_dk-dD_k3pi))/(1 + r_dpi^2 * rD_k3pi^2 + 2*r_dpi*rD_k3pi*kD_k3pi*cos(g)*cos(d_dpi-dD_k3pi))", *p)));

}

void PDF_GLWADS_DKDpi_K3pi_newVars::initObservables()
{
	observables = new RooArgList("observables");
	observables->add(*(new RooRealVar("aads_dk_k3pi_obs",  "A_{ADS} (DK K3#pi)",    0, -1e4, 1e4)));
	observables->add(*(new RooRealVar("aads_dpi_k3pi_obs", "A_{ADS} (D#pi K3#pi)",  0, -1e4, 1e4)));
	observables->add(*(new RooRealVar("acp_dk_4pi_obs",    "ACP (DK 4#pi)",         0, -1e4, 1e4)));
	observables->add(*(new RooRealVar("acp_dpi_4pi_obs",   "ACP (D#pi 4#pi)",       0, -1e4, 1e4)));
	observables->add(*(new RooRealVar("afav_dk_k3pi_obs",  "Afav (DK K3#pi)"    ,   0, -1e4, 1e4)));
	observables->add(*(new RooRealVar("rads_dk_k3pi_obs",  "R_{ADS} (DK K3#pi)",    1, -1e4, 1e4)));
	observables->add(*(new RooRealVar("rads_dpi_k3pi_obs", "R_{ADS} (D#pi K3#pi)",  1, -1e4, 1e4)));
	observables->add(*(new RooRealVar("rcp_4pi_obs",       "R_{CP} (4#pi)",     	  1, -1e4, 1e4)));
	observables->add(*(new RooRealVar("rkp_k3pi_obs",      "R_{K/#pi} (K3#pi)"   ,  1, -1e4, 1e4)));

  latexObservables.clear();
  latexObservables.push_back("\\AadsDKKppp");
  latexObservables.push_back("\\AadsDPiKppp");
  latexObservables.push_back("\\AcpDKpppp");
  latexObservables.push_back("\\AcpDpipppp");
  latexObservables.push_back("\\AfavDkKppp");
  latexObservables.push_back("\\RadsDKKppp");
  latexObservables.push_back("\\RadsDPiKppp");
  latexObservables.push_back("\\RCPpppp");
  latexObservables.push_back("\\RkpKppp");
}

void PDF_GLWADS_DKDpi_K3pi_newVars::setObservables(config c)
{
	switch(c)
	{
		case truth:
			{
				 setObservablesTruth();
				 break;
			}
		case toy:
			{
				 setObservablesToy();
				 break;
			}
		case lumi3fb:
			{
				 obsValSource = "3fb-1 ANA v7 - lifted from gammadini "; // https://twiki.cern.ch/twiki/pub/LHCbPhysics/B2D0K/LHCb-ANA-2014-071-v7.pdf (see Vavas email 04/08/15)
				 setObservable( "aads_dk_k3pi_obs",  -3.1291e-01 );
				 setObservable( "aads_dpi_k3pi_obs",  2.3228e-02 );
				 setObservable( "acp_dk_4pi_obs",     1.0035e-01 );
				 setObservable( "acp_dpi_4pi_obs",   -4.1278e-03 );
				 setObservable( "afav_dk_k3pi_obs",  -4.1540e-04 );
				 setObservable( "rads_dk_k3pi_obs",   1.3992e-02 );
         setObservable( "rads_dpi_k3pi_obs",  3.7718e-03 );
				 setObservable( "rcp_4pi_obs",        9.7540e-01 );
				 setObservable( "rkp_k3pi_obs",       7.9306e-02 );
				 break;
			}
    case check:
      {
				 obsValSource = "check";
				 setObservable( "aads_dk_k3pi_obs",  -3.1291e-01 );
				 setObservable( "aads_dpi_k3pi_obs",  2.3228e-02 );
				 setObservable( "acp_dk_4pi_obs",     1.0035e-01 );
				 setObservable( "acp_dpi_4pi_obs",   -4.1278e-03 );
				 setObservable( "afav_dk_k3pi_obs",  -4.1540e-04 );
				 setObservable( "rads_dk_k3pi_obs",   1.3992e-02 );
         //setObservable( "rads_dpi_k3pi_obs",  3.7718e-03 ); // THE REAL VALUE
				 setObservable( "rads_dpi_k3pi_obs",  3.39e-03 ); // check value
				 setObservable( "rcp_4pi_obs",        9.7540e-01 );
				 setObservable( "rkp_k3pi_obs",       7.9306e-02 );
				 break;
      }
		default:
			{
				 cout << "PDF_GLWADS_DKDpi_K3pi_newVars::setObservables() : ERROR : config "+ConfigToTString(c)+" not found." << endl;
				 exit(1);
			}
	}
}

void PDF_GLWADS_DKDpi_K3pi_newVars::setUncertainties(config c)
{
	switch(c)
	{
		case lumi3fb:
			{
				 obsErrSource = "3fb-1 ANA v7 - lifted from gammadini "; // https://twiki.cern.ch/twiki/pub/LHCbPhysics/B2D0K/LHCb-ANA-2014-071-v7.pdf (see Vavas email 04/08/15)

						StatErr[0]  = 1.02e-01 ; // aads_dk_k3pi_obs
						StatErr[1]  = 4.80e-02 ; // aads_dpi_k3pi_obs
						StatErr[2]  = 3.36e-02 ; // acp_dk_4pi_obs
						StatErr[3]  = 7.91e-03 ; // acp_dpi_4pi_obs
						StatErr[4]  = 1.19e-02 ; // afav_dk_k3pi_obs
						StatErr[5]  = 1.49e-03 ; // rads_dk_k3pi_obs
						StatErr[6]  = 1.84e-04 ; // rads_dpi_k3pi_obs
						StatErr[7]  = 3.67e-02 ; // rcp_4pi_obs
						StatErr[8]  = 9.77e-04 ; // rkp_k3pi_obs

						SystErr[0]  = 0.015    ; // aads_dk_k3pi_obs
						SystErr[1]  = 0.0068   ; // aads_dpi_k3pi_obs
						SystErr[2]  = 0.0079   ; // acp_dk_4pi_obs
						SystErr[3]  = 0.0032   ; // acp_dpi_4pi_obs
						SystErr[4]  = 0.0030   ; // afav_dk_k3pi_obs
						SystErr[5]  = 0.00041  ; // rads_dk_k3pi_obs
						SystErr[6]  = 0.00005  ; // rads_dpi_k3pi_obs
						SystErr[7]  = 0.010    ; // rcp_4pi_obs
						SystErr[8]  = 0.0020   ; // rkp_k3pi_obs

						break;
			}
		default:
			{
				 cout << "PDF_GLWADS_DKDpi_K3pi_newVars::setUncertainties() : ERROR : config "+ConfigToTString(c)+" not found." << endl;
				 exit(1);
			}
	}
}

void PDF_GLWADS_DKDpi_K3pi_newVars::setCorrelations(config c)
{
	resetCorrelations();
	switch(c)
	{
		case lumi3fb:
			{
				corSource = "3fb-1 ANA v7 - lifted from gammadini "; // https://twiki.cern.ch/twiki/pub/LHCbPhysics/B2D0K/LHCb-ANA-2014-071-v7.pdf (see Vavas email 04/08/15)

				// lifted from gammadini
				const double statCorrelations[81] = {
					1.0   ,-0.0618, 0.0018, 0.0088, 0.0064, 0.0821,-0.0040, 0.0020, 0.0028,
				 -0.0618, 1.0   , 0.0050, 0.0202, 0.0166, 0.0135,-0.0218,-0.0001, 0.0005,
					0.0018, 0.0050, 1.0   ,-0.0197, 0.0239,-0.0011, 0.0000,-0.0182,-0.0023,
					0.0088, 0.0202,-0.0197, 1.0   , 0.0968,-0.0001,-0.0001,-0.0041,-0.0004,
					0.0064, 0.0166, 0.0239, 0.0968, 1.0   ,-0.0005,-0.0001,-0.0009, 0.0012,
					0.0821, 0.0135,-0.0011,-0.0001,-0.0005, 1.0   ,-0.0464, 0.0409,-0.0990,
				 -0.0040,-0.0218, 0.0000,-0.0001,-0.0001,-0.0464, 1.0   ,-0.0041, 0.0116,
					0.0020,-0.0001,-0.0182,-0.0041,-0.0009, 0.0409,-0.0041, 1.0   ,-0.3080,
					0.0028, 0.0005,-0.0023,-0.0004, 0.0012,-0.0990, 0.0116,-0.3080, 1.0   };

				corStatMatrix = TMatrixDSym( nObs, statCorrelations );

				// lifted from gammadini
				const double systCorrelations[144] = {
					1.00,  0.36, -0.06,  0.27,  0.30,  0.04,  0.09,  0.78, -0.43, -0.04,  0.23, -0.14,
					0.36,  1.00, -0.03,  0.31,  0.22, -0.06, -0.55,  0.59, -0.47, -0.01,  0.12, -0.04,
				 -0.06, -0.03,  1.00, -0.02, -0.80,  0.09,  0.09, -0.10, -0.06,  0.03, -0.28,  0.07,
					0.27,  0.31, -0.02,  1.00,  0.19, -0.42, -0.01,  0.35, -0.28, -0.22,  0.11, -0.07,
					0.30,  0.22, -0.80,  0.19,  1.00,  0.11,  0.09,  0.37, -0.21, -0.16,  0.20, -0.13,
					0.04, -0.06,  0.09, -0.42,  0.11,  1.00,  0.30, -0.03,  0.06, -0.08, -0.03, -0.09,
					0.09, -0.55,  0.09, -0.01,  0.09,  0.30,  1.00, -0.11,  0.05, -0.01, -0.02, -0.02,
					0.78,  0.59, -0.10,  0.35,  0.37, -0.03, -0.11,  1.00, -0.57, -0.14,  0.33, -0.22,
				 -0.43, -0.47, -0.06, -0.28, -0.21,  0.06,  0.05, -0.57,  1.00,  0.19,  0.10,  0.02,
				 -0.04, -0.01,  0.03, -0.22, -0.16, -0.08, -0.01, -0.14,  0.19,  1.00,  0.17,  0.21,
					0.23,  0.12, -0.28,  0.11,  0.20, -0.03, -0.02,  0.33,  0.10,  0.17,  1.00, -0.11,
				 -0.14, -0.04,  0.07, -0.07, -0.13, -0.09, -0.02, -0.22,  0.02,  0.21, -0.11,  1.00};

				corSystMatrix = TMatrixDSym( nObs, systCorrelations );

				break;
			}
		default:
			{
				 cout << "PDF_GLWADS_DKDpi_K3pi_newVars::setCorrelations() : ERROR : config "+ConfigToTString(c)+" not found." << endl;
				 exit(1);
			}
	}
}

void PDF_GLWADS_DKDpi_K3pi_newVars::buildPdf()
{
	pdf = new RooMultiVarGaussian("pdf_"+name, "pdf_"+name, *(RooArgSet*)observables, *(RooArgSet*)theory, covMatrix);
}
