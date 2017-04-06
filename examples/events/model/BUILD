cc_library(
    name = "model",
	srcs = glob([
		"*.cpp",
	]),
	hdrs = glob([
		"*.h",
	]),
    deps = [
        "//:die-tk-base",
    ],
	copts = [
		"--std=c++1y",
	],
	visibility = ["//examples/events:__subpackages__"],
)
