SET(DATADIR "${SHARE_INSTALL_PREFIX}/cadencethrow/data")

if(APPLE)
  SET(DATADIR "${BIN_INSTALL_DIR}/cadencethrow.app/Contents/Resources/data") #OMG NASTY
endif(APPLE)

add_subdirectory(icons)
#if(UNIX AND NOT APPLE)
#  install(FILES cadencethrow.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
#endif(UNIX AND NOT APPLE)
#install(DIRECTORY about DESTINATION ${DATADIR})
#install(DIRECTORY intro DESTINATION ${DATADIR})
#install(DIRECTORY levels DESTINATION ${DATADIR})
#install(DIRECTORY map DESTINATION ${DATADIR})
install(DIRECTORY menu DESTINATION ${DATADIR})
#install(DIRECTORY voice DESTINATION ${DATADIR})
install(DIRECTORY fonts DESTINATION ${DATADIR})
#install(FILES loading.png DESTINATION ${DATADIR})
install(FILES cadence.png
crash.flac
fail.flac
intro.avi
loading1.png
loading3.png
lost.flac
prepare.avi
space.jpg
success.flac
throw.flac
fail.avi
fail.png
intro.flac
loading2.png
loading4.png
prepare.flac
success.avi
throw.avi
win.flac DESTINATION ${DATADIR})
