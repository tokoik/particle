#pragma once

///
/// デバッグ用関数
///
/// @file errorcheck.h
/// @author Kohe Tokoi
/// @date 2025/5/29
///

// ここの関数はデバッグビルドの時だけ実体を宣言しリリースビルドでは実体を持たない
#if defined(_DEBUG)

///
/// OpenGL のエラーをチェックしてエラーがあればエラーメッセージを出力する
///
/// @param[in] name エラー発生時に標準エラー出力に出力するファイル名などの文字列、nullptr なら何も出力しない
/// @oaram[in] line エラー発生時に標準エラー出力に出力する行番号などの整数値
///
extern auto _errorcheck(const char* name, unsigned int line) -> void;

///
/// OpenGL のエラーの発生を検知したときにソースファイル名と行番号を付加したエラーメッセージを表示する
///
/// @def errorcheck()
///
/// @note このマクロを置いた場所（より前）で OpenGL のエラーが発生していた時に、
/// このマクロを置いたソースファイル名と行番号を出力する。
/// リリースビルド時には無視する。
///
#  define errorcheck() _errorcheck(__FILE__, __LINE__)
#else
#  define errorcheck()
#endif
