#include <stdio.h>
#include <math.h>
#include <mpi.h>
//#include <crtdbg.h>

//#include "memory_debug.h"
#include "complex.h"
#include "particle.h"
#include "simulation.h"
#include "util.h"
#include "constants.h"
#include "matrixElement.h"
#include "random.h"
#include "specialmath.h"
#include "mpi_util.h"
#include "paths.h"

double evaluateError(double** hessenbergMatrix, double* vector, double beta, int n) {
	double* resVector = new double[n + 1];

	for (int i = 0; i < n + 1; ++i) {
		resVector[i] = 0;
		for (int j = 0; j < n; ++j) {
			resVector[i] += hessenbergMatrix[i][j] * vector[j];
		}
		if (i == 0) {
			resVector[i] -= beta;
		}
	}

	double norm = 0;
	for (int i = 0; i < n + 1; ++i) {
		norm += resVector[i] * resVector[i];
	}

	delete[] resVector;

	return sqrt(norm);
}

double**** multiplySpecialMatrixVector(std::vector<MatrixElement>**** matrix, Vector3d*** vector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	double**** result = new double***[xnumberAdded];

	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 2 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 2 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 2 - additionalBinNumber;

	for (int i = 0; i < xnumberAdded; ++i) {
		result[i] = new double**[ynumberAdded];
		for (int j = 0; j < ynumberAdded; ++j) {
			result[i][j] = new double*[znumberAdded];
			for (int k = 0; k < znumberAdded; ++k) {
				result[i][j][k] = new double[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
				}
			}
		}
	}

	for (int i = minI; i <= maxI; ++i) {
		//for (i = 1; i < xnumber; ++i) {
		//result[i] = new double**[ynumberAdded];
		for (int j = minJ; j <= maxJ; ++j) {
			//result[i][j] = new double*[znumberAdded];
			for (int k = minK; k <= maxK; ++k) {
				//result[i][j][k] = new double[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						MatrixElement element = matrix[i][j][k][l][m];

						result[i][j][k][l] += element.value * vector[element.i][element.j][element.k][element.l];
					}
				}
			}
		}
	}

	return result;
}

double**** multiplySpecialMatrixVector(std::vector<MatrixElement>**** matrix, double**** vector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	double**** result = new double***[xnumberAdded];

	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 2 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 2 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 2 - additionalBinNumber;

	for (int i = 0; i < xnumberAdded; ++i) {
		result[i] = new double**[ynumberAdded];
		for (int j = 0; j < ynumberAdded; ++j) {
			result[i][j] = new double*[znumberAdded];
			for (int k = 0; k < znumberAdded; ++k) {
				result[i][j][k] = new double[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
				}
			}
		}
	}

	for (int i = minI; i <= maxI; ++i) {
		//for (i = 1; i < xnumber; ++i) {
		//result[i] = new double**[ynumberAdded];
		for (int j = minJ; j <= maxJ; ++j) {
			//result[i][j] = new double*[znumberAdded];
			for (int k = minK; k <= maxK; ++k) {
				//result[i][j][k] = new double[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						MatrixElement element = matrix[i][j][k][l][m];

						result[i][j][k][l] += element.value * vector[element.i][element.j][element.k][element.l];
					}
				}
			}
		}
	}

	return result;
}

void multiplySpecialMatrixVector(double**** result, std::vector<MatrixElement>**** matrix, Vector3d*** vector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 2 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 2 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 2 - additionalBinNumber;

	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
				}
			}
		}
	}

	for (int i = minI; i <= maxI; ++i) {
		for (int j = minJ; j <= maxJ; ++j) {
			for (int k = minK; k <= maxK; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						MatrixElement element = matrix[i][j][k][l][m];

						result[i][j][k][l] += element.value * vector[element.i][element.j][element.k][element.l];
					}
				}
			}
		}
	}
}

void multiplySpecialMatrixVector(double**** result, std::vector<MatrixElement>**** matrix, double**** vector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 2 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 2 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 2 - additionalBinNumber;

	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
				}
			}
		}
	}

	for (int i = minI; i <= maxI; ++i) {
		for (int j = minJ; j <= maxJ; ++j) {
			for (int k = minK; k <= maxK; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					result[i][j][k][l] = 0;
					MatrixElement firstElement = matrix[i][j][k][l][0];
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						MatrixElement element = matrix[i][j][k][l][m];

						result[i][j][k][l] += element.value * vector[element.i][element.j][element.k][element.l];
					}
				}
			}
		}
	}
}

