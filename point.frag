#version 430 core

// 粒子の色
uniform vec4 particle_color;

// フラグメントの色
layout (location = 0) out vec4 color;

void main()
{
  // フラグメントの色を出力する
  color = particle_color;
}
