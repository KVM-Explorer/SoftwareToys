#include "geometry.h"
#include "tgaimage.h"
#include <array>
#include <format>
using Matrix = mat<4, 4>;

struct VSInput {
  Matrix viewport;
  Matrix project;
  Matrix viewmodel;
  vec3 lightDir;
};

struct PSInput {
  TGAImage diffuse;
  vec3 intensity;
  mat<2, 3> uv;
};

class IShader {
public:
  virtual ~IShader(){};
  virtual vec4 vertex(vec3 pt, vec3 normal, vec2 uv, int idx) = 0;
  virtual bool fragment(vec3 bar, TGAColor &color) = 0;
};

class GouraudShader : public IShader {
public:
  PSInput psInput;
  VSInput vsInput;

  virtual ~GouraudShader() {}

  explicit GouraudShader(VSInput input, const TGAImage &diffuse)
      : vsInput(input) {
    psInput.diffuse = diffuse;
  }

  vec4 vertex(vec3 pt, vec3 normal, vec2 uv, int idx) override {
    vec4 v = embed<4>(pt);
    psInput.intensity[idx] = std::max(0.0, normal * vsInput.lightDir);
    psInput.uv.set_col(idx, uv);
    auto ret = vsInput.project * vsInput.viewmodel * v;
    ret = ret / ret[3]; // 齐次归一化到NDC
                        // if (ret[2] > 0 || ret[2] < -1) {
    static float minV = std::numeric_limits<float>::max();
    static float maxV = -std::numeric_limits<float>::max();
    minV = std::min(minV, static_cast<float>(ret[2]));
    maxV = std::max(maxV, static_cast<float>(ret[2]));
    std::cout<< std::format("MinV: {} MaxV: {}\n",minV,maxV);
    // }
    return vsInput.viewport * ret;
  }
  bool fragment(vec3 bar, TGAColor &color) override {
    float intensity = bar * psInput.intensity;
    vec2 uv = psInput.uv * bar;
    uv.x = uv.x * psInput.diffuse.width();
    uv.y = uv.y * psInput.diffuse.height();
    color = psInput.diffuse.get(uv.x, uv.y);
    // for (int k = 0; k < 4; k++)
    //   color[k] = color.bgra[k] * intensity;
    // color =
    //     TGAColor({static_cast<uint8_t>(255 * intensity),
    //               static_cast<uint8_t>(intensity * 255),
    //               static_cast<uint8_t>(intensity * 255)});
    // color = TGAColor({255, 255, 255, 0});
    return false;
  }
};

void pipeline(std::array<vec4, 3> pts, IShader &shader, TGAImage &image,
              TGAImage &zbuffer);
