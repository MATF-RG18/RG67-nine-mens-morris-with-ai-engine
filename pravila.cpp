#include "pravila.h"
#include "heuristike.h"


//proverava da li je igrac odigrao legalan potez u prvoj fazi igre
//legalanPotezPrvaFaza vraca 101 ako nije legaln potez
//vraca 1 ako si napravio micu pa sad imas i drugi deo poteza u kome jedes
//vraca 0 ako si samo imao jedan potez i on je ok
int legalanPotezPrvaFaza(int tabla[24], int igracNaPotezu, int potez)  {
    if (potez < 0 || potez > 23 || tabla[potez] != 0)
        return 101;
    else
        return zatvorenaMicaPrvaFaza(tabla, igracNaPotezu, potez);
}



//mora mu se proslediti potez (tj. pozicija) na kojoj se stvarno nalazi protivnikova figura
//vraca 0 ako moze da se odnese ta figura, vraca 1 ako ne moze da se odnese ta figura
int legalnoNosenje(int tabla[24], int igracNaPotezu, int pozicija) {
  int brojac;
  int indikator = 0;

  for (brojac = 0; brojac < 24; brojac++)
    if(tabla[brojac] == -igracNaPotezu && uMici(tabla, brojac)) {
      indikator = 1;
      break;
    }

  if (indikator == 1 && uMici(tabla, pozicija))
    return 1;

  return 0;
  /*
  znaci ako postoji bilo koja koja nije u mici a ta koju ti hoces da ondeses jeste u mici ne moze ta
  ako ta koju hoces nije u mici mozes da nosis
  ako je indikator ostao 0, dakle sve protivnikove figure su u micama, mozes da nosis i tu iako je u mici
  */
}
