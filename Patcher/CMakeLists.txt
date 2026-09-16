add_executable(Patcher SafeMain.cpp)
set_target_properties(Patcher PROPERTIES OUTPUT_NAME TBCPatcher)
target_link_libraries(Patcher PRIVATE Version.lib)
set_property(TARGET Patcher PROPERTY CXX_STANDARD 17)
