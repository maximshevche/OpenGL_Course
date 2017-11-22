#include <string>
#include <ios>
#include <fstream>
#include <iostream>
#include <sstream>
#include "ShadersLoader.h"

std::string shaders::loadShaderSourceFromFile(const char *path) {
    std::string shaderCode;
    std::ifstream fileStream;

    fileStream.exceptions(std::ios::badbit);
    try {
        fileStream.open(path, std::ios::in);

        if (!fileStream.is_open()) {
            throw std::runtime_error("Can't read shader data from file");
        }

        std::stringstream stringstream;
        stringstream << fileStream.rdbuf();
        shaderCode = stringstream.str();

        fileStream.close();
    } catch (std::ios::failure& exception) {
        std::cerr << "Error opening file '" << path << "', exception - '" << exception.what() << "', error code - '" << exception.code() << "'.\n";
    }

    return shaderCode;
}