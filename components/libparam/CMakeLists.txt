#cmake_minimum_required(VERSION 3.16)
#
##if(${CONF_LIST_DYNAMIC} OR (${CONF_LIST_POOL}>0))
#if(${CONF_LIST_DYNAMIC})
#    add_compile_definitions(PARAM_HAVE_SYS_QUEUE)
#endif()
#
#add_compile_definitions(PARAM_LIST_DYNAMIC=${CONF_LIST_DYNAMIC})
#add_compile_definitions(PARAM_LIST_POOL=${CONF_LIST_POOL})
#add_compile_definitions(PARAM_HAVE_SCHEDULER=${CONF_SCHEDULER})
#add_compile_definitions(PARAM_HAVE_COMMANDS=${CONF_COMMANDS})
#add_compile_definitions(PARAM_VMEM_64_BITS_API)
#add_compile_definitions(MPACK_FLOAT=${CONF_HAVE_FLOAT})
#add_compile_definitions(MPACK_STDIO=${CONF_HAVE_FOPEN})
#add_compile_definitions(MPACK_STDIO=${CONF_HAVE_FOPEN})
#
#if(${CONF_HAVE_FOPEN})
#    if(${CONF_LIST_DYNAMIC})
#        add_compile_definitions(MPACK_STDIO)
#    else()
#        message("Enabling have_fopen without also enabling list_dynamic causes mpack related build failures, setting MPACK_STDIO=0")
#        add_compile_definitions(MPACK_STDIO=0)
#    endif()
#endif()
#
#
set(srcs
    "src/param/list/param_list.c"
    "src/param/param_client.c"
    "src/param/param_serializer.c"
    "src/param/param_server.c"
    #"src/param/param_string.c"
    "src/param/param_queue.c"
    "src/param/param_wildcard.c"
    "src/param/param.c"
    "src/mpack/mpack.c"
    "src/vmem/vmem_client.c"
    "src/vmem/vmem_server.c"
    "src/vmem/vmem.c"
    "src/vmem/vmem_block.c"
    "src/objstore/objstore.c"
)

#if(${CONF_HAVE_FOPEN})
#    list(APPEND srcs
#        "src/vmem/vmem_file.c"
#        "src/vmem/vmem_mmap.c"
#    )
#endif()
#
#if(${CONF_SCHEDULER})
#    list(APPEND srcs
#        "src/param/scheduler/param_scheduler.c"
#    )
#endif()
#
#if(${CONF_SCHEDULER_CLIENT})
#    list(APPEND srcs
#        "src/param/scheduler/param_scheduler_client.c"
#    )
#endif()
#
#if(${CONF_COMMANDS})
#    list(APPEND srcs
#        "src/param/commands/param_commands.c"
#    )
#endif()
#
#if(${CONF_COMMANDS_CLIENT})
#    list(APPEND srcs
#        "src/param/commands/param_commands_client.c"
#    )
#endif()
#
#if(${CONF_VMEM_FRAM})
#    list(APPEND srcs
#        "src/vmem/vmem_fram.c"
#        "src/vmem/vmem_fram_cache.c"
#    )
#endif()
#
#
#if(${CONF_COLLECTOR})
#    list(APPEND srcs
#        "src/param/collector/param_collector_config.c"
#        "src/param/collector/param_collector.c"
#    )
#endif()
#
idf_component_register(SRCS ${srcs} INCLUDE_DIRS "include" REQUIRES libcsp)
