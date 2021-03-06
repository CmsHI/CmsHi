
/** \class HHistogram  Ganzhur/HafHistogram/src/HHistogram.cc
 *
 * Description:
 * Analysis code of the CMS experiment;
 * Original version is a part of HAF package developed for CMS: UserCode/HAF  
 * Histogram class
 *
 * \author Marcel Kunze,        Ruhr-University Bochum, Germany 
 * \author Serguei Ganjour,     CEA-Saclay/IRFU, FR
 *
 * \version $Id: HHistogram.cc,v 1.5 2010/01/07 10:44:17 ganzhur Exp $
 *
 */

#include <iostream>

using namespace std;

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TAxis.h"
#include "TDirectory.h"

#include "UserCode/HafHistogram/interface/HHistID.h"
#include "UserCode/HafHistogram/interface/HHistogram.h"

ClassImp(HHistogram)
ClassImp(HMassHistogram)
ClassImp(HEnergyHistogram)
ClassImp(HMomentumHistogram)
ClassImp(HEoverPHistogram)
ClassImp(HMoverPHistogram)
ClassImp(HDalitzPlot)

// Constructors:
// 1-D histo:
HHistogram::HHistogram( const char* hname, const char* htitle, 
			       Int_t nbins, Axis_t lowX, Axis_t highX ) :
TNamed( hname,htitle )
{   
    // Create a 1-D ROOT histo:
    histp= new TH1F( hname, htitle, nbins, lowX, highX );    
    histp->SetDirectory(gDirectory);
}

HHistogram::HHistogram( const char* hname, const char* htitle, 
			       Int_t nbinsX, Axis_t lowX, Axis_t highX,
			       Int_t nbinsY, Axis_t lowY, Axis_t highY ) :
TNamed( hname,htitle )
{    
    // Create a 2-D ROOT histo:
    histp= new TH2F( hname, htitle, nbinsX, lowX, highX, nbinsY, lowY, highY );   
    histp->SetDirectory(gDirectory);
}

HHistogram::HHistogram( const char* hname, const char* htitle, 
			       Int_t nbins, Axis_t lowX, Axis_t highX, 
			       Axis_t lowY, Axis_t highY ) :
TNamed( hname,htitle )
{   
    // Create a 1-D profile ROOT histo with default error calculation:
    // error= sigma/sqrt(N)
    histp= new TProfile( (char*)hname, (char*)htitle, nbins, lowX, highX,lowY, highY );    
}

HHistogram::~HHistogram() 
{
    delete histp;
}

void HHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    if( histp->GetDimension() == 2 ||
	( histp->GetDimension() == 1 && 
	histp->IsA() == TProfile::Class() ) ) {
	Axis_t y = weight;
	weight = 1.0;
	((TH2 *)histp)->Fill( x, y, weight );
    }
    else {
	histp->Fill( x, weight );
    }
    return;    
}

void HHistogram::Accumulate1( Axis_t x, Stat_t weight ) 
{    
    histp->Fill( x, weight );
}

void HHistogram::Accumulate( Axis_t x, Axis_t y, Stat_t weight ) 
{    
    ((TH2 *)histp)->Fill( x, y, weight );   
}

void HHistogram::Accumulate2( Axis_t x, Axis_t y, Stat_t weight ) 
{   
    ((TH2 *)histp)->Fill( x, y, weight );     
}


void HHistogram::Reset() 
{
    histp->Reset();
}


Float_t HHistogram::GetContents( Int_t nbinsX, Int_t nbinsY ) const 
{  
    Float_t cont= 0;
    if( nbinsY == 0 ) {
	cont= (Float_t) histp->GetBinContent( nbinsX );
    }
    else {
	cont= (Float_t) histp->GetCellContent( nbinsX, nbinsY );
    }
    return cont;    
}

Float_t HHistogram::GetErrors( Int_t nbinsX, Int_t nbinsY ) const 
{   
    Float_t error= 0;
    if( nbinsY == 0 ) {
	error = (Float_t) histp->GetBinError( nbinsX );
    }
    else {
	error = (Float_t) histp->GetCellError( nbinsX, nbinsY );
    }
    return error;   
}



Int_t HHistogram::GetEntries() const { return (Int_t) histp->GetEntries(); }

Float_t HHistogram::GetWtSum() const 
{    
    return (Float_t) histp->GetSumOfWeights();    
}



Int_t HHistogram::GetNbins( Int_t theDim ) const {
    
    Int_t nbins= 0;
    if( theDim == 0 ) {
	nbins= histp->GetNbinsX();
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) nbins= histp->GetNbinsY();
    }
    return nbins;
    
}

Float_t HHistogram::GetLow( Int_t theDim ) const {
    
    Float_t low= 0;
    if( theDim == 0 ) {
	low= histp->GetXaxis()->GetBinLowEdge( 1 );
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) {
	    low= histp->GetYaxis()->GetBinLowEdge( 1 );
	}
    }
    return low;
    
}

Float_t HHistogram::GetHigh( Int_t theDim ) const {
    
    TAxis* axisp= 0;
    if( theDim == 0 ) {
	axisp= histp->GetXaxis();
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) axisp= histp->GetYaxis();
    }
    Float_t high= 0;
    if( axisp != 0 ) {
	Int_t n= axisp->GetNbins();
	high= axisp->GetBinLowEdge( n ) + axisp->GetBinWidth( n );
    }
    return high;
    
}

