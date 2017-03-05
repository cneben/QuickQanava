#version 150 core

attribute highp vec2 vertex;
attribute highp vec2 previous;
attribute highp vec2 next;

out vec2 pp;
out vec2 np;

void main() {
    pp = previous;
    np = next;
    gl_Position = vec4( vertex, 0., 1. );
}
