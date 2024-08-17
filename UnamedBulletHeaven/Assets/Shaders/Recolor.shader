Shader "Sprites/BaseRecolor"
{
	Properties
	{
		_MainTex("Texture",2D) = "white" {}
		_ColorTex("Colors", 2D) = "white" {}
	}

		SubShader
		{
			Cull Off
			Blend One OneMinusSrcAlpha
			Pass
			{
				CGPROGRAM

				#pragma vertex vertexFunc
				#pragma fragment fragFunc
				#include "UNITYCG.cginc"

				sampler2D _MainTex;
				sampler2D _ColorTex;

				struct v2f
				{
					float4 pos: SV_POSITION;
					half2 uv: TEXCOORD0;
				};

				v2f vertexFunc(appdata_base v)
				{
					v2f o;
					o.pos = UnityObjectToClipPos(v.vertex);
					o.uv = v.texcoord;

					return o;
				}

				float4 _MainTex_TexelSize;
				float4 _ColorTex_TexelSize;

				fixed4 fragFunc(v2f i) : COLOR
				{
					half4 c = tex2D(_MainTex, i.uv);

					//To make the sprite, divide 1 by the number of colors
					//Multiply by 10 (for %)
					//the darkest pixel need a value of half the result
					//then add that result to each subsequent color
					//you (probably) wont reach full brightness or 0 brightness

					float pos = c.r;

					float2 loc = (0,pos);
					half4 newC = tex2D(_ColorTex, loc);
					newC.rgb *= c.a;
					newC.a = c.a;

					return newC;
				}

				ENDCG
			}
		}
}