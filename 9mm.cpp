
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "heuristike.h"
#include "pravila.h"
#include "alphabeta.h"

#define dubinaStabla 8 //easy = 7, noraml = 8, hard = 9 (ako hoces da cekas do sutra)



int main() {
    int tabla[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int igracNaPotezu = -1; // 1 je AI engine, -1 je igrac
    int brPoteza = 1;
    int fazaIgre = 1;
    int dubina = dubinaStabla;
    int koeficijenti[19] = {11, 11, 2, 11, 10, 16, 3, 16, 21, 10, 31, 13, 42, 1, 1086,
                            10, 3, 19, 1200};

    srand(time(NULL));
    int random = std::rand() % 3;

    if (random == 0) {
        tabla[4] = 1;
        std::cout << "4 -1" << std::endl;
    }
    if (random == 1) {
        tabla[10] = 1;
        std::cout << "10 -1" << std::endl;
    }
    if (random == 2) {
        tabla[19] = 1;
        std::cout << "19 -1" << std::endl;
    }
    if (random == 3) {
        tabla[13] = 1;
        std::cout << "13 -1" << std::endl;
    }



   //  * potez[0] je selekcija ili postavljanje figure u fazi 1, potez[1] je pozicija na koju hocemo da
  // * pomerimo figuru koju smo selektovali u vazi 2, potez[2] je indikator 0/1 da li treba gledati
   //  * potez[2], potez[3] je pozicija sa koje nosimo protivnikovu figuru, potez[4] indikator za nosenje

    int potez[3] = {-1, -1, -1};

    int i;

    int indikatorLegalnogPoteza = 101;

    while (1) {

        int rezultat[4] = {0, 0, 0, 0};
        brojBlokiranih(tabla, rezultat);

        if ((brPoteza >= 18) && ((rezultat[2] + rezultat[3]) > 14)) {
            dubina = dubinaStabla + 1;
        }
        else if (brPoteza == 2 || brPoteza == 3 || brPoteza == 4 || brPoteza == 5 || rezultat[2] == 3 || rezultat[3] == 3)
            dubina = dubinaStabla - 1;
        else {
            dubina = dubinaStabla;
        }

        int pobeda;
        if (fazaIgre != 1 && (rezultat[3] == 2 || rezultat[3] == rezultat[1]))
            pobeda = 1;
        else if (fazaIgre != 1 && (rezultat[2] == 2 || rezultat[2] == rezultat[0]))
                pobeda = -1;
        else if (rezultat[3] == 9 && rezultat[3] == rezultat[1])
            pobeda = 1;
        else if (rezultat[2] == 9 && rezultat[2] == rezultat[0])
            return -1;
        else
            pobeda = 0;


        if (pobeda == 1) {
            std::cout << "Na zalost izgubili ste. Kraj igre" << std::endl;
            break;
        }

        if (pobeda == -1) {
            std::cout << "Svaka cast, pobedili ste!!!. Kraj igre" << std::endl;
            break;
        }


        int brojFiguraNiz[2] = {0, 0};

        brojFigura(tabla, brojFiguraNiz);

        if ((brojFiguraNiz[1] == 3 || brojFiguraNiz[0] == 3) && fazaIgre == 2)
            fazaIgre = 3;

        if (brPoteza == 18)
            fazaIgre = 2;

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
                alphabeta(tabla, potez, dubina, brPoteza, koeficijenti);
                std::cout << potez[0] << " " << potez[2] << std::endl;
            }


            if (potez[0] == -1) {
                std::cout << "Bolji si. Predajem patiju" << std::endl;
                return 0;
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


        else if (fazaIgre == 3) {
            std::cout << "Ostalo ti je tri figure pa mozes da skocis bilo gde na tabli" << std::endl;
            for (i = 0; i < 24; i++)
                std::cout << i << ":" << tabla[i] << "  ";
            std::cout << std::endl;

            if (igracNaPotezu == -1) {
                //igrac je na potezu
                do {
                    std::cin >> potez[0];
                    std::cin >> potez[1];
                    if (tabla[potez[0]] == -1 && tabla[potez[1]] == 0)
                        break;
                    else {
                        std::cout << "Morate igrati sa svojom figurom na praznu poziciju bilo gde" << std::endl;
                        continue;
                    }
                } while(1);

                if (zatvorenaMicaDrugaITrecaFaza(tabla, -1, potez)) {
                    do {
                        do {
                            std::cout << "Zatvorili ste micu. Unesite poziciju figure koju hocete da nosite" << std::endl;
                            std::cin >> potez[2];
                            if (potez[2] == 1)
                                break;
                            else
                                std::cout << "Morate uneti poziciju protivnikove figure" << std::endl;
                        } while(1);
                    } while (legalnoNosenje(tabla, igracNaPotezu, potez[2]));

                }
            }
            else {
                //engine je na potezu
                alphabeta(tabla, potez, dubina, brPoteza, koeficijenti);
                std::cout << potez[0] << " " << potez[1] << " " << potez[2] << std::endl;

            }


            if (potez[0] == -1) {
                std::cout << "Bolji si. Predajem patiju" << std::endl;
                return 0;
            }

            brPoteza++;
            tabla[potez[0]] = 0;
            tabla[potez[1]] = igracNaPotezu;

            if (potez[2] != -1) {  //potez[2] je figura pozicija sa koje nosimo
                tabla[potez[2]] = 0;
            }

            potez[0] = -1;
            potez[1] = -1;
            potez[2] = -1;
            igracNaPotezu = -igracNaPotezu;
        }



        else if (fazaIgre == 2) {
            for (i = 0; i < 24; i++)
                std::cout << i << ":" << tabla[i] << "  ";
            std::cout << std::endl;

            if (igracNaPotezu == -1) {
                //igrac je na potezu
                do {
                    std::cin >> potez[0];
                    std::cin >> potez[1];

                    int indikator = 0;
                    int pomNiz[4] = {-101, -101, -101, -101};
                    moguciPoteziDrugaFaza(tabla, potez[0], pomNiz);

                    int k;
                    for (k = 0; k < 4; k++) {
                        if (pomNiz[k] == potez[1]) {
                            indikator = 1;
                            break;
                        }
                    }

                    if (tabla[potez[0]] == -1 && indikator == 1)
                        break;
                    else {
                        std::cout << "Morate igrati sa svojom figurom na praznu poziciju do nje" << std::endl;
                        continue;
                    }
                } while(1);

                if (zatvorenaMicaDrugaITrecaFaza(tabla, -1, potez)) {
                    do {
                        do {
                            std::cout << "Zatvorili ste micu. Unesite poziciju figure koju hocete da nosite" << std::endl;
                            std::cin >> potez[2];
                            if (tabla[potez[2]] == 1)
                                break;
                            else
                                std::cout << "Morate uneti poziciju protivnikove figure" << std::endl;
                        } while(1);
                    } while (legalnoNosenje(tabla, igracNaPotezu, potez[2]));

                }
            }
            else {
                //engine je na potezu
                alphabeta(tabla, potez, dubina, brPoteza, koeficijenti);
                std::cout << potez[0] << " " << potez[1] << " " << potez[2] << std::endl;

            }

            if (potez[0] == -1) {
                std::cout << "Bolji si. Predajem patiju" << std::endl;
                return 0;
            }


            brPoteza++;
            tabla[potez[0]] = 0;
            tabla[potez[1]] = igracNaPotezu;

            if (potez[2] != -1) {  //potez[2] je figura pozicija sa koje nosimo
                tabla[potez[2]] = 0;
            }

            potez[0] = -1;
            potez[1] = -1;
            potez[2] = -1;
            igracNaPotezu = -igracNaPotezu;
        }
    }

    return 0;

}

