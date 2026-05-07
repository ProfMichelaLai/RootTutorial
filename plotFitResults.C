#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"
#include <vector>
#include <iostream>

void plotFitResults() {

    gStyle->SetOptStat(0);

    // -----------------------------
    // Open the TTree
    // -----------------------------
    TFile *fin = TFile::Open("fit_results.root");
    if (!fin || fin->IsZombie()) {
        std::cout << "Error: cannot open fit_results.root" << std::endl;
        return;
    }

    TTree *t = (TTree*) fin->Get("fit");
    if (!t) {
        std::cout << "Error: TTree 'fit' not found" << std::endl;
        return;
    }

    // -----------------------------
    // Set branch addresses
    // -----------------------------
    int ybin, status;
    double ycenter;
    double par[6], err[6];
    double chi2_ndf;

    t->SetBranchAddress("ybin", &ybin);
    t->SetBranchAddress("ycenter", &ycenter);
    t->SetBranchAddress("par", par);
    t->SetBranchAddress("err", err);
    t->SetBranchAddress("chi2_ndf", &chi2_ndf);
    t->SetBranchAddress("status", &status);

    // -----------------------------
    // Histogram: chi2/ndf vs ybin
    // -----------------------------
    int Ny = t->GetEntries();
    TH2D *h_chi2 = new TH2D("h_chi2", "chi2/ndf vs ybin; ybin; chi2/ndf",
                            Ny, 0, Ny, 100, 0, 5);

    // -----------------------------
    // Graphs for parameters
    // -----------------------------
    std::vector<double> vx, vy[6], ve[6];

    // -----------------------------
    // Loop over entries
    // -----------------------------
    Long64_t N = t->GetEntries();

    for (Long64_t i = 0; i < N; i++) {
        t->GetEntry(i); //pick all the variables in the Tree, with the values in that row/entry

        if (status != 0) continue;   // keep only good fits

        // Fill chi2/ndf histogram
        h_chi2->Fill(ybin, chi2_ndf);

        // Fill parameter graphs
        vx.push_back(ybin);
        for (int p = 0; p < 6; p++) {
            vy[p].push_back(par[p]);
            ve[p].push_back(err[p]);
        }
    }

    // -----------------------------
    // Draw chi2/ndf vs ybin
    // -----------------------------
    TCanvas *c1 = new TCanvas("c1", "chi2/ndf vs ybin", 800, 600);
    h_chi2->Draw("COLZ"); 

    // -----------------------------
    // Draw parameter graphs
    // -----------------------------
    TCanvas *c2 = new TCanvas("c2", "Fit parameters vs ybin", 1200, 800);
    c2->Divide(3,2);

    for (int p = 0; p < 6; p++) {
        c2->cd(p+1);  //change the panel in the canvas

        TGraphErrors *g = new TGraphErrors(vx.size(), &vx[0],&vy[p][0],0, &ve[p][0]);
        //TGraphError with N points as the size of that vector, x , y for each pad, associated error
        g->SetTitle(Form("par[%d] vs ybin; ybin; par[%d]", p, p));
        g->SetMarkerStyle(20);
        g->SetMarkerSize(0.8);
        g->Draw("AP");
    }

    c1->SaveAs("chi2_ndf_vs_ybin.png");
    c2->SaveAs("parameters_vs_ybin.png");

    cout << "Plots saved." << endl;
}
