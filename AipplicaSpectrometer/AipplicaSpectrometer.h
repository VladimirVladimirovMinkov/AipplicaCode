#pragma once
#include "windows.h"
#include "libMPSSE_spi.h"
#include "libAPSpecW.h"


namespace AipplicaSpectrometer {
	public ref class Spectro
	{
		public: 
			static int CheckBusy();
			static int InitUSB();
			static int ReadPattern(uint8* pptrnbfr);
			static int ReadID(uint8* prbuf);
			static int ReadCal(uint8* pptrnbfr);
			static int WriteCalData(float coeff[7]);
			static int ReadArray(uint8* pptrnbfr);
			static int ReadFrame(uint8* pptrnbfr);
			static int SetIntTime(uint16 inttime);
			static void Close();
	};
}
