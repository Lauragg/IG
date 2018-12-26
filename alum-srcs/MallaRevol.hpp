// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
  protected:

   unsigned
      nper , // numero de perfiles
      nvp  ; // numero de vertices por perfil

    void crearMallaRevol(const std::vector<Tupla3f> & original,
                    const bool     crear_tapas,
                    const bool     cerrar_malla);

    std::vector<Tupla3f> rotarPerfil(const std::vector<Tupla3f> & perfil, const Matriz4f & rotacion){
      std::vector<Tupla3f> rotado;
      for(unsigned i=0; i<perfil.size(); i++){
        rotado.push_back(rotacion*perfil[i]);
      }
      return rotado;
    }

    MallaRevol(const std::string & nombreIni) : MallaInd( nombreIni ){}

    void crearTablaTextura();


   public:
      // crea una malla de revolucion
      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla ) ;

} ;

class Toro : public MallaRevol{
  protected:
    float r1,r2;

  public:
    Toro(
          const unsigned num_verts_per, //numero de vértices del perfil orignal main
          const unsigned nperfiles, // número de perfiles N
          float radio1,             // radio de la base
          float radio2,           // altura de nuestro cilindro
          const bool crear_tapas, //true para crear crear_tapas
          const bool cerrar_malla //true para cerrar la malla
        );
};

class Cilindro : public MallaRevol{
  protected:
    float r, h; //radio y altura

  public:

    Cilindro(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    ) : Cilindro(num_verts_per, nperfiles, 1.0,1.0,crear_tapas,cerrar_malla){}

    Cilindro(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      float base,             // radio de la base
      float altura,           // altura de nuestro cilindro
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    );
};

class Cono : public MallaRevol{
  protected:
    float r,h; //radio y altura

  public :

    Cono(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    ) : Cono(num_verts_per, nperfiles, 1.0,1.0,crear_tapas,cerrar_malla){};

    Cono(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      float base,             // radio de la base
      float altura,           // altura de nuestro cono
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    );

};

class Esfera : public MallaRevol{
  protected:
    float r; //radio de nuestra esfera

  public:

    Esfera(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    ) : Esfera(num_verts_per, nperfiles,1.0,crear_tapas,cerrar_malla){}
    Esfera(
      const unsigned num_verts_per, //numero de vértices del perfil orignal main
      const unsigned nperfiles, // número de perfiles N
      float radio,            // radio de nuestra esfera
      const bool crear_tapas, //true para crear crear_tapas
      const bool cerrar_malla //true para cerrar la malla
    );
};

#endif
