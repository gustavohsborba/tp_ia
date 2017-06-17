cmake_minimum_required(VERSION 3.7)
project(tp_final)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        magic.cpp magic.h
        Gui.cpp Gui.h
        mymath.h
        NeuralNetwork.cpp NeuralNetwork.h
        RigidBody2D.cpp RigidBody2D.h
        Cor.h
        main.cpp NeuralTrainer.cpp NeuralTrainer.h)

add_executable(tp_final ${SOURCE_FILES})
target_link_libraries(tp_final GL GLU glut)