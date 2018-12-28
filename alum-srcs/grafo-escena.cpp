// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"
#include <typeinfo>

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
  cv.pilaMateriales.push();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  for (unsigned i = 0; i < entradas.size(); i++) {
    if (entradas[i].tipo == TipoEntNGE::objeto) {
      entradas[i].objeto->visualizarGL(cv);
    }
    else if (entradas[i].tipo == TipoEntNGE::transformacion) {
      glMatrixMode(GL_MODELVIEW);
      glMultMatrixf(*(entradas[i].matriz));
    }else if (entradas[i].tipo == TipoEntNGE::material){
      cv.pilaMateriales.activarMaterial(entradas[i].material);
    }
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  cv.pilaMateriales.pop();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
   // COMPLETAR: práctica 3: inicializar un nodo vacío (sin entradas)
   // ........

}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
   for(unsigned i=0;i<entradas.size();i++){
     //cout<< entradas.size() <<endl;
    if (entradas[i].tipo == TipoEntNGE::objeto){
     entradas[i].objeto->fijarColorNodo(nuevo_color+Tupla3f{0.1,0.1,0.1});
      //  cout << " Fijar color Nodo Bucle " << i<<endl;
    }
   }


}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
  entradas.push_back(entrada);
  return entradas.size()-1;

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   if(indice >= 0 && indice < entradas.size()){
     if(entradas[indice].tipo==TipoEntNGE::transformacion && entradas[indice].matriz!=nullptr)
      return entradas[indice].matriz;
   }

   cout << "ERROR: no hay un puntero válido a una matriz en : "<< indice << endl;

   exit(EXIT_FAILURE);

}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........

}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
   return parametros.size();

}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned indice )
{
   if(indice >= 0 && indice < parametros.size()){
      return &parametros[indice];
   }

   cout << "ERROR: no hay un puntero válido a un parametro en : "<< indice << endl;

   exit(EXIT_FAILURE);

}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
  for(unsigned i=0; i<parametros.size(); i++)
      parametros[i].siguiente_cuadro();


}

Bailarin::Cuerpo::Cuerpo(){
  agregar(MAT_Escalado(4.0,8.0,2.0));
  agregar(new Cubo());
}
Bailarin::Extremidad::MediaExtremidad::MediaExtremidad(){
  agregar(new Esfera(10,10,false,true));
  agregar(new Cilindro(10,10,1.0,3.0,true,true));



}

Bailarin::Extremidad::Extremidad(Tupla3f desp){
  agregar(MAT_Traslacion(desp));
  sup=agregar(MAT_Ident());
  agregar(new MediaExtremidad());
  agregar(MAT_Traslacion(0.0,3.0,0.0));
  inf=agregar(MAT_Ident());
  agregar(new MediaExtremidad());

}

Bailarin::ParExt::ParExt(Tupla3f desp_ambos, Tupla3f desp_izq, Tupla3f desp_der){
  izq=new Extremidad(desp_izq);
  der=new Extremidad(desp_der);
  agregar(MAT_Traslacion(desp_ambos));
  agregar(izq);
  agregar(der);
}

Bailarin::Cabeza::Cabeza(){
  agregar(MAT_Traslacion(0.0,4.0,0.0));
  agregar(new Cilindro(10,10,0.5,1.0,false,false));
  agregar(MAT_Traslacion(0.0,1.5,0.0));
  agregar(new Esfera(10,10,false,false));
}

Matriz4f brazos(float v){
//  static unsigned contador_brazos=0;
  //contador_brazos++;
  //if(contador_brazos)
  return MAT_Rotacion(v,1.0,0.0,0.0);
}
Matriz4f antebrazos(float v){
  return MAT_Rotacion(v,-1.0,0.0,0.0);
}

