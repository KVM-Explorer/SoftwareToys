#include "geometry.h"
#include "tgaimage.h"
#include <array>
using Matrix = mat<4, 4>;

struct VSInput {
  Matrix viewport;
  Matrix project;
  Matrix viewmodel;
};

struct PSInput {
  vec3 intensity;
};

class IShader {
public:
  virtual ~IShader(){};
  virtual vec4 vertex(vec3 pt) = 0;
  virtual bool fragment(vec3 bar, TGAColor &color) = 0;
};

class GouraudShader : public IShader {
public:
  PSInput psInput;
  VSInput vsInput;

  virtual ~GouraudShader() {}

  explicit GouraudShader(VSInput input) : vsInput(input) {}

  vec4 vertex(vec3 pt) override {
    vec4 v = embed<4>(pt);
    auto ret = vsInput.viewport * vsInput.project * vsInput.viewmodel * v;
    return ret;
  }
  bool fragment(vec3 bar, TGAColor &color) override {
    auto intensity = bar * psInput.intensity;
    color = TGAColor({static_cast<uint8_t>(255 * intensity),
                      static_cast<uint8_t>(intensity * 255),
                      static_cast<uint8_t>(intensity * 255)});
    return false;
  }
};

void pipeline(std::array<vec4, 3> pts, IShader &shader, TGAImage &image,
              TGAImage &zbuffer);
