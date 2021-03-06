#include <cmath>
#include <mpi.h>
//#include <crtdbg.h>

//#include "memory_debug.h"

#include "util.h"
#include "constants.h"
#include "particle.h"
#include "vector3d.h"
#include "simulation.h"
#include "paths.h"


Vector3d Simulation::correlationTempEfield(Particle* particle) {
	return correlationGeneralEfield(particle, tempEfield);
}

Vector3d Simulation::correlationNewEfield(Particle* particle) {
	return correlationGeneralEfield(particle, newEfield);
}

Vector3d Simulation::correlationBfield(Particle* particle) const {
	Vector3d result = Vector3d(0, 0, 0);
	int maxI = splineOrder + 2;
	int maxJ = splineOrder + 2;
	if(ynumberGeneral == 1) {
		maxJ = 1;
	}
	int maxK = splineOrder + 2;
	if(znumberGeneral == 1) {
		maxK = 1;
	}
	for (int i = 0; i < maxI; ++i) {
		int cellI = particle->correlationMapCell.xindex[i];
		for (int j = 0; j < maxJ; ++j) {
			int cellJ = particle->correlationMapCell.yindex[j];
			if(ynumberGeneral == 1) {
				cellJ = 0;
			}
			for (int k = 0; k < maxK; ++k) {
				int cellK = particle->correlationMapCell.zindex[k];

				if(znumberGeneral == 1) {
					cellK = 0;
				}

				Vector3d B = Bfield[cellI][cellJ][cellK];

				double correlation = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] *
					particle->correlationMapCell.zcorrelation[k];
				result = result + B * correlation;
			}
		}
	}

	return result;
}

Vector3d Simulation::correlationNewBfield(Particle* particle) const {
	Vector3d result = Vector3d(0, 0, 0);
	int maxI = splineOrder + 2;
	int maxJ = splineOrder + 2;
	if(ynumberGeneral == 1) {
		maxJ = 1;
	}
	int maxK = splineOrder + 2;
	if(znumberGeneral == 1) {
		maxK = 1;
	}
	for (int i = 0; i < maxI; ++i) {
		int cellI = particle->correlationMapCell.xindex[i];
		for (int j = 0; j < maxJ; ++j) {
			int cellJ = particle->correlationMapCell.yindex[j];
			if(ynumberGeneral == 1) {
				cellJ = 0;
			}
			for (int k = 0; k < maxK; ++k) {
				int cellK = particle->correlationMapCell.zindex[k];

				if(znumberGeneral == 1) {
					cellK = 0;
				}



				Vector3d B = newBfield[cellI][cellJ][cellK];

				double correlation = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] *
					particle->correlationMapCell.zcorrelation[k];
				result = result + B * correlation;
			}
		}
	}

	return result;
}

Vector3d Simulation::correlationEfield(Particle* particle) {
	return correlationGeneralEfield(particle, Efield);
}

Vector3d Simulation::correlationGeneralEfield(Particle* particle, Vector3d*** field) {
	//alertNaNOrInfinity(particle.coordinates.x, "particle.x = NaN in correlationGeneralEfield\n");
	/*int xcount = floor(((particle.coordinates.x - xgrid[0]) / deltaX) + 0.5);
	int ycount = floor(((particle.coordinates.y - ygrid[0]) / deltaY) + 0.5);
	int zcount = floor(((particle.coordinates.z - zgrid[0]) / deltaZ) + 0.5);
	if (xcount < 0) {
		errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
		printf("xcount < 0 in correlationGeneralEfield\n");
		fprintf(errorLogFile, "xcount < 0 in correlationGeneralEfield\n");
		printf("xgrid[0] = %g particle.x = %g number = %d rank = %d\n", xgrid[0], particle.coordinates.x, particle.number,
		       rank);
		fprintf(errorLogFile, "xgrid[0] = %g particle.x = %g number = %d rank = %d\n", xgrid[0], particle.coordinates.x,
		        particle.number, rank);
		MPI_Finalize();
		exit(0);
	}
	if (xcount > xnumberAdded) {
		errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
		printf("xcount > xnumber + 1 in correlationGeneralEfield\n");
		fprintf(errorLogFile, "xcount > xnumber + 1 in correlationGeneralEfield\n");
		printf("xgrid[xnumber + 1] = %g particle.x = %g\n", xgrid[xnumberAdded], particle.coordinates.x);
		fprintf(errorLogFile, "xgrid[xnumber + 1] = %g particle.x = %g\n", xgrid[xnumberAdded], particle.coordinates.x);
		MPI_Finalize();
		exit(0);
	}*/

	Vector3d result = Vector3d(0, 0, 0);

	int maxI = splineOrder + 2;
	int maxJ = splineOrder + 2;
	if(ynumberGeneral == 1) {
		maxJ = 1;
	}
	int maxK = splineOrder + 2;
	if(znumberGeneral == 1) {
		maxK = 1;
	}
	for (int i = 0; i < maxI; ++i) {
		int nodeI = particle->correlationMapNode.xindex[i];
		for (int j = 0; j < maxJ; ++j) {
			int nodeJ = particle->correlationMapNode.yindex[j];
			if(ynumberGeneral == 1) {
				nodeJ = 0;
			}
			for (int k = 0; k < maxK; ++k) {
				int nodeK = particle->correlationMapNode.zindex[k];

				if(znumberGeneral == 1) {
					nodeK = 0;
				}

				Vector3d E = field[nodeI][nodeJ][nodeK];

				double correlation = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] *
					particle->correlationMapNode.zcorrelation[k];
				result = result + E * correlation;
			}
		}
	}

	return result;
}

Vector3d Simulation::correlationBunemanBfield(Particle* particle) {
	return correlationBunemanGeneralBfield(particle, bunemanBx, bunemanBy, bunemanBz);
}

Vector3d Simulation::correlationBunemanNewBfield(Particle* particle) {
	return correlationBunemanGeneralBfield(particle, bunemanNewBx, bunemanNewBy, bunemanNewBz);
}

Vector3d Simulation::correlationBunemanGeneralBfield(Particle* particle, double*** fieldX, double*** fieldY,
                                                     double*** fieldZ) {
	Vector3d result = Vector3d(0, 0, 0);
	int maxI = splineOrder + 2;
	int maxJ = splineOrder + 2;
	if(ynumberGeneral == 1) {
		maxJ = 1;
	}
	int maxK = splineOrder + 2;
	if(znumberGeneral == 1) {
		maxK = 1;
	}
	for (int i = 0; i < maxI; ++i) {
		int cellI = particle->correlationMapCell.xindex[i];
		int nodeI = particle->correlationMapNode.xindex[i];
		for (int j = 0; j < maxJ; ++j) {
			int cellJ = particle->correlationMapCell.yindex[j];
			int nodeJ = particle->correlationMapNode.yindex[j];
			if(ynumberGeneral == 1) {
				cellJ = 0;
				nodeJ = 0;
			}
			for (int k = 0; k < maxK; ++k) {
				int cellK = particle->correlationMapCell.zindex[k];
				int nodeK = particle->correlationMapNode.zindex[k];

				if(znumberGeneral == 1) {
					cellK = 0;
					nodeK = 0;
				}

				double correlationX = 0;
				double correlationY = 0;
				double correlationZ = 0;

				
				switch(Simulation::dimensionType) {
				case DimensionType::THREE_D: correlationX = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] * particle->correlationMapCell.zcorrelation[k];
					correlationY = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] * particle->correlationMapCell.zcorrelation[k];
					correlationZ = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] * particle->correlationMapNode.zcorrelation[k];
					break;
				case DimensionType::TWO_D_XY: correlationX = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j];
					correlationY = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j];
					correlationZ = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j];
					break;
				case DimensionType::TWO_D_XZ: correlationX = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.zcorrelation[k];
					correlationY = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapCell.zcorrelation[k];
					correlationZ = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.zcorrelation[k];
					break;
				case DimensionType::ONE_D: correlationX = particle->correlationMapNode.xcorrelation[i];
					correlationY = particle->correlationMapCell.xcorrelation[i];
					correlationZ = particle->correlationMapCell.xcorrelation[i];
					break;
				default: correlationX = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] * particle->correlationMapCell.zcorrelation[k];
					correlationY = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] * particle->correlationMapCell.zcorrelation[k];
					correlationZ = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] * particle->correlationMapNode.zcorrelation[k];
				}

				double Bx = fieldX[nodeI][cellJ][cellK];
				result.x = result.x + Bx * correlationX;

				double By = fieldY[cellI][nodeJ][cellK];
				result.y = result.y + By * correlationY;


				double Bz = fieldZ[cellI][cellJ][nodeK];
				result.z = result.z + Bz * correlationZ;
			}
		}
	}

	return result;
}

Vector3d Simulation::correlationBunemanEfield(Particle* particle) {
	return correlationBunemanGeneralEfield(particle, bunemanEx, bunemanEy, bunemanEz);
}

Vector3d Simulation::correlationBunemanNewEfield(Particle* particle) {
	return correlationBunemanGeneralEfield(particle, bunemanNewEx, bunemanNewEy, bunemanNewEz);
}

