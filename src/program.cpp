#include "program.h"

Program::Program(char* vertexFile, char* fragmentFile){
    QGLFunctions gl(QGLContext::currentContext());
    //Load vertex shader
    std::cout << "Compiling vertex shader..." << std::endl;
    std::string vertexSource = loadFromFile(vertexFile);
    vertexShaderId = gl.glCreateShader(GL_VERTEX_SHADER);
    const char* c_vertexSource = vertexSource.c_str();
    gl.glShaderSource(vertexShaderId, 1, &c_vertexSource, NULL);
    gl.glCompileShader(vertexShaderId);
    checkError(vertexShaderId);

    //Load fragment shader
    std::cout << "Compiling fragment shader..." << std::endl;
    std::string fragmentSource = loadFromFile(fragmentFile);
    fragmentShaderId = gl.glCreateShader(GL_FRAGMENT_SHADER);
    const char* c_fragmentSource = fragmentSource.c_str();
    gl.glShaderSource(fragmentShaderId, 1, &c_fragmentSource, NULL);
    gl.glCompileShader(fragmentShaderId);
    checkError(fragmentShaderId);

    //Create program
    programId = gl.glCreateProgram();
    gl.glAttachShader(programId, vertexShaderId);
	gl.glAttachShader(programId, fragmentShaderId);
	gl.glLinkProgram(programId);
}

unsigned int Program::getId(){
    return programId;
}

std::string Program::loadFromFile(char* path){
    std::string fileContent;
    std::ifstream stream;
	stream.open(path, std::ios::in);

	if(stream.is_open()) {
		std::stringstream sstr;
		sstr << stream.rdbuf();
		fileContent = sstr.str();
		stream.close();
	}

    return fileContent;
}

void Program::checkError(unsigned int Id){
    QGLFunctions gl(QGLContext::currentContext());
    int length = 0;

    gl.glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &length);
    if(length <= 0) return;

    std::vector<char> error_log(length+1);
    gl.glGetShaderInfoLog(Id, length, NULL, &error_log[0]);
    std::cerr << &error_log[0] << std::endl;
}