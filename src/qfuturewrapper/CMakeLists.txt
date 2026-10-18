include(Install)

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

InstallConfig(qfuturewrapper.hpp qfuturewrapper.ipp)