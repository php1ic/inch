#include "inch/key.hpp"

#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <algorithm>


void Key::setScale(const Options& draw, const Partition& part) const
{
  if (!draw.key)
    {
      scale = 0.0;
      return;
    }

  // Set the key height by checking how many partition types are used
  std::for_each(part.values.cbegin(), part.values.cend(), [&](const auto val) {
    height += (val.draw) ? single_partition_height : 0;
  });

  // We don't want the key to shrink below a certain size.
  scale = (draw.limits.getZRange() > KEY_YOFFSET) ? draw.limits.getZRange() / height : KEY_YOFFSET / height;

  // Nor do we want it to be larger than a certain size.
  if (scale > max_scale || draw.chart_selection == ChartSelection::FULL_CHART
      || draw.limits.getZRange() == Limits::MAX_Z)
    {
      scale = max_scale;
    }
}