void arnoldiIterations(std::vector<MatrixElement>**** matrix, double** outHessenbergMatrix, int n,
                       LargeVectorBasis* gmresBasis, double** prevHessenbergMatrix, int xnumberAdded, int ynumberAdded,
                       int znumberAdded, int additionalBinNumber, int lnumber, bool periodic, int rank, int nprocs, double* leftOutGmresBuffer, double* rightOutGmresBuffer, double* leftInGmresBuffer, double* rightInGmresBuffer, double* frontOutGmresBuffer, double* backOutGmresBuffer, double* frontInGmresBuffer, double* backInGmresBuffer, double* bottomOutGmresBuffer, double* topOutGmresBuffer, double* bottomInGmresBuffer, double* topInGmresBuffer, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {

	MPI_Barrier(cartComm);

	for (int i = 0; i < n; ++i) {
		if (i < n - 1) {
			for (int j = 0; j < n - 2; ++j) {
				outHessenbergMatrix[i][j] = prevHessenbergMatrix[i][j];
			}
			delete[] prevHessenbergMatrix[i];
		} else {
			for (int j = 0; j < n - 2; ++j) {
				outHessenbergMatrix[i][j] = 0;
			}
		}
	}
	delete[] prevHessenbergMatrix;
	//printf("update hessenberg\n");
	if (n >= gmresBasis->capacity) {
		gmresBasis->resize(2 * n);
	}
	multiplySpecialMatrixVector(gmresBasis->array[n - 1], matrix, gmresBasis->array[n - 2], xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
	gmresBasis->size += 1;
	//printf("mult special matrix");

	//printf("start exchange\n");

	MPI_Barrier(cartComm);

	if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
		sendLargeVectorToLeftReceiveFromRight(gmresBasis->array[n - 1], leftOutGmresBuffer, rightInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == 0) {
		receiveLargeVectorFromRight(gmresBasis->array[n - 1], rightInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == cartDim[0] - 1) {
		sendLargeVectorToLeft(gmresBasis->array[n - 1], leftOutGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	}

	if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
		sendLargeVectorToRightReceiveFromLeft(gmresBasis->array[n - 1], rightOutGmresBuffer, leftInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == 0) {
		sendLargeVectorToRight(gmresBasis->array[n - 1], rightOutGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == cartDim[0] - 1) {
		receiveLargeVectorFromLeft(gmresBasis->array[n - 1], leftInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	}

	sendLargeVectorToFrontReceiveFromBack(gmresBasis->array[n - 1], frontOutGmresBuffer, backInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	sendLargeVectorToBackReceiveFromFront(gmresBasis->array[n - 1], backOutGmresBuffer, frontInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);

	sendLargeVectorToBottomReceiveFromTop(gmresBasis->array[n - 1], bottomOutGmresBuffer, topInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	sendLargeVectorToTopReceiveFromBottom(gmresBasis->array[n - 1], topOutGmresBuffer, bottomInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);


	for (int m = 0; m < n - 1; ++m) {
		//double a = scalarMultiplyLargeVectors(resultBasis[m], tempVector, xnumber, ynumber, znumber, lnumber);
		outHessenbergMatrix[m][n - 2] = scalarMultiplyLargeVectors(gmresBasis->array[m], gmresBasis->array[n - 1], xnumberAdded, ynumberAdded,
		                                                           znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		//printf("outHessenbergMatrix[%d][%d] = %g\n", m, n-2, outHessenbergMatrix[m][n - 2]);

		//for (int i = 0; i < xnumber+1; ++i) {

		int minI = 1 + additionalBinNumber;
		if (cartCoord[0] == 0 && !periodic) {
			minI = 0;
		}
		int maxI = xnumberAdded - 2 - additionalBinNumber;
		if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
			maxI = xnumberAdded - 1;
		}
		int minJ = 1 + additionalBinNumber;
		int maxJ = ynumberAdded - 2 - additionalBinNumber;
		int minK = 1 + additionalBinNumber;
		int maxK = znumberAdded - 2 - additionalBinNumber;

		for (int i = 0; i < xnumberAdded; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						if (i >= minI && i <= maxI && j >= minJ && j <= maxJ && k >= minK && k <= maxK) {
							gmresBasis->array[n - 1][i][j][k][l] -= outHessenbergMatrix[m][n - 2] * gmresBasis->array[m][i][j][k][l];
						} else {
							gmresBasis->array[n - 1][i][j][k][l] = 0;
						}
					}
				}
			}
		}
	}
	//printf("finish orthogonalisation\n");
	outHessenbergMatrix[n - 1][n - 2] = sqrt(
		scalarMultiplyLargeVectors(gmresBasis->array[n - 1], gmresBasis->array[n - 1], xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim));
	//printf("outHessenbergMatrix[%d][%d] = %g\n", n-1, n-2, outHessenbergMatrix[n - 1][n - 2]);
	if (outHessenbergMatrix[n - 1][n - 2] > 0) {
		for (int i = 0; i < xnumberAdded; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						gmresBasis->array[n - 1][i][j][k][l] /= outHessenbergMatrix[n - 1][n - 2];
					}
				}
			}
		}

		MPI_Barrier(cartComm);	
	} else {
		printf("outHessenbergMatrix[n-1][n-2] == 0\n");
		for (int i = 0; i < xnumberAdded; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						gmresBasis->array[n - 1][i][j][k][l] = 0;
					}
				}
			}
		}
	}

	if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
			sendLargeVectorToLeftReceiveFromRight(gmresBasis->array[n - 1], leftOutGmresBuffer, rightInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == 0) {
			receiveLargeVectorFromRight(gmresBasis->array[n - 1], rightInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == cartDim[0] - 1) {
			sendLargeVectorToLeft(gmresBasis->array[n - 1], leftOutGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		}

		if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
			sendLargeVectorToRightReceiveFromLeft(gmresBasis->array[n - 1], rightOutGmresBuffer, leftInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == 0) {
			sendLargeVectorToRight(gmresBasis->array[n - 1], rightOutGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == cartDim[0] - 1) {
			receiveLargeVectorFromLeft(gmresBasis->array[n - 1], leftInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		}

		sendLargeVectorToFrontReceiveFromBack(gmresBasis->array[n - 1], frontOutGmresBuffer, backInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		sendLargeVectorToBackReceiveFromFront(gmresBasis->array[n - 1], backOutGmresBuffer, frontInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);

		sendLargeVectorToBottomReceiveFromTop(gmresBasis->array[n - 1], bottomOutGmresBuffer, topInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		sendLargeVectorToTopReceiveFromBottom(gmresBasis->array[n - 1], topOutGmresBuffer, bottomInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
}

void generalizedMinimalResidualMethod(std::vector<MatrixElement>**** matrix, double**** rightPart, double**** outvector,
                                      int xnumberAdded, int ynumberAdded, int znumberAdded, int lnumber, int xnumberGeneral,
                                      int znumberGeneral, int ynumberGeneral, int additionalBinNumber, double precision,
                                      int maxIteration, bool periodic, int verbocity, double* leftOutGmresBuffer, double* rightOutGmresBuffer, double* leftInGmresBuffer, double* rightInGmresBuffer, double* frontOutGmresBuffer, double* backOutGmresBuffer, double* frontInGmresBuffer, double* backInGmresBuffer, double* bottomOutGmresBuffer, double* topOutGmresBuffer, double* bottomInGmresBuffer, double* topInGmresBuffer, LargeVectorBasis* gmresBasis, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int rank;
	int nprocs;
	MPI_Comm_size(cartComm, &nprocs);
	MPI_Comm_rank(cartComm, &rank);

	MPI_Barrier(cartComm);
	if ((rank == 0) && (verbocity > 0)) printf("start GMRES\n");


	MPI_Barrier(cartComm);
	exchangeLargeVector(rightPart, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutGmresBuffer, rightOutGmresBuffer, leftInGmresBuffer, rightInGmresBuffer, frontOutGmresBuffer, backOutGmresBuffer, frontInGmresBuffer, backInGmresBuffer, bottomOutGmresBuffer, topOutGmresBuffer, bottomInGmresBuffer, topInGmresBuffer);

	double norm = sqrt(scalarMultiplyLargeVectors(rightPart, rightPart, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim));
	//printf("norm = %g\n", norm);
	alertNaNOrInfinity(norm, "right partnorm = NaN in gmres\n");

	if (norm == 0) {
		for (int i = 0; i < xnumberAdded; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						outvector[i][j][k][l] = 0;
					}
				}
			}
		}
		return;
	}

	//#pragma omp parallel for
	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					rightPart[i][j][k][l] /= norm;
				}
			}
		}
	}

	double normRightPart = sqrt(scalarMultiplyLargeVectors(rightPart, rightPart, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim));
	int matrixDimension = lnumber * xnumberGeneral * ynumberGeneral * znumberGeneral;

	double** hessenbergMatrix;
	double** newHessenbergMatrix;
	hessenbergMatrix = new double*[1];
	hessenbergMatrix[0] = new double[1];
	hessenbergMatrix[0][0] = 0;

	double** Qmatrix = new double*[2];
	double** Rmatrix = new double*[2];
	double** oldQmatrix = new double*[2];
	double** oldRmatrix = new double*[2];

	for (int i = 0; i < 2; ++i) {
		Qmatrix[i] = new double[2];
		oldQmatrix[i] = new double[2];
	}

	Rmatrix[0] = new double[1];
	Rmatrix[1] = new double[1];
	oldRmatrix[0] = new double[1];
	//oldQmatrix[0] = new double[1];
	oldRmatrix[1] = new double[1];

	if (gmresBasis->capacity <= 0) {
		gmresBasis->resize(10);
	}
	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					gmresBasis->array[0][i][j][k][l] = rightPart[i][j][k][l];
				}
			}
		}
	}
	gmresBasis->size = 1;

	int n = 2;
	double beta = 1.0;
	double error = beta;
	double* y = new double[1];

	double rho;
	double sigma;
	double cosn;
	double sinn;
	double module;

	double relativeError = 1;
	//double maxRelativeError = maxErrorLevel / (matrixDimension);
	double maxRelativeError = precision / (matrixDimension);
	//double maxRelativeError = precision;

	while ((relativeError > max2(maxRelativeError, 1E-15) && (n < min2(maxIteration, matrixDimension + 3)))) {
		if ((rank == 0) && (verbocity > 1)) printf("GMRES iteration %d\n", n);
		newHessenbergMatrix = new double*[n];
		for (int i = 0; i < n; ++i) {
			newHessenbergMatrix[i] = new double[n - 1];
		}
		arnoldiIterations(matrix, newHessenbergMatrix, n, gmresBasis, hessenbergMatrix, xnumberAdded, ynumberAdded, znumberAdded,
		                  additionalBinNumber, lnumber, periodic, rank, nprocs, leftOutGmresBuffer, rightOutGmresBuffer, leftInGmresBuffer, rightInGmresBuffer, frontOutGmresBuffer, backOutGmresBuffer, frontInGmresBuffer, backInGmresBuffer, bottomOutGmresBuffer, topOutGmresBuffer, bottomInGmresBuffer, topInGmresBuffer, cartComm, cartCoord, cartDim);

		hessenbergMatrix = newHessenbergMatrix;

		if (n == 2) {
			rho = hessenbergMatrix[0][0];
			sigma = hessenbergMatrix[1][0];

			module = sqrt(rho * rho + sigma * sigma);

			cosn = rho / module;
			sinn = sigma / module;

			Qmatrix[0][0] = cosn;
			Qmatrix[0][1] = sinn;
			Qmatrix[1][0] = -sinn;
			Qmatrix[1][1] = cosn;

			oldQmatrix[0][0] = Qmatrix[0][0];
			oldQmatrix[0][1] = Qmatrix[0][1];
			oldQmatrix[1][0] = Qmatrix[1][0];
			oldQmatrix[1][1] = Qmatrix[1][1];

			Rmatrix[0][0] = module;
			Rmatrix[1][0] = 0;

			oldRmatrix[0][0] = Rmatrix[0][0];
			oldRmatrix[1][0] = Rmatrix[1][0];

		} else {
			Rmatrix = new double*[n];
			for (int i = 0; i < n; ++i) {
				Rmatrix[i] = new double[n - 1];
				if (i < n - 1) {
					for (int j = 0; j < n - 2; ++j) {
						Rmatrix[i][j] = oldRmatrix[i][j];
					}
				} else {
					for (int j = 0; j < n - 2; ++j) {
						Rmatrix[i][j] = 0;
					}
				}
			}

			Qmatrix = new double*[n];
			for (int i = 0; i < n; ++i) {
				Qmatrix[i] = new double[n];
				if (i < n - 1) {
					for (int j = 0; j < n - 1; ++j) {
						Qmatrix[i][j] = oldQmatrix[i][j];
					}
					Qmatrix[i][n - 1] = 0;
				} else {
					for (int j = 0; j < n - 1; ++j) {
						Qmatrix[i][j] = 0;
					}
					Qmatrix[n - 1][n - 1] = 1;
				}
			}

			for (int i = 0; i < n; ++i) {
				Rmatrix[i][n - 2] = 0;
				for (int j = 0; j < n; ++j) {
					Rmatrix[i][n - 2] += Qmatrix[i][j] * hessenbergMatrix[j][n - 2];
				}
			}
			rho = Rmatrix[n - 2][n - 2];
			sigma = Rmatrix[n - 1][n - 2];

			module = sqrt(rho * rho + sigma * sigma);

			cosn = rho / module;
			sinn = sigma / module;

			Rmatrix[n - 2][n - 2] = module;
			Rmatrix[n - 1][n - 2] = 0;

			for (int j = 0; j < n - 1; ++j) {
				Qmatrix[n - 2][j] = cosn * oldQmatrix[n - 2][j];
				Qmatrix[n - 1][j] = -sinn * oldQmatrix[n - 2][j];
			}
			Qmatrix[n - 2][n - 1] = sinn;
			Qmatrix[n - 1][n - 1] = cosn;
		}

		delete[] y;
		y = new double[n - 1];
		//printf("n = %d\n", n);

		for (int i = n - 2; i >= 0; --i) {
			y[i] = beta * Qmatrix[i][0];
			//printf("y[%d] = %g\n", i, y[i]);
			for (int j = n - 2; j > i; --j) {
				y[i] -= Rmatrix[i][j] * y[j];
			}
			if (Rmatrix[i][i] > 0) {
				y[i] /= Rmatrix[i][i];
			} else {
				y[i] = 0;
				printf("Rmatrix[%d][%d] = 0\n", i, i);
			}
			//printf("y[%d] = %g\n", i, y[i]);
			alertNaNOrInfinity(y[i], "y = NaN\n");
		}

		error = fabs(beta * Qmatrix[n - 1][0]);

		relativeError = error / normRightPart;

		for (int i = 0; i < n - 1; ++i) {
			delete[] oldQmatrix[i];
			delete[] oldRmatrix[i];
		}
		if (n == 2) {
			delete[] oldQmatrix[1];
			delete[] oldRmatrix[1];
		}
		delete[] oldQmatrix;
		delete[] oldRmatrix;

		oldQmatrix = Qmatrix;
		oldRmatrix = Rmatrix;

		n++;
	}

	n = n - 1;
	//if(rank == 0) printf("total GMRES iteration = %d\n", n);

	//out result


	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					outvector[i][j][k][l] = 0;
					for (int m = 0; m < n - 1; ++m) {
						outvector[i][j][k][l] += gmresBasis->array[m][i][j][k][l] * y[m] * norm;
						//outvector[i][l] += basis[m][i][l] * y[m];
					}
				}
			}
		}
	}

	gmresBasis->clear();

	//printf("rank = %d outvector[0][0][0][1] = %g\n", rank, outvector[0][0][0][1]);
	//printf("rank = %d outvector[1][0][0][1] = %g\n", rank, outvector[1][0][0][1]);
	//printf("rank = %d outvector[2][0][0][1] = %g\n", rank, outvector[2][0][0][1]);
	//printf("rank = %d outvector[xnumber - 2][0][0][1] = %g\n", rank, outvector[xnumber - 2][0][0][1]);
	//printf("rank = %d outvector[xnumber - 1][0][0][1] = %g\n", rank, outvector[xnumber - 1][0][0][1]);
	//printf("rank = %d outvector[xnumber][0][0][1] = %g\n", rank, outvector[xnumber][0][0][1]);

	MPI_Barrier(cartComm);

	if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
		sendLargeVectorToLeftReceiveFromRight(outvector, leftOutGmresBuffer, rightInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == 0) {
		receiveLargeVectorFromRight(outvector, rightInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == cartDim[0] - 1) {
		sendLargeVectorToLeft(outvector, leftOutGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	}

	if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
		sendLargeVectorToRightReceiveFromLeft(outvector, rightOutGmresBuffer, leftInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == 0) {
		sendLargeVectorToRight(outvector, rightOutGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	} else if (cartCoord[0] == cartDim[0] - 1) {
		receiveLargeVectorFromLeft(outvector, leftInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	}

	sendLargeVectorToFrontReceiveFromBack(outvector, frontOutGmresBuffer, backInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	sendLargeVectorToBackReceiveFromFront(outvector, backOutGmresBuffer, frontInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);

	sendLargeVectorToBottomReceiveFromTop(outvector, bottomOutGmresBuffer, topInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
	sendLargeVectorToTopReceiveFromBottom(outvector, topOutGmresBuffer, bottomInGmresBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);

	//printf("rank = %d outvector[0][0][0][1] = %g\n", rank, outvector[0][0][0][1]);
	//printf("rank = %d outvector[1][0][0][1] = %g\n", rank, outvector[1][0][0][1]);
	//printf("rank = %d outvector[2][0][0][1] = %g\n", rank, outvector[2][0][0][1]);
	//printf("rank = %d outvector[xnumber - 2][0][0][1] = %g\n", rank, outvector[xnumber - 2][0][0][1]);
	//printf("rank = %d outvector[xnumber - 1][0][0][1] = %g\n", rank, outvector[xnumber - 1][0][0][1]);
	//printf("rank = %d outvector[xnumber][0][0][1] = %g\n", rank, outvector[xnumber][0][0][1]);

	for (int i = 0; i < n; ++i) {
		delete[] Qmatrix[i];
		delete[] Rmatrix[i];
		delete[] hessenbergMatrix[i];
	}
	delete[] Qmatrix;
	delete[] Rmatrix;
	delete[] hessenbergMatrix;
	delete[] y;

	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					rightPart[i][j][k][l] *= norm;
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						double value = matrix[i][j][k][l][m].value;
						//matrix[i][l][m].value *= norm;
						value = matrix[i][j][k][l][m].value;
					}
				}
			}
		}
	}
}

