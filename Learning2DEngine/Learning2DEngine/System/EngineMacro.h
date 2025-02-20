#pragma once
// The developer can define them in a game project => Properties => C/C++ => Preprocesssor => Preprocesssor Definitions

// If it is 0, it does not use thread.
#ifndef L2DE_MAX_COMPONENT_PER_THREAD
	#define L2DE_MAX_COMPONENT_PER_THREAD 0
#endif

// If it is 0, Update does not use thread
#ifndef L2DE_UPDATE_MAX_COMPONENT_PER_THREAD
	#define L2DE_UPDATE_MAX_COMPONENT_PER_THREAD L2DE_MAX_COMPONENT_PER_THREAD
#endif

// If it is 0, LateUpdate does not use thread
#ifndef L2DE_LATEUPDATE_MAX_COMPONENT_PER_THREAD
	#define L2DE_LATEUPDATE_MAX_COMPONENT_PER_THREAD L2DE_MAX_COMPONENT_PER_THREAD
#endif

// If it is 0, LateUpdate does not use thread
#ifndef L2DE_COLLIDER_MAX_COMPONENT_PER_THREAD
	#define L2DE_COLLIDER_MAX_COMPONENT_PER_THREAD L2DE_MAX_COMPONENT_PER_THREAD
#endif