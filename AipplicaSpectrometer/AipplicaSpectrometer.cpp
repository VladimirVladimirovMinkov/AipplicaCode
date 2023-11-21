#include "pch.h"

#include "AipplicaSpectrometer.h"

namespace AipplicaSpectrometer {
	int Spectro::CheckBusy() {
		return AP_CheckBusy();
	}
	int Spectro::InitUSB() {
		return AP_InitUSB();
	}
	int Spectro::ReadPattern(uint8* pptrnbfr) {
		return AP_ReadPattern(pptrnbfr);
	}
	int Spectro::ReadID(uint8* prbuf) {
		return AP_ReadID(prbuf);
	}
	int Spectro::ReadCal(uint8* pptrnbfr) {
		return AP_ReadCal(pptrnbfr);
	}
	int Spectro::WriteCalData(float coeff[7]) {
		return AP_WriteCalData(coeff);
	}
	int Spectro::ReadArray(uint8* pptrnbfr) {
		return AP_ReadArray(pptrnbfr);
	}
	int Spectro::ReadFrame(uint8* pptrnbfr) {
		return AP_ReadFrame(pptrnbfr);
	}
	int Spectro::SetIntTime(uint16 inttime) {
		return AP_SetIntTime(inttime);
	}
	void Spectro::Close() {
		return AP_Close();
	}
}