double scalarMultiplyLargeVectors(double**** a, double**** b, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber,
                                  int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 2 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 2 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 2 - additionalBinNumber;

	double result[1];
	double globalResult[1];
	globalResult[0] = 0;
	result[0] = 0;

	for (int i = minI; i <= maxI; ++i) {
		for (int j = minJ; j <= maxJ; ++j) {
			for (int k = minK; k <= maxK; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					result[0] += a[i][j][k][l] * b[i][j][k][l];
					alertNaNOrInfinity(a[i][j][k][l], "a[i][j][k][l] = NaN in scalarMult\n");
					alertNaNOrInfinity(result[0], "result[0] = NaN in scalarMult\n");
				}
			}
		}
	}

	MPI_Allreduce(result, globalResult, 1, MPI_DOUBLE, MPI_SUM, cartComm);
	return globalResult[0];
}

double scalarMultiplyLargeVectors(Vector3d*** a, Vector3d*** b, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber,
                                  int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 2 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 2 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 2 - additionalBinNumber;

	double result[1];
	double globalResult[1];
	globalResult[0] = 0;
	result[0] = 0;

	for (int i = minI; i <= maxI; ++i) {
		for (int j = minJ; j <= maxJ; ++j) {
			for (int k = minK; k <= maxK; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					result[0] += a[i][j][k][l] * b[i][j][k][l];
				}
			}
		}
	}

	MPI_Allreduce(result, globalResult, 1, MPI_DOUBLE, MPI_SUM, cartComm);
	return globalResult[0];
}

void simpleIterationSolver(double**** outVector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, int rank, int nprocs, int xnumberGeneral, int znumberGeneral, int ynumberGeneral, double precision, int maxIteration, bool periodic, int verbocity, double* leftOutBuffer, double* rightOutBuffer, double* leftInBuffer, double* rightInBuffer, double* frontOutBuffer, double* backOutBuffer, double* frontInBuffer, double* backInBuffer, double* bottomOutBuffer, double* topOutBuffer, double* bottomInBuffer, double* topInBuffer, BaseRightPartEvaluator& rightPartEvaluator, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	double**** tempVector = new double***[xnumberAdded + 1];
	for (int i = 0; i < xnumberAdded + 1; ++i) {
		tempVector[i] = new double**[ynumberAdded];
		for (int j = 0; j < ynumberAdded; ++j) {
			tempVector[i][j] = new double*[znumberAdded];
			for (int k = 0; k < znumberAdded; ++k) {
				tempVector[i][j][k] = new double[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					tempVector[i][j][k][l] = 0;
				}
			}
		}
	}
	double maxErrorLevel = precision / (xnumberGeneral * ynumberAdded * znumberAdded * lnumber);
	double relativeError = 1;
	int iterationCount = 0;
	double norm = scalarMultiplyLargeVectors(outVector, outVector, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
	while (iterationCount < maxIteration && relativeError > maxErrorLevel) {
		iterationCount++;
		for (int i = 0; i < xnumberAdded + 1; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						tempVector[i][j][k][l] = rightPartEvaluator.rightPart(outVector, i, j, k, l);
						alertNaNOrInfinity(tempVector[i][j][k][l], "tempvector = NaN in simpleIterationSolver\n");
					}
				}
			}
		}
		if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
			sendLargeVectorToLeftReceiveFromRight(tempVector, leftOutBuffer, rightInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == 0) {
			receiveLargeVectorFromRight(tempVector, rightInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == cartDim[0] - 1) {
			sendLargeVectorToLeft(tempVector, leftOutBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		}

		if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
			sendLargeVectorToRightReceiveFromLeft(tempVector, rightOutBuffer, leftInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == 0) {
			sendLargeVectorToRight(tempVector, rightOutBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == cartDim[0] - 1) {
			receiveLargeVectorFromLeft(tempVector, leftInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		}

		sendLargeVectorToFrontReceiveFromBack(tempVector, frontOutBuffer, backInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		sendLargeVectorToBackReceiveFromFront(tempVector, backOutBuffer, frontInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);

		sendLargeVectorToBottomReceiveFromTop(tempVector, bottomOutBuffer, topInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		sendLargeVectorToTopReceiveFromBottom(tempVector, topOutBuffer, bottomInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);

		relativeError = normDifferenceLargeVectors(tempVector, outVector, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim) / norm;
		double weight = 0.2;
		for (int i = 0; i < xnumberAdded + 1; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						outVector[i][j][k][l] = tempVector[i][j][k][l];
					}
				}
			}
		}
	}

	for (int i = 0; i < xnumberAdded + 1; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				delete[] tempVector[i][j][k];
			}
			delete[] tempVector[i][j];
		}
		delete[] tempVector[i];
	}
	delete[] tempVector;
}

