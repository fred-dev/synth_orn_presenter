#version 150

uniform sampler2DRect tex0;
uniform float rand;
uniform int range;

in vec2 varyingtexcoord;
out vec4 outputColor;


void main (void)
{
    
    vec2 texCoord = vec2(varyingtexcoord.x , varyingtexcoord.y);
    vec4 col = texture(tex0,texCoord);
    
    outputColor.rgba = col.rgba;
}
