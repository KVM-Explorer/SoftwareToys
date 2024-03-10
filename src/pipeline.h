#include "geometry.h"
#include "tgaimage.h"
#include <array>
#include <format>
#include <optional>

using Matrix = mat<4, 4>;

struct VSInput {
  Matrix viewport;
  Matrix project;
  Matrix viewmodel;
  Matrix projectNDC;
  vec3 lightDir;
};

struct PSInput {
  TGAImage diffuse;
  vec3 intensity;
  mat<2, 3> uv;
  vec3 lightDir;
};

class IShader {
public:
  virtual ~IShader(){};
  virtual vec4 vertex(vec3 pt, std::optional<vec3> normal,
                      std::optional<vec2> uv, int idx) = 0;
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

  vec4 vertex(vec3 pt, std::optional<vec3> normal, std::optional<vec2> uv,
              int idx) override {
    vec4 v = embed<4>(pt);
    if (normal.has_value())
      psInput.intensity[idx] = std::max(0.0, normal.value() * vsInput.lightDir);
    if (uv.has_value())
      psInput.uv.set_col(idx, uv.value());
    auto ret = vsInput.project * vsInput.viewmodel * v;
    ret = ret / ret[3]; // 齐次归一化到NDC
    auto retNDC = vsInput.projectNDC * vsInput.viewmodel * v;
    retNDC = retNDC / retNDC[3];
    // if (ret[2] > 0 || ret[2] < -1) {
    static float minV = std::numeric_limits<float>::max();
    static float maxV = -std::numeric_limits<float>::max();
    static float minNDC = std::numeric_limits<float>::max();
    static float maxNDC = -std::numeric_limits<float>::max();
    minV = std::min(minV, static_cast<float>(ret[2]));
    maxV = std::max(maxV, static_cast<float>(ret[2]));
    minNDC = std::min(minV, static_cast<float>(retNDC[2]));
    maxNDC = std::max(maxV, static_cast<float>(retNDC[2]));
    std::cout << std::format("MinV: {} MaxV: {} minNDC: {} maxNDC: {}\n", minV,
                             maxV, minNDC, maxNDC);
    // minNDC 存在异常，下界有问题：MinV: 0.4274048 MaxV: 0.5459242 minNDC:
    // -1.0682955 maxNDC: 0.5459242
    // }
    return vsInput.viewport * retNDC;
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

class PhongShading : public IShader {
private:
  Matrix uniformM;
  Matrix uniformInvM;
  Matrix viewport;
  PSInput psInput;

public:
  PhongShading(TGAImage &diffuse, VSInput input);
  vec4 vertex(vec3 pt, std::optional<bool> normal, std::optional<vec2> uv,
              int idx);
  bool fragment(vec3 bar, TGAColor &color) override;
};

class ShadowShader : public IShader {
public:
  ShadowShader();
  vec4 vertex(vec3 pt, std::optional<vec3> normal, std::optional<vec2> uv,
              int idx) override;
  bool fragment(vec3 bar, TGAColor &color) override;
};

void pipeline(std::array<vec4, 3> pts, IShader &shader, TGAImage &image,
              TGAImage &zbuffer);
