#ifndef PLAYER_H
#define PLAYER_H

#include "Evento.h" 

class Player {
private:
    int saudeMental;
    int energia;
    int social;
    float ira;
    int semestre;

public:
    Player() : saudeMental(80), energia(100), social(50), ira(7.0), semestre(1) {}

    void aplicarImpacto(const ImpactoStats& impacto) {
        saudeMental += impacto.deltaSaudeMental;
        energia += impacto.deltaEnergIA;
        social += impacto.deltaSocial;
        ira += impacto.deltaCRA;

        // Limita os valores (ex: 0 a 100)
        if (saudeMental > 100) saudeMental = 100;
        if (energia > 100) energia = 100;
        if (social > 100) social = 100;
        if (ira > 10.0) ira = 10.0;
        if (saudeMental < 0) saudeMental = 0;
        if (energia < 0) energia = 0;
        if (social < 0) social = 0;
        if (ira < 0) ira = 0;
    }

    // Chamado na tela "Continua..."
    void avancarSemestre() {
        semestre++;
        // Reseta um pouco os status para o novo semestre
        energia = 100;
        saudeMental = (saudeMental + 100) / 2; // Recupera metade da saúde mental perdida
    }

    // Getters para a UI
    int getSaudeMental() const { return saudeMental; }
    int getEnergia() const { return energia; }
    int getSocial() const { return social; }
    float getIRA() const { return ira; }

    // Condição de Fim de Jogo automático
    bool isGameOver() const {
        // Se a saúde mental zerar OU o ira for muito baixo por muito tempo
        return saudeMental <= 0 || (ira < 4.0 && semestre > 2);
    }
};

#endif 