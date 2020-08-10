project(echo_srv)
set(CMAKE_CXX_STANDARD 11)
add_executable(echo_srv echo_service.c error.c)

# client
add_executable(echo_cli echo_client.c str_cli.c)
