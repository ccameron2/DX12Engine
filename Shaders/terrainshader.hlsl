#include "common.hlsl"

struct VIn
{
	float3 PosL : POSITION;
	float4 Colour : COLOUR;
	float3 NormalL : NORMAL;
};

struct VOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float4 Colour : COLOUR;
	float3 NormalW : NORMAL;
};

VOut VS(VIn vin)
{
	VOut vout;
	
	// Transform to homogeneous clip space.
	float4 posW = mul(float4(vin.PosL, 1.0f), World);
	vout.PosW = posW.xyz;
	
	vout.NormalW = mul(vin.NormalL, (float3x3) World);
	
	vout.PosH = mul(posW, ViewProj);
	
	// Pass vertex colour into the pixel shader.
	vout.Colour = vin.Colour;
    
	return vout;
}

float4 PS(VOut pIn) : SV_Target
{
	VOut vOut;
	
	float3 dx = ddx(pIn.PosW);
	float3 dy = ddy(pIn.PosW);
	
	float3 n = normalize(cross(dx, dy));
	float3 v = normalize(EyePosW - pIn.PosW); // Get normal to camera, called v for view vector in PBR equations
	
    // Calculate the steepness based on the dot product of the surface normal and the up vector (0, 1, 0)
	float steepness = abs(dot(n, float3(0, 1, 0)));
    
    // Set the thresholds for terrain types
	float stoneThreshold = 0.7f;
	float dirtThreshold = 0.85f;
    
	float3 albedo;
    
	if (steepness < stoneThreshold)
	{
		// Stone terrain
		albedo = float3(0.5f, 0.5f, 0.5f); // Gray color for space rock
	}
	else if (steepness < dirtThreshold)
	{
		// Dirt terrain
		albedo = float3(0.4f, 0.4f, 0.4f); // Dark gray color for space rock
	}
	else
	{
		// Grass terrain
		albedo = float3(0.3f, 0.3f, 0.3f); // Dark gray color for space surface
	}
    
    // Set roughness, metalness, and AO to constant values
	float roughness = 0.95f;
	float metalness = 1.0f;
	float ao = 1.0f;
    
	return CalculateLighting(albedo, roughness, metalness, ao, n, v);
}