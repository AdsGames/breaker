/*
 * Vec2
 * Allan Legemaate
 * 07/01/2017
 * A tiny 2d vector
 */
#ifndef VEC2_H
#define VEC2_H

class vec2{
  public:
    vec2();
    vec2( float new_x, float new_y);
    vec2( float x_y);

    float x;
    float y;
};

#endif // VEC2_H
