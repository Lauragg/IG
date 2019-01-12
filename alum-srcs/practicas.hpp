// *********************************************************************
// **
// ** Informática Gráfica
// ** Declaraciones públicas auxiliares, comunes a todas las prácticas
// **
// *********************************************************************

#ifndef IG_PRACTICAS_HPP
#define IG_PRACTICAS_HPP

#include <string>
#include "materiales.hpp"
#include "cauce.hpp"
#include "Parametro.hpp"

// --------------------------------------------------------------------
// declaraciones adelantadas de clases (útiles para punteros)

class ColFuentesLuz ;


// ---------------------------------------------------------------------
// tipo de datos enumerado para los modos de visualización:

typedef enum
{
   modoPuntos,
   modoAlambre,
   modoSolido,

   modoColorNodoPlano,
   //modoColorNodoSuave, //NUEVO
   modoMateriales,
   //modoSeleccion,
   numModosVis
}
   ModosVis;


const std::string nombreModo[numModosVis] =
   {
      "modoPuntos",
      "modoAlambre",
      "modoSolido",

      "modoColorNodoPlano",
      //"modoColorNodoSuave", //NUEVO
      "modoMateriales"
    //  "modoSeleccion"
   } ;

// --------------------------------------------------------------------
// contexto de visualización

class ContextoVis
{
   public:

   ModosVis       modoVis        ;   // modo de visualización actual
   bool           dibuAlambreEsc ;   // dibujar alambre escalado en modos solido/ajedrez
   bool           usarVBOs  ;        // true-> usar modo diferido (VBOS), false->usar modo inmediato
   bool           usarShader ;       // true -> usar shader prog, false no usar shader prog
   bool           modoSeleccionFBO ; // true -> dibujando en modo selección con FBO invisible (puede ser el back-buffer)
   bool           modoSeleccionRB ;  // true -> dibujando en modo selección usando render buffer de opengl
   int            identAct ;         // identificador actual en modo seleccion (nunca -1, >=0), inicialmente 0 antes de raiz
   PilaMateriales pilaMateriales ;   // pila de materiales
   ColFuentesLuz * colFuentes ;      // colección de fuentes de luz activa

   ContextoVis()
   {
      modoVis          = modoAlambre ;  // poner alambre por defecto
      dibuAlambreEsc   = true ;
      usarVBOs         = false ;
      usarShader       = false ;
      modoSeleccionRB  = false ;
      modoSeleccionFBO = false ;
      colFuentes       = nullptr ;
   }

};
// exportar 'redibujar_ventana' para que sea visible desde las prácticas (??)
extern bool redibujar_ventana ;

// funciones que deben ser llamadas desde las prácticas
void FijarFuncDesocupado( void (*func_desocupado_nueva)(void) ) ;
void FGE_Redibujado();
void FGE_Desocupado() ;
void FijarColorIdent( const int ident ) ; // 0 ≤ ident < 2
int  LeerIdentEnPixel( int xpix, int ypix );

/*class BufferSeleccion{
  private:
    BufferSeleccion(){}
    BufferSeleccion(BufferSeleccion const & copy);
    BufferSeleccion & operator=(BufferSeleccion const & copy);

  public:
    static BufferSeleccion & getInstance(){
      static BufferSeleccion instance;
      return instance;
    }

    static const GLsizei tamB=1024; //tamaño del buffer (==4 Kb p. ej.)
    GLuint vec[tamB]; //vector de entradas enteras (buffer)

    //variables calculadas en finModoSel:
    int numNombres; //número de nombres (bloques) en el buffer
    GLuint * nombreMin; //puntero al nombre más cercano (dentro de vec)
    int longNombreMin; //longitud de nombreMin

    //métodos:
    //BufferSeleccion(){} //Vamos a ver. En las diapos pone que es Singleton y luego me declaras esto público. Aclárate.
    void inicioModoSel(){
      glSelectBuffer(tamB,vec); //decirle a OpenGL donde está el buffer de selección.
      glRenderMode(GL_SELECT); // entrar en modo seleccion
      glInitNames();          // vacía la pila de nombres
    }
    void finModoSel(){ // fin del modo selección
      int profZmin; //mínima profundidad hasta el momento
      GLuint * bloque=vec; //puntero a la base del bloque actual del buffer
      numNombres = glRenderMode(GL_RENDER); //lee núm. de bloques, volver modo render
      for(unsigned i=0; i< numNombres; i++){
        if(i==0 || bloque[1] < profZmin){ //si la profundidad es menor
          profZmin=bloque[1]; //guardar nueva prof. mínima
          longNombreMin=bloque[0]; //registar longitud del nombre
          nombreMin=&(bloque[3]); //registrar puntero a nombre mínimo
        }
        bloque+=bloque[0]+3; //avanzar puntero a la base del bloque siguiente
      }

    }
};*/





#endif
