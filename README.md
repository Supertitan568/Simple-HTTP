Simple HTTP

This is a simple http webserver written in C.

This binds to port 80 and sends any files within the bin directory

By default this sends index.html

This is not meant to be used for production

Commands to get this program started:
make
cd bin
sudo ./simple_http_server

Planned Future Updates:
Going through and changing out less secure functions for more secure ones
Letting this program bind to another port besides port 80 so it is not run as root
Adding more MIME types
Daemonizing this program to let it run in the background
Creating more Http headers
etc...

Resources I used to create this:
https://github.com/JeffreytheCoder/Simple-HTTP-Server/tree/master
https://bhch.github.io/posts/2017/11/writing-an-http-server-from-scratch/
https://developer.mozilla.org/en-US/docs/Web/HTTP

