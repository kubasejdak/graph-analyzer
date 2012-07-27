#=== MODULES ===

# BinaryInput
set(BINARY_INPUT_SRC
	AbstractInput.cpp
	BinaryInput.cpp)

add_library(binary_input ${BINARY_INPUT_SRC})

# PcapInput
set(PCAP_INPUT_SRC
	AbstractInput.cpp
	PcapInput.cpp)

add_library(pcap_input ${PCAP_INPUT_SRC})