Vector3d Simulation::correlationBunemanGeneralEfield(Particle* particle, double*** fieldX, double*** fieldY,
                                                     double*** fieldZ) {
	Vector3d result = Vector3d(0, 0, 0);
	int maxI = splineOrder + 2;
	int maxJ = splineOrder + 2;
	if(ynumberGeneral == 1) {
		maxJ = 1;
	}
	int maxK = splineOrder + 2;
	if(znumberGeneral == 1) {
		maxK = 1;
	}
	for (int i = 0; i < maxI; ++i) {
		int cellI = particle->correlationMapCell.xindex[i];
		int nodeI = particle->correlationMapNode.xindex[i];
		for (int j = 0; j < maxJ; ++j) {
			int cellJ = particle->correlationMapCell.yindex[j];
			int nodeJ = particle->correlationMapNode.yindex[j];
			if(ynumberGeneral == 1) {
				cellJ = 0;
				nodeJ = 0;
			}
			for (int k = 0; k < maxK; ++k) {
				int cellK = particle->correlationMapCell.zindex[k];
				int nodeK = particle->correlationMapNode.zindex[k];

				if(znumberGeneral == 1) {
					cellK = 0;
					nodeK = 0;
				}

				double correlationX;
				double correlationY;
				double correlationZ;
				switch (Simulation::dimensionType){
				case DimensionType::THREE_D: correlationX = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] * particle->correlationMapNode.zcorrelation[k];
					correlationY = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] * particle->correlationMapNode.zcorrelation[k];
					correlationZ = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] * particle->correlationMapCell.zcorrelation[k];
					break;
				case DimensionType::TWO_D_XY: correlationX = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j];
					correlationY = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j];
					correlationZ = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j];
					break;
				case DimensionType::TWO_D_XZ: correlationX = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.zcorrelation[k];
					correlationY = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapNode.zcorrelation[k];
					correlationZ = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.zcorrelation[k];
					break;
				case DimensionType::ONE_D: correlationX = particle->correlationMapCell.xcorrelation[i];
					correlationY = particle->correlationMapNode.xcorrelation[i];
					correlationZ = particle->correlationMapNode.xcorrelation[i];
					break;
				default: correlationX = particle->correlationMapCell.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] * particle->correlationMapNode.zcorrelation[k];
					correlationY = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapCell.ycorrelation[j] * particle->correlationMapNode.zcorrelation[k];
					correlationZ = particle->correlationMapNode.xcorrelation[i] * particle->correlationMapNode.ycorrelation[j] * particle->correlationMapCell.zcorrelation[k];
					break;
				}

				double Ex = fieldX[cellI][nodeJ][nodeK];
				result.x = result.x + Ex * correlationX;

				double Ey = fieldY[nodeI][cellJ][nodeK];
				result.y = result.y + Ey * correlationY;

				double Ez = fieldZ[nodeI][nodeJ][cellK];
				result.z = result.z + Ez * correlationZ;
			}
		}
	}

	return result;
}

void Simulation::correlationBunemanEBfields(Particle* particle, double*** Ex, double*** Ey, double*** Ez, double*** Bx, double*** By, double*** Bz, Vector3d& E, Vector3d& B) {
	E = Vector3d(0, 0, 0);
	B = Vector3d(0, 0, 0);
	int maxI = splineOrder + 2;
	int maxJ = splineOrder + 2;
	if(ynumberGeneral == 1) {
		maxJ = 1;
	}
	int maxK = splineOrder + 2;
	if(znumberGeneral == 1) {
		maxK = 1;
	}
	int* cellXindex = particle->correlationMapCell.xindex;
	int* nodeXindex = particle->correlationMapNode.xindex;
	int* cellYindex = particle->correlationMapCell.yindex;
	int* nodeYindex = particle->correlationMapNode.yindex;
	int* cellZindex = particle->correlationMapCell.zindex;
	int* nodeZindex = particle->correlationMapNode.zindex;

	double* cellXcorrelation = particle->correlationMapCell.xcorrelation;
	double* nodeYcorrelation = particle->correlationMapNode.ycorrelation;
	double* nodeZcorrelation = particle->correlationMapNode.zcorrelation;
	double* nodeXcorrelation = particle->correlationMapNode.xcorrelation;
	double* cellYcorrelation = particle->correlationMapCell.ycorrelation;
	double* cellZcorrelation = particle->correlationMapCell.zcorrelation;

	for (int i = 0; i < maxI; ++i) {
		
		int cellI = cellXindex[i];
		int nodeI = nodeXindex[i];
		for (int j = 0; j < maxJ; ++j) {
			int cellJ = cellYindex[j];
			int nodeJ = nodeYindex[j];
			if(ynumberGeneral == 1) {
				cellJ = 0;
				nodeJ = 0;
			}
			for (int k = 0; k < maxK; ++k) {
				int cellK = cellZindex[k];
				int nodeK = nodeZindex[k];

				if(znumberGeneral == 1) {
					cellK = 0;
					nodeK = 0;
				}

				double correlationEX;
				double correlationEY;
				double correlationEZ;

				double correlationBX;
				double correlationBY;
				double correlationBZ;

				switch (Simulation::dimensionType){
				case DimensionType::THREE_D: correlationEX = cellXcorrelation[i] * nodeYcorrelation[j] * nodeZcorrelation[k];
					correlationEY = nodeXcorrelation[i] * cellYcorrelation[j] * nodeZcorrelation[k];
					correlationEZ = nodeXcorrelation[i] * nodeYcorrelation[j] * cellZcorrelation[k];

					correlationBX = nodeXcorrelation[i] * cellYcorrelation[j] * cellZcorrelation[k];
					correlationBY = cellXcorrelation[i] * nodeYcorrelation[j] * cellZcorrelation[k];
					correlationBZ = cellXcorrelation[i] * cellYcorrelation[j] * nodeZcorrelation[k];
					break;
				case DimensionType::TWO_D_XY: correlationEX = cellXcorrelation[i] * nodeYcorrelation[j];
					correlationEY = nodeXcorrelation[i] * cellYcorrelation[j];
					correlationEZ = nodeXcorrelation[i] * nodeYcorrelation[j];

					correlationBX = nodeXcorrelation[i] * cellYcorrelation[j];
					correlationBY = cellXcorrelation[i] * nodeYcorrelation[j];
					correlationBZ = cellXcorrelation[i] * cellYcorrelation[j];
					break;
				case DimensionType::TWO_D_XZ: correlationEX = cellXcorrelation[i] * nodeZcorrelation[k];
					correlationEY = nodeXcorrelation[i] * nodeZcorrelation[k];
					correlationEZ = nodeXcorrelation[i] * cellZcorrelation[k];

					correlationBX = nodeXcorrelation[i] * cellZcorrelation[k];
					correlationBY = cellXcorrelation[i] * cellZcorrelation[k];
					correlationBZ = cellXcorrelation[i] * nodeZcorrelation[k];
					break;
				case DimensionType::ONE_D: correlationEX = cellXcorrelation[i];
					correlationEY = nodeXcorrelation[i];
					correlationEZ = nodeXcorrelation[i];

					correlationBX = nodeXcorrelation[i];
					correlationBY = cellXcorrelation[i];
					correlationBZ = cellXcorrelation[i];
					break;
				default: correlationEX = cellXcorrelation[i] * nodeYcorrelation[j] * nodeZcorrelation[k];
					correlationEY = nodeXcorrelation[i] * cellYcorrelation[j] * nodeZcorrelation[k];
					correlationEZ = nodeXcorrelation[i] * nodeYcorrelation[j] * cellZcorrelation[k];

					correlationBX = nodeXcorrelation[i] * cellYcorrelation[j] * cellZcorrelation[k];
					correlationBY = cellXcorrelation[i] * nodeYcorrelation[j] * cellZcorrelation[k];
					correlationBZ = cellXcorrelation[i] * cellYcorrelation[j] * nodeZcorrelation[k];
					break;
				}

				E.x = E.x + Ex[cellI][nodeJ][nodeK] * correlationEX;

				E.y = E.y + Ey[nodeI][cellJ][nodeK] * correlationEY;

				E.z = E.z + Ez[nodeI][nodeJ][cellK] * correlationEZ;

				B.x = B.x + Bx[nodeI][cellJ][cellK] * correlationBX;

				B.y = B.y + By[cellI][nodeJ][cellK] * correlationBY;

				B.z = B.z + Bz[cellI][cellJ][nodeK] * correlationBZ;
			}
		}
	}
}

