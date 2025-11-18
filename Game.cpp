#include "Game.h"
#include "UIManager.h"
#include <iostream>
#include <set> // Para o destrutor

Game::Game(sf::RenderWindow& jan, const sf::Font& f)
    : janela(jan), fonte(f), arvoreEventos(nullptr), eventoAtual(nullptr), 
      status(GameStatus::RUNNING) // O jogo começa rodando
{
    // UI é um ponteiro
    ui = new UIManager(jan, f);

    carregarTexturas();
    construirArvore();
    
    eventoAtual = arvoreEventos; // Começa na raiz
    ui->update(player, eventoAtual, texturas[eventoAtual->idBackground]);
}

Game::~Game() {
    // Deleta o UI e a Árvore
    delete ui;
    std::set<Evento*> visitados;
    deletarArvore(arvoreEventos, visitados);
}

void Game::carregarTexturas() {
    // São as imagens
    if (!texturas["bg_inicio"].loadFromFile("./img/bg_inicio.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_inicio.png");
    }
    if (!texturas["bg_aula"].loadFromFile("./img/bg_aula.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_aula.png");
    }
    if (!texturas["bg_ru"].loadFromFile("./img/bg_ru.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_ru.png");
    }
    if (!texturas["bg_festa"].loadFromFile("./img/bg_festa.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_festa.png");
    }
    if (!texturas["bg_jubilado"].loadFromFile("./img/bg_jubilado.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_jubilado.png");
    }
    if (!texturas["bg_dc"].loadFromFile("./img/bg_dc.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_dc.png");
    }
    if (!texturas["bg_at"].loadFromFile("./img/bg_at.png")) {
        throw std::runtime_error("Erro ao carregar ./img/bg_at.png");
    }
}


