#pragma once

// OpenGL 関連
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 標準ライブラリ
#include <iostream>

///
/// ウィンドウ関連の処理
///
class Window
{
  /// ウィンドウの識別子
  GLFWwindow* const window;

public:

  ///
  /// コンストラクタ
  ///
  /// @param width ウィンドウの幅
  /// @param height ウィンドウの高さ
  /// @param title ウィンドウのタイトル
  /// 
  Window(int width = 640, int height = 480, const char* title = "GLFW Window")
    : window(glfwCreateWindow(width, height, title, NULL, NULL))
  {
    // ウィンドウが作成できなかったら
    if (window == NULL)
    {
      // エラーメッセージを出して
      std::cerr << "Can't create GLFW window." << std::endl;

      // 終了する
      exit(1);
    }

    // 現在のウィンドウを処理対象にする
    glfwMakeContextCurrent(window);

    // GLEW を初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
      // GLEW の初期化に失敗したらエラーメッセージを出して
      std::cerr << "Can't initialize GLEW" << std::endl;

      // 終了する
      exit(1);
    }

    // 表示はディスプレイのリフレッシュレートに同期させる
    glfwSwapInterval(1);

    // ウィンドウのサイズ変更時に呼び出す処理を登録する
    glfwSetWindowSizeCallback(window, resize);

    // 開いたウィンドウに初期設定を適用する
    resize(window, width, height);
  }

  ///
  /// デストラクタ
  ///
  virtual ~Window()
  {
    // ウィンドウを破棄する
    glfwDestroyWindow(window);
  }

  ///
  /// 描画ループの継続判定
  /// 
  /// @return 描画を継続する場合は true
  ///
  explicit operator bool()
  {
    // イベントを取り出す
    glfwWaitEvents();

    // ウィンドウを閉じる必要がなければ true を返す
    return !glfwWindowShouldClose(window);
  }

  ///
  /// ダブルバッファリング
  ///
  void swapBuffers() const
  {
    // カラーバッファを入れ替える
    glfwSwapBuffers(window);
  }

  ///
  /// ウィンドウのサイズ変更時の処理
  /// 
  /// @param window サイズ変更の対象のウィンドウの識別子
  /// @param width サイズ変更後のウィンドウの幅
  /// @param height サイズ変更後のウィンドウの高さ
  ///
  static void resize(GLFWwindow* window, int width, int height)
  {
    // フレームバッファの大きさを得る
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    // フレームバッファ全体をビューポートにする
    glViewport(0, 0, fbWidth, fbHeight);
  }
};
