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
  const auto particle{ static_cast<Particle*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) };

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
      particle[i].position = { s * cos(t), s * sin(t), r * v, 1.0f };
      particle[i].velocity = { 0.0f, 0.0f, 0.0f };
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
      particle[i].position = { dist(engine), dist(engine), dist(engine), 1.0f };
      particle[i].velocity = { 0.0f, 0.0f, 0.0f };
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

#if defined(IMGUI_VERSION)
  // ImGui のバージョンをチェックする
  IMGUI_CHECKVERSION();

  // ImGui のコンテキストを作成する
  ImGui::CreateContext();

  // ImGui のバックエンドに OpenGL / GLFW を使う
  ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
  ImGui_ImplOpenGL3_Init();

  // ImGui のスタイルを設定する
  auto& io{ ImGui::GetIO() };
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  //ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // 日本語を表示できるメニューフォントを読み込む
  if (!io.Fonts->AddFontFromFileTTF("ImGui/Mplus1-Regular.ttf", 18,
    nullptr, io.Fonts->GetGlyphRangesJapanese()))
  {
    // GLEW の初期化に失敗したのでエラーメッセージを出して
    std::cerr << "Can't load menu font." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // メニューを表示するなら true
  bool showMenu{ true };
#endif

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

  // 粒子の処理を初期化するコンピュートシェーダのプログラムオブジェクトを作成する
  const auto setup{ loadCompute("setup.comp") };

  // プログラムオブジェクトが作成できなかったら
  if (setup == 0)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create setup shader." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // 粒子の衝突を処理するコンピュートシェーダのプログラムオブジェクトを作成する
  const auto collide{ loadCompute("collide.comp") };

  // プログラムオブジェクトが作成できなかったら
  if (collide == 0)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create collide shader." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // 粒子の位置を更新するコンピュートシェーダのプログラムオブジェクトを作成する
  const auto update{ loadCompute("update.comp") };

  // プログラムオブジェクトが作成できなかったら
  if (update == 0)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create update shader." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // 図形の種類
  enum
  {
    Sphere = 0, // 球状
    Box         // 箱状
  };

  // デフォルトの図形は球状
  int shape{ Sphere };

  // 図形のスケール
  GLfloat scale{ 1.0f };

  // 図形を作成する
  Object object(PARTICLE_COUNT);
  generateParticles(object, scale, shape == Sphere);

  //
  // 粒子群の物理パラメータ
  //
  struct Physics
  {
    // 重力
    alignas(16) glm::vec3 gravity;

    // 地面の高さ
    alignas(4) GLfloat floor_height;

    // 地面の反発係数
    alignas(4) GLfloat floor_restitution;

    // 粒子の反発係数
    alignas(4) GLfloat particle_restitution;

    // 粒子の質量
    alignas(4) GLfloat mass;

    // 粒子の半径
    alignas(4) GLfloat radius;

    // 粒子の重なり
    alignas(4) GLfloat overlap;

    // 時間間隔
    alignas(4) GLfloat timestep;
  };

  Physics physics
  {
    // 重力
    { 0.0f, -1.0f, 0.0f },

    // 地面の高さ
    -1.0f,

    // 地面の反発係数
    0.3f,

    // 粒子の反発係数
    0.2f,

    // 粒子の質量
    1.0f,

    // 粒子の半径
    0.1f,

    // 粒子の重なり
    0.0001f,

    // 時間間隔
    1.0f / 60.0f
  };

  // 粒子群の物理パラメータを格納するユニフォームバッファオブジェクト
  GLuint ubo;

  // ユニフォームバッファオブジェクトを作成する
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof physics, &physics, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // バーテックスシェーダで点の大きさを変えられるようにする
  GLfloat distance[] = { 0.0, 0.0, 1.0 };
  glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance);
  glEnable(GL_POINT_SMOOTH);

  // 背景色を指定する
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
#if defined(IMGUI_VERSION)
    // メニューを表示するなら
    if (showMenu)
    {
      // メニューを表示する
      ImGui::SetNextWindowPos(ImVec2(2, 2), ImGuiCond_Once);
      ImGui::SetNextWindowSize(ImVec2(308, 316), ImGuiCond_Once);
      ImGui::Begin(u8"コントロールパネル", &showMenu);
      ImGui::DragFloat3(u8"重力", &physics.gravity[0], 0.01f, -10.0f, 10.0f);
      ImGui::DragFloat(u8"地面の高さ", &physics.floor_height, 0.01f, -10.0f, 10.0f);
      ImGui::DragFloat(u8"地面の反発係数", &physics.floor_restitution, 0.01f, 0.0f, 1.0f);
      ImGui::DragFloat(u8"粒子の反発係数", &physics.particle_restitution, 0.01f, 0.0f, 1.0f);
      ImGui::DragFloat(u8"粒子の質量", &physics.mass, 0.01f, 0.01f, 10.0f);
      ImGui::DragFloat(u8"粒子の半径", &physics.radius, 0.01f, 0.01f, 1.0f);
      ImGui::DragFloat(u8"粒子の重なり", &physics.overlap, 0.0001f, 0.0f, 0.1f);
      ImGui::SeparatorText(u8"図形の設定");
      ImGui::InputFloat(u8"図形のスケール", &scale, 0.1f, 1.0f, "%.2f");
      ImGui::RadioButton(u8"球", &shape, Sphere);
      ImGui::SameLine();
      ImGui::RadioButton(u8"箱", &shape, Box);
      ImGui::SameLine();
      if (ImGui::Button(u8"リセット")) generateParticles(object, scale, shape == Sphere);
      ImGui::End();
    }
    else
    {
      // スペースをタイプしたらメニューを表示する
      showMenu = glfwGetKey(window.get(), GLFW_KEY_SPACE) != GLFW_RELEASE;
    }