// ###   CONSTRUÇÃO DA ÁRVORE (HISTÓRIA)                          ###
void Game::construirArvore() {
    
    // --- FINAIS DE JOGO (Nós Folha Definitivos) ---
    Evento* final_jubilado = new Evento("Fim do Semestre 4. Seu IRA ficou abaixo de 4.0 por muitos semestres. A UFSCar te enviou o email de jubilamento.", "bg_jubilado", EventoTipo::FINAL_JOGO);
    Evento* final_surto_saude = new Evento("Fim do Semestre 3. Sua Saúde Mental chegou a 0. Você não aguenta mais e tranca o curso por 'burnout'. Fim de jogo.", "bg_jubilado", EventoTipo::FINAL_JOGO);
    Evento* final_formado_honras = new Evento("Formatura! Você passou por tudo, manteve o IRA acima de 9.0 com a ajuda de Lucas, publicou artigos com a Prof. Ana (sua mentora de IPA) e tem um namoro saudável com Alice. Você recebe o diploma com honras. Parabéns!", "bg_inicio", EventoTipo::FINAL_JOGO);
    Evento* final_formado_sozinho = new Evento("Formatura. Você se forma... sozinho. Você traiu a confiança de Alice e Lucas. Seu IRA é alto, mas você não tem ninguém para comemorar. O reitor chama seu nome em um auditório vazio.", "bg_jubilado", EventoTipo::FINAL_JOGO);
    Evento* final_formado_festeiro = new Evento("Formatura! 8 anosDepois... Você e Bruno finalmente conseguem o diploma! Seu IRA é 5.1, mas as histórias que você viveu na Atlética e no 'Palquinho' são lendárias. 'O que importa é o que a gente viveu!'", "bg_festa", EventoTipo::FINAL_JOGO);
    Evento* final_transferencia = new Evento("Fim do Semestre 4. Você percebe que Computação não é pra você. Você pede transferência para Estatística para ficar com Alice e focar em ciência de dados. Um novo começo!", "bg_ru", EventoTipo::FINAL_JOGO);

    // --- SEMESTRE 1: BIXO (Cálculo 1, IPA, GA) -----------------------
    arvoreEventos = new Evento("Semestre 1. Bixo! Primeiro dia. A aula de IPA (Introdução à Programação) da Prof. Ana parece incrível.", "bg_inicio",
                               "Ir à aula de IPA. (Quero começar bem)",
                               "Pular a aula. A Recepção dos Bixos da Atlética é no 'Palquinho'.");
    arvoreEventos->impactoEsq = ImpactoStats(+5, -10, 0, +0.2f);
    arvoreEventos->impactoDir = ImpactoStats(+10, -20, +20, -0.1f);

    Evento* evt_1_aula_ip = new Evento("A aula de IPA foi ótima. A Prof. Ana é muito inspiradora. Na saída, você vê um colega, Lucas, apanhando com a lista de GA na BCo.", "bg_aula",
                                     "'Oi, sou bixo também. Quer ajuda? A gente pode tentar junto.'",
                                     "Melhor não incomodar. Vou focar na minha lista de Cálculo.");
    evt_1_aula_ip->impactoEsq = ImpactoStats(+5, -10, +15, +0.1f); 
    evt_1_aula_ip->impactoDir = ImpactoStats(-5, -10, -5, +0.2f); 

    Evento* evt_1_conhece_lucas = new Evento("Vocês passam a tarde na BCo. Lucas é muito focado. 'Temos prova do Carrasco de C1 semana que vem. Quer estudar junto?'", "bg_ru",
                                           "'Com certeza! Dizem que ele reprova 70%.'",
                                           "'Valeu, mas prefiro estudar sozinho no meu ritmo.'");
    evt_1_conhece_lucas->impactoEsq = ImpactoStats(0, -10, +10, +0.4f); 
    evt_1_conhece_lucas->impactoDir = ImpactoStats(-5, -10, -5, +0.2f);

    Evento* evt_1_atletica = new Evento("A recepção é no 'Palquinho'. Um veterano, Bruno, te entrega uma 'Bomba'. 'Beba, bixo! Bem-vindo à UFSCar!'", "bg_festa",
                                      "'Valeu, veterano!' (Bebe tudo de uma vez)",
                                      "'Vou com calma, tenho aula de Cálculo 1 amanhã.'");
    evt_1_atletica->impactoEsq = ImpactoStats(+10, -30, +20, -0.2f);
    evt_1_atletica->impactoDir = ImpactoStats(0, -10, +5, 0);   

    Evento* evt_1_trote = new Evento("Depois da bebida, o Trote começa. Pintura, gincana e pedágio no semáforo. 'Bixo, tem que participar de tudo!', grita Bruno.", "bg_festa",
                                   "'Bora! Já tô sujo mesmo!' (Participa de tudo)",
                                   "'Vou ficar só olhando, valeu.' (Fica de canto)");
    evt_1_trote->impactoEsq = ImpactoStats(+15, -20, +20, -0.1f);
    evt_1_trote->impactoDir = ImpactoStats(-5, -5, -10, 0);   

    Evento* evt_1_conhece_bruno = new Evento("Você e Bruno viram amigos. Ele te chama pra todas as festas no Palquinho. A P1 de Cálculo 1 do Carrasco é semana que vem.", "bg_festa",
                                           "'Bora pro Palquinho de novo! É pré-P1, não dá azar!'",
                                           "'Não posso, Bruno. Preciso estudar pro Carrasco.'");
    evt_1_conhece_bruno->impactoEsq = ImpactoStats(+15, -40, +20, -0.5f);
    evt_1_conhece_bruno->impactoDir = ImpactoStats(-10, -10, -10, +0.2f);

    Evento* evt_1_prova_festa = new Evento("Dia da P1 de C1. Você está de ressaca. O Prof. Carrasco distribui a prova no AT-05. Parece grego.", "bg_at",
                                         "Tentar fazer o que dá. (Assumir a nota baixa)",
                                         "Olhar pro lado. Lucas parece saber tudo. (Tentar colar)");
    evt_1_prova_festa->impactoEsq = ImpactoStats(-10, -10, 0, -1.0f);
    evt_1_prova_festa->impactoDir = ImpactoStats(0, -5, 0, 0);     

    Evento* evt_1_prova_estudo = new Evento("Dia da P1 de C1, no AT-05. Você estudou (sozinho ou com Lucas) e se sente preparado.", "bg_at",
                                          "Fazer a prova com confiança.",
                                          "Ajudar Bruno, que está do seu lado e suando frio. (Sussurrar respostas)");
    evt_1_prova_estudo->impactoEsq = ImpactoStats(+5, -10, 0, +1.0f); 
    evt_1_prova_estudo->impactoDir = ImpactoStats(+5, -10, +15, +0.5f);

    // --- MUDANÇA AQUI ---
    Evento* evt_1_cola_flagra = new Evento("Você tenta colar, mas o Prof. Carrasco vê. 'Aluno, me entregue sua prova. Te vejo na Chefia do Departamento.'", "bg_jubilado", EventoTipo::FINAL_SEMESTRE);
    
    // --- FINAIS DO SEMESTRE 1 ---
    Evento* final_S1_reprovado_calc = new Evento("Fim do Semestre 1. Você REPROVOU em Cálculo 1. Bem-vindo à DP.", "bg_inicio", EventoTipo::FINAL_SEMESTRE);
    Evento* final_S1_aprovado = new Evento("Fim do Semestre 1. Você PASSOU em Cálculo 1! Limpo para o próximo.", "bg_inicio", EventoTipo::FINAL_SEMESTRE);
    
    // -----------------------------------------------------------------
    // --- SEMESTRE 2: VETERANO (AED1, CAP, C2, TUSCA) ---------------
    // -----------------------------------------------------------------
    
    Evento* evt_2_raiz_dp = new Evento("Semestre 2. Aula de AED1 com o Prof. Ferrari, CAP, C2 e a DP de C1. É pesado. Você está no RU.", "bg_ru",
                                     "Vou focar 100%. Só BCo e aula.",
                                     "Preciso socializar. Vou no 'Palquinho' hoje.");
    evt_2_raiz_dp->impactoEsq = ImpactoStats(-10, -20, -10, +0.3f);
    evt_2_raiz_dp->impactoDir = ImpactoStats(+10, -30, +20, -0.3f);

    Evento* evt_2_raiz_limpo = new Evento("Semestre 2. Aula de AED1 com o Prof. Ferrari, CAP, C2 e Álgebra Linear. Você está limpo! Você está no RU.", "bg_ru",
                                        "Focar em AED1. Ouvi dizer que o Ferrari é osso duro.",
                                        "Vou relaxar. Bruno está chamando pro 'Palquinho'.");
    evt_2_raiz_limpo->impactoEsq = ImpactoStats(+5, -10, 0, +0.3f);
    evt_2_raiz_limpo->impactoDir = ImpactoStats(+10, -30, +20, -0.1f);
    
    Evento* evt_2_conhece_Alice_ru = new Evento("No RU, você vê uma garota da Estatística rindo. Ela derruba o suco perto de você.", "bg_ru",
                                              "Ajudar a limpar. 'Semana corrida, né? Sou da BCC.'",
                                              "Fingir que não viu e continuar comendo.");
    evt_2_conhece_Alice_ru->impactoEsq = ImpactoStats(+20, -5, +15, 0); 
    evt_2_conhece_Alice_ru->impactoDir = ImpactoStats(-5, 0, -5, 0);  

    Evento* evt_2_conhece_Alice_festa = new Evento("No 'Palquinho', Bruno te apresenta Alice, da Estatística. 'Ela é cobiçada, bixo, boa sorte.'", "bg_festa",
                                                 "'Oi Alice, tudo bem? Bruno fala muito de você.'",
                                                 "'E aí.' (Virar e pegar outra cerveja)");
    evt_2_conhece_Alice_festa->impactoEsq = ImpactoStats(+20, -10, +15, 0);
    evt_2_conhece_Alice_festa->impactoDir = ImpactoStats(0, -5, +5, 0);  

    Evento* evt_2_pre_tusca_Alice = new Evento("Você e Alice estão saindo. A TUSCA (Jogos em outra cidade) está chegando. Bruno te chama pra ir com a Atlética.", "bg_inicio",
                                             "'Alice, vamos pra TUSCA? Vai ser legal!'",
                                             "'Bruno, vou pra TUSCA com a galera da Atlética!' (Ir sem Alice)");
    evt_2_pre_tusca_Alice->impactoEsq = ImpactoStats(+10, -20, +10, 0); 
    evt_2_pre_tusca_Alice->impactoDir = ImpactoStats(+5, -30, +15, -0.2f);

    Evento* evt_2_pre_tusca_solteiro = new Evento("A TUSCA está chegando. Você está solteiro. Bruno te chama pra ir com a Atlética.", "bg_inicio",
                                                "'Óbvio! Ouvi dizer que é a melhor festa do ano!'",
                                                "'Não posso, a P2 de AED1 do Ferrari é logo depois.'");
    evt_2_pre_tusca_solteiro->impactoEsq = ImpactoStats(+15, -40, +25, -0.5f);
    evt_2_pre_tusca_solteiro->impactoDir = ImpactoStats(-10, -10, -10, +0.5f);

    Evento* evt_2_traicao = new Evento("TUSCA. Festa à fantasia. Você bebeu demais. Bruno sumiu. Uma garota de outra faculdade te chama pra dançar. Alice não está vendo.", "bg_festa",
                                     "'Só vou dançar.' (Mas a noite esquenta...)",
                                     "'Não, valeu. Tô procurando minha namorada/amigos.' (Ser fiel)");
    evt_2_traicao->impactoEsq = ImpactoStats(+10, -50, -5, -0.3f); 
    evt_2_traicao->impactoDir = ImpactoStats(+10, -20, +10, +0.1f);

    // --- FIM DO SEMESTRE 2 ---
    Evento* final_S2_reprovou_aed1 = new Evento("Fim do Semestre 2. Você foi pra TUSCA e reprovou com o Ferrari em AED1. A Prof. Ana (de IPA) está decepcionada. A DP vai ser divertida...", "bg_jubilado", EventoTipo::FINAL_SEMESTRE);
    Evento* final_S2_passou_namorando = new Evento("Fim do Semestre 2. Você passou em tudo e seu namoro com Alice está ótimo. A vida é boa.", "bg_inicio", EventoTipo::FINAL_SEMESTRE);
    Evento* final_S2_passou_solteiro = new Evento("Fim do Semestre 2. Você passou em tudo, mas está solteiro. Mais tempo pra focar (ou festejar).", "bg_inicio", EventoTipo::FINAL_SEMESTRE);

    // --- SEMESTRE 3: CRISE (AOC, POO, AED2) --------------------------
    Evento* evt_3_raiz_bem = new Evento("Semestre 3. Desafio em dobro: POO e o trabalho de AOC. Você está no laboratório do DC.", "bg_dc",
                                      "Fazer dupla com Lucas. (Vamos varar a noite no lab programando a FPGA!)",
                                      "Fazer dupla com Bruno. (A gente 'pega um VHDL da net')");
    evt_3_raiz_bem->impactoEsq = ImpactoStats(+5, -20, +5, +0.4f); 
    evt_3_raiz_bem->impactoDir = ImpactoStats(+5, -10, +10, -0.2f);

    Evento* evt_3_plagio = new Evento("Bruno achou um VHDL pronto na internet. 'É só mudar o nome dos registradores, professor nem vê.'", "bg_dc",
                                    "'De jeito nenhum, Bruno. Isso é plágio. Vou fazer do zero.'",
                                    "'Ok... Mas se der problema, a culpa é sua.' (Copiar)");
    evt_3_plagio->impactoEsq = ImpactoStats(-10, -50, -10, +0.1f);
    evt_3_plagio->impactoDir = ImpactoStats(0, -5, +5, +0.5f);  

    Evento* evt_3_plagio_descoberto = new Evento("O professor de AOC descobriu o plágio. 'Vocês dois, na sala do Chefe do Departamento. AGORA.'", "bg_jubilado",
                                               "Ir para a Reunião.", "Ir para a Reunião.");
    
    Evento* evt_3_reuniao_chefe = new Evento("O Prof. Silva (Chefe do DC) está furioso. 'Plágio é inaceitável! De quem foi a ideia?' Bruno está quieto.", "bg_dc",
                                           "'Professor, a culpa foi minha. Eu peguei o trabalho. O Bruno não sabia.' (Salvar Bruno)",
                                           "'Foi o Bruno! Ele me forçou a fazer, eu não sabia que era plágio!' (Trair Bruno)");
    evt_3_reuniao_chefe->impactoEsq = ImpactoStats(-30, 0, +10, -2.0f);
    evt_3_reuniao_chefe->impactoDir = ImpactoStats(-20, 0, -30, -1.5f);

    Evento* evt_3_Alice_descobre = new Evento("Você está com Alice, mas ela vê fotos da TUSCA no Instagram de Bruno. Ela te reconhece beijando outra. '...O que é isso?'", "bg_inicio",
                                            "'Não é o que parece! Eu estava bêbado, não significou nada! Me perdoa!'",
                                            "'Alice, eu... não sei o que dizer. Eu errei.' (Confessar)");
    evt_3_Alice_descobre->impactoEsq = ImpactoStats(-40, -10, -20, -0.5f);
    evt_3_Alice_descobre->impactoDir = ImpactoStats(-30, -5, -15, -0.3f); 

    // --- SEMESTRE 4: RETA FINAL (SO, BD, Redes) ----------------------    
    Evento* evt_4_raiz_processo = new Evento("Semestre 4. Você tem um processo disciplinar. Seu IRA é 3.5. É sua última chance antes do jubilamento.", "bg_jubilado",
                                           "Trancar tudo. Só estudar as DPs.",
                                           "Desistir. (Ir pro Palquinho todo dia)");
    evt_4_raiz_processo->impactoEsq = ImpactoStats(-20, -40, -20, +1.0f);
    evt_4_raiz_processo->impactoDir = ImpactoStats(+10, -10, +10, -1.0f);

    Evento* evt_4_raiz_termino = new Evento("Semestre 4. Você e Alice terminaram. Você se sente péssimo. Lucas te chama pra uma IC e a Prof. Ana (de IPA) te oferece uma monitoria no AT-06.", "bg_at",
                                          "'Sim, Lucas. Preciso focar na IC.' (Mergulhar no trabalho)",
                                          "'Vou pegar a monitoria de IPA. Preciso de um tempo.' (Depressão)");
    evt_4_raiz_termino->impactoEsq = ImpactoStats(-10, -20, +10, +0.8f);
    evt_4_raiz_termino->impactoDir = ImpactoStats(-30, -10, -10, -0.5f);

    Evento* evt_4_raiz_namorando = new Evento("Semestre 4. Você e Alice estão ótimos. Ela quer fazer um intercâmbio. 'Você não acha que a gente devia ir?'", "bg_inicio",
                                            "'Meu foco é a UFSCar. Não posso ir agora.'",
                                            "'Eu vou com você. A gente dá um jeito.' (Mudar de vida)");
    evt_4_raiz_namorando->impactoEsq = ImpactoStats(-5, -10, +5, +0.3f);
    evt_4_raiz_namorando->impactoDir = ImpactoStats(+10, -5, +10, 0);   


    // ###   PASSO 3: CONECTAR A ÁRVORE (LIGAR OS PONTEIROS)          ###

    // --- CONEXÕES SEMESTRE 1 ---
    arvoreEventos->proximoEventoEsq = evt_1_aula_ip;
    arvoreEventos->proximoEventoDir = evt_1_atletica;

    evt_1_aula_ip->proximoEventoEsq = evt_1_conhece_lucas;
    evt_1_aula_ip->proximoEventoDir = evt_1_prova_estudo; 
    
    evt_1_conhece_lucas->proximoEventoEsq = evt_1_prova_estudo;
    evt_1_conhece_lucas->proximoEventoDir = evt_1_prova_estudo;

    evt_1_atletica->proximoEventoEsq = evt_1_trote; 
    evt_1_atletica->proximoEventoDir = evt_1_prova_estudo; 
    
    evt_1_trote->proximoEventoEsq = evt_1_conhece_bruno; 
    evt_1_trote->proximoEventoDir = evt_1_conhece_bruno; 
    
    evt_1_conhece_bruno->proximoEventoEsq = evt_1_prova_festa; 
    evt_1_conhece_bruno->proximoEventoDir = evt_1_prova_estudo;

    evt_1_prova_festa->proximoEventoEsq = final_S1_reprovado_calc; 
    evt_1_prova_festa->proximoEventoDir = evt_1_cola_flagra;       
    
    evt_1_prova_estudo->proximoEventoEsq = final_S1_aprovado;      
    evt_1_prova_estudo->proximoEventoDir = final_S1_aprovado;      
    
    evt_1_cola_flagra->proximoEventoEsq = evt_2_raiz_dp;
    evt_1_cola_flagra->proximoEventoDir = evt_2_raiz_dp;

    // --- CONEXÕES PÓS-SEMESTRE 1 (Raízes do S2) ---
    final_S1_reprovado_calc->proximoEventoEsq = evt_2_raiz_dp;
    final_S1_reprovado_calc->proximoEventoDir = evt_2_raiz_dp;
    
    final_S1_aprovado->proximoEventoEsq = evt_2_raiz_limpo;
    final_S1_aprovado->proximoEventoDir = evt_2_raiz_limpo;

    // --- CONEXÕES SEMESTRE 2 ---
    evt_2_raiz_dp->proximoEventoEsq = evt_2_conhece_Alice_ru;     
    evt_2_raiz_dp->proximoEventoDir = evt_2_conhece_Alice_festa; 
    
    evt_2_raiz_limpo->proximoEventoEsq = evt_2_conhece_Alice_ru;    
    evt_2_raiz_limpo->proximoEventoDir = evt_2_conhece_Alice_festa; 

    evt_2_conhece_Alice_ru->proximoEventoEsq = evt_2_pre_tusca_Alice;   
    evt_2_conhece_Alice_ru->proximoEventoDir = evt_2_pre_tusca_solteiro; 
    
    evt_2_conhece_Alice_festa->proximoEventoEsq = evt_2_pre_tusca_Alice;   
    evt_2_conhece_Alice_festa->proximoEventoDir = evt_2_pre_tusca_solteiro;

    evt_2_pre_tusca_Alice->proximoEventoEsq = evt_2_traicao; 
    evt_2_pre_tusca_Alice->proximoEventoDir = evt_2_traicao; 
    
    evt_2_pre_tusca_solteiro->proximoEventoEsq = final_S2_reprovou_aed1; 
    evt_2_pre_tusca_solteiro->proximoEventoDir = final_S2_passou_solteiro; 

    evt_2_traicao->proximoEventoEsq = evt_3_Alice_descobre;     
    evt_2_traicao->proximoEventoDir = final_S2_passou_namorando; 

    // --- CONEXÕES PÓS-SEMESTRE 2 (Raízes do S3) ---
    final_S2_reprovou_aed1->proximoEventoEsq = evt_3_raiz_bem;
    final_S2_reprovou_aed1->proximoEventoDir = evt_3_raiz_bem;
    
    final_S2_passou_namorando->proximoEventoEsq = evt_3_raiz_bem;
    final_S2_passou_namorando->proximoEventoDir = evt_3_raiz_bem;
    
    final_S2_passou_solteiro->proximoEventoEsq = evt_3_raiz_bem;
    final_S2_passou_solteiro->proximoEventoDir = evt_3_raiz_bem;

    // --- CONEXÕES SEMESTRE 3 ---
    evt_3_raiz_bem->proximoEventoEsq = evt_4_raiz_namorando; 
    evt_3_raiz_bem->proximoEventoDir = evt_3_plagio;         

    evt_3_plagio->proximoEventoEsq = evt_4_raiz_namorando; 
    evt_3_plagio->proximoEventoDir = evt_3_plagio_descoberto;  

    evt_3_plagio_descoberto->proximoEventoEsq = evt_3_reuniao_chefe;
    evt_3_plagio_descoberto->proximoEventoDir = evt_3_reuniao_chefe;

    evt_3_reuniao_chefe->proximoEventoEsq = evt_4_raiz_processo; 
    evt_3_reuniao_chefe->proximoEventoDir = final_formado_sozinho;

    evt_3_Alice_descobre->proximoEventoEsq = evt_4_raiz_termino; 
    evt_3_Alice_descobre->proximoEventoDir = evt_4_raiz_termino; 

    // --- CONEXÕES SEMESTRE 4 (FINAIS) ---
    evt_4_raiz_processo->proximoEventoEsq = final_formado_festeiro; 
    evt_4_raiz_processo->proximoEventoDir = final_jubilado;         

    evt_4_raiz_termino->proximoEventoEsq = final_formado_honras;  
    evt_4_raiz_termino->proximoEventoDir = final_surto_saude;     

    evt_4_raiz_namorando->proximoEventoEsq = final_formado_honras; 
    evt_4_raiz_namorando->proximoEventoDir = final_transferencia;  
}



