#include "Utility.h"

using namespace std;

bool readFile(const string &sourcePath, string &out)
{
  out = "";
  string line;
  ifstream myfile(sourcePath);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
      out += line + "\n";
    myfile.close();
  }
  else
  {
    cout << "Unable to open file";
    return false;
  }

  return true;
}