Bailarin::Bailarin(){
  ParExt * inf=new ParExt(Tupla3f{0.0,3.0,0.0},Tupla3f{-2.5,0.0,0.0},Tupla3f{2.5,0.0,0.0});
  ParExt * sup=new ParExt(Tupla3f{0.0,3.0,0.0},Tupla3f{-2.5,0.0,0.0},Tupla3f{2.5,0.0,0.0});
  unsigned vertical=agregar(MAT_Ident());
  unsigned horizontal=agregar(MAT_Ident());
  agregar(new Cuerpo());
  agregar(new Cabeza());
  agregar(sup);
  agregar(MAT_Rotacion(180.0,1.0,0.0,0.0));
  agregar(inf);

  Parametro p1("Subir y bajar brazos.",sup->mat_izq_sup(),
                brazos,true, 90.0, 70, 0.06);
  parametros.push_back(p1);
  Parametro p2("Subir y bajar antebrazos.",sup->mat_izq_inf(),
                antebrazos,true, 50.0, 50, 0.06);
  parametros.push_back(p2);
  Parametro p3("Subir y bajar brazos.",sup->mat_der_sup(),
                brazos,true, 90.0, 70, 0.06);
  parametros.push_back(p3);
  Parametro p4("Subir y bajar antebrazos.",sup->mat_der_inf(),
                antebrazos,true, 50.0, 50, 0.06);
  parametros.push_back(p4);


  Parametro p5("Giro cuerpo.",leerPtrMatriz(horizontal),
                [=](float v){return MAT_Rotacion(v,0.0,1.0,0.0);},
                true, 0.0, 70, 0.03);
  parametros.push_back(p5);
  Parametro p6("Levantar cuerpo.",leerPtrMatriz(vertical),
                [=](float v){return MAT_Traslacion(0.0,v,0.0);},
                true, 0.0, 2, 0.12);
  parametros.push_back(p6);


  Parametro p7("Subir y bajar piernas.",inf->mat_izq_sup(),
              [=](float v){return MAT_Rotacion(v,1.0,0.0,0.0);},
              true, -40.0, 40, 0.12);
  parametros.push_back(p7);
  Parametro p8("Subir y bajar desde rodillas.",inf->mat_izq_inf(),
                [=](float v){return MAT_Rotacion(v,-1.0,0.0,0.0);},
                true, -50.0, 50, 0.12);
  parametros.push_back(p8);
  Parametro p9("Subir y bajar piernas.",inf->mat_der_sup(),
                [=](float v){return MAT_Rotacion(v,1.0,0.0,0.0);},
                true, -40.0, 40, 0.12);
  parametros.push_back(p9);
  Parametro p10("Subir y bajar desde rodillas.",inf->mat_der_inf(),
              [=](float v){return MAT_Rotacion(v,-1.0,0.0,0.0);},
                true, -50.0, 50, 0.12);
  parametros.push_back(p10);

  fijarColorNodo(Tupla3f{0.1,0.1,0.1});

}

void Bailarin::levantarBrazos(){
  for(unsigned i=0; i<4; i++)
    parametros[i].siguiente_cuadro();
}

void Bailarin::girarse(){
  parametros[4].siguiente_cuadro();
}

void Bailarin::agacharse(){
  for(unsigned i=5; i<numParametros();i++)
    parametros[i].siguiente_cuadro();
}

Peones::PeonMadera::PeonMadera(){
  agregar(MAT_Traslacion(Tupla3f{-5.0,0.0,0.0}));
  agregar(new MaterialPeonMadera());
  agregar(new MallaRevol("../plys/peon.ply",10,true,true,true));
}
Peones::PeonBlanco::PeonBlanco(){
  agregar(MAT_Traslacion(Tupla3f{0.0,0.0,0.0}));
  agregar(new MaterialPeonBlanco());
  agregar(new MallaRevol("../plys/peon.ply",10,true,true,true));
}
Peones::PeonNegro::PeonNegro(){
  agregar(MAT_Traslacion(Tupla3f{5.0,0.0,0.0}));
  agregar(new MaterialPeonNegro);
  agregar(new MallaRevol("../plys/peon.ply",10,true,true,true));
}
Peones::Peones(){
  agregar(new PeonMadera());
  agregar(new PeonBlanco());
  agregar(new PeonNegro());

}
