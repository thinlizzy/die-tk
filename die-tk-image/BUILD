package(default_visibility = ["//visibility:public"])

cc_library(
	name = "die-tk-image-base",
	hdrs = [
		"src/convert.h",
	],
	deps = [
		"//:die-tk-base",
		"@libimage//:libimage",
	],
)

cc_library(
	name = "die-tk-image-win32",
	srcs = [
		"win32/convertImage.cpp",
	],
	deps = [
		":die-tk-image-base",
		"//:die-tk-win32",
	],
)

cc_library(
	name = "die-tk-image-linux",
	srcs = [
		"linux/convertImage.cpp",
	],
	deps = [
		":die-tk-image-base",
		"//:die-tk-linux",
	],
)
