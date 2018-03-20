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
	path = "/c/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64",
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

# bindings for multiplatform

# if linux then

# bind(
#     name = "die-tk",
#     actual = "//:die-tk-linux",
# )

# bind(
#     name = "die-tk-image",
#     actual = "//die-tk-image:die-tk-image-linux",
# )

# bind(
#     name = "window_system",
#     actual = "@linux_libs//:x11",
# )

# else if win32 then

bind(
    name = "die-tk",
    actual = "//:die-tk-win32",
)

bind(
    name = "die-tk-image",
    actual = "//die-tk-image:die-tk-image-win32",
)

bind(
    name = "window_system",
    actual = "@win32_libs//:win32",
)

# end if

# make libimage visible for die-tk-image

# git_repository(
#     name = "libimage",
#     remote = "https://github.com/thinlizzy/libimage",
#     commit = "bde81940c6449712c82500f5292c1a94634fd8d3",
# )

local_repository(
    name = "libimage",
    path = "../libimage",
)

# remove after https://github.com/bazelbuild/bazel/issues/2757 is fixed

# linux

new_local_repository(
	name = "linux_freeimage",
	path = "/usr/lib",
	build_file_content = """
cc_library(
	name = "freeimage",
	srcs = ["libfreeimage.a"],
	visibility = ["//visibility:public"],
)
""",
)

# win32

new_local_repository(
	name = "win32_freeimage",
	path = "/d/diego/progs/c++/FreeImage/x64",
	build_file_content = """
cc_library(
	name = "freeimage",
	srcs = ["FreeImage.lib"],
	hdrs = ["include/FreeImage.h"],
	strip_include_prefix = "include",
	visibility = ["//visibility:public"],
)
""",
)

# bindings for multiplatform

# if linux then

# bind(
#     name = "freeimage",
#     actual = "@linux_freeimage//:freeimage",
# )

# else if win32 then

bind(
    name = "freeimage",
    actual = "@win32_freeimage//:freeimage",
)

# end if
