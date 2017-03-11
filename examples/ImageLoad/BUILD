filegroup(
	name = "images",
	data = glob([
		"*.png",
		"*.jpg",
	]),
)

cc_library(
	name = "imageLoad",
	srcs = ["imageLoad.cpp"],
	deps = [
		"//die-tk:die-tk-base",
		"//die-tk/die-tk-image:die-tk-image-base",
	],
	copts = [
		"--std=c++1y",
		"-Idie-tk/src",
		"-Idie-tk/die-tk-image/src",
	],
)

cc_binary(
	name = "imageLoad-win32",
	data = [":images"],
	deps = [
		":imageLoad",
		"//die-tk:die-tk-win32",
		"//die-tk/die-tk-image:die-tk-image-win32",
		"@win32_libs//:gdi32",
		"@win32_user_libs//:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
)

cc_binary(
	name = "imageLoad-linux",
	data = [":images"],
	deps = [
		":imageLoad",
		"//die-tk:die-tk-linux",
		"//die-tk/die-tk-image:die-tk-image-linux",
		"@linux_libs//:x11",
		"@linux_libs//:freeimage",
	],
	copts = [
		"--std=c++1y",
	],
)
