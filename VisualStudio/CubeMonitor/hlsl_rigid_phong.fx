// 정적 오브젝트를 출력하기 위한 셰이더 코드.

// -------------------------------------------------------------
// 전역변수
// -------------------------------------------------------------
float4x4 mWorld;
float4x4 mVP;		// 로컬에서 투영공간으로의 좌표변환
float4x4 mWIT;
float4x4 mWVPT;
float3 vEyePos;
float4 vFog;
float4 fogColor = {0.58823f, 0.58823f, 0.58823f, 1}; // RGB(150,150,150)
float shininess = 90;
float4 globalAmbient = {0.2f, 0.2f, 0.2f, 1.0f};


struct Light
{
	float3 dir;				// world space direction
	float3 pos;				// world space position
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float spotInnerCone;	// spot light inner cone (theta) angle
	float spotOuterCone;	// spot light outer cone (phi) angle
	float radius;           // applies to point and spot lights only
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 emissive;
	float4 specular;
	float shininess;
};

Light light;
Material material;



// ------------------------------------------------------------
// 텍스처
// ------------------------------------------------------------
texture colorMapTexture;
sampler colorMap = sampler_state
{
    Texture = <colorMapTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};


// ------------------------------------------------------------
// 그림자맵
// ------------------------------------------------------------
texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};


// ------------------------------------------------------------
// 노멀맵
// ------------------------------------------------------------
texture normalMapTexture;
sampler2D normalMap = sampler_state
{
    Texture = <normalMapTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};


struct VS_OUTPUT
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float3 Eye : TEXCOORD1;
	float3 N : TEXCOORD2;
};


struct VS_BUMP_OUTPUT
{
    float4 Pos	 : POSITION;
	float2 Tex : TEXCOORD0;
	float4 Diffuse : COLOR0;
	float4 Specular : COLOR1;
	float3 HalfVector : TEXCOORD1;
	float3 LightDir : TEXCOORD2;
};


struct VS_SHADOW_OUTPUT
{
	float4 Pos : POSITION;
	float4 Diffuse : COLOR0;
};


struct VS_OUTPUT_SHADOW
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float4 TexShadow : TEXCOORD1;
	float3 Eye : TEXCOORD2;
	float3 N : TEXCOORD3;
};



// -------------------------------------------------------------
// 0패스:정점셰이더
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
	float4 worldPos = mul(Pos, mWorld);
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.Pos = mul(worldPos, mVP);
	Out.N = N;
	Out.Eye = vEyePos - worldPos.xyz;
	Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 0패스: 픽셀셰이더, 퐁셰이딩
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	float3 L = -light.dir;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	float4 color = light.ambient * material.ambient
				+ light.diffuse * material.diffuse * max(0, dot(N,L));
				+ light.specular * pow( max(0, dot(N,H)), 16);

	float4 Out = color * tex2D(colorMap, In.Tex);
    return Out;
}



// -------------------------------------------------------------
// 1패스:정점셰이더, 그림자 맵 생성.
// -------------------------------------------------------------
VS_SHADOW_OUTPUT VS_pass1(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_SHADOW_OUTPUT Out = (VS_SHADOW_OUTPUT)0;  // 출력데이터
    
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );
	Out.Diffuse = float4(1,1,1,1);

    return Out;
}



