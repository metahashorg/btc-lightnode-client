set(BOOST_LIBS_INCLUDE ../3rd_party/boost/include)
set(BOOST_LIBS_LIBS ../3rd_party/boost/lib)
find_library(BOOST1 NAMES libboost_filesystem.a HINTS ${BOOST_LIBS_LIBS})
find_library(BOOST2 NAMES libboost_program_options.a HINTS ${BOOST_LIBS_LIBS})
find_library(BOOST3 NAMES libboost_system.a HINTS ${BOOST_LIBS_LIBS})

add_executable(${PROJECT_NAME} /
    main.cpp
    exception/except.cpp
    settings/settings.cpp
    http_server.cpp
    http_json_rpc_request.cpp
    json_rpc.cpp
    http_session.cpp
    log/log.cpp

    btctx/Base58.cpp
    btctx/btctx.cpp
    btctx/wif.cpp

    ethtx/crossguid/Guid.cpp
    ethtx/scrypt/crypto_scrypt-nosse.cpp
    ethtx/scrypt/crypto_scrypt_saltgen.cpp
    ethtx/scrypt/sha256.cpp
    ethtx/cert.cpp
    ethtx/cert2.cpp
    ethtx/ethtx.cpp
    ethtx/rlp.cpp
    ethtx/utils2.cpp

    btc_wallet/BtcWallet.cpp
    btc_wallet/FileSystem.cpp
    btc_wallet/TypedException.cpp
    btc_wallet/jsonUtils.cpp

    task_handlers/utils.cpp
    task_handlers/base_handler.cpp
    task_handlers/network_handler.cpp
    task_handlers/generate_handler.cpp
    task_handlers/create_tx_base_handler.cpp
    task_handlers/create_tx_handler.cpp
    task_handlers/send_tx_handler.cpp
    task_handlers/fetch_balance_handler.cpp
    task_handlers/fetch_history_handler.cpp
    task_handlers/get_tx_handler.cpp
    task_handlers/get_dump_block_by_hash_handler.cpp
    task_handlers/get_dump_block_by_number_handler.cpp
    task_handlers/get_blocks_handler.cpp
    task_handlers/get_block_by_number_handler.cpp
    task_handlers/get_block_by_hash_handler.cpp
    task_handlers/get_last_txs_handler.cpp
    task_handlers/get_count_blocks_handler.cpp
    task_handlers/task_handlers.cpp
    )

set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(
    ./
    ../3rd_party
    ../3rd_party/p7logger/include
    ../3rd_party/gmp
    #${Boost_LIBRARY_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    )

#include_directories(${PROJECT_NAME} ../3rd_party/leveldb/lib)
#find_library(LIB_LEVEL_DB leveldb HINTS ../3rd_party/leveldb/lib)
#target_link_libraries(${PROJECT_NAME} ${LIB_LEVEL_DB})

find_package(Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

#find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
target_link_libraries(${PROJECT_NAME} ${BOOST1} ${BOOST2} ${BOOST3})

#link_directories(${PROJECT_NAME} ${Boost_LIBRARY_DIRS})
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
#target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
include_directories(${OPENSSL_INCLUDE_DIR})

find_library(LIB_CRYPTO NAMES libcrypto.so HINTS /usr/local/lib/)
find_library(LIB_SSL NAMES libssl.so HINTS /usr/local/lib/)
find_library(LIB_LEVELDB NAMES libleveldb.a HINTS ../3rd_party/leveldb/lib)
find_library(LIB_P7 NAMES libP7.a HINTS ../3rd_party/p7logger/lib)

find_library(LIB_GMP NAMES libgmp.a HINTS /../3rd_party/gmp/lib)
find_library(LIB_GMPXX NAMES libgmpxx.a HINTS ../3rd_party/gmp/lib)

target_link_libraries(${PROJECT_NAME} ${LIB_CRYPTO} ${LIB_SSL} ${LIB_LEVELDB} ${LIB_P7} ${LIB_GMP} ${LIB_GMPXX})

#include_directories(../3rd_party/leveldb/include/)

set (CMAKE_CXX_FLAGS "-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -pthread -Ofast -Wall -Wextra -Werror -Wno-unused-parameter -Wfloat-equal")

target_compile_options(${PROJECT_NAME} PRIVATE -no-pie -D_GLIBCXX_USE_CXX11_ABI=0)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-no-pie -D_GLIBCXX_USE_CXX11_ABI=0")
#set (CMAKE_CXX_FLAGS "-g -rdynamic")

find_library(SECP secp256k1 HINTS ../3rd_party/secp256k1/lib/linux/)
target_link_libraries(${PROJECT_NAME} ${SECP})

set(PROJECT_LIBS
    stdc++fs.a
    uuid
    ssl
    crypto
    cryptopp
    dl
    rt
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})

#find_package(Threads)
#target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

#find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
#link_directories(${PROJECT_NAME} ${Boost_LIBRARY_DIRS})
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

#find_package(OpenSSL REQUIRED)
#target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})

#set(PROJECT_LIBS
#    boost_program_options
#    boost_system
#    stdc++fs.a
#    uuid
#    ssl
#    crypto
#    cryptopp
#    secp256k1
#    gmp
#    gmpxx
#)

#target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})
