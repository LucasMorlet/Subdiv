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
    /*
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
    */
    //this->normales = std::vector<GLfloat> ( &(lesNormales[0]), &(lesNormales[3*nb_sommets]) );
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
                this->tex_coords.push_back( list[1].toFloat() );
                this->tex_coords.push_back( list[2].toFloat() );
            }
            else if ( list[0] == "vn" )
            {
                /*
                this->normales.push_back( list[1].toFloat() );
                this->normales.push_back( list[2].toFloat() );
                this->normales.push_back( list[3].toFloat() );
                */
            }
            else if ( list[0] == "f" )
            {
                if ( this->sommets_indexes.empty() )
                {
                    this->type_face = list.size() - 1;
                    if ( this->tex_coords.size() > 0 )
                    {
                        this->sommets_indexes.resize( 3 * this->tex_coords.size() / 2 );
                    }
                    else
                    {
                        this->sommets_indexes.resize( this->sommets.size() );
                        this->tex_coords.resize( 2 * this->sommets_indexes.size() / 3 ); // Coordonnées vides pour éviter les pointeurs out of bounds
                    }
                }
                for ( int i = 1 ; i < list.size() ; i++ )
                {
                    QStringList subList = list[i].split( "/" );
                    unsigned int index_sommet  = subList[0].toUInt() - 1;
                    unsigned int index_texture;
                    if ( subList[1] == "" )
                    {
                         index_texture = index_sommet; // Si aucune texture n'est indiquée
                    }
                    else
                    {
                         index_texture = subList[1].toUInt() - 1;
                    }
                    // On copie le sommet géométrique dans la liste des sommets indexés selon leurs coordonnées de textures
                    this->sommets_indexes[ 3*index_texture     ] = this->sommets[ 3*index_sommet     ];
                    this->sommets_indexes[ 3*index_texture + 1 ] = this->sommets[ 3*index_sommet + 1 ];
                    this->sommets_indexes[ 3*index_texture + 2 ] = this->sommets[ 3*index_sommet + 2 ];
                    // On renseigne les faces géométrique et de textures
                    this->indices_faces.push_back( index_sommet  );
                    this->indices_faces_indexees.push_back( index_texture );
                }
                /*
                for ( int i = 1 ; i < list.size() ; i++ )
                {
                    QStringList subList = list[i].split( "/", QString::SkipEmptyParts );
                    this->indices_faces.push_back( subList[0].toUInt() - 1 ); // Les .obj comptent leurs indices en partant de 1
                }*/
            }
            else if ( list[0] == "#" )
            {
                // Les commentaires ou lignes vides
            }
            else if ( list[0] == "o" || list[0] == "s" || list[0] == "mtllib" || list[0] == "usemtl" )
            {
                // Les informations de matériau que l'on n'utilise pas
            }
            else
            {
                std::cout << "Caractère inconnu : " << list[0].toStdString() << std::endl;
            }
        }
    }

    this->position = glm::vec3 ( 0.0f, 0.0f, 0.0f );
    this->scale = glm::vec3 ( 1.5f, 1.5f, 1.5f );
    this->roll   = 0.0f;
    this->pitch  = 0.0f;
    this->yaw    = 0.0f;
    this->calculerMatrice();
    this->materiau = Materiau();

    // Les textures
    fich.truncate( fich.lastIndexOf('/') );
    QString path_color = fich + "/textures/" + this->nom + "_color.png";
    QString path_norm = fich + "/textures/" + this->nom + "_norm.png";
    if ( QFile::exists( path_color ) )
    {
        this->color_map = Texture ( path_color, color );
    }
    else
    {
        this->color_map = Texture ( fich + "/textures/default_color.png", color );
    }
    if ( QFile::exists( path_norm ) )
    {
        this->normal_map = Texture ( path_norm, normal );
    }
    else
    {
        this->normal_map = Texture ( fich + "/textures/default_norm.png", normal );
    }
}
const QString& Objet::getNom( void ) const
{
    return this->nom;
}
const std::vector<GLuint>& Objet::getIndices ( void ) const
{
    return this->indices_faces;
}
const std::vector<GLuint>& Objet::getIndicesText  ( void ) const
{
    return this->indices_faces_indexees;
}
const std::vector<GLfloat>& Objet::getSommets( void ) const
{
    return this->sommets;
}
const std::vector<GLfloat>& Objet::getSommetsText( void ) const
{
    return this->sommets_indexes;
}

