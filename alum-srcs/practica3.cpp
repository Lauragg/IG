// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"



using namespace std ;

static unsigned objetoActivo3 = 0 ; // objeto activo: malla ply (0), malla revol (1)
static constexpr int numObjetos3 = 1 ;
static Bailarin * objetos3[numObjetos3] = { nullptr };
static unsigned p3_grado_libertad_activo=0;
static bool p3_animaciones_activas=false;



// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;
   objetos3[0]=new Bailarin;
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
        objetoActivo3 = (objetoActivo3+1) % numObjetos3 ;
        cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3 ;
        if ( objetos3[objetoActivo3] != nullptr )
           cout << " (" << (objetos3[objetoActivo3]->leerNombre()) << ")." << endl ;
        else
           cout << " (objeto no creado)" << endl ;

         res=true;
         break ;

      case 'A' :
         p3_animaciones_activas= !p3_animaciones_activas;

         if(p3_animaciones_activas)
          FijarFuncDesocupado(FGE_Desocupado);
         cout << "Las animaciones están: "
         << (p3_animaciones_activas ? "activadas" : "desactivadas") << endl;

         res=true;
         break ;

      case 'G' :
        if (objetos3[objetoActivo3] != nullptr) {
          p3_grado_libertad_activo = (p3_grado_libertad_activo + 1) %
          objetos3[objetoActivo3]->numParametros();
          res=true;
          cout << "Nuevo grado libertad activado:" << p3_grado_libertad_activo<<endl;
        }
        break ;

      case 'R' :
        p3_animaciones_activas = false;
        if (objetos3[objetoActivo3] != nullptr)
          for (unsigned i = 0; i < objetos3[objetoActivo3]->numParametros(); i++)
            objetos3[objetoActivo3]->leerPtrParametro(i)->reset();
         res=true;
         break ;

      case '>' :
         if(objetos3[objetoActivo3]!=nullptr){
           if(p3_animaciones_activas){
             (objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo))->acelerar();
           }else{
             (objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo))->incrementar();
           }
           res=true;
         }

         break ;

      case '<' :
        if(objetos3[objetoActivo3]!=nullptr){
          if(p3_animaciones_activas){
            objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo)->decelerar();
          }else{
            objetos3[objetoActivo3]->leerPtrParametro(p3_grado_libertad_activo)->decrementar();
          }
          res=true;
        }
        break ;

      case '1':
        if(objetos3[objetoActivo3]!=nullptr && !p3_animaciones_activas){
          objetos3[objetoActivo3]->levantarBrazos();
        res=true;
        }
        break;

      case '2':
        if(objetos3[objetoActivo3]!=nullptr && !p3_animaciones_activas){
          objetos3[objetoActivo3]->girarse();
        res=true;
        }
        break;
      case '3':
        if(objetos3[objetoActivo3]!=nullptr && !p3_animaciones_activas){
          objetos3[objetoActivo3]->agacharse();
        res=true;
        }
        break;

      case '8':
        if(objetos3[objetoActivo3]!=nullptr){
         for(unsigned i=0; i<objetos3[objetoActivo3]->numParametros();i++)
          if(p3_animaciones_activas){
            objetos3[objetoActivo3]->leerPtrParametro(i)->decelerar();
          }else{
            objetos3[objetoActivo3]->leerPtrParametro(i)->decrementar();
          }
          res=true;
        }
        break ;
      case '9':
        if(objetos3[objetoActivo3]!=nullptr){
         for(unsigned i=0; i<objetos3[objetoActivo3]->numParametros();i++)
          if(p3_animaciones_activas){
            (objetos3[objetoActivo3]->leerPtrParametro(i))->acelerar();
          }else{
            (objetos3[objetoActivo3]->leerPtrParametro(i))->incrementar();
          }
          res=true;
        }

        break ;
      default :
         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
  objetos3[objetoActivo3]->visualizarGL(cv);


}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
  // no hacer nada si no es necesario, y desactivar
  if(!p3_animaciones_activas)
    return false;

  //modificar los parámetros animables según sus velocidades actuales
  if(objetos3[objetoActivo3]!=nullptr)
    objetos3[objetoActivo3]->siguienteCuadro();

  //forzar llamada a VisualizarFrame en la próxima interacción del bucle
  redibujar_ventana=true;

  //terminar, manteniendo activa la gestión del evento
  return true;
}
