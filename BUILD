package(default_visibility = ["//visibility:public"])

cc_library(
	name = "die-tk-base",
	srcs = glob([
		"die-tk/*.cpp",
		"die-tk/components/*.cpp",
		"die-tk/controls/*.cpp",
		"die-tk/objects/*.cpp",
		"die-tk/portable/*.cpp",
		"die-tk/util/*.cpp",
	]),
	hdrs = glob([
		"die-tk/*.h",
		"die-tk/components/*.h",
		"die-tk/controls/*.h",
		"die-tk/objects/*.h",
		"die-tk/portable/*.hpp",
		"die-tk/util/*.h",
		"die-tk/util/*.hpp",
	]),
)

cc_library(
	name = "die-tk-win32",
	srcs = glob([
		"die-tk/win32/*.cpp",
		"die-tk/win32/components/*.cpp",
		"die-tk/win32/controls/*.cpp",
	]),
	hdrs = glob([
		"die-tk/win32/*.h",
		"die-tk/win32/components/*.h",
		"die-tk/win32/controls/*.h",
	]),
	deps = [
		":die-tk-base",
		"@win32_libs//:win32",
	],
)

cc_library(
	name = "die-tk-linux",
	srcs = glob([
		"die-tk/linux/*.cpp",
		"die-tk/linux/components/*.cpp",
		"die-tk/linux/controls/*.cpp",
		"die-tk/linux/utf8/*.cpp",
	]),
	hdrs = glob([
		"die-tk/linux/*.h",
		"die-tk/linux/components/*.h",
		"die-tk/linux/controls/*.h",
		"die-tk/linux/utf8/*.h",
	]),
	deps = [
		":die-tk-base",
		"@linux_libs//:x11",
	],
	copts = ["--std=c++1y"],
)
