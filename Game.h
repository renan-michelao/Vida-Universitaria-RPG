#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <map>
#include <set>
#include "Player.h"
#include "Evento.h"

class UIManager; 

class Game {
public:
    // Enum para o main.cpp saber o que fazer
    enum class GameStatus { RUNNING, HARD_END };

private:
    sf::RenderWindow& janela;
    sf::Font fonte;
    Player player;
    UIManager* ui;

   // Ponteiros de gerenciamento da árvore
    Evento* arvoreEventos; // Ponteiro para a Raiz
    Evento* eventoAtual;   // Ponteiro para o nó atual
    // -------------------------------

    GameStatus status; // Guarda o estado atual do jogo

    // Mapa de texturas de fundo (imagens)
    std::map<std::string, sf::Texture> texturas;

    // --- Funções da Árvore ---
    void construirArvore(); // Onde a história do jogo é criada
    
    void deletarArvore(Evento* no, std::set<Evento*>& visitados); 

    void carregarTexturas(); // carrega as imagens do jogo
    void aplicarEscolha(int escolha); // 1=Esq, 2=Dir, 3=Proximo

public:
    Game(sf::RenderWindow& jan, const sf::Font& f);
    ~Game();

    // Funções chamadas pelo main loop
    void processarEvento(sf::Event evento);
    void atualizar();
    void desenhar();

    // Funções para o main gerenciar o estado
    GameStatus getStatus() const { return status; }
    std::string getFinalMessage() const;
};

#endif