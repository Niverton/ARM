#ifndef CANVAS_H
#define CANVAS_H

#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>
#include <QGLWidget>
#include <cstdlib>
#include <QGLShaderProgram>
#include <QGLShader>
#include <QMatrix4x4>
#include <memory>

template <typename T>
struct Vec3 {
	T x, y, z;
};

class Canvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
	std::vector<Vec3<float>> vertexArray;
	std::vector<Vec3<unsigned short>> faceArray;
    std::unique_ptr<QGLShaderProgram> program;
    //virtual void keyPressEvent( QKeyEvent *keyEvent );

private:
    void initializeProgram();
    void initializeGeometry();
    unsigned int vertexBuffer, faceBuffer, instanceBuffer;
    //std::vector<QVector2> positionArray;
    std::unique_ptr<QGLShader> vertexShader, fragmentShader;
    QMatrix4x4 mView, mProj, mObj;
};

#endif
