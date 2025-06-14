#pragma once

///
/// シェーダ関連の処理
///
/// @file shader.h
/// @author Kohe Tokoi
/// @date 2025/5/28
///

// シェーダ関連の宣言は gl.h に含まれていないので glew.h を使う
#include <GL/glew.h>

// 標準ライブラリ
#include <string>

///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vert バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] frag フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
extern auto createProgram(const std::string& vsrc, const std::string& fsrc,
  const std::string& vert = "vertex shader", const std::string& frag = "fragment shader") -> GLuint;

///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
extern auto loadProgram(const std::string& vert, const std::string& frag) -> GLuint;

///
/// コンピュートシェーダのソースプログラムの文字列を読み込んでプログラムオブジェクトを作成する
///
/// @param[in] csrc コンピュートシェーダのソースプログラムの文字列
/// @param[in] cmsg コンピュートシェーダのコンパイル時のメッセージに追加する文字列
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
extern auto createCompute(const std::string& csrc, const std::string& cmsg) -> GLuint;

///
/// コンピュートシェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] comp コンピュートシェーダのソースファイル名
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
extern auto loadCompute(const std::string& comp) -> GLuint;