void Simulation::correlationBunemanEBfieldsWithoutMaps(Particle* particle, double*** Ex, double*** Ey, double*** Ez, double*** Bx, double*** By, double*** Bz, Vector3d& E, Vector3d& B) {
	double WleftCellX;
	double WrightCellX;
	int leftCellXindex = 0;
	int rightCellXindex = 0;

	double WleftNodeX;
	double WrightNodeX;
	int leftNodeXindex = 0;
	int rightNodeXindex = 0;

	double WleftCellY;
	double WrightCellY;
	int leftCellYindex = 0;
	int rightCellYindex = 0;

	double WleftNodeY;
	double WrightNodeY;
	int leftNodeYindex = 0;
	int rightNodeYindex = 0;

	double WleftCellZ;
	double WrightCellZ;
	int leftCellZindex = 0;
	int rightCellZindex = 0;

	double WleftNodeZ;
	double WrightNodeZ;
	int leftNodeZindex = 0;
	int rightNodeZindex = 0;
	
	int xcount = floor((particle->coordinates.x - xgrid[0]) / deltaX);
	if(particle->coordinates.x < middleXgrid[xcount]) {
		leftCellXindex = xcount - 1;
		rightCellXindex = xcount;
		WrightCellX = 0.5 + (particle->coordinates.x - xgrid[xcount])/deltaX;
		WleftCellX = 1.0 - WrightCellX;
	} else {
		leftCellXindex = xcount;
		rightCellXindex = xcount+1;
		WleftCellX = 0.5 + (xgrid[xcount+1] - particle->coordinates.x)/deltaX;
		WrightCellX = 1.0 - WleftCellX;
	}

	xcount = floor(((particle->coordinates.x - xgrid[0]) / deltaX) + 0.5);

	if(particle->coordinates.x < xgrid[xcount]) {
		leftNodeXindex = xcount - 1;
		rightNodeXindex = xcount;
		WrightNodeX = 0.5 + (particle->coordinates.x - middleXgrid[xcount - 1])/deltaX;
		WleftNodeX = 1.0 - WrightNodeX;
	} else {
		leftNodeXindex = xcount;
		rightNodeXindex = xcount+1;
		WleftNodeX = 0.5 + (middleXgrid[xcount] - particle->coordinates.x)/deltaX;
		WrightNodeX = 1.0 - WleftNodeX;
	}

	if(ynumberGeneral > 1) {
		int ycount = floor((particle->coordinates.y - ygrid[0]) / deltaY);
		if(particle->coordinates.y < middleYgrid[ycount]) {
			leftCellYindex = ycount - 1;
			rightCellYindex = ycount;
			WrightCellY = 0.5 + (particle->coordinates.y - ygrid[ycount])/deltaY;
			WleftCellY = 1.0 - WrightCellY;
		} else {
			leftCellYindex = ycount;
			rightCellYindex = ycount+1;
			WleftCellY = 0.5 + (ygrid[ycount+1] - particle->coordinates.y)/deltaY;
			WrightCellY = 1.0 - WleftCellY;
		}

		ycount = floor(((particle->coordinates.y - ygrid[0]) / deltaY) + 0.5);
	
		if(particle->coordinates.y < ygrid[ycount]) {
			leftNodeYindex = ycount - 1;
			rightNodeYindex = ycount;
			WrightNodeY = 0.5 + (particle->coordinates.y - middleYgrid[ycount - 1])/deltaY;
			WleftNodeY = 1.0 - WrightNodeY;
		} else {
			leftNodeYindex = ycount;
			rightNodeYindex = ycount+1;
			WleftNodeY = 0.5 + (middleYgrid[ycount] - particle->coordinates.y)/deltaY;
			WrightNodeY = 1.0 - WleftNodeY;
		}
	}

	if(znumberGeneral > 1) {
		int zcount = floor((particle->coordinates.z - zgrid[0]) / deltaZ);
		if(particle->coordinates.z < middleZgrid[zcount]) {
			leftCellZindex = zcount - 1;
			rightCellZindex = zcount;
			WrightCellZ = 0.5 + (particle->coordinates.z - zgrid[zcount])/deltaZ;
			WleftCellZ = 1.0 - WrightCellZ;
		} else {
			leftCellZindex = zcount;
			rightCellZindex = zcount+1;
			WleftCellZ = 0.5 + (zgrid[zcount+1] - particle->coordinates.z)/deltaZ;
			WrightCellZ = 1.0 - WleftCellZ;
		}

		zcount = floor(((particle->coordinates.z - zgrid[0]) / deltaZ) + 0.5);

		if(particle->coordinates.z < zgrid[zcount]) {
			leftNodeZindex = zcount - 1;
			rightNodeZindex = zcount;
			WrightNodeZ = 0.5 + (particle->coordinates.z - middleZgrid[zcount - 1])/deltaZ;
			WleftNodeZ = 1.0 - WrightNodeZ;
		} else {
			leftNodeZindex = zcount;
			rightNodeZindex = zcount+1;
			WleftNodeZ = 0.5 + (middleZgrid[zcount] - particle->coordinates.z)/deltaZ;
			WrightNodeZ = 1.0 - WleftNodeZ;
		}
	}

	E = Vector3d(0, 0, 0);
	B = Vector3d(0, 0, 0);
	switch (Simulation::dimensionType){
	case THREE_D:
		E.x += bunemanEx[leftCellXindex][leftNodeYindex][leftNodeZindex]*WleftCellX*WleftNodeY*WleftNodeZ;
		E.y += bunemanEy[leftNodeXindex][leftCellYindex][leftNodeZindex]*WleftNodeX*WleftCellY*WleftNodeZ;
		E.z += bunemanEz[leftNodeXindex][leftNodeYindex][leftCellZindex]*WleftNodeX*WleftNodeY*WleftCellZ;

		B.x += bunemanBx[leftNodeXindex][leftCellYindex][leftCellZindex]*WleftNodeX*WleftCellY*WleftCellZ;
		B.y += bunemanBy[leftCellXindex][leftNodeYindex][leftCellZindex]*WleftCellX*WleftNodeY*WleftCellZ;
		B.z += bunemanBz[leftCellXindex][leftCellYindex][leftNodeZindex]*WleftCellX*WleftCellY*WleftNodeZ;
		////
		E.x += bunemanEx[leftCellXindex][leftNodeYindex][rightNodeZindex]*WleftCellX*WleftNodeY*WrightNodeZ;
		E.y += bunemanEy[leftNodeXindex][leftCellYindex][rightNodeZindex]*WleftNodeX*WleftCellY*WrightNodeZ;
		E.z += bunemanEz[leftNodeXindex][leftNodeYindex][rightCellZindex]*WleftNodeX*WleftNodeY*WrightCellZ;

		B.x += bunemanBx[leftNodeXindex][leftCellYindex][rightCellZindex]*WleftNodeX*WleftCellY*WrightCellZ;
		B.y += bunemanBy[leftCellXindex][leftNodeYindex][rightCellZindex]*WleftCellX*WleftNodeY*WrightCellZ;
		B.z += bunemanBz[leftCellXindex][leftCellYindex][rightNodeZindex]*WleftCellX*WleftCellY*WrightNodeZ;
		////
		E.x += bunemanEx[leftCellXindex][rightNodeYindex][leftNodeZindex]*WleftCellX*WrightNodeY*WleftNodeZ;
		E.y += bunemanEy[leftNodeXindex][rightCellYindex][leftNodeZindex]*WleftNodeX*WrightCellY*WleftNodeZ;
		E.z += bunemanEz[leftNodeXindex][rightNodeYindex][leftCellZindex]*WleftNodeX*WrightNodeY*WleftCellZ;

		B.x += bunemanBx[leftNodeXindex][rightCellYindex][leftCellZindex]*WleftNodeX*WrightCellY*WleftCellZ;
		B.y += bunemanBy[leftCellXindex][rightNodeYindex][leftCellZindex]*WleftCellX*WrightNodeY*WleftCellZ;
		B.z += bunemanBz[leftCellXindex][rightCellYindex][leftNodeZindex]*WleftCellX*WrightCellY*WleftNodeZ;
		/////
		E.x += bunemanEx[leftCellXindex][rightNodeYindex][rightNodeZindex]*WleftCellX*WrightNodeY*WrightNodeZ;
		E.y += bunemanEy[leftNodeXindex][rightCellYindex][rightNodeZindex]*WleftNodeX*WrightCellY*WrightNodeZ;
		E.z += bunemanEz[leftNodeXindex][rightNodeYindex][rightCellZindex]*WleftNodeX*WrightNodeY*WrightCellZ;

		B.x += bunemanBx[leftNodeXindex][rightCellYindex][rightCellZindex]*WleftNodeX*WrightCellY*WrightCellZ;
		B.y += bunemanBy[leftCellXindex][rightNodeYindex][rightCellZindex]*WleftCellX*WrightNodeY*WrightCellZ;
		B.z += bunemanBz[leftCellXindex][rightCellYindex][rightNodeZindex]*WleftCellX*WrightCellY*WrightNodeZ;
		///////////////////////////////////////////

		E.x += bunemanEx[rightCellXindex][leftNodeYindex][leftNodeZindex]*WrightCellX*WleftNodeY*WleftNodeZ;
		E.y += bunemanEy[rightNodeXindex][leftCellYindex][leftNodeZindex]*WrightNodeX*WleftCellY*WleftNodeZ;
		E.z += bunemanEz[rightNodeXindex][leftNodeYindex][leftCellZindex]*WrightNodeX*WleftNodeY*WleftCellZ;

		B.x += bunemanBx[rightNodeXindex][leftCellYindex][leftCellZindex]*WrightNodeX*WleftCellY*WleftCellZ;
		B.y += bunemanBy[rightCellXindex][leftNodeYindex][leftCellZindex]*WrightCellX*WleftNodeY*WleftCellZ;
		B.z += bunemanBz[rightCellXindex][leftCellYindex][leftNodeZindex]*WrightCellX*WleftCellY*WleftNodeZ;
		////
		E.x += bunemanEx[rightCellXindex][leftNodeYindex][rightNodeZindex]*WrightCellX*WleftNodeY*WrightNodeZ;
		E.y += bunemanEy[rightNodeXindex][leftCellYindex][rightNodeZindex]*WrightNodeX*WleftCellY*WrightNodeZ;
		E.z += bunemanEz[rightNodeXindex][leftNodeYindex][rightCellZindex]*WrightNodeX*WleftNodeY*WrightCellZ;

		B.x += bunemanBx[rightNodeXindex][leftCellYindex][rightCellZindex]*WrightNodeX*WleftCellY*WrightCellZ;
		B.y += bunemanBy[rightCellXindex][leftNodeYindex][rightCellZindex]*WrightCellX*WleftNodeY*WrightCellZ;
		B.z += bunemanBz[rightCellXindex][leftCellYindex][rightNodeZindex]*WrightCellX*WleftCellY*WrightNodeZ;
		////
		E.x += bunemanEx[rightCellXindex][rightNodeYindex][leftNodeZindex]*WrightCellX*WrightNodeY*WleftNodeZ;
		E.y += bunemanEy[rightNodeXindex][rightCellYindex][leftNodeZindex]*WrightNodeX*WrightCellY*WleftNodeZ;
		E.z += bunemanEz[rightNodeXindex][rightNodeYindex][leftCellZindex]*WrightNodeX*WrightNodeY*WleftCellZ;

		B.x += bunemanBx[rightNodeXindex][rightCellYindex][leftCellZindex]*WrightNodeX*WrightCellY*WleftCellZ;
		B.y += bunemanBy[rightCellXindex][rightNodeYindex][leftCellZindex]*WrightCellX*WrightNodeY*WleftCellZ;
		B.z += bunemanBz[rightCellXindex][rightCellYindex][leftNodeZindex]*WrightCellX*WrightCellY*WleftNodeZ;
		/////
		E.x += bunemanEx[rightCellXindex][rightNodeYindex][rightNodeZindex]*WrightCellX*WrightNodeY*WrightNodeZ;
		E.y += bunemanEy[rightNodeXindex][rightCellYindex][rightNodeZindex]*WrightNodeX*WrightCellY*WrightNodeZ;
		E.z += bunemanEz[rightNodeXindex][rightNodeYindex][rightCellZindex]*WrightNodeX*WrightNodeY*WrightCellZ;

		B.x += bunemanBx[rightNodeXindex][rightCellYindex][rightCellZindex]*WrightNodeX*WrightCellY*WrightCellZ;
		B.y += bunemanBy[rightCellXindex][rightNodeYindex][rightCellZindex]*WrightCellX*WrightNodeY*WrightCellZ;
		B.z += bunemanBz[rightCellXindex][rightCellYindex][rightNodeZindex]*WrightCellX*WrightCellY*WrightNodeZ;

		break;
	case TWO_D_XY:
		E.x += bunemanEx[leftCellXindex][leftNodeYindex][0]*WleftCellX*WleftNodeY;
		E.y += bunemanEy[leftNodeXindex][leftCellYindex][0]*WleftNodeX*WleftCellY;
		E.z += bunemanEz[leftNodeXindex][leftNodeYindex][0]*WleftNodeX*WleftNodeY;

		B.x += bunemanBx[leftNodeXindex][leftCellYindex][0]*WleftNodeX*WleftCellY;
		B.y += bunemanBy[leftCellXindex][leftNodeYindex][0]*WleftCellX*WleftNodeY;
		B.z += bunemanBz[leftCellXindex][leftCellYindex][0]*WleftCellX*WleftCellY;

		E.x += bunemanEx[leftCellXindex][rightNodeYindex][0]*WleftCellX*WrightNodeY;
		E.y += bunemanEy[leftNodeXindex][rightCellYindex][0]*WleftNodeX*WrightCellY;
		E.z += bunemanEz[leftNodeXindex][rightNodeYindex][0]*WleftNodeX*WrightNodeY;

		B.x += bunemanBx[leftNodeXindex][rightCellYindex][0]*WleftNodeX*WrightCellY;
		B.y += bunemanBy[leftCellXindex][rightNodeYindex][0]*WleftCellX*WrightNodeY;
		B.z += bunemanBz[leftCellXindex][rightCellYindex][0]*WleftCellX*WrightCellY;
		//////////////////////
		E.x += bunemanEx[rightCellXindex][leftNodeYindex][0]*WrightCellX*WleftNodeY;
		E.y += bunemanEy[rightNodeXindex][leftCellYindex][0]*WrightNodeX*WleftCellY;
		E.z += bunemanEz[rightNodeXindex][leftNodeYindex][0]*WrightNodeX*WleftNodeY;

		B.x += bunemanBx[rightNodeXindex][leftCellYindex][0]*WrightNodeX*WleftCellY;
		B.y += bunemanBy[rightCellXindex][leftNodeYindex][0]*WrightCellX*WleftNodeY;
		B.z += bunemanBz[rightCellXindex][leftCellYindex][0]*WrightCellX*WleftCellY;

		E.x += bunemanEx[rightCellXindex][rightNodeYindex][0]*WrightCellX*WrightNodeY;
		E.y += bunemanEy[rightNodeXindex][rightCellYindex][0]*WrightNodeX*WrightCellY;
		E.z += bunemanEz[rightNodeXindex][rightNodeYindex][0]*WrightNodeX*WrightNodeY;

		B.x += bunemanBx[rightNodeXindex][rightCellYindex][0]*WrightNodeX*WrightCellY;
		B.y += bunemanBy[rightCellXindex][rightNodeYindex][0]*WrightCellX*WrightNodeY;
		B.z += bunemanBz[rightCellXindex][rightCellYindex][0]*WrightCellX*WrightCellY;
		break;
	case TWO_D_XZ:
		E.x += bunemanEx[leftCellXindex][0][leftNodeZindex]*WleftCellX*WleftNodeZ;
		E.y += bunemanEy[leftNodeXindex][0][leftNodeZindex]*WleftNodeX*WleftNodeZ;
		E.z += bunemanEz[leftNodeXindex][0][leftCellZindex]*WleftNodeX*WleftCellZ;

		B.x += bunemanBx[leftNodeXindex][0][leftCellZindex]*WleftNodeX*WleftCellZ;
		B.y += bunemanBy[leftCellXindex][0][leftCellZindex]*WleftCellX*WleftCellZ;
		B.z += bunemanBz[leftCellXindex][0][leftNodeZindex]*WleftCellX*WleftNodeZ;

		E.x += bunemanEx[leftCellXindex][0][rightNodeZindex]*WleftCellX*WrightNodeZ;
		E.y += bunemanEy[leftNodeXindex][0][rightNodeZindex]*WleftNodeX*WrightNodeZ;
		E.z += bunemanEz[leftNodeXindex][0][rightCellZindex]*WleftNodeX*WrightCellZ;

		B.x += bunemanBx[leftNodeXindex][0][rightCellZindex]*WleftNodeX*WrightCellZ;
		B.y += bunemanBy[leftCellXindex][0][rightCellZindex]*WleftCellX*WrightCellZ;
		B.z += bunemanBz[leftCellXindex][0][rightNodeZindex]*WleftCellX*WrightNodeZ;
		///////////////////////////////
		E.x += bunemanEx[rightCellXindex][0][leftNodeZindex]*WrightCellX*WleftNodeZ;
		E.y += bunemanEy[rightNodeXindex][0][leftNodeZindex]*WrightNodeX*WleftNodeZ;
		E.z += bunemanEz[rightNodeXindex][0][leftCellZindex]*WrightNodeX*WleftCellZ;

		B.x += bunemanBx[rightNodeXindex][0][leftCellZindex]*WrightNodeX*WleftCellZ;
		B.y += bunemanBy[rightCellXindex][0][leftCellZindex]*WrightCellX*WleftCellZ;
		B.z += bunemanBz[rightCellXindex][0][leftNodeZindex]*WrightCellX*WleftNodeZ;

		E.x += bunemanEx[rightCellXindex][0][rightNodeZindex]*WrightCellX*WrightNodeZ;
		E.y += bunemanEy[rightNodeXindex][0][rightNodeZindex]*WrightNodeX*WrightNodeZ;
		E.z += bunemanEz[rightNodeXindex][0][rightCellZindex]*WrightNodeX*WrightCellZ;

		B.x += bunemanBx[rightNodeXindex][0][rightCellZindex]*WrightNodeX*WrightCellZ;
		B.y += bunemanBy[rightCellXindex][0][rightCellZindex]*WrightCellX*WrightCellZ;
		B.z += bunemanBz[rightCellXindex][0][rightNodeZindex]*WrightCellX*WrightNodeZ;
		break;
	case ONE_D:
		E.x += bunemanEx[leftCellXindex][0][0]*WleftCellX;
		E.y += bunemanEy[leftNodeXindex][0][0]*WleftNodeX;
		E.z += bunemanEz[leftNodeXindex][0][0]*WleftNodeX;

		B.x += bunemanBx[leftNodeXindex][0][0]*WleftNodeX;
		B.y += bunemanBy[leftCellXindex][0][0]*WleftCellX;
		B.z += bunemanBz[leftCellXindex][0][0]*WleftCellX;
		////////////////////
		E.x += bunemanEx[rightCellXindex][0][0]*WrightCellX;
		E.y += bunemanEy[rightNodeXindex][0][0]*WrightNodeX;
		E.z += bunemanEz[rightNodeXindex][0][0]*WrightNodeX;

		B.x += bunemanBx[rightNodeXindex][0][0]*WrightNodeX;
		B.y += bunemanBy[rightCellXindex][0][0]*WrightCellX;
		B.z += bunemanBz[rightCellXindex][0][0]*WrightCellX;
		break;
	default:
		printf("wrong dimension type\n");
		MPI_Finalize();
		exit(0);
		break;
	}
}

