<div align="center">
    <h1>🐍 Jogo da Cobrinha para a placa BitDogLab 🎮</h1>
    <p>
        <a href="../README.md">🇺🇸 English</a> | <a href="#visão-geral">Visão Geral</a> | <a href="#recursos">Recursos</a> | <a href="#como-jogar">Como Jogar</a> | <a href="#instalação">Instalação</a> | <a href="#contribuindo">Contribuindo</a> | <a href="#licença">Licença</a>
    </p>
</div>

---

🌐 **Versões em Outros Idiomas**: Prefere a versão em inglês? [Clique aqui](../README.md).

---

## 📖 Visão Geral

Este projeto é uma implementação do clássico **Jogo da Cobrinha** para a placa [BitDogLab](https://github.com/BitDogLab/BitDogLab). O jogo aproveita os recursos de hardware da placa, incluindo display OLED, matriz de LEDs NeoPixel, buzzer e joystick, para proporcionar uma experiência de jogo interativa e divertida.

O projeto é baseado em:
- **[Exemplos do BitDogLab](https://github.com/BitDogLab/BitDogLab-C)**: Para integração de hardware e controle de periféricos.
- **[Snake Game em C](https://github.com/ciscocarvalho/snake-game-c)**: Uma implementação anterior do Jogo da Cobrinha em C.

---

## ✨ Recursos

- **Jogabilidade Interativa**: Controle a cobra usando o joystick e colete comida para crescer.
- **Feedback Visual**:
  - **Display OLED**: Exibe o status do jogo e mensagens (por exemplo, "VOCÊ VENCEU" ou "VOCÊ PERDEU").
  - **Matriz de LEDs NeoPixel**: Exibe a cobra, a comida e o tabuleiro do jogo com cores vibrantes.
- **Feedback de Áudio**:
  - **Buzzer**: Reproduz efeitos sonoros para eventos como coletar comida ou fim de jogo.
- **Código Modular**: Organizado em módulos bem definidos para facilitar a manutenção e expansão.
- **Desenvolvimento Multiplataforma**: Construído usando o Raspberry Pi Pico SDK e configurado para o VS Code.

---

## 🎮 Como Jogar

1. **Conecte a placa BitDogLab** ao computador.
2. **Pressione o Botão B** para iniciar o jogo.
3. **Controle a cobra** usando o joystick:
   - Mova para cima, baixo, esquerda ou direita para guiar a cobra.
4. **Colete comida** para fazer a cobra crescer.
5. **Evite colisões** com o próprio corpo da cobra.
6. **Fim de Jogo**:
   - Se a cobra colidir consigo mesma, o jogo termina.
   - Pressione **Botão B** para reiniciar ou **Botão A** para sair.

---

## 🛠️ Instalação

### Pré-requisitos
- **Hardware**:
  - Placa BitDogLab.
  - Cabo USB para alimentação e programação.
- **Software**:
  - [VS Code](https://code.visualstudio.com/).
  - [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk).
  - Compilador ARM GCC.

### Passos
1. Clone este repositório:
   ```bash
   git clone https://github.com/ciscocarvalho/snake-game-bitdoglab.git
   ```

2. Abra o projeto no VS Code.

3. Configure o Raspberry Pi Pico SDK e a toolchain (siga o guia de configuração).

4. Compile o projeto usando a extensão CMake.

5. Carregue o arquivo .uf2 na placa BitDogLab.

---

## 🤝 Contribuindo
Contribuições são bem-vindas! Se você deseja melhorar este projeto, siga estas etapas:

1. Faça um fork do repositório.

2. Crie uma nova branch (git checkout -b feature/NomeDaSuaFeature).

3. Faça commit das suas alterações (git commit -m 'Adiciona nova feature').

4. Envie as alterações para a branch (git push origin feature/NomeDaSuaFeature).

5. Abra um pull request.

---

<div align="center"> <p>Feito com ❤️ por <a href="https://github.com/ciscocarvalho">Francisco da Costa Carvalho</a></p> </div>
