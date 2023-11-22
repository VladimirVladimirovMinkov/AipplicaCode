#include "pch.h"

#include "AipplicaSpectrometer.h"
#include <cstdio>

static void VerifySig(int sig) {
	if (sig > 15) throw gcnew System::InvalidOperationException("Function returned a invalid signature");
}

namespace AipplicaSpectrometer {
	void Spectro::CheckBusy() {
		VerifySig(AP_CheckBusy());
	}
	void Spectro::InitUSB() {
		VerifySig(AP_InitUSB());
	}
	void Spectro::ReadPattern(array<System::Byte>^ patternBuffer) {
		if (patternBuffer == nullptr || patternBuffer->Length < 3000) {
			throw gcnew System::ArgumentException("Array must be non-null and have at least 3000 elements.");
		}

		// Pin the managed array
		pin_ptr<System::Byte> pinnedPatternBuffer = &patternBuffer[0];

		VerifySig(AP_ReadPattern(pinnedPatternBuffer));
	}
	void Spectro::ReadID(array<System::Byte>^ idBuffer) {
		if (idBuffer == nullptr || idBuffer->Length < 8) {
			throw gcnew System::ArgumentException("Array must be non-null and have at least 8 elements.");
		}

		// Pin the managed array
		pin_ptr<System::Byte> pinnedIdBuffer = &idBuffer[0];

		VerifySig(AP_ReadID(pinnedIdBuffer));
	}
	void Spectro::ReadCal(array<System::Byte>^ patternBuffer) {
		if (patternBuffer == nullptr || patternBuffer->Length < 3000) {
			throw gcnew System::ArgumentException("Array must be non-null and have at least 3000 elements.");
		}

		// Pin the managed array
		pin_ptr<System::Byte> pinnedPatternBuffer = &patternBuffer[0];

		VerifySig(AP_ReadCal(pinnedPatternBuffer));
	}
	void Spectro::WriteCalData(array<float>^ coeff) {
		if (coeff == nullptr || coeff->Length < 7) {
			throw gcnew System::ArgumentException("Array must be non-null and have at least 3000 elements.");
		}

		// Pin the managed array
		pin_ptr<float> pinnedCoeff = &coeff[0];

		VerifySig(AP_WriteCalData(pinnedCoeff));
	}
	void Spectro::ReadArray(array<System::Byte>^ signalBuffer) {
		if (signalBuffer == nullptr || signalBuffer->Length < 3000) {
			throw gcnew System::ArgumentException("Array must be non-null and have at least 3000 elements.");
		}

		// Pin the managed array
		pin_ptr<System::Byte> pinnedSignalBuffer = &signalBuffer[0];

		VerifySig(AP_ReadArray(pinnedSignalBuffer));
	}
	void Spectro::ReadFrame(array<System::Byte>^ frameBuffer) {
		if (frameBuffer == nullptr || frameBuffer->Length < 3000) {
			throw gcnew System::ArgumentException("Array must be non-null and have at least 3000 elements.");
		}

		// Pin the managed array
		pin_ptr<System::Byte> pinnedFrameBuffer = &frameBuffer[0];

		VerifySig(AP_ReadFrame(pinnedFrameBuffer));
	}
	void Spectro::SetIntTime(uint16 uint16time) {
		VerifySig(AP_SetIntTime(uint16time));
	}

	const int microsecPerMsec = 1000;
	// Define minimum and maximum counts
	const uint16 minCount = 2500;
	const uint16 maxCount = 65535;
	void Spectro::SetIntTimeMs(int msec) {
		// Conversion factor: 1 millisecond = 1000 microseconds
		// Convert msec to microsecond and then to count
		uint16 count = (msec * microsecPerMsec) / 4;

		// Check if count is below minimum or above maximum, and adjust accordingly
		if (count < minCount) count = minCount;
		else if (count > maxCount) count = maxCount;

		VerifySig(AP_SetIntTime(count));
	}

	void Spectro::DisplaySignal(array<System::Byte>^ referenceSignalBuffer, array<System::Byte>^ signalBuffer) {
		if (referenceSignalBuffer == nullptr || referenceSignalBuffer->Length < 3000) {
			throw gcnew System::ArgumentException("referenceSignalBuffer must be non-null and have at least 3000 elements.");
		}
		if (signalBuffer == nullptr || signalBuffer->Length < 3000) {
			throw gcnew System::ArgumentException("signalBuffer must be non-null and have at least 3000 elements.");
		}

		FILE* gnuplot;
		gnuplot = _popen("gnuplot", "w");
		if (!gnuplot) {
			throw gcnew System::InvalidOperationException("Unable to open gnuplot");
		}
		fprintf(gnuplot, "set style data linespoints\n");
		fprintf(gnuplot, "set title 'Spectrum'\n");

		// Pin the managed array
		pin_ptr<System::Byte> pinnedRefSigBuffer = &referenceSignalBuffer[0];
		pin_ptr<System::Byte> pinnedSigBuffer = &signalBuffer[0];

		fprintf(gnuplot, "plot '-' u 1:2 t 'Spectrum' w lp\n");
		for (int i = 0; i <= 1500; i++) {
			fprintf(gnuplot, "%d %d\n", i, (pinnedRefSigBuffer[2 * i] - pinnedSigBuffer[2 * i]) * 256 + pinnedRefSigBuffer[2 * i + 1] - pinnedSigBuffer[2 * i + 1]);
		}

		fprintf(gnuplot, "e\n");
		fflush(gnuplot);
	}

	void Spectro::Close() {
		AP_Close();
	}
}