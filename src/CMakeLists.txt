include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(main main.cpp blockchain.cpp blockchain_type.cpp base58.cpp)
target_link_libraries(main ${PROJECT_SOURCE_DIR}/include/cryptopp/lib/libcryptopp.a)