double Simulation::correlationWithBbin(Particle& particle, int i, int j, int k) {
	int tempI = -1;
	for (int index = 0; index < splineOrder + 2; ++index) {
		if (particle.correlationMapCell.xindex[index] == i) {
			tempI = index;
			break;
		}
	}
	if (tempI == -1) {
		return 0.0;
	}
	double correlationX = particle.correlationMapCell.xcorrelation[tempI];
	tempI = -1;
	for (int index = 0; index < splineOrder + 2; ++index) {
		if (particle.correlationMapCell.yindex[index] == j) {
			tempI = index;
			break;
		}
	}
	if (tempI == -1) {
		return 0.0;
	}
	double correlationY = particle.correlationMapCell.ycorrelation[tempI];

	tempI = -1;
	for (int index = 0; index < splineOrder + 2; ++index) {
		if (particle.correlationMapCell.zindex[index] == k) {
			tempI = index;
			break;
		}
	}
	if (tempI == -1) {
		return 0.0;
	}
	double correlationZ = particle.correlationMapCell.zcorrelation[tempI];

	return correlationX * correlationY * correlationZ;
}

double Simulation::correlationWithEbin(Particle& particle, int i, int j, int k) {
	int tempI = -1;
	for (int index = 0; index < splineOrder + 2; ++index) {
		if (particle.correlationMapNode.xindex[index] == i) {
			tempI = index;
			break;
		}
	}
	if (tempI == -1) {
		return 0.0;
	}
	double correlationX = particle.correlationMapNode.xcorrelation[tempI];

	tempI = -1;
	for (int index = 0; index < splineOrder + 2; ++index) {
		if (particle.correlationMapNode.yindex[index] == j) {
			tempI = index;
			break;
		}
	}
	if (tempI == -1) {
		return 0.0;
	}
	double correlationY = particle.correlationMapNode.ycorrelation[tempI];

	tempI = -1;
	for (int index = 0; index < splineOrder + 2; ++index) {
		if (particle.correlationMapNode.zindex[index] == k) {
			tempI = index;
			break;
		}
	}
	if (tempI == -1) {
		return 0.0;
	}
	double correlationZ = particle.correlationMapNode.zcorrelation[tempI];


	return correlationX * correlationY * correlationZ;
}

