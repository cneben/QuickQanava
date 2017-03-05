#version 150

uniform highp mat4 combmatrix;
uniform highp mat4 mvmatrix;
uniform highp float width;

layout (lines) in;
layout (triangle_strip, max_vertices=4) out;

in vec2 pp[2];
in vec2 np[2];

smooth out float dist;

void main()
{
    vec4 p0 = vec4( pp[0], 0, 1 );
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec4 p3 = vec4( np[1], 0, 1 );

    vec4 line0 = normalize( p1 - p0 );
    vec4 line1 = normalize( p2 - p1 );
    vec4 line2 = normalize( p3 - p2 );
    vec4 normal = vec4(-line1.y, line1.x, 1., 0.);  /* normal is already normalized since lines are */
    float interDist = 1.0;
    float w = max( width, 1.0 );
    // interDist = 0.;              /* Can't disable any AA for pure horizontal or vertical lines, or mitter join will be half antialised in closed polylines... */
    float wd2 = w / 2.0;            /* wd2=width divided by two     */

    vec4 t1 = normalize( line0 + line1 );   /* t=tangent of lines l0 and l1 */
    vec4 nt1 = normal* wd2;                      /* nt=normal to tangent         */
    bool bevel = false;
    if ( p0 != p1 ) {  /* For the first point in line strip, first point = previous, use line normal instead of line tangent normal */
        nt1 = vec4( -t1.y, t1.x, 0., 0. );
        float nt1l = 1.0 / dot( normal, nt1 ) * wd2;  /* nt1l= normal to tangent length=mitter join length */
        if ( nt1l > 4. * wd2 )
            bevel = true;                   /* Detect infinite mitter join */
        nt1 *= nt1l;
    }

    vec4 t2 = normalize( line1 + line2 );   /* t2=tangent of lines l1 and l2 */
    vec4 nt2 = normal * wd2;                      /* nt2=normal to tangent         */
    if ( p2 != p3 ) {  /* For the last point in line strip, second point = next, use line normal instead of line tangent normal */
        nt2 = vec4( -t2.y, t2.x, 0., 0. );
        float nt2l = 1.0 / dot( normal, nt2 ) * wd2;  /* nt2l= normal to tangent length=mitter join length */
        if ( nt2l > 4. * wd2 )              /* Detect infinite mitter join */
            bevel = true;
        nt2 *= nt2l;
    }
    if ( bevel ) {
        nt1 = normal * wd2;     /* In bevel join, use standard line segment normal  */
        nt2 = normal * wd2;     /* Scale 1.0 length normal to half line width       */
    }   /* Otherwise use pre-generated mitter join                 */

    /* Emit geometry projected in the full Qt combined matrix */
    gl_Position = combmatrix * ( p1 + nt1 );
    dist = interDist;
    EmitVertex();

    gl_Position = combmatrix * ( p1 - nt1 );
    dist = -interDist;
    EmitVertex();

    gl_Position = combmatrix * ( p2 + nt2 );
    dist = interDist;
    EmitVertex();

    gl_Position = combmatrix * ( p2 - nt2 );
    dist = -interDist;
    EmitVertex();
    EndPrimitive();
}
