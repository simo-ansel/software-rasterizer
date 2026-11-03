#include <fstream>
#include <iostream>
#include <string_view>

#include "ppm.hpp"
#include "scenes.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: render <scene> <file>\n";
        return 2;
    }
    const std::string_view scene = argv[1];
    const char* path = argv[2];

    // One scene per stage
    if (scene != "t1_gradient") {
        std::cerr << "unknown scene: " << scene << "\n";
        return 2;
    }
    const raster::Framebuffer fb = raster::render_gradient(320, 180);

    // Binary mode: text mode would alter the bytes equal to '\n' on Windows
    std::ofstream out(path, std::ios::binary);
    raster::write_ppm(out, fb);
    out.flush();
    if (!out) {
        std::cerr << "cannot write " << path << "\n";
        return 1;
    }
}
