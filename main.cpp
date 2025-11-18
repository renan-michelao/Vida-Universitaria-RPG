#include <SFML/Graphics.hpp>
#include "Game.h"
#include "UIManager.h"
#include <iostream>

const int LARGURA_JANELA = 1024;
const int ALTURA_JANELA = 768;


enum class GameState { MENU, PLAYING, GAME_OVER };

int main() {
    sf::RenderWindow janela(sf::VideoMode(LARGURA_JANELA, ALTURA_JANELA), "UFSCar: O Desafio do Jubilamento");
    janela.setFramerateLimit(60);

    sf::Font fonte;
    if (!fonte.loadFromFile("./google-carlito-fonts/Carlito-Regular.ttf")) {
        std::cerr << "Erro ao carregar fonte!" << std::endl;
        return -1;
    }

    GameState estado = GameState::MENU;
    Game* jogo = nullptr;

   
    estado = GameState::PLAYING;
    try {
        jogo = new Game(janela, fonte);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
   

    // Texto para tela de Fim de Jogo
    sf::Text fullscreenText;
    fullscreenText.setFont(fonte);
    fullscreenText.setFillColor(sf::Color::White);


    while (janela.isOpen()) {
        
        // --- LOOP DE EVENTOS ---
        sf::Event evento;
        while (janela.pollEvent(evento)) {
            
            if (evento.type == sf::Event::Closed) {
                janela.close();
            }

            
            if (estado == GameState::PLAYING && jogo != nullptr) {
                jogo->processarEvento(evento);
            } 
            else if (estado == GameState::GAME_OVER) {
                // Permite fechar com Clique ou Espaço
                if (evento.type == sf::Event::MouseButtonPressed ||
                    (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Space)) 
                {
                    janela.close();
                }
            }
        }
        // --- FIM DO LOOP DE EVENTOS ---


        // --- LÓGICA DE ESTADOS SIMPLIFICADA ---
        if (estado == GameState::PLAYING && jogo != nullptr) {
            
            jogo->atualizar(); 

            // A única transição é para Fim de Jogo
            if (jogo->getStatus() == Game::GameStatus::HARD_END) {
                estado = GameState::GAME_OVER;
                
                std::string msgFinal = jogo->getFinalMessage();
                std::string wrappedMsg = UIManager::wrapText(msgFinal, LARGURA_JANELA - 80, fonte, 30);
                
                fullscreenText.setCharacterSize(30); 
                fullscreenText.setString(sf::String::fromUtf8(wrappedMsg.begin(), wrappedMsg.end()));
                
                sf::FloatRect bounds = fullscreenText.getLocalBounds();
                fullscreenText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
                fullscreenText.setPosition(LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f);
            }
        }
        
        // --- DESENHO ---
        janela.clear(sf::Color::Black); // limpa a tela

        if (estado == GameState::PLAYING && jogo != nullptr) {
            jogo->desenhar(); 
        } 
        else if (estado == GameState::GAME_OVER) {
            janela.draw(fullscreenText);
        } 
        else if (estado == GameState::MENU) {
            fullscreenText.setString("MENU (Ainda nao implementado)\nClique para iniciar");
            janela.draw(fullscreenText);
        }
        
        janela.display(); // exibe a tela
    }

    delete jogo;
    return 0;
}