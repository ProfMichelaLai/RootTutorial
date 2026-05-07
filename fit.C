#include <iostream>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "TFitResult.h"
#include "TF1.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TParameter.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMatrixDSym.h"
#include "TFitResultPtr.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TStyle.h"

double gaussians(double *x, double *p) {
    return p[0] * TMath::Gaus(x[0], p[1], p[2]) +
           p[3] * TMath::Gaus(x[0], p[4], p[5]);
}

void fit() {

    // Open file
    TFile *f = TFile::Open("fout_1.root", "READ");
    if (!f || f->IsZombie()) {
        std::cout << "Error: cannot open the ROOT file" << std::endl;
        return;
    }

    // Retrieve histograms
    TH1D *h1D = (TH1D*) f->Get("h1D");
    TH2D *h2D = (TH2D*) f->Get("h2D");

    if (!h1D || !h2D) {
        std::cout << "Error: one or more histograms not found" << std::endl;
        return;
    }

    gStyle->SetOptFit(1111);

    TCanvas *c1 = new TCanvas("c1", "c1", 900, 700);
    c1->SetGrid();

    h1D->Draw("histo");

    // Define double Gaussian
    TF1 *fgaussians = new TF1("fgaussians", gaussians, -5, 5, 6);
    fgaussians->SetParameters(100, -2, 1, 200, 2, 0.5);
    fgaussians->SetParNames("A1", "Mean1", "Sigma1",
                            "A2", "Mean2", "Sigma2");

    // Perform fit
    TFitResultPtr r = h1D->Fit(fgaussians, "S"); //S uses chi-square as parameter to be optimized

    // Check fit status
    if (r->Status() != 0) {
        std::cout << "WARNING: Fit failed with status = "
                  << r->Status() << std::endl;
    }

    // Print parameters after the fit optimization
    int npar = fgaussians->GetNpar();
    for (int i = 0; i < npar; i++) {
        std::cout << fgaussians->GetParName(i) << ": "
                  << r->Parameter(i) << " +/- "
                  << r->ParError(i) << std::endl;
    }

    // print covariance matrix
     r->GetCovarianceMatrix().Print();

    // print correlation matrix
    r->GetCorrelationMatrix().Print();
    fgaussians->SetLineColor(kRed);
    fgaussians->SetLineWidth(3);
    fgaussians->Draw("same");  //draw the function with the optimized parameters, added by Fit
    c1->Update();

}
