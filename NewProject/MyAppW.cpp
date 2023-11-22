#include <stdio.h>
#include "windows.h"
#include "libMPSSE_spi.h"
#include "libAPSpecW.h"
#include <stdexcept>
#include "conio.h"
#include <cstdint>

uint16 msecToCount(int msec) {
	// Conversion factor: 1 millisecond = 1000 microseconds
	const int microsecPerMsec = 1000;

	// Convert msec to microsecond and then to count
	uint16 count = (msec * microsecPerMsec) / 4;

	// Define minimum and maximum counts
	const uint16 minCount = 2500;
	const uint16 maxCount = 65535;

	// Check if count is below minimum or above maximum, and adjust accordingly
	if (count < minCount) {
		count = minCount;
	}
	else if (count > maxCount) {
		count = maxCount;
	}

	return count;
}

void VerifySig(int sig) {
	if (sig > 15) throw std::runtime_error("Invalid return signature!");
}

void WriteCSV(float* calV, uint8* ptrnbfr, uint8* RefS, uint8* SigS, int size, const char* fileName) {
	FILE* csvFile;
	fopen_s(&csvFile, fileName, "w");
	if (csvFile == NULL) {
		printf("Error opening file!\n");
		exit(EXIT_FAILURE);
	}
	fprintf(csvFile, "%u,%u,%u,%f,%f,%f\n", ptrnbfr[0], RefS[0], SigS[0], calV[0], calV[1], calV[2]);
	for (int i = 1; i < size; i++) {
		fprintf(csvFile, "%u,%u,%u,,,\n", ptrnbfr[i], RefS[i], SigS[i]);
	}
	fclose(csvFile);
}

int main()
{
	const char* fileName = "outData.csv";
	// Maximum is 262msec, Minimum is 10msec
	uint16 integrationTime = msecToCount(10);

	uint8 ptrnbfr[3000] = { 0x0 };
	uint8 RefS[3000] = { 0x0 };
	uint8 SigS[3000] = { 0x0 };
	uint8 *pptrnbfr, *pRefS, *pSigS;
	pptrnbfr = &ptrnbfr[0];
	pRefS = &RefS[0];
	pSigS = &SigS[0];

	VerifySig(AP_InitUSB());
	printf("Initalized USB Chip\n");
	
	uint8 idbfr[8];
	for (int i = 0; i < 10; i++) {
		AP_ReadID(&idbfr[0]);
		if ((idbfr[1] == 0x1) && (idbfr[3] == 0x1) && (idbfr[5] == 0x01) && (idbfr[7] == 0x02)) {
			printf("(ReadID) FoundID at %d\n", i);
			break;
		}
	}

	VerifySig(AP_ReadPattern(pptrnbfr));

	AP_ReadID(&idbfr[0]);
	printf("(ReadID) %d %d %d %d\n", idbfr[1], idbfr[3], idbfr[5], idbfr[7]);

	printf("\nCalibrating...\n");
	VerifySig(AP_ReadCal(pptrnbfr));

	uint8 NumCalCoeff = ptrnbfr[1] / 4;
	uint8 fourbytes[4]{};
	float calV[3]{};
	printf("NumofCalCoeff: %d\n", NumCalCoeff);
	for (int i = 0; i < NumCalCoeff; i++) {
		for (int j = 1; j < 5; j++) { fourbytes[j - 1] = ptrnbfr[8 * i + 2 * j + 1]; }
		printf("C[%d] %f    \n", i, calV[i] = *(float*)fourbytes);
	}
	printf("\n");

	printf("Setting Integration Time\n");
	VerifySig(AP_SetIntTime(integrationTime));

	printf("Reading Dark Signal\n\n");
	VerifySig(AP_ReadPattern(pptrnbfr));
	VerifySig(AP_ReadArray(pRefS));

	FILE* gnuplot;
	gnuplot = _popen("gnuplot", "w");
	if (!gnuplot) {
		perror("popen");
		exit(EXIT_FAILURE);
	}
	fprintf(gnuplot, "set style data linespoints\n");
	fprintf(gnuplot, "set title 'Spectrum'\n");

	while (1) {
		printf("Reading Spectral Signal (Press enter to exit)\n");
		VerifySig(AP_ReadPattern(pptrnbfr));
		VerifySig(AP_ReadArray(pSigS));

		fprintf(gnuplot, "plot '-' u 1:2 t 'Spectrum' w lp\n");
		for (int i = 0; i <= 1500; i++) {
			fprintf(gnuplot, "%d %d\n", i, (pRefS[2 * i] - pSigS[2 * i]) * 256 + pRefS[2 * i + 1] - pSigS[2 * i + 1]);
		}

		fprintf(gnuplot, "e\n");
		fflush(gnuplot);
		
		WriteCSV(calV, ptrnbfr, RefS, SigS, 3000, fileName);

		if (_kbhit()) {
			char ch = _getch();
			// Enter pressed
			if (ch == '\r' || ch == '\n') break;
		}
	}

	printf("Press any key to exit\n");
	getchar();

	_pclose(gnuplot);

	AP_Close();
	exit(EXIT_SUCCESS);

}