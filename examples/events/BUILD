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
)

cc_binary(
	name = "events",
	deps = [
		":events-lib",
		"//:die-tk",
	],
)
