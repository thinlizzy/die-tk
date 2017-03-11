cc_library(
	name = "paintboxes",
	srcs = ["paintboxes.cpp"],
	deps = [
		"//die-tk:die-tk-base",
	],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
	],
)

cc_binary(
	name = "paintboxes-win32",
	deps = [
		":paintboxes",
		"//die-tk:die-tk-win32",
	],
	copts = [
		"--std=c++1y",
	],
	linkopts = [
		"-lgdi32",
	],
)

cc_binary(
	name = "paintboxes-linux",
	deps = [
		":paintboxes",
		"//die-tk:die-tk-linux",
		"@system_libs//:x11",
	],
	copts = [
		"--std=c++1y",
	],
)
