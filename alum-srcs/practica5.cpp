// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.hpp"

#include "CamaraInter.hpp"

using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
static unsigned objetoActivo5 = 0 ; // objeto activo: malla ply (0), malla revol (1)
static constexpr int numObjetos5 = 2 ;
static NodoGrafoEscena * objetos5[numObjetos5] = { nullptr,nullptr };
static unsigned camaraActiva=0;
static constexpr int numCamaras = 4;
static CamaraInteractiva * camaras[numCamaras]={nullptr,nullptr,nullptr,nullptr};

// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;
static int arrastrar_x0,arrastrar_y0;


// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   // COMPLETAR: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)
   viewport=Viewport(0,0,vp_ancho,vp_alto);
   //Añadimos algunos de los objetos de anteriores prácticas.

   //Añadimos cámaras interactivas.

   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   // COMPLETAR: práctica 5: actualizar viewport, actualizar y activar la camara actual
   //     (en base a las dimensiones del viewport)
   viewport.ancho=vp_ancho;
   viewport.alto=vp_alto;
   camaras[camaraActiva]->ratio_yx_vp=vp_alto/vp_ancho; //FIXME: ¿QUÉ VALOR PONGO AQUÍ?
   camaras[camaraActiva]->calcularViewfrustum();


}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{

   // COMPLETAR: práctica 5: activar las fuentes de luz y visualizar la escena
   //      (se supone que la camara actual ya está activada)
   // .......

}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{

   bool result = true ;

   switch ( toupper( tecla ) )
   {
      case 'C':
         // COMPLETAR: práctica 5: activar siguiente camara
         camaraActiva=(camaraActiva+1)%numCamaras;
         camaras[camaraActiva]->activar();

         break ;

      case 'V':
         // COMPLETAR: práctica 5: conmutar la cámara actual entre modo examinar y el modo primera persona
        if(camaras[camaraActiva]->examinar)
          camaras[camaraActiva]->modoPrimeraPersona();
        else
          camaras[camaraActiva]->modoExaminar();

         break ;

      case '-':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (positivo) (desplaZ)
         camaras[camaraActiva]->desplaZ(0.1);

         break;

      case '+':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (negativo) (desplaZ)
         camaras[camaraActiva]->desplaZ(-0.1);

         break;

      default:
         result = false ;
         break ;
	}
   return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{

   bool result = true ;


   switch ( tecla )
   {
      case GLFW_KEY_LEFT:
         // COMPLETAR: práctica 5: desplazamiento/rotacion hacia la izquierda (moverHV)
         camaras[camaraActiva]->moverHV(-3.0,0.0);

         break;
      case GLFW_KEY_RIGHT:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia la derecha (moverHV)
         camaras[camaraActiva]->moverHV(+3.0,0.0);

         break;
      case GLFW_KEY_UP:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia arriba (moverHV)
         camaras[camaraActiva]->moverHV(0.0,+3.0);

         break;
      case GLFW_KEY_DOWN:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia abajo (moverHV)
         camaras[camaraActiva]->moverHV(0.0,-3.0);

         break;
      default:
         result = false ;
         break ;
	}

   return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

bool P5_ClickIzquierdo( int x, int y )
{

   // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)

   // 1. crear un 'contextovis' apropiado
   ContextoVis contextovis;
   contextovis.modoVis=modoSolido;
   //contextovis.modoSeleccionRB=true;// true -> dibujando en modo selección usando render buffer de opengl
   contextovis.modoSeleccionFBO=true; // true -> dibujando en modo selección con FBO invisible (puede ser el back-buffer)
   glColor3ub(0,0,0);

   // 2. visualizar en modo selección (sobre el backbuffer)
   P5_DibujarObjetos(contextovis);

   // 3. leer el color del pixel, si es 0 no se hace nada
   int identificador=LeerIdentEnPixel(x,y);
   if(identificador==0){
     std::cout<<"No se ha seleccionado ningún objeto"<<std::endl;
     return false;
   }

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   Objeto3D * objeto;
   Tupla3f centro;
   bool encontrado=objetos5[objetoActivo5]->buscarObjeto(identificador,MATRIZMODADO?,&objeto,centro);
   if(encontrado){
     std::cout <<"El objeto seleccionado es: " <<objeto->leerNombre() << "\nSu identificador es: " << objeto->leerIdentificador()<<std::endl;
     modoExaminar() //TERMINAR
   }

}
void P5_modo_Seleccion(){

}
// --------------------------------------------------------------------
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   // COMPLETAR: práctica 5: activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
   modo_arrastrar=true;
   arrastrar_x0=x;
   arrastrar_y0=y;

}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // COMPLETAR: práctica 5: desactivar bool del modo arrastrar
   modo_arrastrar=false;

}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // COMPLETAR: práctica 5: calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
   camaras[camaraActiva]->moverHV(x-arrastrar_x0,y-arrastrar_y0);


}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction )
{
   // COMPLETAR: práctica 5: acercar/alejar la camara (desplaZ)
   camaras[camaraActiva]->desplaZ(direction);

   return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   const unsigned char
    byteR=( ident           )%0x100U, //rojo=byte menos significativo
    byteG=( ident / 0x100U  )%0x100U, //verde=byte intermedio
    byteB=( ident / 0x10000U)%0x100U; //azul=byte más significativo (el 0)

    glColor3ub(byteR,byteG,byteB);
}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   unsigned char bytes[3];
   glReadPixels(xpix,ypix,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)bytes);

   return bytes[0]+(0x100U*bytes[1])+(0x10000U*bytes[2]);
}
//---------------
