cc_library(
    name = "view",
	srcs = glob([
		"*.cpp",
	]),
	hdrs = glob([
		"*.h",
	]),
    deps = [
        "//:die-tk-base",
        "//examples/events/model",
    ],
	copts = [
		"--std=c++1y",
	],
	visibility = ["//examples/events:__subpackages__"],
)
