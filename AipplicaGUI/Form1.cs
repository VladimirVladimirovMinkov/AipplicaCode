using AipplicaSpectrometer;
using System.IO;

namespace AipplicaGUI
{
    public partial class Form1 : Form
    {
        private readonly byte[] _patternBuffer = new byte[3000];
        private readonly byte[] _referenceSignal = new byte[3000];
        private readonly byte[] _spectralSignal = new byte[3000];

        private readonly double[] _formattedSignal = new double[1500];
        private readonly double[] _formattedSignalX = new double[1500];

        private readonly float _squaredCal;
        private readonly float _linearCal;
        private readonly float _offsetCal;

        private bool isClosing = false;
        private bool readingData = false;
        private Task? _dataTask = null;

        public Form1()
        {
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

            Spectro.ReadPattern(_patternBuffer);

            Spectro.ReadID(idBuffer);
            Console.WriteLine($"ReadID: {idBuffer[1]} {idBuffer[3]} {idBuffer[5]} {idBuffer[7]}");

            Console.WriteLine("\nCalibrating...");
            Spectro.ReadCal(_patternBuffer);

            int NumCalCoeff = _patternBuffer[1] / 4;
            byte[] fourbytes = new byte[4];
            float[] calV = new float[NumCalCoeff];
            Console.WriteLine($"NumofCalCoeff: {NumCalCoeff}");
            for (int i = 0; i < NumCalCoeff; i++)
            {
                for (int j = 1; j < 5; j++) { fourbytes[j - 1] = _patternBuffer[8 * i + 2 * j + 1]; }
                Console.WriteLine($"C[{i}] {calV[i] = BitConverter.ToSingle(fourbytes)}");
            }
            _squaredCal = calV[0];
            _linearCal = calV[1];
            _offsetCal = calV[2];

            InitializeComponent();

            formsPlot1.Plot.XLabel("Wavelength (nm)");
            formsPlot1.Plot.YLabel("Count");
            formsPlot1.Refresh();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;

            _dataTask = Task.Run(() =>
            {
                int _currentItegTime = (int)numericUpDown1.Value;
                Spectro.SetIntTimeMs(_currentItegTime);

                // Update the reference signal
                Spectro.ReadPattern(_patternBuffer);
                Spectro.ReadArray(_referenceSignal);

                while (true)
                {
                    if (!readingData)
                    {
                        Spectro.ReadPattern(_patternBuffer);
                        Spectro.ReadArray(_spectralSignal);

                        for (int i = 0; i < _formattedSignal.Length; i++)
                        {
                            _formattedSignal[i] = (_referenceSignal[2 * i] - _spectralSignal[2 * i]) * 256 + _referenceSignal[2 * i + 1] - _spectralSignal[2 * i + 1];
                            _formattedSignalX[i] = _offsetCal + _linearCal * i + _squaredCal * i * i;
                        }
                    }

                    if (isClosing) break;
                    formsPlot1.Invoke(() =>
                    {
                        formsPlot1.Plot.Clear();
                        formsPlot1.Plot.AddScatter(_formattedSignalX, _formattedSignal);
                        formsPlot1.Refresh();
                    });

                    int newIntegTime = (int)numericUpDown1.Value;
                    if (_currentItegTime != newIntegTime)
                    {
                        Spectro.SetIntTimeMs(newIntegTime);
                        _currentItegTime = newIntegTime;

                        // Update the reference signal
                        Spectro.ReadPattern(_patternBuffer);
                        Spectro.ReadArray(_referenceSignal);
                    }
                }
            });
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            isClosing = true;

            if (_dataTask != null && !_dataTask.IsCompletedSuccessfully) _dataTask.Wait();

            base.OnFormClosing(e);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                readingData = true;
                using StreamWriter streamWriter = new StreamWriter("./data.csv");
                for (int i = 0; i < _formattedSignal.Length; i++)
                {
                    streamWriter.WriteLine($"{_formattedSignalX[i]},{_formattedSignal[i]}");
                }
            } finally
            {
                readingData = false;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }
    }
}
