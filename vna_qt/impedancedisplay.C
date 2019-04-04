#include "utility.H"
#include "impedancedisplay.H"
#include "ui_impedancedisplay.h"
#include <iostream>

ImpedanceDisplay::ImpedanceDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImpedanceDisplay)
    
{
    ui->setupUi(this);
    match = new Match();
}

ImpedanceDisplay::~ImpedanceDisplay()
{
    delete ui;
}

void ImpedanceDisplay::setValue(complex<double> s11, double freqHz, double z0) {
    complex<double> Z = -z0*(s11+1.)/(s11-1.);
    complex<double> Y = -(s11-1.)/(z0*(s11+1.));
    ui->l_impedance->setText(qs(ssprintf(127, "  %.2f\n%s j%.2f", Z.real(), Z.imag()>=0 ? "+" : "-", fabs(Z.imag()))));
    ui->l_admittance->setText(qs(ssprintf(127, "  %.4f\n%s j%.4f", Y.real(), Y.imag()>=0 ? "+" : "-", fabs(Y.imag()))));
    ui->l_s_admittance->setText(qs(ssprintf(127, "  %.4f\n%s j%.4f", 1./Z.real(), Z.imag()>=0 ? "+" : "-", fabs(1./Z.imag()))));
    ui->l_p_impedance->setText(qs(ssprintf(127, "  %.2f\n|| j%.2f", 1./Y.real(), 1./Y.imag())));
    double value = capacitance_inductance(freqHz, Z.imag());
    ui->l_series->setText(qs(ssprintf(127, "%.2f Ω\n%.2f %s%s", Z.real(), fabs(si_scale(value)), si_unit(value), value>0?"H":"F")));

    value = capacitance_inductance_Y(freqHz, Y.imag());
    ui->l_parallel->setText(qs(ssprintf(127, "%.2f Ω\n%.2f %s%s", 1./Y.real(), fabs(si_scale(value)), si_unit(value), value>0?"H":"F")));

    match->setValue(s11, freqHz);

    //
    if(match->highZ) {
        ui->match_1->setText(qs(ssprintf(127, "L %.2f %sH C %.2f %sF", fabs(si_scale(match->l1)), fabs(si_scale(match->c1)), si_unit(match->l1), si_unit(match->c1))));
        ui->match_2->setText(qs(ssprintf(127, "C %.2f %sFL %.2f %sH", fabs(si_scale(match->l2)), fabs(si_scale(match->c2)), si_unit(match->l2), si_unit(match->c2))));
    } else {
        if(match->match1SeriesCapacitor) {
            ui->match_1->setText(qs(ssprintf(127, "C %.2f %sF C %.2f %sF", fabs(si_scale(match->c1)), fabs(si_scale(match->l1)), si_unit(match->c1), si_unit(match->l1))));
        } else {
            ui->match_1->setText(qs(ssprintf(127, "C %.2f %sF L %.2f %sH", fabs(si_scale(match->c1)), fabs(si_scale(match->l1)), si_unit(match->c1), si_unit(match->l1))));
        }
        if(match->match2SeriesInductor) {
            ui->match_2->setText(qs(ssprintf(127, "L %.2f %sH L %.2f %sH", fabs(si_scale(match->l2)), fabs(si_scale(match->c2)), si_unit(match->l2), si_unit(match->c2))));
        } else {
            ui->match_2->setText(qs(ssprintf(127, "L %.2f %sH C %.2f %sF", fabs(si_scale(match->l2)), fabs(si_scale(match->c2)), si_unit(match->l2), si_unit(match->c2))));
        }
    }
}

void ImpedanceDisplay::clearValue() {
    QString p = "-";
    ui->l_impedance->setText(p);
    ui->l_admittance->setText(p);
    ui->l_s_admittance->setText(p);
    ui->l_p_impedance->setText(p);
    ui->l_series->setText(p);
    ui->l_parallel->setText(p);
    ui->match_1->setText(p);
    ui->match_2->setText(p);
}
