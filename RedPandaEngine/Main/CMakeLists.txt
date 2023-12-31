project("RedPandaEngine")

add_executable(${PROJECT_NAME} "RedPandaEngine.cpp" "../Graphics/Graphics.cpp" "../Physics/Physics.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC "../")

#glm
find_package(glm CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

#glew
find_package(glew CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC GLEW::GLEW)

#glfw
find_package(glfw3 CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

#imgui
find_package(imgui CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)

#plog
find_package(plog CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${PLOG_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC plog::plog)
