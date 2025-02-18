#pragma once

// If it is 0, it does not use thread.
// The developer have to define it in game project => Properties => C/C++ => Preprocesssor => Preprocesssor Definitions
//#ifndef L2DE_MAX_COMPONENT_PER_THREAD
//	#define L2DE_MAX_COMPONENT_PER_THREAD 0
//#endif

// If it is 0, Update does not use thread
#ifndef L2DE_UPDATE_MAX_COMPONENT_PER_THREAD
	#define L2DE_UPDATE_MAX_COMPONENT_PER_THREAD (L2DE_MAX_COMPONENT_PER_THREAD || 0)
#endif

// If it is 0, LateUpdate does not use thread
#ifndef L2DE_LATEUPDATE_MAX_COMPONENT_PER_THREAD
	#define L2DE_LATEUPDATE_MAX_COMPONENT_PER_THREAD (L2DE_MAX_COMPONENT_PER_THREAD || 0)
#endif

// If it is 0, LateUpdate does not use thread
#ifndef L2DE_COLLIDER_MAX_COMPONENT_PER_THREAD
	#define L2DE_COLLIDER_MAX_COMPONENT_PER_THREAD (L2DE_MAX_COMPONENT_PER_THREAD || 0)
#endif

// It is 1 if the Engine use thread somewhere.
// It can be useful if Engine should use mutexes. (The ComponentHandlers can use mutex)
#if !defined(L2DE_USE_THREAD) && (L2DE_UPDATE_MAX_COMPONENT_PER_THREAD > 0 || L2DE_LATEUPDATE_MAX_COMPONENT_PER_THREAD > 0 || L2DE_COLLIDER_MAX_COMPONENT_PER_THREAD > 0)
	#define L2DE_USE_THREAD 1
#else
	#define L2DE_USE_THREAD 0
#endif