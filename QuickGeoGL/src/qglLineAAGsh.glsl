#version 150 core

uniform highp mat4 combmatrix;
uniform highp mat4 mvmatrix;
uniform highp float width;

layout (lines) in;                              /* Drawing GL_LINES */
layout (triangle_strip, max_vertices=4) out;

smooth out float dist;

void main()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec4 line1 = normalize( p2 - p1 );
    vec4 normal = vec4( -line1.y, line1.x, 1., 0.);   /* normal is already normalized since line1 is ...  */
    float interDist = 1.0;
    float w = width;
    if ( ( abs( normal.x ) < 0.00001 ) ||
         ( abs( normal.y ) < 0.00001 ) ) {
        interDist = 0.;    /* Disable any AA for pure horizontal or vertical lines */
        w = max( width, 1.0 );  /* width can't be below 1, or nothing will be outputed to fragment shader... */
    } else {
        w = max( width, 1.1 );    /* For non horiz/vert lines, artificially increase line width to generate more  */
    }                                   /* antialiased pixels on its border for 1.0 width lines */
    normal *= ( w / 2.0 );  /* Scaled 1.0 length normal to half line width          */

    /* Emit geometry projected in the full Qt combined matrix */
    gl_Position = combmatrix * ( p1 + normal );
    dist = interDist;
    EmitVertex();

    gl_Position = combmatrix * ( p1 - normal );
    dist = -interDist;
    EmitVertex();

    gl_Position = combmatrix * ( p2 + normal );
    dist = interDist;
    EmitVertex();

    gl_Position = combmatrix * ( p2 - normal );
    dist = -interDist;
    EmitVertex();
    EndPrimitive();
}
