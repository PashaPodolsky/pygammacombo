/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOGLWCARTCOORDVAR
#define ROOGLWCARTCOORDVAR

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooGlwCartCoordVar : public RooAbsReal {
public:
  enum GlwConfig { acpp, rcpp };
  RooGlwCartCoordVar() {} ; 
  RooGlwCartCoordVar(const char *name, const char *title,
	      GlwConfig _c,
	      RooAbsReal& _xp,
	      RooAbsReal& _xm,
	      RooAbsReal& _yp,
	      RooAbsReal& _ym);
  RooGlwCartCoordVar(const RooGlwCartCoordVar& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooGlwCartCoordVar(*this,newname); }
  virtual ~RooGlwCartCoordVar();

protected:

  RooRealProxy xp;
  RooRealProxy xm;
  RooRealProxy yp;
  RooRealProxy ym;
  GlwConfig c;
 
  Double_t evaluate() const;

private:
  ClassDef(RooGlwCartCoordVar, 1);
};
 
#endif
