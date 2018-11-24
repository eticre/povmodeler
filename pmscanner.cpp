/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2003 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#include <ctype.h>
#include <stdlib.h>


#include "pmdebug.h"
#include "pmscanner.h"
#include "pmtokens.h"


//#define PMSCAN_DEBUG

PMReservedWordDict::PMReservedWordDict( PMDictMode mode )
      : QMap< QString, int >()
{
   switch( mode )
   {
      case PMDReservedWords:
         insert( "aa_level", int( AA_LEVEL_TOK ) );
         insert( "aa_threshold", int( AA_THRESHOLD_TOK ) );
         insert( "abs", int( ABS_TOK ) );
         insert( "absorption", int( ABSORPTION_TOK ) );
         insert( "accuracy", int( ACCURACY_TOK ) );
         insert( "acos", int( ACOS_TOK ) );
         insert( "acosh", int( ACOSH_TOK ) );
         insert( "adaptive", int( ADAPTIVE_TOK ) );
         insert( "adc_bailout", int( ADC_BAILOUT_TOK ) );
         insert( "agate", int( AGATE_TOK ) );
         insert( "agate_turb", int( AGATE_TURB_TOK ) );
         insert( "all", int( ALL_TOK ) );
         insert( "all_intersections", int( ALL_INTERSECTIONS_TOK ) );
         insert( "alpha", int( ALPHA_TOK ) );
         insert( "altitude", int( ALTITUDE_TOK ) );
         insert( "always_sample", int ( ALWAYS_SAMPLE_TOK ) );
         insert( "emission", int( EMISSION_TOK ) );
         insert( "emission_light", int( EMISSION_LIGHT_TOK ) );
         insert( "ambient", int( AMBIENT_TOK ) );
         insert( "ambient_light", int( AMBIENT_LIGHT_TOK ) );
         insert( "angle", int( ANGLE_TOK ) );
         insert( "aperture", int( APERTURE_TOK ) );
         insert( "arc_angle", int( ARC_ANGLE_TOK ) );
         insert( "area_light", int( AREA_LIGHT_TOK ) );
         insert( "autostop", int ( AUTOSTOP_TOK ) );
         insert( "circular", int( AREA_CIRCULAR_TOK ) );
         insert( "asc", int( ASC_TOK ) );
         insert( "asin", int( ASIN_TOK ) );
         insert( "asinh", int( ASINH_TOK ) );
         insert( "assumed_gamma", int( ASSUMED_GAMMA_TOK ) );
         insert( "atan", int( ATAN_TOK ) );
         insert( "atan2", int( ATAN2_TOK ) );
         insert( "atanh", int( ATANH_TOK ) );
         insert( "atmosphere", int( ATMOSPHERE_TOK ) );
         insert( "atmospheric_attenuation", int( ATMOSPHERIC_ATTENUATION_TOK ) );
         insert( "attenuating", int( ATTENUATING_TOK ) );
         insert( "average", int( AVERAGE_TOK ) );
         insert( "b_spline", int( B_SPLINE_TOK ) );
         insert( "background", int( BACKGROUND_TOK ) );
         insert( "bezier_spline", int( BEZIER_SPLINE_TOK ) );
         insert( "bicubic_patch", int( BICUBIC_PATCH_TOK ) );
         insert( "black_hole", int( BLACK_HOLE_TOK ) );
         insert( "blob", int( BLOB_TOK ) );
         insert( "blue", int( BLUE_TOK ) );
         insert( "blur_samples", int( BLUR_SAMPLES_TOK ) );
         insert( "bounded_by", int( BOUNDED_BY_TOK ) );
         insert( "box", int( BOX_TOK ) );
         insert( "boxed", int( BOXED_TOK ) );
         insert( "bozo", int( BOZO_TOK ) );
         insert( "brick", int( BRICK_TOK ) );
         insert( "brick_size", int( BRICK_SIZE_TOK ) );
         insert( "brightness", int( BRIGHTNESS_TOK ) );
         insert( "brilliance", int( BRILLIANCE_TOK ) );
         insert( "bumps", int( BUMPS_TOK ) );
         insert( "bumpy1", int( BUMPY1_TOK ) );
         insert( "bumpy2", int( BUMPY2_TOK ) );
         insert( "bumpy3", int( BUMPY3_TOK ) );
         insert( "bump_map", int( BUMP_MAP_TOK ) );
         insert( "bump_size", int( BUMP_SIZE_TOK ) );
         insert( "camera", int( CAMERA_TOK ) );
         insert( "caustics", int( CAUSTICS_TOK ) );
         insert( "ceil", int( CEIL_TOK ) );
         insert( "checker", int( CHECKER_TOK ) );
         insert( "chr", int( CHR_TOK ) );
         insert( "cells", int( CELLS_TOK ) );
         insert( "clipped_by", int( CLIPPED_BY_TOK ) );
         insert( "clock", int( CLOCK_TOK ) );
         insert( "clock_delta", int( CLOCK_DELTA_TOK ) );
         insert( "collect", int( COLLECT_TOK ) );
         insert( "color", int( COLOR_TOK ) );
         insert( "color_map", int( COLOR_MAP_TOK ) );
         insert( "colour", int( COLOUR_TOK ) );
         insert( "colour_map", int( COLOUR_MAP_TOK ) );
         insert( "component", int( COMPONENT_TOK ) );
         insert( "composite", int( COMPOSITE_TOK ) );
         insert( "concat", int( CONCAT_TOK ) );
         insert( "cone", int( CONE_TOK ) );
         insert( "confidence", int( CONFIDENCE_TOK ) );
         insert( "conic_sweep", int( CONIC_SWEEP_TOK ) );
         insert( "conserve_energy", int( CONSERVE_ENERGY_TOK ) );
         insert( "constant", int( CONSTANT_TOK ) );
         insert( "contained_by", int( CONTAINED_BY_TOK ) );
         insert( "control0", int( CONTROL0_TOK ) );
         insert( "control1", int( CONTROL1_TOK ) );
         insert( "cos",  int( COS_TOK ) );
         insert( "cosh", int( COSH_TOK ) );
         insert( "count", int( COUNT_TOK ) );
         insert( "crackle", int( CRACKLE_TOK ) );
         insert( "crand", int( CRAND_TOK ) );
         insert( "cube", int( CUBE_TOK ) );
         insert( "cubic", int( CUBIC_TOK ) );
         insert( "cubic_spline", int( CUBIC_SPLINE_TOK ) );
         insert( "cubic_wave", int( CUBIC_WAVE_TOK ) );
         insert( "cylinder", int( CYLINDER_TOK ) );
         insert( "cylindrical", int( CYLINDRICAL_TOK ) );
         insert( "degrees", int( DEGREES_TOK ) );
         insert( "dents", int( DENTS_TOK ) );
         insert( "density", int( DENSITY_TOK ) );
         insert( "density_file", int( DENSITY_FILE_TOK ) );
         insert( "density_map", int( DENSITY_MAP_TOK ) );
         insert( "df3", int( DF3_TOK ) );
         insert( "difference", int( DIFFERENCE_TOK ) );
         insert( "diffuse", int( DIFFUSE_TOK ) );
         insert( "direction", int( DIRECTION_TOK ) );
         insert( "disc", int( DISC_TOK ) );
         insert( "dispersion", int ( DISPERSION_TOK ) );
         insert( "dispersion_samples", int ( DISPERSION_SAMPLES_TOK ) );
         insert( "dist_exp", int ( DIST_EXP_TOK ) );
         insert( "distance", int( DISTANCE_TOK ) );
         insert( "distance_maximum", int( DISTANCE_MAXIMUM_TOK ) );
         insert( "div", int( DIV_TOK ) );
         insert( "double_illuminate", int( DOUBLE_ILLUMINATE_TOK ) );
         insert( "dust", int( DUST_TOK ) );
         insert( "dust_type", int( DUST_TYPE_TOK ) );
         insert( "eccentricity", int( ECCENTRICITY_TOK ) );
         insert( "emission", int( EMISSION_TOK ) );
         insert( "emitting", int( EMITTING_TOK ) );
         insert( "error", int( ERROR_TOK ) );
         insert( "error_bound", int( ERROR_BOUND_TOK ) );
         insert( "evaluate", int( EVALUATE_TOK ) );
         insert( "exp", int( EXP_TOK ) );
         insert( "expand_thresholds", int (EXPAND_THRESHOLDS_TOK) );
         insert( "exponent", int( EXPONENT_TOK ) );
         insert( "exterior", int( EXTERIOR_TOK ) );
         insert( "extinction", int( EXTINCTION_TOK ) );
         insert( "fade_distance", int( FADE_DISTANCE_TOK ) );
         insert( "fade_power", int( FADE_POWER_TOK ) );
         insert( "falloff", int( FALLOFF_TOK ) );
         insert( "falloff_angle", int( FALLOFF_ANGLE_TOK ) );
         insert( "false", int( FALSE_TOK ) );
         insert( "file_exists", int( FILE_EXISTS_TOK ) );
         insert( "filter", int( FILTER_TOK ) );
         insert( "finish", int( FINISH_TOK ) );
         insert( "fisheye", int( FISHEYE_TOK ) );
         insert( "flatness", int( FLATNESS_TOK ) );
         insert( "flip", int( FLIP_TOK ) );
         insert( "floor", int( FLOOR_TOK ) );
         insert( "focal_point", int( FOCAL_POINT_TOK ) );
         insert( "fog", int( FOG_TOK ) );
         insert( "fog_alt", int( FOG_ALT_TOK ) );
         insert( "fog_offset", int( FOG_OFFSET_TOK ) );
         insert( "fog_type", int( FOG_TYPE_TOK ) );
         insert( "form", int( FORM_TOK ) );
         insert( "fresnel", int( FRESNEL_TOK ) );
         insert( "frequency", int( FREQUENCY_TOK ) );
         insert( "function", int( FUNCTION_TOK ) );
         insert( "gather", int( GATHER_TOK ) );
         insert( "gif", int( GIF_TOK ) );
         insert( "global_lights", int ( GLOBAL_LIGHTS_TOK ) );
         insert( "global_settings", int( GLOBAL_SETTINGS_TOK ) );
         insert( "glowing", int( GLOWING_TOK ) );
         insert( "gradient", int( GRADIENT_TOK ) );
         insert( "granite", int( GRANITE_TOK ) );
         insert( "gray_threshold", int( GRAY_THRESHOLD_TOK ) );
         insert( "green", int( GREEN_TOK ) );
         insert( "halo", int( HALO_TOK ) );
         insert( "height_field", int( HEIGHT_FIELD_TOK ) );
         insert( "hexagon", int( HEXAGON_TOK ) );
         insert( "hf_gray_16", int( HF_GRAY_16_TOK ) );
         insert( "hierarchy", int( HIERARCHY_TOK ) );
         insert( "hollow", int( HOLLOW_TOK ) );
         insert( "hypercomplex", int( HYPERCOMPLEX_TOK ) );
         insert( "iff", int( IFF_TOK ) );
         insert( "image_map", int( IMAGE_MAP_TOK ) );
         insert( "incidence", int( INCIDENCE_TOK ) );
         insert( "inside_vector", int( INSIDE_VECTOR_TOK ) );
         insert( "int", int( INT_TOK ) );
         insert( "interior", int( INTERIOR_TOK ) );
         insert( "interior_texture", int( INTERIOR_TEXTURE_TOK ) );
         insert( "interpolate", int( INTERPOLATE_TOK ) );
         insert( "intersection", int( INTERSECTION_TOK ) );
         insert( "intervals", int( INTERVALS_TOK ) );
         insert( "inverse", int( INVERSE_TOK ) );
         insert( "ior", int( IOR_TOK ) );
         insert( "irid", int( IRID_TOK ) );
         insert( "irid_wavelength", int( IRID_WAVELENGTH_TOK ) );
         insert( "isosurface", int( ISOSURFACE_TOK ) );
         insert( "jitter", int( JITTER_TOK ) );
         insert( "julia", int( JULIA_TOK ) );
         insert( "julia_fractal", int( JULIA_FRACTAL_TOK ) );
         insert( "lambda", int( LAMBDA_TOK ) );
         insert( "lathe", int( LATHE_TOK ) );
         insert( "leopard", int( LEOPARD_TOK ) );
         insert( "light_group", int ( LIGHT_GROUP_TOK ) );
         insert( "light_source", int( LIGHT_SOURCE_TOK ) );
         insert( "linear", int( LINEAR_TOK ) );
         insert( "linear_spline", int( LINEAR_SPLINE_TOK ) );
         insert( "linear_sweep", int( LINEAR_SWEEP_TOK ) );
         insert( "location", int( LOCATION_TOK ) );
         insert( "log", int( LOG_TOK ) );
         insert( "looks_like", int( LOOKS_LIKE_TOK ) );
         insert( "look_at", int( LOOK_AT_TOK ) );
         insert( "low_error_factor", int( LOW_ERROR_FACTOR_TOK ) );
         insert( "magnet", int ( MAGNET_TOK ) );
         insert( "major_radius", int( MAJOR_RADIUS_TOK ) );
         insert( "mandel", int( MANDEL_TOK ) );
         insert( "map_type", int( MAP_TYPE_TOK ) );
         insert( "marble", int( MARBLE_TOK ) );
         insert( "material", int( MATERIAL_TOK ) );
         insert( "material_map", int( MATERIAL_MAP_TOK ) );
         insert( "matrix", int( MATRIX_TOK ) );
         insert( "max", int( MAX_TOK ) );
         insert( "max_gradient", int( MAX_GRADIENT_TOK ) );
         insert( "max_intersections", int( MAX_INTERSECTIONS_TOK ) );
         insert( "max_iteration", int( MAX_ITERATION_TOK ) );
         insert( "max_sample", int( MAX_SAMPLE_TOK ) );
         insert( "max_trace", int( MAX_TRACE_TOK ) );
         insert( "max_trace_level", int( MAX_TRACE_LEVEL_TOK ) );
         insert( "max_value", int( MAX_VALUE_TOK ) );
         insert( "media", int( MEDIA_TOK ) );
         insert( "media_attenuation", int( MEDIA_ATTENUATION_TOK ) );
         insert( "media_interaction", int( MEDIA_INTERACTION_TOK ) );
         insert( "merge", int( MERGE_TOK ) );
         insert( "mesh", int( MESH_TOK ) );
         insert( "metallic", int( METALLIC_TOK ) );
         insert( "method", int( METHOD_TOK ) );
         insert( "metric", int( METRIC_TOK ) );
         insert( "min", int( MIN_TOK ) );
         insert( "minimum_reuse", int( MINIMUM_REUSE_TOK ) );
         insert( "mod", int( MOD_TOK ) );
         insert( "mortar", int( MORTAR_TOK ) );
         insert( "nearest_count", int( NEAREST_COUNT_TOK ) );
         insert( "no", int( NO_TOK ) );
         insert( "noise_generator", int( NOISE_GENERATOR_TOK ) );
         insert( "normal", int( NORMAL_TOK ) );
         insert( "normal_map", int( NORMAL_MAP_TOK ) );
         insert( "no_image", int( NO_IMAGE_TOK ) );
         insert( "no_reflection", int( NO_REFLECTION_TOK ) );
         insert( "no_shadow", int( NO_SHADOW_TOK ) );
         insert( "number_of_waves", int( NUMBER_OF_WAVES_TOK ) );
         insert( "object", int( OBJECT_TOK ) );
         insert( "octaves", int( OCTAVES_TOK ) );
         insert( "off", int( OFF_TOK ) );
         insert( "offset", int( OFFSET_TOK ) );
         insert( "omega", int( OMEGA_TOK ) );
         insert( "omnimax", int( OMNIMAX_TOK ) );
         insert( "on", int( ON_TOK ) );
         insert( "once", int( ONCE_TOK ) );
         insert( "onion", int( ONION_TOK ) );
         insert( "open", int( OPEN_TOK ) );
         insert( "orient", int( ORIENT_TOK ) );
         insert( "orthographic", int( ORTHOGRAPHIC_TOK ) );
         insert( "panoramic", int( PANORAMIC_TOK ) );
         insert( "parallel", int( PARALLEL_TOK ) );
         insert( "pass_through", int ( PASS_THROUGH_TOK ) );
         insert( "pattern1", int( PATTERN1_TOK ) );
         insert( "pattern2", int( PATTERN2_TOK ) );
         insert( "pattern3", int( PATTERN3_TOK ) );
         insert( "perspective", int( PERSPECTIVE_TOK ) );
         insert( "pgm", int( PGM_TOK ) );
         insert( "phase", int( PHASE_TOK ) );
         insert( "phong", int( PHONG_TOK ) );
         insert( "phong_size", int( PHONG_SIZE_TOK ) );
         insert( "photons", int ( PHOTONS_TOK ) );
         insert( "pi", int( PI_TOK ) );
         insert( "pigment", int( PIGMENT_TOK ) );
         insert( "pigment_map", int( PIGMENT_MAP_TOK ) );
         insert( "planar", int( PLANAR_TOK ) );
         insert( "plane", int( PLANE_TOK ) );
         insert( "png", int( PNG_TOK ) );
         insert( "point_at", int( POINT_AT_TOK ) );
         insert( "poly", int( POLY_TOK ) );
         insert( "poly_wave", int( POLY_WAVE_TOK ) );
         insert( "polygon", int( POLYGON_TOK ) );
         insert( "pot", int( POT_TOK ) );
         insert( "pow", int( POW_TOK ) );
         insert( "ppm", int( PPM_TOK ) );
         insert( "precision", int( PRECISION_TOK ) );
         insert( "pretrace_end", int( PRETRACE_END_TOK ) );
         insert( "pretrace_start", int( PRETRACE_START_TOK ) );
         insert( "prism", int( PRISM_TOK ) );
         insert( "projected_through", int( PROJECTED_THROUGH_TOK ) );
         insert( "pwr", int( PWR_TOK ) );
         insert( "quadratic_spline", int( QUADRATIC_SPLINE_TOK ) );
         insert( "quadric", int( QUADRIC_TOK ) );
         insert( "quartic", int( QUARTIC_TOK ) );
         insert( "quaternion", int( QUATERNION_TOK ) );
         insert( "quick_color", int( QUICK_COLOR_TOK ) );
         insert( "quick_colour", int( QUICK_COLOUR_TOK ) );
         insert( "quilted", int( QUILTED_TOK ) );
         insert( "radial", int( RADIAL_TOK ) );
         insert( "radians", int( RADIANS_TOK ) );
         insert( "radiosity", int( RADIOSITY_TOK ) );
         insert( "radius", int( RADIUS_TOK ) );
         insert( "rainbow", int( RAINBOW_TOK ) );
         insert( "ramp_wave", int( RAMP_WAVE_TOK ) );
         insert( "rand", int( RAND_TOK ) );
         insert( "ratio", int( RATIO_TOK ) );
         insert( "reciprocal", int( RECIPROCAL_TOK ) );
         insert( "recursion_limit", int( RECURSION_LIMIT_TOK ) );
         insert( "red", int( RED_TOK ) );
         insert( "reflection", int( REFLECTION_TOK ) );
         insert( "reflection_exponent", int( REFLECTION_EXPONENT_TOK ) );
         insert( "refraction", int( REFRACTION_TOK ) );
         insert( "repeat", int( REPEAT_TOK ) );
         insert( "rgb", int( RGB_TOK ) );
         insert( "rgbf", int( RGBF_TOK ) );
         insert( "rgbft", int( RGBFT_TOK ) );
         insert( "rgbt", int( RGBT_TOK ) );
         insert( "right", int( RIGHT_TOK ) );
         insert( "ripples", int( RIPPLES_TOK ) );
         insert( "rotate", int( ROTATE_TOK ) );
         insert( "roughness", int( ROUGHNESS_TOK ) );
         insert( "samples", int( SAMPLES_TOK ) );
         insert( "scale", int( SCALE_TOK ) );
         insert( "scallop_wave", int( SCALLOP_WAVE_TOK ) );
         insert( "scattering", int( SCATTERING_TOK ) );
         insert( "seed", int( SEED_TOK ) );
         insert( "shadowless", int( SHADOWLESS_TOK ) );
         insert( "sin", int( SIN_TOK ) );
         insert( "sine_wave", int( SINE_WAVE_TOK ) );
         insert( "sinh", int( SINH_TOK ) );
         insert( "sky", int( SKY_TOK ) );
         insert( "sky_sphere", int( SKY_SPHERE_TOK ) );
         insert( "slice", int( SLICE_TOK ) );
         insert( "slope", int( SLOPE_TOK ) );
         insert( "slope_map", int( SLOPE_MAP_TOK ) );
         insert( "smooth", int( SMOOTH_TOK ) );
         insert( "smooth_triangle", int( SMOOTH_TRIANGLE_TOK ) );
         insert( "solid", int( SOLID_TOK ) );
         insert( "sor", int( SOR_TOK ) );
         insert( "spacing", int ( SPACING_TOK ) );
         insert( "specular", int( SPECULAR_TOK ) );
         insert( "sphere", int( SPHERE_TOK ) );
         insert( "sphere_sweep", int ( SPHERE_SWEEP_TOK ) );
         insert( "spherical", int( SPHERICAL_TOK ) );
         insert( "spiral", int( SPIRAL_TOK ) );
         insert( "spiral1", int( SPIRAL1_TOK ) );
         insert( "spiral2", int( SPIRAL2_TOK ) );
         insert( "srgb", int( SRGB_TOK ) );
         insert( "tiling", int( TILING_TOK ) );
         insert( "spotlight", int( SPOTLIGHT_TOK ) );
         insert( "spotted", int( SPOTTED_TOK ) );
         insert( "sqr", int( SQR_TOK ) );
         insert( "sqrt", int( SQRT_TOK ) );
         insert( "str", int( STR_TOK ) );
         insert( "strcmp", int( STRCMP_TOK ) );
         insert( "strength", int( STRENGTH_TOK ) );
         insert( "strlen", int( STRLEN_TOK ) );
         insert( "strlwr", int( STRLWR_TOK ) );
         insert( "strupr", int( STRUPR_TOK ) );
         insert( "sturm", int( STURM_TOK ) );
         insert( "substr", int( SUBSTR_TOK ) );
         insert( "superellipsoid", int( SUPERELLIPSOID_TOK ) );
         insert( "sys", int( SYS_TOK ) );
         insert( "t", int( T_TOK ) );
         insert( "tan", int( TAN_TOK ) );
         insert( "tanh", int( TANH_TOK ) );
         insert( "target", int( TARGET_TOK ) );
         insert( "test_camera_1", int( TEST_CAMERA_1_TOK ) );
         insert( "test_camera_2", int( TEST_CAMERA_2_TOK ) );
         insert( "test_camera_3", int( TEST_CAMERA_3_TOK ) );
         insert( "test_camera_4", int( TEST_CAMERA_4_TOK ) );
         insert( "text", int( TEXT_TOK ) );
         insert( "texture", int( TEXTURE_TOK ) );
         insert( "texture_map", int( TEXTURE_MAP_TOK ) );
         insert( "tga", int( TGA_TOK ) );
         insert( "thickness", int( THICKNESS_TOK ) );
         insert( "threshold", int( THRESHOLD_TOK ) );
         insert( "tightness", int( TIGHTNESS_TOK ) );
         insert( "tile2", int( TILE2_TOK ) );
         insert( "tiles", int( TILES_TOK ) );
         insert( "tolerance", int( TOLERANCE_TOK ) );
         insert( "toroidal", int( TOROIDAL_TOK ) );
         insert( "torus", int( TORUS_TOK ) );
         insert( "track", int( TRACK_TOK ) );
         insert( "transform", int( TRANSFORM_TOK ) );
         insert( "translate", int( TRANSLATE_TOK ) );
         insert( "transmit", int( TRANSMIT_TOK ) );
         insert( "triangle", int( TRIANGLE_TOK ) );
         insert( "triangle_wave", int( TRIANGLE_WAVE_TOK ) );
         insert( "true", int( TRUE_TOK ) );
         insert( "ttf", int( TTF_TOK ) );
         insert( "turbulence", int( TURBULENCE_TOK ) );
         insert( "turb_depth", int( TURB_DEPTH_TOK ) );
         insert( "type", int( TYPE_TOK ) );
         insert( "u", int( U_TOK ) );
         insert( "ultra_wide_angle", int( ULTRA_WIDE_ANGLE_TOK ) );
         insert( "union", int( UNION_TOK ) );
         insert( "up", int( UP_TOK ) );
         insert( "use_color", int( USE_COLOR_TOK ) );
         insert( "use_colour", int( USE_COLOUR_TOK ) );
         insert( "use_index", int( USE_INDEX_TOK ) );
         insert( "u_steps", int( U_STEPS_TOK ) );
         insert( "uv_mapping", int( UV_MAPPING_TOK ) );
         insert( "uv_vectors", int( UV_VECTORS_TOK ) );
         insert( "v", int( V_TOK ) );
         insert( "val", int( VAL_TOK ) );
         insert( "variance", int( VARIANCE_TOK ) );
         insert( "vaxis_rotate", int( VAXIS_ROTATE_TOK ) );
         insert( "vcross", int( VCROSS_TOK ) );
         insert( "vdot", int( VDOT_TOK ) );
         insert( "vlength", int( VLENGTH_TOK ) );
         insert( "vnormalize", int( VNORMALIZE_TOK ) );
         insert( "volume_object", int( VOLUME_OBJECT_TOK ) );
         insert( "volume_rendered", int( VOLUME_RENDERED_TOK ) );
         insert( "vol_with_light", int( VOL_WITH_LIGHT_TOK ) );
         insert( "vrotate", int( VROTATE_TOK ) );
         insert( "v_steps", int( V_STEPS_TOK ) );
         insert( "warp", int( WARP_TOK ) );
         insert( "water_level", int( WATER_LEVEL_TOK ) );
         insert( "waves", int( WAVES_TOK ) );
         insert( "width", int( WIDTH_TOK ) );
         insert( "wood", int( WOOD_TOK ) );
         insert( "wrinkles", int( WRINKLES_TOK ) );
         insert( "x", int( X_TOK ) );
         insert( "y", int( Y_TOK ) );
         insert( "yes", int( YES_TOK ) );
         insert( "z", int( Z_TOK ) );
         break;
      case PMDDirectives:
         insert( "break", int( BREAK_TOK ) );
         insert( "case", int( CASE_TOK ) );
         insert( "debug", int( DEBUG_TOK ) );
         insert( "declare", int( DECLARE_TOK ) );
         insert( "default", int( DEFAULT_TOK ) );
         insert( "else", int( ELSE_TOK ) );
         insert( "end", int( END_TOK ) );
         insert( "if", int( IF_TOK ) );
         insert( "ifdef", int( IFDEF_TOK ) );
         insert( "ifndef", int( IFNDEF_TOK ) );
         insert( "include", int( INCLUDE_TOK ) );
         insert( "range", int( RANGE_TOK ) );
         insert( "render", int( RENDER_TOK ) );
         insert( "statistics", int( STATISTICS_TOK ) );
         insert( "switch", int( SWITCH_TOK ) );
         insert( "version", int( VERSION_TOK ) );
         insert( "warning", int( WARNING_TOK ) );
         insert( "while", int( WHILE_TOK ) );
         break;
   }
}

