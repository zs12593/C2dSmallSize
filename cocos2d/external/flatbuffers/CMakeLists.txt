
set(FLATBUFFERS_SRC
    ${EXTERNAL_DIR}/flatbuffers/flatc.cpp
    ${EXTERNAL_DIR}/flatbuffers/idl_gen_cpp.cpp
    ${EXTERNAL_DIR}/flatbuffers/idl_gen_fbs.cpp
    ${EXTERNAL_DIR}/flatbuffers/idl_gen_general.cpp
    ${EXTERNAL_DIR}/flatbuffers/idl_gen_go.cpp
    ${EXTERNAL_DIR}/flatbuffers/idl_gen_text.cpp
    ${EXTERNAL_DIR}/flatbuffers/idl_parser.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)
