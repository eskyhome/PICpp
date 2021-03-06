#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#include "constants.h"
#include "particle.h"
#include "util.h"
#include "vector3d.h"
#include "matrix3d.h"

Particle::Particle(int n, double m, double q, double w, ParticleTypes t, ParticleTypeContainer type_container, double x0, double px0, double py0, double pz0, double dx0) {
	number = n;

	mass = m;
	charge = q;
	weight = w;
	type = t;
	typeContainer = type_container;

	x = x0;

	momentum.x = px0;
	momentum.y = py0;
	momentum.z = pz0;

	initialMomentum = momentum;

	dx = dx0;

	y = 0;
	z = 0;

	escaped = false;
}

Particle::Particle(const Particle& particle) {
	number = -1; //todo

	mass = particle.mass;
	charge = particle.charge;
	weight = particle.weight;
	type = particle.type;
	typeContainer = particle.typeContainer;

	x = particle.x;
	momentum = particle.momentum;
	initialMomentum = particle.initialMomentum;
	rotationTensor = particle.rotationTensor;

	dx = particle.dx;

	y = particle.y;
	z = particle.z;

	escaped = particle.escaped;
}

double Particle::shapeFunctionX(const double& xvalue) {
	return Bspline(x, dx, xvalue);
}

double Particle::momentumAbs() {
	return momentum.norm();
}

Vector3d Particle::velocity(double c) {
	double p2 = momentum.x * momentum.x + momentum.y * momentum.y + momentum.z * momentum.z;
	double mc2 = mass * c * c;
	if (p2 < relativisticPrecision * mass * mass * c * c) {
		return momentum / mass;
	}
	double gamma_factor = sqrt(p2 * c * c + mc2 * mc2) / mc2;

	return momentum / (mass * gamma_factor);
}

double Particle::velocityX(double c) {
	double p2 = momentum.x * momentum.x + momentum.y * momentum.y + momentum.z * momentum.z;
	double mc2 = mass * c * c;
	if (p2 < relativisticPrecision * mass * mass * c * c) {
		return momentum.x / mass;
	}
	double gamma_factor = sqrt(p2 * c * c + mc2 * mc2) / mc2;
	return momentum.x / (mass * gamma_factor);
}

double Particle::velocityY(double c) {
	double p2 = momentum.x * momentum.x + momentum.y * momentum.y + momentum.z * momentum.z;
	double mc2 = mass * c * c;
	if (p2 < relativisticPrecision * mass * mass * c * c) {
		return momentum.y / mass;
	}
	double gamma_factor = sqrt(p2 * c * c + mc2 * mc2) / mc2;
	return momentum.y / (mass * gamma_factor);
}

double Particle::velocityZ(double c) {
	double p2 = momentum.x * momentum.x + momentum.y * momentum.y + momentum.z * momentum.z;
	double mc2 = mass * c * c;
	if (p2 < relativisticPrecision * mass * mass * c * c) {
		return momentum.z / mass;
	}
	double gamma_factor = sqrt(p2 * c * c + mc2 * mc2) / mc2;
	return momentum.z / (mass * gamma_factor);
}

void Particle::addVelocity(Vector3d& v, double c) {
	if (v.norm() > c) {
		printf("ERROR v > c\n");
		FILE* errorLogFile = fopen("./output/errorLog.dat", "w");
		fprintf(errorLogFile, "v/c > 1 in addVelocity\n");
		fclose(errorLogFile);
		exit(0);
	}
	double c2 = c * c;

	Vector3d vel = velocity(c);
	vel += v;

	Matrix3d* rotation = Matrix3d::createBasisByOneVector(v);
	Matrix3d* inverse = rotation->Inverse();

	Vector3d rotatedV = (*inverse) * velocity(c);

	double gamma = 1 / (sqrt(1 - v.scalarMult(v) / c2));
	double vnorm = v.norm();
	double denominator = 1 + vnorm * rotatedV.z / c2;

	Vector3d shiftedV;

	shiftedV.z = (vnorm + rotatedV.z) / (denominator);
	shiftedV.y = rotatedV.y / (gamma * denominator);
	shiftedV.x = rotatedV.x / (gamma * denominator);

	Vector3d vel1 = (*rotation) * shiftedV;

	//todo relativistic!
	//setMomentumByV(vel, c);
	setMomentumByV(vel1, c);

	delete rotation;
	delete inverse;
}

void Particle::setMomentumByV(Vector3d v, double c) {
	if (v.norm() > c) {
		printf("ERROR v > c\n");
		printf("v = %g\n", v.norm());
		printf("c = %g\n", c);
		FILE* errorLogFile = fopen("./output/errorLog.dat", "w");
		fprintf(errorLogFile, "v/c > 1 in setMomentumByV\n");
		fclose(errorLogFile);
		exit(0);
	}
	double v2 = v.scalarMult(v);
	double c2 = c*c;
	if(v2 < relativisticPrecision*relativisticPrecision*c2){
		momentum = v * mass;
		return;
	}
	double gamma_factor = 1 / sqrt(1 - v2 / c2);
	momentum = v * (mass * gamma_factor);
}

double Particle::gammaFactor(double c) {
	double p2 = momentum.x * momentum.x + momentum.y * momentum.y + momentum.z * momentum.z;
	double mc2 = mass * c * c;
	double result = sqrt(p2 * c * c + mc2 * mc2) / mc2;
	alertNaNOrInfinity(result, "gamma = NaN");
	return result;
}

double Particle::energy(double c) {
	double p2 = momentum.x * momentum.x + momentum.y * momentum.y + momentum.z * momentum.z;
	if (p2 < relativisticPrecision * mass * mass * c * c) {
		return p2 / (2 * mass);
	}
	double gamma_factor = gammaFactor(c);
	//return gamma_factor*mass*c*c;
	return (gamma_factor - 1) * mass * c * c;
}
