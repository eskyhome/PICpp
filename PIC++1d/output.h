#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "vector"
#include "stdio.h"

#include "vector3d.h"
#include "simulation.h"

void outputDistribution(FILE* outFile, std::vector<Particle*> particles, int particleType);
void outputTraectory(FILE* outFile, Particle* particle, double time, double plasma_period, double gyroradius);
void outputGrid(FILE* outFile, double* grid, int number, double scale = 1.0);
void outputFields(FILE* outEfile, FILE* outBfile, Vector3d* Efield, Vector3d* Bfield, int xnumber, double plasma_priod, double gyroradius, double fieldScale);
void outputConcentrations(FILE* outFile, double* electronConcentration, double* protonConcentration, double* chargeDensity, double* shiftChargeDensity, int xnumber, double plasma_period, double gyroradius, double fieldScale);
void outputVelocity(FILE* outFile, FILE* outElectronFile, Vector3d* velocity, Vector3d* electronVelocity, int xnumber, double plasma_period, double gyroradius);
void outputGeneral(FILE* outFile, Simulation* simulatiom);
void outputFlux(FILE* outFile, Vector3d* electricFlux, Vector3d* externalElectricFlux, int xnumber, double plasma_period, double gyroradius, double fieldScale);
void outputDivergenceError(FILE* outFile, Simulation* simulation, double plasma_period, double gyroradius, double fieldScale);
void outputVectorArray(FILE* outFile, Vector3d* vector3d, int number, double scale = 1.0);

void outputSimulationBackup(FILE* generalFile, FILE* Efile, FILE* Bfile, FILE* particlesFile, Simulation* simulation); 
void outputParticles(FILE* outFile, Simulation* simulation);
void outputParticle(FILE* outFile, Particle* particle);


#endif