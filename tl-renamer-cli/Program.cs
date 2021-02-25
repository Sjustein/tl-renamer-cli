using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;

namespace tl_renamer_cli
{
    class Program
    {
        private static char PathSep;

        static void Main(string[] args)
        {
            // Find the path seperator for the current operating system
            PathSep = Environment.OSVersion.Platform.ToString() == "Win32NT" ? '\\' : '/';

            // Check if the directory has been specified in a cli argument
            string Dir;
            if (args.Length > 0)
                Dir = args[0];
            else
            {
                Console.Write("Input the path to the directory with the files to be renamed] ");
                Dir = Console.ReadLine();
            }

            // Check if the last character is a path seperator. Otherwise, add it
            if (Dir[Dir.Length - 1] != PathSep)
                Dir += PathSep;

            ProcessDirectory(Dir);
        }

        private static void ProcessDirectory(string Dir)
        {
            string[] Files = Directory.GetFiles(Dir);
            Dictionary<string, long> FileCreateTimes = new Dictionary<string, long>();

            // Gather the file creation time of the files
            foreach (string F in Files)
                FileCreateTimes.Add(F, Directory.GetCreationTime(F).Ticks);

            // Order the directory by create date
            FileCreateTimes = FileCreateTimes.OrderBy(x => x.Value).ToDictionary(x => x.Key, x => x.Value);

            // Create the output directory, or reset it if it already exists
            if (Directory.Exists(Dir + $"Converted{PathSep}"))
                foreach (string S in Directory.GetFiles(Dir + $"Converted{PathSep}"))
                    File.Delete(S);
            else
                Directory.CreateDirectory(Dir + $"Converted{PathSep}");

            // Rename the files and copy them to the output directory
            int i = 0;
            foreach (KeyValuePair<string, long> Cur in FileCreateTimes)
            {
                Console.WriteLine(Cur.Key + " -> " + Dir + "Converted\\" + i + Path.GetExtension(Cur.Key));
                File.Copy(Cur.Key, Dir + "Converted\\" + i + Path.GetExtension(Cur.Key));
                i++;
            }

            Console.WriteLine("Succes");
        }
    }
}
