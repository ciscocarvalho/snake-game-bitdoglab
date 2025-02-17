<div align="center">
    <h1>🐍 Snake Game for the BitDogLab board 🎮</h1>
    <p>
        <a href="./docs/README.pt-br.md">🇧🇷 Português (Brasil)</a> | <a href="#overview">Overview</a> | <a href="#features">Features</a> | <a href="#how-to-play">How to Play</a> | <a href="#installation">Installation</a> | <a href="#contributing">Contributing</a> | <a href="#license">License</a>
    </p>
</div>

---

🌐 **Language Versions**: Looking for the Portuguese version? [Click here](./docs/README.pt-br.md).

---

## 📖 Overview

This project is an implementation of the classic **Snake Game** for the [BitDogLab](https://github.com/BitDogLab/BitDogLab) board. The game leverages the board's hardware capabilities, including an OLED display, NeoPixel LED matrix, buzzer, and joystick, to deliver a fun and interactive gaming experience.

The project is based on:
- **[BitDogLab Examples](https://github.com/BitDogLab/BitDogLab-C)**: For hardware integration and peripheral control.
- **[Snake Game in C](https://github.com/ciscocarvalho/snake-game-c)**: A previous implementation of the Snake Game in C.

---

## ✨ Features

- **Interactive Gameplay**: Control the snake using the joystick and collect food to grow.
- **Visual Feedback**:
  - **OLED Display**: Shows game status, and messages (e.g., "YOU WIN" or "YOU LOSE").
  - **NeoPixel LED Matrix**: Displays the snake, food, and game board in vibrant colors.
- **Audio Feedback**:
  - **Buzzer**: Plays sound effects for events like collecting food or game over.
- **Modular Code**: Organized into well-defined modules for easy maintenance and expansion.
- **Cross-Platform Development**: Built using the Raspberry Pi Pico SDK and configured for VS Code.

---

## 🎮 How to Play

1. **Connect the BitDogLab board** to your computer.
2. **Press Button B** to start the game.
3. **Control the snake** using the joystick:
   - Move up, down, left, or right to guide the snake.
4. **Collect food** to grow the snake.
5. **Avoid collisions** with the snake's own body.
6. **Game Over**:
   - If the snake collides with itself, the game ends.
   - Press **Button B** to restart or **Button A** to exit.

---

## 🛠️ Installation

### Prerequisites
- **Hardware**:
  - BitDogLab board.
  - USB cable for power and programming.
- **Software**:
  - [VS Code](https://code.visualstudio.com/).
  - [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk).
  - ARM GCC compiler.

### Steps
1. Clone this repository:
   ```bash
   git clone https://github.com/ciscocarvalho/snake-game-bitdoglab.git
   ```

2. Open the project in VS Code.

3. Configure the Raspberry Pi Pico SDK and toolchain (follow the setup guide).

4. Build the project using the CMake extension.

5. Upload the .uf2 file to the BitDogLab board.

---

## 🤝 Contributing
Contributions are welcome! If you'd like to improve this project, please follow these steps:

1. Fork the repository.

2. Create a new branch (git checkout -b feature/YourFeatureName).

3. Commit your changes (git commit -m 'Add some feature').

4. Push to the branch (git push origin feature/YourFeatureName).

5. Open a pull request.

---

<div align="center"> <p>Made with ❤️ by <a href="https://github.com/ciscocarvalho">Francisco da Costa Carvalho</a></p> </div>
