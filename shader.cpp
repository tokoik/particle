///
/// シェーダ関連の処理の実装
///
/// @file shader.cpp
/// @author Kohe Tokoi
/// @date 2025/5/28
///
#include "shader.h"

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <sstream>

///
/// シェーダオブジェクトのコンパイル結果を表示する
///
/// @param[in] shader シェーダオブジェクト名
/// @param[in] str コンパイルエラーが発生した場所を示す文字列
/// @return コンパイル結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printShaderInfoLog(GLuint shader, const std::string& str) -> GLboolean
{
  // コンパイル結果を取得する
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;

  // シェーダのコンパイル時のログの長さを取得する
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

  // シェーダのコンパイル時のログがあるなら
  if (bufSize > 1)
  {
    // ログの内容を取得する
    std::string infoLog(bufSize, '\0');
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << infoLog << std::endl;
  }

  // コンパイル結果を返す
  return status;
}

///
/// プログラムオブジェクトのリンク結果を表示する
///
/// @param[in] program プログラムオブジェクト名
/// @return リンク結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printProgramInfoLog(GLuint program) -> GLboolean
{
  // リンク結果を取得する
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

  // シェーダのリンク時のログの長さを取得する
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

  // シェーダのリンク時のログがあるなら
  if (bufSize > 1)
  {
    // ログの内容を取得する
    std::string infoLog(bufSize, '\0');
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << infoLog << std::endl;
  }

  // リンク結果を返す
  return status;
}

///
/// シェーダオブジェクトを作成してプログラムオブジェクトに組み込む
///
/// @param program シェーダオブジェクトを組み込むプログラムオブジェクトの名前
/// @param src シェーダのソースプログラムの文字列
/// @param msg メッセージに追加する文字列
/// @param type シェーダの種類（GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER）
/// @return シェーダオブジェクトの作成とプログラムオブジェクトへの組み込みに成功したら true
///
static auto createShader(GLuint program, const std::string& src, const std::string& msg,
  GLenum type) -> bool
{
  // プログラムオブジェクトが作成できていなかったら
  if (program == 0)
  {
    // エラーメッセージを表示して戻る
    std::cerr << "Error: Could not create program object." << std::endl;
    return false;
  }

  // シェーダオブジェクトを作成する
  const auto shader{ glCreateShader(type) };

  // シェーダオブジェクトが作成できなかったら
  if (shader == 0)
  {
    // エラーメッセージを表示して戻る
    std::cerr << "Error: Could not create shader object." << std::endl;
    return false;
  }

  // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
  auto ptr{ src.c_str() };
  glShaderSource(shader, 1, &ptr, nullptr);
  glCompileShader(shader);

  // コンパイル時のメッセージを表示してコンパイル結果を得る
  const auto status{ printShaderInfoLog(shader, msg) != GL_FALSE };

  // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
  if (status) glAttachShader(program, shader);

  // シェーダオブジェクトに削除マークを付ける
  glDeleteShader(shader);

  // コンパイル結果を返す
  return status;
}

///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vmsg バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] fmsg フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
/// @note この関数は、
/// バーテックスシェーダとフラグメントシェーダの両方のソースプログラムを受け取りますが、
/// フラグメントシェーダのソースプログラムは空文字列でも構いません。
/// これは Transform Feedback を使用する場合などで、
/// glEnable(GL_RASTERIZER_DISCARD) としてラスタライザを無効にしたときには、
/// フラグメントシェーダが不要になるためです。
/// なお、バーテックスシェーダのソースプログラムがコンパイルエラーになった場合は、
/// フラグメントシェーダのコンパイルは行いません。
///
auto createProgram(const std::string& vsrc, const std::string& fsrc,
  const std::string& vmsg, const std::string& fmsg) -> GLuint
{
  // バーテックスシェーダのソースの文字列が与えられていなければ
  if (vsrc.empty())
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: No vertex shader specified." << std::endl;
    return 0;
  }

  // 空のプログラムオブジェクトを作成する
  const auto program{ glCreateProgram() };

  // バーテックスシェーダの作成と組み込みに成功したら
  if (createShader(program, vsrc, vmsg, GL_VERTEX_SHADER))
  {
    // フラグメントシェーダがないかフラグメントシェーダの作成と組み込みに成功したら
    if (fsrc.empty() || createShader(program, fsrc, fmsg, GL_FRAGMENT_SHADER))
    {
      // プログラムオブジェクトをリンクして
      glLinkProgram(program);

      // エラーがなければプログラムオブジェクトを返す
      if (printProgramInfoLog(program)) return program;
    }
  }

  // エラーのときはプログラムオブジェクトを削除して 0 を返す
  glDeleteProgram(program);
  return 0;
}

