# For install utils

function (InstallConfig header_name imp_name)
# Create QFutureWrapper in the install include directory
install(CODE [[
    file(WRITE "${CMAKE_INSTALL_PREFIX}/include/QFutureWrapper"
        "#include \"qfuturewrapper.hpp\"\n"
    )
]])

# Install the compiled library
install(TARGETS qfuturewrapper
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

# Install the public header
install(FILES
        qfuturewrapper.hpp
        qfuturewrapper.ipp
        DESTINATION include
)
endfunction()

