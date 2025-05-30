///
/// パーティクル処理のサンプルコード
///
/// @file main.cpp
/// @author Kohe Tokoi
/// @date 2025/5/24
///

// Windows の OpenGL ライブラリをリンクする
#pragma comment(lib, "opengl32.lib")

// ウィンドウ関連の処理
#include "Window.h"

// OpenGL のエラーチェック
#include "errorcheck.h"

// 標準ライブラリ
#include <iostream>

///
/// メインプログラム
///
/// @return プログラムが正常に終了した場合は 0
///
auto main() -> int
{
  // GLFW を初期化する
  if (glfwInit() == GL_FALSE)
  {
    // GLFW の初期化に失敗したのでエラーメッセージを出して
    std::cerr << "Can't initialize GLFW." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // プログラム終了時の処理を登録する
  atexit(glfwTerminate);

  // ウィンドウを作成する
  Window window;

  // ウィンドウが作成できなかったら
  if (window.get() == nullptr)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create GLFW window." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // GLEW の初期化時にすべての API のエントリポイントを見つけるようにして
  glewExperimental = GL_TRUE;

  // GLEW を初期化する
  if (glewInit() != GLEW_OK)
  {
    // GLEW の初期化に失敗したのでエラーメッセージを出して
    std::cerr << "Can't initialize GLEW." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // OpenGL のエラーが発生していないかチェックする
    errorcheck();

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
