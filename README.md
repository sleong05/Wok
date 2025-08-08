# ♟️ WOK Chess Engine

WOK is a fast, lightweight chess engine written in modern C++, supporting both native desktop and WebAssembly (WASM) builds.

## 🌐 Try It Out

Play against the engine in your browser: [Live Demo](https://yourdomain.com)  
🖥️ **Note**: The engine runs slower in-browser due to WebAssembly limitations (limited threading, memory constraints, and reduced SIMD support).

---

## 🧩 Versions

### 🖥️ Native Desktop Version (SFML)
- Built with C++ and [SFML](https://www.sfml-dev.org/) for graphics and input.
- Offers a fully featured offline GUI.
- Fastest performance and ideal for testing engine strength.

### 🌍 Browser Version (WASM)
- Compiled with [Emscripten](https://emscripten.org/) to WebAssembly.
- Integrated with [chessboard.js](https://github.com/oakmac/chessboardjs) for UI and [chess.js](https://github.com/jhlywa/chess.js) for move legality and game state.
- Lightweight UI with animated moves, highlights, and end-game detection.
- Fallback support: if the engine crashes, a random move is made instead.

---

## 🚀 Features

- ✅ Minimax with alpha-beta pruning
- ✅ Iterative deepening
- ✅ Quiescence search
- ✅ Transposition table with Zobrist hashing
- ✅ Multi-Threading

---

## 🛠️ Build Instructions

### Native (SFML) Build

**Requirements:**
- C++17 or later
- [SFML 2.5+](https://www.sfml-dev.org/)

```bash
make
./wok
