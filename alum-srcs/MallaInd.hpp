// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      // COMPLETAR: incluir aquí las variables y métodos privados que sean
      // necesarios para una malla indexada
      // ......
      std::vector<Tupla3f> vertices; //tabla de vértices (num_ver entradas)
      std::vector<Tupla3i> triangulos; //tabla de triángulos (num_tri entradas)
      unsigned num_ver; //número de vértices
      unsigned num_tri; //número de triángulos

      GLuint id_vbo_ver; //identificador del VBO con la tabla de vértices
      GLuint id_vbo_tri; //identificador del VBO con la tabla de triángulos

      unsigned tam_ver; //tamaño en bytes de la tabla de vértices
      unsigned tam_tri; //tamaño en bytes de la tabla de triángulos

      bool vbo_creado;

      GLuint id_vbo_color_ver;
      GLuint id_vbo_normal_ver;

      std::vector<Tupla3f> color_ver;
      //std::vector<Tupla3f> color_tri;
      std::vector<Tupla3f> normal_ver;
      std::vector<Tupla3f> normal_tri;

      std::vector<Tupla2f> coordenadas_textura; //tamaño normal_ver.size()

      //Función auxiliar para la creación de un VBO:
      GLuint VBO_Crear(GLuint tipo, GLuint tam, GLvoid * puntero);

      void crearVBOs();

      void ponerColores();

      void fijarColorNodo(const Tupla3f & color);


      // calculo de las normales de esta malla
      void calcular_normales();

      //visualizar con Begin/End usando glVertex (neutro?)
      void visualizarBE();
      //visualizar con Draw Elementes neutro
      //void visualizarDE();
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
      Cubo(float arista);
      Cubo(Tupla3f centro, float arista);
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      Tetraedro();
      Tetraedro(float arista);
      Tetraedro(Tupla3f centro, float arista);
      Tetraedro(Tupla3f centro, std::vector<Tupla3f> vector);
};


#endif
