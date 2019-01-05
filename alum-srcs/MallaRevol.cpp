// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************


MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla  )
                        :MallaInd(std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ))
{
  // ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
  vector<float> ver;
  vector<Tupla3f> original;
  ply::read_vertices(nombre_arch.c_str(),ver);
  for(unsigned i=2; i<ver.size();i+=3)
    original.push_back(Tupla3f{ver[i-2],ver[i-1],ver[i]});
  nper=nperfiles;
  nvp=original.size();
  num_ver=nper*nvp;
  crearMallaRevol(original,crear_tapas,cerrar_malla);

  ponerColores();
  // calcular la tabla de normales
  calcular_normales();
  num_tri=triangulos.size();


}

void MallaRevol::crearMallaRevol(const vector<Tupla3f> & original,
                const bool     crear_tapas,
                const bool     cerrar_malla)
{
  // Calculamos el ángulo de rotación y almacenamos
  float alpha = cerrar_malla ? (360.0 / (float)nper) : (360.0 / (float)(nper-1));
  Matriz4f rotacion = MAT_Rotacion(alpha, 0,1.0,0);
  std::vector<Tupla3f> aux=original;

  //Guardamos el perfil original
  vertices.insert(vertices.end(),aux.begin(),aux.end());

  //Creamos los perfiles originales y los unimos para formar las caras.
  for(unsigned i=0; i<nper-1; i++){
    //Rotamos y añadimos un nuevo perfil.
    aux=rotarPerfil(aux,rotacion);
    vertices.insert(vertices.end(),aux.begin(),aux.end());

    //Añadir caras
    for(unsigned j=0; j<nvp-1; j++){
      int vert1 = i*nvp + j,
          vert2 = vert1+1,
          vert3 = (i+1)*nvp + j,
          vert4 = vert3+1;

      triangulos.push_back({vert3,vert4,vert2});
      triangulos.push_back({vert3,vert2,vert1});
    }
  }

  //En caso de que haya que cerrar la malla, lo uno.
  if(cerrar_malla){
    for(unsigned j=0; j<nvp-1;j++){
      int vert1 = j, //Pertenece al primer perfil
          vert2 = vert1+1,
          vert3 = (nper-1)*nvp + j, //Pertenece al último perfil
          vert4 = vert3+1;

      triangulos.push_back({vert3,vert2,vert4});
      triangulos.push_back({vert3,vert1,vert2});
    }
  }

  if(crear_tapas){
    //cout << endl << "Cerrar Malla " << cerrar_malla <<endl
      //  << "crear_tapas " << crear_tapas <<endl;
    //Primera tapa
    vertices.push_back({0.0,vertices[0](Y),0.0});
    num_ver++;
    for(unsigned i=0; i + nvp < num_ver; i+=nvp)
      triangulos.push_back({num_ver-1,(i+nvp)%(num_ver-1),i});


    //Segunda tapa
    vertices.push_back({0.0,vertices[num_ver-2](Y),0.0});
    num_ver++;
    for(unsigned i=nvp-1; i + nvp - 3 < num_ver; i+=nvp)
      triangulos.push_back({num_ver-1,i,i+nvp});


      // Última cara
    triangulos.push_back({nper*nvp-1, nvp - 1,num_ver - 1});
  }



}
// *****************************************************************************

void MallaRevol::crearTablaTextura(){
  vector<float> distancia;
  distancia.push_back(0.0);
  for(unsigned j=0; j<nvp-1; j++)
    distancia.push_back(distancia[j]+sqrt((vertices[j+1]-vertices[j]).lengthSq()));

  for(unsigned i=0; i<nper;i++){
    for(unsigned j=0; j<nvp;j++){
      coordenadas_textura.push_back(Tupla2f{(float)i/(float)(nper-1),1-distancia[j]/distancia[nvp-1]});
    }
  }
  std::cout<<"Creada Tabla Textura"<<std::endl;

}

// *****************************************************************************


Cilindro::Cilindro(const unsigned num_verts_per, const unsigned nperfiles,
  float base, float altura, const bool crear_tapas, const bool cerrar_malla)
    :MallaRevol(std::string("malla por revolución de un cilindro" )) {

  r=base;
  h=altura;
  vector<Tupla3f> original;

  for(unsigned i=0; i<num_verts_per;i++)
    original.push_back(Tupla3f{r,h*i/num_verts_per,0.0});
  nper=nperfiles;
  nvp=num_verts_per; //original.size()
  num_ver=nper*nvp;
  crearMallaRevol(original,crear_tapas,cerrar_malla);

  ponerColores();
  // calcular la tabla de normales
  //calcular_normales();
  num_tri=triangulos.size();

}




Cono::Cono(const unsigned num_verts_per, const unsigned nperfiles,
  float base, float altura, const bool crear_tapas, const bool cerrar_malla)
    :MallaRevol(std::string("malla por revolución de un cono" )) {

  r=base;
  h=altura;
  vector<Tupla3f> original;

  for(unsigned i=0; i<num_verts_per;i++)
    original.push_back(Tupla3f{r*i/num_verts_per,h*i/num_verts_per,0.0});
  nper=nperfiles;
  nvp=num_verts_per; //original.size()
  num_ver=nper*nvp;
  crearMallaRevol(original,crear_tapas,cerrar_malla);

  ponerColores();
  // calcular la tabla de normales
  //calcular_normales();
  num_tri=triangulos.size();
}

Esfera::Esfera(const unsigned num_verts_per, const unsigned nperfiles,
  float radio, const bool crear_tapas, const bool cerrar_malla)
    :MallaRevol(std::string("malla por revolución de una esfera" )) {

  r=radio;
  vector<Tupla3f> original;

  for(unsigned i=0; i<num_verts_per;i++){
    float y = r * (-1 + (float) 2 * i / (num_verts_per - 1));
    //float y=r*i/num_verts_per;
    original.push_back(Tupla3f{sqrt(r*r-y*y),y,0.0});
  }
  nper=nperfiles;
  nvp=num_verts_per; //original.size()
  num_ver=nper*nvp;
  crearMallaRevol(original,crear_tapas,cerrar_malla);

  ponerColores();
  // calcular la tabla de normales
  //calcular_normales();
num_tri=triangulos.size();

}

Toro::Toro(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      float radio1,             // radio de la base
      float radio2,           // altura de nuestro cilindro
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    ):MallaRevol("Malla por revolución de un Toro"){
      r1=radio1;
      r2=radio2;

      vector<Tupla3f> original;
      Tupla3f tupla{radio1,0.0,0.0};
      float alpha = 360.0 / 10;
      for(unsigned i=0; i<10;i++){
        original.push_back(MAT_Traslacion(0.0,0.0,r1)*tupla);
        tupla=alpha*tupla;
      }

      nper=nperfiles;
      nvp=num_verts_per; //original.size()
      num_ver=nper*nvp;
      crearMallaRevol(original,crear_tapas,cerrar_malla);

      ponerColores();
      // calcular la tabla de normales
      //calcular_normales();
      num_tri=triangulos.size();
    }
