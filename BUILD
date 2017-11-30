package(default_visibility = ["//visibility:public"])

cc_library(
	name = "die-tk-base",
	srcs = glob([
		"src/*.cpp",
		"src/components/*.cpp",
		"src/controls/*.cpp",
		"src/objects/*.cpp",
		"src/portable/*.cpp",
		"src/util/*.cpp",
	]),
	hdrs = glob([
		"src/*.h",
		"src/components/*.h",
		"src/controls/*.h",
		"src/objects/*.h",
		"src/portable/*.hpp",
		"src/util/*.h",
		"src/util/*.hpp",
	]),
)

cc_library(
	name = "die-tk-win32",
	srcs = glob([
		"win32/*.cpp",
		"win32/components/*.cpp",
		"win32/controls/*.cpp",
	]),
	hdrs = glob([
		"win32/*.h",
		"win32/components/*.h",
		"win32/controls/*.h",
	]),
	deps = [
		":die-tk-base",
		"@win32_libs//:win32",
	],
)

cc_library(
	name = "die-tk-linux",
	srcs = glob([
		"linux/*.cpp",
		"linux/components/*.cpp",
		"linux/controls/*.cpp",
		"linux/utf8/*.cpp",
	]),
	hdrs = glob([
		"linux/*.h",
		"linux/components/*.h",
		"linux/controls/*.h",
		"linux/utf8/*.h",
	]),
	deps = [
		":die-tk-base",
		"@linux_libs//:x11",
	],
	copts = ["--std=c++1y"],
)
