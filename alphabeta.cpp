#include "heuristike.h"
#include "pravila.h"
#include "alphabeta.h"

#include <iostream>


static int global = 0;


//slicno kao za max
int min(int tabla[24], int alpha, int beta, int poslednjiPotezPozitivnog, int poslednjiPotezNegativnog,
        int dubina, int brojPoteza) {
    global++;
    int pobeda;
    int rezultat[4] = {0, 0, 0, 0};
    brojBlokiranih(tabla, rezultat);

    if (rezultat[1] == 2 || rezultat[1] == rezultat[3])
        pobeda = 1;
    else
        if (rezultat[0] == 2 || rezultat[0] == rezultat[2])
            pobeda = -1;
        else
            pobeda = 0;

    if (brojPoteza < 18) {
        //jos uvek je prva faza igre

        if (dubina == 0) {
            //izlaz iz rekurzije
            int dvaITriKonf[2] = {0, 0};
            brojDvaITriKonfiguracija(tabla, dvaITriKonf);

            //sto je negativni u boljoj poziciji to je vrednost povratne vrednosti manja
            //zbog toga oduzimamo vrednost ako je upravo zatvorio micu u poslednjem potezu
            return (11 * (poslednjiPotezNegativnog + poslednjiPotezPozitivnog)) + (8 * brojFiguraUMicamaPrvaFaza(tabla)) +
                   (1 * rezultat[0] + rezultat[1]) + (11 * rezultat[2] + rezultat[3]) +
                   (10 * dvaITriKonf[0]) + (6 * dvaITriKonf[1]);
        }

        //jos uvek ne izlazimo iz rekurzije vec gledamo koji je najbolji potez za negativnog
        int minimalnaVrednost = 16777216;
        int vrednost = 0;
        int brojac;

        for (brojac = 0; brojac < 24; brojac++) {
            if (tabla[brojac] == 0) {
                tabla[brojac] = -1; //posto smo u min-u znamo da je negativni na potezu

                if (uMici(tabla, brojac)) {
                    poslednjiPotezNegativnog = 1;
                    int j;

                    for(j = 0; j < 24; j++) {
                        if (tabla[j] == 1 && !legalnoNosenje(tabla, -1, j)) {
                            tabla[j] = 0; //skidamo protivnikovu figuru
                            vrednost = max(tabla, alpha, beta, nullptr, poslednjiPotezPozitivnog,
                                           poslednjiPotezNegativnog, dubina - 1, brojPoteza + 1);
                            tabla[j] = 1; //vracamo kako je bilo posle evaluacije tog poteza
                            if (vrednost < minimalnaVrednost)
                                minimalnaVrednost = vrednost;
                            if (minimalnaVrednost <= alpha) {
                                tabla[brojac] = 0;
                                return minimalnaVrednost;
                            }
                            if(minimalnaVrednost < beta)
                                beta = minimalnaVrednost;
                        }
                    }

                }
                else {
                    //odigrani potez nije napravio micu
                    poslednjiPotezNegativnog = 0;
                    vrednost = max(tabla, alpha, beta, nullptr, poslednjiPotezPozitivnog,
                                   poslednjiPotezNegativnog, dubina - 1, brojPoteza + 1);
                    if (vrednost < minimalnaVrednost)
                        minimalnaVrednost = vrednost;
                    if (minimalnaVrednost <= alpha) {
                        tabla[brojac] = 0;
                        return minimalnaVrednost;
                    }
                    if (minimalnaVrednost < beta)
                        beta = minimalnaVrednost;
                }
                tabla[brojac] = 0;
            }
        }
        return minimalnaVrednost;
    }
    else {
        std::cout << "Dalje neces moci" << std::endl;
    }
}


/*
 * Potez je taj najbolji potez koji engine treba da odigra na kraju. Prosledjujemo dalje i
 * poslednji (tj. upravo) odigrani potez kako bi u sledecem pozivu imali proveru za upravo zatvorenu
 * micu. Ova heuristika se dodaje da bi smo bili sigurni da ce engine otvarati i zatvarati mice
 * Moglo je i bez toga uz dobre koeficijente za broj figura, broj otvorenih i zatvorenih mica ali
 * ovako smo sigurni da se nece desiti da napravi micu i kaze super je ovo i nikad je ne otvori :)
 */
