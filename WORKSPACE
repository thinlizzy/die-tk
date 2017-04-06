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
	path = "/c/mingw-dists/mingw-w64/x86_64-6.2.0-release-posix-seh-rt_v5-rev1/mingw64/x86_64-w64-mingw32/lib",
	build_file_content = """
cc_library(
	name = "gdi32",
	srcs = ["libgdi32.a"],
	visibility = ["//visibility:public"],
)
""",
)

# bindings for multiplatform

# if linux then

bind(
    name = "die-tk",
    actual = "//:die-tk-linux",
)

bind(
    name = "die-tk-image",
    actual = "//die-tk-image:die-tk-image-linux",
)

bind(
    name = "window_system",
    actual = "@linux_libs//:x11",
)

# else if win32 then

# bind(
#     name = "die-tk",
#     actual = "//:die-tk-win32",
# )

# bind(
#     name = "die-tk-image",
#     actual = "//die-tk-image:die-tk-image-win32",
# )

# bind(
#     name = "window_system",
#     actual = "@win32_libs//:gdi32",
# )

# end if

# make libimage visible for die-tk-image

git_repository(
    name = "libimage",
    remote = "https://github.com/thinlizzy/libimage",
    commit = "bde81940c6449712c82500f5292c1a94634fd8d3",
)

# local_repository(
#     name = "libimage",
#     path = "../libimage",
# )
