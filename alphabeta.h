#ifndef ALPHABETA_H
#define ALPHABETA_H

int max(int tabla[24], int alpha, int beta, int potez[3], int poslednjiPotezPozitivnog,
        int poslednjiPotezNegativnog, int dubina, int brojPoteza);
int min(int tabla[24], int alpha, int beta, int poslednjiPotezPozitivnog,
        int poslednjiPotezNegativnog, int dubina, int brojPoteza);
void alphabeta(int tabla[24], int potez[3], int dubina, int brojPoteza);

#endif // ALPHABETA_H
