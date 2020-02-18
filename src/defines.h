#pragma once

#include <string>
#include <iostream>

namespace toy_renderer {

const std::string rootPath("/Users/apple/Pepcy_Ch/Program/Courses/CG/toy-renderer");
const std::string assetsPath = rootPath + "/assets";
const std::string scenePath = assetsPath + "/scenes";
const std::string texturePath = assetsPath + "/textures";
const std::string GLshaderPath = assetsPath + "/GLshaders";

const bool isRetina = 1;

// float point type alias
#ifdef TR_FP_64bit
using Float = double;
#else
using Float = float;
#endif

// log
#define LOG(expr) std::cerr << expr << std::endl
#define CHECK(expr) std::cerr << #expr ": " << (expr) << std::endl

}
