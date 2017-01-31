cc_library(
    name = "events",
	srcs = glob([
		"main.cpp",
	]),
    deps = [
        "//die-tk:die-tk-base",
        "//die-tk/examples/events/model",
        "//die-tk/examples/events/view",
    ],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
	],
)

cc_binary(
    name = "events-win32",
    deps = [
    	":events",
        "//die-tk:die-tk-win32",
    ],
	copts = [
		"--std=c++1y",
	],
    linkopts = [
    	"-lgdi32",
    ],
)

cc_binary(
    name = "events-linux",
    deps = [
    	":events",
        "//die-tk:die-tk-linux",
    ],
	copts = [
		"--std=c++1y",
	],
    linkopts = [
    	"-lX11",
    ],
)
