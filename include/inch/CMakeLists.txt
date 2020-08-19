# Locate all of the header files so IDEs can find them
# Keep list alphabetical
set(HEADERS
  chart.hpp
  chartColour.hpp
  chartSelection.hpp
  chartType.hpp
  converter.hpp
  dripline.hpp
  eps_chart.hpp
  eps_dripline.hpp
  eps_grid.hpp
  eps_magicNumbers.hpp
  eps_rProcess.hpp
  eps_key.hpp
  fileType.hpp
  grid.hpp
  io.hpp
  isotopeDisplayMode.hpp
  key.hpp
  limits.hpp
  magicNumbers.hpp
  massTable.hpp
  nuclide.hpp
  options.hpp
  partition.hpp
  rProcess.hpp
  svg_chart.hpp
  tikz_chart.hpp
  ui.hpp
  version.hpp
  )


configure_file(
  "version.hpp.in"
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/version.hpp"
  @ONLY
  )
