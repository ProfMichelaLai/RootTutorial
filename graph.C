#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TString.h>
#include <TStyle.h>

#include <iostream>
#include <fstream>
#include <vector>

void graph() {

    // Folder containing the files
    TString folder = "./data/";   // <-- change if needed

    // File names, defined as vector of TStrings
    std::vector<TString> files = {
        "LArscint.csv",
        "TPBadsorb.csv",
        "TPBemission.csv"
    };

    // Legend labels (same as files)
    std::vector<TString> labels = {
        "LAr Scintillation Spectrum",
        "TPB Absorption",
        "TPB Emission"
    };

    // Fancy Colors for each graph
    int colors[3] = {kRed, kBlue, kGreen};

    // Create a multigraph, the best tool to have multiple TGraphs all plotted together
    TMultiGraph *mg = new TMultiGraph();

    // Legend
    TLegend *leg = new TLegend(0.10, 0.60, 0.40, 0.90);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    // Loop over files.
    for (int i = 0; i < files.size(); i++) {

        TString path = folder + files[i]; 
        std::ifstream infile(path.Data());

        if (!infile.is_open()) {
            std::cerr << "Error: Cannot open " << path << std::endl;
            continue;
        }

        std::vector<double> wavelength;  
        std::vector<double> efficiency;

        double w, e;
        while (infile >> w >> e) {  //reading the columns, line by line, and storing in the variables w and e, which are destroyed and recreated at each iteration
            wavelength.push_back(w); //assign w to wavelength vector
            efficiency.push_back(e);
        }

        infile.close();

        // Create TGraph
        TGraph *gr = new TGraph(wavelength.size(), &wavelength[0], &efficiency[0]);
        gr->SetLineColor(colors[i]);
        gr->SetLineWidth(3);
        gr->SetMarkerColor(colors[i]);
        gr->SetMarkerStyle(20);

        mg->Add(gr, "L");

        // Use custom legend label
        leg->AddEntry(gr, labels[i], "L");
    }

    // Draw everything
    TCanvas *c = new TCanvas("c", "Efficiency Curves", 900, 700);
    mg->SetTitle(";Wavelength [nm];Arbitrary Units");
    mg->Draw("A");
    leg->Draw();

    c->SetGrid();
}
