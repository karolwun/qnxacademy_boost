cc_binary(
    name = 'parse_args',
    srcs = ['parse_args.cc'],    
    linkopts = ["-lboost_program_options"]
)

cc_binary(
    name = 'logging',
    srcs = ['logging.cc'],    
    linkopts = ["-lboost_log", "-lpthread"],
    copts = ["-DBOOST_LOG_DYN_LINK"]
)