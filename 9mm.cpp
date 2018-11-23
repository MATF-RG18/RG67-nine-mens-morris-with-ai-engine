#include <iostream>
#include "heuristike.h"
#include "pravila.h"
#include "alphabeta.h"

int main() {
    int tabla[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int igracNaPotezu = -1; // 1 je AI engine, -1 je igrac
    int brPoteza = 0;
    int fazaIgre = 1;

    /*
     * potez[0] je selekcija ili postavljanje figure u fazi 1, potez[1] je pozicija na koju hocemo da
     * pomerimo figuru koju smo selektovali u vazi 2, potez[2] je indikator 0/1 da li treba gledati
     * potez[2], potez[3] je pozicija sa koje nosimo protivnikovu figuru, potez[4] indikator za nosenje
     */
    int potez[3] = {-1, -1, -1};

    int i;

    int indikatorLegalnogPoteza = 101;

    while (1) {
        if (fazaIgre == 1) {
            for (i = 0; i < 24; i++)
                std::cout << i << ":" << tabla[i] << "  ";
            std::cout << std::endl;

            if (igracNaPotezu == -1) {
                //igrac je na potezu
                std::cin >> potez[0];
                indikatorLegalnogPoteza = legalanPotezPrvaFaza(tabla, igracNaPotezu, potez[0]);

                if (indikatorLegalnogPoteza == 101) {
                    std::cout << "Ovo nije legalan potez za prvu fazu igre. Igrajte ponovo" << std::endl;
                    continue;
                }
                else if (indikatorLegalnogPoteza == 1) {
                    do {
                        while(1) {
                            std::cout << "Unsite figuru koju hocete da odnesete" << std::endl;
                            std::cin >> potez[2];
                            if (! (tabla[potez[2]] == -igracNaPotezu))
                                std::cout << "Morate da kliknete na neku protivnikovu figuru" << std::endl;
                            else
                                break;
                        }
                    }
                    while(legalnoNosenje(tabla, igracNaPotezu, potez[2]));
                }

            }
            else {
                //engine je na potezu
                alphabeta(tabla, potez, 7, brPoteza);
                std::cout << potez[0] << " " << potez[2] << std::endl;
            }

            brPoteza++;
            tabla[potez[0]] = igracNaPotezu;

            if (potez[2] != -1) {  //potez[2] je figura pozicija sa koje nosimo
                tabla[potez[2]] = 0;
            }

            potez[0] = -1;
            potez[1] = -1;
            potez[2] = -1;
            indikatorLegalnogPoteza = 101;
            igracNaPotezu = -igracNaPotezu;
        }
    }

    return 0;

}
