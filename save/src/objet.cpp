#include "objet.h"

Objet::Objet()
{
    this->nom = "Test";
    unsigned int nb_sommets = 8;
    unsigned int nb_faces = 2*6;
    GLfloat lesSommets[ 3 * nb_sommets ] =
    {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f
    };
    this->sommets = std::vector<GLfloat> ( &(lesSommets[0]), &(lesSommets[3*nb_sommets]) );

    GLfloat lesNormales[ 3*nb_sommets ] =
    {
        0.0f -0.5f, 0.0f -0.5f, 0.0f -0.5f,
        0.0f -0.5f, 0.0f -0.5f, 1.0f -0.5f,
        0.0f -0.5f, 1.0f -0.5f, 1.0f -0.5f,
        0.0f -0.5f, 1.0f -0.5f, 0.0f -0.5f,

        1.0f -0.5f, 0.0f -0.5f, 0.0f -0.5f,
        1.0f -0.5f, 0.0f -0.5f, 1.0f -0.5f,
        1.0f -0.5f, 1.0f -0.5f, 1.0f -0.5f,
        1.0f -0.5f, 1.0f -0.5f, 0.0f -0.5f
    };
    this->normales = std::vector<GLfloat> ( &(lesNormales[0]), &(lesNormales[3*nb_sommets]) );
    GLuint lesIndices[ 3*nb_faces ] =
    {
        1, 3, 0, 1, 2, 3,
        1, 5, 2, 5, 6, 2,
        5, 4, 6, 4, 7, 6,
        0, 7, 4, 0, 3, 7,
        6, 7, 3, 2, 6, 3,
        0, 4, 1, 4, 5, 1
    };
    this->indices_faces = std::vector<GLuint> ( &(lesIndices[0]), &(lesIndices[3*nb_faces]) );
}
Objet::Objet ( QString fich )
{
    QFile fichier ( fich );
    if ( !fichier.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        std::cout << "Erreur lors de l'ouverture du fichier" << std::endl;
        return;
    }
    QFileInfo info = QFileInfo( fich );
    this->nom = info.baseName();
    QTextStream in( &fichier );

    this->isPatch      = false;
    this->nb_patchs    = 0;

    while ( !in.atEnd() )
    {
        QStringList list = in.readLine().split( " ", QString::SkipEmptyParts );
        if ( list.size() > 0 )
        {
            if ( list[0] == "v" )
            {
                this->sommets.push_back( list[1].toFloat() );
                this->sommets.push_back( list[2].toFloat() );
                this->sommets.push_back( list[3].toFloat() );
            }
            else if ( list[0] == "vt" )
            {
                this->textures.push_back( list[1].toFloat() );
                this->textures.push_back( list[2].toFloat() );
            }
            else if ( list[0] == "vn" )
            {
                this->normales.push_back( list[1].toFloat() );
                this->normales.push_back( list[2].toFloat() );
                this->normales.push_back( list[3].toFloat() );
            }
            else if ( list[0] == "f" )
            {
                this->type_face = list.size() - 1;
                for ( int i = 1 ; i < list.size() ; i++ )
                {
                    QStringList subList = list[i].split( "/", QString::SkipEmptyParts );
                    this->indices_faces.push_back( subList[0].toUInt() - 1 ); // Les .obj comptent leurs indices en partant de 1
                    // TODO prise en compte des normales / textures ( sous la forme V/T/N dans le .obj )
                }
            }
            else if ( list[0] == "" || list[0] == "#" ){}
            else
            {
                std::cout << "Caractère inconnu : " << list[0].toStdString() << std::endl;
            }
        }
    }

    this->posX   = 0.0f;
    this->posY   = 0.0f;
    this->posZ   = 0.0f;
    this->scaleX = 1.5f;
    this->scaleY = 1.5f;
    this->scaleZ = 1.5f;
    this->roll   = 0.0f;
    this->pitch  = 0.0f;
    this->yaw    = 0.0f;

    this->materiau = Materiau();
}
QString Objet::getNom ( void ) const
{
    return this->nom;
}
std::vector<GLuint> Objet::getIndices ( void ) const
{
    return this->indices_faces;
}
std::vector<GLfloat> Objet::getSommets( void ) const
{
    return this->sommets;
}