// ###   LÓGICA DO JOGO (processar, atualizar, desenhar)          ###
void Game::deletarArvore(Evento* no, std::set<Evento*>& visitados) {
    if (no == nullptr || visitados.count(no)) {
        return;
    }
    deletarArvore(no->proximoEventoEsq, visitados); 
    deletarArvore(no->proximoEventoDir, visitados); 
    
    visitados.insert(no);
    delete no;                           
}

// Processa um único evento vindo do main.cpp
void Game::processarEvento(sf::Event evento) {
    if (evento.type == sf::Event::Closed) {
        janela.close();
    }

    if (evento.type == sf::Event::MouseButtonPressed) {
        if (evento.mouseButton.button == sf::Mouse::Left) {
            if (status != GameStatus::RUNNING) return;
            
            sf::Vector2f mousePos(evento.mouseButton.x, evento.mouseButton.y);
            // PASSA O TIPO DE EVENTO ATUAL PARA O UIMANAGER
            int escolha = ui->handleClick(mousePos); // 1=Esq, 2=Dir, 3=Proximo

            if (escolha != 0) {
                aplicarEscolha(escolha);
            }
        }
    }

    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Space) {
             if (status == GameStatus::RUNNING && eventoAtual->tipo == EventoTipo::FINAL_SEMESTRE) {
                 // Se for um final de semestre, o Espaço age como o botão "3" (de próximo)
                 aplicarEscolha(3);
             }
        }
    }
}


