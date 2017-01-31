cc_library(
    name = "model",
	srcs = glob([
		"*.cpp",
	]),
	hdrs = glob([
		"*.h",
	]),
    deps = [
        "//die-tk:die-tk-base",
    ],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
	],
	visibility = ["//die-tk/examples/events:__subpackages__"],
)
