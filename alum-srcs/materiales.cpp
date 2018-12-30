// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
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


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
  enviada=false;
  glGenTextures(1,&ident_textura);
  imagen=new jpg::Imagen(nombreArchivoJPG);
  modo_gen_ct=mgct_desactivada;

}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   if(modo_gen_ct==mgct_coords_objeto){
     glTexGenfv(GL_S,GL_OBJECT_PLANE,coefs_s);
     glTexGenfv(GL_T,GL_OBJECT_PLANE,coefs_t);
   }else if(modo_gen_ct==mgct_coords_ojo){
     glTexGenfv(GL_S,GL_EYE_PLANE,coefs_s);
     glTexGenfv(GL_T,GL_EYE_PLANE,coefs_t);
   }
   glBindTexture(GL_TEXTURE_2D,ident_textura);
   gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,imagen->tamX(),imagen->tamY(),GL_RGB,GL_UNSIGNED_BYTE,imagen->leerPixels() );
   enviada=true;

}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{
    if(!enviada) enviar();
   // COMPLETAR: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,ident_textura);
   if(modo_gen_ct==mgct_desactivada){
     glDisable(GL_TEXTURE_GEN_S);
     glDisable(GL_TEXTURE_GEN_T);
   }else{
     glEnable(GL_TEXTURE_GEN_S);
     glEnable(GL_TEXTURE_GEN_T);
     if(modo_gen_ct==mgct_coords_ojo){
       glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
       glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
     }else{
       glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
       glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
     }

   }


}
// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   tra.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   // COMPLETAR: práctica 4: inicializar material usando text,ka,kd,ks,exp
   tex=text;
   iluminacion=true;
   coloresCero();
   //del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente=del.ambiente  = VectorRGB( ka, ka, ka, 1.0);
   tra.difusa=del.difusa    = VectorRGB( kd, kd, kd, 1.0 );
   tra.especular=del.especular = VectorRGB( ks, ks, ks, 1.0 );
   tra.exp_brillo=del.exp_brillo = exp;
   //tra.emision   = VectorRGB(0.0,0.0,0.0,1.0);


   ponerNombre("material con textura e iluminación") ;

 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   // COMPLETAR: práctica 4: inicializar material usando colorAmbDif,ka,kd,ks,exp
   tex=nullptr;
   iluminacion=true;
   coloresCero();

   glColor3fv(colorAmbDif); //Establecemos el color que queremos utilizar a partir de ahora.
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   //Hacemos que el color ambiental y el difuso sean ese color que hemos establecido.

   tra.ambiente=del.ambiente  = VectorRGB( ka, ka, ka, 1.0);
   tra.difusa=del.difusa    = VectorRGB( kd, kd, kd, 1.0 );
   tra.especular=del.especular = VectorRGB( ks, ks, ks, 1.0 );
   tra.exp_brillo=del.exp_brillo = exp;

   ponerNombre("material color plano, ilum.") ;
}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   // COMPLETAR: práctica 4: inicializar material usando un color plano sin iluminación
   tex=nullptr;
   iluminacion=false;
   coloresCero();
   color=Tupla4f{r,g,b,1.0};

   ponerNombre("material con color plano y sin iluminación");

}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
   // COMPLETAR: práctica 4: activar un material
   if(iluminacion){

     //Nótese que en nuestro caso, del=tra siempre pero como lo tenemos separado
     //en dos partes, aquí también lo haremos por si pide separarlo en el examen.
     glMaterialfv(GL_FRONT,GL_EMISSION,del.emision);
     glMaterialfv(GL_FRONT,GL_AMBIENT,del.ambiente);
     glMaterialfv(GL_FRONT,GL_DIFFUSE,del.difusa);
     glMaterialfv(GL_FRONT,GL_SPECULAR,del.especular);
     glMaterialf(GL_FRONT,GL_SHININESS,del.exp_brillo);

     glMaterialfv(GL_BACK,GL_EMISSION,tra.emision);
     glMaterialfv(GL_BACK,GL_AMBIENT,tra.ambiente);
     glMaterialfv(GL_BACK,GL_DIFFUSE,tra.difusa);
     glMaterialfv(GL_BACK,GL_SPECULAR,tra.especular);
     glMaterialf(GL_BACK,GL_SHININESS,tra.exp_brillo);

     glEnable(GL_LIGHTING);

   }else{
     glDisable(GL_LIGHTING);
     glColor4fv(color);
     glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
     glColorMaterial(GL_FRONT_AND_BACK,GL_EMISSION);
     glColorMaterial(GL_FRONT_AND_BACK,GL_SPECULAR);
   }

   if(tex==nullptr){
     glDisable(GL_TEXTURE_2D);
   }else{
     tex->activar();
   }

}

//**********************************************************************

