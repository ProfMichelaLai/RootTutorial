#include <iostream>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "TF1.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TParameter.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TH1D.h"
#include "TH2D.h"


void toyMC_histograms() {
	//Use Monte Carlo numeric generators to fill a 1D histogram and print these on a file
    TRandom3 *ran = new TRandom3(12345);   // seed for reproducibility
    const int N = 100000;  // number of events

    // Parameters of the two Gaussians
    double mean1 = -2, sigma1 = 2.0;
    double mean2 = 2.0, sigma2 = 0.7;

    double p0 = 0.5; // probability of choosing Gaussian 1
    
    //Define three histograms, two 1Dimensional, one 2Dimensional
    TH1D *h = new TH1D("h", "Two merging Gaussians; x; Events", 200, -5, 5);
    TH1D *h2 = new TH1D("h2", "Two merging Gaussians; x; Events", 200, -5, 5);
    
    TH2D *h3 = new TH2D("h3", "2D Gaussian; x; y; Events", 200, -5, 5,  200, -5, 5 );

    for (int i = 0; i < N; i++) { //Loop to sample N (x,y) couple, ina toy-Monte Carlo fashion, with specific random distributions 
        double x, y;
        if (ran->Uniform() < p0){
            x = ran->Gaus(mean1, sigma1);
            y = ran->Gaus(mean1, sigma1);
        }
        else{
            x = ran->Gaus(mean2, sigma2);
            y = ran->Gaus(mean2, sigma2);

        } 
        h->Fill(x);  //Adds 1 event to the bin corresponding to x
        //h2->SetBinContent(i,x); //Directly sets the bin content
        
        //example to make the two histograms match 
        int bin = h2->FindBin(x);
		h2->SetBinContent(bin, h2->GetBinContent(bin) + 1);
		
		h3->Fill(x,y);
		
        
    }
    TCanvas *c1 = new TCanvas("c1", "Two Gaussians", 800, 600);
    h->Draw("histo");
    TCanvas *c2 = new TCanvas("c2", "Two Gaussians-bincontent", 800, 600);
    h2->Draw("histo");
    /////////////////////////////////////////////////////////
    
    TCanvas *c3 = new TCanvas("c3", "2D Gaussian", 800, 600);
    h3->Draw("COLZ");
    
    //////////////////////////////////////////////////////
    //With histograms, integrals and sums are extremely straightforward
    double x1 =0;
    double x2 = 1;
    int bin1 = h->GetXaxis()->FindBin(x1);
    int bin2 = h->GetXaxis()->FindBin(x2);
    double gaussian_right_area = h->Integral(bin1, bin2, "width"); // the actual integral, the sum of the bin contents x the bin widths
    double gaussian_right_sumcontents  = h->Integral(bin1, bin2) ; //the sum of the bin contents
    
    cout << "The integral of the histogram between " << x1<< " and " << x2 << " is " << gaussian_right_area << endl;    
    cout << "The sum of the histogram bins between " << x1<< " and " << x2 << " is " << gaussian_right_sumcontents << endl;    
    
    
    

    
    x1= 1;
    x2 = 5;
    double y1 = 1 ;
    double y2 = 5 ;
    
    int binx1 = h3->GetXaxis()->FindBin(x1);
    int binx2 = h3->GetXaxis()->FindBin(x2);
    int biny1 = h3->GetYaxis()->FindBin(y1); 
    int biny2 = h3->GetYaxis()->FindBin(y2);
    
    double area = h3->Integral(binx1, binx2, biny1, biny2, "width");
    cout << "The integral of the 2D histogram in our range is " << area << endl;    

    
    // Convert bin contents into density=bincontent/bin width
    h3->Scale(1.0, "width");
    // Now normalize the total integral to 1
    double total = h3->Integral(1, h3->GetNbinsX(),1, h3->GetNbinsY(),"width");
    h3->Scale(1.0 / total);
    cout << h3->Integral("width") << endl;
    //Now this is a Probability Density Function! 
    
    
    
    
    
    
    ///////////////////////////////////////////////////

    int k = 1;
    std::string fname = "fout_" + std::to_string(k) + ".root";

    TFile* fout = new TFile(fname.c_str(), "RECREATE");

    h->Write("h1D");
    h3->Write("h2D");
    fout->Close();
    


    cout << "Output file written as " << fname  << endl;
    
}

