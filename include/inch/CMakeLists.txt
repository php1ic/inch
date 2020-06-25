# Locate all of the header files so IDEs can find them
# Keep list alphabetical
set(HEADERS
  chart.hpp
  chartColour.hpp
  chartSelection.hpp
  chartType.hpp
  converter.hpp
  dripline.hpp
  fileType.hpp
  grid.hpp
  isotopeDisplayMode.hpp
  io.hpp
  key.hpp
  limits.hpp
  magicNumbers.hpp
  massTable.hpp
  nuclide.hpp
  options.hpp
  partition.hpp
  prolog.hpp
  rProcess.hpp
  ui.hpp
  version.hpp
  )


configure_file(
  "version.hpp.in"
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/version.hpp"
  @ONLY
  )
