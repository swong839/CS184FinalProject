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

GLint randomInt(const GLint min, const GLint max)
{
  return (rand() % (max - min)) + min;
}

GLfloat randomFloat(const GLfloat min, const GLfloat max)
{
  return ((GLfloat)rand() / (GLfloat)RAND_MAX) * (max - min) + min;
}

glm::vec2 randomVec2(const glm::vec2 &min, const glm::vec2 &max)
{
  return glm::vec2(
    randomFloat(min.x, max.x), 
    randomFloat(min.y, max.y));
}

glm::vec3 randomVec3(const glm::vec3 &min, const glm::vec3 &max)
{
  return glm::vec3(
    randomFloat(min.x, max.x),
    randomFloat(min.y, max.y),
    randomFloat(min.z, max.z));
}