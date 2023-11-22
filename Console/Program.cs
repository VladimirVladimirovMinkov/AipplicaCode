using AipplicaSpectrometer;

// See https://aka.ms/new-console-template for more information
Console.WriteLine("Hello, World!");
Spectro.InitUSB();

byte[] idBuffer = new byte[8];
for (int i = 0; i < 10; i++)
{
    Spectro.ReadID(idBuffer);
    if ((idBuffer[1] == 0x1) && (idBuffer[3] == 0x1) && (idBuffer[5] == 0x01) && (idBuffer[7] == 0x02))
    {
        Console.WriteLine($"ReadID: FoundID at {i}");
        break;
    }
}

byte[] patternBuffer = new byte[3000];
Spectro.ReadPattern(patternBuffer);

Spectro.ReadID(idBuffer);
Console.WriteLine($"ReadID: {idBuffer[1]} {idBuffer[3]} {idBuffer[5]} {idBuffer[7]}");

Console.WriteLine("\nCalibrating...");
Spectro.ReadCal(patternBuffer);

int NumCalCoeff = patternBuffer[1] / 4;
byte[] fourbytes = new byte[4];
float[] calV = new float[NumCalCoeff];
Console.WriteLine($"NumofCalCoeff: {NumCalCoeff}");
for (int i = 0; i < NumCalCoeff; i++)
{
    for (int j = 1; j < 5; j++) { fourbytes[j - 1] = patternBuffer[8 * i + 2 * j + 1]; }
    Console.WriteLine($"C[{i}] {calV[i] = BitConverter.ToSingle(fourbytes)}");
}
Console.WriteLine("");

int integrationTimeMs = 10;
Console.WriteLine($"Setting Integration Time to {integrationTimeMs}");
Spectro.SetIntTimeMs(integrationTimeMs);

Console.WriteLine("Reading Dark Signal\n\n");
byte[] referenceSignal = new byte[3000];
Spectro.ReadPattern(patternBuffer);
Spectro.ReadArray(referenceSignal);

Console.WriteLine($"Reading Spectral Signal");
byte[] spectralSignal = new byte[3000];
Spectro.ReadPattern(patternBuffer);
Spectro.ReadArray(spectralSignal);

Spectro.DisplaySignal(referenceSignal, spectralSignal);

Spectro.Close();

Console.ReadKey();
