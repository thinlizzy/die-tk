filegroup(
	name = "images",
	data = glob([
		"*.png",
		"*.jpg",
	]),
)

cc_library(
	name = "imageload-lib",
	srcs = ["imageLoad.cpp"],
	deps = [
		"//:die-tk-base",
		"//die-tk-image:die-tk-image-base",
	],
	copts = ["-DBAZEL_FOR_WINDOWS"],
)

cc_binary(
	name = "ImageLoad",
	data = [":images"],
	deps = [
		":imageload-lib",
		"//die-tk-image",
		"//:die-tk",
	],
)