double Simulation::correlationBspline(const double& x, const double& dx, const double& leftx, const double& rightx) {
	if (rightx < leftx) {
		printf("rightx < leftx\n");
		fflush(stdout);
		errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
		fprintf(errorLogFile, "rightx = %15.10g < leftx = %15.10g\n", rightx, leftx);
		fclose(errorLogFile);
		MPI_Finalize();
		exit(0);
	}

	double correlation = 0;

	if (x < leftx - dx)
		return 0;
	if (x > rightx + dx)
		return 0;

	switch (splineOrder) {
	case -1:
		if (x >= leftX && x < rightX) {
			correlation = 1.0;
		} else {
			correlation = 0;
		}
		break;
	case 0:
		if (x < leftx + dx) {
			correlation = 0.5 * (x + dx - leftx) / dx;
		} else if (x > rightx - dx) {
			correlation = 0.5 * (rightx - (x - dx)) / dx;
		} else {
			correlation = 1;
		}
		if (correlation < 0 && correlation > -1E-14) {
			correlation = 0;
		}
		break;
	case 1:
		if (x < leftx) {
			correlation = 0.5 * sqr(x + dx - leftx) / (dx*dx);
		} else if (x > rightx) {

			correlation = 0.5 * sqr(rightx - (x - dx)) / (dx*dx);
		} else {
			correlation = 1 - 0.5 * (sqr(x + dx - rightx) + sqr(leftx - (x - dx))) / (dx*dx);
		}
		if (correlation < 0 && correlation > -1E-14) {
			correlation = 0;
		}
		break;
	case 2:

		if (x + dx < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			correlation = (-9.0 * t3 / 16.0) + (27.0 * t2 / 16.0) + (-27.0 * t / 16.0) + 9.0 / 16.0;
		} else if (x + dx / 3 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			correlation = (27.0 * t3 / 16.0) + (-9.0 * t2 / 16.0) + (-15.0 * t / 16.0) + 23.0 / 48.0;
		} else if (x - dx / 3 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			correlation = (-27.0 * t3 / 16.0) + (-63.0 * t2 / 16.0) + (-33.0 * t / 16.0) + 17.0 / 48.0;
		} else if (x - dx < rightx) {
			double t = (rightx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			correlation = (9.0 * t3 / 16.0) + (27.0 * t2 / 16.0) + (27.0 * t / 16.0) + 9.0 / 16.0;
		} else {
			return 0;
		}
		if (correlation < 0 && correlation > -1E-14) {
			correlation = 0;
		}
		break;
	case 3:
		if (x + dx < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			correlation = (2.0 * t4 / 3.0) + (-8.0 * t3 / 3.0) + (4 * t2) + (-8.0 * t / 3.0) + 2.0 / 3.0;
		} else if (x + dx / 2 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			correlation = (-8.0 * t4 / 3.0) + (4 * t3) + (-t2) + (-t) + 11.0 / 24.0;
		} else if (x < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			correlation = (4 * t4) + (4 * t3) + (-t2) + (-t) + 11.0 / 24.0;
		} else if (x - dx / 2 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			correlation = (-8.0 * t4 / 3.0) + (-28.0 * t3 / 3.0) + (-11 * t2) + (-13.0 * t / 3) + 1.0 / 24;
		} else if (x - dx < rightx) {
			double t = (rightx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			correlation = (2.0 * t4 / 3.0) + (8.0 * t3 / 3.0) + (4 * t2) + (8.0 * t / 3.0) + 2.0 / 3.0;
		} else {
			return 0;
		}
		if (correlation < 0 && correlation > -1E-14) {
			correlation = 0;
		}
		break;
	case 4:
		if (x + dx < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			double t5 = t * t4;
			correlation = (-625.0 * t5 / 768.0) + (3125.0 * t4 / 768.0) + (-3125.0 * t3 / 384.0) + (3125.0 * t2 / 384.0) + (-
				3125.0 * t / 768.0) + 625.0 / 768.0;
			/*if(correlation < 0 && correlation > -1E-14){
			    correlation = 0;
			}
			if(correlation < 0) {

			    errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
			    fprintf(errorLogFile, "correlation < 0");
			    printf("correlation < 0");
			    fclose(errorLogFile);
			    MPI_Finalize();
			    exit(0);

			}*/
		} else if (x + 3 * dx / 5 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			double t5 = t * t4;
			correlation = (3125.0 * t5 / 768.0) + (-8125.0 * t4 / 768.0) + (3625.0 * t3 / 384.0) + (-925.0 * t2 / 384.0) + (-
				695.0 * t / 768.0) + 1667.0 / 3840;
			/*if(correlation < 0 && correlation > -1E-14){
			    correlation = 0;
			}
			if(correlation < 0) {

			    errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
			    fprintf(errorLogFile, "correlation < 0");
			    printf("correlation < 0");
			    fclose(errorLogFile);
			    MPI_Finalize();
			    exit(0);

			}*/
		} else if (x + dx / 5 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			double t5 = t * t4;
			correlation = (-3125.0 * t5 / 384.0) + (625.0 * t4 / 384.0) + (875.0 * t3 / 192.0) + (-275.0 * t2 / 192.0) + (-385.0
				* t / 384.0) + 841.0 / 1920.0;
			/*if(correlation < 0 && correlation > -1E-14){
			    correlation = 0;
			}
			if(correlation < 0) {

			    errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
			    fprintf(errorLogFile, "correlation < 0");
			    printf("correlation < 0");
			    fclose(errorLogFile);
			    MPI_Finalize();
			    exit(0);

			}*/
		} else if (x - dx / 5 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			double t5 = t * t4;
			correlation = (3125.0 * t5 / 384.0) + (6875.0 * t4 / 384.0) + (2125.0 * t3 / 192.0) + (-25.0 * t2 / 192.0) - (335.0 *
				t / 384.0) + 851.0 / 1920.0;
			/*if(correlation < 0 && correlation > -1E-14){
			    correlation = 0;
			}
			if(correlation < 0) {

			    errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
			    fprintf(errorLogFile, "correlation < 0");
			    printf("correlation < 0");
			    fclose(errorLogFile);
			    MPI_Finalize();
			    exit(0);

			}*/
		} else if (x - 3 * dx / 5 < rightx) {
			double t = (leftx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			double t5 = t * t4;
			correlation = (-3125.0 * t5 / 768.0) + (-14375.0 * t4 / 768.0) + (-12625.0 * t3 / 384.0) + (-10175.0 * t2 / 384.0) +
				(-6745.0 * t / 768.0) - 1943.0 / 3840.0;
			/*if(correlation < 0 && correlation > -1E-14){
			    correlation = 0;
			}
			if(correlation < 0) {

			    errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
			    fprintf(errorLogFile, "correlation < 0");
			    printf("correlation < 0");
			    fclose(errorLogFile);
			    MPI_Finalize();
			    exit(0);

			}*/
		} else if (x - dx < rightx) {
			double t = (rightx - x) / dx;
			double t2 = t * t;
			double t3 = t * t2;
			double t4 = t * t3;
			double t5 = t * t4;
			correlation = (625.0 * t5 / 768.0) + (3125.0 * t4 / 768.0) + (3125.0 * t3 / 384.0) + (3125.0 * t2 / 384.0) + (3125.0
				* t / 768.0) + 625.0 / 768.0;
			/*if(correlation < 0 && correlation > -1E-14){
			    correlation = 0;
			}
			if(correlation < 0) {

			    errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
			    fprintf(errorLogFile, "correlation < 0");
			    printf("correlation < 0");
			    fclose(errorLogFile);
			    MPI_Finalize();
			    exit(0);

			}*/
		} else {
			return 0;
		}
		if (correlation < 0 && correlation > -1E-14) {
			correlation = 0;
		}
		break;
	default:
		errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
		fprintf(errorLogFile, "spline order is wrong");
		fclose(errorLogFile);
		MPI_Finalize();
		exit(0);
	}


	if (correlation > 1) {
		errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
		fprintf(errorLogFile, "correlation > 1");
		printf("correlation > 1");
		fclose(errorLogFile);
		MPI_Finalize();
		exit(0);
	}
	if (correlation < 0) {

		errorLogFile = fopen((outputDir + "errorLog.dat").c_str(), "w");
		fprintf(errorLogFile, "correlation < 0");
		printf("correlation < 0");
		fclose(errorLogFile);
		MPI_Finalize();
		exit(0);

	}

	return correlation;
}

void Simulation::updateParticleCorrelationMaps() {
	for (int pcount = 0; pcount < particles.size(); pcount++) {
		Particle* particle = particles[pcount];
		if(solverType == BUNEMAN) {
			updateBunemanCorrelationMaps(particle);
		} else {
			updateCorrelationMaps(particle);
		}
	}
}

void Simulation::updateCorrelationMaps(Particle* particle) {
	updateCorrelationMapCell(particle);
	updateCorrelationMapNode(particle);
}

void Simulation::updateCorrelationMapsX(Particle* particle) {
	updateCorrelationMapCellX(particle);
	updateCorrelationMapNodeX(particle);
}

void Simulation::updateCorrelationMapsY(Particle* particle) {
	updateCorrelationMapCellY(particle);
	updateCorrelationMapNodeY(particle);
}

void Simulation::updateCorrelationMapsZ(Particle* particle) {
	updateCorrelationMapCellZ(particle);
	updateCorrelationMapNodeZ(particle);
}

void Simulation::updateCorrelationMapCellX(Particle* particle) {
	/*if (particle.coordinates.x < xgrid[additionalBinNumber]) {
		printf("particle.coordinates.x < 0 %g particle number = %d\n", particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.x > xgrid[xnumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.x > xgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}*/
	int xcount = floor((particle->coordinates.x - xgrid[0]) / deltaX);
	if ((splineOrder % 2) == 0) {
		bool leftSideX = particle->coordinates.x < middleXgrid[xcount];
		int leftShiftX = leftSideX ? -1 : 0;
		int tempIndex = 0;
		for (int i = xcount + leftShiftX - (splineOrder / 2); i <= xcount + leftShiftX + (splineOrder / 2) + 1; ++i) {
			particle->correlationMapCell.xindex[tempIndex] = i;
			particle->correlationMapCell.xcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.x, particle->dx, xgrid[0] + i * deltaX, xgrid[0] + (i + 1) * deltaX);
			tempIndex++;
		}
	} else {
		int tempIndex = 0;
		if (splineOrder == -1) {
			particle->correlationMapCell.xindex[tempIndex] = xcount;
			particle->correlationMapCell.xcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.x, particle->dx, xgrid[0] + xcount * deltaX, xgrid[0] + (xcount + 1) * deltaX);
		} else {
			for (int i = xcount - (splineOrder / 2) - 1; i <= xcount + (splineOrder / 2) + 1; ++i) {
				particle->correlationMapCell.xindex[tempIndex] = i;
				particle->correlationMapCell.xcorrelation[tempIndex] = correlationBspline(
					particle->coordinates.x, particle->dx, xgrid[0] + i * deltaX, xgrid[0] + (i + 1) * deltaX);
				tempIndex++;
			}
		}
	}
}

void Simulation::updateCorrelationMapCellY(Particle* particle) {
	/*if(ynumberGeneral > 1){
	if (particle.coordinates.y < ygrid[additionalBinNumber]) {
		printf("particle.coordinates.y < 0 %g particle number = %d\n", particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.y > ygrid[ynumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.y > ygrid[ynumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.y, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int ycount = floor((particle->coordinates.y - ygrid[0]) / deltaY);
	if ((splineOrder % 2) == 0) {
		bool leftSideY = particle->coordinates.y < middleYgrid[ycount];
		int leftShiftY = leftSideY ? -1 : 0;
		int tempIndex = 0;
		for (int j = ycount + leftShiftY - (splineOrder / 2); j <= ycount + leftShiftY + (splineOrder / 2) + 1; ++j) {
			particle->correlationMapCell.yindex[tempIndex] = j;
			particle->correlationMapCell.ycorrelation[tempIndex] = correlationBspline(
				particle->coordinates.y, particle->dy, ygrid[0] + j * deltaY, ygrid[0] + (j + 1) * deltaY);
			tempIndex++;
		}
	} else {
		int tempIndex = 0;
		if (splineOrder == -1) {
			particle->correlationMapCell.yindex[tempIndex] = ycount;
			particle->correlationMapCell.ycorrelation[tempIndex] = correlationBspline(
				particle->coordinates.y, particle->dy, ygrid[0] + ycount * deltaY, ygrid[0] + (ycount + 1) * deltaY);
		} else {
			for (int j = ycount - (splineOrder / 2) - 1; j <= ycount + (splineOrder / 2) + 1; ++j) {
				particle->correlationMapCell.yindex[tempIndex] = j;
				particle->correlationMapCell.ycorrelation[tempIndex] = correlationBspline(
					particle->coordinates.y, particle->dy, ygrid[0] + j * deltaY, ygrid[0] + (j + 1) * deltaY);
				tempIndex++;
			}
		}
	}
}

void Simulation::updateCorrelationMapCellZ(Particle* particle) {
	/*if(znumberGeneral > 1){
	if (particle.coordinates.z < zgrid[additionalBinNumber]) {
		printf("particle.coordinates.z < 0 %g particle number = %d\n", particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.z > zgrid[znumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.z > zgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int zcount = floor((particle->coordinates.z - zgrid[0]) / deltaZ);
	if ((splineOrder % 2) == 0) {
		int tempIndex = 0;
		bool leftSideZ = particle->coordinates.z < middleZgrid[zcount];
		int leftShiftZ = leftSideZ ? -1 : 0;
		for (int k = zcount + leftShiftZ - (splineOrder / 2); k <= zcount + leftShiftZ + (splineOrder / 2) + 1; ++k) {
			particle->correlationMapCell.zindex[tempIndex] = k;
			particle->correlationMapCell.zcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.z, particle->dz, zgrid[0] + k * deltaZ, zgrid[0] + (k + 1) * deltaZ);
			tempIndex++;
		}
	} else {
		int tempIndex = 0;
		if (splineOrder == -1) {
			particle->correlationMapCell.zindex[tempIndex] = zcount;
			particle->correlationMapCell.zcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.z, particle->dz, zgrid[0] + zcount * deltaZ, zgrid[0] + (zcount + 1) * deltaZ);
		} else {
			for (int k = zcount - (splineOrder / 2) - 1; k <= zcount + (splineOrder / 2) + 1; ++k) {
				particle->correlationMapCell.zindex[tempIndex] = k;
				particle->correlationMapCell.zcorrelation[tempIndex] = correlationBspline(
					particle->coordinates.z, particle->dz, zgrid[0] + k * deltaZ, zgrid[0] + (k + 1) * deltaZ);
				tempIndex++;
			}
		}
	}
}

void Simulation::updateCorrelationMapCell(Particle* particle) {
	updateCorrelationMapCellX(particle);
	if(ynumberGeneral > 1){
		updateCorrelationMapCellY(particle);
	}
	if(znumberGeneral > 1){
		updateCorrelationMapCellZ(particle);
	}

	/*double fullCorrelation = 0;
	for (int i = 0; i < splineOrder + 2; ++i) {
		for (int j = 0; j < splineOrder + 2; ++j) {
			for (int k = 0; k < splineOrder + 2; ++k) {
				fullCorrelation += particle.correlationMapCell.xcorrelation[i] * particle.correlationMapCell.ycorrelation[j] *
					particle.correlationMapCell.zcorrelation[k];
			}
		}
	}
	if ((fullCorrelation < 1.0 - 1E-10) || (fullCorrelation > 1.0 + 1E-10)) {
		printf("full correlation cell = %20.15g\n", fullCorrelation);
	}*/
}

void Simulation::updateCorrelationMapNodeX(Particle* particle) {
	/*if (particle.coordinates.x < xgrid[additionalBinNumber]) {
		printf("particle.coordinates.x < 0 %g particle number = %d\n", particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.x > xgrid[xnumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.x > xgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}*/
	int xcount = floor(((particle->coordinates.x - xgrid[0]) / deltaX) + 0.5);
	if ((splineOrder % 2) == 0) {
		bool leftSideX = particle->coordinates.x < xgrid[xcount];
		int leftShiftX = leftSideX ? -1 : 0;
		int tempIndex = 0;
		for (int i = xcount + leftShiftX - (splineOrder / 2); i <= xcount + leftShiftX + (splineOrder / 2) + 1; ++i) {
			particle->correlationMapNode.xindex[tempIndex] = i;
			particle->correlationMapNode.xcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.x, particle->dx, xgrid[0] + (i - 0.5) * deltaX, xgrid[0] + (i + 0.5) * deltaX);
			tempIndex++;
		}
	} else {
		int tempIndex = 0;
		if (splineOrder == -1) {
			particle->correlationMapNode.xindex[tempIndex] = xcount;
			particle->correlationMapNode.xcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.x, particle->dx, xgrid[0] + (xcount - 0.5) * deltaX, xgrid[0] + (xcount + 0.5) * deltaX);
		} else {
			for (int i = xcount - (splineOrder / 2) - 1; i <= xcount + (splineOrder / 2) + 1; ++i) {
				particle->correlationMapNode.xindex[tempIndex] = i;
				particle->correlationMapNode.xcorrelation[tempIndex] = correlationBspline(
					particle->coordinates.x, particle->dx, xgrid[0] + (i - 0.5) * deltaX, xgrid[0] + (i + 0.5) * deltaX);
				tempIndex++;
			}
		}
	}
}

