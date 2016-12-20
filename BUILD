cc_library(
	name = "die-tk-base",
	srcs = glob(["src/*.cpp"]),
	hdrs = glob(["src/*.h"]),
	copts = ["--std=c++1y"],
	linkstatic = 1,
)

cc_library(
	name = "die-tk-win32",
	srcs = glob(["win32/*.cpp"]),
	hdrs = glob(["win32/*.h"]),
	deps = [":die-tk-base"],
	copts = ["--std=c++1y"],
	linkstatic = 1,
)