void Game::aplicarEscolha(int escolha) {
    
    // Se a escolha for "Próximo Semestre" (botão 3 ou Espaço)
    if (escolha == 3 && eventoAtual->tipo == EventoTipo::FINAL_SEMESTRE) {
        player.avancarSemestre();
        // Avança para o próximo semestre (ambos os filhos apontam para o mesmo lugar)
        eventoAtual = eventoAtual->proximoEventoEsq; 
        
        if (eventoAtual == nullptr) { // Bug na história (precisa de continuação)
            std::cerr << "ERRO: Nó de final de semestre não está ligado ao próximo semestre." << std::endl;
            status = GameStatus::HARD_END;
            return;
        }
        
        // Atualiza a UI com o novo evento e continua
        ui->update(player, eventoAtual, texturas[eventoAtual->idBackground]);
        return;
    }

    // Se for uma escolha narrativa normal (1 ou 2)
    if (escolha == 1) {
        player.aplicarImpacto(eventoAtual->impactoEsq);
        eventoAtual = eventoAtual->proximoEventoEsq;
    } else if (escolha == 2) {
        player.aplicarImpacto(eventoAtual->impactoDir);
        eventoAtual = eventoAtual->proximoEventoDir;
    } else {
        return; // Clique inválido 
    }

    // Verifica o estado depois de avançar
    if (eventoAtual == nullptr) {
        std::cerr << "ERRO: História incompleta! Nó nulo encontrado." << std::endl;
        status = GameStatus::HARD_END;
        return;
    } 
    
    // Verifica se o novo nó é um final
    if (eventoAtual->tipo == EventoTipo::FINAL_JOGO) {
        status = GameStatus::HARD_END;
    }
    
    // Verifica se os stats levaram a um game over
    if (player.isGameOver()) {
        status = GameStatus::HARD_END;
    }
    
    // ATUALIZA A UI com o novo evento (seja narrativo ou um final de semestre)
    // Isso faz o texto "Fim do Semestre 1..." aparecer na caixa.
    ui->update(player, eventoAtual, texturas[eventoAtual->idBackground]);
}


std::string Game::getFinalMessage() const {
    if (player.isGameOver()) {
        return "Sua Saude Mental chegou a 0 ou seu IRA foi para o buraco.\n\nFIM DE JOGO.";
    }
    if (eventoAtual) {
        return eventoAtual->descricao; // Retorna o texto do nó final
    }
    return "Fim de jogo.";
}


void Game::atualizar() {
    
}

// desenha todos os elementos na tela a cada chamada
void Game::desenhar() {
    ui->draw();
}