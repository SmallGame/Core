/****************************************************************************
 *      renderpasses.cc: Render Passes operations
 *      This is part of the yafaray package
 *      Copyright (C) 2015  David Bluecame
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library; if not, write to the Free Software
 *      Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <core_api/color.h>
#include <core_api/renderpasses.h>

__BEGIN_YAFRAY

renderPasses_t::renderPasses_t()
{ 
	extPasses.reserve(PASS_EXT_TOTAL_PASSES);

	extPasses.push_back(extPass_t(PASS_EXT_COMBINED, PASS_INT_COMBINED));	//by default we will have an external Combined pass
	
	this->generate_pass_maps();
}

       
size_t renderPasses_t::numExtPasses() const { return extPasses.size(); }
        
int renderPasses_t::externalPassType(size_t pass_seq) const { return extPasses[pass_seq].externalPassType; }
	
std::string renderPasses_t::externalPassTypeString(size_t pass_seq) const { return extPassMapIntString.find(extPasses[pass_seq].externalPassType)->second; }
        
int renderPasses_t::externalTyleType(size_t pass_seq) const { return extPasses[pass_seq].externalTyleType; }

int renderPasses_t::internalYafPassType(externalPassTypes_t pass) const { return extPasses[pass].internalYafPassType; }
        
int renderPasses_t::internalYafPassType(int pass) const { return extPasses[pass].internalYafPassType; }

void renderPasses_t::generate_pass_maps()
{
	//External Render passes - mapping String and External Pass Type
	//IMPORTANT: the external strings MUST MATCH the pass property names in Blender. These must also match the property names in Blender-Exporter without the "pass_" prefix. 
	extPassMapStringInt["Combined"] = PASS_EXT_COMBINED;
	extPassMapStringInt["Depth"] = PASS_EXT_Z_DEPTH;
	extPassMapStringInt["Vector"] = PASS_EXT_VECTOR;
	extPassMapStringInt["Normal"] = PASS_EXT_NORMAL;
	extPassMapStringInt["UV"] = PASS_EXT_UV;
	extPassMapStringInt["Color"] = PASS_EXT_COLOR;
	extPassMapStringInt["Emit"] = PASS_EXT_EMIT;
	extPassMapStringInt["Mist"] = PASS_EXT_MIST;
	extPassMapStringInt["Diffuse"] = PASS_EXT_DIFFUSE;
	extPassMapStringInt["Spec"] = PASS_EXT_SPECULAR;
	extPassMapStringInt["AO"] = PASS_EXT_AO;
	extPassMapStringInt["Env"] = PASS_EXT_ENV;
	extPassMapStringInt["Indirect"] = PASS_EXT_INDIRECT;
	extPassMapStringInt["Shadow"] = PASS_EXT_SHADOW;
	extPassMapStringInt["Reflect"] = PASS_EXT_REFLECT;
	extPassMapStringInt["Refract"] = PASS_EXT_REFRACT;
	extPassMapStringInt["IndexOB"] = PASS_EXT_OBJ_INDEX;
	extPassMapStringInt["IndexMA"] = PASS_EXT_MAT_INDEX;
	extPassMapStringInt["DiffDir"] = PASS_EXT_DIFFUSE_DIRECT;
	extPassMapStringInt["DiffInd"] = PASS_EXT_DIFFUSE_INDIRECT;
	extPassMapStringInt["DiffCol"] = PASS_EXT_DIFFUSE_COLOR;
	extPassMapStringInt["GlossDir"] = PASS_EXT_GLOSSY_DIRECT;
	extPassMapStringInt["GlossInd"] = PASS_EXT_GLOSSY_INDIRECT;
	extPassMapStringInt["GlossCol"] = PASS_EXT_GLOSSY_COLOR;
	extPassMapStringInt["TransDir"] = PASS_EXT_TRANS_DIRECT;
	extPassMapStringInt["TransInd"] = PASS_EXT_TRANS_INDIRECT;
	extPassMapStringInt["TransCol"] = PASS_EXT_TRANS_COLOR;
	extPassMapStringInt["SubsurfaceDir"] = PASS_EXT_SUBSURFACE_DIRECT;
	extPassMapStringInt["SubsurfaceInd"] = PASS_EXT_SUBSURFACE_INDIRECT;
	extPassMapStringInt["SubsurfaceCol"] = PASS_EXT_SUBSURFACE_COLOR;

	//Generation of reverse map (pass type -> pass_string)
	for(std::map<std::string, int>::const_iterator it = extPassMapStringInt.begin(); it != extPassMapStringInt.end(); ++it)
	{
		extPassMapIntString[it->second] = it->first;
	}

	//Internal YafaRay Render passes - mapping String and Internal YafaRay Render passes
	//IMPORTANT: the internal strings MUST MATCH the valid values for the pass properties in Blender Exporter
	intPassMapStringInt["combined"] = PASS_INT_COMBINED;
	intPassMapStringInt["z-depth-norm"] = PASS_INT_Z_DEPTH_NORM;
	intPassMapStringInt["z-depth-abs"] = PASS_INT_Z_DEPTH_ABS;
	intPassMapStringInt["debug-normal-smooth"] = PASS_INT_NORMAL_SMOOTH;
	intPassMapStringInt["debug-normal-geom"] = PASS_INT_NORMAL_GEOM;
	intPassMapStringInt["adv-radiance"] = PASS_INT_RADIANCE;
	intPassMapStringInt["debug-uv"] = PASS_INT_UV;
	intPassMapStringInt["emit"] = PASS_INT_EMIT;
	intPassMapStringInt["mist"] = PASS_INT_MIST;
	intPassMapStringInt["diffuse"] = PASS_INT_DIFFUSE;
	intPassMapStringInt["diffuse-noshadow"] = PASS_INT_DIFFUSE_NO_SHADOW;
	intPassMapStringInt["ao"] = PASS_INT_AO;
	intPassMapStringInt["ao-clay"] = PASS_INT_AO_CLAY;
	intPassMapStringInt["env"] = PASS_INT_ENV;
	intPassMapStringInt["indirect"] = PASS_INT_INDIRECT_ALL;
	intPassMapStringInt["adv-indirect"] = PASS_INT_INDIRECT;
	intPassMapStringInt["shadow"] = PASS_INT_SHADOW;
	intPassMapStringInt["reflect"] = PASS_INT_REFLECT_ALL;
	intPassMapStringInt["refract"] = PASS_INT_REFRACT_ALL;
	intPassMapStringInt["adv-reflect"] = PASS_INT_REFLECT_PERFECT;
	intPassMapStringInt["adv-refract"] = PASS_INT_REFRACT_PERFECT;
	intPassMapStringInt["obj-index-abs"] = PASS_INT_OBJ_INDEX_ABS;
	intPassMapStringInt["obj-index-norm"] = PASS_INT_OBJ_INDEX_NORM;
	intPassMapStringInt["obj-index-auto"] = PASS_INT_OBJ_INDEX_AUTO;
	intPassMapStringInt["obj-index-mask"] = PASS_INT_OBJ_INDEX_MASK;
	intPassMapStringInt["obj-index-mask-shadow"] = PASS_INT_OBJ_INDEX_MASK_SHADOW;
	intPassMapStringInt["obj-index-mask-all"] = PASS_INT_OBJ_INDEX_MASK_ALL;
	intPassMapStringInt["mat-index-abs"] = PASS_INT_MAT_INDEX_ABS;
	intPassMapStringInt["mat-index-norm"] = PASS_INT_MAT_INDEX_NORM;
	intPassMapStringInt["mat-index-auto"] = PASS_INT_MAT_INDEX_AUTO;
	intPassMapStringInt["mat-index-mask"] = PASS_INT_MAT_INDEX_MASK;
	intPassMapStringInt["mat-index-mask-shadow"] = PASS_INT_MAT_INDEX_MASK_SHADOW;
	intPassMapStringInt["mat-index-mask-all"] = PASS_INT_MAT_INDEX_MASK_ALL;
	intPassMapStringInt["adv-diffuse-indirect"] = PASS_INT_DIFFUSE_INDIRECT;
	intPassMapStringInt["adv-diffuse-color"] = PASS_INT_DIFFUSE_COLOR;
	intPassMapStringInt["adv-glossy"] = PASS_INT_GLOSSY;
	intPassMapStringInt["adv-glossy-indirect"] = PASS_INT_GLOSSY_INDIRECT;
	intPassMapStringInt["adv-glossy-color"] = PASS_INT_GLOSSY_COLOR;
	intPassMapStringInt["adv-trans"] = PASS_INT_TRANS;
	intPassMapStringInt["adv-trans-indirect"] = PASS_INT_TRANS_INDIRECT;
	intPassMapStringInt["adv-trans-color"] = PASS_INT_TRANS_COLOR;
	intPassMapStringInt["adv-subsurface"] = PASS_INT_SUBSURFACE;
	intPassMapStringInt["adv-subsurface-indirect"] = PASS_INT_SUBSURFACE_INDIRECT;
	intPassMapStringInt["adv-subsurface-color"] = PASS_INT_SUBSURFACE_COLOR;
	intPassMapStringInt["debug-normal-smooth"] = PASS_INT_NORMAL_SMOOTH;
	intPassMapStringInt["debug-normal-geom"] = PASS_INT_NORMAL_GEOM;
	intPassMapStringInt["debug-nu"] = PASS_INT_DEBUG_NU;
	intPassMapStringInt["debug-nv"] = PASS_INT_DEBUG_NV;
	intPassMapStringInt["debug-dpdu"] = PASS_INT_DEBUG_DPDU;
	intPassMapStringInt["debug-dpdv"] = PASS_INT_DEBUG_DPDV;
	intPassMapStringInt["debug-dsdu"] = PASS_INT_DEBUG_DSDU;
	intPassMapStringInt["debug-dsdv"] = PASS_INT_DEBUG_DSDV;
	intPassMapStringInt["adv-surface-integration"] = PASS_INT_SURFACE_INTEGRATION;
	intPassMapStringInt["adv-volume-integration"] = PASS_INT_VOLUME_INTEGRATION;
	intPassMapStringInt["adv-volume-transmittance"] = PASS_INT_VOLUME_TRANSMITTANCE;
	intPassMapStringInt["debug-aa-samples"] = PASS_INT_AA_SAMPLES;

	//Generation of reverse map (pass type -> pass_string)
	for(std::map<std::string, int>::const_iterator it = intPassMapStringInt.begin(); it != intPassMapStringInt.end(); ++it)
	{
		intPassMapIntString[it->second] = it->first;
	}
}

void renderPasses_t::pass_add(const std::string& sExternalPass, const std::string& sInternalPass)
{
	//This function adds a new external pass, linked to a certain internal pass, based on the text strings indicated in the parameters.
	
	//By default, in case the strings are not found in the maps, set the types to "disabled"
	int extPassType = PASS_EXT_DISABLED;
	int intPassType = PASS_INT_DISABLED;
	
	//Convert the string into the external pass type using the pass type maps
	std::map<std::string, int>::const_iterator extPassMapIterator = extPassMapStringInt.find(sExternalPass);
	if(extPassMapIterator != extPassMapStringInt.end()) extPassType = extPassMapIterator->second;
	
	//Convert the string into the internal pass type using the pass type maps
	std::map<std::string, int>::const_iterator intPassMapIterator = intPassMapStringInt.find(sInternalPass);
	if(intPassMapIterator != intPassMapStringInt.end()) intPassType = intPassMapIterator->second;
	
	if(extPassType != PASS_EXT_COMBINED && extPassType != PASS_EXT_DISABLED && intPassType != PASS_INT_DISABLED)
	{
		//If both external and internal pass types exist and are not disabled, then add the External Pass with the appropiate link to the associated internal pass
		//Also, don't add another external Combined pass, as it's added by default, to avoid duplication of the Combined pass.
		extPasses.push_back(extPass_t(extPassType, intPassType));
	}
    
    //If any internal pass needs an auxiliary internal pass, enable also the auxiliary passes.
}



extPass_t::extPass_t(int extPassType, int intPassType):
			externalPassType(extPassType), internalYafPassType(intPassType)
{ 
	switch(extPassType)  //These are the tyle types needed for Blender
	{
		case PASS_EXT_COMBINED:		externalTyleType = PASS_EXT_TILE_4_RGBA;		break;
		case PASS_EXT_Z_DEPTH:		externalTyleType = PASS_EXT_TILE_1_GRAYSCALE;	break;
		case PASS_EXT_VECTOR:		externalTyleType = PASS_EXT_TILE_4_RGBA;		break;
		case PASS_EXT_COLOR:		externalTyleType = PASS_EXT_TILE_4_RGBA;		break;
		case PASS_EXT_MIST:			externalTyleType = PASS_EXT_TILE_1_GRAYSCALE;	break;
		case PASS_EXT_OBJ_INDEX:	externalTyleType = PASS_EXT_TILE_1_GRAYSCALE;	break;
		case PASS_EXT_MAT_INDEX:	externalTyleType = PASS_EXT_TILE_1_GRAYSCALE;	break;
		default: 					externalTyleType = PASS_EXT_TILE_3_RGB;			break;
	}
	
	enabled = ((extPassType == PASS_EXT_DISABLED || extPassType >= PASS_EXT_TOTAL_PASSES) ? false : true);
}




colorIntPasses_t::colorIntPasses_t(renderPasses_t &renderPasses):highestInternalPassUsed(PASS_INT_COMBINED), passDefinitions(renderPasses)
{
	highestInternalPassUsed = PASS_INT_DISABLED;
	
	//for performance, even if we don't actually use all the possible internal passes, we reserve a contiguous memory block
	intPasses.reserve(PASS_INT_TOTAL_PASSES);
	enabledIntPasses.reserve(PASS_INT_TOTAL_PASSES);
	
	//by default, if no passes are explicitally enabled, we create the Combined pass by default
	enable_pass(PASS_INT_COMBINED);
}
        
bool colorIntPasses_t::enabled(int pass) const
{
	if(pass <= highestInternalPassUsed) return enabledIntPasses[pass];
	
	else return false;
}
        
void colorIntPasses_t::enable_pass(int pass)
{
	if(enabled(pass) || pass == PASS_INT_DISABLED) return;
	
	if(pass > highestInternalPassUsed)
	{
		for(int idx = highestInternalPassUsed+1; idx <= pass; ++idx)
		{
			intPasses.push_back(init_color(idx));
								
			if(idx == pass) enabledIntPasses.push_back(true);
			else enabledIntPasses.push_back(false);
		}
		
		highestInternalPassUsed = pass;
	}
	enabledIntPasses[pass] = true;
}
        
colorA_t& colorIntPasses_t::color(int pass)
{
	return intPasses[pass];
}
                
colorA_t& colorIntPasses_t::operator()(int pass)
{
	return color(pass);
}

void colorIntPasses_t::reset_colors()
{
	for(int idx = PASS_INT_COMBINED; idx <= highestInternalPassUsed; ++idx)
	{
		color(idx) = init_color(idx);
	}
}
        
colorA_t colorIntPasses_t::init_color(int pass)
{
	switch(pass)    //Default initialization color in general is black/opaque, except for SHADOW and MASK passes where the default is black/transparent for easier masking
	{
		case PASS_INT_SHADOW:
		case PASS_INT_OBJ_INDEX_MASK:
		case PASS_INT_OBJ_INDEX_MASK_SHADOW:
		case PASS_INT_OBJ_INDEX_MASK_ALL:
		case PASS_INT_MAT_INDEX_MASK:
		case PASS_INT_MAT_INDEX_MASK_SHADOW:
		case PASS_INT_MAT_INDEX_MASK_ALL: return colorA_t(0.f, 0.f, 0.f, 0.f); break;
		default: return colorA_t(0.f, 0.f, 0.f, 1.f); break;
	}            
}

void colorIntPasses_t::multiply_colors(float factor)
{
	for(int idx = PASS_INT_COMBINED; idx <= highestInternalPassUsed; ++idx)
	{
		color(idx) *= factor;
	}
}

colorA_t colorIntPasses_t::probe_set(const int& pass, const colorA_t& renderedColor, const bool& condition /*= true */)
{
	if(condition && enabled(pass)) color(pass) = renderedColor;
	
	return renderedColor;
}

