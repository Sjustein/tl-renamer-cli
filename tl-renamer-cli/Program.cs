using System;
using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace tl_renamer_cli
{
    class Program
    {
        static void Main(string[] args)
        {
            string Dir;
            if (args.Length > 0)
                Dir = args[0];
            else
            {
                Console.Write("Input the path to the directory with the files to be renamed] ");
                Dir = Console.ReadLine();
            }

            if (Dir[Dir.Length - 1] != '\\')
                Dir += '\\';

            string[] Files = Directory.GetFiles(Dir);
            Dictionary<string, long> FileAccessTimes = new Dictionary<string, long>();

            // Gather the file creation time of the files
            foreach (string F in Files)
                FileAccessTimes.Add(F, Directory.GetLastWriteTime(F).Ticks);

            // Order the directory
            FileAccessTimes = FileAccessTimes.OrderBy(x => x.Value).ToDictionary(x => x.Key, x => x.Value);

            // Create the output directory
            if (Directory.Exists(Dir + "Converted\\"))
                foreach (string S in Directory.GetFiles(Dir + "Converted\\"))
                    File.Delete(S);
            else
                Directory.CreateDirectory(Dir + "Converted\\");

            // Rename the files and copy them to the output directory
            int i = 0;
            foreach (KeyValuePair<string, long> Cur in FileAccessTimes)
            {
                Console.WriteLine(Cur.Key + " -> " + Dir + "Converted\\" + i + Path.GetExtension(Cur.Key));
                File.Copy(Cur.Key, Dir + "Converted\\" + i + Path.GetExtension(Cur.Key));
                i++;
            }

            Console.WriteLine("Succes");
        }
    }
}
