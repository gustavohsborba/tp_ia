cmake_minimum_required(VERSION 3.7)
project(tp_final)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/game/magic.cpp src/game/magic.h
        src/game/Gui.cpp src/game/Gui.h
        src/engine/Vector.h
        src/neural/NeuralNetwork.cpp src/neural/NeuralNetwork.h
        src/engine/RigidBody2D.cpp src/engine/RigidBody2D.h
        src/engine/Cor.h
        src/main.cpp src/neural/NeuralTrainer.cpp src/neural/NeuralTrainer.h src/engine/Vector.cpp src/neural/NeuralNetworkLayer.cpp src/neural/NeuralNetworkLayer.h)

add_executable(tp_final ${SOURCE_FILES})
target_link_libraries(tp_final GL GLU glut)