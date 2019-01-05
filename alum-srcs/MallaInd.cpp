// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

// *****************************************************************************
// funciones auxiliares


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd(): MallaInd("malla indexada, anónima")
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
  //ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
  ponerNombre(nombreIni) ;
  vbo_creado=false;
  id_vbo_ver=id_vbo_tri=0;
  id_vbo_color_ver=id_vbo_normal_ver=id_vbo_textura_ver=0;
  tam_ver=tam_tri=tam_tex=0;

}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
  //std::cout << "Calculo normales"<<std::endl;
  assert(!vertices.empty() && !triangulos.empty());
  normal_ver=std::vector<Tupla3f>(vertices.size(),Tupla3f{0.0,0.0,0.0});
   // Primero, comenzamos calculando la tabla de 1normales de las caras.
   for(unsigned i=0; i<triangulos.size(); i++){
     //Tomamos los vértices correspondientes a nuestras caras.
     Tupla3f p=vertices[triangulos[i](0)];
     Tupla3f q=vertices[triangulos[i](1)];
     Tupla3f r=vertices[triangulos[i](2)];

     // A continuación, calculamos las aristas.
     Tupla3f a=q-p;
     Tupla3f b=r-p;

     //Y, finalmente, calculamos el vector normal y hacemos que tenga módulo unidad.
     Tupla3f normal=a.cross(b);
     if(normal(X)>0.0 || normal(Y)>0.0 || normal(Z)>0.0)
      normal=normal.normalized();

     normal_tri.push_back(normal);

     //Y ya sólo nos queda calcular las normales de los vértices.
     for(unsigned j=0; j<3; j++){
       normal_ver[triangulos[i](j)]=normal_ver[triangulos[i](j)]+normal;
       if(normal_ver[triangulos[i](j)](X)>0.0 || normal_ver[triangulos[i](j)](Y)>0.0 || normal_ver[triangulos[i](j)](Z)>0.0)
        normal_ver[triangulos[i](j)]=normal_ver[triangulos[i](j)].normalized();
     }
   }

}
//Función auxiliar para la creación de un VBO:
GLuint MallaInd::VBO_Crear(GLuint tipo, GLuint tam, GLvoid * puntero){
  assert(tipo==GL_ARRAY_BUFFER || tipo==GL_ELEMENT_ARRAY_BUFFER);
  GLuint id_vbo; //resultado: identificador del VBO
  glGenBuffers(1, &id_vbo); //crear nuevo VBO, obtener identificador
  glBindBuffer(tipo, id_vbo); //activar VBO usando su identificador
  glBufferData(tipo, tam, puntero, GL_STATIC_DRAW); //transferencia RAM-> GPU
  glBindBuffer(tipo, 0); //desactivar el VBO (activar 0)
  return id_vbo;
}

void MallaInd::crearVBOs(){
  tam_ver=sizeof(float)*3L*num_ver;
  tam_tri=sizeof(unsigned)*3L*num_tri;
  tam_tex=sizeof(float)*2L*coordenadas_textura.size();
  // crear VBO conteniendo la tabla de vértices
  id_vbo_ver=VBO_Crear(GL_ARRAY_BUFFER, tam_ver, vertices.data());
  // crear VBO conteniendo la tabla de tríangulos (índices de vértices)
  id_vbo_tri=VBO_Crear(GL_ELEMENT_ARRAY_BUFFER,tam_tri, triangulos.data());

  if(color_ver.size()>0)
    id_vbo_color_ver=VBO_Crear(GL_ARRAY_BUFFER,tam_ver,color_ver.data());

  if(normal_ver.size()>0)
    id_vbo_normal_ver=VBO_Crear(GL_ARRAY_BUFFER,tam_ver,normal_ver.data());

  if(coordenadas_textura.size()>0)
    id_vbo_textura_ver=VBO_Crear(GL_ARRAY_BUFFER,tam_tex,coordenadas_textura.data());

  vbo_creado=true;
}

void MallaInd::ponerColores(){
  //Creamos vector de colores e inicializamos
  //color_ver.clear();
  //std::cout << "Numero Vertices " << num_ver << std::flush;
  for(unsigned i=0; i<num_ver; i++)
    color_ver.push_back(Tupla3f{1.0,0.0,(float)(i)/(num_ver)});
  //std::cout << "Asigno colores " << i << std::flush;
}

