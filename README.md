bitseal
===

The most secure method of Bitcoin address creation known to man

performs key-stretching via specified number of sha256
rounds on a given input key

inherently has the ability to protect Bitcoin private keys in 
the same way a timed safe operates.  by choosing a very 
high number of rounds a private key may take considerable 
time to retrieve.  for example: 2 trillion rounds will take
approximately 2 weeks to perform on an L9300 Core 2 Duo CPU.  
given that the key-stretching technique in use is 
single-threaded it is unlikely that a GPU or other highly
parallelizable device will provide significant 
performance enhancements.

===

to build; gcc, openssl libraries, and the make command are to be installed

on debian/ubuntu these packages may be installed via:

`sudo apt-get install git gcc libssl-dev make`

download the code:

`git clone https://github.com/campassi/bitseal.git`

edit bitseal.c and change the parameters to fit your liking
when complete, save and run make:

`make`

and finally run the code:

`./bitseal`