void Simulation::updateCorrelationMapNodeY(Particle* particle) {
	/*if(ynumberGeneral > 1){
	if (particle.coordinates.y < ygrid[additionalBinNumber]) {
		printf("particle.coordinates.y < 0 %g particle number = %d\n", particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.y > ygrid[ynumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.y > ygrid[ynumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.y, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int ycount = floor(((particle->coordinates.y - ygrid[0]) / deltaY) + 0.5);
	if ((splineOrder % 2) == 0) {
		bool leftSideY = particle->coordinates.y < ygrid[ycount];
		int leftShiftY = leftSideY ? -1 : 0;
		int tempIndex = 0;
		for (int j = ycount + leftShiftY - (splineOrder / 2); j <= ycount + leftShiftY + (splineOrder / 2) + 1; ++j) {
			particle->correlationMapNode.yindex[tempIndex] = j;
			particle->correlationMapNode.ycorrelation[tempIndex] = correlationBspline(
				particle->coordinates.y, particle->dy, ygrid[0] + (j - 0.5) * deltaY, ygrid[0] + (j + 0.5) * deltaY);
			tempIndex++;
		}
	} else {
		int tempIndex = 0;
		if (splineOrder == -1) {
			particle->correlationMapNode.yindex[tempIndex] = ycount;
			particle->correlationMapNode.ycorrelation[tempIndex] = correlationBspline(
				particle->coordinates.y, particle->dy, ygrid[0] + (ycount - 0.5) * deltaY, ygrid[0] + (ycount + 0.5) * deltaY);
		} else {
			for (int j = ycount - (splineOrder / 2) - 1; j <= ycount + (splineOrder / 2) + 1; ++j) {
				particle->correlationMapNode.yindex[tempIndex] = j;
				particle->correlationMapNode.ycorrelation[tempIndex] = correlationBspline(
					particle->coordinates.y, particle->dy, ygrid[0] + (j - 0.5) * deltaY, ygrid[0] + (j + 0.5) * deltaY);
				tempIndex++;
			}
		}
	}
}

void Simulation::updateCorrelationMapNodeZ(Particle* particle) {
	/*if(znumberGeneral > 1){
	if (particle.coordinates.z < zgrid[additionalBinNumber]) {
		printf("particle.coordinates.z < 0 %g particle number = %d\n", particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.z > zgrid[znumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.z > zgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int zcount = floor(((particle->coordinates.z - zgrid[0]) / deltaZ) + 0.5);
	if ((splineOrder % 2) == 0) {
		bool leftSideZ = particle->coordinates.z < zgrid[zcount];
		int leftShiftZ = leftSideZ ? -1 : 0;
		int tempIndex = 0;
		for (int k = zcount + leftShiftZ - (splineOrder / 2); k <= zcount + leftShiftZ + (splineOrder / 2) + 1; ++k) {
			particle->correlationMapNode.zindex[tempIndex] = k;
			particle->correlationMapNode.zcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.z, particle->dz, zgrid[0] + (k - 0.5) * deltaZ, zgrid[0] + (k + 0.5) * deltaZ);
			tempIndex++;
		}
	} else {
		int tempIndex = 0;
		if (splineOrder == -1) {
			particle->correlationMapNode.zindex[tempIndex] = zcount;
			particle->correlationMapNode.zcorrelation[tempIndex] = correlationBspline(
				particle->coordinates.z, particle->dz, zgrid[0] + (zcount - 0.5) * deltaZ, zgrid[0] + (zcount + 0.5) * deltaZ);
		} else {
			for (int k = zcount - (splineOrder / 2) - 1; k <= zcount + (splineOrder / 2) + 1; ++k) {
				particle->correlationMapNode.zindex[tempIndex] = k;
				particle->correlationMapNode.zcorrelation[tempIndex] = correlationBspline(
					particle->coordinates.z, particle->dz, zgrid[0] + (k - 0.5) * deltaZ, zgrid[0] + (k + 0.5) * deltaZ);
				tempIndex++;
			}
		}
	}
}

void Simulation::updateCorrelationMapNode(Particle* particle) {
	updateCorrelationMapNodeX(particle);
	if(ynumberGeneral > 1){
		updateCorrelationMapNodeY(particle);
	}
	if(znumberGeneral > 1){
		updateCorrelationMapNodeZ(particle);
	}

	/*double fullCorrelation = 0;
	for (int i = 0; i < splineOrder + 2; ++i) {
		for (int j = 0; j < splineOrder + 2; ++j) {
			for (int k = 0; k < splineOrder + 2; ++k) {
				fullCorrelation += particle.correlationMapNode.xcorrelation[i] * particle.correlationMapNode.ycorrelation[j] *
					particle.correlationMapNode.zcorrelation[k];
			}
		}
	}
	if ((fullCorrelation < 1.0 - 1E-10) || (fullCorrelation > 1.0 + 1E-10)) {
		printf("full correlation node = %20.15g\n", fullCorrelation);
	}*/
}


