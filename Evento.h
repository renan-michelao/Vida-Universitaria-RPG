#ifndef EVENTO_H
#define EVENTO_H

#include <string>

// Estrutura auxiliar para definir o impacto de uma escolha
struct ImpactoStats {
    int deltaSaudeMental;
    int deltaEnergIA;
    int deltaSocial;
    float deltaCRA;

    // Construtor
    ImpactoStats(int sm = 0, int e = 0, int s = 0, float c = 0.0f)
        : deltaSaudeMental(sm), deltaEnergIA(e), deltaSocial(s), deltaCRA(c) {}
};

// Define o "tipo" de nó para sabermos como o jogo deve reagir
enum class EventoTipo { 
    NARRATIVA,      // Um evento normal da história (2 botões)
    FINAL_SEMESTRE, // Um final de semestre (1 botão "próximo")
    FINAL_JOGO      // Um final definitivo (sem botões)
};


// Esta classe É O NÓ da Árvore Binária
class Evento {
public:
    std::string descricao;
    std::string idBackground;
    std::string textoOpcaoEsq;
    std::string textoOpcaoDir;
    EventoTipo tipo; // Guarda o tipo do evento

    ImpactoStats impactoEsq;
    ImpactoStats impactoDir;

    //  A ÁRVORE BINÁRIA ESTÁ AQUI
    Evento* proximoEventoEsq;    // Ponteiro para o filho da ESQUERDA
    Evento* proximoEventoDir;    // Ponteiro para o filho da DIREITA
    // ----------------------------------

    // Construtor principal (para nós de narrativa)
    Evento(std::string desc, std::string bg, std::string opE, std::string opD)
        : descricao(desc), idBackground(bg), 
          textoOpcaoEsq(opE), textoOpcaoDir(opD),
          tipo(EventoTipo::NARRATIVA),
          proximoEventoEsq(nullptr), proximoEventoDir(nullptr) {}

    // Construtor para FINAIS (Nós Folha)
    Evento(std::string descFinal, std::string bg, EventoTipo t)
        : descricao(descFinal), idBackground(bg),
          textoOpcaoEsq(""), textoOpcaoDir(""),
          tipo(t), // Deve ser FINAL_SEMESTRE ou FINAL_JOGO
          proximoEventoEsq(nullptr), proximoEventoDir(nullptr) {}

    // Verifica se é um nó folha (um final de qualquer tipo)
    bool isFinal() const {
        return tipo == EventoTipo::FINAL_SEMESTRE || tipo == EventoTipo::FINAL_JOGO;
    }
};

#endif 