filegroup(
	name = "images",
	data = glob([
		"*.png",
	]),
)

cc_binary(
	name = "ImageScaling",
	srcs = ["imageScaling.cpp"],
	deps = [
		"//die-tk-image",
		"//:die-tk",
	],
	copts = select({
		"@bazel_tools//src/conditions:windows": ["-DBAZEL_FOR_WINDOWS"],
		"//conditions:default": [],
	}),
	data = [":images"],
)
