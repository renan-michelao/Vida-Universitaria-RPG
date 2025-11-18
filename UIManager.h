#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Evento.h"
#include <sstream> // Para a função wrapText

// Classe auxiliar para criar uma barra de stat visual
class StatBar {
public:
    sf::RectangleShape fundo;
    sf::RectangleShape valor;
    sf::Text rotulo;
    StatBar() {}
    void setup(sf::Font& fonte, std::string nome, float x, float y, sf::Color cor) {
        fundo.setSize(sf::Vector2f(200, 20));
        fundo.setPosition(x, y);
        fundo.setFillColor(sf::Color(50, 50, 50));
        fundo.setOutlineColor(sf::Color::White);
        fundo.setOutlineThickness(1);
        valor.setSize(sf::Vector2f(200, 20));
        valor.setPosition(x, y);
        valor.setFillColor(cor);
        rotulo.setFont(fonte);
        rotulo.setString(nome);
        rotulo.setCharacterSize(16);
        rotulo.setPosition(x + 210, y - 2);
    }
    void update(int val) {
        float perc = val / 100.0f;
        if (perc < 0) perc = 0;
        if (perc > 1) perc = 1;
        valor.setSize(sf::Vector2f(perc * 200.0f, 20));
    }
    void draw(sf::RenderWindow& janela) {
        janela.draw(fundo);
        janela.draw(valor);
        janela.draw(rotulo);
    }
};

class UIManager {
private:
    sf::RenderWindow& janela;
    const sf::Font& fonte;
    sf::Sprite spriteFundo;
    sf::RectangleShape caixaTextoFundo;
    sf::Text textoDescricao;
    sf::RectangleShape botaoEsq, botaoDir;
    sf::Text textoBotaoEsq, textoBotaoDir;

    // --- NOVO BOTÃO ADICIONADO ---
    sf::RectangleShape botaoProximo;
    sf::Text textoBotaoProximo;
    // ----------------------------

    StatBar barraSaudeMental;
    StatBar barraEnergia;
    StatBar barraSocial;
    sf::Text textoCRA;

