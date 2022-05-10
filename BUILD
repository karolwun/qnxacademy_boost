cc_binary(
    name = 'parse_args',
    srcs = ['parse_args.cc'],    
    linkopts = ["-lboost_program_options"]
)

cc_binary(
    name = 'logging',
    srcs = ['logging.cc'],    
    linkopts = ["-lboost_log_setup", "-lboost_log", "-lboost_thread", "-lpthread"],
    copts = ["-DBOOST_LOG_DYN_LINK"]
)

cc_binary(
    name = "lexical_cast",
    srcs = ["lexical_cast.cc"]
)

cc_binary(
    name = "ipc",
    srcs = ["ipc.cc"],
    linkopts = ["-lpthread", "-lrt"]
)

cc_binary(
    name = 'cpp17',
    srcs = ["cpp17.cc"],
    linkopts = ["-lboost_filesystem"]
)