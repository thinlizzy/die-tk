# linux

new_local_repository(
	name = "linux_libs",
	path = "/usr/lib",
	build_file_content = """
cc_library(
	name = "x11",
	# pkg-config --variable=libdir x11
	srcs = ["x86_64-linux-gnu/libX11.so"],
	visibility = ["//visibility:public"],
)
""",
)

# win32

new_local_repository(
	name = "win32_libs",
	path = "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64",
#	path = "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64",
	build_file_content = """
cc_library(
	name = "win32",
	srcs = [
		"Gdi32.lib",
		"User32.lib",
	],
	visibility = ["//visibility:public"],
)
""",
)

# make libimage visible for die-tk-image

# git_repository(
#     name = "libimage",
#     remote = "https://github.com/thinlizzy/libimage",
#     branch = "master",
# )

local_repository(
    name = "libimage",
    path = "../libimage",
)
