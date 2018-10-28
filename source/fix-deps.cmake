if(${TARGET} STREQUAL "BBC_MICROBIT_CLASSIC_GCC")
  # There seems to be a bug in the build that means that this gets left out for Max7219 module
  # include_directories("../../../yotta_modules/mbed-classic/targets/hal/TARGET_NORDIC/TARET_MCU_NRF51822/TARGET_NRF51_MICROBIT")
  target_include_directories(microbit-max7219-display-driver PRIVATE "../../../yotta_modules/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_NRF51_MICROBIT")
  target_include_directories(microbit-max7219-display-driver PRIVATE "../../../yotta_modules/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/")
  target_include_directories(microbit-max7219-display-driver PRIVATE "../../../yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/")
endif()
