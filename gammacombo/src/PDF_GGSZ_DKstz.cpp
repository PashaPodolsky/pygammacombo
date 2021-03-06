/**
 * Gamma Combination
 * Author: M. Kenzie and A. Vallier (matthew.kenzie@cern.ch, avallier@cern.ch)
 * Date: June 2015
 *
 **/

#include "PDF_GGSZ_DKstz.h"

	PDF_GGSZ_DKstz::PDF_GGSZ_DKstz(config cObs, config cErr, config cCor, ParametersAbs* pars)
: PDF_Abs(4)
{
	if ( pars ) p = pars;
	else p = new ParametersGammaCombo();
	name = "ggsz_dkstz";
	initParameters();
	initRelations();
	initObservables();
	setObservables(cObs);
	setUncertainties(cErr);
	setCorrelations(cCor);
	buildCov();
	buildPdf();
}

PDF_GGSZ_DKstz::~PDF_GGSZ_DKstz(){}


void PDF_GGSZ_DKstz::initParameters()
{
	parameters = new RooArgList("parameters");
	parameters->add(*(p->get("r_dkstz")));
	parameters->add(*(p->get("d_dkstz")));
	parameters->add(*(p->get("g")));
  parameters->add(*(p->get("R_dkstz")));
  parameters->add(*(p->get("delta_dkstz")));
}


void PDF_GGSZ_DKstz::initRelations()
{
	theory = new RooArgList("theory"); ///< the order of this list must match that of the COR matrix!
	RooArgSet *p = (RooArgSet*)parameters;
	theory->add(*(new RooFormulaVar("xm_dkstz_th", "x- (DK*)", "(r_dkstz*R_dkstz)*cos((d_dkstz+delta_dkstz)-g)", *p)));
	theory->add(*(new RooFormulaVar("ym_dkstz_th", "y- (DK*)", "(r_dkstz*R_dkstz)*sin((d_dkstz+delta_dkstz)-g)", *p)));
	theory->add(*(new RooFormulaVar("xp_dkstz_th", "x+ (DK*)", "(r_dkstz*R_dkstz)*cos((d_dkstz+delta_dkstz)+g)", *p)));
	theory->add(*(new RooFormulaVar("yp_dkstz_th", "y+ (DK*)", "(r_dkstz*R_dkstz)*sin((d_dkstz+delta_dkstz)+g)", *p)));
	//theory->add(*(new RooFormulaVar("xm_dkstz_th", "x- (DK*)", "r_dkstz*cos(d_dkstz+delta_dkstz-g)", *p)));
	//theory->add(*(new RooFormulaVar("ym_dkstz_th", "y- (DK*)", "r_dkstz*sin(d_dkstz+delta_dkstz-g)", *p)));
	//theory->add(*(new RooFormulaVar("xp_dkstz_th", "x+ (DK*)", "r_dkstz*cos(d_dkstz+delta_dkstz+g)", *p)));
	//theory->add(*(new RooFormulaVar("yp_dkstz_th", "y+ (DK*)", "r_dkstz*sin(d_dkstz+delta_dkstz+g)", *p)));
}


void PDF_GGSZ_DKstz::initObservables()
{
	observables = new RooArgList("observables"); ///< the order of this list must match that of the COR matrix!
	observables->add(*(new RooRealVar("xm_dkstz_obs", "x- (DK* GGSZ) obs", 0, -1.5, 1.5)));
	observables->add(*(new RooRealVar("ym_dkstz_obs", "y- (DK* GGSZ) obs", 0, -1.5, 1.5)));
	observables->add(*(new RooRealVar("xp_dkstz_obs", "x+ (DK* GGSZ) obs", 0, -1.5, 1.5)));
	observables->add(*(new RooRealVar("yp_dkstz_obs", "y+ (DK* GGSZ) obs", 0, -1.5, 1.5)));
}


void PDF_GGSZ_DKstz::setObservables(config c)
{
	switch(c)
	{
		case truth:{
					   setObservablesTruth();
					   break;
				   }
		case toy:{
					 setObservablesToy();
					 break;
				 }
		case lumi3fb:{
						 obsValSource = "Alexis' estimate"; // pers. comm. 10/6/2015
						 setObservable("xm_dkstz_obs", -0.03);
						 setObservable("ym_dkstz_obs", -0.24);
						 setObservable("xp_dkstz_obs",0.16);
						 setObservable("yp_dkstz_obs",0.18);
						 break;
					 }
		case lhcb:{
						 obsValSource = "MI GGSZ from Sneha's email"; // Sneha's mail 22/01/16
						 setObservable("xm_dkstz_obs", -0.32);
						 setObservable("ym_dkstz_obs",  0.30);
						 setObservable("xp_dkstz_obs",  0.04);
						 setObservable("yp_dkstz_obs", -0.80);
						 break;
					 }
    // for HFAG
    case belle:{
						 obsValSource = "arXiv:1509.01098";
						 setObservable("xm_dkstz_obs", 0.4);
						 setObservable("ym_dkstz_obs",-0.6);
						 setObservable("xp_dkstz_obs", 0.1);
						 setObservable("yp_dkstz_obs", 0.3);
						 break;
           }
		default:{
					cout << "PDF_GGSZ_DKstz::setObservables() : ERROR : config not found: " << ConfigToTString(c) << endl;
					exit(1);
				}
	}
}


