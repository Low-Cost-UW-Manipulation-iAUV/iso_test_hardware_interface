iso_test_hardware_interface
===========================
This is a hardware interface for testing a ros_control effort interface controller.

It includes an IIR filter based simulation which is fed with coefficients via the parameter server:

<code>
ros_control_iso:
  simulation: # Parameters for the versatile_dynamics simulation which is based on an IIR filter 
    x:
      coeff_b: [ 0, 0.0222499707936119, 0.02213899878580774] #Numerator
      coeff_a: [ 1, -1.985111939603063, 0.9851119396030626] #Denominator
      vector_size: 3  #the length of the above coefficients (BTW both vectors need to be of same size)
      simulation_rate: 200 #Simulation Rate in Hz
    y:
      coeff_b: [ 0, 0.0222499707936119, 0.02213899878580774] #Numerator
      coeff_a: [ 1, -1.985111939603063, 0.9851119396030626] #Denominator
      vector_size: 3  #the length of the above coefficients (BTW both vectors need to be of same size)
      simulation_rate: 200 #Simulation Rate in Hz
</code>

These parameters can be generated in Matlab by doing the following:

create a continuous transfer function to encode your behaviour

  <code>system = tf([],[]); </code>

turn it into a discrete transfer function:

  <code>digisystem = c2d(system, sample_period) </code>

get the coeffs out of there, in vector format:

  <code>tfdata(digisystem, 'v'); </code>