colorA_t colorIntPasses_t::probe_set(const int& pass, const colorIntPasses_t& colorPasses, const bool& condition /*= true */)
{
	if(condition && enabled(pass) && colorPasses.enabled(pass))
	{
		intPasses[pass] = colorPasses.intPasses[pass];	
		return  colorPasses.intPasses[pass];
	}
	else return colorA_t(0.f);
}

colorA_t colorIntPasses_t::probe_add(const int& pass, const colorA_t& renderedColor, const bool& condition /*= true */)
{
	if(condition && enabled(pass)) color(pass) += renderedColor;
	
	return renderedColor;
}

colorA_t colorIntPasses_t::probe_add(const int& pass, const colorIntPasses_t& colorPasses, const bool& condition /*= true */)
{
	if(condition && enabled(pass) && colorPasses.enabled(pass))
	{
		intPasses[pass] += colorPasses.intPasses[pass];	
		return  colorPasses.intPasses[pass];
	}
	else return colorA_t(0.f);
}

colorA_t colorIntPasses_t::probe_mult(const int& pass, const colorA_t& renderedColor, const bool& condition /*= true */)
{
	if(condition && enabled(pass)) color(pass) *= renderedColor;
	
	return renderedColor;
}

colorA_t colorIntPasses_t::probe_mult(const int& pass, const colorIntPasses_t& colorPasses, const bool& condition /*= true */)
{
	if(condition && enabled(pass) && colorPasses.enabled(pass))
	{
		intPasses[pass] *= colorPasses.intPasses[pass];	
		return  colorPasses.intPasses[pass];
	}
	else return colorA_t(0.f);
}

colorIntPasses_t & colorIntPasses_t::operator *=(CFLOAT f)
{
	for(int idx = PASS_INT_COMBINED; idx <= highestInternalPassUsed; ++idx)
	{
		color(idx) *= f;
	}
	return *this;
}

colorIntPasses_t & colorIntPasses_t::operator *=(color_t &a)
{
	for(int idx = PASS_INT_COMBINED; idx <= highestInternalPassUsed; ++idx)
	{
		color(idx) *= a;
	}
	return *this;
}

colorIntPasses_t & colorIntPasses_t::operator *=(colorA_t &a)
{
	for(int idx = PASS_INT_COMBINED; idx <= highestInternalPassUsed; ++idx)
	{
		color(idx) *= a;
	}
	return *this;
}

colorIntPasses_t & colorIntPasses_t::operator +=(colorIntPasses_t &a)
{
	for(int idx = PASS_INT_COMBINED; idx <= highestInternalPassUsed; ++idx)
	{
		color(idx) += a.color(idx);
	}
	return *this;
}

int colorIntPasses_t::get_highest_internal_pass_used() const { return highestInternalPassUsed; }


__END_YAFRAY