void Objet::setPatchs ( const std::vector<GLuint>& ind )
{
    this->patchs_indexes = ind;
}
void Objet::setNbPatchs(unsigned int i)
{
    this->nb_patchs = i;
}
void Objet::setSommetsPatchs ( const std::vector<GLfloat>& ind ) // Si on a besoin de faire de la "vraie" subdivision (Doo-Sabin)
{
    this->sommets_patchs = ind;
}
void Objet::setIndicesPatchs ( const std::vector<GLuint>& ind )
{
    this->patchs = ind;
}
void Objet::setFacesPatchs ( const std::vector<GLuint>& ind )
{
    this->faces_patchs = ind;
}
void Objet::setMateriau ( float amb , float diff, float spec, int hard )
{
    this->materiau.setMateriau( amb, diff, spec, hard );
}

void Objet::initVBO ( void )
{
    // Dans le cas de la subdivision
    if ( this->isPatch )
    {
        // Sommets de départ avec les coordonnées de textures associées (input du pipeline)
        glGenBuffers( 1, &(this->buff_sommets_patchs_depart) );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, this->buff_sommets_patchs_depart );
        glBufferData( GL_SHADER_STORAGE_BUFFER, this->sommets_patchs.size() * sizeof(GLfloat), &(this->sommets_patchs[0]), GL_STATIC_DRAW);

        // Position des sommets après passage dans le Vertex Shader (donc potentiellement modifiés) qui seront utilisés dans le TES
        glGenBuffers( 1, &(this->buff_sommets_patchs) );
        glBindBuffer( GL_SHADER_STORAGE_BUFFER, this->buff_sommets_patchs );
        glBufferData( GL_SHADER_STORAGE_BUFFER, this->sommets_patchs.size() * sizeof(GLfloat), &(this->sommets_patchs[0]), GL_STATIC_DRAW);

        // Indices des sommets par faces
        glGenBuffers( 1, &(this->buff_faces_patchs) );
        glBindBuffer( GL_ARRAY_BUFFER, this->buff_faces_patchs );
        glBufferData( GL_ARRAY_BUFFER, this->faces_patchs.size() * sizeof(GLuint), &(this->faces_patchs[0]), GL_STATIC_DRAW);

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
        // Sommets    /* Attention, possibilité de gérer plusieurs coordonnées de textures / normales par sommets*/
        glGenBuffers( 1, &(this->buff_sommets) );
        glBindBuffer(GL_ARRAY_BUFFER, this->buff_sommets );
        glBufferData(GL_ARRAY_BUFFER, this->sommets_indexes.size() * sizeof(GLfloat), &(this->sommets_indexes[0]), GL_STATIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, this->sommets.size() * sizeof(GLfloat), &(this->sommets[0]), GL_STATIC_DRAW);

        // Indices des sommets par faces
        glGenBuffers( 1, &(this->buff_indices) );
        glBindBuffer( GL_ARRAY_BUFFER, this->buff_indices );
        glBufferData( GL_ARRAY_BUFFER, this->indices_faces_indexees.size() * sizeof(GLuint), &(this->indices_faces_indexees[0]), GL_STATIC_DRAW);
    }
    // Textures
    glGenBuffers( 1, &(this->buff_textures) );
    glBindBuffer(GL_ARRAY_BUFFER, this->buff_textures );
    glBufferData(GL_ARRAY_BUFFER, this->tex_coords.size() * sizeof(GLfloat), &(this->tex_coords[0]), GL_STATIC_DRAW);

    this->color_map.sendToMemory();
    this->normal_map.sendToMemory();
}

const glm::vec3& Objet::getPosition( void ) const
{
    return this->position;
}
const glm::mat4& Objet::getMatriceModel( void ) const
{
    return this->matrice_modele;
}
unsigned int Objet::getTypeFace( void ) const
{
    return this->type_face;
}
void Objet::setTypePatch( bool b )
{
    this->isPatch = b;
}
const Materiau& Objet::getMateriau( void ) const
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

    // Textures
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_textures );
    shader.setVertexAttribut ( "texture", 2, GL_FLOAT, 0 );

    /* Normales
    glBindBuffer( GL_ARRAY_BUFFER, this->buff_normales );
    shader.setVertexAttribut ( "normale", 3, GL_FLOAT, 0 );
    */

    // Le rendu
    this->materiau.sendToShader( shader.getIndice() );
    this->color_map.sendToShader ( shader.getIndice() );
    this->normal_map.sendToShader( shader.getIndice() );
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

void Objet::calculerMatrice ( void )
{
    glm::mat4 T = glm::translate( this->position );
    glm::mat4 R = glm::mat4(1.0f); // A bosser mais je galère toujours autant avec les rotations
    glm::mat4 S = glm::scale( this->scale );
    this->matrice_modele = T * R * S;
}

void Objet::toString ( void ) const
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
