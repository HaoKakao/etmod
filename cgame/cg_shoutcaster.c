// cg_shoutcaster.c

#include "cg_local.h"

/*
================
CG_WorldToScreen

Take any world coord and convert it to a 2D virtual 640x480 screen coord
================
*/
qboolean CG_WorldToScreen( vec3_t point, float *x, float *y )
{
	vec3_t	trans;
	float	z, xc, yc, px, py;

	VectorSubtract( point, cg.refdef_current->vieworg, trans );
	z = DotProduct( trans, cg.refdef_current->viewaxis[0] );

	if( z <= .001f ) {
		return qfalse;
	}

	xc = 640.f / 2.f;
	yc = 480.f / 2.f;

	px = tan( cg.refdef_current->fov_x * M_PI / 360.f );
	py = tan( cg.refdef_current->fov_y * M_PI / 360.f );

	*x = xc - DotProduct( trans, cg.refdef_current->viewaxis[1] ) *
		xc / ( z * px );
	*y = yc - DotProduct( trans, cg.refdef_current->viewaxis[2] ) *
		yc / ( z * py );

	return qtrue;
}

/*
================
CG_PointIsVisible

Is point visible from camera viewpoint?
================
*/
qboolean CG_PointIsVisible( vec3_t point )
{
	trace_t	trace;

	CG_Trace( &trace, cg.refdef_current->vieworg, NULL, NULL,
		point, -1, CONTENTS_SOLID );

	if( trace.fraction < 1.f ) {
		return qfalse;
	}

	return qtrue;
}

/*
================
CG_DrawLandmine
================
*/
void CG_DrawLandmine( centity_t *cent, refEntity_t *ent ) {
	int color = ( int )255 - ( 255 * fabs( sin( cg.time * 0.002 ) ) );

	if( cent->currentState.teamNum % 4 == TEAM_AXIS ) {
		// red landmines
		ent->shaderRGBA[0] = 255;
		ent->shaderRGBA[1] = color;
		ent->shaderRGBA[2] = color;
		ent->shaderRGBA[3] = 255;
	} else {
		// blue landmines
		ent->shaderRGBA[0] = color;
		ent->shaderRGBA[1] = color;
		ent->shaderRGBA[2] = 255;
		ent->shaderRGBA[3] = 255;
	}

	ent->customShader = cgs.media.shoutcastLandmineShader;
}