PMReservedWordDict::~PMReservedWordDict()
{
}

PMReservedWordDict PMScanner::m_reservedWords( PMReservedWordDict::PMDReservedWords );
PMReservedWordDict PMScanner::m_directives( PMReservedWordDict::PMDDirectives );

const char* c_commentName = "*PMName ";
const int c_commentNameLength = 8;

const char* c_commentRawBegin = "*PMRawBegin";
const int c_commentRawBeginLength = 11;

const char* c_commentRawEnd = "//*PMRawEnd";
const int c_commentRawEndLength = 11;


PMScanner::PMScanner( QIODevice* device )
{
   m_svalueAlloc = 256;
   m_svalue = ( char* ) malloc( m_svalueAlloc );
   m_svalue[0] = '\0';
   m_lastAlloc = m_svalue + m_svalueAlloc;
   m_lastChar = m_svalue;

   m_ivalue = 0;
   m_fvalue = 0;
   m_pDevice = device;
   m_line = 1;
   m_char = 0;
   m_indentation = 0;
   m_rawIndentation = 0;
   m_bFunctionMode = false;

//   m_lineData = "";
//   m_lineDataPos = 100;
//   m_lineDataLength = 0;
   nextChar();
}

PMScanner::~PMScanner()
{
   if( m_svalue )
      free( m_svalue );
}

