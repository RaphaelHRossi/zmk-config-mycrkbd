if(CONFIG_SHIELD_EVA_OLED AND CONFIG_ZMK_DISPLAY)
  target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
  target_sources(app PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/eva_render.c
    ${CMAKE_CURRENT_LIST_DIR}/src/eva_screen.c
  )
endif()
