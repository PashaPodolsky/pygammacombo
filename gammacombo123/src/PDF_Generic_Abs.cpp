/**
 * Gamma Combination
 * Author: Maximilian Schlupp, max.schlupp@cern.ch
 * Date: December 2013
 *
 *  
 **/

#include "PDF_Generic_Abs.h"


PDF_Generic_Abs::PDF_Generic_Abs(RooWorkspace* w, int nObs, OptParser* opt) 
: PDF_Abs(nObs){
  observables     = NULL;
  parameters      = NULL;
  wspc            = w;//new RooWorkspace(*w);
  pdfName         = "notSet";
  globalVarsName  = "notSet";
  constraintName  = "notSet";
  areObsSet       = areParsSet = areRangesSet = isPdfSet = isDataSet = isToyDataSet = false;
  arg             = opt;
  fitStatus       = -10;
  globVals        = NULL;
  _constraintPdf  = NULL;
  _NLL            = NULL;
  minNllFree      = 0;
  minNllScan      = 0;
  minNll          = 0;
};

PDF_Generic_Abs::~PDF_Generic_Abs(){
  delete wspc;
};
/*
///
/// Return RooArgSet that contains all parameters.
/// 
const RooArgSet* PDF_Generic_Abs::getParameters()
{
  return wspc->set(parName);
}

///
/// Return RooArgSet that contains all observables.
/// 
const RooArgSet* PDF_Generic_Abs::getObservables()
{
  return wspc->set(obsName);
}
*/

void  PDF_Generic_Abs::initObservables(const TString& setName){
    if( areObservablesSet() ){ 
    std::cout << "WARNING in PDF_Generic_Abs::initObservables -- Observables already set" << std::endl; 
    return;
  }
  if(! isPdfInitialized() ){
    std::cout << "FATAL in PDF_Generic_Abs::initObservables -- first call PDF_Generic_Abs::initPdf to init the PDF!" << std::endl;
    exit(-1);
  }
  wspc->renameSet(setName,obsName);
  observables = (RooArgList*) wspc->set(obsName);
  areObsSet = true;
};

void  PDF_Generic_Abs::initParameters(const TString& setName){
  if( areParametersSet() ){ 
    std::cout << "WARNING in PDF_Generic_Abs::initParameters -- Parameters already set" << std::endl; 
    return;
  }
  if(! isPdfInitialized() ){
    std::cout << "FATAL in PDF_Generic_Abs::initParameters -- first call PDF_Generic_Abs::initPdf to init the PDF!" << std::endl;
    exit(-1);
  }
  wspc->renameSet(setName,parName);
  parameters = (RooArgList*) wspc->set(parName);
  areParsSet = true;
};

void  PDF_Generic_Abs::initObservables(const vector<TString>& obsNames){
  if( areObservablesSet() ){ 
    std::cout << "WARNING in PDF_Generic_Abs::initObservables -- Observables already set" << std::endl; 
    return;
  }
  if(! isPdfInitialized() ){
    std::cout << "FATAL in PDF_Generic_Abs::initObservables -- first call PDF_Generic_Abs::initPdf to init the PDF!" << std::endl;
    exit(-1);
  }
  observables = new RooArgList("observables");
  Utils::fillArgList(observables, wspc, obsNames);
  wspc->defineSet(obsName,*observables);//, RooFit::Silence());
  areObsSet = true;
};

void  PDF_Generic_Abs::initParameters(const vector<TString>& parNames){
  if( areParametersSet() ){ 
    std::cout << "WARNING in PDF_Generic_Abs::initParameters -- Parameters already set" << std::endl; 
    return;
  }
  if(! isPdfInitialized() ){
    std::cout << "FATAL in PDF_Generic_Abs::initParameters -- first call PDF_Generic_Abs::initPdf to init the PDF!" << std::endl;
    exit(-1);
  }
  parameters = new RooArgList("parameters");
  Utils::fillArgList(parameters, wspc, parNames);
  wspc->defineSet(parName, *parameters);//, RooFit::Silence());
  areParsSet = true;
  if(arg->debug) std::cout << "DEBUG in PDF_Generic_Abs::initParameters --pars filled" << std::endl;
};

void PDF_Generic_Abs::initPDF(const TString& name){
  if(isPdfSet){
    std::cout << "WARNING in PDF_Generic_Abs::initPDF -- PDF already set" << std::endl; 
    std::cout << "WARNING in PDF_Generic_Abs::initPDF -- PDF will not be overwritten" << std::endl; 
    std::cout << "WARNING in PDF_Generic_Abs::initPDF -- !!!" << std::endl; 
    return;
  }
  pdfName   = name;
  pdf       = wspc->pdf(pdfName);
  isPdfSet  = true;
  obsName    = "obs_"+pdfName;
  parName    = "par_"+pdfName;

  std::cout << "INFO in PDF_Generic_Abs::initPDF -- PDF initialized" << std::endl;
  return;
};

void PDF_Generic_Abs::setVarRange(const TString &varName, const TString &rangeName, 
                                  const double &rangeMin, const double &rangeMax){
  RooRealVar* var = wspc->var(varName);
  if(!var){
    std::cout << "ERROR in PDF_Generic_Abs::setVarRange -- No Var with Name: " 
              << varName << " found!!" << std::endl;
    return;
  }
  if(!(rangeName== "free" || rangeName== "phys" || rangeName == "scan" || rangeName == "bboos" || rangeName == "force")){
    std::cout << "ERROR in PDF_Generic_Abs::setVarRange -- UNKNOWN range name! -- return" << std::endl;
  }
  RooMsgService::instance().setGlobalKillBelow(ERROR);
  if(rangeMin == rangeMax){
    std::cout << "WARNING in PDF_Generic_Abs::setVarRange -- rangeMin == rangeMax! If you want to set parameter constant "
              << "use e.g. RooRealVar::setConstant. Expect crash in CL calculation!" << std::endl;
  }
  var->setRange(rangeName, rangeMin, rangeMax);
  RooMsgService::instance().setGlobalKillBelow(INFO);
};

void PDF_Generic_Abs::setToyData(RooDataSet* ds){
  toyObservables  = ds; 
  isToyDataSet    = kTRUE;
  return;
};

void PDF_Generic_Abs::print(){
  if(isPdfSet){
    std::cout << "PDF:\t" << this->getPdfName() << std::endl;
  }
  if(wspc){
    std::cout << "Worspace:\t" << std::endl;
    wspc->Print();
  }
  return;
};