// -------------------------------------------------------------
// 2패스: 정점셰이더, 포그 출력
// -------------------------------------------------------------
VS_OUTPUT VS_pass2(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
	float4 worldPos = mul(Pos, mWorld);
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.Pos = mul(worldPos, mVP);
	Out.N = N;
	Out.Eye = vEyePos - worldPos.xyz;
	Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 2패스: 픽셀셰이더, 포그 출력.
// -------------------------------------------------------------
float4 PS_pass2(VS_OUTPUT In) : COLOR
{
	float3 L = -light.dir;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	float4 color = light.ambient * material.ambient
				+ light.diffuse * material.diffuse * max(0, dot(N,L));
				+ light.specular * pow( max(0, dot(N,H)), 16);

	float4 Out = color * tex2D(colorMap, In.Tex);

	float distance = length(In.Eye);
	float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	Out = lerp(Out, fogColor, l);

    return Out;
}



// -------------------------------------------------------------
// 3패스: 정점셰이더, 포그 + 그림자 출력.
// -------------------------------------------------------------
VS_OUTPUT_SHADOW VS_pass3(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT_SHADOW Out = (VS_OUTPUT_SHADOW)0;        // 출력데이터
    
	float4 worldPos = mul(Pos, mWorld);
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.Pos = mul(worldPos, mVP);
	Out.N = N;
	Out.Eye = vEyePos - worldPos.xyz;
	Out.Tex = Tex;
	Out.TexShadow = mul( Pos, mWVPT );
    
    return Out;
}

// -------------------------------------------------------------
// 3패스:픽셀셰이더, 포그 + 그림자 출력.
// -------------------------------------------------------------
float4 PS_pass3(VS_OUTPUT_SHADOW In) : COLOR
{
	float3 L = -light.dir;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	float4 color = 	light.ambient * material.ambient
				+ light.diffuse * material.diffuse * max(0, dot(N,L));
				+ light.specular * pow( max(0, dot(N,H)), 16);

	float4 Out = color * tex2D(colorMap, In.Tex);

	float4 shadow = tex2Dproj( ShadowMapSamp, In.TexShadow );
	Out = Out * saturate(color - (0.8f*shadow));

	float distance = length(In.Eye);
	float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	Out = lerp(Out, fogColor, l);

    return Out;
}



// -------------------------------------------------------------
// 4패스: 정점셰이더, 범프 매핑
// D3D9NormalMapping 소스를 참조 함.
// http://www.dhpoware.com/demos/d3d9NormalMapping.html
// -------------------------------------------------------------
VS_BUMP_OUTPUT VS_pass4(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0,		// 텍스쳐 좌표
	  float3 tangent : TANGENT	,	// tangent 벡터
	  float3 binormal : BINORMAL 	// binormal 벡터
)
{
    VS_BUMP_OUTPUT Out = (VS_BUMP_OUTPUT)0;

	float3 worldPos = mul(Pos, mWorld).xyz;
	float3 lightDir = -light.dir;
	float3 viewDir = vEyePos - worldPos;
	float3 halfVector = normalize(normalize(lightDir) + normalize(viewDir));
	
	float3 n = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.
	float3 t = normalize( mul(tangent, (float3x3)mWIT) ); // 월드 좌표계에서의 탄젠트
	float3 b = normalize( mul(binormal, (float3x3)mWIT) ); // 월드 좌표계에서의 바이노멀
	float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);
    
	Out.Pos = mul(float4(worldPos,1), mVP);
	Out.Tex = Tex;
	Out.HalfVector = mul(halfVector, tbnMatrix);
	Out.LightDir = mul(lightDir, tbnMatrix);
	Out.Diffuse = material.diffuse * light.diffuse;
	Out.Specular = material.specular * light.specular;
    
    return Out;
}


// -------------------------------------------------------------
// 4패스: 픽셀셰이더, 범프 매핑
// -------------------------------------------------------------
float4 PS_pass4(VS_BUMP_OUTPUT In) : COLOR
{
    float3 n = normalize(tex2D(normalMap, In.Tex).rgb * 2.0f - 1.0f);
    float3 h = normalize(In.HalfVector);
    float3 l = normalize(In.LightDir);

    float nDotL = saturate(dot(n, l));
    float nDotH = saturate(dot(n, h));
    float power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, shininess);

    float4 color = material.ambient * (globalAmbient + light.ambient) 
						+ (In.Diffuse * nDotL) 
						+ (In.Specular * power);

	return  color * tex2D(colorMap, In.Tex);
}


	
// -------------------------------------------------------------
// 테크닉
// -------------------------------------------------------------
technique TShader
{

	// 퐁 셰이딩
    pass P0
    {
        VertexShader = compile vs_3_0 VS_pass0();
		PixelShader  = compile ps_3_0 PS_pass0();
    }

	// 그림자 맵 생성 셰이더
	Pass P1
	{
		VertexShader = compile vs_3_0 VS_pass1();
	}


	// 포그 셰이딩.
    pass P2
    {
        VertexShader = compile vs_3_0 VS_pass2();
		PixelShader  = compile ps_3_0 PS_pass2();
    }


    // 모델 + 그림자 출력 셰이더
    pass P3
    {
        VertexShader = compile vs_3_0 VS_pass3();
		PixelShader  = compile ps_3_0 PS_pass3();
    }


	// 모델 + 범프매핑
    pass P4
    {
        VertexShader = compile vs_3_0 VS_pass4();
		PixelShader  = compile ps_3_0 PS_pass4();
    }


}
