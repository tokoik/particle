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

// シェーダの読み込み処理
#include "shader.h"

// 図形関連の処理
#include "Object.h"

// 粒子数
const auto PARTICLE_COUNT{ 10000 };

// 乱数
#include <random>

// GLM 関連
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

// 標準ライブラリ
#include <iostream>

///
/// 点群データの作成
///
/// @param[in] object 点群データを作成する対象のオブジェクト
/// @param[in] scale 点群データのスケール
/// @param[in] sphere 球状に配置する場合は true、立方体状に配置する場合は false
///
void generateParticles(const Object& object, float scale, bool sphere = true)
{
  // 乱数生成器を初期化する
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());

  // 頂点バッファオブジェクトをバインドして頂点データをマップする
  glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
  const auto position{ static_cast<glm::vec4*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) };

  // 球状に配置するか立方体状に配置するか
  if (sphere)
  {
    // 球状に配置する場合は、0.0f から 1.0f の範囲の一様乱数を生成する
    std::uniform_real_distribution<GLfloat> dist(0.0f, 1.0f);

    // 粒子の初期位置を設定する
    for (auto i = 0; i < object.count; ++i)
    {
      const float u{ dist(engine) };
      const float v{ dist(engine) * 2.0f - 1.0f };
      const float w{ dist(engine) };
      const float r{ cbrt(w) * scale };
      const float s{ sqrt(1.0f - v * v) * r };
      const float t{ u * 6.2831853f };

      // 粒子を球状に配置する
      position[i] = { s * cos(t), s * sin(t), r * v, 1.0f };
    }
  }
  else
  {
    // 立方体状に配置する場合は、-0.5f * scale から 0.5f * scale の範囲の一様乱数を生成する
    std::uniform_real_distribution<GLfloat> dist(-0.5f * scale, 0.5f * scale);

    // 粒子の初期位置を設定する
    for (auto i = 0; i < object.count; ++i)
    {
      // 粒子を立方体状に配置する
      position[i] = { dist(engine), dist(engine), dist(engine), 1.0f };
    }
  }

  // バッファオブジェクトの結合を解除する
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

  // プログラムオブジェクトを作成する
  const auto program{ loadProgram("point.vert", "point.frag") };

  // プログラムオブジェクトが作成できなかったら
  if (program == 0)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create program object." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // uniform 変数 mc の場所を取得する
  const auto mcLoc{ glGetUniformLocation(program, "mc") };

  // 図形を作成する
  Object object(PARTICLE_COUNT);
  generateParticles(object, 1.0f);

  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // プログラムオブジェクトを指定する
    glUseProgram(program);

    // ビュー変換行列を設定する
    const auto view{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) };

    // 投影変換行列を設定する
    const auto projection{ glm::perspective(glm::radians(60.0f), window.getAspect(), 1.0f, 10.0f) };

    // uniform 変数 mc に値を設定する
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, glm::value_ptr(projection * view));

    // 図形を指定する
    glBindVertexArray(object.vao);

    // 図形を描画する
    glDrawArrays(GL_POINTS, 0, object.count);

    // OpenGL のエラーが発生していないかチェックする
    errorcheck();

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
