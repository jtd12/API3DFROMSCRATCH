#ifndef DEF_MATRIX_HPP
#define DEF_MATRIX_HPP

#include"vector.h"
#include <stack> // Inclure cette ligne pour utiliser std::stack
using namespace std;

class Matrix4x4 {
public:

    float m[4][4];
    // Initialiser en matrice identité
    Matrix4x4();
    Matrix4x4(float mat[4][4]);
    void setTranslation(float tx, float ty, float tz);
    void setScaling(float sx, float sy, float sz);
    void setColumn(int colIndex, const vector3d& v);
    void setRotation(float pitch, float yaw, float roll);
    vector3d transformVector(const vector3d& v) const;
    Matrix4x4& setRotationX(float angle);
    Matrix4x4& setRotationY(float angle);
    Matrix4x4 operator*(const Matrix4x4& other) const;
    Matrix4x4& setRotationZ(float angle);
    vector3d multiply(const vector3d& v) const;
    vector3d apply(const vector3d& v) const;
    Matrix4x4 inverse() const;
    vector3d operator*(const vector3d& v) const;
    



};

class MatrixStack {
private:
    std::stack<Matrix4x4> stack;

public:
    MatrixStack() {
        stack.push(Matrix4x4());  // Initialiser avec la matrice identité
    }

    void pushMatrix() {
        stack.push(stack.top());
    }

    void popMatrix() {
        if (stack.size() > 1) {
            stack.pop();
        } else {
            std::cerr << "Erreur : Impossible de dépiler la matrice, la pile est vide !" << std::endl;
        }
    }

    Matrix4x4 top() const {
        return stack.top();
    }

    void applyTransformation(const Matrix4x4& transformation) {
       // stack.top() = stack.top() * transformation;
    }
};

#endif
