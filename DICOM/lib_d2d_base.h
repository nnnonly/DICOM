#pragma once

#ifndef LIB_EXT_D2D

	#ifndef BUILD_LIB
		#pragma comment(lib, "lib_d2d.lib")
		#define LIB_EXT_D2D __declspec(dllexport)

	#else

		#define LIB_EXT_D2D __declspec(dllimport)

	#endif // !BUILD_LIB

#endif // !LIB_EXT_D2D
