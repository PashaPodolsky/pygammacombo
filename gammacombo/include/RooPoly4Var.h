/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOPOLY4VAR
#define ROOPOLY4VAR

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooPoly4Var : public RooAbsReal {
public:
  RooPoly4Var() {} ; 
  RooPoly4Var(const char *name, const char *title,
	      RooAbsReal& _xobs,
	      double& _p0,
	      double& _p1,
	      double& _p2,
	      double& _p3,
	      double& _p4);
  RooPoly4Var(const RooPoly4Var& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooPoly4Var(*this,newname); }
  virtual ~RooPoly4Var();

protected:

  RooRealProxy xobs;
  double p0;
  double p1;
  double p2;
  double p3;
  double p4;
  
  Double_t evaluate() const;

private:
  ClassDef(RooPoly4Var, 1);
};
 
#endif
