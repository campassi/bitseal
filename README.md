bitseal
===

performs key-stretching via specified number of sha256
rounds on a given key

inherently has the ability to protect private keys in 
the same way a timed safe operates.  by choosing a very 
high number of rounds a private key may take considerable 
time to retrieve.  for example: 2 trillion rounds will take
approximately 2 weeks to perform on an L9300 Core 2 Duo CPU.  
given that the key-stretching technique in use is 
single-threaded it is unlikely that a GPU will provide significant 
performance enhancements.

===

to build; gcc, openssl libraries, and the make command are to be installed

on debian these packages may be installed via:

`apt-get install gcc libssl-dev make`

the resulting executable will be named 'bitseal'
