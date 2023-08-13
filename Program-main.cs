using AssettoCorsaSharedMemory;
using System;
using System.IO.Ports;
using System.Linq;
using System.Threading;

namespace Asseto_Corsa_Telemetry_Router
{
    class Program
    {
        static private short _currentSpeed = 0;
        static private short _currentTyreDirt = 0;
        static private short _currentCarDamage = 0;
        static string _portName = "COM2";
        static int _baudRate = 9600;
        static SerialPort _arduinoPort;

        static void Main(string[] args)
        {
            Console.WriteLine("start");

            // Initialize the SerialPort instance for Arduino communication
            _arduinoPort = new SerialPort(_portName, _baudRate);

            // Open the serial port
            try
            {
                _arduinoPort.Open();

                AssettoCorsa ac = new AssettoCorsa();
                ac.PhysicsInterval = 10;
                ac.PhysicsUpdated += ac_PhysicUpdated;
                ac.Start(); // Connect to shared memory and start interval timers 

                while (true)
                {
                    // Send the current speed to the Arduino
                    SendDataToArduino();

                    Thread.Sleep(500);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error while opening or accessing COM port: {ex.Message}");
            }
            finally
            {
                // Make sure to close the serial port when exiting the loop or in case of an exception
                if (_arduinoPort.IsOpen)
                    _arduinoPort.Close();
            }
        }

        static void ac_PhysicUpdated(object sender, PhysicsEventArgs e)
        {
            _currentSpeed = (short)e.Physics.SpeedKmh;
            _currentTyreDirt = (short)(10 * (float)Math.Round(e.Physics.TyreDirtyLevel.Sum(), 1));
            _currentCarDamage = (short)(1000 * (float)Math.Round(e.Physics.CarDamage.Sum(), 3));

            Console.WriteLine(_currentSpeed.ToString() + " " + _currentTyreDirt.ToString() + " " + _currentCarDamage.ToString());
        }

        static void SendDataToArduino()
        {
            try
            {
                // Convert _currentSpeed to a string
                string dataToSend = _currentSpeed.ToString() + " " + _currentTyreDirt.ToString() + " " + _currentCarDamage.ToString();

                // Send data to Arduino
                _arduinoPort.WriteLine(dataToSend);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error while sending data to Arduino: {ex.Message}");
            }
        }
    }
}
