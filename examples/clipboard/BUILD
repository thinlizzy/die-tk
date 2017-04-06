cc_library(
	name = "clipboard-lib",
	srcs = ["clipboard.cpp"],
	deps = [
		"//:die-tk-base",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "clipboard",
	deps = [
		":clipboard-lib",
		"//external:die-tk",
	],
	copts = [
		"--std=c++1y",
	],
)
