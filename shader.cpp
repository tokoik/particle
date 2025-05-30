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
#include <vector>

///
/// シェーダオブジェクトのコンパイル結果を表示する
///
/// @param[in] shader シェーダオブジェクト名
/// @param[in] str コンパイルエラーが発生した場所を示す文字列
/// @return コンパイル結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printShaderInfoLog(GLuint shader, const char* str) -> GLboolean
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
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << &infoLog[0] << std::endl;
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
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << &infoLog[0] << std::endl;
  }

  // リンク結果を返す
  return status;
}

///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vert バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] frag フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
/// @note この関数は、
/// バーテックスシェーダとフラグメントシェーダの両方のソースプログラムを受け取りますが、
/// フラグメントシェーダのソースプログラムは nullptr でも構いません。
/// これは Transform Feedback を使用する場合などで、
/// glEnable(GL_RASTERIZER_DISCARD) としてラスタライザを無効にしたときには、
/// フラグメントシェーダが不要になるためです。
/// なお、バーテックスシェーダのソースプログラムがエラーになった場合も、
/// エラーチェックのためにフラグメントシェーダはコンパイルするようにしています。
/// したがって、どちらかのソースプログラムでエラーが発生したことは、
/// リンク時に検出して報告することになります。
///
auto createProgram(const char* vsrc, const char* fsrc,
  const char* vert, const char* frag) -> GLuint
{
  // バーテックスシェーダのソースの文字列が与えられていなければ
  if (vsrc == nullptr)
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: Source file not specified." << std::endl;
    return 0;
  }

  // 空のプログラムオブジェクトを作成する
  const GLuint program{ glCreateProgram() };

  // プログラムオブジェクトが作成できなかったら
  if (program == 0)
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: Could not create program object." << std::endl;
    return 0;
  }

  // バーテックスシェーダのシェーダオブジェクトを作成する
  const auto vertShader{ glCreateShader(GL_VERTEX_SHADER) };

  // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
  glShaderSource(vertShader, 1, &vsrc, nullptr);
  glCompileShader(vertShader);

  // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
  if (printShaderInfoLog(vertShader, vert)) glAttachShader(program, vertShader);

  // シェーダオブジェクトに削除マークをつけておく
  glDeleteShader(vertShader);

  // フラグメントシェーダのソースの文字列が与えられていれば
  if (fsrc != nullptr)
  {
    // フラグメントシェーダのシェーダオブジェクトを作成する
    const auto fragShader{ glCreateShader(GL_FRAGMENT_SHADER) };

    // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
    glShaderSource(fragShader, 1, &fsrc, nullptr);
    glCompileShader(fragShader);

    // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
    if (printShaderInfoLog(fragShader, frag)) glAttachShader(program, fragShader);

    // シェーダオブジェクトに削除マークをつけておく
    glDeleteShader(fragShader);
  }

  // プログラムオブジェクトをリンクしてエラーがなければプログラムオブジェクトを返す
  glLinkProgram(program);
  if (printProgramInfoLog(program)) return program;

  // エラーのときはプログラムオブジェクトを削除して 0 を返す
  glDeleteProgram(program);
  return 0;
}

///
/// シェーダのソースファイルを読み込む
///
/// @param[in] name シェーダのソースファイル名
/// @param[out] buffer 読み込んだソースファイルのテキスト
/// @return 読み込みに成功したら true、失敗したら false
///
static auto readShaderSource(const char* name,
  std::vector<GLchar>& buffer) -> bool
{
  // ソースファイル名が nullptr だったら読み込み失敗を返す
  if (name == nullptr) return false;

  // ソースファイルを開く
  std::ifstream file(name, std::ios::binary);

  // ソースファイルが開けなかったら
  if (file.fail())
  {
    // エラーメッセージを表示して
    std::cerr << "Error: Can't open source file: " << name << std::endl;

    // 読み込み失敗を返す
    return false;
  }

  // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
  file.seekg(0L, std::ios::end);
  const auto length{ static_cast<GLsizei>(file.tellg()) };

  // ファイルサイズのメモリを確保
  buffer.resize(length + 1);

  // ファイルを先頭から読み込む
  file.seekg(0L, std::ios::beg);
  file.read(buffer.data(), length);

  // 読み込んだ文字列の末尾にヌル文字を追加する
  buffer[length] = '\0';

  // 読み込みが失敗していたら
  if (file.fail())
  {
    // エラーメッセージを表示して
    std::cerr << "Error: Could not read souce file: " << name << std::endl;

    // ファイルを閉じて
    file.close();

    // 読み込み失敗を返す
    return false;
  }

  // 読み込みが成功したらファイルを閉じて
  file.close();

  // 読み込み成功
  return true;
}

///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
auto loadProgram(const char* vert, const char* frag) -> GLuint
{
  // バーテックスシェーダのソースファイルを読み込む
  std::vector<GLchar> vsrc;
  const auto vstat{ readShaderSource(vert, vsrc) };

  // フラグメントシェーダのソースファイルを読み込む
  std::vector<GLchar> fsrc;
  const auto fstat{ readShaderSource(frag, fsrc) };

  // 両方のソースファイルが読み込めたらプログラムオブジェクトを作成する
  return vstat && fstat ? createProgram(vsrc.data(), fsrc.data(), vert, frag) : 0;
}