int max(int tabla[24], int alpha, int beta, int potez[3], int poslednjiPotezPozitivnog,
        int poslednjiPotezNegativnog, int dubina, int brojPoteza) {
    global++;
    int pobeda;
    int rezultat[4] = {0, 0, 0, 0};
    brojBlokiranih(tabla, rezultat);

    if (rezultat[1] == 2 || rezultat[1] == rezultat[3])
        pobeda = 1;
    else
        if (rezultat[0] == 2 || rezultat[0] == rezultat[2])
            pobeda = -1;
        else
            pobeda = 0;


    if (brojPoteza < 18) {
        //jos uvek je prva faza igre

        if (dubina == 0) {
            //izlaz iz rekurzije
            int dvaITriKonf[2] = {0, 0};
            brojDvaITriKonfiguracija(tabla, dvaITriKonf);

            return (11 * (poslednjiPotezPozitivnog + poslednjiPotezNegativnog)) + (9 * brojFiguraUMicamaPrvaFaza(tabla)) +
                   (1 * rezultat[0] + rezultat[1]) + (11 * rezultat[2] + rezultat[3]) +
                   (10 * dvaITriKonf[0]) + (6 * dvaITriKonf[1]);
        }

        //ako dubina jos uvek nije nula ne izlazimo iz rekurzije vec gledamo
        //koji je najbolji potez koji moze da se odigra iz ove pozicije
        int maksimalnaVrednost = -16777216;
        int vrednost = 0;
        int brojac;

        for (brojac = 0; brojac < 24; brojac++) {
            if (tabla[brojac] == 0) {
                tabla[brojac] = 1; //znamo da je pozitivni igrac onaj koji se trudi da maxuje

                if (uMici(tabla, brojac)) {
                    poslednjiPotezPozitivnog = 1;
                    //ako je to sto smo odigrali zatvorilo micu, sada imamo drugi deo poteza gde
                    //nosimo protivnikovu figuru
                    int j;

                    for (j = 0; j < 24; j++) {
                        if (tabla[j] == -1 && !legalnoNosenje(tabla, 1, j)) {
                            tabla[j] = 0; //skidamo protivnikovu figuru
                            vrednost = min(tabla, alpha, beta, poslednjiPotezPozitivnog,
                                           poslednjiPotezNegativnog, dubina - 1, brojPoteza + 1);
                            tabla[j] = -1; //vracamo kako je bilo posle evaluacije tog poteza
                            if (vrednost > maksimalnaVrednost) {
                                maksimalnaVrednost = vrednost;
                                //posto smo nasli bolji potez igramo ga
                                if (potez) {
                                    potez[0] = brojac; //igramo na poziciju brojac
                                    potez[2] = j; //nosimo figuru sa pozicije j
                                }
                            }
                            if (maksimalnaVrednost >= beta) {
                                tabla[brojac] = 0;
                                return maksimalnaVrednost;
                            }
                            if(maksimalnaVrednost > alpha)
                                alpha = maksimalnaVrednost;
                        }
                    }
                }
                else {
                    //ta pozicija na koju se igra ne pravi micu
                    poslednjiPotezPozitivnog = 0;

                    vrednost = min(tabla, alpha, beta, poslednjiPotezPozitivnog,
                                   poslednjiPotezNegativnog, dubina - 1, brojPoteza + 1);
                    if (vrednost > maksimalnaVrednost) {
                        maksimalnaVrednost = vrednost;
                        if (potez) {
                            potez[0] = brojac;
                            potez[2] = -1;
                        }
                    }
                    if (maksimalnaVrednost >= beta) {
                        tabla[brojac] = 0;
                        return maksimalnaVrednost;
                    }
                    if (maksimalnaVrednost > alpha)
                        alpha = maksimalnaVrednost;
                }
                tabla[brojac] = 0;
            }
            //else nije legalan potez jer nije slobodna pozicija
        }
        //izvrteli smo sve kombinacije poteza i nosenja i vracamo maksimalnu vrednost
        return maksimalnaVrednost;
    }
}


void alphabeta(int tabla[24], int potez[3], int dubina, int brojPoteza) {
    global = 0;
    max(tabla, -65536, 65536, potez, -1, -1, dubina, brojPoteza);
    std::cout << global << std::endl;
}
