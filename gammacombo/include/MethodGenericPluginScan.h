/*
 * Gamma Combination
 * Author: Maximilian Schlupp, max.schlupp@cern.ch
 * Date: November 2013
 * 
 * Class MethodGenericPluginScan - implements a FC plugin scan using dataset fits
 * Instead of extracting physics parameters from experimental observables the 
 * MethodGenericPluginScan can be used to directly calculate FC based errors 
 * from the provided PDF. The PDF needs to be able to generate and fit toys. 
 * If you want to implement your own use-case, see the GaussianPlusExp example.
 *
 * Unusual output in the ToyTree:
 *          - nBergerBoos tracks violation of FC boundaries for the free fit
 *          - BergerBoos_id tracks violation of FC boundaries for the scan fit
 *          - id tracks if FC boundaries get violated twice 
 */

#ifndef MethodGenericPluginScan_h
#define MethodGenericPluginScan_h

#include "MethodPluginScan.h"
#include "PDF_Generic_Abs.h"
#include "RooSlimFitResult.h"
#include "TLeaf.h"
#include "TBranch.h"

class MethodGenericPluginScan : public MethodPluginScan
{
public:
    MethodGenericPluginScan(PDF_Generic_Abs* PDF, OptParser* opt, 
                            bool provideFitResult = false, RooFitResult* result = 0);
    void                drawDebugPlots(int runMin, int runMax, TString fileNameBaseIn = "default");
    float               getParValAtScanpoint(float point, TString parName);
    MethodProbScan*     getProfileLH(){return this->profileLH;};
    virtual void        initScan();
    void                loadParameterLimits();
    void                loadPLHPoint(float point, int index=-1);
    void                loadPLHPoint(int index);
    inline  void        performProbScanOnly(bool yesNo=true){doProbScanOnly = yesNo;};
    void                performBootstrapTest(int nSamples=1000, const TString& ext ="");
    virtual void        print();
    void                printDebug(const RooFitResult& r);
    TChain*             readFiles(int runMin, int runMax, int &nFilesRead, int &nFilesMissing, TString fileNameBaseIn = "default");
    void                readScan1dTrees(int runMin, int runMax, TString fileNameBaseIn = "default");
    virtual int         scan1d(int nRun=1);
    inline  void        setInputFile(TString name){inputFiles.push_back(name); explicitInputFile=true;};
    inline  void        setExtProfileLH(TTree* tree){profileLHPoints = tree; externalProfileLH = true;};
    inline  void        addFile(TString name){inputFiles.push_back(name);};

    PDF_Generic_Abs*        pdf;
    TH1F*                   probPValues;
    TTree*                  profileLHPoints;
    bool                    drawPlots;
    bool                    explicitInputFile;
    bool                    doProbScanOnly;
    bool                    externalProfileLH;
    std::vector<TString>    inputFiles;
    std::vector<double>     bootstrapPVals;
    TChain*                 chain;
    RooFitResult*           dataFreeFitResult;
};

#endif
