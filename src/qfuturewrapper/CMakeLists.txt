qt_add_library(
        qfuturewrapper SHARED

        qfuturewrapper.hpp
        qfuturewrapper.ipp
)

set_target_properties(qfuturewrapper PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
        qfuturewrapper PRIVATE

        Qt6::Core
)

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