void Objet::setPatchs ( const std::vector<GLuint>& ind )
{
    this->patchs_indexes = ind;
}
void Objet::setNbPatchs(unsigned int i)
{
    this->nb_patchs = i;
}
void Objet::setSommetsPatchs ( const std::vector<GLfloat> &ind ) // Si on a besoin de faire de la "vraie" subdivision (Doo-Sabin)
{
    this->sommets_patchs = ind;
}
void Objet::setIndicesPatchs ( const std::vector<GLuint>& ind )
{
    this->patchs = ind;
}
void Objet::setFacesPatchs ( const std::vector<GLuint> &ind )
{
    this->faces_patchs = ind;
}

void Objet::initVBO ( void )
{
    /*
    // Textures
    glGenBuffers( 1, &(this->buff_textures) );
    glBindBuffer(GL_ARRAY_BUFFER, this->buff_textures );
    glBufferData(GL_ARRAY_BUFFER, this->nb_texCoords * 2 * sizeof(GLfloat), &(this->textures[0]), GL_STATIC_DRAW);

    // Normales
    glGenBuffers( 1, &(this->buff_normales) );
    glBindBuffer(GL_ARRAY_BUFFER, this->buff_normales );
    glBufferData(GL_ARRAY_BUFFER, this->nb_normales * 3 * sizeof(GLfloat), &(this->normales[0]), GL_STATIC_DRAW);
    */

    // Dans le cas de la subdivision
    if ( this->isPatch )
    {
        // Sommets de départ (input du pipeline)
        glGenBuffers( 1, &(this->buff_sommets_patchs_depart) );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, this->buff_sommets_patchs_depart );
        glBufferData( GL_SHADER_STORAGE_BUFFER, this->sommets_patchs.size() * sizeof(GLfloat), &(this->sommets_patchs[0]), GL_STATIC_DRAW);

        // Sommets après passage dans le Vertex Shader
        glGenBuffers( 1, &(this->buff_sommets_patchs) );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, this->buff_sommets_patchs );
        glBufferData( GL_SHADER_STORAGE_BUFFER, this->sommets_patchs.size() * sizeof(GLfloat), &(this->sommets_patchs[0]), GL_STATIC_DRAW);

        // Indices des sommets par faces
        glGenBuffers( 1, &(this->buff_faces_patchs) );
        glBindBuffer( GL_ARRAY_BUFFER, this->buff_faces_patchs );
        glBufferData(GL_ARRAY_BUFFER, this->faces_patchs.size() * sizeof(GLuint), &(this->faces_patchs[0]), GL_STATIC_DRAW);

        // Indices des sommets par patchs
        glGenBuffers( 1, &(this->buff_patchs) );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, this->buff_patchs );
        glBufferData( GL_SHADER_STORAGE_BUFFER, this->patchs.size() * sizeof(GLuint), &(this->patchs[0]), GL_STATIC_DRAW);

        // Les indices des patchs (début + taille)
        glGenBuffers( 1, &(this->buff_patchs_indexes) );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, this->buff_patchs_indexes );
        glBufferData( GL_SHADER_STORAGE_BUFFER, this->patchs_indexes.size() * sizeof(GLuint), &(this->patchs_indexes[0]), GL_STATIC_DRAW);
    }
    else
    {
        // Sommets
        glGenBuffers( 1, &(this->buff_sommets) );
        glBindBuffer(GL_ARRAY_BUFFER, this->buff_sommets );
        glBufferData(GL_ARRAY_BUFFER, this->sommets.size() * sizeof(GLfloat), &(this->sommets[0]), GL_STATIC_DRAW);

        // Indices des sommets par faces
        glGenBuffers( 1, &(this->buff_indices) );
        glBindBuffer( GL_ARRAY_BUFFER, this->buff_indices );
        glBufferData(GL_ARRAY_BUFFER, this->indices_faces.size() * sizeof(GLuint), &(this->indices_faces[0]), GL_STATIC_DRAW);
    }
}

