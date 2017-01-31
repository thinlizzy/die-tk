cc_library(
    name = "view",
	srcs = glob([
		"*.cpp",
	]),
	hdrs = glob([
		"*.h",
	]),
    deps = [
        "//die-tk:die-tk-base",
        "//die-tk/examples/events/model",
    ],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
	],
	visibility = ["//die-tk/examples/events:__subpackages__"],
)
