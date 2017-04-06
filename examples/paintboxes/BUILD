cc_library(
	name = "paintboxes-lib",
	srcs = ["paintboxes.cpp"],
	deps = [
		"//:die-tk-base",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "paintboxes",
	deps = [
		":paintboxes-lib",
		"//external:die-tk",
	],
	copts = [
		"--std=c++1y",
	],
)
