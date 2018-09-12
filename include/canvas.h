#ifndef CANVAS_H
#define CANVAS_H

#include <QtOpenGL>
#include <QGLWidget>
#include <stdlib>

template <typename T>
struct Vec3 {
	T x, y, z;
}

class Canvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    //virtual void keyPressEvent( QKeyEvent *keyEvent );
        
public slots:
    virtual void timeOutSlot();

private:
    unsigned int vertexBuffer, faceBuffer;
	std::vector<Vec3<unsigned int>> vertexArray;
	std::vector<Vec3<short>> faceArray;
};


#endif