void MallaInd::fijarColorNodo(const Tupla3f & color){
  color_ver.clear();
  for(unsigned i=0; i<num_ver; i++)
    color_ver.push_back(color);

  //std::cout << "FIJADOS COLORES "<<std::endl;
}

//------------------------------------------------------------------------------
void MallaInd::visualizarBE(){
  glBegin(GL_TRIANGLES);
  for(unsigned i=0; i<num_tri; i++)
    for(unsigned j=0; j<3; j++){
      //leer y enviar coordenadas del vértice j del triángulo i
      unsigned indice_ver=triangulos[i](j);
      if(color_ver.size()>0) glColor3fv(color_ver[indice_ver]);
      if(normal_ver.size()>0) glNormal3fv(normal_ver[indice_ver]);
      glVertex3fv(vertices[indice_ver]); //triangulos[i](j) < num_ver

    }
  glEnd();
}

/*void MallaInd::visualizarDE(){

}*/

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
  //Hablitar y especificar normales.
  if(normal_ver.size()>0){
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,normal_ver.data());
  }
  //Habilitar y especificar colores.
  if(color_ver.size()>0){
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3,GL_FLOAT,0,color_ver.data());
  }

  // habilitar uso de un array de vértices
  glEnableClientState(GL_VERTEX_ARRAY);
  // especificar puntero a tabla de coords. de vértices
  glVertexPointer(3,GL_FLOAT,0,vertices.data() );
  // dibujar usando vértices indexados
  // params.: (1) tipo de primitivas (2) número de índices
  // (3) tipo de índices (4) puntero a tabla de triáng.

  glDrawElements(GL_TRIANGLES,3*num_tri,GL_UNSIGNED_INT,triangulos.data()) ;
  // deshabilitar el array de vértices
  glDisableClientState(GL_VERTEX_ARRAY);
  //glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void MallaInd::visualizarDE_NT(){
  glVertexPointer(3,GL_FLOAT,0,vertices.data());
  glTexCoordPointer(2,GL_FLOAT,0,coordenadas_textura.data());
  glNormalPointer(GL_FLOAT,0,normal_ver.data());

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glDrawElements(GL_TRIANGLES,3*num_tri,GL_UNSIGNED_INT,triangulos.data());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv ){
  if(!vbo_creado) crearVBOs();

  if(color_ver.size()>0){
    glBindBuffer(GL_ARRAY_BUFFER,id_vbo_color_ver);
    glColorPointer(3,GL_FLOAT,0,0);
    glEnableClientState(GL_COLOR_ARRAY);
  }
  if(normal_ver.size()>0){
    glBindBuffer(GL_ARRAY_BUFFER,id_vbo_normal_ver);
    glNormalPointer(GL_FLOAT,0,0);
    glEnableClientState(GL_NORMAL_ARRAY);
  }
  glBindBuffer(GL_ARRAY_BUFFER,id_vbo_ver);
  glVertexPointer(3, GL_FLOAT, 0, 0); //formato y offset (=0)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY); //activar uso vertex array

  //visualizar con glDrawElements (puntero a tabla==NULL)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri);
  glDrawElements(GL_TRIANGLES, 3L*num_tri, GL_UNSIGNED_INT, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

}

void MallaInd::visualizarVBOs_NT(){
  //Igual que el otro pero con texturas en lugar de colores.

  if(!vbo_creado) crearVBOs();

  if(coordenadas_textura.size()>0){
    glBindBuffer(GL_ARRAY_BUFFER,id_vbo_textura_ver);
    glTexCoordPointer(2,GL_FLOAT,0,0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if(normal_ver.size()>0){
    glBindBuffer(GL_ARRAY_BUFFER,id_vbo_normal_ver);
    glNormalPointer(GL_FLOAT,0,0);
    glEnableClientState(GL_NORMAL_ARRAY);
  }
  glBindBuffer(GL_ARRAY_BUFFER,id_vbo_ver);
  glVertexPointer(3, GL_FLOAT, 0, 0); //formato y offset (=0)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY); //activar uso vertex array

  //visualizar con glDrawElements (puntero a tabla==NULL)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri);
  glDrawElements(GL_TRIANGLES, 3L*num_tri, GL_UNSIGNED_INT, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
  GLenum mode;
  GLenum sombra;
  unsigned uso=0;
  switch(cv.modoVis) {
    case modoSolido:
      mode = GL_FILL;
      sombra = GL_SMOOTH;
      break;
    case modoPuntos:
      mode = GL_POINT;
      sombra = GL_FLAT;
      break;
    case modoMateriales:
      mode = GL_FILL;
      sombra = GL_SMOOTH;
      uso=2;
      break;
    case modoColorNodoPlano:
      mode = GL_FILL;
      sombra = GL_FLAT;
      uso=2;
      break;
    case modoAlambre:  // modo por defecto
    default:
      mode = GL_LINE;
      sombra=GL_FLAT;
  }

  glPolygonMode(GL_FRONT_AND_BACK, mode);
  glShadeModel(sombra);

  if(color_ver.empty()){
    ponerColores();
    //std::cout << "entro" <<std::flush;
  }
  if(normal_ver.empty()){
    calcular_normales();
  }
  // Visualizar
  if (cv.usarVBOs)
    uso++;

  switch (uso) {
    case 3:
      visualizarVBOs_NT();
      break;
    case 2:
      visualizarDE_NT();
      break;
    case 1:
      visualizarDE_VBOs(cv);
      break;
    case 0:
    default:
      visualizarDE_MI(cv);
  }
}
// *****************************************************************************

// *****************************************************************************
Cubo::Cubo(): Cubo(1.0){}
Cubo::Cubo(float arista): Cubo(Tupla3f{0.0,0.0,0.0}, arista){}
Cubo::Cubo(Tupla3f centro,float arista)
:  MallaInd( "malla cubo" )
{
  ponerCentroOC(centro);
  num_ver=8;
  num_tri=12;
  vertices={leerCentroOC() + arista*Tupla3f{-0.5, -0.5, -0.5}, // 0
            leerCentroOC() + arista*Tupla3f{+0.5, -0.5, -0.5}, // 1
            leerCentroOC() + arista*Tupla3f{+0.5, +0.5, -0.5}, // 2
            leerCentroOC() + arista*Tupla3f{-0.5, +0.5, -0.5}, // 3
            leerCentroOC() + arista*Tupla3f{-0.5, +0.5, +0.5}, // 4
            leerCentroOC() + arista*Tupla3f{-0.5, -0.5, +0.5}, // 5
            leerCentroOC() + arista*Tupla3f{+0.5, -0.5, +0.5}, // 6
            leerCentroOC() + arista*Tupla3f{+0.5, +0.5, +0.5}};// 7

  triangulos={{0,2,1},{0,3,2},{0,4,3},
        {0,5,4},{4,5,6},{4,6,7},
        {6,1,2},{6,2,7},{0,6,5},
        {0,1,6},{3,4,7},{3,7,2}};

}
// *****************************************************************************
Tetraedro::Tetraedro(): Tetraedro(1.0){}
Tetraedro::Tetraedro(float arista)
: Tetraedro(Tupla3f{0.0,0.0,0.0},arista){}
Tetraedro::Tetraedro(Tupla3f centro, float arista)
: MallaInd("malla tetraedro"){
  ponerCentroOC(centro);
  num_ver=4;
  num_tri=4;
  vertices = {leerCentroOC() + arista*Tupla3f{0.5, 0.0, -0.5/sqrt(2)},
              leerCentroOC() + arista*Tupla3f{0.0, 0.5, 0.5/sqrt(2)},
              leerCentroOC() + arista*Tupla3f{-0.5, 0.0, -0.5/sqrt(2)},
              leerCentroOC() + arista*Tupla3f{0.0, -0.5, 0.5/sqrt(2)}};
  triangulos={{0,2,1},{0,1,3},{0,3,2},{1,2,3}};
}
Tetraedro::Tetraedro(Tupla3f centro, std::vector<Tupla3f> vector)
:  MallaInd( "malla tetraedro")
{
  assert(vector.size()>=4);
  ponerCentroOC(centro);
  vertices=vector;
  num_ver=4;
  num_tri=4;
  triangulos={{0,1,2},{0,1,3},{0,2,3},{1,2,3}};
}


// *****************************************************************************