double normDifferenceLargeVectors(double**** a, double**** b, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, bool periodic, int rank, int nprocs, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int minI = 1 + additionalBinNumber;
	if (cartCoord[0] == 0 && !periodic) {
		minI = 0;
	}
	int maxI = xnumberAdded - 1 - additionalBinNumber;
	if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
		maxI = xnumberAdded - 1;
	}
	int minJ = 1 + additionalBinNumber;
	int maxJ = ynumberAdded - 1 - additionalBinNumber;
	int minK = 1 + additionalBinNumber;
	int maxK = znumberAdded - 1 - additionalBinNumber;
	double diff[1];
	double tempDiff[1];
	diff[0] = 0;
	for (int i = minI; i <= maxI; ++i) {
		for (int j = minJ; j <= maxJ; ++j) {
			for (int k = minK; k <= maxK; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					diff[0] += sqr(a[i][j][k][l] - b[i][j][k][l]);
				}
			}
		}
	}

	MPI_Reduce(diff, tempDiff, 1, MPI_DOUBLE, MPI_SUM, 0, cartComm);
	if (rank == 0) {
		tempDiff[0] = sqrt(tempDiff[0]);
	}
	MPI_Bcast(tempDiff, 1, MPI_DOUBLE, 0, cartComm);
	return tempDiff[0];
}

void transposeSpecialMatrix(std::vector<MatrixElement>**** result, std::vector<MatrixElement>**** matrix, int xnumber, int ynumber, int znumber, int lnumber) {
	for (int i = 0; i < xnumber; ++i) {
		for (int j = 0; j < ynumber; ++j) {
			for (int k = 0; k < znumber; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						MatrixElement element = matrix[i][j][k][l][m];

						result[element.i][element.j][element.k][element.l].push_back(MatrixElement(element.value, i, j, k, l));
					}
				}
			}
		}
	}
}

//a4*x^4 + a3*x^3 + a2*x^2 + a1*x + a0 = 0
double solve4orderEquation(double a4, double a3, double a2, double a1, double a0, double startX) {
	double nextX = startX;
	double prevX = startX;

	int iterationCount = 0;
	while ((fabs(nextX - prevX) > fabs(prevX * 1.0E-20) || iterationCount == 0) && (iterationCount < 10000)) {
		prevX = nextX;
		nextX = prevX - polynom4Value(a4, a3, a2, a1, a0, prevX) / polynom4DerivativeValue(a4, a3, a2, a1, prevX);
		iterationCount++;
	}

	return nextX;
}

double polynom4Value(double a4, double a3, double a2, double a1, double a0, double x) {
	return (((a4 * x + a3) * x + a2) * x + a1) * x + a0;
}

double polynom4DerivativeValue(double a4, double a3, double a2, double a1, double x) {
	return ((4.0 * a4 * x + 3.0 * a3) * x + 2.0 * a2) * x + a1;
}

double solveBigOrderEquation(const double* coefficients, int n, double startX) {
	double nextX = startX;
	double prevX = startX;

	int iterationCount = 0;
	while ((fabs(nextX - prevX) > fabs(prevX * 1.0E-20) || iterationCount == 0) && (iterationCount < 10000)) {
		prevX = nextX;
		nextX = prevX - polynomValue(coefficients, prevX, n) / polynomDerivativeValue(coefficients, prevX, n);
		iterationCount++;
	}

	return nextX;
}

double polynomValue(const double* coefficients, double x, int n) {
	double result = coefficients[0];
	for (int i = 1; i <= n; ++i) {
		result = result * x + coefficients[i];
	}
	return result;
}

double polynomDerivativeValue(const double* coefficients, double x, int n) {
	double result = n * coefficients[0];
	for (int i = 1; i < n; ++i) {
		result = result * x + (n - i) * coefficients[i];
	}
	return result;
}

Complex*** fastFourierTransition(double*** a, int xnumber, int ynumber, int znumber) {
	int kx = xnumber;
	while (kx > 1) {
		if (kx % 2 != 0) {
			printf("xnumber is not 2^N\n");
		}
		kx = kx / 2;
	}
	int ky = ynumber;
	while (ky > 1) {
		if (ky % 2 != 0) {
			printf("ynumber is not 2^N\n");
		}
		ky = ky / 2;
	}
	int kz = znumber;
	while (kx > 1) {
		if (kx % 2 != 0) {
			printf("znumber is not 2^N\n");
		}
		kz = kz / 2;
	}

	Complex*** result = new Complex**[xnumber];
	for (int i = 0; i < xnumber; ++i) {
		result[i] = new Complex*[ynumber];
		for (int j = 0; j < ynumber; ++j) {
			result[i][j] = new Complex[znumber];
			for (int k = 0; k < znumber; ++k) {
				result[i][j][k] = Complex(a[i][j][k], 0);
			}
		}
	}
	sortInputFastFourierX(result, xnumber, ynumber, znumber);

	Complex* tempResult;
	if (xnumber > 1) {
		tempResult = new Complex[xnumber];
		for (int ycount = 0; ycount < ynumber; ++ycount) {
			for (int zcount = 0; zcount < znumber; ++zcount) {
				for (int i = 0; i < xnumber / 2; ++i) {
					result[i * 2][ycount][zcount] = result[i * 2][ycount][zcount] + result[i * 2 + 1][ycount][zcount];
					result[i * 2 + 1][ycount][zcount] = result[i * 2][ycount][zcount] - result[i * 2 + 1][ycount][zcount];
				}

				int k = 4;
				while (k <= xnumber) {
					int l = xnumber / k;
					for (int i = 0; i < l; ++i) {
						for (int m = 0; m < k / 2; ++m) {
							tempResult[i * k + m] = result[i * k + m][ycount][zcount] + result[i * k + (k / 2) + m][ycount][zcount] * complexExp(
								-2 * pi * m / k);
							tempResult[i * k + (k / 2) + m] = result[i * k + m][ycount][zcount] - result[i * k + (k / 2) + m][ycount][zcount] * complexExp(
								-2 * pi * m / k);
						}
					}

					for (int i = 0; i < xnumber; ++i) {
						result[i][ycount][zcount] = tempResult[i];
					}
					k = k * 2;
				}
			}
		}
		delete[] tempResult;
	}

	if (ynumber > 1) {
		sortInputFastFourierY(result, xnumber, ynumber, znumber);
		tempResult = new Complex[ynumber];
		for (int xcount = 0; xcount < xnumber; ++xcount) {
			for (int zcount = 0; zcount < znumber; ++zcount) {
				for (int i = 0; i < ynumber / 2; ++i) {
					result[xcount][i * 2][zcount] = result[xcount][i * 2][zcount] + result[xcount][i * 2 + 1][zcount];
					result[xcount][i * 2 + 1][zcount] = result[xcount][i * 2][zcount] - result[xcount][i * 2 + 1][zcount];
				}

				int k = 4;
				while (k <= ynumber) {
					int l = ynumber / k;
					for (int i = 0; i < l; ++i) {
						for (int m = 0; m < k / 2; ++m) {
							tempResult[i * k + m] = result[xcount][i * k + m][zcount] + result[xcount][i * k + (k / 2) + m][zcount] * complexExp(
								-2 * pi * m / k);
							tempResult[i * k + (k / 2) + m] = result[xcount][i * k + m][zcount] - result[xcount][i * k + (k / 2) + m][zcount] * complexExp(
								-2 * pi * m / k);
						}
					}

					for (int i = 0; i < ynumber; ++i) {
						result[xcount][i][zcount] = tempResult[i];
					}
					k = k * 2;
				}
			}
		}
		delete[] tempResult;
	}

	if (znumber > 1) {
		sortInputFastFourierY(result, xnumber, ynumber, znumber);
		tempResult = new Complex[znumber];
		for (int xcount = 0; xcount < xnumber; ++xcount) {
			for (int ycount = 0; ycount < ynumber; ++ycount) {
				for (int i = 0; i < znumber / 2; ++i) {
					result[xcount][ycount][i * 2] = result[xcount][ycount][i * 2] + result[xcount][ycount][i * 2 + 1];
					result[xcount][ycount][i * 2 + 1] = result[xcount][ycount][i * 2] - result[xcount][ycount][i * 2 + 1];
				}

				int k = 4;
				while (k <= znumber) {
					int l = znumber / k;
					for (int i = 0; i < l; ++i) {
						for (int m = 0; m < k / 2; ++m) {
							tempResult[i * k + m] = result[xcount][ycount][i * k + m] + result[xcount][ycount][i * k + (k / 2) + m] * complexExp(
								-2 * pi * m / k);
							tempResult[i * k + (k / 2) + m] = result[xcount][ycount][i * k + m] - result[xcount][ycount][i * k + (k / 2) + m] * complexExp(
								-2 * pi * m / k);
						}
					}

					for (int i = 0; i < znumber; ++i) {
						result[xcount][ycount][i] = tempResult[i];
					}
					k = k * 2;
				}
			}
		}
		delete[] tempResult;
	}


	return result;
}

