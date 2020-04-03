template<typename T> void ShaderProgram::setUniform ( const char* nom, GLenum type, T valeur ) const
{
    GLint uniform = glGetUniformLocation( this->indice, nom );
    switch ( type )
    {
        // Si l'attribut est entier
        case GL_INT :
        case GL_BYTE :
        case GL_SHORT :
        {
            glUniform1i ( uniform, valeur );
        }
        break;
        case GL_UNSIGNED_INT :
        case GL_UNSIGNED_BYTE :
        case GL_UNSIGNED_SHORT :
        {
            glUniform1ui ( uniform, valeur );
        }
        break;
        case GL_DOUBLE :
        case GL_FLOAT :
        {
            glUniform1f ( uniform, valeur );
        }
        break;
    }

}
