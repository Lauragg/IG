// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"


using namespace std ;

// COMPLETAR: práctica 4: declaración de variables de la práctica 4 (static)
static unsigned objetoActivo4 = 0 ; // objeto activo: malla ply (0), malla revol (1)
static constexpr int numObjetos4 = 2 ;
static NodoGrafoEscena * objetos4[numObjetos4] = { nullptr,nullptr };
static ColFuentesLuz fuentes4;
static unsigned angulo4=0; //0 alpha, 1 beta


// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   // COMPLETAR: práctica 4: inicializar objetos de la práctica 4
   fuentes4.insertar(new FuenteDireccional(-10,30,Tupla4f{0.5,0.4,0,0.5}));
   fuentes4.insertar(new FuentePosicional(Tupla3f{-7.5,0,0},Tupla4f{0.1,0.4,0.7,0.5}));
   objetos4[0]=new Peones();
   objetos4[1]=new CocaCola();
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto: es true solo cuando se ha procesado alguna tecla

   switch ( toupper( tecla ) )
   {
      case 'G' :
         // COMPLETAR: práctica 4: activar el siguiente ángulo (longitud o latitud)
         angulo4=(angulo4++)%2;
         res=true;
         break ;

      case '>' :
         // COMPLETAR: práctica 4: incrementar el ángulo activo
         for(unsigned i=0; i<fuentes4.numFuentes();i++){
           fuentes4.ptrFuente(i)->variarAngulo(angulo4,15);
         }
         res=true;

         break ;

      case '<' :
         // COMPLETAR: práctica 4: decrementar el ángulo activo
         for(unsigned i=0; i<fuentes4.numFuentes();i++){
           fuentes4.ptrFuente(i)->variarAngulo(angulo4,-15);
         }
         res=true;

         break ;

      case 'O' :
        objetoActivo4 = (objetoActivo4+1) % numObjetos4 ;
        cout << "práctica 4: nuevo objeto activo es: " << objetoActivo4 ;
        if ( objetos4[objetoActivo4] != nullptr )
           cout << " (" << objetos4[objetoActivo4]->leerNombre() << ")." << endl ;
        else
           cout << " (objeto no creado)" << endl ;

        res=true;
        break;

      default :
         break ;
   }
   return res ;

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
  glEnable(GL_LIGHTING);
  objetos4[objetoActivo4]->visualizarGL(cv);
  fuentes4.activar(cv.modoVis);
  glDisable(GL_LIGHTING);

}