void sortInputFastFourierX(double*** a, int xnumber, int ynumber, int znumber) {
	double* tempA = new double[xnumber];
	for (int ycount = 0; ycount < ynumber; ++ycount) {
		for (int zcount = 0; zcount < znumber; ++zcount) {
			int k = xnumber;

			while (k > 2) {
				int m = xnumber / k;

				for (int i = 0; i < m; ++i) {
					for (int j = 0; j < k / 2; ++j) {
						tempA[i * k + j] = a[i * k + 2 * j][ycount][zcount];
						tempA[i * k + (k / 2) + j] = a[i * k + 2 * j + 1][ycount][zcount];
					}
				}

				for (int i = 0; i < xnumber; ++i) {
					a[i][ycount][zcount] = tempA[i];
				}

				k = k / 2;
			}
		}
	}
	delete[] tempA;
}

void sortInputFastFourierY(double*** a, int xnumber, int ynumber, int znumber) {
	double* tempA = new double[ynumber];
	for (int xcount = 0; xcount < xnumber; ++xcount) {
		for (int zcount = 0; zcount < znumber; ++zcount) {
			int k = ynumber;

			while (k > 2) {
				int m = ynumber / k;

				for (int i = 0; i < m; ++i) {
					for (int j = 0; j < k / 2; ++j) {
						tempA[i * k + j] = a[xcount][i * k + 2 * j][zcount];
						tempA[i * k + (k / 2) + j] = a[xcount][i * k + 2 * j + 1][zcount];
					}
				}

				for (int i = 0; i < ynumber; ++i) {
					a[xcount][i][zcount] = tempA[i];
				}

				k = k / 2;
			}
		}
	}
	delete[] tempA;
}

void sortInputFastFourierZ(double*** a, int xnumber, int ynumber, int znumber) {
	double* tempA = new double[znumber];
	for (int xcount = 0; xcount < xnumber; ++xcount) {
		for (int ycount = 0; ycount < ynumber; ++ycount) {
			int k = znumber;

			while (k > 2) {
				int m = znumber / k;

				for (int i = 0; i < m; ++i) {
					for (int j = 0; j < k / 2; ++j) {
						tempA[i * k + j] = a[xcount][ycount][i * k + 2 * j];
						tempA[i * k + (k / 2) + j] = a[xcount][ycount][i * k + 2 * j + 1];
					}
				}

				for (int i = 0; i < znumber; ++i) {
					a[xcount][ycount][i] = tempA[i];
				}

				k = k / 2;
			}
		}
	}
	delete[] tempA;
}

double*** fastFourierReverceTransition(Complex*** a, int xnumber, int ynumber, int znumber) {
	int kx = xnumber;
	while (kx > 1) {
		if (kx % 2 != 0) {
			printf("xnumber is not 2^N\n");
		}
		kx = kx / 2;
	}
	int ky = ynumber;
	while (ky > 1) {
		if (ky % 2 != 0) {
			printf("ynumber is not 2^N\n");
		}
		ky = ky / 2;
	}
	int kz = znumber;
	while (kx > 1) {
		if (kx % 2 != 0) {
			printf("znumber is not 2^N\n");
		}
		kz = kz / 2;
	}

	Complex*** result = new Complex**[xnumber];
	for (int i = 0; i < xnumber; ++i) {
		result[i] = new Complex*[ynumber];
		for (int j = 0; j < ynumber; ++j) {
			result[i][j] = new Complex[znumber];
			for (int k = 0; k < znumber; ++k) {
				result[i][j][k] = a[i][j][k];
			}
		}
	}
	sortInputFastFourierX(result, xnumber, ynumber, znumber);

	Complex* tempResult;
	if (xnumber > 1) {
		tempResult = new Complex[xnumber];
		for (int ycount = 0; ycount < ynumber; ++ycount) {
			for (int zcount = 0; zcount < znumber; ++zcount) {
				for (int i = 0; i < xnumber / 2; ++i) {
					result[i * 2][ycount][zcount] = result[i * 2][ycount][zcount] + result[i * 2 + 1][ycount][zcount];
					result[i * 2 + 1][ycount][zcount] = result[i * 2][ycount][zcount] - result[i * 2 + 1][ycount][zcount];
				}

				int k = 4;
				while (k <= xnumber) {
					int l = xnumber / k;
					for (int i = 0; i < l; ++i) {
						for (int m = 0; m < k / 2; ++m) {
							tempResult[i * k + m] = result[i * k + m][ycount][zcount] + result[i * k + (k / 2) + m][ycount][zcount] * complexExp(
								2 * pi * m / k);
							tempResult[i * k + (k / 2) + m] = result[i * k + m][ycount][zcount] - result[i * k + (k / 2) + m][ycount][zcount] * complexExp(
								2 * pi * m / k);
						}
					}

					for (int i = 0; i < xnumber; ++i) {
						result[i][ycount][zcount] = tempResult[i];
					}
					k = k * 2;
				}
			}
		}
		delete[] tempResult;
	}

	if (ynumber > 1) {
		sortInputFastFourierY(result, xnumber, ynumber, znumber);
		tempResult = new Complex[ynumber];
		for (int xcount = 0; xcount < xnumber; ++xcount) {
			for (int zcount = 0; zcount < znumber; ++zcount) {
				for (int i = 0; i < ynumber / 2; ++i) {
					result[xcount][i * 2][zcount] = result[xcount][i * 2][zcount] + result[xcount][i * 2 + 1][zcount];
					result[xcount][i * 2 + 1][zcount] = result[xcount][i * 2][zcount] - result[xcount][i * 2 + 1][zcount];
				}

				int k = 4;
				while (k <= ynumber) {
					int l = ynumber / k;
					for (int i = 0; i < l; ++i) {
						for (int m = 0; m < k / 2; ++m) {
							tempResult[i * k + m] = result[xcount][i * k + m][zcount] + result[xcount][i * k + (k / 2) + m][zcount] * complexExp(
								2 * pi * m / k);
							tempResult[i * k + (k / 2) + m] = result[xcount][i * k + m][zcount] - result[xcount][i * k + (k / 2) + m][zcount] * complexExp(
								2 * pi * m / k);
						}
					}

					for (int i = 0; i < ynumber; ++i) {
						result[xcount][i][zcount] = tempResult[i];
					}
					k = k * 2;
				}
			}
		}
		delete[] tempResult;
	}

	if (znumber > 1) {
		sortInputFastFourierY(result, xnumber, ynumber, znumber);
		tempResult = new Complex[znumber];
		for (int xcount = 0; xcount < xnumber; ++xcount) {
			for (int ycount = 0; ycount < ynumber; ++ycount) {
				for (int i = 0; i < znumber / 2; ++i) {
					result[xcount][ycount][i * 2] = result[xcount][ycount][i * 2] + result[xcount][ycount][i * 2 + 1];
					result[xcount][ycount][i * 2 + 1] = result[xcount][ycount][i * 2] - result[xcount][ycount][i * 2 + 1];
				}

				int k = 4;
				while (k <= znumber) {
					int l = znumber / k;
					for (int i = 0; i < l; ++i) {
						for (int m = 0; m < k / 2; ++m) {
							tempResult[i * k + m] = result[xcount][ycount][i * k + m] + result[xcount][ycount][i * k + (k / 2) + m] * complexExp(
								2 * pi * m / k);
							tempResult[i * k + (k / 2) + m] = result[xcount][ycount][i * k + m] - result[xcount][ycount][i * k + (k / 2) + m] * complexExp(
								2 * pi * m / k);
						}
					}

					for (int i = 0; i < znumber; ++i) {
						result[xcount][ycount][i] = tempResult[i];
					}
					k = k * 2;
				}
			}
		}
		delete[] tempResult;
	}

	double*** realPart = new double**[xnumber];
	for (int i = 0; i < xnumber; ++i) {
		realPart[i] = new double*[ynumber];
		for (int j = 0; j < ynumber; ++j) {
			realPart[i][j] = new double[znumber];
			for (int k = 0; k < znumber; ++k) {
				realPart[i][j][k] = result[i][j][k].re / (xnumber * ynumber * znumber);
			}
			delete[] result[i][j];
		}
		delete[] result[i];
	}

	delete[] result;

	return realPart;
}