///
/// シェーダのソースファイルを読み込む
///
/// @param[in] name シェーダのソースファイル名
/// @return 読み込んだソースファイルの文字列、読み込めなければ ""
///
static auto readShaderSource(const std::string& name) -> std::string
{
  // ソースファイルを開く
  std::ifstream file(name, std::ios::binary);

  // ソースファイルが開けなかったら
  if (file.fail())
  {
    // エラーメッセージを表示してから読み込み失敗として戻る
    std::cerr << "Error: Can't open source file: " << name << std::endl;
    return "";
  }

  // ファイル全体を読み込む
  std::stringstream buffer;
  buffer << file.rdbuf();

  // 読み込みエラーが起きていたら
  if (file.fail())
  {
    // エラーメッセージを表示してから読み込み失敗として戻る
    std::cerr << "Error: Can't read source file: " << name << std::endl;
    return "";
  }

  // 読み込んだ文字列を返す（file はデストラクタで自動的に閉じられる）
  return buffer.str();
}

///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
auto loadProgram(const std::string& vert, const std::string& frag) -> GLuint
{
  // バーテックスシェーダのソースファイル名が与えられていなければ
  if (vert.empty())
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: No shader source file specified." << std::endl;
    return 0;
  }

  // バーテックスシェーダのソースファイルを読み込む
  const std::string vsrc{ readShaderSource(vert) };

  // バーテックスシェーダのソースファイルが読み込めなければ 0 を返す
  if (vsrc.empty()) return 0;

  // フラグメントシェーダのソースの文字列（初期値は empty）
  std::string fsrc;

  // フラグメントシェーダのソースファイル名が指定されていたら
  if (!frag.empty())
  {
    // フラグメントシェーダのソースファイルを読み込んで
    fsrc = readShaderSource(frag);

    // 読み込めていなかったら 0 を返す
    if (fsrc.empty()) return 0;
  }

  // プログラムオブジェクトを作成する
  return createProgram(vsrc, fsrc, vert, frag);
}

///
/// コンピュートシェーダのソースプログラムの文字列を読み込んでプログラムオブジェクトを作成する
///
/// @param[in] csrc コンピュートシェーダのソースプログラムの文字列
/// @param[in] cmsg コンピュートシェーダのコンパイル時のメッセージに追加する文字列
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
auto createCompute(const std::string& csrc, const std::string& cmsg) -> GLuint
{
  // 空のプログラムオブジェクトを作成する
  const auto program{ glCreateProgram() };

  // コンピュートシェーダの作成と組み込みに成功したら
  if (createShader(program, csrc, cmsg, GL_COMPUTE_SHADER))
  {
    // プログラムオブジェクトをリンクして
    glLinkProgram(program);

    // エラーがなければプログラムオブジェクトを返す
    if (printProgramInfoLog(program)) return program;
  }

  // エラーのときはプログラムオブジェクトを削除して 0 を返す
  glDeleteProgram(program);
  return 0;
}

///
/// コンピュートシェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] comp コンピュートシェーダのソースファイル名
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
auto loadCompute(const std::string& comp) -> GLuint
{
  // コンピュートシェーダのソースファイルを読み込んで
  std::string csrc{ readShaderSource(comp) };

  // ソースファイルが読めたらプログラムオブジェクトを作成する
  if (!csrc.empty()) return createCompute(csrc, comp);

  // ソースファイルが読めなかったので 0 を返す
  return 0;
}
