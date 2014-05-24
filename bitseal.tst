:::::::::::::::::::::::::::
::deep SHA256 round testing
bitseal 64-bit Debian 7 vps
passwd: 'Satoshi Nakamoto'
sharnd: '0x93a4bfec9','39632764617'
calctm: '17674 secs'
prvkey: '26f8f2f71b246d21bf292d724648ff00452e32060c0ed8b7811f3817886d04c2'
prvb58: '5J7T5dUirMX2LLJVoVy3gxxVBUFNrifK7hRaUbwvK89QxeAvNj5'
pubb58: '1Dx6nfHg6YrvMUsK6o8gdaQEoPk1poAAKT'

passwd: 'rabbit*&^%'                                               
sharnd: '39632764617','0x93a4bfec9'
calctm: '16823 secs'
prvkey: '1076379daaafae6933a3070570c3b0c9895401c4556f8c078e89cd1f8846d7b1'
prvb58: '5HwY5M3p3hrmA53jJcJEBsURnR7PZqJs55bVABj4aHS74ejmU3N'
pubb58: '12aQkUbrmvmxVHokZYVMuE7jCYACxpoKnz'

bitseal 32-bit Debian 7 vps
passwd_: 'Satoshi Nakamoto'
sha_rnd: '0x93a4bfec9','39632764617'
calctm_: '27511 secs'
privkey: '26f8f2f71b246d21bf292d724648ff00452e32060c0ed8b7811f3817886d04c2'
privb58: '5J7T5dUirMX2LLJVoVy3gxxVBUFNrifK7hRaUbwvK89QxeAvNj5'
pub_b58: '1Dx6nfHg6YrvMUsK6o8gdaQEoPk1poAAKT'

passwd_: 'rabbit*&^%'
sha_rnd: '0x93a4bfec9','39632764617'
calctm_: '27590 secs'
privkey: '1076379daaafae6933a3070570c3b0c9895401c4556f8c078e89cd1f8846d7b1'
privb58: '5HwY5M3p3hrmA53jJcJEBsURnR7PZqJs55bVABj4aHS74ejmU3N'
pub_b58: '12aQkUbrmvmxVHokZYVMuE7jCYACxpoKnz'

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::short repetitive testing to check for out of bounds errors
::the public b58 is most complex so test it
::any breakage before it will be reflected here
while true; do ./bitseal | grep pubb58 | cut -d "'" -f 2 >> output; done
grep -v 1Z2A5K7WxYpYLCSMXKKeiNSvamS7C34UQ output

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::simple checking across multiple platforms with short rounds
bitseal 64-bit Debian 7 phys
passwd: 'Satoshi Nakamoto'                                                
sharnd: '10067967','0x999fff'
calctm: '7 secs'
prvkey: '4640eeb02392e618749f5abe6de9bf199a6c82f894c1919e26ce5ad87c65c38f'
prvb58: '5JME7iBPhFJ7SatrqoXrh86yD5Xtf54T8rEfFmEQLtz5t8L5UJD'
pubb58: '14S5b16e4hmYbytxNe7y53v8zjEQRDx8Ru'

bitseal 64-bit Debian 7 vps
passwd: 'Satoshi Nakamoto'                                                  
sharnd: '10067967','0x999fff'
calctm: '4 secs'
prvkey: '4640eeb02392e618749f5abe6de9bf199a6c82f894c1919e26ce5ad87c65c38f'
prvb58: '5JME7iBPhFJ7SatrqoXrh86yD5Xtf54T8rEfFmEQLtz5t8L5UJD'
pubb58: '14S5b16e4hmYbytxNe7y53v8zjEQRDx8Ru'

bitseal 32-bit Debian 7 vps
passwd: 'Satoshi Nakamoto'                                                
sharnd: '10067967','0x999fff'
calctm: '15 secs'
prvkey: '4640eeb02392e618749f5abe6de9bf199a6c82f894c1919e26ce5ad87c65c38f'
prvb58: '5JME7iBPhFJ7SatrqoXrh86yD5Xtf54T8rEfFmEQLtz5t8L5UJD'
pubb58: '14S5b16e4hmYbytxNe7y53v8zjEQRDx8Ru'

:::::::::::::::::::::::::::::::
::check with alternate programs
c=$(echo -n "Satoshi Nakamoto" | sha256sum | awk '{print $1}'); echo -n 0x$c | xxd -r -c 64 | sha256sum
21ee5734569831d26b398831435f01a137102322ea4fceff2cd5f0ae99080727

passwd: 'Satoshi Nakamoto'                                                
sharnd: '2','0x2'
calctm: '0 secs'
prvkey: '21ee5734569831d26b398831435f01a137102322ea4fceff2cd5f0ae99080727'
prvb58: '5J5EJaKzoB7TjjRzyJDGpPhiDo9jQevCKpXYjHZq4wekMYG7Pjt'
pubb58: '1CNRGf4Xcvjozp7hdJ3ucWUAndWtsCV4de

#bash will start tripping up after lots of rounds since it doesn't
#handle binary that well.
