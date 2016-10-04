#version 150 core

uniform lowp float opacity;
uniform lowp vec4 color;
uniform highp float width;
smooth in float dist;

void main() {
    lowp float feather = ( width < 1.00001 ? width :
                                             1.0 - smoothstep( 0.65, 1.0, abs(dist) ) );
    gl_FragColor = color * opacity * feather;
}