Float_t HHistogram::GetAvg( Int_t theDim ) const {
    
    Float_t mean= 0;
    if( theDim == 0 ) {
	mean= (Float_t) histp->GetMean( 1 );
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) mean= (Float_t) histp->GetMean( 2 );
    }
    return mean;
    
}

Float_t HHistogram::GetCovar( Int_t dim1, Int_t dim2 ) const {
    
    // Calculate weighted sums:
    TAxis* xaxisp= histp->GetXaxis();
    TAxis* yaxisp= histp->GetYaxis();
    Int_t maxxbin= histp->GetNbinsX();
    Int_t maxybin= histp->GetNbinsY();
    Double_t meanx= 0;
    Double_t meany= 0;
    Double_t meanx2= 0;
    Double_t meany2= 0;
    Double_t meanxy= 0;
    for( Int_t ybin= 1; ybin <= maxybin; ++ybin ) {
	Double_t cony= 0;
	for( Int_t xbin= 1; xbin <= maxxbin; ++xbin ) {
	    Double_t cellc= histp->GetCellContent( xbin, ybin );
	    cony+= cellc;
	    Double_t xbinc= xaxisp->GetBinCenter( xbin );
	    meanx+= cellc * xbinc;
	    meanx2+= cellc * xbinc*xbinc;
	    meanxy+= cellc * xbinc * yaxisp->GetBinCenter( ybin );
	}
	Double_t ybinc= yaxisp->GetBinCenter( ybin );
	meany+= cony * ybinc;
	meany2+= cony * ybinc*ybinc;
    }
    Double_t sumcon= histp->GetSumOfWeights();
    if( sumcon ) {
	meanx/= sumcon;
	meany/= sumcon;
	meanx2/= sumcon;
	meany2/= sumcon;
	meanxy/= sumcon;
    }
    else {
	meanx= 0;
	meany= 0;
	meanx2= 0;
	meany2= 0;
	meanxy= 0;
    }
    
    // Return covariance between dim1 and dim2:
    Float_t cov= 0;
    if( dim1 == 0 && dim2 == 0 ) {
	cov = (Float_t) (meanx2 - meanx*meanx);
    }
    else if( (dim1 == 0 && dim2 == 1) || 
	     (dim1 == 1 && dim2 == 0 ) ) {
	cov = (Float_t) (meanxy - meanx*meany);
    }
    else if( dim1 == 1 && dim2 == 1 ) {
	cov = (Float_t) (meany2 - meany*meany);
    }
    return cov;
    
}

Int_t HHistogram::GetType() const {
    
    Int_t ht= 0;
    if( histp->GetDimension() == 1 ) ht= 1;
    else if( histp->GetDimension() == 2 ) ht= 2;
    return ht;
    
}

HHistID HHistogram::GetHistID() const {
    
    return HHistID( histp->GetName() );
    
}

Bool_t HHistogram::PtrIsEqual( TObject* ptr ) const {
    
    Bool_t result;
    if( (TObject*) histp == ptr ) result= kTRUE; 
    else result= kFALSE;
    return result;
    
}

HHistogram& HHistogram::operator<<(Axis_t x)
{
    Accumulate(x); return *this;
}


//----------------Spezialization-----------------

//----------------
// Constructors --
//----------------

HMassHistogram::HMassHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup)  
: HHistogram(name,title,nbins,xlow,xup)
{
}

HEnergyHistogram::HEnergyHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup)  
: HHistogram(name,title,nbins,xlow,xup)
{
}

HMomentumHistogram::HMomentumHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup)  
: HHistogram(name,title,nbins,xlow,xup)
{
}

HEoverPHistogram::HEoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
					   ,Int_t nbinsy,Axis_t ylow,Axis_t yup)
					   : HHistogram(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup)
{
}

HMoverPHistogram::HMoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
					   ,Int_t nbinsy,Axis_t ylow,Axis_t yup)
					   : HHistogram(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup)
{
}

//--------------
// Destructor --
//--------------

HMassHistogram::~HMassHistogram( ) 
{
}

HEnergyHistogram::~HEnergyHistogram( ) 
{
}

HMomentumHistogram::~HMomentumHistogram( ) 
{
}

HEoverPHistogram::~HEoverPHistogram( ) 
{
}

HMoverPHistogram::~HMoverPHistogram( ) 
{
}

HDalitzPlot::~HDalitzPlot( ) 
{
}

//----------------------
//-- public Functions --
//----------------------

void HMassHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    Accumulate1( x, weight );
}


void HEnergyHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    Accumulate1( x, weight );
}

void HMomentumHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    Accumulate1( x, weight );
}


void HEoverPHistogram::Accumulate( Axis_t x, Axis_t y, Stat_t weight ) 
{    
    Accumulate2( x, y, weight );
}

void HMoverPHistogram::Accumulate( Axis_t x, Axis_t y, Stat_t weight ) 
{    
    Accumulate2( x, y, weight );
}


HDalitzPlot::HDalitzPlot(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
				 ,Int_t nbinsy,Axis_t ylow,Axis_t yup)
				 : HHistogram(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup)
{
}

/*
* This function fills a Dalitzplot for the particles name1,name2 and name3 from descriptor p.
* horizontal axis: (name1,name2), vertikal axis: (name2,name3)
* There are three cases:
* 1) Fill the plot for three identical particles (6 entries)
* 2) Fill the plot for two different particles (2 entries)
* 3) Fill the plot for three different particles (1 entry)
*/