glm::vec3 Objet::getPosition( void ) const
{
    return glm::vec3 ( this->posX, this->posY, this->posZ );
}
glm::mat4 Objet::getMatriceModel( void ) const
{
    glm::mat4 T = glm::translate( glm::vec3(this->posX, this->posY, this->posZ) );
    glm::mat4 R = glm::mat4(1.0f); // A bosser mais je galère toujours autant avec les rotations
    glm::mat4 S = glm::scale( glm::vec3(this->scaleX, this->scaleY, this->scaleZ) );
    return T * R * S;
}
unsigned int Objet::getTypeFace( void ) const
{
    return this->type_face;
}
void Objet::setTypePatch( bool b )
{
    this->isPatch = b;
}
Materiau& Objet::getMateriau( void )
{
    return this->materiau;
}

void Objet::afficher ( void ) const
{
    // Positions
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_sommets );
    glVertexPointer( 3, GL_FLOAT, 0, 0 );
    /*
    // Textures
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_textures );
    glTexCoordPointer( 2, GL_FLOAT, 0, 0 );

    // Normales
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_normales );
    glNormalPointer( GL_FLOAT, 0, 0 );
    */
    // Le rendu
    glEnableClientState( GL_VERTEX_ARRAY );
    this->rendu();
    glDisableClientState( GL_VERTEX_ARRAY );
}
void Objet::afficher ( ShaderProgram& shader ) const
{
    // Positions
    if ( isPatch )
    {
        glBindBuffer( GL_ARRAY_BUFFER, this->buff_sommets_patchs_depart );
        shader.setVertexAttribut ( "position", 3, GL_FLOAT, 0 );
    }
    else
    {
        glBindBuffer( GL_ARRAY_BUFFER, this->buff_sommets );
        shader.setVertexAttribut ( "position", 3, GL_FLOAT, 0 );
    }

    /* Attention, possibilité de gérer plusieurs coordonnées de textures / normales par sommets
    // Textures
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_textures );
    shader.setVertexAttribut ( "texture", 2, GL_FLOAT, 0 );

    // Normales
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_normales );
    shader.setVertexAttribut ( "normale", 3, GL_FLOAT, 0 );
    */

    // Le rendu
    this->materiau.sendToShader( shader.getIndice() );
    this->rendu();
}
void Objet::rendu ( void ) const
{
    // GL_ARRAY_BUFFER -> pas indexé
    // GL_ELEMENT_ARRAY_BUFFER -> indexé
    glPointSize( 1.0f );
    if ( isPatch )
    {
        // Si on est en mode subdivision, on doit envoyer au shader les informations supplémentaires de voisinage
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, this->buff_sommets_patchs );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, this->buff_patchs );
        glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, this->buff_patchs_indexes );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

        // L'affichage dépend du maillage de contrôle
        glPatchParameteri( GL_PATCH_VERTICES,  this->type_face );                      // Le nombre de points par face
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->buff_faces_patchs );              // La liste des faces
        glDrawElements( GL_PATCHES, this->faces_patchs.size(), GL_UNSIGNED_INT, 0 );   // On lance le rendu
    }
    else
    {
        // L'affichage dépend du maillage lu dans le .obj
        glPatchParameteri( GL_PATCH_VERTICES,  this->type_face );                       // Le nombre de points par face
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->buff_indices );                    // La liste des faces
        glDrawElements( GL_PATCHES, this->indices_faces.size(), GL_UNSIGNED_INT, 0 );   // On lance le rendu
    }

}
void Objet::toString ( void )
{
    for ( unsigned int i = 0 ; i < this->sommets.size() ; i += 3 )
    {
        std::cout << "v : ( " << this->sommets[3*i] << ", "  << this->sommets[3*i+1] << ", "  << this->sommets[3*i+2] << " ) " << std::endl;
    }
    for ( unsigned int i = 0 ; i < this->indices_faces.size() ; i++ )
    {
        std::cout << "f : ( ";
        for ( unsigned int j = 0 ; j < this->type_face-1 ; j++ )
        {
            std::cout << this->indices_faces[3*i+j]+1 << ", ";
        }
        std::cout << this->indices_faces[3*i+this->type_face-1]+1 << " ) " << std::endl;
    }
}
