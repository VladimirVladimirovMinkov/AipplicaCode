#pragma once
#include "windows.h"
#include "include/libAPSpecW.h"
#include "include/libMPSSE_spi.h"


namespace AipplicaSpectrometer {
	public ref class Spectro
	{
		public: 
			static void CheckBusy();
			static void InitUSB();
			static void ReadPattern(array<System::Byte>^ patternBuffer);
			static void ReadID(array<System::Byte>^ idBuffer);
			static void ReadCal(array<System::Byte>^ patternBuffer);
			static void WriteCalData(array<float>^ coeff);
			static void ReadArray(array<System::Byte>^ signalBuffer);
			static void ReadFrame(array<System::Byte>^ frameBuffer);
			static void SetIntTime(uint16 inttime);
			static void SetIntTimeMs(int inttime);
			static void DisplaySignal(array<System::Byte>^ referenceSignalBuffer, array<System::Byte>^ signalBuffer);
			static void Close();
	};
}
