#include "ControlPlane/World.hpp"
#include "ControlPlane/Avdecc.hpp"

namespace ControlPlane
{
std::string avdeccDescriptorTypeName[] = {"entity",
                                          "configuration",
                                          "audio_unit",
                                          "video_unit",
                                          "sensor_unit",
                                          "stream_input",
                                          "stream_output",
                                          "jack_input",
                                          "jack_output",
                                          "avb_interface",
                                          "clock_source",
                                          "memory_object",
                                          "locale",
                                          "strings",
                                          "stream_port_input",
                                          "stream_port_output",
                                          "external_port_input",
                                          "external_port_output",
                                          "internal_port_input",
                                          "internal_port_output",
                                          "audio_cluster",
                                          "video_cluster",
                                          "sensor_cluster",
                                          "audio_map",
                                          "video_map",
                                          "sensor_map",
                                          "control",
                                          "signal_selector",
                                          "mixer",
                                          "matrix",
                                          "matrix_signal",
                                          "signal_splitter",
                                          "signal_combiner",
                                          "signal_demultiplexer",
                                          "signal_multiplexer",
                                          "signal_transcoder",
                                          "clock_domain",
                                          "control_block"};
}