void Simulation::updateBunemanCorrelationMaps(Particle* particle) {
	updateBunemanCorrelationMapCell(particle);
	updateBunemanCorrelationMapNode(particle);
}

void Simulation::updateBunemanCorrelationMapsX(Particle* particle) {
	updateBunemanCorrelationMapCellX(particle);
	updateBunemanCorrelationMapNodeX(particle);
}

void Simulation::updateBunemanCorrelationMapsY(Particle* particle) {
	updateCorrelationMapCellY(particle);
	updateCorrelationMapNodeY(particle);
}

void Simulation::updateBunemanCorrelationMapsZ(Particle* particle) {
	updateCorrelationMapCellZ(particle);
	updateCorrelationMapNodeZ(particle);
}

void Simulation::updateBunemanCorrelationMapCellX(Particle* particle) {
	/*if (particle.coordinates.x < xgrid[additionalBinNumber]) {
		printf("particle.coordinates.x < 0 %g particle number = %d\n", particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.x > xgrid[xnumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.x > xgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}*/
	int xcount = floor((particle->coordinates.x - xgrid[0]) / deltaX);
	if(particle->coordinates.x < middleXgrid[xcount]) {
		particle->correlationMapCell.xindex[0] = xcount - 1;
		particle->correlationMapCell.xindex[1] = xcount;
		double w = 0.5 + (particle->coordinates.x - xgrid[xcount])/deltaX;
		particle->correlationMapCell.xcorrelation[0] = 1.0 - w;
		particle->correlationMapCell.xcorrelation[1] = w;
	} else {
		particle->correlationMapCell.xindex[0] = xcount;
		particle->correlationMapCell.xindex[1] = xcount+1;
		double w = 0.5 + (xgrid[xcount+1] - particle->coordinates.x)/deltaX;
		particle->correlationMapCell.xcorrelation[0] = w;
		particle->correlationMapCell.xcorrelation[1] = 1.0 - w;
	}
}

