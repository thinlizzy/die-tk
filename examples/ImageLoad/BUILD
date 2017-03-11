cc_library(
	name = "imageLoad",
	srcs = ["imageLoad.cpp"],
	deps = [
		"//die-tk:die-tk-base",
		"//libimage",
	],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
	],
)

cc_binary(
	name = "imageLoad-win32",
	deps = [
		":imageLoad",
		"//die-tk:die-tk-win32",
		"@win32_libs//:gdi32",
		"@win32_user_libs//:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "imageLoad-linux",
	deps = [
		":imageLoad",
		"//die-tk:die-tk-linux",
		"@linux_libs//:x11",
		"@linux_libs//:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
)