void sortInputFastFourierX(Complex*** a, int xnumber, int ynumber, int znumber) {
	Complex* tempA = new Complex[xnumber];
	for (int ycount = 0; ycount < ynumber; ++ycount) {
		for (int zcount = 0; zcount < znumber; ++zcount) {
			int k = xnumber;

			while (k > 2) {
				int m = xnumber / k;

				for (int i = 0; i < m; ++i) {
					for (int j = 0; j < k / 2; ++j) {
						tempA[i * k + j] = a[i * k + 2 * j][ycount][zcount];
						tempA[i * k + (k / 2) + j] = a[i * k + 2 * j + 1][ycount][zcount];
					}
				}

				for (int i = 0; i < xnumber; ++i) {
					a[i][ycount][zcount] = tempA[i];
				}

				k = k / 2;
			}
		}
	}
	delete[] tempA;
}

void sortInputFastFourierY(Complex*** a, int xnumber, int ynumber, int znumber) {
	Complex* tempA = new Complex[ynumber];
	for (int xcount = 0; xcount < xnumber; ++xcount) {
		for (int zcount = 0; zcount < znumber; ++zcount) {
			int k = ynumber;

			while (k > 2) {
				int m = ynumber / k;

				for (int i = 0; i < m; ++i) {
					for (int j = 0; j < k / 2; ++j) {
						tempA[i * k + j] = a[xcount][i * k + 2 * j][zcount];
						tempA[i * k + (k / 2) + j] = a[xcount][i * k + 2 * j + 1][zcount];
					}
				}

				for (int i = 0; i < ynumber; ++i) {
					a[xcount][i][zcount] = tempA[i];
				}

				k = k / 2;
			}
		}
	}
	delete[] tempA;
}

void sortInputFastFourierZ(Complex*** a, int xnumber, int ynumber, int znumber) {
	Complex* tempA = new Complex[znumber];
	for (int xcount = 0; xcount < xnumber; ++xcount) {
		for (int ycount = 0; ycount < ynumber; ++ycount) {
			int k = znumber;

			while (k > 2) {
				int m = znumber / k;

				for (int i = 0; i < m; ++i) {
					for (int j = 0; j < k / 2; ++j) {
						tempA[i * k + j] = a[xcount][ycount][i * k + 2 * j];
						tempA[i * k + (k / 2) + j] = a[xcount][ycount][i * k + 2 * j + 1];
					}
				}

				for (int i = 0; i < znumber; ++i) {
					a[xcount][ycount][i] = tempA[i];
				}

				k = k / 2;
			}
		}
	}
	delete[] tempA;
}

void conjugateGradientMethod(std::vector<MatrixElement>**** matrix, double**** rightPart, double**** outVector,
                             int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, double precision, int maxIteration, bool periodic, int verbosity, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int rank;
	int nprocs;
	MPI_Comm_size(cartComm, &nprocs);
	MPI_Comm_rank(cartComm, &rank);
	if (rank == 0 && verbosity > 0) printf("start conjugate gradient\n");

	double**** residual = new double***[xnumberAdded];
	double**** prevResidual = new double***[xnumberAdded];
	double**** z = new double***[xnumberAdded];
	double**** tempVector = new double***[xnumberAdded];

	for (int i = 0; i < xnumberAdded; ++i) {
		residual[i] = new double**[ynumberAdded];
		prevResidual[i] = new double**[ynumberAdded];
		z[i] = new double**[ynumberAdded];
		tempVector[i] = new double**[ynumberAdded];
		for (int j = 0; j < ynumberAdded; ++j) {
			residual[i][j] = new double*[znumberAdded];
			prevResidual[i][j] = new double*[znumberAdded];
			z[i][j] = new double*[znumberAdded];
			tempVector[i][j] = new double*[znumberAdded];
			for (int k = 0; k < znumberAdded; ++k) {
				residual[i][j][k] = new double[lnumber];
				prevResidual[i][j][k] = new double[lnumber];
				z[i][j][k] = new double[lnumber];
				tempVector[i][j][k] = new double[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					outVector[i][j][k][l] = 0;
					prevResidual[i][j][k][l] = rightPart[i][j][k][l];
					z[i][j][k][l] = rightPart[i][j][k][l];
					tempVector[i][j][k][l] = 0;
				}
			}
		}
	}


	/*for (int i = 0; i < xnumber; ++i) {
		for (int j = 0; j < ynumber; ++j) {
			for (int k = 0; k < znumber; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
						MatrixElement element = matrix[i][j][k][l][m];
						prevResidual[i][j][k][l] += element.value * outVector[element.i][element.j][element.k][element.l];
					}
				}
			}
		}
	}*/

	int iteration = 0;


	double prevResidualNorm2 = scalarMultiplyLargeVectors(prevResidual, prevResidual, xnumberAdded, ynumberAdded, znumberAdded,
	                                                      additionalBinNumber, lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);
	double residualNorm2 = prevResidualNorm2;
	double rightPartNorm2 = scalarMultiplyLargeVectors(rightPart, rightPart, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);

	double relativeError = sqrt(residualNorm2 / rightPartNorm2);

	while ((iteration < maxIteration) && (iteration < xnumberAdded * ynumberAdded * znumberAdded * lnumber) && (relativeError > (precision / (xnumberAdded * ynumberAdded * znumberAdded * lnumber)))) {
		if (rank == 0 && verbosity > 1) printf("conjugate gradient iteration %d\n", iteration);

		multiplySpecialMatrixVector(tempVector, matrix, z, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);

		double alpha = prevResidualNorm2 / scalarMultiplyLargeVectors(tempVector, z, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber,
		                                                              lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);

		for (int i = 0; i < xnumberAdded + 1; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						outVector[i][j][k][l] += alpha * z[i][j][k][l];
						residual[i][j][k][l] = prevResidual[i][j][k][l] - alpha * tempVector[i][j][k][l];
					}
				}
			}
		}

		residualNorm2 = scalarMultiplyLargeVectors(residual, residual, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);

		double beta = residualNorm2 / prevResidualNorm2;

		for (int i = 0; i < xnumberAdded; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						z[i][j][k][l] = residual[i][j][k][l] + beta * z[i][j][k][l];
					}
				}
			}
		}

		prevResidualNorm2 = residualNorm2;

		relativeError = sqrt(residualNorm2 / rightPartNorm2);
		iteration++;
	}

	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				delete[] residual[i][j][k];
				delete[] prevResidual[i][j][k];
				delete[] z[i][j][k];
				delete[] tempVector[i][j][k];
			}
			delete[] residual[i][j];
			delete[] prevResidual[i][j];
			delete[] z[i][j];
			delete[] tempVector[i][j];
		}
		delete[] residual[i];
		delete[] prevResidual[i];
		delete[] z[i];
		delete[] tempVector[i];
	}
	delete[] residual;
	delete[] prevResidual;
	delete[] z;
	delete[] tempVector;
}

