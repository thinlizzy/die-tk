cc_library(
	name = "clipboard",
	srcs = ["clipboard.cpp"],
	deps = [
		"//die-tk:die-tk-base",
	],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
	],
)

cc_binary(
	name = "clipboard-win32",
	deps = [
		":clipboard",
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
	name = "clipboard-linux",
	deps = [
		":clipboard",
		"//die-tk:die-tk-linux",
		"@linux_libs//:x11",
	],
	copts = [
		"--std=c++1y",
	],
)