#endif

    // 更新処理を行う
    window.update();

    // 点の大きさをウィンドウの高さに合わせる
    glPointSize(static_cast<GLfloat>(window.getSize().y * 0.01));

#if defined(IMGUI_VERSION)
    // ユニフォームバッファオブジェクトに物理パラメータを書き込む
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof physics, &physics);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
#endif

    // シェーダストレージバッファオブジェクトを 0 番の結合ポイントに結合する
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, object.vbo);

    // ユニフォームバッファオブジェクトを 1 番の結合ポイントに結合する
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);

    // 粒子の処理を初期化するコンピュートシェーダを指定する
    glUseProgram(setup);

    // 計算を実行する
    glDispatchCompute(object.count, 1, 1);

    // シェーダストレージバッファオブジェクトへの書き込み完了を待つ
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 粒子の初と津を処理するコンピュートシェーダを指定する
    glUseProgram(collide);

    // 計算を実行する
    glDispatchCompute(object.count, 1, 1);

    // シェーダストレージバッファオブジェクトへの書き込み完了を待つ
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 粒子の位置を更新するコンピュートシェーダを指定する
    glUseProgram(update);

    // 計算を実行する
    glDispatchCompute(object.count, 1, 1);

    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // プログラムオブジェクトを指定する
    glUseProgram(program);

    // モデル変換行列を設定する
    const auto& model{ window.getModel(GLFW_MOUSE_BUTTON_LEFT) };

    // ビュー変換行列を設定する
    const auto view{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) };

    // 投影変換行列を設定する
    const auto projection{ glm::perspective(glm::radians(60.0f), window.getAspect(), 1.0f, 10.0f) };

    // uniform 変数 mc に値を設定する
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, glm::value_ptr(projection * view * model));

    // 図形を指定する
    glBindVertexArray(object.vao);

    // 図形を描画する
    glDrawArrays(GL_POINTS, 0, object.count);

    // OpenGL のエラーが発生していないかチェックする
    errorcheck();

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }

#if defined(IMGUI_VERSION)
  // ImGui のバックエンドをシャットダウンする
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();

  // ImGui のコンテキストを破棄する
  ImGui::DestroyContext();
#endif
}
