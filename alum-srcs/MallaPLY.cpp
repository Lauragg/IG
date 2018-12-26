// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
: MallaInd(string("malla leída del archivo '") + nombre_arch + "'" ){
  //ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );
  vector<float> ver;
  vector<int> tri;

  ply::read(nombre_arch.c_str(),ver,tri);
  for(unsigned i=2; i<ver.size(); i+=3)
    vertices.push_back(Tupla3f{ver[i-2],ver[i-1],ver[i]});
  for(unsigned i=2; i<tri.size(); i+=3)
    triangulos.push_back(Tupla3i{tri[i-2],tri[i-1],tri[i]});
   // calcular la tabla de normales

   num_ver=vertices.size();
   num_tri=triangulos.size();
 calcular_normales();

}

// *****************************************************************************
