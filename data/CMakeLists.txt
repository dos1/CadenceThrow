SET(DATADIR "${SHARE_INSTALL_PREFIX}/cadencethrow/data")

if(APPLE)
  SET(DATADIR "${BIN_INSTALL_DIR}/cadencethrow.app/Contents/Resources/data") #OMG NASTY
endif(APPLE)

#add_subdirectory(icons)
#if(UNIX AND NOT APPLE)
#  install(FILES cadencethrow.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
#endif(UNIX AND NOT APPLE)
#install(DIRECTORY about DESTINATION ${DATADIR})
#install(DIRECTORY intro DESTINATION ${DATADIR})
#install(DIRECTORY levels DESTINATION ${DATADIR})
#install(DIRECTORY map DESTINATION ${DATADIR})
#install(DIRECTORY menu DESTINATION ${DATADIR})
#install(DIRECTORY voice DESTINATION ${DATADIR})
#install(DIRECTORY fonts DESTINATION ${DATADIR})
#install(FILES loading.png DESTINATION ${DATADIR})
#install(FILES table.png DESTINATION ${DATADIR})
