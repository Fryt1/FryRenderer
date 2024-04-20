#ifdef GL_ES
precision mediump float;
#endif

uniform float uLigIntensity;
uniform vec3 uLightColor;

out FragColor

void main() {
    
  //gl_FragColor = vec4(1,1,1, 1.0);
  FragColor = vec4(uLightColor, 1.0);
}