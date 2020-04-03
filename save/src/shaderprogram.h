#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QString>
#include "glm/glm.hpp"
#include "shader.h"
#include <vector>

class ShaderProgram
{
    private :
        bool on_off;
        GLuint indice;

    public :
        ShaderProgram( void );
        ~ShaderProgram( void );

        GLuint getIndice ( void ) const;
        bool isActivated ( void ) const;
        void addShader ( Shader* s );
        void delShader ( Shader* s );

        template<typename T> void setUniform ( const char* nom, GLenum type, T valeur ) const;
        void setVertexAttribut ( const char* nom, GLint nb, GLenum type, GLsizei stride ) const;
        void setVector(const char* nom, const std::vector<GLfloat>& tableau ) const;
        void setMat4x4 ( const char* nom, const glm::mat4& valeur ) const;

        void create ( void );
        void link ( void );
        void activer ( QString* str );
        void desactiver ( QString* str );
        void changerEtat ( QString* str );

        static void infoGPU ( void );
};

#include "shaderprogram.tpp" // Pour les fonctions templatées

#endif // SHADERPROGRAM_H
