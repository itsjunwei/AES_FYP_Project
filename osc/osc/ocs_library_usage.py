OSC_IP_ADDRESS = '192.168.0.196'
CHANNEL = 'C1'

lecroy_if = lecroy_interface.Lecroy(ip_address=OSC_IP_ADDRESS)
lecroy_if.prepare_for_trace_capture()
lecroy_if.wait_lecroy()

##### End of the setup ####

##### Collect one trace ####
_, interpreted_format = lecroy_if.get_native_signal_float(CHANNEL)
thisArray = numpy.asarray(interpreted_format[:])