FuenteLuz::FuenteLuz( const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   /*longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi = longi_ini ;
   lati  = lati_ini ;*/

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

/*void FuenteLuz::activar()
{
   // COMPLETAR: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
   if(ind_fuente>=0){
    glEnable(GL_LIGHT0+ind_fuente);

    glLightfv(GL_LIGHT0+ind_fuente,GL_AMBIENT,col_ambiente);
    glLightfv(GL_LIGHT0+ind_fuente,GL_DIFFUSE,col_difuso);
    glLightfv(GL_LIGHT0+ind_fuente,GL_SPECULAR,col_especular);
  }

}*/

//----------------------------------------------------------------------

bool FuenteDireccional::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   //if ( actualizar )
   //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}

void FuenteDireccional::activar()
{
   // COMPLETAR: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
   if(ind_fuente>=0){
     //Habilitamos
    glEnable(GL_LIGHT0+ind_fuente);

    //Colores de ambiente
    glLightfv(GL_LIGHT0+ind_fuente,GL_AMBIENT,col_ambiente);
    glLightfv(GL_LIGHT0+ind_fuente,GL_DIFFUSE,col_difuso);
    glLightfv(GL_LIGHT0+ind_fuente,GL_SPECULAR,col_especular);

    //Establecemos la dirección de la luz utilizando coordenadas polares.
    const Tupla4f ejeZ={0.0,0.0,1.0,0.0};
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glRotatef(longi,0.0,1.0,0.0);
    glRotatef(lati,-1.0,0.0,0.0);

    glLightfv(GL_LIGHT0+ind_fuente,GL_POSITION,ejeZ);
    glPopMatrix();

  }

}

//****************************************************************************

FuenteDireccional::FuenteDireccional(float alpha_inicial,float beta_inicial, const VectorRGB & p_color)
  : FuenteLuz(p_color), longi_ini(alpha_inicial), lati_ini(beta_inicial), longi(alpha_inicial), lati(beta_inicial){}

void FuenteDireccional::variarAngulo(unsigned angulo, float incremento){
  if(angulo==0){
    longi+=incremento;
  }else{
    lati+=incremento;
  }
}

FuentePosicional::FuentePosicional(const Tupla3f & posicion, const VectorRGB & p_color)
  : FuenteLuz(p_color), posicion(posicion){}

void FuentePosicional::activar()
  {
     // COMPLETAR: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
     if(ind_fuente>=0){
       //Habilitamos
      glEnable(GL_LIGHT0+ind_fuente);

      //Colores de ambiente
      glLightfv(GL_LIGHT0+ind_fuente,GL_AMBIENT,col_ambiente);
      glLightfv(GL_LIGHT0+ind_fuente,GL_DIFFUSE,col_difuso);
      glLightfv(GL_LIGHT0+ind_fuente,GL_SPECULAR,col_especular);


      glLightfv(GL_LIGHT0+ind_fuente,GL_POSITION,Tupla4f{posicion(X),posicion(Y),posicion(Z),1.0});
    }

  }

bool FuentePosicional::gestionarEventoTeclaEspecial( int key )
  {
     bool actualizar = true ;
     const float incr = 3.0f ;

     switch( key )
     {
      /*  case GLFW_KEY_RIGHT:
           longi = longi+incr ;
           break ;
        case GLFW_KEY_LEFT:
           longi = longi-incr ;
           break ;
        case GLFW_KEY_UP:
           lati = std::min( lati+incr, 90.0f) ;
           break ;
        case GLFW_KEY_DOWN:
           lati = std::max( lati-incr, -90.0f ) ;
           break ;
        case GLFW_KEY_HOME:
           lati  = lati_ini ;
           longi = longi_ini ;
           break ;*/
        default :
           actualizar = false ;
           cout << "tecla no usable para la fuente de luz." << endl << flush ;
     }

     //if ( actualizar )
     //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
     return actualizar ;
  }

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = 8 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   if(vpf.size()<max_num_fuentes){ //Añadimos esto porque si ya hemos llegado al
     pf->ind_fuente = vpf.size() ; //tope no tiene sentido que sigamos añadiendo.
     vpf.push_back( pf ) ;
   }
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( unsigned id_prog )
{
   // COMPLETAR: práctica 4: activar una colección de fuentes de luz
   if(id_prog==3 || id_prog==4){
     for(unsigned i=0; i<vpf.size();i++){
       vpf[i]->activar();
     }
     for(unsigned i=vpf.size(); i<max_num_fuentes;i++){
       glDisable(GL_LIGHT0+i);
     }
   }else{
     for(unsigned i=0; i<max_num_fuentes;i++){
       glDisable(GL_LIGHT0+i);
     }
   }

}
//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------
ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}

/************************************************************************
**
**
**
************************************************************************/
MaterialLata::MaterialLata()
 : Material(new Textura("../imgs/lata-coke.jpg"),0.0,1,1,1){

}
MaterialTapasLata::MaterialTapasLata()
 : Material(NULL,0.0,1,1,1){
   del.emision=tra.emision=Tupla4f{0.2,0.2,0.2,1.0};
}
MaterialPeonMadera::MaterialPeonMadera()
: Material(new Textura("../imgs/text-madera.jpg"),0.0,1,1,1){
  del.emision=tra.emision=Tupla4f{0.5,0.5,0.5,1.0};
}
MaterialPeonBlanco::MaterialPeonBlanco()
: Material(NULL,0.0,1,0,0){
  del.emision=tra.emision=Tupla4f{1,1,1,1.0};

}
MaterialPeonNegro::MaterialPeonNegro()
: Material(nullptr,0,0.2,1,1) {
  del.emision=tra.emision=Tupla4f{0.0,0.0,0.0,1.0};
}
