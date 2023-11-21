#include "windows.h"
#include "libMPSSE_spi.h"
#include "libAPSpecW.h"

#include "pch.h"
#include "APSpec.h"

public ref class AipplicaSpectrometer {
	static int CheckBusy() {
		return AP_CheckBusy();
	}
	static int InitUSB() {
		return AP_InitUSB();
	}
	static int ReadPattern(uint8* pptrnbfr) {
		return AP_ReadPattern(pptrnbfr);
	}
	static int ReadID(uint8* prbuf) {
		return AP_ReadID(prbuf);
	}
	static int ReadCal(uint8* pptrnbfr) {
		return AP_ReadCal(pptrnbfr);
	}
	static int WriteCalData(float coeff[7]) {
		return AP_WriteCalData(coeff);
	}
	static int ReadArray(uint8* pptrnbfr) {
		return AP_ReadArray(pptrnbfr);
	}
	static int ReadFrame(uint8* pptrnbfr) {
		return AP_ReadFrame(pptrnbfr);
	}
	static int SetIntTime(uint16 inttime) {
		return AP_SetIntTime(inttime);
	}
	static void Close() {
		return AP_Close();
	}
};