void PMScanner::nextChar()
{
   do
   {
      if (m_pDevice->atEnd())
         m_char = -1;
      else 
         m_char = m_pDevice->read( 1 ).at( 0 );
   }
   while( m_char == '\r' );
}

void PMScanner::clearSValue()
{
   m_svalue[0] = '\0';
   m_lastChar = m_svalue;
}

void PMScanner::addChar( char c )
{
   *m_lastChar = c;
   m_lastChar++;

   if( m_lastChar == m_lastAlloc )
   {
      m_svalueAlloc += 64;
      m_svalue = ( char* ) realloc( m_svalue, m_svalueAlloc );
      m_lastAlloc = m_svalue + m_svalueAlloc;
      m_lastChar = m_lastAlloc - 64;
   }

   *m_lastChar = '\0';
}

bool PMScanner::isseparation( int c )
{
   if( c < 0 )
      return true;
   if( isspace( c ) )
      return true;
   switch( c )
   {
      case '{':
      case '}':
      case '<':
      case '>':
      case '(':
      case ')':
      case '[':
      case ']':
      case '+':
      case '-':
      case '*':
      case '/':
      case ',':
      case ';':
      case '=':
      case '.':
         return true;
         break;
      default:
         return false;
   }
   return false;
}


void PMScanner::scanError( int c )
{
   m_token = SCANNER_ERROR_TOK;
   if( isprint( c ) )
      m_error = ( "Unexpected character " + QString( ( char )c ) + " after " + m_svalue );
   else
      m_error = ( "Unexpected char " + QString::number( c ) + " after " + m_svalue );


#ifdef PMSCAN_DEBUG
   qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
}