    // Flag para saber quais botões desenhar
    EventoTipo tipoEventoAtual; 

public:
    // Função estática para quebra de linha (para main.cpp)
    static std::string wrapText(const std::string& text, float maxWidth, const sf::Font& font, unsigned int charSize) {
        sf::Text tempText;
        tempText.setFont(font);
        tempText.setCharacterSize(charSize);
        std::string result, currentLine;
        std::stringstream ss(text);
        std::string word;
        
        while (ss >> word) {
            std::string testLine = currentLine;
            if (!currentLine.empty()) testLine += " ";
            testLine += word;
            
            tempText.setString(sf::String::fromUtf8(testLine.begin(), testLine.end()));
            
            if (tempText.getLocalBounds().width > maxWidth) {
                result += currentLine + "\n";
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }
        result += currentLine;
        return result;
    }


    UIManager(sf::RenderWindow& jan, const sf::Font& f) : janela(jan), fonte(f), tipoEventoAtual(EventoTipo::NARRATIVA) {
        float w = janela.getSize().x;
        float h = janela.getSize().y;

        // Caixa de descrição
        caixaTextoFundo.setSize(sf::Vector2f(w - 40, h / 3));
        caixaTextoFundo.setPosition(20, h - (h / 3) - 20);
        caixaTextoFundo.setFillColor(sf::Color(0, 0, 0, 180));
        caixaTextoFundo.setOutlineColor(sf::Color::White);
        caixaTextoFundo.setOutlineThickness(2);

        textoDescricao.setFont(fonte);
        textoDescricao.setCharacterSize(24);
        textoDescricao.setPosition(40, h - (h / 3) - 10);
        
        // Botões de escolha (Esq/Dir)
        sf::Vector2f tamBotao(w / 2 - 40, 80);
        botaoEsq.setSize(tamBotao);
        botaoEsq.setPosition(20, h - (h / 3) + 120);
        botaoEsq.setFillColor(sf::Color(40, 40, 80));
        
        botaoDir.setSize(tamBotao);
        botaoDir.setPosition(w / 2 + 20, h - (h / 3) + 120);
        botaoDir.setFillColor(sf::Color(80, 40, 40));

        textoBotaoEsq.setFont(fonte);
        textoBotaoEsq.setCharacterSize(20);
        
        textoBotaoDir.setFont(fonte);
        textoBotaoDir.setCharacterSize(20);

        // --- CONFIGURAÇÃO DO NOVO BOTÃO ---
        botaoProximo.setSize(sf::Vector2f(w - 40, 80));
        botaoProximo.setPosition(20, h - (h / 3) + 120);
        botaoProximo.setFillColor(sf::Color(40, 80, 40)); // Verde
        
        textoBotaoProximo.setFont(fonte);
        textoBotaoProximo.setCharacterSize(20);
        textoBotaoProximo.setString("Comecar Proximo Semestre");
        // (Posição será definida no update)
        // ----------------------------------

        // Stats
        barraSaudeMental.setup(const_cast<sf::Font&>(f), "Saude Mental", 20, 20, sf::Color::Cyan);
        barraEnergia.setup(const_cast<sf::Font&>(f), "Energia", 20, 50, sf::Color::Yellow);
        barraSocial.setup(const_cast<sf::Font&>(f), "Social", 20, 80, sf::Color::Magenta);
        textoCRA.setFont(fonte);
        textoCRA.setCharacterSize(20);
        textoCRA.setPosition(20, 110);
        textoCRA.setFillColor(sf::Color::Green);
    }

    // --- FUNÇÃO UPDATE MODIFICADA ---
    void update(const Player& player, Evento* evento, const sf::Texture& texturaFundo) {
        // Guarda o tipo de evento para sabermos o que desenhar
        tipoEventoAtual = evento->tipo;
        
        // Fundo
        spriteFundo.setTexture(texturaFundo);
        float scaleX = janela.getSize().x / (float)texturaFundo.getSize().x;
        float scaleY = janela.getSize().y / (float)texturaFundo.getSize().y;
        spriteFundo.setScale(scaleX, scaleY);
        
        // --- TEXT WRAP ---
        float descMaxWidth = caixaTextoFundo.getSize().x - 40;
        std::string wrappedDesc = UIManager::wrapText(evento->descricao, descMaxWidth, fonte, textoDescricao.getCharacterSize());
        textoDescricao.setString(sf::String::fromUtf8(wrappedDesc.begin(), wrappedDesc.end()));

        // Lógica para qual botão mostrar
        if (tipoEventoAtual == EventoTipo::NARRATIVA) {
            float buttonMaxWidth = botaoEsq.getSize().x - 40;
            std::string wrappedBtnEsq = UIManager::wrapText(evento->textoOpcaoEsq, buttonMaxWidth, fonte, textoBotaoEsq.getCharacterSize());
            std::string wrappedBtnDir = UIManager::wrapText(evento->textoOpcaoDir, buttonMaxWidth, fonte, textoBotaoDir.getCharacterSize());

            textoBotaoEsq.setString(sf::String::fromUtf8(wrappedBtnEsq.begin(), wrappedBtnEsq.end()));
            textoBotaoDir.setString(sf::String::fromUtf8(wrappedBtnDir.begin(), wrappedBtnDir.end()));

            // Centraliza texto Botão Esquerdo
            sf::FloatRect boundsEsq = textoBotaoEsq.getLocalBounds();
            textoBotaoEsq.setOrigin(boundsEsq.left + boundsEsq.width / 2.0f, boundsEsq.top + boundsEsq.height / 2.0f);
            textoBotaoEsq.setPosition(botaoEsq.getPosition().x + botaoEsq.getSize().x / 2.0f, botaoEsq.getPosition().y + botaoEsq.getSize().y / 2.0f);

            // Centraliza texto Botão Direito
            sf::FloatRect boundsDir = textoBotaoDir.getLocalBounds();
            textoBotaoDir.setOrigin(boundsDir.left + boundsDir.width / 2.0f, boundsDir.top + boundsDir.height / 2.0f);
            textoBotaoDir.setPosition(botaoDir.getPosition().x + botaoDir.getSize().x / 2.0f, botaoDir.getPosition().y + botaoDir.getSize().y / 2.0f);
        
        } else if (tipoEventoAtual == EventoTipo::FINAL_SEMESTRE) {
            // Centraliza texto Botão Próximo
            sf::FloatRect boundsProx = textoBotaoProximo.getLocalBounds();
            textoBotaoProximo.setOrigin(boundsProx.left + boundsProx.width / 2.0f, boundsProx.top + boundsProx.height / 2.0f);
            textoBotaoProximo.setPosition(botaoProximo.getPosition().x + botaoProximo.getSize().x / 2.0f, botaoProximo.getPosition().y + botaoProximo.getSize().y / 2.0f);
        }
        
        // Stats
        barraSaudeMental.update(player.getSaudeMental());
        barraEnergia.update(player.getEnergia());
        barraSocial.update(player.getSocial());
        std::string strCRA = "CRA: " + std::to_string(player.getIRA()).substr(0, 4);
        textoCRA.setString(sf::String::fromUtf8(strCRA.begin(), strCRA.end()));
    }

    // --- FUNÇÃO HANDLECLICK MODIFICADA ---
    int handleClick(sf::Vector2f mousePos) {
        if (tipoEventoAtual == EventoTipo::NARRATIVA) {
            if (botaoEsq.getGlobalBounds().contains(mousePos)) return 1;
            if (botaoDir.getGlobalBounds().contains(mousePos)) return 2;
        } 
        else if (tipoEventoAtual == EventoTipo::FINAL_SEMESTRE) {
            if (botaoProximo.getGlobalBounds().contains(mousePos)) return 3; // Retorna 3 para "Próximo"
        }
        return 0; // Nenhum clique válido
    }

    // --- FUNÇÃO DRAW MODIFICADA ---
    void draw() {
        janela.draw(spriteFundo);
        janela.draw(caixaTextoFundo);
        janela.draw(textoDescricao);

        // Lógica de qual botão desenhar
        if (tipoEventoAtual == EventoTipo::NARRATIVA) {
             janela.draw(botaoEsq);
             janela.draw(botaoDir);
             janela.draw(textoBotaoEsq);
             janela.draw(textoBotaoDir);
        } else if (tipoEventoAtual == EventoTipo::FINAL_SEMESTRE) {
             janela.draw(botaoProximo);
             janela.draw(textoBotaoProximo);
        }
        // Se for FINAL_JOGO, não desenha botões

        barraSaudeMental.draw(janela);
        barraEnergia.draw(janela);
        barraSocial.draw(janela);
        janela.draw(textoCRA);
    }
};

#endif