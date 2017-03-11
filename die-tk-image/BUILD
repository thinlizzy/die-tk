package(default_visibility = ["//visibility:public"])

cc_library(
	name = "die-tk-image-base",
	hdrs = [
		"src/convert.h",
	],
	deps = [
		"//die-tk:die-tk-base",
		"//libimage",
	],
	copts = [
		"--std=c++1y",
	],
	linkstatic = 1,
)

cc_library(
	name = "die-tk-image-win32",
	srcs = [
		"win32/convertImage.cpp",
	],
	deps = [
		":die-tk-image-base",
		"//die-tk:die-tk-win32",
	],
	copts = ["--std=c++1y"],
	linkstatic = 1,
)

cc_library(
	name = "die-tk-image-linux",
	srcs = [
		"linux/convertImage.cpp",
	],
	deps = [
		":die-tk-image-base",
		"//die-tk:die-tk-linux",
	],
	copts = ["--std=c++1y"],
	linkstatic = 1,
)
