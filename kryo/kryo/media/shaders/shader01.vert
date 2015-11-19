varying vec4 light;

void main()
{
    light = gl_Color;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
    gl_FogFragCoord = gl_Position.z;
}