int PMScanner::nextToken()
{
   int status = START_ST;
   int cdepth = 0;
   bool consumed;
   bool end = false;

   clearSValue();
   m_ivalue = 0;
   m_fvalue = 0;
//   m_error = "";

   if( m_bFunctionMode )
   {
      m_bFunctionMode = false;
      // FIXME: TODO brackets in comments will not be scanned correctly
      int count = 1;

      while( count > 0 )
      {
         if( m_char < 0 )
            count = 0;
         else if( m_char == '{' )
            count++;
         else if( m_char == '}' )
            count--;
         if( count > 0 )
         {
            addChar( m_char );
            nextChar();
         }
      }

      if( m_char != '}' )
      {
         m_error = ( "Function statement not terminated" );
#ifdef PMSCAN_DEBUG
         qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
         m_token = SCANNER_ERROR_TOK;
      }
      else
         m_token = FUNCTION_TOK;
   }
   else while( status != TOKEN_END_ST )
   {
      consumed = true;
      if( m_char < 0 )
         end = true;
      switch( status )
      {
         case START_ST:   //begin
            if( m_char < 0 )
            {
               m_token = EOF_TOK;
               addChar( 'E' );
               addChar( 'O' );
               addChar( 'F' );
               status = TOKEN_END_ST;
               consumed = false;
               break;
            }
            if( m_char == '\n' )
            {
               m_line ++;
               m_indentation = 0;
               break;
            }
            if( m_char == ' ' )
            {
               m_indentation++;
               break;
            }
            if( m_char == '\t' )
            {
               m_indentation += 8 - m_indentation % 8;
               break;
            }
            if( iscntrl( m_char ) )
               break;
            if( isspace( m_char ) )
               break;
            if( isalpha( m_char ) || ( m_char == '_' ) )
            {
               status = ID_ENDST;
               addChar( m_char );
               break;
            }
            if( isdigit( m_char ) )
            {
               status = INTEGER_ENDST;
               addChar( m_char );
               break;
            }
            switch( m_char )
            {
               case '.':
                  status = POINT_ST;
                  addChar( m_char );
                  break;
               case '#':
                  status = DIRECTIVE1_ST;
                  break;
               case '"':
                  status = STRING1_ST;
                  break;
               case '/':
                  status = SLASH_ST;
                  break;
               default:
                  addChar( m_char );
                  m_token = m_char;
                  status = TOKEN_END_ST;

#ifdef PMSCAN_DEBUG
                  qDebug() << "Line " << m_line << ": Single char '"
                                    << QString( QChar( ( char ) m_char ) ) << "'\n";
#endif
                  break;
            }
            break;
         case ID_ENDST:                // indentifier or reserved word
            if( isalnum( m_char ) || ( m_char == '_' ) )
            {
               addChar( m_char );
               break;
            }
            else if( isseparation( m_char ) )
            {
               consumed = false;
               m_token = m_reservedWords[ m_svalue ];//eticre import srgb
               if( m_token < 0 )
                  m_token = ID_TOK;

#ifdef PMSCAN_DEBUG
               if( m_token == ID_TOK )
                  qDebug() << "Line " << m_line << ": Indentifier: \""
                                    << m_svalue << "\"\n";
               else
                  qDebug() << "Line " << m_line << ": Reserved word: \""
                                    << m_svalue << "\"\n";
#endif


               status = TOKEN_END_ST;
            }
            else
            {
               status = TOKEN_END_ST;
               scanError( m_char );
               consumed = false;
            }
            break;
         case INTEGER_ENDST:
            if( isdigit ( m_char ) )
            {
               addChar( m_char );
               break;
            }
            else if( m_char == '.' )
            {
               status = FLOAT1_ST;
               addChar( m_char );
               break;
            }
            else if( ( m_char == 'e' ) || ( m_char == 'E' ) )
            {
               status = FLOAT_EXP1_ST;
               addChar( m_char );
               break;
            }
            else if( isseparation( m_char ) )
            {
               consumed = false;
               m_ivalue = atoi( m_svalue );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Integer: "
                                 << m_ivalue << "\n";
#endif
               m_token = INTEGER_TOK;
               status = TOKEN_END_ST;
            }
            else
            {
               status = TOKEN_END_ST;
               scanError( m_char );
               consumed = false;
            }
            break;
         case POINT_ST:
            if( isdigit( m_char ) )
            {
               status = FLOAT_ENDST;
               addChar( m_char );
            }
            else
            {
               status = TOKEN_END_ST;
               consumed = false;
               m_token = '.';
            }
            break;
         case FLOAT1_ST:
            if( isdigit( m_char ) )
            {
               status = FLOAT_ENDST;
               addChar( m_char );
               break;
            }
            else
            {
               scanError( m_char );
               status = TOKEN_END_ST;
               consumed = false;
            }
            break;
         case FLOAT_ENDST:
            if( isdigit ( m_char ) )
            {
               addChar( m_char );
               break;
            }
            if( ( m_char == 'e' ) || ( m_char == 'E' ) )
            {
               status = FLOAT_EXP1_ST;
               addChar( m_char );
               break;
            }
            else if( isseparation( m_char ) )
            {
               consumed = false;
               m_fvalue = QByteArray( m_svalue ).toDouble();
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Float: "
                                 << m_fvalue << "\n";
#endif
               m_token = FLOAT_TOK;
               status = TOKEN_END_ST;
            }
            else
            {
               scanError( m_char );
               status = TOKEN_END_ST;
               consumed = false;
            }
            break;
         case FLOAT_EXP1_ST:
            if( ( m_char == '-' ) || ( m_char == '+' ) )
            {
               status = FLOAT_EXP2_ST;
               addChar( m_char );
               break;
            }
            if( isdigit( m_char ) )
            {
               status = FLOAT_EXP_ENDST;
               addChar( m_char );
               break;
            }
            else
            {
               scanError( m_char );
               consumed = false;
               status = TOKEN_END_ST;
            }
            break;
         case FLOAT_EXP2_ST:
            if( isdigit( m_char ) )
            {
               status = FLOAT_EXP_ENDST;
               addChar( m_char );
               break;
            }
            else
            {
               scanError( m_char );
               consumed = false;
               status = TOKEN_END_ST;
            }
            break;
         case FLOAT_EXP_ENDST:
            if( isdigit ( m_char ) )
            {
               addChar( m_char );
               break;
            }
            else if( isseparation( m_char ) )
            {
               consumed = false;
               m_fvalue = QByteArray( m_svalue ).toDouble();
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Float: "
                                 << m_fvalue << "\n";
#endif
               m_token = FLOAT_TOK;
               status = TOKEN_END_ST;
            }
            else
            {
               scanError( m_char );
               consumed = false;
               status = TOKEN_END_ST;
            }
            break;
         case DIRECTIVE1_ST:
            if( m_char == ' ' )
            {
               // special treatment for povray inc files
               // "#   debug" directives
               break;
            }
            else if( isalpha ( m_char ) )
            {
               status = DIRECTIVE_ENDST;
               addChar( m_char );
               break;
            }
            else
            {
               scanError( m_char );
               consumed = false;
               status = TOKEN_END_ST;
            }
            break;
         case DIRECTIVE_ENDST:
            if( isalpha( m_char ) )
            {
               addChar( m_char );
               break;
            }
            else if( isseparation( m_char ) )
            {
               consumed = false;
#ifdef PMSCAN_DEBUG
                  qDebug() << "Line " << m_line << ": Directive: \""
                                    << m_svalue << "\"\n";
#endif
               m_token = m_directives[ m_svalue ];
               if( m_token < 0 )
               {
                  m_error = ( "Unknown directive" );
#ifdef PMSCAN_DEBUG
                  qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif

                  m_token = SCANNER_ERROR_TOK;
               }
               status = TOKEN_END_ST;
            }
            else
            {
               scanError( m_char );
               consumed = false;
               status = TOKEN_END_ST;
            }
            break;
         case STRING1_ST:
            switch( m_char )
            {
               case '\n':
               case '\r':
                  consumed = false;
                  m_error = ( "String not terminated" );
                  m_token = SCANNER_ERROR_TOK;
                  status = TOKEN_END_ST;
#ifdef PMSCAN_DEBUG
                  qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
                  break;
               case '"':
#ifdef PMSCAN_DEBUG
                  qDebug() << "Line " << m_line << ": String: \""
                                    << m_svalue << "\"\n";
#endif

                  m_token = STRING_TOK;
                  status = TOKEN_END_ST;
                  break;
               case '\\':
                  status = STRINGBS_ST;
                  addChar( m_char );
                  break;
               default:
                  addChar( m_char );
                  break;
            }
            break;
         case STRINGBS_ST:
            if( ( m_char == '\n' ) || ( m_char == '\r' ) || ( m_char < 0 ) )
            {
               consumed = false;
               m_error = ( "String not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
               break;
            }
            else
            {
               status = STRING1_ST;
               addChar( m_char );
               break;
            }
            break;
         case SLASH_ST:
            switch( m_char )
            {
               case '/':
                  status = LINE_COMMENT_FIRST_ST;
                  break;
               case '*':
                  status = COMMENT_NEW_LINE_ST;
                  cdepth ++;
                  break;
               default:
                  consumed = false;
                  m_token = '/';
                  status = TOKEN_END_ST;
            }
            break;
         case LINE_COMMENT_FIRST_ST:
            // skip the first space char
            if( ( m_char == '\n' ) || ( m_char == '\r' ) || ( m_char < 0 ) )
            {
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Empty line comment\n";
#endif
               consumed = false;
               m_token = LINE_COMMENT_TOK;
               status = TOKEN_END_ST;
               break;
            }
            else if( !isspace( m_char ) )
               addChar( m_char );
            status = LINE_COMMENT_ST;
            break;
         case LINE_COMMENT_ST:
            if( ( m_char == '\n' ) || ( m_char == '\r' ) || ( m_char < 0 ) )
            {
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Line comment: \""
                                 << m_svalue << "\"\n";
#endif
               consumed = false;
               m_token = LINE_COMMENT_TOK;
               status = TOKEN_END_ST;
               break;
            }
            else
            {
               addChar( m_char );
               int l = m_lastChar - m_svalue;
               if( l == c_commentNameLength )
               {
                  if( strcmp( m_svalue, c_commentName ) == 0 )
                  {
                     status = PMNAME_ST;
                     clearSValue();
                  }
               }
               if( l == c_commentRawBeginLength )
               {
                  if( strcmp( m_svalue, c_commentRawBegin ) == 0 )
                  {
                     status = RAW_POVRAY_FIRST_ST;
                     clearSValue();
                  }
               }
               break;
            }
            break;
         case COMMENT_NEW_LINE_ST:
            // skip any white spaces at begin of line.
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Comment not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            else if( m_char == '\n' )
            {
               addChar( '\n' );
               m_line ++;
            }
            else if( !isspace( m_char ) )
            {
               consumed = false;
               status = C_COMMENT_ST;
            }
            break;
         case C_COMMENT_ST:
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Comment not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            else
            {
               switch( m_char )
               {
                  case '*':
                     status = COMMENT_ST_ST;
                     break;
                  case '/':
                     status = COMMENT_SL_ST;
                     addChar( m_char );
                     break;
                  case '\n':
                     addChar( m_char );
                     m_line ++;
                     status = COMMENT_NEW_LINE_ST;
                     break;
                  default:
                     addChar( m_char );
               }
            }
            break;
         case COMMENT_ST_ST:
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Comment not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            else
            {
               switch( m_char )
               {
                  case '/':
                     cdepth --;
                     if( cdepth == 0 )
                     {
#ifdef PMSCAN_DEBUG
                        qDebug() << "Line " << m_line << ": Comment: \""
                                          << m_svalue << "\"\n";
#endif
                        m_token = COMMENT_TOK;
                        status = TOKEN_END_ST;
                        break;
                     }
                     else
                     {
                        status = C_COMMENT_ST;
                        addChar( '*' );
                        addChar( '/' );
                        break;
                     }
                  case '*':
                     addChar( m_char );
                     break;
                  case '\n':
                     status = C_COMMENT_ST;
                     addChar( '*' );
                     addChar( m_char );
                     m_line ++;
                     break;
                  default:
                     status = C_COMMENT_ST;
                     addChar( '*' );
                     addChar( m_char );
                     break;
               }
            }
            break;
         case COMMENT_SL_ST:
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Comment not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            else
            {
               switch( m_char )
               {
                  case '/':
                     addChar( m_char );
                     break;
                  case '*':
                     status = C_COMMENT_ST;
                     addChar( m_char );
                     cdepth ++;
                     break;
                  case '\n':
                     status = C_COMMENT_ST;
                     addChar( m_char );
                     m_line ++;
                     break;
                  default:
                     status = C_COMMENT_ST;
                     addChar( m_char );
               }
            }
            break;
         case PMNAME_ST:
            if( ( m_char == '\n' ) || ( m_char == '\r' ) || ( m_char < 0 ) )
            {
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": PMName: \""
                                 << m_svalue << "\"\n";
#endif
               consumed = false;
               m_token = PMNAME_TOK;
               status = TOKEN_END_ST;
               break;
            }
            else
            {
               addChar( m_char );
               break;
            }
            break;
         case RAW_POVRAY_FIRST_ST: // skip the first line
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Raw povray not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            if( m_char == '\n' )
            {
               m_line++;
               m_rawIndentation = 0;
               status = RAW_POVRAY_LB_ST;
            }
            break;
         case RAW_POVRAY_LB_ST:
            switch( m_char )
            {
               case '\n':
                  addChar( m_char );
                  m_line++;
                  m_rawIndentation = 0;
                  // status = RAW_POVRAY_LB_ST;
                  break;
               case ' ':
                  m_rawIndentation++;
                  if( m_rawIndentation >= m_indentation )
                     status = RAW_POVRAY_ST;
                  break;
               case '\t':
                  m_rawIndentation += 8 - m_rawIndentation % 8;
                  if( m_rawIndentation >= m_indentation )
                     status = RAW_POVRAY_ST;
                  break;
               default:
                  consumed = false;
                  status = RAW_POVRAY_ST;
                  break;
            }
            break;
         case RAW_POVRAY_ST:
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Raw povray not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            else if( m_char == '\n' )
            {
               addChar( m_char );
               m_line++;
               m_rawIndentation = 0;
               status = RAW_POVRAY_LB_ST;
            }
            else if( m_char == '/' )
            {
               m_rawPovrayEnd = 1;
               status = RAW_POVRAY_END_ST;
            }
            else
               addChar( m_char );
            break;
         case RAW_POVRAY_END_ST:
            if( m_char < 0 )
            {
               consumed = false;
               m_error = ( "Raw povray not terminated" );
#ifdef PMSCAN_DEBUG
               qDebug() << "Line " << m_line << ": Error " << m_error << "\n";
#endif
               m_token = SCANNER_ERROR_TOK;
               status = TOKEN_END_ST;
            }
            else if( m_char != c_commentRawEnd[m_rawPovrayEnd] )
            {
               status = RAW_POVRAY_ST;
               int i;
               for( i = 0; i < m_rawPovrayEnd; i++ )
                  addChar( c_commentRawEnd[i] );
               consumed = false;
            }
            else
            {
               m_rawPovrayEnd++;
               if( m_rawPovrayEnd >= c_commentRawEndLength )
               {
                  status = RAW_POVRAY_END_END_ST;
                  if( m_lastChar > m_svalue )
                  {
                     if( *( m_lastChar - 1 ) == '\n' )
                     {
                        m_lastChar--;
                        *m_lastChar = 0;
                     }
                  }
               }
            }
            break;
         case RAW_POVRAY_END_END_ST:
            if( ( m_char < 0 ) || ( m_char == '\n' ) )
            {
               consumed = false;
               status = TOKEN_END_ST;
               m_token = RAW_POVRAY_TOK;
            }
            break;
         case TOKEN_END_ST:
            break;
      }
      if( consumed )
         nextChar();
      if( end && ( status != TOKEN_END_ST ) )
      {
         status = TOKEN_END_ST;
         qCritical(  ) << "Error in scanner: No TOKEN_END_ST after EOF\n";
      }
   }

   return m_token;
}

void PMScanner::scanFunction()
{
   m_bFunctionMode = true;
}
