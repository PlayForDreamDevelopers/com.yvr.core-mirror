Shader "Unlit/PassthroughShader"
{
    Properties
    {
        
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
    
                UNITY_VERTEX_INPUT_INSTANCE_ID //Insert
            };

            //v2f output struct

            struct v2f
            {

                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
                int eyeIndex : TEXCOORD1;

                UNITY_VERTEX_OUTPUT_STEREO //Insert
            };

            v2f vert (appdata v)
            {
                v2f o;
    
                UNITY_SETUP_INSTANCE_ID(v); //Insert
                UNITY_INITIALIZE_OUTPUT(v2f, o); //Insert
                UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o); //Insert
    
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                o.eyeIndex = unity_StereoEyeIndex;

                return o;
            }

            UNITY_DECLARE_TEX2DARRAY(_MainTex); //Insert

            fixed4 frag (v2f i) : SV_Target
            {
                UNITY_SETUP_STEREO_EYE_INDEX_POST_VERTEX(i); //Insert
    
                fixed4 col = UNITY_SAMPLE_TEX2DARRAY(_MainTex, fixed3(i.uv.x, i.uv.y, i.eyeIndex)); //Insert
    
    
                return col;
            }
            ENDCG
        }
    }
}
