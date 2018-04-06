package(default_visibility = ["//visibility:public"])

cc_library(
	name = "die-tk-image-base",
	hdrs = [
		"convert.h",
	],
	deps = [
		"//:die-tk-base",
		"@libimage//image",
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

cc_library(
	name = "die-tk-image",
	deps = select({
		"@bazel_tools//src/conditions:windows": [":die-tk-image-win32"],
		"//conditions:default": [":die-tk-image-linux"],
	}),
)