void biconjugateGradientMethod(std::vector<MatrixElement>**** matrix, double**** rightPart, double**** outVector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, double precision, int maxIteration, bool periodic, int verbosity, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int rank;
	int nprocs;
	MPI_Comm_size(cartComm, &nprocs);
	MPI_Comm_rank(cartComm, &rank);
	if (rank == 0 && verbosity > 0) printf("start biconjugate gradient\n");
	double**** residual = new double***[xnumberAdded + 1];
	double**** prevResidual = new double***[xnumberAdded + 1];
	double**** z = new double***[xnumberAdded + 1];
	double**** p = new double***[xnumberAdded + 1];
	double**** s = new double***[xnumberAdded + 1];
	double**** tempVector = new double***[xnumberAdded + 1];
	double**** tempVector2 = new double***[xnumberAdded + 1];
	std::vector<MatrixElement>**** transposedMatrix = new std::vector<MatrixElement>***[xnumberAdded + 1];

	for (int i = 0; i < xnumberAdded + 1; ++i) {
		residual[i] = new double**[ynumberAdded];
		prevResidual[i] = new double**[ynumberAdded];
		z[i] = new double**[ynumberAdded];
		p[i] = new double**[ynumberAdded];
		s[i] = new double**[ynumberAdded];
		tempVector[i] = new double**[ynumberAdded];
		tempVector2[i] = new double**[ynumberAdded];
		transposedMatrix[i] = new std::vector<MatrixElement>**[ynumberAdded];
		for (int j = 0; j < ynumberAdded; ++j) {
			residual[i][j] = new double*[znumberAdded];
			prevResidual[i][j] = new double*[znumberAdded];
			z[i][j] = new double*[znumberAdded];
			p[i][j] = new double*[znumberAdded];
			s[i][j] = new double*[znumberAdded];
			tempVector[i][j] = new double*[znumberAdded];
			tempVector2[i][j] = new double*[znumberAdded];
			transposedMatrix[i][j] = new std::vector<MatrixElement>*[znumberAdded];
			for (int k = 0; k < znumberAdded; ++k) {
				residual[i][j][k] = new double[lnumber];
				prevResidual[i][j][k] = new double[lnumber];
				z[i][j][k] = new double[lnumber];
				p[i][j][k] = new double[lnumber];
				s[i][j][k] = new double[lnumber];
				tempVector[i][j][k] = new double[lnumber];
				tempVector2[i][j][k] = new double[lnumber];
				transposedMatrix[i][j][k] = new std::vector<MatrixElement>[lnumber];
				for (int l = 0; l < lnumber; ++l) {
					outVector[i][j][k][l] = 0;
					prevResidual[i][j][k][l] = rightPart[i][j][k][l];
					z[i][j][k][l] = rightPart[i][j][k][l];
					p[i][j][k][l] = rightPart[i][j][k][l];
					s[i][j][k][l] = rightPart[i][j][k][l];
					tempVector[i][j][k][l] = 0;
					tempVector2[i][j][k][l] = 0;
				}
			}
		}
	}

	transposeSpecialMatrix(transposedMatrix, matrix, xnumberAdded, ynumberAdded, znumberAdded, lnumber);


	int iteration = 0;


	double prevResidualNorm2 = scalarMultiplyLargeVectors(p, prevResidual, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);
	double residualNorm2 = prevResidualNorm2;
	double rightPartNorm2 = scalarMultiplyLargeVectors(rightPart, rightPart, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);

	double relativeError = sqrt(residualNorm2 / rightPartNorm2);

	while ((iteration < maxIteration) && (iteration < xnumberAdded * ynumberAdded * znumberAdded * lnumber) && (relativeError > (precision / (xnumberAdded * ynumberAdded * znumberAdded * lnumber)))) {
		if (rank == 0 && verbosity > 1) printf("biconjugate gradient iteration %d\n", iteration);

		multiplySpecialMatrixVector(tempVector, matrix, z, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		multiplySpecialMatrixVector(tempVector2, transposedMatrix, s, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);

		double alpha = prevResidualNorm2 / scalarMultiplyLargeVectors(tempVector, s, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber,
		                                                              lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);

		for (int i = 0; i < xnumberAdded + 1; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						outVector[i][j][k][l] += alpha * z[i][j][k][l];
						residual[i][j][k][l] = prevResidual[i][j][k][l] - alpha * tempVector[i][j][k][l];
						p[i][j][k][l] = p[i][j][k][l] - alpha * tempVector2[i][j][k][l];
					}
				}
			}
		}

		residualNorm2 = scalarMultiplyLargeVectors(p, residual, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, false, rank, nprocs, cartComm, cartCoord, cartDim);

		double beta = residualNorm2 / prevResidualNorm2;

		for (int i = 0; i < xnumberAdded + 1; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						z[i][j][k][l] = residual[i][j][k][l] + beta * z[i][j][k][l];
						s[i][j][k][l] = p[i][j][k][l] + beta * s[i][j][k][l];
					}
				}
			}
		}

		prevResidualNorm2 = residualNorm2;

		relativeError = sqrt(residualNorm2 / rightPartNorm2);
		iteration++;
	}

	for (int i = 0; i < xnumberAdded + 1; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				delete[] residual[i][j][k];
				delete[] prevResidual[i][j][k];
				delete[] z[i][j][k];
				delete[] p[i][j][k];
				delete[] s[i][j][k];
				delete[] tempVector[i][j][k];
				delete[] tempVector2[i][j][k];
				delete[] transposedMatrix[i][j][k];
			}
			delete[] residual[i][j];
			delete[] prevResidual[i][j];
			delete[] z[i][j];
			delete[] p[i][j];
			delete[] s[i][j];
			delete[] tempVector[i][j];
			delete[] tempVector2[i][j];
			delete[] transposedMatrix[i][j];
		}
		delete[] residual[i];
		delete[] prevResidual[i];
		delete[] z[i];
		delete[] p[i];
		delete[] s[i];
		delete[] tempVector[i];
		delete[] tempVector2[i];
		delete[] transposedMatrix[i];
	}
	delete[] residual;
	delete[] prevResidual;
	delete[] z;
	delete[] p;
	delete[] s;
	delete[] tempVector;
	delete[] tempVector2;
	delete[] transposedMatrix;
}

void biconjugateStabilizedGradientMethod(std::vector<MatrixElement>**** matrix, double**** rightPart,
                                         double**** outVector, int xnumberAdded, int ynumberAdded, int znumberAdded, int additionalBinNumber,
                                         int lnumber, int xnumberGeneral, int ynumberGeneral, int znumberGeneral, double precision, int maxIteration, bool periodic, int verbosity, MPI_Comm& cartComm, int* cartCoord, int* cartDim,
										 double**** residual, double**** firstResidual, double**** v, double**** p, double**** s, double**** t,
										 double* leftOutBuffer, double* rightOutBuffer, double* leftInBuffer, double* rightInBuffer, double* frontOutBuffer, double* backOutBuffer, double* frontInBuffer, double* backInBuffer, double* bottomOutBuffer, double* topOutBuffer, double* bottomInBuffer, double* topInBuffer, bool& converges) {
	int rank;
	int nprocs;
	MPI_Comm_size(cartComm, &nprocs);
	MPI_Comm_rank(cartComm, &rank);
	if (rank == 0 && verbosity > 0)printf("start biconjugate gradient\n");

	converges = false;

	double alpha = 1;
	double rho = 1;
	double omega = 1;
	int totalNumber = xnumberGeneral*ynumberGeneral*znumberGeneral*lnumber;
	exchangeLargeVector(rightPart, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);
	double rightPartNorm2 = scalarMultiplyLargeVectors(rightPart, rightPart, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);

	double tempNorm = sqrt(rightPartNorm2)/totalNumber;

	if(tempNorm < 1E-100){
		for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					outVector[i][j][k][l] = 0;
				}
			}
		}
		}
		return;
	}

	/*for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					outVector[i][j][k][l] = rightPart[i][j][k][l]/tempNorm;
				}
			}
		}
	}
	exchangeLargeVector(outVector, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);*/

	multiplySpecialMatrixVector(t, matrix, outVector, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
	exchangeLargeVector(t, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);


	for (int i = 0; i < xnumberAdded; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					outVector[i][j][k][l] = 0;
					firstResidual[i][j][k][l] = (rightPart[i][j][k][l]) - t[i][j][k][l];
					residual[i][j][k][l] = (rightPart[i][j][k][l]) - t[i][j][k][l];
					v[i][j][k][l] = 0;
					p[i][j][k][l] = 0;
					s[i][j][k][l] = 0;
					t[i][j][k][l] = 0;
				}
			}
		}
	}


	int iteration = 0;


	double prevResidualNorm2 = scalarMultiplyLargeVectors(residual, residual, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
	double residualNorm2 = prevResidualNorm2;

	double relativeError = sqrt(residualNorm2 / prevResidualNorm2);

	if(fabs(rightPartNorm2) < 1E-300){
		return;
	}

	while ((iteration < maxIteration) && (iteration < totalNumber) && (relativeError > (precision / (totalNumber)))) {
		if (rank == 0 && verbosity > 1) printf("biconjugate gradient iteration %d\n", iteration);

		double newRho = scalarMultiplyLargeVectors(firstResidual, residual, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		if(fabs(rho) < 1E-100){
			if(rank == 0) printf("rho = 0 in biconjugate\n");
			return;
		}
		if(fabs(omega) < 1E-100){
			if(rank == 0) printf("omega = 0 in biconjugate\n");
			return;
		}
		double beta = (newRho / rho) * (alpha / omega);
		rho = newRho;

		int minI = 1 + additionalBinNumber;
		if (cartCoord[0] == 0 && !periodic) {
			minI = 0;
		}
		int maxI = xnumberAdded - 2 - additionalBinNumber;
		if (cartCoord[0] == cartDim[0] - 1 && !periodic) {
			maxI = xnumberAdded - 1;
		}
		int minJ = 1 + additionalBinNumber;
		int maxJ = ynumberAdded - 2 - additionalBinNumber;
		int minK = 1 + additionalBinNumber;
		int maxK = znumberAdded - 2 - additionalBinNumber;

		for (int i = 0; i < xnumberAdded; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						s[i][j][k][l] = 0;
					}
				}
			}
		}

		//for (int i = 0; i < xnumberAdded; ++i) {
			//for (int j = 0; j < ynumberAdded; ++j) {
				//for (int k = 0; k < znumberAdded; ++k) {
		for (int i = minI; i <= maxI; ++i) {
			for (int j = minJ; j <= maxJ; ++j) {
				for (int k =minK; k <= maxK; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						p[i][j][k][l] = residual[i][j][k][l] + beta * (p[i][j][k][l] - omega * v[i][j][k][l]);
					}
				}
			}
		}
		exchangeLargeVector(p, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);
		multiplySpecialMatrixVector(v, matrix, p, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		exchangeLargeVector(v, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);
		double firstRscalarV = scalarMultiplyLargeVectors(firstResidual, v, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		if(fabs(firstRscalarV) < 1E-100){
			if(rank == 0) printf("firstRscalarV = 0 in biconjugate\n");
			return;
		}
		alpha = rho / firstRscalarV;

		//for (int i = 0; i < xnumberAdded; ++i) {
			//for (int j = 0; j < ynumberAdded; ++j) {
				//for (int k = 0; k < znumberAdded; ++k) {
		for (int i = minI; i <= maxI; ++i) {
			for (int j = minJ; j <= maxJ; ++j) {
				for (int k = minK; k <= maxK; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						s[i][j][k][l] = residual[i][j][k][l] - alpha * v[i][j][k][l];
					}
				}
			}
		}

		exchangeLargeVector(s, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);
		multiplySpecialMatrixVector(t, matrix, s, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		exchangeLargeVector(t, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);
		double tnorm2 = scalarMultiplyLargeVectors(t, t,xnumberAdded,ynumberAdded,znumberAdded,additionalBinNumber,lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);
		if(fabs(tnorm2) < 1E-100){
			if(rank == 0) printf("tnorm2 = 0 in biconjugate\n");
			return;
		}
		omega = scalarMultiplyLargeVectors(t, s, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim) / tnorm2;

		//for (int i = 0; i < xnumberAdded; ++i) {
			//for (int j = 0; j < ynumberAdded; ++j) {
				//for (int k = 0; k < znumberAdded; ++k) {
		for (int i = minI; i <= maxI; ++i) {
			for (int j = minJ; j <= maxJ; ++j) {
				for (int k = minK; k <= maxK; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						outVector[i][j][k][l] = outVector[i][j][k][l] + omega * s[i][j][k][l] + alpha * p[i][j][k][l];
						residual[i][j][k][l] = s[i][j][k][l] - omega * t[i][j][k][l];
					}
				}
			}
		}
		exchangeLargeVector(outVector, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);
		exchangeLargeVector(residual, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, periodic, cartComm, cartCoord, cartDim, leftOutBuffer, rightOutBuffer, leftInBuffer, rightInBuffer, frontOutBuffer, backOutBuffer, frontInBuffer, backInBuffer, bottomOutBuffer, topOutBuffer, bottomInBuffer, topInBuffer);

		residualNorm2 = scalarMultiplyLargeVectors(residual, residual, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim);

		prevResidualNorm2 = residualNorm2;

		relativeError = sqrt(residualNorm2/rightPartNorm2);
		iteration++;
	}

	converges = true;

	if(relativeError > 0.1){
		converges = false;
	}

	for(int i = 0; i < xnumberAdded; ++i){
		for(int j = 0; j < ynumberAdded; ++j){
			for(int k = 0; k < znumberAdded; ++k){
				for(int l = 0; l < lnumber; ++l){
					//outVector[i][j][k][l] *= tempNorm;
				}
			}
		}
	}
}

