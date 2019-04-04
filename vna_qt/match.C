#define _USE_MATH_DEFINES
#include "match.H"
#include "ui_match.h"
#include <cmath>
#include <iostream>

Match::Match()
{
}

void Match::setValue(complex<double> s11, double freqHz, double Z0) {
    complex<double> Z = -Z0*(s11+1.)/(s11-1.);
    double Zr = Z.real();
    double Zi = Z.imag();
    highZ = Zr > Z0;
    if(highZ) {
        conductanceCW  = (Zi + sqrtf(Zr / Z0) * sqrtf(Zr * Zr + Zi * Zi - Z0 * Zr)) / (Zr * Zr + Zi * Zi);
        conductanceCCW = (Zi - sqrtf(Zr / Z0) * sqrtf(Zr * Zr + Zi * Zi - Z0 * Zr)) / (Zr * Zr + Zi * Zi);
        resistanceCW   = (conductanceCW * Z0 * Zr - Zi) / (1 - conductanceCW * Zi);
        resistanceCCW  = (conductanceCCW * Z0 * Zr - Zi) / (1 - conductanceCCW * Zi);
    } else {
        conductanceCW  = validate(1 / Z0 * sqrtf((Z0 - Zr) / Zr));
        conductanceCCW = validate(-(1 / Z0) * sqrtf((Z0 - Zr) / Zr));
        resistanceCW   = validate(sqrtf(Zr * (Z0 - Zr)) - Zi);
        resistanceCCW  = validate(-sqrtf(Zr * (Z0 - Zr)) - Zi);
    }
    calc(freqHz);
}

float Match::validate(float val) {
    float range = 1e-12;
    return isfinite(val) && range > val && val > -range ? 0 : val;
}

void Match::calc(double freq) {
    float w   = 2 * M_PI * freq;
    //by default : antenna -> series -> shunt -> source
    c1 = conductanceCW / w; //clockwise on conductance circle -- shunt capacitor
    l1 = resistanceCW / w; //clockwise on resistance circle -- series inductor
    c2 = resistanceCCW ? -1 / (w * resistanceCCW) : 0; //counter-clockwise on resistance circle -- series capacitor
    l2 = conductanceCCW ? -1 / (w * conductanceCCW) : 0; //counter-clockwise on conductance circle -- shunt inductor

    //if highZ, shunt is nearer to antenna, otherwise, nearer to source

    //if(!highZ) {
        //change to : antenna -> shunt -> series -> source
        if(l1<0) {
            match1SeriesCapacitor = true;
            //raise flag for inductor is now a series? capacitor
            l1 = resistanceCW ? -1 / (w * resistanceCW) : 0;
        } else {
            match1SeriesCapacitor = false;
        }
        if(c2<0) {
            match2SeriesInductor = true;
            //raise flag for capacitor is now a series? inductor
            c2 = resistanceCCW / w;
        } else {
            match2SeriesInductor = false;
        }
    //}
}