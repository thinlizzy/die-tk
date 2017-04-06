cc_library(
	name = "events-lib",
	srcs = glob([
		"main.cpp",
	]),
	deps = [
		"//:die-tk-base",
		"//examples/events/model",
		"//examples/events/view",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "events",
	deps = [
		":events-lib",
		"//external:die-tk",
	],
	copts = [
		"--std=c++1y",
	],
)