void gaussSeidelMethod(std::vector<MatrixElement>**** matrix, double**** rightPart, double**** outVector, int xnumberAdded,
                       int ynumberAdded, int znumberAdded, int additionalBinNumber, int lnumber, int xnumberGeneral,
                       int znumberGeneral, int ynumberGeneral, double precision, int maxIteration, bool periodic, bool startFromZero, int verbocity, MPI_Comm& cartComm, int* cartCoord, int* cartDim) {
	int rank;
	int nprocs;
	MPI_Comm_size(cartComm, &nprocs);
	MPI_Comm_rank(cartComm, &rank);
	double* rightOutBuffer = new double[ynumberAdded * znumberAdded * lnumber];
	double* rightInBuffer = new double[ynumberAdded * znumberAdded * lnumber];
	double* leftOutBuffer = new double[ynumberAdded * znumberAdded * lnumber];
	double* leftInBuffer = new double[ynumberAdded * znumberAdded * lnumber];

	double* backOutBuffer = new double[xnumberAdded * znumberAdded * lnumber];
	double* backInBuffer = new double[xnumberAdded * znumberAdded * lnumber];
	double* frontOutBuffer = new double[xnumberAdded * znumberAdded * lnumber];
	double* frontInBuffer = new double[xnumberAdded * znumberAdded * lnumber];

	double* topOutBuffer = new double[ynumberAdded * xnumberAdded * lnumber];
	double* topInBuffer = new double[ynumberAdded * xnumberAdded * lnumber];
	double* bottomOutBuffer = new double[ynumberAdded * xnumberAdded * lnumber];
	double* bottomInBuffer = new double[ynumberAdded * xnumberAdded * lnumber];

	if ((rank == 0) && (verbocity > 0)) printf("start gauss-seidel\n");
	double normRightPart = scalarMultiplyLargeVectors(rightPart, rightPart, xnumberAdded, ynumberAdded, znumberAdded, additionalBinNumber, lnumber, periodic, rank, nprocs, cartComm, cartCoord, cartDim) / (xnumberGeneral * ynumberGeneral * znumberGeneral * lnumber);
	for (int i = 0; i < xnumberAdded + 1; ++i) {
		for (int j = 0; j < ynumberAdded; ++j) {
			for (int k = 0; k < znumberAdded; ++k) {
				for (int l = 0; l < lnumber; ++l) {
					//outVector[i][j][k][l] = uniformDistribution()*normRightPart/matrix[0][0][0][0][0].vfalue;
					if (startFromZero) {
						outVector[i][j][k][l] = 0;
					}
				}
			}
		}
	}


	int curIteration = 0;
	while (curIteration < maxIteration) {
		if ((rank == 0) && (verbocity > 1)) printf("Gauss-Seidel iteration %d\n", curIteration);
		if (rank > 0) receiveLargeVectorFromLeft(rightPart, leftInBuffer, xnumberAdded, ynumberAdded, znumberAdded, lnumber, additionalBinNumber, cartComm);
		for (int i = 0; i < xnumberAdded + 1; ++i) {
			for (int j = 0; j < ynumberAdded; ++j) {
				for (int k = 0; k < znumberAdded; ++k) {
					for (int l = 0; l < lnumber; ++l) {
						double sum = rightPart[i][j][k][l];
						double a = 1;
						for (int m = 0; m < matrix[i][j][k][l].size(); ++m) {
							MatrixElement element = matrix[i][j][k][l][m];
							if (!indexEqual(element, i, j, k, l)) {
								sum -= element.value * outVector[element.i][element.j][element.k][element.l];
							} else {
								a = element.value;
							}
						}
						outVector[i][j][k][l] = sum / a;
					}
				}
			}
		}
		if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
			sendLargeVectorToLeftReceiveFromRight(outVector, leftOutBuffer, rightInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == 0) {
			receiveLargeVectorFromRight(outVector, rightInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == cartDim[0] - 1) {
			sendLargeVectorToLeft(outVector, leftOutBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		}

		if (periodic || (cartCoord[0] > 0 && cartCoord[0] < cartDim[0] - 1)) {
			sendLargeVectorToRightReceiveFromLeft(outVector, rightOutBuffer, leftInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == 0) {
			sendLargeVectorToRight(outVector, rightOutBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		} else if (cartCoord[0] == cartDim[0] - 1) {
			receiveLargeVectorFromLeft(outVector, leftInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		}

		sendLargeVectorToFrontReceiveFromBack(outVector, frontOutBuffer, backInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		sendLargeVectorToBackReceiveFromFront(outVector, backOutBuffer, frontInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);

		sendLargeVectorToBottomReceiveFromTop(outVector, bottomOutBuffer, topInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);
		sendLargeVectorToTopReceiveFromBottom(outVector, topOutBuffer, bottomInBuffer, xnumberAdded, ynumberAdded, znumberAdded, 3, additionalBinNumber, cartComm);

		curIteration++;
	}

	delete[] leftOutBuffer;
	delete[] rightInBuffer;
	delete[] rightOutBuffer;
	delete[] leftInBuffer;

	delete[] frontOutBuffer;
	delete[] frontInBuffer;
	delete[] backOutBuffer;
	delete[] backInBuffer;

	delete[] bottomOutBuffer;
	delete[] bottomInBuffer;
	delete[] topOutBuffer;
	delete[] topInBuffer;
}

bool indexLower(const MatrixElement& element, int i, int j, int k, int l) {
	if (element.i < i) {
		return true;
	}
	if (element.i > i) {
		return false;
	}
	if (element.j < j) {
		return true;
	}
	if (element.j > j) {
		return false;
	}
	if (element.k < k) {
		return true;
	}
	if (element.k > k) {
		return false;
	}
	if (element.l < l) {
		return true;
	}
	if (element.l > l) {
		return false;
	}

	return false;
}

bool indexEqual(const MatrixElement& element, int i, int j, int k, int l) {
	return (element.i == i) && (element.j == j) && (element.k == k) && (element.l == l);
}

bool indexUpper(const MatrixElement& element, int i, int j, int k, int l) {
	return ((!indexEqual(element, i, j, k, l)) && (!indexLower(element, i, j, k, l)));
}




