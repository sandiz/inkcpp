add_executable(inkcpp_test catch.hpp Main.cpp 
    Array.cpp 
    Pointer.cpp 
    Stack.cpp
    Tree.cpp
    )

target_link_libraries(inkcpp_test PUBLIC inkcpp)