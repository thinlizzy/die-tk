package(default_visibility = ["//visibility:public"])

cc_library(
	name = "die-tk-dialogs-base",
	hdrs = [
		"Dialogs.h",
		"SelectFileParams.h",
	],
	deps = [
		"//:die-tk-base",
	],
)

cc_library(
	name = "die-tk-dialogs-win32",
	srcs = [
		"win32/Dialogs.cpp",
	],
	deps = [
		":die-tk-dialogs-base",
		"//:die-tk-win32",
		"@win32_libs//:comdlg32",
	],
)

cc_library(
	name = "die-tk-dialogs-linux",
	srcs = [
		"linux/Dialogs.cpp",
		"linux/libsofd.c",
		"linux/libsofd.h",
		"linux/messagebox.c",
		"linux/messagebox.h",
	],
	deps = [
		":die-tk-dialogs-base",
		"//:die-tk-linux",
		"@linux_libs//:x11",
	],
)

cc_library(
	name = "die-tk-dialogs",
	deps = select({
		"@bazel_tools//src/conditions:windows": [":die-tk-dialogs-win32"],
		"//conditions:default": [":die-tk-dialogs-linux"],
	}),
)
