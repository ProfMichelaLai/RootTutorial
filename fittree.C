#include <iostream>
#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TF1.h"
#include "TTree.h"
#include "TMatrixDSym.h"
#include "TFitResultPtr.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"

double gaussians(double *x, double *p) {
    return p[0] * TMath::Gaus(x[0], p[1], p[2]) +
           p[3] * TMath::Gaus(x[0], p[4], p[5]);
}

void fittree() {

    gStyle->SetOptFit(0);

    // -----------------------------
    // Open input file and TH2D
    // -----------------------------
    TFile *fin = TFile::Open("fout_1.root", "READ");
    if (!fin || fin->IsZombie()) {
        std::cout << "Error: cannot open input file" << std::endl;
        return;
    }

    TH2D *h2D = (TH2D*) fin->Get("h2D");
    if (!h2D) {
        std::cout << "Error: TH2D not found" << std::endl;
        return;
    }

    // -----------------------------
    // We now setup our TTree structure, as well as the TFile which will store the TTree
    // -----------------------------
    TFile *fout = new TFile("fit_results.root", "RECREATE");
    TTree *t = new TTree("fit", "Slice-by-slice Gaussian fits");

    // Fit results
    double par[6], err[6];
    double chi2, ndf, chi2_ndf, edm; //chi-squared, degrees of freedom, reduced chi-squared, Estimated Distance to Minimum of the chi-square
    int status, ncalls;  //status = 0 means fit converged well, ncalls, how many times the chi-squared was evaluated to get in that status

    // Covariance and correlation matrices
    double cov[36];
    double corr[36];
    
        // Y-bin info
    int ybin;
    double ycenter;


    // Branches
    t->Branch("ybin", &ybin, "ybin/I");
    t->Branch("ycenter", &ycenter, "ycenter/D");

    t->Branch("par", par, "par[6]/D");
    t->Branch("err", err, "err[6]/D");

    t->Branch("chi2", &chi2, "chi2/D");
    t->Branch("ndf", &ndf, "ndf/D");
    t->Branch("chi2_ndf", &chi2_ndf, "chi2_ndf/D");
    t->Branch("edm", &edm, "edm/D");
    t->Branch("status", &status, "status/I");
    t->Branch("ncalls", &ncalls, "ncalls/I");

    t->Branch("cov", cov, "cov[36]/D");
    t->Branch("corr", corr, "corr[36]/D");


    // -----------------------------
    // Loop over Y bins
    // -----------------------------
    h2D->Rebin2D(4);
    int Ny = h2D->GetNbinsY();


    for (int iy = 1; iy <= Ny; iy++) {

        ybin = iy;
        ycenter = h2D->GetYaxis()->GetBinCenter(iy);

        // ProjectionX for this Y-bin
        TH1D *hpx = h2D->ProjectionX(Form("hpx_%d", iy), iy, iy);


        // Fit function
        TF1 *fgaus = new TF1("fgaus", gaussians, -5, 5, 6);
        fgaus->SetParameters(0.4, -2, 1, 0.7, 2, 0.5);

        // Fit quietly
        TFitResultPtr r = hpx->Fit(fgaus, "S Q");

        // Store parameters
        for (int p = 0; p < 6; p++) {
            par[p] = r->Parameter(p);
            err[p] = r->ParError(p);
        }

        chi2      = r->Chi2();
        ndf       = r->Ndf();
        chi2_ndf  = (ndf > 0 ? chi2 / ndf : -1);
        edm       = r->Edm();
        status    = r->Status();
        ncalls    = r->NCalls();

        // Covariance and correlation matrices
        TMatrixDSym covM  = r->GetCovarianceMatrix();
        TMatrixDSym corrM = r->GetCorrelationMatrix();

        int k = 0;
        for (int a = 0; a < 6; a++) {
            for (int b = 0; b < 6; b++) {
                cov[k]  = covM(a,b);
                corr[k] = corrM(a,b);
                k++;
            }
        }


        // Fill TTree entry
        t->Fill();

        delete hpx;
        delete fgaus;
    }

    // -----------------------------
    t->Write();
    fout->Close();

    std::cout << "Done. Fit results saved to fit_results.root" << std::endl;
}
