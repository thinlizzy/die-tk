cc_library(
	name = "clipboard-lib",
	srcs = ["clipboard.cpp"],
	deps = [
		"//:die-tk-base",
	],
)

cc_binary(
	name = "clipboard",
	deps = [
		":clipboard-lib",
		"//:die-tk",
	],
)
