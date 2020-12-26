#include <string>

class Shader
{
  void compile(std::string vertex, std::string fragment);
public:
  Shader(std::string vertex, std::string fragment);
};
