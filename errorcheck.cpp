///
/// デバッグ用関数の定義
///
/// @file errorcheck.cpp
/// @author Kohe Tokoi
/// @date 2025/5/29
///
#include "errorcheck.h"

// ここの関数はデバッグビルドの時だけ定義する
#if defined(_DEBUG)

// OpenGL の関数や定数の宣言
#include <GL/glew.h>

// エラーメッセージの出力用
#include <iostream>

///
/// OpenGL のエラーをチェックしてエラーがあればエラーメッセージを出力する
///
/// @param[in] name エラー発生時に標準エラー出力に出力するファイル名などの文字列、nullptr なら何も出力しない
/// @oaram[in] line エラー発生時に標準エラー出力に出力する行番号などの整数値
///
auto _errorcheck(const char* name, unsigned int line) -> void
{
  const GLenum error{ glGetError() };

  if (error != GL_NO_ERROR)
  {
    if (name != nullptr && *name != '\0')
    {
      std::cerr << name;
      if (line > 0) std::cerr << " (" << line << ")";
      std::cerr << ": ";
    }

    switch (error)
    {
    case GL_INVALID_ENUM:
      std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;
      break;
    case GL_INVALID_VALUE:
      std::cerr << "A numeric argument is out of range" << std::endl;
      break;
    case GL_INVALID_OPERATION:
      std::cerr << "The specified operation is not allowed in the current state" << std::endl;
      break;
    case GL_OUT_OF_MEMORY:
      std::cerr << "There is not enough memory left to execute the command" << std::endl;
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr << "The specified operation is not allowed current frame buffer" << std::endl;
      break;
    default:
      std::cerr << "An OpenGL error has occured: " << std::hex << std::showbase << error << std::endl;
      break;
    }
  }
}
#endif
