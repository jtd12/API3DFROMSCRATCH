  #include"matrix.hpp"



  Matrix4x4::Matrix4x4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    
          Matrix4x4::Matrix4x4(float mat[4][4]) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = mat[i][j];
    }
    


vector3d Matrix4x4::transformVector(const vector3d& v) const {
    return {
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    };
}


void Matrix4x4::setRotation(float pitch, float yaw, float roll) {
    float cosPitch = cos(pitch), sinPitch = sin(pitch);
    float cosYaw = cos(yaw), sinYaw = sin(yaw);
    float cosRoll = cos(roll), sinRoll = sin(roll);

    m[0][0] = cosYaw * cosRoll;
    m[0][1] = -cosYaw * sinRoll;
    m[0][2] = sinYaw;
    
    m[1][0] = sinPitch * sinYaw * cosRoll + cosPitch * sinRoll;
    m[1][1] = -sinPitch * sinYaw * sinRoll + cosPitch * cosRoll;
    m[1][2] = -sinPitch * cosYaw;
    
    m[2][0] = -cosPitch * sinYaw * cosRoll + sinPitch * sinRoll;
    m[2][1] = cosPitch * sinYaw * sinRoll + sinPitch * cosRoll;
    m[2][2] = cosPitch * cosYaw;
}


vector3d Matrix4x4::operator*(const vector3d& v) const {
    // Création d'un vecteur homogène à partir de vector3d
    float result[4];
    result[0] = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]; // x
    result[1] = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]; // y
    result[2] = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]; // z
    result[3] = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]; // w

    // Gestion de la division par w (évite la division par zéro)
    if (result[3] != 0.0f) {
        return vector3d(result[0] / result[3], result[1] / result[3], result[2] / result[3]);
    } else {
        return vector3d(result[0], result[1], result[2]); // Évite les divisions par 0
    }
}
    

	Matrix4x4 Matrix4x4:: inverse() const {
    Matrix4x4 result;
    float temp[16];

    // Copier m[4][4] dans temp[16]
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i * 4 + j] = m[i][j];
        }
    }

    // Initialisation de la matrice résultat comme matrice identité
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    for (int i = 0; i < 4; i++) {
        // Chercher le pivot
        float maxElement = std::abs(temp[i * 4 + i]);
        int maxRow = i;
        for (int j = i + 1; j < 4; j++) {
            if (std::abs(temp[j * 4 + i]) > maxElement) {
                maxElement = std::abs(temp[j * 4 + i]);
                maxRow = j;
            }
        }

        if (maxRow != i) {
            // Échanger les lignes
            for (int k = 0; k < 4; k++) {
                std::swap(temp[i * 4 + k], temp[maxRow * 4 + k]);
                std::swap(result.m[i][k], result.m[maxRow][k]);
            }
        }

        // Normalisation de la ligne du pivot
        float pivot = temp[i * 4 + i];
        for (int j = 0; j < 4; j++) {
            temp[i * 4 + j] /= pivot;
            result.m[i][j] /= pivot;
        }

        // Réduire les autres lignes
        for (int j = 0; j < 4; j++) {
            if (j != i) {
                float factor = temp[j * 4 + i];
                for (int k = 0; k < 4; k++) {
                    temp[j * 4 + k] -= factor * temp[i * 4 + k];
                    result.m[j][k] -= factor * result.m[i][k];
                }
            }
        }
    }
    return result;
}

	
		Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    
    
    

        vector3d Matrix4x4::multiply(const vector3d& v) const {
        vector3d result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        return result;
    }
    
     
    


	void  Matrix4x4::setScaling(float sx, float sy, float sz) {
        m[0][0] = sx;  m[0][1] = 0;   m[0][2] = 0;   m[0][3] = 0;
        m[1][0] = 0;   m[1][1] = sy;  m[1][2] = 0;   m[1][3] = 0;
        m[2][0] = 0;   m[2][1] = 0;   m[2][2] = sz;  m[2][3] = 0;
        m[3][0] = 0;   m[3][1] = 0;   m[3][2] = 0;   m[3][3] = 1;
    }

    // Définir une matrice de translation
    void Matrix4x4::setTranslation(float tx, float ty, float tz) {
        m[0][3] = tx;
        m[1][3] = ty;
        m[2][3] = tz;
    }

    // Appliquer la transformation à un vecteur
    vector3d Matrix4x4::apply(const vector3d& v) const {
        return vector3d(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3]
        );
    }
    
    void Matrix4x4::setColumn(int colIndex, const vector3d& v) {
        if (colIndex < 0 || colIndex >= 4) {
            std::cerr << "Index de colonne invalide!" << std::endl;
            return;
        }

        // Définir les éléments de la colonne
        m[0][colIndex] = v.x;
        m[1][colIndex] = v.y;
        m[2][colIndex] = v.z;
        m[3][colIndex] = 0.0f;  // Le quatrième élément est souvent 0 pour une transformation de vecteurs 3D
    }
    
    
    Matrix4x4& Matrix4x4::setRotationX(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    this->m[0][0] = 1;   this->m[0][1] = 0;   this->m[0][2] = 0;   this->m[0][3] = 0;
    this->m[1][0] = 0;   this->m[1][1] = c;   this->m[1][2] = -s;  this->m[1][3] = 0;
    this->m[2][0] = 0;   this->m[2][1] = s;   this->m[2][2] = c;   this->m[2][3] = 0;
    this->m[3][0] = 0;   this->m[3][1] = 0;   this->m[3][2] = 0;   this->m[3][3] = 1;
    return *this;
}

    Matrix4x4& Matrix4x4:: setRotationY(float angle) {
	float c = cos(angle);
    float s = sin(angle);

    this->m[0][0] = c;  this->m[0][1] = 0; this->m[0][2] = -s; this->m[0][3] = 0;
    this->m[1][0] = 0;  this->m[1][1] = 1; this->m[1][2] =  0; this->m[1][3] = 0;
    this->m[2][0] = s;  this->m[2][1] = 0; this->m[2][2] =  c; this->m[2][3] = 0;
    this->m[3][0] = 0;  this->m[3][1] = 0; this->m[3][2] =  0; this->m[3][3] = 1;
    
    return *this; // Retourne la matrice pour chaînage
    }
    
    Matrix4x4& Matrix4x4::setRotationZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    this->m[0][0] = c;   this->m[0][1] = -s;  this->m[0][2] = 0;   this->m[0][3] = 0;
    this->m[1][0] = s;   this->m[1][1] = c;   this->m[1][2] = 0;   this->m[1][3] = 0;
    this->m[2][0] = 0;   this->m[2][1] = 0;   this->m[2][2] = 1;   this->m[2][3] = 0;
    this->m[3][0] = 0;   this->m[3][1] = 0;   this->m[3][2] = 0;   this->m[3][3] = 1;
    return *this;
}