void Simulation::updateBunemanCorrelationMapCellY(Particle* particle) {
	/*if(ynumberGeneral > 1){
	if (particle.coordinates.y < ygrid[additionalBinNumber]) {
		printf("particle.coordinates.y < 0 %g particle number = %d\n", particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.y > ygrid[ynumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.y > ygrid[ynumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.y, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int ycount = floor((particle->coordinates.y - ygrid[0]) / deltaY);
	if(particle->coordinates.y < middleYgrid[ycount]) {
		particle->correlationMapCell.yindex[0] = ycount - 1;
		particle->correlationMapCell.yindex[1] = ycount;
		double w = 0.5 + (particle->coordinates.y - ygrid[ycount])/deltaY;
		particle->correlationMapCell.ycorrelation[0] = 1.0 - w;
		particle->correlationMapCell.ycorrelation[1] = w;
	} else {
		particle->correlationMapCell.yindex[0] = ycount;
		particle->correlationMapCell.yindex[1] = ycount+1;
		double w = 0.5 + (ygrid[ycount+1] - particle->coordinates.y)/deltaY;
		particle->correlationMapCell.ycorrelation[0] = w;
		particle->correlationMapCell.ycorrelation[1] = 1.0 - w;
	}
}

void Simulation::updateBunemanCorrelationMapCellZ(Particle* particle) {
	/*if(znumberGeneral > 1){
	if (particle.coordinates.z < zgrid[additionalBinNumber]) {
		printf("particle.coordinates.z < 0 %g particle number = %d\n", particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.z > zgrid[znumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.z > zgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int zcount = floor((particle->coordinates.z - zgrid[0]) / deltaZ);
	if(particle->coordinates.z < middleZgrid[zcount]) {
		particle->correlationMapCell.zindex[0] = zcount - 1;
		particle->correlationMapCell.zindex[1] = zcount;
		double w = 0.5 + (particle->coordinates.z - zgrid[zcount])/deltaZ;
		particle->correlationMapCell.zcorrelation[0] = 1.0 - w;
		particle->correlationMapCell.zcorrelation[1] = w;
	} else {
		particle->correlationMapCell.zindex[0] = zcount;
		particle->correlationMapCell.zindex[1] = zcount+1;
		double w = 0.5 + (zgrid[zcount+1] - particle->coordinates.z)/deltaZ;
		particle->correlationMapCell.zcorrelation[0] = w;
		particle->correlationMapCell.zcorrelation[1] = 1.0 - w;
	}
}

void Simulation::updateBunemanCorrelationMapCell(Particle* particle) {
	updateBunemanCorrelationMapCellX(particle);
	if(ynumberGeneral > 1){
		updateBunemanCorrelationMapCellY(particle);
	}
	if(znumberGeneral > 1){
		updateBunemanCorrelationMapCellZ(particle);
	}

	/*double fullCorrelation = 0;
	for (int i = 0; i < splineOrder + 2; ++i) {
		for (int j = 0; j < splineOrder + 2; ++j) {
			for (int k = 0; k < splineOrder + 2; ++k) {
				fullCorrelation += particle.correlationMapCell.xcorrelation[i] * particle.correlationMapCell.ycorrelation[j] *
					particle.correlationMapCell.zcorrelation[k];
			}
		}
	}
	if ((fullCorrelation < 1.0 - 1E-10) || (fullCorrelation > 1.0 + 1E-10)) {
		printf("full correlation cell = %20.15g\n", fullCorrelation);
	}*/
}

void Simulation::updateBunemanCorrelationMapNodeX(Particle* particle) {
	/*if (particle.coordinates.x < xgrid[additionalBinNumber]) {
		printf("particle.coordinates.x < 0 %g particle number = %d\n", particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.x > xgrid[xnumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.x > xgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.x, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, x[0] = %g x[xnumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, xgrid[0], xgrid[xnumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}*/
	int xcount = floor(((particle->coordinates.x - xgrid[0]) / deltaX) + 0.5);

	if(particle->coordinates.x < xgrid[xcount]) {
		particle->correlationMapNode.xindex[0] = xcount - 1;
		particle->correlationMapNode.xindex[1] = xcount;
		double w = 0.5 + (particle->coordinates.x - middleXgrid[xcount - 1])/deltaX;
		particle->correlationMapNode.xcorrelation[0] = 1.0 - w;
		particle->correlationMapNode.xcorrelation[1] = w;
	} else {
		particle->correlationMapNode.xindex[0] = xcount;
		particle->correlationMapNode.xindex[1] = xcount+1;
		double w = 0.5 + (middleXgrid[xcount] - particle->coordinates.x)/deltaX;
		particle->correlationMapNode.xcorrelation[0] = w;
		particle->correlationMapNode.xcorrelation[1] = 1.0 - w;
	}
}

void Simulation::updateBunemanCorrelationMapNodeY(Particle* particle) {
	/*if(ynumberGeneral > 1){
	if (particle.coordinates.y < ygrid[additionalBinNumber]) {
		printf("particle.coordinates.y < 0 %g particle number = %d\n", particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.y > ygrid[ynumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.y > ygrid[ynumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.y, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, y[0] = %g y[ynumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.y, ygrid[0], ygrid[ynumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int ycount = floor(((particle->coordinates.y - ygrid[0]) / deltaY) + 0.5);

	if(particle->coordinates.y < ygrid[ycount]) {
		particle->correlationMapNode.yindex[0] = ycount - 1;
		particle->correlationMapNode.yindex[1] = ycount;
		double w = 0.5 + (particle->coordinates.y - middleYgrid[ycount - 1])/deltaY;
		particle->correlationMapNode.ycorrelation[0] = 1.0 - w;
		particle->correlationMapNode.ycorrelation[1] = w;
	} else {
		particle->correlationMapNode.yindex[0] = ycount;
		particle->correlationMapNode.yindex[1] = ycount+1;
		double w = 0.5 + (middleYgrid[ycount] - particle->coordinates.y)/deltaY;
		particle->correlationMapNode.ycorrelation[0] = w;
		particle->correlationMapNode.ycorrelation[1] = 1.0 - w;
	}
}

void Simulation::updateBunemanCorrelationMapNodeZ(Particle* particle) {
	/*if(znumberGeneral > 1){
	if (particle.coordinates.z < zgrid[additionalBinNumber]) {
		printf("particle.coordinates.z < 0 %g particle number = %d\n", particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	if (particle.coordinates.z > zgrid[znumberAdded - additionalBinNumber]) {
		printf("particle.coordinates.z > zgrid[xnumberAdded - 1 - additionalBinNumber] %g particle number = %d\n",
		       particle.coordinates.z, particle.number);
		printf("particle.x = %g particle.y = %g particle.x = %g, z[0] = %g z[znumberAdded] = %g", particle.coordinates.x,
		       particle.coordinates.y, particle.coordinates.x, zgrid[0], zgrid[znumberAdded]);
		Vector3d velocity = particle.getVelocity();
		printf("particle.vx = %g particle.vy = %g particle.vz = %g", velocity.x, velocity.y, velocity.z);
		MPI_Finalize();
		exit(0);
	}
	}*/
	int zcount = floor(((particle->coordinates.z - zgrid[0]) / deltaZ) + 0.5);

	if(particle->coordinates.z < zgrid[zcount]) {
		particle->correlationMapNode.zindex[0] = zcount - 1;
		particle->correlationMapNode.zindex[1] = zcount;
		double w = 0.5 + (particle->coordinates.z - middleZgrid[zcount - 1])/deltaZ;
		particle->correlationMapNode.zcorrelation[0] = 1.0 - w;
		particle->correlationMapNode.zcorrelation[1] = w;
	} else {
		particle->correlationMapNode.zindex[0] = zcount;
		particle->correlationMapNode.zindex[1] = zcount+1;
		double w = 0.5 + (middleZgrid[zcount] - particle->coordinates.z)/deltaZ;
		particle->correlationMapNode.zcorrelation[0] = w;
		particle->correlationMapNode.zcorrelation[1] = 1.0 - w;
	}
}

void Simulation::updateBunemanCorrelationMapNode(Particle* particle) {
	updateBunemanCorrelationMapNodeX(particle);
	if(ynumberGeneral > 1){
		updateBunemanCorrelationMapNodeY(particle);
	}
	if(znumberGeneral > 1){
		updateBunemanCorrelationMapNodeZ(particle);
	}

	/*double fullCorrelation = 0;
	for (int i = 0; i < splineOrder + 2; ++i) {
		for (int j = 0; j < splineOrder + 2; ++j) {
			for (int k = 0; k < splineOrder + 2; ++k) {
				fullCorrelation += particle.correlationMapNode.xcorrelation[i] * particle.correlationMapNode.ycorrelation[j] *
					particle.correlationMapNode.zcorrelation[k];
			}
		}
	}
	if ((fullCorrelation < 1.0 - 1E-10) || (fullCorrelation > 1.0 + 1E-10)) {
		printf("full correlation node = %20.15g\n", fullCorrelation);
	}*/
}
