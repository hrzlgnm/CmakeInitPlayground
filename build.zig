const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const exe = b.addExecutable(.{
        .name = "gol",
        .optimize = optimize,
        .target = target,
    });
    const exe_cflags = [_][]const u8{
        "-std=c++17",
        "-fvisibility-inlines-hidden",
        "-Werror=type-limits",
        "-Wno-missing-braces",
        "-Wno-comment",
        "-Wshadow=compatible-local",
        "-Werror=return-type",
    };
    exe.addCSourceFile("source/main.cpp", &exe_cflags);
    exe.linkLibCpp();
    exe.linkSystemLibraryName("c++");

    b.installArtifact(exe);
}
