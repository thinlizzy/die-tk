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
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "ImageLoad",
	data = [":images"],
	deps = [
		":imageload-lib",
		"//external:die-tk-image",
		"//external:die-tk",
	],
	copts = [
		"--std=c++1y",
	],
)
