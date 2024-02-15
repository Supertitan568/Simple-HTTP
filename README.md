# Simple HTTP

This is a simple http webserver for linux written in C.

This binds to port 8080 and sends any files within the /var/website directory

By default this sends index.html

This is not meant to be used for production

## Commands to get this program started:
```shell
make
sudo make install
sudo iptables -t nat -I PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports 8080
sudo systemctl start simple_http_server
```

## Planned Future Updates:

Going through and changing out less secure functions for more secure ones

Adding more MIME types

Creating more Http headers

etc...

## Resources I used to create this:
https://github.com/JeffreytheCoder/Simple-HTTP-Server/tree/master

https://bhch.github.io/posts/2017/11/writing-an-http-server-from-scratch/

https://developer.mozilla.org/en-US/docs/Web/HTTP

