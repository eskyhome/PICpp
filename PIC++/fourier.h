#ifndef FOURIER_H
#define FOURIER_H

#include "mpi.h"

class Complex;

void fourierTranslationXonePoint(Complex*** input, Complex*** output, bool direct, int xnumber, int j, int k, int xnumberGeneral, MPI_Comm& cartComm, int* xabsoluteIndex, int* cartCoord, int* cartDim);
Complex fourierTranslationXoneHarmonic(Complex*** input, bool direct, int xnumber, int j, int k, int xnumberGeneral, MPI_Comm& reducedCartComm, int knumber, int* xabsoluteIndex, int* cartCoord, int* cartDim);
void fourierTranslationYonePoint(Complex*** input, Complex*** output, bool direct, int ynumber, int i, int k, int ynumberGeneral, MPI_Comm& cartComm, int* yabsoluteIndex, int* cartCoord, int* cartDim);
Complex fourierTranslationYoneHarmonic(Complex*** input, bool direct, int ynumber, int i, int k, int ynumberGeneral, MPI_Comm& reducedCartComm, int knumber, int* yabsoluteIndex, int* cartCoord, int* cartDim);
void fourierTranslationZonePoint(Complex*** input, Complex*** output, bool direct, int znumber, int i, int j, int znumberGeneral, MPI_Comm& cartComm, int* zabsoluteIndex, int* cartCoord, int* cartDim);
Complex fourierTranslationZoneHarmonic(Complex*** input, bool direct, int znumber, int i, int j, int znumberGeneral, MPI_Comm& reducedCartComm, int knumber, int* zabsoluteIndex, int* cartCoord, int* cartDim);


void fourierTranslation(Complex*** input, Complex*** output, bool direct, int xnumber, int ynumber, int znumber, int xnumberGeneral, int ynumberGeneral, int znumberGeneral, MPI_Comm& cartComm, int* xabsoluteIndex, int* yabsoluteIndex, int* zabsoluteIndex, int* cartCoord, int* cartDim);

#endif