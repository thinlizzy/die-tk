cc_library(
	name = "two-windows-lib",
	srcs = ["two-windows.cpp"],
	deps = [
		"//:die-tk-base",
	],
)

cc_binary(
	name = "two-windows",
	deps = [
		":two-windows-lib",
		"//:die-tk",
	],
)
