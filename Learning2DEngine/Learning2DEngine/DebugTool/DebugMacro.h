#pragma once


#ifndef L2DE_DEBUG
	#define L2DE_DEBUG (_DEBUG || 0)
#endif

#if L2DE_DEBUG && !defined(L2DE_DEBUG_MAX_BASE_DELTATIME)
	#define L2DE_DEBUG_MAX_BASE_DELTATIME 0.1f
#endif