void PDF_GGSZ_DKstz::setUncertainties(config c)
{
	switch(c)
	{
		case lumi3fb:{
						 obsErrSource = "Alexis' estimate"; // pers. comm. 10/6/2015
						 StatErr[0] = 0.15; // xm
						 StatErr[1] = 0.19; // ym
						 StatErr[2] = 0.16; // xp
						 StatErr[3] = 0.19; // yp
						 SystErr[0] = 0.052;// xm
						 SystErr[1] = 0.043;// ym
						 SystErr[2] = 0.045;// xp
						 SystErr[3] = 0.060;// yp
						 break;
						  }
		case lhcb:{
						 obsErrSource = "MI GGSZ from Sneha's email"; // Sneha's mail 22/01/16
						 StatErr[0] = 0.20; // xm
						 StatErr[1] = 0.24; // ym
						 StatErr[2] = 0.34; // xp
						 StatErr[3] = 0.31; // yp

             SystErr[0] = 0.04;// xm
						 SystErr[1] = 0.05;// ym
						 SystErr[2] = 0.04;// xp
						 SystErr[3] = 0.06;// yp
						 break;
						  }
    // for HFAG
		case belle:{
						 obsErrSource =  "arXiv:1509.01098";
						 StatErr[0] = 0.8; // xm
						 StatErr[1] = 0.9; // ym
						 StatErr[2] = 0.55; // xp
						 StatErr[3] = 0.65; // yp
						 SystErr[0] = sqrt( sq(0.1)+sq(0.0) );// xm
						 SystErr[1] = sqrt( sq(0.1)+sq(0.1) );// ym
						 SystErr[2] = sqrt( sq(0.1)+sq(0.1) );// xp
						 SystErr[3] = sqrt( sq(0.1)+sq(0.1) );// yp
						 break;
						  }
		default:{
					cout << "PDF_GGSZ_DKstz::setUncertainties() : ERROR : config not found: " << ConfigToTString(c) << endl;
					exit(1);
				}
	}
}


void PDF_GGSZ_DKstz::setCorrelations(config c)
{
	resetCorrelations();
	switch(c)
	{
    case lumi3fb: {
							  corSource = "no correlations";
                break;
              }
    case lumi3fb_estimate:{
							  corSource = "estimate";
							  //                     xm      ym      xp      yp
							  double dataStat[]  = { 1.   , -0.247,  0.038, -0.003,		// xm
								                      -0.247,  1.   , -0.011,  0.012,		// ym
								                       0.038, -0.011,  1.   ,  0.002,		// xp
								                      -0.003,  0.012,  0.002,  1.     };// yp
							  corStatMatrix = TMatrixDSym(nObs,dataStat);
							  //                     xm      ym      xp      yp
							  double dataSyst[]  = { 1.   ,  0.005, -0.025,  0.070,   // xm
								                       0.005,  1.   ,  0.009, -0.141,   // ym
								                      -0.025,  0.009,  1.   ,  0.008,   // xp
								                       0.070, -0.141,  0.008,  1.     };// yp
							  corSystMatrix = TMatrixDSym(nObs,dataSyst);
							  break;
						  }
    case lhcb:{
							  corSource = "MI GGSZ from Sneha's email"; // Sneha's mail 22/01/16
							  //                     xm      ym      xp      yp
							  double dataStat[]  = { 1.   ,  0.13 ,  0.00 , -0.01 ,		// xm
								                       0.13 ,  1.   , -0.01 ,  0.02 ,		// ym
								                       0.00 , -0.01 ,  1.   ,  0.13 ,		// xp
								                      -0.01 ,  0.02 ,  0.13 ,  1.     };// yp
							  corStatMatrix = TMatrixDSym(nObs,dataStat);
							  //                     xm      ym      xp      yp
							  double dataSyst[]  = { 1.   ,  0.00 ,  0.00 ,  0.00 ,   // xm
								                       0.00 ,  1.   ,  0.00 ,  0.00 ,   // ym
								                       0.00 ,  0.00 ,  1.   ,  0.00 ,   // xp
								                       0.00 ,  0.00 ,  0.00 ,  1.     };// yp
							  corSystMatrix = TMatrixDSym(nObs,dataSyst);
							  break;
						  }
    // for HFAG
    case belle:{
							  corSource = "estimate";
							  //                     xm      ym      xp      yp
							  double dataStat[]  = { 1.  ,  0.07,  0.00, -0.00,		// xm
								                       0.07,  1.  , -0.00,  0.00,		// ym
								                       0.00,  0.00,  1.  , -0.07,		// xp
								                       0.00,  0.00, -0.07,  1.    };// yp
							  corStatMatrix = TMatrixDSym(nObs,dataStat);
							  //                     xm      ym      xp      yp
							  double dataSyst[]  = { 1.  ,  0.00,  0.00,  0.00,   // xm
								                       0.00,  1.  ,  0.00,  0.00,   // ym
								                       0.00,  0.00,  1.  ,  0.00,   // xp
								                       0.00,  0.00,  0.00,  1.    };// yp
							  corSystMatrix = TMatrixDSym(nObs,dataSyst);
							  break;
						  }
		default:{
					cout << "PDF_GGSZ::setCorrelations() : ERROR : config not found: " << ConfigToTString(c) << endl;
					exit(1);
				}
	}
}


void PDF_GGSZ_DKstz::buildPdf()
{
	pdf = new RooMultiVarGaussian("pdf_"+name, "pdf_"+name, *(RooArgSet*)observables, *(RooArgSet*)theory, covMatrix);
}
