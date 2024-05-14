#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D Normal_Smoothness;
uniform sampler2D rboDepth;

uniform mat4 uProjectionMatrix;
uniform mat4 uInverseProjectionMatrix;
uniform mat4 uInverseViewMatrix;

uniform vec3 uLightColor;

uniform vec3 samples[64];
uniform vec2 uNoiseScale;
uniform sampler2D texNoise;

uniform float radius;



in highp vec2 TexCoords;

out vec4 FragColor;



vec3 ScreenToView(vec2 screenPos, float depth)
{
    // Convert screen position to clip space
    vec4 clipSpacePosition = vec4(screenPos * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    // Convert clip space position to view space
    vec4 viewSpacePosition = uInverseProjectionMatrix * clipSpacePosition;

    return viewSpacePosition.xyz;
}
mat3 transpose(mat3 m) {
    return mat3(m[0][0], m[1][0], m[2][0],
                m[0][1], m[1][1], m[2][1],
                m[0][2], m[1][2], m[2][2]);
}


void main() {
  vec4 Normal_Smoothness = texture(Normal_Smoothness, TexCoords);


  vec3 normal_WS = Normal_Smoothness.rgb;// 将法线从切线空间转换到世界空间
  
  float depth = texture(rboDepth, TexCoords).r;


  vec3 normal_VS = transpose(mat3(uInverseViewMatrix)) * normal_WS; // 将法线从世界空间转换到视图空间
  vec3 vFragPos_VS = ScreenToView(TexCoords, depth);
  vec3 randomVec = texture(texNoise,uNoiseScale*TexCoords).xyz;

  vec3 tangent = normalize(randomVec - normal_VS * dot(randomVec, normal_VS));
  vec3 bitangent = cross(normal_VS, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal_VS);

  float occlusion = 0.0;
  for(int i = 0; i < 64; ++i)
  {
      // 获取样本位置
      vec3 sample = TBN * samples[i]; // 切线->观察空间
      sample = vFragPos_VS + sample * radius; 
      vec4 offset = vec4(sample, 1.0);
      offset = uProjectionMatrix * offset; // 观察->裁剪空间
      offset.xyz /= offset.w; // 透视划分
      offset.xyz = offset.xyz * 0.5 + 0.5; // 变换到0.0 - 1.0的值域
      float sampleDepth = -texture(rboDepth, offset.xy).w;
      float rangeCheck = smoothstep(0.0, 1.0, radius / abs(vFragPos_VS.z - sampleDepth));
      occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;   

  }
  occlusion = 1.0 - (occlusion / 64);
  FragColor = vec4(occlusion,0,0,0);  

  

}

