using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading;

namespace SampleApp
{
    class Program
    {
        static void FuncA()
        {
            Console.WriteLine("Sample app!");
        }

        static void Main(string[] args)
        {
            FuncA();
        }
    }
}
