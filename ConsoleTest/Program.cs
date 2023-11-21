using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleTest
{
    internal class Program
    {
        [DllImport("./TestDLL.dll")]
        private static extern int Test();
        static void Main(string[] args)
        {
            Console.WriteLine(Test());
            Console.ReadKey();
        }
    }
}
