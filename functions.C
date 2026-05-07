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


//We here define any global variable that we wish to call everywhere in this macro

double gaussian1 (double *x, double *p ) {
   return TMath::Gaus(x[0],p[0],p[1] );  
   //x is the vector of the variables. This is 1D Gaussian. 
   //p is the vector of its parameters. A gaussian will be defined by the mean p[0] and the standard deviation p[1]
}
 

void functions(){
    cout << "This example will teach how to create, adjust functions on a canvas" << endl; //that's how you print on the terminal
	
	double xmin = -20;  double xmax = 20;  
 
    TCanvas *c1 = new TCanvas("c1", "Energy Deposition Comparison", 900, 700);
    c1->SetGrid();
    // create TF1 using a free C function
   TF1 *f1 = new TF1("Gaussian-Left",gaussian1,xmin,xmax,2);
   f1->SetParameters(-10,5);
   
   f1->SetLineColor(kRed);
   f1->SetLineStyle(2);
   f1->SetLineWidth(2);
   
   
   TF1 *f2 = new TF1("Gaussian-Right",gaussian1,xmin,xmax,2);
   f2->SetParameters(10,2);  
   f2->SetLineColor(kBlue);
   f2->SetLineStyle(2);
   f2->SetLineWidth(2);
   
   
    // Sum two functions
    TF1 *fsum = new TF1("fsum",[f1, f2](double *x, double *p){return f1->Eval(x[0]) + f2->Eval(x[0]) + 0.2;},xmin, xmax, 0);
    fsum->SetLineColor(kGreen);
    fsum->SetLineWidth(5);
    fsum->SetTitle("Examples with Gaussians;X axis title;Y axis title");
    
    fsum->Draw();
    f1->Draw("same");
    f2->Draw("same");
   

   
   // Legend
   TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
   leg->AddEntry(f1, "Gaussian-Left", "l");
   leg->AddEntry(f2,  "Gaussian-Right", "l");
   leg->AddEntry(fsum, "Sum", "l");
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->Draw();
    c1->Modified();
    c1->Update();
   

 
	
}