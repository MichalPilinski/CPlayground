//
// Created by Michau on 12/09/2023.
//

#ifndef SDL2_RENDERER_MATERIAL_H
#define SDL2_RENDERER_MATERIAL_H

struct RgbColor {
    int r;
    int g;
    int b;
};

struct Material {
    struct RgbColor color;
    double specularConstant;
    double diffuseConstant;
    double ambientConstant;
    double shininessConstant;
};
#endif //SDL2_RENDERER_MATERIAL_H
