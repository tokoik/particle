# particle - パーティクルシミュレーション

## 1. 概要

本プログラムは、OpenGL のポイントスプライトおよびシェーダを用いて多数のパーティクルを高速にアニメーション・レンダリングするサンプルプログラムです。

- 移行元ブログ記事:
  - [パーティクルシステムの描画 - 床井研究室](https://tokoik.github.io/blog/2025-07-27.html)

## 2. 対応環境

- **Windows**: Visual Studio 2019 / 2022 / 2026 (CMake 経由で GLFW を自動構成)
- **macOS**: Xcode (GLFW を自動ダウンロード、OpenGL Framework を使用)
- **Ubuntu Linux**: GCC / Make (システム標準の libglfw3-dev, libgl1-mesa-dev を使用)

## 3. ビルド手順

### Windows (Visual Studio)

```pwsh
cmake -B build -S .
cmake --build build --config Release
```

### macOS (Xcode)

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

### Ubuntu Linux (Makefile)

```bash
sudo apt-get update
sudo apt-get install -y libglfw3-dev libgl1-mesa-dev
cmake -B build -S .
cmake --build build
```

## 4. 起動方法

ビルド完了後、生成された実行ファイルを実行します。

- **Windows**: `build/Release/particle.exe`
- **macOS**: `build/Release/particle.app`
- **Linux**: `build/particle`

## 5. 操作方法

- **マウスドラッグ**: 視点の回転
- **[q] / [Q] / [ESC]**: プログラムの終了

## 6. プログラムの解説

質点群の力学計算を行って VBO に格納し、頂点シェーダおよびフラグメントシェーダを通じてパーティクルのテクスチャマッピングやブレンディングを行って描画します。
