cc_library(
	name = "paintboxes-lib",
	srcs = ["paintboxes.cpp"],
	deps = [
		"//:die-tk-base",
	],
)

cc_binary(
	name = "paintboxes",
	deps = [
		":paintboxes-lib",
		"//:die-tk",
	],
)
