#ifndef PROGRAM_H
#define PROGRAM_H

#include <QtOpenGL>
#include <QGLFunctions>

#include <iostream>
#include <fstream>
#include <sstream>

class Program{
public:
    Program(char* vertexFile, char* fragmentFile);
    unsigned int getId();
private:
    QGLFunctions* gl;
    unsigned int vertexShaderId, fragmentShaderId, programId;
    void checkError(unsigned int Id);
    std::string loadFromFile(char* path);
};

#endif