#include <iostream>
#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"
#include "TMath.h"

double gaussian(double *x, double *p) {
    return TMath::Gaus(x[0], p[0], p[1]);
}

void make_plot(double xmin, double xmax) {
    TCanvas *c1 = new TCanvas("c1", "Energy Deposition Comparison", 900, 700);
    c1->SetGrid();

    // Left Gaussian
    TF1 *f1 = new TF1("GaussianLeft", gaussian, xmin, xmax, 2);
    f1->SetParameters(-10, 5);
    f1->SetLineColor(kRed);
    f1->SetLineStyle(2);
    f1->SetLineWidth(2);

    // Right Gaussian
    TF1 *f2 = new TF1("GaussianRight", gaussian, xmin, xmax, 2);
    f2->SetParameters(10, 2);
    f2->SetLineColor(kBlue);
    f2->SetLineStyle(2);
    f2->SetLineWidth(2);

    // Sum of both
    TF1 *fsum = new TF1("fsum",
        [f1, f2](double *x, double *p) {
            return f1->Eval(x[0]) + f2->Eval(x[0]) + 0.2;
        },
        xmin, xmax, 0);

    fsum->SetLineColor(kGreen + 2);
    fsum->SetLineWidth(4);
    fsum->SetTitle("Examples with Gaussians;X axis;Y axis");

    // Draw
    fsum->Draw();
    f1->Draw("same");
    f2->Draw("same");

    // Legend
    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->AddEntry(f1, "Gaussian Left", "l");
    leg->AddEntry(f2, "Gaussian Right", "l");
    leg->AddEntry(fsum, "Sum", "l");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();

    c1->Update();
    c1->WaitPrimitive();

}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: ./functions xmin xmax\n";
        return 1;
    }

    double xmin = atof(argv[1]);
    double xmax = atof(argv[2]);

    std::cout << "Running Gaussian example with xmin=" 
              << xmin << " xmax=" << xmax << std::endl;

    make_plot(xmin, xmax);
    return 0;
}
