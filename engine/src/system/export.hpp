#ifndef HYPER_EXPORT_HPP
#define HYPER_EXPORT_HPP

#ifdef HYPER_API_EXPORTS
	#define HYPER_API __declspec(dllexport)
#elif HYPER_STATIC_EXPORTS
	#define HYPER_API
#else
	#define HYPER_API __declspec(dllimport)
#endif

#endif
